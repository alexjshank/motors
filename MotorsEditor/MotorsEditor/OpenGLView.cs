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
using System.Threading;

namespace MotorsEditor
{
    class OpenGLView : OpenGLControl
    {
        public Terrain terrain = new Terrain();
        public float angle = 0;

        int lastMouseX = 0;
        bool mouseDown;
        bool[] keys = new bool[512];

        public override void glDraw()
        {
            GL.glClear(GL.GL_COLOR_BUFFER_BIT | GL.GL_DEPTH_BUFFER_BIT);
            GL.glLoadIdentity();

            GL.gluLookAt(terrain.MAP_SIZE / 2, terrain.MAP_SIZE / 2, -terrain.MAP_SIZE / 2, terrain.MAP_SIZE / 2, 0, terrain.MAP_SIZE / 2, 0, 1, 0);
            GL.glScalef(1, 0.2f, 1);

            GL.glTranslatef(terrain.MAP_SIZE / 2, 0, terrain.MAP_SIZE / 2);
            GL.glRotatef(angle, 0, 1, 0);
            GL.glTranslatef(-terrain.MAP_SIZE / 2, 0, -terrain.MAP_SIZE / 2);
  
            terrain.RenderHeightmap();
        }



        protected override void InitGLContext()
        {
            GL.glShadeModel(GL.GL_SMOOTH);
            GL.glClearColor(0.3f, 0.3f, 1.0f, 1.0f);
            GL.glClearDepth(1.0f);
            GL.glEnable(GL.GL_DEPTH_TEST);
            GL.glDepthFunc(GL.GL_LEQUAL);
            GL.glHint(GL.GL_PERSPECTIVE_CORRECTION_HINT, GL.GL_NICEST);

            GL.glEnable(GL.GL_TEXTURE_2D);

            this.MouseDown += new MouseEventHandler(OpenGLView_MouseDown);
            this.MouseUp += new MouseEventHandler(OpenGLView_MouseUp);
            this.MouseMove += new MouseEventHandler(OpenGLView_MouseMove);
        }

        void OpenGLView_MouseMove(object sender, MouseEventArgs e)
        {
            if (mouseDown)
            {
                angle += (e.X - lastMouseX);
                lastMouseX = e.X;
                Refresh();
            }
        }

        void OpenGLView_MouseUp(object sender, MouseEventArgs e)
        {
            mouseDown = false;
        }

        void OpenGLView_MouseDown(object sender, MouseEventArgs e)
        {
            lastMouseX = e.X;
            mouseDown = true;
        }

        protected override void OnSizeChanged(EventArgs e)
        {
            base.OnSizeChanged(e);

            Size s = Size;
            double aspect_ratio = (double)s.Width / (double)s.Height;
            GL.glMatrixMode(GL.GL_PROJECTION); 
            GL.glLoadIdentity();

            GL.gluPerspective(45.0f, aspect_ratio, 0.1f, 1000.0f);
            GL.glMatrixMode(GL.GL_MODELVIEW); 
            GL.glLoadIdentity();
        }


    }

    public class Terrain
    {
        public int MAP_SIZE = 256;
        protected int STEP_SIZE = 4;
        private static bool render = true;
        private byte[] heightmap;
        private static float scaleValue = 1.5f;
        public uint texture = 0;

        public byte[] GetData()
        {
            return heightmap;
        }

        public float Height(int X, int Y)
        {
            return ((float)rawHeight(X, Y)) * scaleValue;
        }

        public int rawHeight(int X, int Y)
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
            
            return 0;
        }

        public void LoadRawFile(string filename)
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
                string errorMsg = "An Error Occurred While Loading And Parsing World Data:\n\t" + filename + "\n\nException: "+ e.Message;
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

        public uint LoadTexture(string filename)
        {
            uint tex = 0;
            Bitmap bmp = new Bitmap(filename);
            if (bmp != null)
            {
                tex = LoadTexture(bmp);
                bmp.Dispose();
            }
            return tex;
        }


        public uint LoadTexture(Bitmap bmp)
        {
            uint[] texture = new uint[1];
            BitmapData bmpData = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height), ImageLockMode.ReadOnly, PixelFormat.Format24bppRgb);
            if (bmpData != null)
            {
                GL.glGenTextures(1, texture);
                GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MIN_FILTER, GL.GL_LINEAR);
                GL.glTexParameteri(GL.GL_TEXTURE_2D, GL.GL_TEXTURE_MAG_FILTER, GL.GL_LINEAR);
                GL.glTexImage2D(GL.GL_TEXTURE_2D, 0, (int)GL.GL_RGB8, bmp.Width, bmp.Height, 0, GL.GL_BGR_EXT, GL.GL_UNSIGNED_BYTE, bmpData.Scan0);
                bmp.UnlockBits(bmpData);
            }
            return texture[0];
        }

        public void FromBitmap(Bitmap bmp)
        {
            heightmap = new byte[bmp.Width * bmp.Height];
            for (int y = 0; y < bmp.Height; y++)
            {
                for (int x = 0; x < bmp.Width; x++)
                {
                    Color c = bmp.GetPixel(x, y);
                    heightmap[x + (y * bmp.Width)] = (byte)((c.R + c.G + c.B) / 3);
                }
            }
        }

        public void RenderHeightmap()
        {
            int X, Y;
            int x, z;
            float y;

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
                    x = X;
                    y = Height(X, Y);
                    z = Y;
                    SetVertexColor(x, z);
                    GL.glTexCoord2f(((float)x) / MAP_SIZE, ((float)z) / MAP_SIZE);
                    GL.glVertex3f((float)x, y, (float)z);

                    // Get The (X, Y, Z) Value For The Top Left Vertex
                    x = X;
                    y = Height(X, Y + STEP_SIZE);
                    z = Y + STEP_SIZE;
                    SetVertexColor(x, z);
                    GL.glTexCoord2f(((float)x) / MAP_SIZE, ((float)z) / MAP_SIZE);
                    GL.glVertex3f((float)x, y, (float)z);

                    // Get The (X, Y, Z) Value For The Top Right Vertex
                    x = X + STEP_SIZE;
                    y = Height(X + STEP_SIZE, Y + STEP_SIZE);
                    z = Y + STEP_SIZE;
                    SetVertexColor(x, z);
                    GL.glTexCoord2f(((float)x) / MAP_SIZE, ((float)z) / MAP_SIZE);
                    GL.glVertex3f((float)x, y, (float)z);

                    // Get The (X, Y, Z) Value For The Bottom Right Vertex
                    x = X + STEP_SIZE;
                    y = Height(X + STEP_SIZE, Y);
                    z = Y;
                    SetVertexColor(x, z);
                    GL.glTexCoord2f(((float)x) / MAP_SIZE, ((float)z) / MAP_SIZE);
                    GL.glVertex3f((float)x, y, (float)z);
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

            float fColor = 0.5f + (rawHeight(x, y) / 256.0f);

            GL.glColor3f(fColor, fColor, fColor);
        }
    }
}
