using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using CsGL.OpenGL;
using System.Reflection;

namespace MotorsEditor
{
    public partial class Form1 : Form
    {
        string GameDirectory = "";
        bool heightmapEditor_MouseDown = false;
        Graphics heightmapEditor_Graph;
        Pen heightmapEditor_pen;
        private OpenGLView view;
        private BrushSelector selector;
        private Noise noiseDialog = new Noise();

        private Dictionary<string, Image> brushes = new Dictionary<string, Image>();
        private Image currentBrush;
        private Graphics tTex_graph;
        private bool tTex_mouseDown = false;
        private Point tTex_lastMousePos = new Point(0, 0);
        private ImageAttributes ia = new ImageAttributes();
        private ColorMatrix cm = new ColorMatrix();


        private void timer1_Tick(object sender, EventArgs e)
        {
            view.Refresh();
        }  

        public Form1()
        {
            InitializeComponent();

            view = new OpenGLView();
            
            tabControl2.TabPages[2].Controls.Add(view);
            view.Dock = DockStyle.Fill;
            timer1.Enabled = false;
            selector = new BrushSelector();


            string currentElement="";
            string value="";
            try
            {
                XmlTextReader reader = new XmlTextReader("mru.xml");
                if (reader != null)
                {
                    while (reader.Read())
                    {
                        switch (reader.NodeType)
                        {
                        case XmlNodeType.Element:
                            currentElement = reader.Name.ToUpper();
                            value = "";
                            break;
                        case XmlNodeType.Text:
                            switch (currentElement)
                            {
                                case "MRU":
                                    value += reader.Value;
                                    break;
                                default:
                                    MessageBox.Show(currentElement);
                                    break;
                            }
                            break;
                        case XmlNodeType.EndElement:
                            if (MRUListBox.Items.IndexOf(value) < 0)
                            {
                                MRUListBox.Items.Add(value);
                            }
                            break;
                        }
                    }
                    reader.Close();
                }
            }
            catch (FileNotFoundException)
            {
                // create a new empty one..
            }

            terrainTexture.Image = new Bitmap(1024, 1024, PixelFormat.Format32bppPArgb);
            tTex_graph = Graphics.FromImage(terrainTexture.Image);
            cm.Matrix33 = 0.5f;
            ia.SetColorMatrix(cm);

            noiseDialog.outputBox = HeightmapEditor;
            HeightmapEditor.Image = new Bitmap(512, 512);
            UpdateBrush();
            newToolStripButton_Click(this, null);
        }

