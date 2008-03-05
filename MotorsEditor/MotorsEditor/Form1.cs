using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
using System.Threading;
using CsGL.OpenGL;

namespace MotorsEditor
{
    public partial class Form1 : Form
    {
        string GameDirectory = "";
        bool heightmapEditor_MouseDown = false;
        Graphics heightmapEditor_Graph;
        Pen heightmapEditor_pen;
        private OpenGLView view;

        private void timer1_Tick(object sender, EventArgs e)
        {
            view.Refresh();
        }  

        public Form1()
        {
            InitializeComponent();

            view = new OpenGLView();
            tabControl2.TabPages[1].Controls.Add(view);
            view.Dock = DockStyle.Fill;
            timer1.Enabled = true;


            string currentElement="";
            string value="";
            XmlTextReader reader = new XmlTextReader("mru.xml");
            if (reader != null)
            {
                try
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
                catch (FileNotFoundException e)
                {
                    // create a new empty one..
                }
            }


            HeightmapEditor.Image = new Bitmap(512, 512);
            heightmapEditor_Graph = Graphics.FromImage(HeightmapEditor.Image);
            heightmapEditor_pen = new Pen(Color.FromArgb(10, Color.Black), 25);
            heightmapEditor_Graph.FillRectangle(Brushes.Black, new Rectangle(0, 0, 512, 512));
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

        private void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {

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
        }

        void loadConfigFile()
        {
            ConfigGrid.Rows.Clear();
            StreamReader configFile;
            try
            {
                configFile = new StreamReader(GameDirectory + "/config.cfg");
            }
            catch (FileNotFoundException e)
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

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
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

        private void ChooseColorButton1_Click(object sender, EventArgs e)
        {
            ColorDialog colorDialog1 = new ColorDialog();
            colorDialog1.Color = heightmapEditor_pen.Color;
            if (colorDialog1.ShowDialog() == DialogResult.OK)
            {
                heightmapEditor_pen = new Pen(colorDialog1.Color, heightmapEditor_pen.Width);
            }
        }

        private void HeightmapEditor_SelectBrush_Click(object sender, EventArgs e)
        {

        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(heightmapEditor_pen.Color, int.Parse(numericUpDown1.Value.ToString()));
        }

        private void openGLControl1_Click(object sender, EventArgs e)
        {
        }

 
    }

    class Vector
    {
        float x, y, z;
    }

    class OpenGLView : OpenGLControl
    {
        Terrain terrain = new Terrain();
        float angle = 0;
        // This function overides the glDraw function found in the CsGL namespace
        // So that you may customise your viewport settings etc...
        public override void glDraw()
        {
            GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
            GL.glLoadIdentity();

            GL.gluLookAt(terrain.MAP_SIZE / 2, 250, -100, terrain.MAP_SIZE / 2, 0, terrain.MAP_SIZE / 2, 0, 1, 0);
            GL.glScalef(1, 0.25f, 1);
            terrain.RenderHeightmap();
        }

        // This function sets up the OpenGL context as normal, but note the GL prefix's!
        protected override void InitGLContext()
        {
            GL.glShadeModel(GL.GL_SMOOTH);
            GL.glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
            GL.glClearDepth(1.0f);
            GL.glEnable(GL.GL_DEPTH_TEST);
            GL.glDepthFunc(GL.GL_LEQUAL);
            GL.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);

            terrain.LoadRawFile("c:/height.raw", 512*512);
        }
        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);
            Size s = Size;
            double aspect_ratio = (double)s.Width / (double)s.Height;
            GL.glMatrixMode(GL.GL_PROJECTION); // Select The Projection Matrix
            GL.glLoadIdentity(); // Reset The Projection Matrix
            // Calculate The Aspect Ratio Of The Window
            GL.gluPerspective(45.0f, aspect_ratio, 0.1f, 1000.0f);
            GL.glMatrixMode(GL.GL_MODELVIEW); // Select The Modelview Matrix
            GL.glLoadIdentity();// Reset The Modelview Matrix
        }

  
    }

    public class Terrain
    {
        public int MAP_SIZE = 512;
        protected int STEP_SIZE = 2;
        private float HEIGHT_RATIO = 1.5f;
        private static bool render = true;
        private byte[] heightmap;
        private static float scaleValue = 0.15f;


        public int Height(int X, int Y)
        {
            int x = X % MAP_SIZE;
            int y = Y % MAP_SIZE;

            if (heightmap == null)
            {
                return 0;
            }
            int index = x + (y * MAP_SIZE);
            if (index > 0 && index < MAP_SIZE * MAP_SIZE)
                return heightmap[index];
            else return 0;
        }

        public void LoadRawFile(string filename, int size)
        {

            FileStream stream = null;
            ASCIIEncoding encoding = new ASCIIEncoding();
            BinaryReader reader = null;

            try
            {
                // Open The File
                stream = new FileStream(filename, FileMode.Open, FileAccess.Read, FileShare.Read);
                reader = new BinaryReader(stream, encoding);
                MAP_SIZE = (int)Math.Sqrt((double)stream.Length);
          
                heightmap = new byte[MAP_SIZE * MAP_SIZE];
                heightmap = reader.ReadBytes(MAP_SIZE * MAP_SIZE);

            }
            catch (Exception e)
            {
                // Handle Any Exceptions While Loading Terrain Data, Exit App
                string errorMsg = "An Error Occurred While Loading And Parsing World Data:\n\t" + filename + "\n" + "\n\nStack Trace:\n\t" + e.StackTrace + "\n";
                MessageBox.Show(errorMsg, "Error", MessageBoxButtons.OK, MessageBoxIcon.Stop);
            }
            finally
            {
                if (reader != null)
                {
                    reader.Close();
                }
                if (stream != null)
                {
                    stream.Close();
                }
            }
        }

        public void RenderHeightmap()
        {
            int X, Y;
            int x, y, z;

            if (heightmap == null)
            {
                return;
            }

            if (render)
            {
                GL.glBegin(GL.GL_QUADS);
            }
            else
            {
                GL.glBegin(GL.GL_LINES);
            }

            for (X = 0; X < MAP_SIZE; X += STEP_SIZE)
                for (Y = 0; Y < MAP_SIZE; Y += STEP_SIZE)
                {
                    // Get The (X, Y, Z) Value For The Bottom Left Vertex
                    x = X;
                    y = Height(X, Y);
                    z = Y;
                    SetVertexColor(x, z);
                    GL.glVertex3i(x, y, z);

                    // Get The (X, Y, Z) Value For The Top Left Vertex
                    x = X;
                    y = Height(X, Y + STEP_SIZE);
                    z = Y + STEP_SIZE;
                    SetVertexColor(x, z);
                    GL.glVertex3i(x, y, z);

                    // Get The (X, Y, Z) Value For The Top Right Vertex
                    x = X + STEP_SIZE;
                    y = Height(X + STEP_SIZE, Y + STEP_SIZE);
                    z = Y + STEP_SIZE;
                    SetVertexColor(x, z);
                    GL.glVertex3i(x, y, z);

                    // Get The (X, Y, Z) Value For The Bottom Right Vertex
                    x = X + STEP_SIZE;
                    y = Height(X + STEP_SIZE, Y);
                    z = Y;
                    SetVertexColor(x, z);
                    GL.glVertex3i(x, y, z);
                }
            GL.glEnd();
            GL.glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }

        private void SetVertexColor(int x, int y)
        {
            if (heightmap == null)
            {
                return;
            }

            float fColor = -0.15f + (Height(x, y) / 256.0f);

            GL.glColor3f(0.0f, 0.0f, fColor);
        }
    }
}
