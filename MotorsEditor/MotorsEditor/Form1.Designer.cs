namespace MotorsEditor
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            System.Windows.Forms.TreeNode treeNode9 = new System.Windows.Forms.TreeNode("World");
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.reloadToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tabPage6 = new System.Windows.Forms.TabPage();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.MRUListBox = new System.Windows.Forms.ListBox();
            this.webBrowser1 = new System.Windows.Forms.WebBrowser();
            this.tabPage5 = new System.Windows.Forms.TabPage();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.splitContainer3 = new System.Windows.Forms.SplitContainer();
            this.toolStrip2 = new System.Windows.Forms.ToolStrip();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.environmentToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.treeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rockToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.buildingsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.farmToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.millToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.barracksToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unitsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.peasantToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.womanToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.soldierToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.triggerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.treeView2 = new System.Windows.Forms.TreeView();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabControl2 = new System.Windows.Forms.TabControl();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.HeightmapEditor = new System.Windows.Forms.PictureBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.ChooseColorButton1 = new System.Windows.Forms.ToolStripButton();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.ConfigGrid = new System.Windows.Forms.DataGridView();
            this.Variable = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.Value = new System.Windows.Forms.DataGridViewTextBoxColumn();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.label1 = new System.Windows.Forms.Label();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.toolStripLabel1 = new System.Windows.Forms.ToolStripLabel();
            this.menuStrip1.SuspendLayout();
            this.tabPage6.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPage5.SuspendLayout();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.splitContainer3.Panel1.SuspendLayout();
            this.splitContainer3.Panel2.SuspendLayout();
            this.splitContainer3.SuspendLayout();
            this.toolStrip2.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabControl2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.HeightmapEditor)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ConfigGrid)).BeginInit();
            this.tabControl1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(896, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(35, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.reloadToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(69, 20);
            this.editToolStripMenuItem.Text = "Config.cfg";
            // 
            // reloadToolStripMenuItem
            // 
            this.reloadToolStripMenuItem.Name = "reloadToolStripMenuItem";
            this.reloadToolStripMenuItem.Size = new System.Drawing.Size(125, 22);
            this.reloadToolStripMenuItem.Text = "Reload";
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(125, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(125, 22);
            this.saveAsToolStripMenuItem.Text = "Save As...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(40, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // tabPage6
            // 
            this.tabPage6.BackColor = System.Drawing.Color.WhiteSmoke;
            this.tabPage6.Controls.Add(this.splitContainer1);
            this.tabPage6.Location = new System.Drawing.Point(4, 22);
            this.tabPage6.Name = "tabPage6";
            this.tabPage6.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage6.Size = new System.Drawing.Size(888, 670);
            this.tabPage6.TabIndex = 3;
            this.tabPage6.Text = "Start Page";
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(3, 3);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.label1);
            this.splitContainer1.Panel1.Controls.Add(this.groupBox1);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.webBrowser1);
            this.splitContainer1.Size = new System.Drawing.Size(882, 664);
            this.splitContainer1.SplitterDistance = 217;
            this.splitContainer1.TabIndex = 0;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Controls.Add(this.MRUListBox);
            this.groupBox1.Location = new System.Drawing.Point(5, 4);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(210, 132);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Recent Projects";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(7, 105);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(105, 23);
            this.button2.TabIndex = 2;
            this.button2.Text = "Open Other...";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(129, 105);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 1;
            this.button1.Text = "Open";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // MRUListBox
            // 
            this.MRUListBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.MRUListBox.FormattingEnabled = true;
            this.MRUListBox.HorizontalScrollbar = true;
            this.MRUListBox.Location = new System.Drawing.Point(7, 20);
            this.MRUListBox.Name = "MRUListBox";
            this.MRUListBox.Size = new System.Drawing.Size(197, 82);
            this.MRUListBox.TabIndex = 0;
            this.MRUListBox.DoubleClick += new System.EventHandler(this.MRUListBox_DoubleClick);
            // 
            // webBrowser1
            // 
            this.webBrowser1.AllowWebBrowserDrop = false;
            this.webBrowser1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.webBrowser1.Location = new System.Drawing.Point(0, 0);
            this.webBrowser1.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser1.Name = "webBrowser1";
            this.webBrowser1.ScriptErrorsSuppressed = true;
            this.webBrowser1.Size = new System.Drawing.Size(661, 664);
            this.webBrowser1.TabIndex = 0;
            this.webBrowser1.Url = new System.Uri("http://www.pandaclubber.com/alex/", System.UriKind.Absolute);
            this.webBrowser1.DocumentCompleted += new System.Windows.Forms.WebBrowserDocumentCompletedEventHandler(this.webBrowser1_DocumentCompleted);
            // 
            // tabPage5
            // 
            this.tabPage5.Controls.Add(this.splitContainer2);
            this.tabPage5.Location = new System.Drawing.Point(4, 22);
            this.tabPage5.Name = "tabPage5";
            this.tabPage5.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage5.Size = new System.Drawing.Size(888, 670);
            this.tabPage5.TabIndex = 2;
            this.tabPage5.Text = "Entities";
            this.tabPage5.UseVisualStyleBackColor = true;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(3, 3);
            this.splitContainer2.Name = "splitContainer2";
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.splitContainer3);
            this.splitContainer2.Size = new System.Drawing.Size(882, 664);
            this.splitContainer2.SplitterDistance = 187;
            this.splitContainer2.TabIndex = 0;
            // 
            // splitContainer3
            // 
            this.splitContainer3.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer3.Location = new System.Drawing.Point(0, 0);
            this.splitContainer3.Name = "splitContainer3";
            this.splitContainer3.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer3.Panel1
            // 
            this.splitContainer3.Panel1.Controls.Add(this.toolStrip2);
            this.splitContainer3.Panel1.Controls.Add(this.treeView2);
            // 
            // splitContainer3.Panel2
            // 
            this.splitContainer3.Panel2.Controls.Add(this.propertyGrid1);
            this.splitContainer3.Panel2.Controls.Add(this.treeView1);
            this.splitContainer3.Size = new System.Drawing.Size(187, 664);
            this.splitContainer3.SplitterDistance = 331;
            this.splitContainer3.TabIndex = 0;
            // 
            // toolStrip2
            // 
            this.toolStrip2.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripDropDownButton1});
            this.toolStrip2.Location = new System.Drawing.Point(0, 0);
            this.toolStrip2.Name = "toolStrip2";
            this.toolStrip2.Size = new System.Drawing.Size(187, 25);
            this.toolStrip2.TabIndex = 1;
            this.toolStrip2.Text = "toolStrip2";
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButton1.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.environmentToolStripMenuItem,
            this.buildingsToolStripMenuItem,
            this.unitsToolStripMenuItem,
            this.triggerToolStripMenuItem});
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(29, 22);
            this.toolStripDropDownButton1.Text = "toolStripDropDownButton1";
            // 
            // environmentToolStripMenuItem
            // 
            this.environmentToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.treeToolStripMenuItem,
            this.rockToolStripMenuItem});
            this.environmentToolStripMenuItem.Name = "environmentToolStripMenuItem";
            this.environmentToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.environmentToolStripMenuItem.Text = "Environment";
            // 
            // treeToolStripMenuItem
            // 
            this.treeToolStripMenuItem.Name = "treeToolStripMenuItem";
            this.treeToolStripMenuItem.Size = new System.Drawing.Size(97, 22);
            this.treeToolStripMenuItem.Text = "Tree";
            // 
            // rockToolStripMenuItem
            // 
            this.rockToolStripMenuItem.Name = "rockToolStripMenuItem";
            this.rockToolStripMenuItem.Size = new System.Drawing.Size(97, 22);
            this.rockToolStripMenuItem.Text = "Rock";
            // 
            // buildingsToolStripMenuItem
            // 
            this.buildingsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.farmToolStripMenuItem,
            this.millToolStripMenuItem,
            this.barracksToolStripMenuItem});
            this.buildingsToolStripMenuItem.Name = "buildingsToolStripMenuItem";
            this.buildingsToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.buildingsToolStripMenuItem.Text = "Buildings";
            // 
            // farmToolStripMenuItem
            // 
            this.farmToolStripMenuItem.Name = "farmToolStripMenuItem";
            this.farmToolStripMenuItem.Size = new System.Drawing.Size(115, 22);
            this.farmToolStripMenuItem.Text = "Farm";
            // 
            // millToolStripMenuItem
            // 
            this.millToolStripMenuItem.Name = "millToolStripMenuItem";
            this.millToolStripMenuItem.Size = new System.Drawing.Size(115, 22);
            this.millToolStripMenuItem.Text = "Mill";
            // 
            // barracksToolStripMenuItem
            // 
            this.barracksToolStripMenuItem.Name = "barracksToolStripMenuItem";
            this.barracksToolStripMenuItem.Size = new System.Drawing.Size(115, 22);
            this.barracksToolStripMenuItem.Text = "Barracks";
            // 
            // unitsToolStripMenuItem
            // 
            this.unitsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.peasantToolStripMenuItem,
            this.womanToolStripMenuItem,
            this.soldierToolStripMenuItem});
            this.unitsToolStripMenuItem.Name = "unitsToolStripMenuItem";
            this.unitsToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.unitsToolStripMenuItem.Text = "Units";
            // 
            // peasantToolStripMenuItem
            // 
            this.peasantToolStripMenuItem.Name = "peasantToolStripMenuItem";
            this.peasantToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.peasantToolStripMenuItem.Text = "Peasant";
            // 
            // womanToolStripMenuItem
            // 
            this.womanToolStripMenuItem.Name = "womanToolStripMenuItem";
            this.womanToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.womanToolStripMenuItem.Text = "Woman";
            // 
            // soldierToolStripMenuItem
            // 
            this.soldierToolStripMenuItem.Name = "soldierToolStripMenuItem";
            this.soldierToolStripMenuItem.Size = new System.Drawing.Size(113, 22);
            this.soldierToolStripMenuItem.Text = "Soldier";
            // 
            // triggerToolStripMenuItem
            // 
            this.triggerToolStripMenuItem.Name = "triggerToolStripMenuItem";
            this.triggerToolStripMenuItem.Size = new System.Drawing.Size(134, 22);
            this.triggerToolStripMenuItem.Text = "Trigger";
            // 
            // treeView2
            // 
            this.treeView2.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.treeView2.Location = new System.Drawing.Point(5, 28);
            this.treeView2.Name = "treeView2";
            treeNode9.Name = "Node0";
            treeNode9.Text = "World";
            this.treeView2.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode9});
            this.treeView2.Size = new System.Drawing.Size(179, 300);
            this.treeView2.TabIndex = 0;
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.propertyGrid1.Location = new System.Drawing.Point(5, 4);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.Size = new System.Drawing.Size(182, 322);
            this.propertyGrid1.TabIndex = 2;
            // 
            // treeView1
            // 
            this.treeView1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.treeView1.Location = new System.Drawing.Point(3, -99);
            this.treeView1.Name = "treeView1";
            this.treeView1.Size = new System.Drawing.Size(181, 0);
            this.treeView1.TabIndex = 1;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.tabControl2);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(888, 670);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Terrain";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tabControl2
            // 
            this.tabControl2.Controls.Add(this.tabPage3);
            this.tabControl2.Controls.Add(this.tabPage4);
            this.tabControl2.Location = new System.Drawing.Point(0, 0);
            this.tabControl2.Name = "tabControl2";
            this.tabControl2.SelectedIndex = 0;
            this.tabControl2.Size = new System.Drawing.Size(888, 670);
            this.tabControl2.TabIndex = 0;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.numericUpDown1);
            this.tabPage3.Controls.Add(this.HeightmapEditor);
            this.tabPage3.Controls.Add(this.toolStrip1);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(880, 644);
            this.tabPage3.TabIndex = 0;
            this.tabPage3.Text = "HeightMap";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // HeightmapEditor
            // 
            this.HeightmapEditor.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.HeightmapEditor.BackColor = System.Drawing.Color.LightGray;
            this.HeightmapEditor.Location = new System.Drawing.Point(6, 31);
            this.HeightmapEditor.Name = "HeightmapEditor";
            this.HeightmapEditor.Size = new System.Drawing.Size(874, 614);
            this.HeightmapEditor.TabIndex = 1;
            this.HeightmapEditor.TabStop = false;
            this.HeightmapEditor.MouseDown += new System.Windows.Forms.MouseEventHandler(this.HeightmapEditor_onMouseDown);
            this.HeightmapEditor.MouseMove += new System.Windows.Forms.MouseEventHandler(this.HeightmapEditor_onMouseMove);
            this.HeightmapEditor.MouseUp += new System.Windows.Forms.MouseEventHandler(this.HeightmapEditor_onMouseUp);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ChooseColorButton1,
            this.toolStripLabel1});
            this.toolStrip1.Location = new System.Drawing.Point(3, 3);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(874, 25);
            this.toolStrip1.TabIndex = 0;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // ChooseColorButton1
            // 
            this.ChooseColorButton1.Image = ((System.Drawing.Image)(resources.GetObject("ChooseColorButton1.Image")));
            this.ChooseColorButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ChooseColorButton1.Name = "ChooseColorButton1";
            this.ChooseColorButton1.Size = new System.Drawing.Size(91, 22);
            this.ChooseColorButton1.Text = "Choose Color";
            this.ChooseColorButton1.Click += new System.EventHandler(this.ChooseColorButton1_Click);
            // 
            // tabPage4
            // 
            this.tabPage4.Location = new System.Drawing.Point(4, 22);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(880, 644);
            this.tabPage4.TabIndex = 1;
            this.tabPage4.Text = "Texture";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.ConfigGrid);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(888, 670);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Config.cfg";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // ConfigGrid
            // 
            this.ConfigGrid.AllowUserToOrderColumns = true;
            this.ConfigGrid.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.ConfigGrid.BackgroundColor = System.Drawing.SystemColors.ActiveBorder;
            this.ConfigGrid.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.ConfigGrid.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Variable,
            this.Value});
            this.ConfigGrid.Location = new System.Drawing.Point(0, 0);
            this.ConfigGrid.Name = "ConfigGrid";
            this.ConfigGrid.Size = new System.Drawing.Size(888, 670);
            this.ConfigGrid.TabIndex = 0;
            // 
            // Variable
            // 
            this.Variable.HeaderText = "Variable";
            this.Variable.Name = "Variable";
            // 
            // Value
            // 
            this.Value.HeaderText = "Value";
            this.Value.Name = "Value";
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage6);
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage5);
            this.tabControl1.HotTrack = true;
            this.tabControl1.Location = new System.Drawing.Point(1, 26);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(896, 696);
            this.tabControl1.TabIndex = 1;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(9, 150);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(189, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "To begin, Open a Project above";
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(160, 6);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(40, 20);
            this.numericUpDown1.TabIndex = 2;
            this.numericUpDown1.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numericUpDown1.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
            // 
            // toolStripLabel1
            // 
            this.toolStripLabel1.Name = "toolStripLabel1";
            this.toolStripLabel1.Size = new System.Drawing.Size(60, 22);
            this.toolStripLabel1.Text = "Brush Size:";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(896, 721);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.menuStrip1);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnClosing);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.tabPage6.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel1.PerformLayout();
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.tabPage5.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.ResumeLayout(false);
            this.splitContainer3.Panel1.ResumeLayout(false);
            this.splitContainer3.Panel1.PerformLayout();
            this.splitContainer3.Panel2.ResumeLayout(false);
            this.splitContainer3.ResumeLayout(false);
            this.toolStrip2.ResumeLayout(false);
            this.toolStrip2.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabControl2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.HeightmapEditor)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.tabPage1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ConfigGrid)).EndInit();
            this.tabControl1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.TabPage tabPage6;
        private System.Windows.Forms.TabPage tabPage5;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabControl tabControl2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.DataGridView ConfigGrid;
        private System.Windows.Forms.DataGridViewTextBoxColumn Variable;
        private System.Windows.Forms.DataGridViewTextBoxColumn Value;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.WebBrowser webBrowser1;
        private System.Windows.Forms.ListBox MRUListBox;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.ToolStripMenuItem reloadToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.PictureBox HeightmapEditor;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton ChooseColorButton1;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private System.Windows.Forms.SplitContainer splitContainer3;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private System.Windows.Forms.TreeView treeView1;
        private System.Windows.Forms.TreeView treeView2;
        private System.Windows.Forms.ToolStrip toolStrip2;
        private System.Windows.Forms.ToolStripDropDownButton toolStripDropDownButton1;
        private System.Windows.Forms.ToolStripMenuItem environmentToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem buildingsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem unitsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem triggerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem treeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem rockToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem farmToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem millToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem barracksToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem peasantToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem womanToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem soldierToolStripMenuItem;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.ToolStripLabel toolStripLabel1;
    }
}