        private void UpdateBrush()
        {
            heightmapEditor_Graph = Graphics.FromImage(HeightmapEditor.Image);
            heightmapEditor_pen = new Pen(selector.brushColor, selector.brushSize);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        public void WriteMRUFile()
        {
            if (GameDirectory != "")
            {
                MRUListBox.Items.Add(GameDirectory);
            }

            XmlTextWriter writer = new XmlTextWriter("mru.xml",Encoding.ASCII);
            writer.WriteStartElement("Config");
            for (int i = 0; i < MRUListBox.Items.Count; i++)
            {
                writer.WriteStartElement("MRU");
                writer.WriteValue(MRUListBox.Items[i]);
                writer.WriteEndElement();
            }
            writer.WriteEndElement();
            writer.Close();
        }

        private void OnClosing(object sender, FormClosingEventArgs e)
        {
            WriteMRUFile();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            LoadGameDir(MRUListBox.Items[MRUListBox.SelectedIndex].ToString());
            tabControl1.SelectTab(1);
        }
        private void MRUListBox_DoubleClick(object sender, EventArgs e)
        {
            LoadGameDir(MRUListBox.Items[MRUListBox.SelectedIndex].ToString());
            tabControl1.SelectTab(1);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            FolderBrowserDialog fbd = new FolderBrowserDialog();
            fbd.Description = "Please Select the root game directory";
            if (fbd.ShowDialog() == DialogResult.OK)
            {
                LoadGameDir(fbd.SelectedPath);
                tabControl1.SelectTab(1);
            }
        }

        private void LoadGameDir(string dir)
        {
            GameDirectory = dir;
            loadConfigFile();
            DirectoryInfo di = new DirectoryInfo(GameDirectory + "\\data\\Topographical");
            FileInfo[] rgFiles = di.GetFiles("*.top");

            treeView3.Nodes[0].Nodes.Clear();
            foreach (FileInfo fi in rgFiles)
            {
                treeView3.Nodes[0].Nodes.Add(fi.Name);
            }

            DirectoryInfo mapDirectory = new DirectoryInfo(GameDirectory + "\\data\\maps");
            FileInfo[] savFiles = mapDirectory.GetFiles("*.sav");

            foreach (FileInfo savFile in savFiles)
            {
                ToolStripMenuItem tsi = new ToolStripMenuItem();
                tsi.Text = savFile.Name;
                tsi.Click += new EventHandler(tsi_Click);
                toolStripDropDownButton3.DropDownItems.Add(tsi);
            }


        }

        void tsi_Click(object sender, EventArgs e)
        {
            LoadSaveFile(GameDirectory + "\\data\\maps\\" + ((ToolStripItem)sender).Text.ToString());
        }

        void loadConfigFile()
        {
            ConfigGrid.Rows.Clear();
            StreamReader configFile;
            try
            {
                configFile = new StreamReader(GameDirectory + "/config.cfg");
            }
            catch (FileNotFoundException)
            {
                MessageBox.Show("Hmm... couldn't open the config file in that directory... are you sure thats the right directory?");
                return;
            }
            if (configFile.Peek() == -1)
            {
                MessageBox.Show("Couldn't open config file!");
                return;
            }

            while (configFile.Peek() != -1)
            {
                string line = configFile.ReadLine();

                int eqLoc = line.IndexOf('=');
                int quLoc = line.IndexOf('"');

                if (eqLoc < 0 || quLoc < 0) continue;

                string[] newRow = { line.Substring(0, eqLoc), line.Substring(quLoc + 1, line.Length - quLoc - 2) };

                ConfigGrid.Rows.Add(newRow);
            }

            configFile.Close();

            ConfigGrid.AutoResizeColumns(DataGridViewAutoSizeColumnsMode.AllCells);
        }

        void saveConfigFile(string filename)
        {
            ConfigGrid.CommitEdit(DataGridViewDataErrorContexts.Commit);

            int rowCount = ConfigGrid.Rows.Count;
            StreamWriter configFile = new StreamWriter(filename);
            if (configFile.BaseStream.CanWrite != true)
            {
                MessageBox.Show("Couldn't open config.cfg for writing :(");
                return;
            }
            string varName, varValue, line;
            for (int i = 0; i < rowCount; i++)
            {
                if (ConfigGrid.Rows[i].Cells[0].Value == null) continue;

                varName = ConfigGrid.Rows[i].Cells[0].Value.ToString();
                varValue = ConfigGrid.Rows[i].Cells[1].Value.ToString();

                line = varName + "=\"" + varValue + "\"";
                configFile.WriteLine(line);
            }

            configFile.Close();
        }

        private void openToolStripButton1_Click(object sender, EventArgs e)
        {
            loadConfigFile();
        }

        private void saveToolStripButton1_Click(object sender, EventArgs e)
        {
            saveConfigFile(GameDirectory + "/config.cfg");
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.FileName = "config.cfg";
            if (sfd.ShowDialog() == DialogResult.OK) 
            {
                saveConfigFile(sfd.FileName);
            }
        }

        int lastX = 0, lastY=0;
        private void HeightmapEditor_onMouseDown(object sender, MouseEventArgs e)
        {
            heightmapEditor_MouseDown = true;
            lastX = e.X;
            lastY = e.Y;
            heightmapEditor_Graph.FillEllipse(heightmapEditor_pen.Brush, new Rectangle(e.X - (int)(heightmapEditor_pen.Width / 2), e.Y - (int)(heightmapEditor_pen.Width / 2), (int)heightmapEditor_pen.Width, (int)heightmapEditor_pen.Width));
            HeightmapEditor.Refresh();
        }

        private void HeightmapEditor_onMouseMove(object sender, MouseEventArgs e)
        {
            if (heightmapEditor_MouseDown)
            {
                // draw here

                heightmapEditor_Graph.DrawLine(heightmapEditor_pen, e.X, e.Y, lastX, lastY);
                heightmapEditor_Graph.FillEllipse(heightmapEditor_pen.Brush, new Rectangle(e.X - (int)(heightmapEditor_pen.Width / 2), e.Y - (int)(heightmapEditor_pen.Width / 2), (int)heightmapEditor_pen.Width, (int)heightmapEditor_pen.Width));

                lastX = e.X;
                lastY = e.Y;

                HeightmapEditor.Refresh();
            }
        }

        private void HeightmapEditor_onMouseUp(object sender, MouseEventArgs e)
        {
            heightmapEditor_MouseDown = false;
        }


        private void HeightmapEditor_SelectBrush_Click(object sender, EventArgs e)
        {

        }
/*
        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(heightmapEditor_pen.Color, int.Parse(numericUpDown1.Value.ToString()));
        }
*/


        private void ChooseBrushButton_Click(object sender, EventArgs e)
        {
            selector.ShowDialog();
            UpdateBrush();
        }


        private void PreviewButton_Click(object sender, EventArgs e)
        {
            Bitmap bmp = new Bitmap(HeightmapEditor.Image);
            view.terrain.FromBitmap(bmp);
            tabControl2.SelectTab(2);
        }

        private void waterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25,0,0,0), heightmapEditor_pen.Width);
        }

        private void beachToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25,50,50,50), heightmapEditor_pen.Width);

        }

        private void ground1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25, 100, 100, 100), heightmapEditor_pen.Width);
        }

        private void ground2ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25, 150, 150, 150), heightmapEditor_pen.Width);
        }

        private void ground3ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25, 200, 200, 200), heightmapEditor_pen.Width);
        }

        private void newToolStripButton_Click(object sender, EventArgs e)
        {
            heightmapEditor_Graph.FillRectangle(Brushes.Black, new Rectangle(0, 0, 512, 512));
            HeightmapEditor.Refresh();
        }

        private void saveTerrainButton_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.DefaultExt = ".top";
            sfd.FileName = "terrain.top";
            if (sfd.ShowDialog() == DialogResult.OK)
            {
                saveTerrain(sfd.FileName);
            }
        }

        private void saveTerrain(string filename)
        {
            Bitmap temp = new Bitmap(HeightmapEditor.Image);
            view.terrain.FromBitmap(temp);

            Stream stream = new FileStream(filename, FileMode.Create, FileAccess.Write, FileShare.Write); 
            BinaryWriter writer = new BinaryWriter(stream);
            writer.Write(view.terrain.GetData());
            writer.Close();
        }

        private void loadTerrain(string filename)
        {
            view.terrain.LoadRawFile(filename);
            Bitmap hbmp = new Bitmap(view.terrain.MAP_SIZE,view.terrain.MAP_SIZE);
            for (int y=0;y<view.terrain.MAP_SIZE;y++) 
            {
                for (int x=0;x<view.terrain.MAP_SIZE;x++) 
                {
                   int height = view.terrain.rawHeight(x,y);
                   hbmp.SetPixel(x, y, Color.FromArgb(height, height, height));
                }
            }
            HeightmapEditor.Image = hbmp;
            HeightmapEditor.Refresh();
            UpdateBrush();
        }

        private void openToolStripButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                loadTerrain(ofd.FileName);
            }
        }

        private void toolStripButton2_Click(object sender, EventArgs e)
        {

        }

        private void toolStripButton3_Click(object sender, EventArgs e)
        {

        }
                
        public struct MapHeader {
	        public string magic;	// ASGE07
	        public short size_x, size_y;
            public string terrainName;
	        public int entityCount;
        };
        
        public struct MapEntity {
	        public int type;	
	        public float x,y;
        };

        private void openToolStripButton2_Click(object sender, EventArgs e)
        {
        
            OpenFileDialog ofd = new OpenFileDialog();
            if (ofd.ShowDialog() == DialogResult.OK)
            {
                LoadSaveFile(ofd.FileName);
            }
        }

        public static int LittleEndianBytesToInt(byte [] src)
        {            
            int result;
            if (src.Length != 4) return -666;

            result = ((int)src[3] << 24) +
                     ((int)src[2] << 16) +
                     ((int)src[1] << 8) +
                     ((int)src[0]);
            return (int)result;
        }

        private void LoadSaveFile(string savefile)
        {
            treeView2.TopNode.Nodes.Clear();
            FileStream stream = new FileStream(savefile, FileMode.Open, FileAccess.Read, FileShare.Read);
            BinaryReader reader = new BinaryReader(stream);
            MapHeader header;

            header.magic = new string(reader.ReadChars(6));
            header.size_x = reader.ReadInt16();
            header.size_y = reader.ReadInt16();
            header.terrainName = new string(reader.ReadChars(32));
            reader.ReadInt16();
            header.entityCount = LittleEndianBytesToInt(reader.ReadBytes(4));

            for (int i = 0; i < header.entityCount; i++)
            {
                MapEntity ent;

                try
                {
                    ent.type = LittleEndianBytesToInt(reader.ReadBytes(4));

                    ent.x = reader.ReadSingle();
                    ent.y = reader.ReadSingle();
                }
                catch (EndOfStreamException)
                {
                    break;
                }
                string typename = "";
                switch (ent.type)
                {
                    case 0:
                        typename = "tree";
                        break;
                    case 1:
                        typename = "tower";
                        break;
                    case 2:
                        typename = "farm";
                        break;
                    case 3:
                        typename = "peasant";
                        break;
                    case 4:
                        typename = "mill";
                        break;
                    case 5:
                        typename = "sheep";
                        break;
                    case 6:
                        typename = "lamp";
                        break;
                    case 8:
                        typename = "woman";
                        break;
                    case 9:
                        typename = "soldier";
                        break;
                    case 10:
                        typename = "trigger";
                        break;
                    case 11:
                        typename = "dock";
                        break;
                    case 12:
                        typename = "ship";
                        break;
                    case 13:
                        typename = "barracks";
                        break;
                    case 14:
                        typename = "waypoint";
                        break;
                    default:
                        typename = "(unknown:" + ent.type + ")";
                        break;
                }

                treeView2.TopNode.Nodes.Add(typename);
                
            }
        }

        private void treeview3_pickHeightmap(object sender, TreeViewEventArgs e)
        {
            loadTerrain(GameDirectory + "\\data\\Topographical\\" + treeView3.SelectedNode.Text.ToString());
        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void addBrushTypeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();
            if (ofd.ShowDialog() == DialogResult.OK) 
            {
                brushes.Add(ofd.FileName, new Bitmap(ofd.FileName.ToString()));
                ToolStripMenuItem item = new ToolStripMenuItem();
                item.Text = ofd.FileName;
                item.Click += new EventHandler(textureSelectBrush);
                item.Image = brushes[ofd.FileName];
                toolStripDropDownButton1.DropDownItems.Add(item);
                currentBrush = brushes[ofd.FileName];
            }
        }

        void textureSelectBrush(object sender, EventArgs e)
        {
            currentBrush = brushes[((ToolStripItem)sender).Text];
        }


        private void terrainTexture_MouseDown(object sender, MouseEventArgs e)
        {
            tTex_mouseDown = true;
            terrainTexture_Draw(e.X, e.Y);
            terrainTexture.Refresh();
            tTex_lastMousePos = e.Location;
        }


        private void terrainTexture_MouseMove(object sender, MouseEventArgs e)
        {
            if (tTex_mouseDown)
            {
                int xDelta = (e.X - tTex_lastMousePos.X);
                int yDelta = (e.Y - tTex_lastMousePos.Y);
                int drawDistance = (int)Math.Sqrt((xDelta * xDelta) + (yDelta * yDelta));
                
                for (int i = 0; i < 1+(drawDistance/20); i++)
                {
                    float lerp = (float)i / (1 + (drawDistance / 20));
                    float x = tTex_lastMousePos.X + xDelta * lerp;
                    float y = tTex_lastMousePos.Y + yDelta * lerp;
                    terrainTexture_Draw((int)x, (int)y);
                }
                tTex_lastMousePos = e.Location;      
                terrainTexture.Refresh();
            }

        }

        private void terrainTexture_MouseUp(object sender, MouseEventArgs e)
        {
            tTex_mouseDown = false;
        }

        private void terrainTexture_Draw(int x, int y)
        {
            int brushSize = 40;
            int halfBrushSize = 20;
            if (currentBrush != null)
            {
                int i = (x - halfBrushSize) % currentBrush.Width;
                int u = (y - halfBrushSize) % currentBrush.Height;
                if (i + brushSize > currentBrush.Width)
                {
                    tTex_graph.DrawImage(currentBrush, new Rectangle(x - halfBrushSize, y - halfBrushSize, brushSize, brushSize), i - currentBrush.Width, u, brushSize, brushSize, GraphicsUnit.Pixel, ia);
                }
                if (u + brushSize > currentBrush.Height)
                {
                    tTex_graph.DrawImage(currentBrush, new Rectangle(x - halfBrushSize, y - halfBrushSize, brushSize, brushSize), i, u - currentBrush.Height, brushSize, brushSize, GraphicsUnit.Pixel, ia);
                }

                tTex_graph.DrawImage(currentBrush, new Rectangle(x - halfBrushSize, y - halfBrushSize, brushSize, brushSize), i, u, brushSize, brushSize, GraphicsUnit.Pixel, ia);
            }
        }

        private void toolStripButton5_Click(object sender, EventArgs e)
        {
            if (view.terrain.texture > 0)
            {
                uint[] tex = new uint[1];
                tex[0] = view.terrain.texture;
                GL.glDeleteTextures(1, tex);
            }
            view.terrain.texture = view.terrain.LoadTexture((Bitmap)terrainTexture.Image);
            tabControl2.SelectTab(2);
        }

        private void toolStripButton6_Click(object sender, EventArgs e)
        {
            if (currentBrush != null)
            {
                tTex_graph.DrawImage(currentBrush,new Point(0,0));
            }
        }

        private void openNoiseDialogButton_Click(object sender, EventArgs e)
        {
            noiseDialog.Show();
        }

        private void tabControl2_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void GenerateFromHeightmap_Click(object sender, EventArgs e)
        {
            if (MessageBox.Show("Are you sure you want to generate a texture from the heightmap?\nYou will loose your current texture if you haven't saved.", "Discard Current Texture?", MessageBoxButtons.YesNoCancel) == DialogResult.Yes)
            {
                int lastHeight = 0;
                int heightDelta = 255 / brushes.Count;
                foreach (Image brush in brushes.Values)
                {
                    genBrushFromHeightmap((Bitmap)HeightmapEditor.Image, brush, lastHeight, lastHeight + heightDelta);
                    lastHeight += heightDelta;
                }
            }
        }

        private void genBrushFromHeightmap(Bitmap source, Image brush, int heightRange_bottom, int heightRange_top)
        {
            Graphics graph = Graphics.FromImage(terrainTexture.Image);
            Pen pen = new Pen(Color.Black, 1);
            int scaleFactorX = terrainTexture.Image.Width / source.Width;
            int scaleFactorY = terrainTexture.Image.Height / source.Height;
            int rc = 0;
            int srcHeight = 0;
            int x, y;
            Rectangle r1 = new Rectangle(), r2 = new Rectangle();

            BitmapData sourceData = source.LockBits(new Rectangle(0, 0, source.Width, source.Height), ImageLockMode.ReadOnly, PixelFormat.Format32bppPArgb);
            int step = 5;
            unsafe
            {
                for (y = 0; y < source.Height; y+=step)
                {
                    for (x = 0; x < source.Width; x+=step)
                    {
                        int* sourcePixel = (int*)((int*)sourceData.Scan0 + (y * sourceData.Width) + x);
                        srcHeight = Color.FromArgb(*sourcePixel).R;

                        if (srcHeight <= heightRange_top && srcHeight >= heightRange_bottom)
                        {
                            r1.X = (x * scaleFactorX); r1.Y = (y * scaleFactorY);
                            r1.Width = 1 * scaleFactorX; r1.Height = 1 * scaleFactorY;

                            r2.X = (x * scaleFactorX) % brush.Width; r2.Y = (y * scaleFactorY) % brush.Height;
                            r2.Width = 1 * scaleFactorX; r2.Height = 1 * scaleFactorY;

                            graph.DrawImage(brush, r1, r2, GraphicsUnit.Pixel);
                        }
                    }
              //**      if (rc++ > 25)
                    {
                        terrainTexture.Refresh();
                        rc = 0;
                    }
                }
            }
            source.UnlockBits(sourceData);
        }

    }

}
