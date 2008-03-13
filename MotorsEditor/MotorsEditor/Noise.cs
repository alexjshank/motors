using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MotorsEditor
{
    public partial class Noise : Form
    {
        public PictureBox outputBox;
        Random r = new Random();

        public Noise()
        {
            InitializeComponent();
        }

        private int rand(int x)
        {
            int y = -x * 1789234;
            int v = (int)(x ^ y) + (y << (x % 16));
            if (v < 0) return -v;
            return v;
        }

        private int r2(int s, int x, int y)
        {
            return rand(s + x + (y * outputBox.Image.Width)) % 255;
        }

        private void GenerateNoise(int size, int seed, float persistence)
        {
            seed *= 98234;
            Graphics graph = Graphics.FromImage(outputBox.Image);
            Pen pen = new Pen(Color.Black,size);
            int p = (int)(255 * persistence);

            for (int y = 0; y < outputBox.Image.Height; y += size)
            {
                for (int x = 0; x < outputBox.Image.Width; x += size)
                {
                    if (size > 1)
                    {
                        Point[] ptGraph = { new Point(x, y), new Point(x + size, y), new Point(x + size, y + size), new Point(x, y + size) };
                        System.Drawing.Drawing2D.PathGradientBrush pgb = new System.Drawing.Drawing2D.PathGradientBrush(ptGraph);

                        int v1 = r2(seed, x, y);
                        int v2 = r2(seed, x + size, y);
                        int v3 = r2(seed, x + size, y + size);
                        int v4 = r2(seed, x, y + size);
                        int v = (v1 + v2 + v3 + v4) / 4;
                        if (v > 255) v = 255;

                        pgb.CenterColor = Color.FromArgb(p, v, v, v);
                        pgb.SurroundColors = new Color[] {  Color.FromArgb(p, v1, v1, v1), 
                                                            Color.FromArgb(p, v2, v2, v2), 
                                                            Color.FromArgb(p, v3, v3, v3), 
                                                            Color.FromArgb(p, v4, v4, v4) };

                        graph.FillRectangle(pgb, new Rectangle(x, y, size, size));
                    }
                    else
                    {
                        int v = r2(seed, x, y);
                        pen.Color = Color.FromArgb((int)(255 * persistence), v, v, v);
                        graph.DrawRectangle(pen, x, y, 1, 1);
                    }
                }
            }
        }

        private void Smooth(int samplesize)
        {
            Color c = new Color();
            System.Drawing.Imaging.BitmapData bmpData = ((Bitmap)outputBox.Image).LockBits(new Rectangle(0, 0, outputBox.Image.Width, outputBox.Image.Height), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppPArgb);
            unsafe
            {
                for (int y = samplesize; y < bmpData.Height - samplesize; y++)
                {
                    for (int x = samplesize; x < bmpData.Width - samplesize; x++)
                    {
                        int sample = 0;
                        int samplecount = 0;
                        for (int i = x - samplesize; i < x + samplesize; i++)
                        {
                            for (int u = y - samplesize; u < y + samplesize; u++)
                            {
                                if (u < 0 || i < 0 || u >= bmpData.Height || i >= bmpData.Width) continue;

                                //unsafe
                                //{
                                int* ptr1 = (int*)(((int*)bmpData.Scan0) + (u * bmpData.Width) + i);
                                c = Color.FromArgb(*ptr1);
                                //}

                                sample += c.R;
                                samplecount++;
                            }
                        }
                        int color = sample / samplecount;
                        //unsafe
                        //{
                        int* ptr2 = (int*)(((int*)bmpData.Scan0) + (y * bmpData.Width) + x);
                        *ptr2 = c.ToArgb();
                        //}
                    }
                }
            }
            ((Bitmap)outputBox.Image).UnlockBits(bmpData);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            int n = (int)noiseOctaves.Value;
            

            GenerateNoise(256, (int)noiseSeed.Value, 1.0f);
          
            for (int i = 1; i < n; i++)
            {
                GenerateNoise(256 / i, (int)noiseSeed.Value, (float)noisePresistence.Value / (i * (float)presistenceReductorDivisor.Value));
            }

            Smooth(1);
  
            outputBox.Refresh();
        }

        private void linkLabel1_LinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
        {
            noiseSeed.Value = r.Next((int)noiseSeed.Minimum, (int)noiseSeed.Maximum);
        }

    }
}