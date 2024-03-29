using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MotorsEditor
{
    public partial class BrushSelector : Form
    {
        public int brushSize = 10;
        public int brushType = 0;
        public Color brushColor = Color.Black;
        private Graphics graph;
        private Pen pen;

        public BrushSelector()
        {
            InitializeComponent();
            BrushPreview.Image = new Bitmap(75, 75);
            graph = Graphics.FromImage(BrushPreview.Image);
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {
            brushSize = int.Parse(brushSizePicker.Value.ToString());
            UpdatePreviewWindow();
        }

        private void brushColorPicker_ValueChanged(object sender, EventArgs e)
        {
            brushColor = Color.FromArgb(255, (int)brushColorPicker.Value, (int)brushColorPicker.Value, (int)brushColorPicker.Value);
        }

        private void UpdatePreviewWindow()
        {
            graph.Clear(Color.White);
            pen = new Pen(brushColor, brushSize);
            switch (brushType) {
                case 0:
                    graph.FillEllipse(pen.Brush, new Rectangle(0, 0, brushSize, brushSize));
                    break;

                case 1:
                    graph.FillRectangle(pen.Brush, new Rectangle(0, 0, brushSize, brushSize));
                    break;

                case 2:
                    graph.FillEllipse(pen.Brush, new Rectangle(0, 0, brushSize, brushSize));
                    break;
            }
            BrushPreview.Refresh();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (comboBox1.SelectedItem == null) return;
            switch (comboBox1.SelectedText)
            {
                case "Square":
                    brushType = 0;
                    break;
                case "Round":
                    brushType = 1;
                    break;
                case "Airbrush":
                    brushType = 2;
                    break;
            }
            UpdatePreviewWindow();
        }

        private void textBox1_TextChanged_1(object sender, EventArgs e)
        {
            int color = (int)brushColorPicker.Value;
            brushColor = Color.FromArgb(255,color,color,color);
            UpdatePreviewWindow();
        }

        private void BrushSelector_Load(object sender, EventArgs e)
        {
            brushColorPicker.Value = brushColor.R;
            brushSizePicker.Value = brushSize;
        }

    }
}