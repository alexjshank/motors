namespace MotorsEditor
{
    partial class Noise
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.noiseOctaves = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.noisePresistence = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label92 = new System.Windows.Forms.Label();
            this.presistenceReductorDivisor = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.noiseOctaves)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.noisePresistence)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.presistenceReductorDivisor)).BeginInit();
            this.SuspendLayout();
            // 
            // noiseOctaves
            // 
            this.noiseOctaves.Location = new System.Drawing.Point(91, 19);
            this.noiseOctaves.Name = "noiseOctaves";
            this.noiseOctaves.Size = new System.Drawing.Size(54, 20);
            this.noiseOctaves.TabIndex = 0;
            this.noiseOctaves.Value = new decimal(new int[] {
            8,
            0,
            0,
            0});
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(38, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Octaves";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(58, 109);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 2;
            this.button1.Text = "Generate";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // noisePresistence
            // 
            this.noisePresistence.DecimalPlaces = 2;
            this.noisePresistence.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.noisePresistence.Location = new System.Drawing.Point(91, 46);
            this.noisePresistence.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.noisePresistence.Name = "noisePresistence";
            this.noisePresistence.Size = new System.Drawing.Size(54, 20);
            this.noisePresistence.TabIndex = 3;
            this.noisePresistence.Value = new decimal(new int[] {
            5,
            0,
            0,
            65536});
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(23, 48);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(62, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Presistence";
            // 
            // label92
            // 
            this.label92.AutoSize = true;
            this.label92.Location = new System.Drawing.Point(55, 75);
            this.label92.Name = "label92";
            this.label92.Size = new System.Drawing.Size(30, 13);
            this.label92.TabIndex = 5;
            this.label92.Text = "PRD";
            // 
            // presistenceReductorDivisor
            // 
            this.presistenceReductorDivisor.DecimalPlaces = 4;
            this.presistenceReductorDivisor.Increment = new decimal(new int[] {
            5,
            0,
            0,
            131072});
            this.presistenceReductorDivisor.Location = new System.Drawing.Point(91, 73);
            this.presistenceReductorDivisor.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            262144});
            this.presistenceReductorDivisor.Name = "presistenceReductorDivisor";
            this.presistenceReductorDivisor.Size = new System.Drawing.Size(54, 20);
            this.presistenceReductorDivisor.TabIndex = 6;
            this.presistenceReductorDivisor.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // Noise
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(189, 145);
            this.Controls.Add(this.presistenceReductorDivisor);
            this.Controls.Add(this.label92);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.noisePresistence);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.noiseOctaves);
            this.Name = "Noise";
            this.Text = "I CAN HAZ RNG";
            ((System.ComponentModel.ISupportInitialize)(this.noiseOctaves)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.noisePresistence)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.presistenceReductorDivisor)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.NumericUpDown noiseOctaves;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.NumericUpDown noisePresistence;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label92;
        private System.Windows.Forms.NumericUpDown presistenceReductorDivisor;
    }
}