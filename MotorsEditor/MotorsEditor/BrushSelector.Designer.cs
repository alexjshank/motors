namespace MotorsEditor
{
    partial class BrushSelector
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
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.brushSizePicker = new System.Windows.Forms.NumericUpDown();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.button1 = new System.Windows.Forms.Button();
            this.BrushPreview = new System.Windows.Forms.PictureBox();
            this.label3 = new System.Windows.Forms.Label();
            this.brushColorPicker = new System.Windows.Forms.NumericUpDown();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.brushSizePicker)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.BrushPreview)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.brushColorPicker)).BeginInit();
            this.SuspendLayout();
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Items.AddRange(new object[] {
            "Round",
            "Square",
            "Airbrush"});
            this.comboBox1.Location = new System.Drawing.Point(46, 19);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(69, 21);
            this.comboBox1.TabIndex = 0;
            this.comboBox1.Text = "Round";
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.brushColorPicker);
            this.groupBox1.Controls.Add(this.label3);
            this.groupBox1.Controls.Add(this.brushSizePicker);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.comboBox1);
            this.groupBox1.Location = new System.Drawing.Point(4, 5);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(123, 116);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Brush";
            // 
            // brushSizePicker
            // 
            this.brushSizePicker.Location = new System.Drawing.Point(46, 52);
            this.brushSizePicker.Name = "brushSizePicker";
            this.brushSizePicker.Size = new System.Drawing.Size(69, 20);
            this.brushSizePicker.TabIndex = 3;
            this.brushSizePicker.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.brushSizePicker.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 54);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(27, 13);
            this.label2.TabIndex = 2;
            this.label2.Text = "Size";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(9, 22);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(31, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "Type";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(133, 12);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(72, 23);
            this.button1.TabIndex = 2;
            this.button1.Text = "Okay";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // BrushPreview
            // 
            this.BrushPreview.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.BrushPreview.Location = new System.Drawing.Point(133, 47);
            this.BrushPreview.Name = "BrushPreview";
            this.BrushPreview.Size = new System.Drawing.Size(72, 72);
            this.BrushPreview.TabIndex = 4;
            this.BrushPreview.TabStop = false;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(2, 86);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(38, 13);
            this.label3.TabIndex = 4;
            this.label3.Text = "Height";
            // 
            // brushColorPicker
            // 
            this.brushColorPicker.Location = new System.Drawing.Point(46, 84);
            this.brushColorPicker.Maximum = new decimal(new int[] {
            255,
            0,
            0,
            0});
            this.brushColorPicker.Name = "brushColorPicker";
            this.brushColorPicker.Size = new System.Drawing.Size(69, 20);
            this.brushColorPicker.TabIndex = 5;
            this.brushColorPicker.ValueChanged += new System.EventHandler(this.brushColorPicker_ValueChanged);
            // 
            // BrushSelector
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(207, 126);
            this.Controls.Add(this.BrushPreview);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.groupBox1);
            this.Name = "BrushSelector";
            this.Text = "BrushSelector";
            this.Load += new System.EventHandler(this.BrushSelector_Load);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.brushSizePicker)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.BrushPreview)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.brushColorPicker)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ComboBox comboBox1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown brushSizePicker;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.PictureBox BrushPreview;
        private System.Windows.Forms.NumericUpDown brushColorPicker;
        private System.Windows.Forms.Label label3;
    }
}