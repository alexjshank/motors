using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using System.IO;

namespace MotorsEditor
{
    public partial class Form1 : Form
    {
        string GameDirectory = "";
        bool heightmapEditor_MouseDown = false;
        Graphics heightmapEditor_Graph;
        Pen heightmapEditor_pen;

        public Form1()
        {
            InitializeComponent();

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
        }

        private void HeightmapEditor_onMouseMove(object sender, MouseEventArgs e)
        {
            if (heightmapEditor_MouseDown)
            {
                // draw here


                heightmapEditor_Graph.DrawLine(heightmapEditor_pen, e.X, e.Y, lastX, lastY);
                
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

   
    }
}
