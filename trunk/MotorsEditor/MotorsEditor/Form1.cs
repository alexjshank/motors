using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;
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
            catch (FileNotFoundException e)
            {
                // create a new empty one..
            }

            HeightmapEditor.Image = new Bitmap(512, 512);
            heightmapEditor_Graph = Graphics.FromImage(HeightmapEditor.Image);
            heightmapEditor_pen = new Pen(Color.FromArgb(10, Color.Black), 25);
            newToolStripButton_Click(this, null);
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
            BrushSelector selector = new BrushSelector();
            selector.brushColor = heightmapEditor_pen.Color;
            selector.brushSize = (int)heightmapEditor_pen.Width;
            selector.ShowDialog();
            heightmapEditor_pen = new Pen(selector.brushColor, selector.brushSize);
        }

        private void PreviewButton_Click(object sender, EventArgs e)
        {
            Bitmap bmp = new Bitmap(HeightmapEditor.Image);
            view.terrain.FromBitmap(bmp);
            tabControl2.SelectTab(1);
        }

        private void waterToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25,0,0,0), heightmapEditor_pen.Width);
        }

        private void beachToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25,30,30,30), heightmapEditor_pen.Width);

        }

        private void ground1ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25, 60, 60, 60), heightmapEditor_pen.Width);
        }

        private void ground2ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25, 90, 90, 90), heightmapEditor_pen.Width);
        }

        private void ground3ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            heightmapEditor_pen = new Pen(Color.FromArgb(25, 120, 120, 120), heightmapEditor_pen.Width);
        }

        private void newToolStripButton_Click(object sender, EventArgs e)
        {
            heightmapEditor_Graph.FillRectangle(Brushes.Black, new Rectangle(0, 0, 512, 512));
        }

        private void saveTerrainButton_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();
            sfd.DefaultExt = ".top";
            sfd.FileName = "terrain.top";
            if (sfd.ShowDialog() == DialogResult.OK)
            {

            }
        }





 
    }

}
