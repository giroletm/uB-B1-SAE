
namespace SAE202
{
    partial class Main
    {
        /// <summary>
        /// Variable nécessaire au concepteur.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Nettoyage des ressources utilisées.
        /// </summary>
        /// <param name="disposing">true si les ressources managées doivent être supprimées ; sinon, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Code généré par le Concepteur Windows Form

        /// <summary>
        /// Méthode requise pour la prise en charge du concepteur - ne modifiez pas
        /// le contenu de cette méthode avec l'éditeur de code.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Main));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveAsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.exportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportImageToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editScaleToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.multiplyAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateNodesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mainTabControl = new System.Windows.Forms.TabControl();
            this.mapTabPage = new System.Windows.Forms.TabPage();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.mapCoordsYLabel = new System.Windows.Forms.Label();
            this.mapCoordsSeparatorLabel = new System.Windows.Forms.Label();
            this.mapCoordsXLabel = new System.Windows.Forms.Label();
            this.mapSelectedPointGroupBox = new System.Windows.Forms.GroupBox();
            this.mapSelectedPointPosYNumUpDown = new System.Windows.Forms.NumericUpDown();
            this.mapSelectedPointPosYLabel = new System.Windows.Forms.Label();
            this.mapSelectedPointPosXLabel = new System.Windows.Forms.Label();
            this.mapSelectedPointPosXNumUpDown = new System.Windows.Forms.NumericUpDown();
            this.mapDefaultBiomeComboBox = new System.Windows.Forms.ComboBox();
            this.mapDefaultBiomeLabel = new System.Windows.Forms.Label();
            this.mapDisplayPanel = new System.Windows.Forms.Panel();
            this.mapDisplayPictureBox = new System.Windows.Forms.PictureBox();
            this.biomesTabPage = new System.Windows.Forms.TabPage();
            this.biomePropertiesGroupBox = new System.Windows.Forms.GroupBox();
            this.biomeChangeInsideWeightNumUpDown = new System.Windows.Forms.NumericUpDown();
            this.biomeChangeIncomingWeightNumUpDown = new System.Windows.Forms.NumericUpDown();
            this.biomeNameLabel = new System.Windows.Forms.Label();
            this.biomeChangeColorButton = new System.Windows.Forms.Button();
            this.biomeChangeNameTextBox = new System.Windows.Forms.TextBox();
            this.biomeChangeInsideWeightLabel = new System.Windows.Forms.Label();
            this.biomeChangeIncomingWeightLabel = new System.Windows.Forms.Label();
            this.biomeChangeColorLabel = new System.Windows.Forms.Label();
            this.biomeChangeNameLabel = new System.Windows.Forms.Label();
            this.removeBiomeButton = new System.Windows.Forms.Button();
            this.addBiomeButton = new System.Windows.Forms.Button();
            this.biomeListBox = new System.Windows.Forms.ListBox();
            this.menuStrip1.SuspendLayout();
            this.mainTabControl.SuspendLayout();
            this.mapTabPage.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.mapSelectedPointGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mapSelectedPointPosYNumUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.mapSelectedPointPosXNumUpDown)).BeginInit();
            this.mapDisplayPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mapDisplayPictureBox)).BeginInit();
            this.biomesTabPage.SuspendLayout();
            this.biomePropertiesGroupBox.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.biomeChangeInsideWeightNumUpDown)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.biomeChangeIncomingWeightNumUpDown)).BeginInit();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.toolsToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(800, 24);
            this.menuStrip1.TabIndex = 0;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.openToolStripMenuItem,
            this.toolStripSeparator1,
            this.saveToolStripMenuItem,
            this.saveAsToolStripMenuItem,
            this.toolStripSeparator2,
            this.exportToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(54, 20);
            this.fileToolStripMenuItem.Text = "Fichier";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.newToolStripMenuItem.Size = new System.Drawing.Size(235, 22);
            this.newToolStripMenuItem.Text = "Nouveau";
            this.newToolStripMenuItem.Click += new System.EventHandler(this.newToolStripMenuItem_Click);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.openToolStripMenuItem.Size = new System.Drawing.Size(235, 22);
            this.openToolStripMenuItem.Text = "Ouvrir...";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(232, 6);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(235, 22);
            this.saveToolStripMenuItem.Text = "Enregistrer";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // saveAsToolStripMenuItem
            // 
            this.saveAsToolStripMenuItem.Name = "saveAsToolStripMenuItem";
            this.saveAsToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)(((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Shift) 
            | System.Windows.Forms.Keys.S)));
            this.saveAsToolStripMenuItem.Size = new System.Drawing.Size(235, 22);
            this.saveAsToolStripMenuItem.Text = "Enregistrer Sous...";
            this.saveAsToolStripMenuItem.Click += new System.EventHandler(this.saveAsToolStripMenuItem_Click);
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(232, 6);
            // 
            // exportToolStripMenuItem
            // 
            this.exportToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exportImageToolStripMenuItem});
            this.exportToolStripMenuItem.Name = "exportToolStripMenuItem";
            this.exportToolStripMenuItem.Size = new System.Drawing.Size(235, 22);
            this.exportToolStripMenuItem.Text = "Exporter";
            // 
            // exportImageToolStripMenuItem
            // 
            this.exportImageToolStripMenuItem.Name = "exportImageToolStripMenuItem";
            this.exportImageToolStripMenuItem.Size = new System.Drawing.Size(107, 22);
            this.exportImageToolStripMenuItem.Text = "Image";
            this.exportImageToolStripMenuItem.Click += new System.EventHandler(this.exportImageToolStripMenuItem_Click);
            // 
            // toolsToolStripMenuItem
            // 
            this.toolsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.editScaleToolStripMenuItem,
            this.multiplyAllToolStripMenuItem,
            this.generateNodesToolStripMenuItem});
            this.toolsToolStripMenuItem.Name = "toolsToolStripMenuItem";
            this.toolsToolStripMenuItem.Size = new System.Drawing.Size(50, 20);
            this.toolsToolStripMenuItem.Text = "Outils";
            // 
            // editScaleToolStripMenuItem
            // 
            this.editScaleToolStripMenuItem.Name = "editScaleToolStripMenuItem";
            this.editScaleToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.editScaleToolStripMenuItem.Text = "Modifier l\'échelle";
            this.editScaleToolStripMenuItem.Click += new System.EventHandler(this.editScaleToolStripMenuItem_Click);
            // 
            // multiplyAllToolStripMenuItem
            // 
            this.multiplyAllToolStripMenuItem.Name = "multiplyAllToolStripMenuItem";
            this.multiplyAllToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.multiplyAllToolStripMenuItem.Text = "Tout multiplier";
            this.multiplyAllToolStripMenuItem.Click += new System.EventHandler(this.multiplyAllToolStripMenuItem_Click);
            // 
            // generateNodesToolStripMenuItem
            // 
            this.generateNodesToolStripMenuItem.Name = "generateNodesToolStripMenuItem";
            this.generateNodesToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.generateNodesToolStripMenuItem.Text = "Générer un maillage";
            this.generateNodesToolStripMenuItem.Click += new System.EventHandler(this.generateNodesToolStripMenuItem_Click);
            // 
            // mainTabControl
            // 
            this.mainTabControl.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mainTabControl.Controls.Add(this.mapTabPage);
            this.mainTabControl.Controls.Add(this.biomesTabPage);
            this.mainTabControl.Location = new System.Drawing.Point(12, 27);
            this.mainTabControl.Name = "mainTabControl";
            this.mainTabControl.SelectedIndex = 0;
            this.mainTabControl.Size = new System.Drawing.Size(776, 411);
            this.mainTabControl.TabIndex = 1;
            // 
            // mapTabPage
            // 
            this.mapTabPage.Controls.Add(this.groupBox1);
            this.mapTabPage.Controls.Add(this.mapDisplayPanel);
            this.mapTabPage.Location = new System.Drawing.Point(4, 22);
            this.mapTabPage.Name = "mapTabPage";
            this.mapTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.mapTabPage.Size = new System.Drawing.Size(768, 385);
            this.mapTabPage.TabIndex = 0;
            this.mapTabPage.Text = "Map";
            this.mapTabPage.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.mapCoordsYLabel);
            this.groupBox1.Controls.Add(this.mapCoordsSeparatorLabel);
            this.groupBox1.Controls.Add(this.mapCoordsXLabel);
            this.groupBox1.Controls.Add(this.mapSelectedPointGroupBox);
            this.groupBox1.Controls.Add(this.mapDefaultBiomeComboBox);
            this.groupBox1.Controls.Add(this.mapDefaultBiomeLabel);
            this.groupBox1.Location = new System.Drawing.Point(499, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(263, 373);
            this.groupBox1.TabIndex = 1;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Propriétés de la map";
            // 
            // mapCoordsYLabel
            // 
            this.mapCoordsYLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.mapCoordsYLabel.Location = new System.Drawing.Point(65, 353);
            this.mapCoordsYLabel.Name = "mapCoordsYLabel";
            this.mapCoordsYLabel.Size = new System.Drawing.Size(37, 13);
            this.mapCoordsYLabel.TabIndex = 7;
            this.mapCoordsYLabel.Text = "00000";
            // 
            // mapCoordsSeparatorLabel
            // 
            this.mapCoordsSeparatorLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.mapCoordsSeparatorLabel.AutoSize = true;
            this.mapCoordsSeparatorLabel.Location = new System.Drawing.Point(49, 353);
            this.mapCoordsSeparatorLabel.Name = "mapCoordsSeparatorLabel";
            this.mapCoordsSeparatorLabel.Size = new System.Drawing.Size(10, 13);
            this.mapCoordsSeparatorLabel.TabIndex = 6;
            this.mapCoordsSeparatorLabel.Text = ";";
            // 
            // mapCoordsXLabel
            // 
            this.mapCoordsXLabel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.mapCoordsXLabel.Location = new System.Drawing.Point(6, 353);
            this.mapCoordsXLabel.Name = "mapCoordsXLabel";
            this.mapCoordsXLabel.Size = new System.Drawing.Size(37, 13);
            this.mapCoordsXLabel.TabIndex = 5;
            this.mapCoordsXLabel.Text = "00000";
            this.mapCoordsXLabel.TextAlign = System.Drawing.ContentAlignment.TopRight;
            // 
            // mapSelectedPointGroupBox
            // 
            this.mapSelectedPointGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.mapSelectedPointGroupBox.Controls.Add(this.mapSelectedPointPosYNumUpDown);
            this.mapSelectedPointGroupBox.Controls.Add(this.mapSelectedPointPosYLabel);
            this.mapSelectedPointGroupBox.Controls.Add(this.mapSelectedPointPosXLabel);
            this.mapSelectedPointGroupBox.Controls.Add(this.mapSelectedPointPosXNumUpDown);
            this.mapSelectedPointGroupBox.Location = new System.Drawing.Point(9, 64);
            this.mapSelectedPointGroupBox.Name = "mapSelectedPointGroupBox";
            this.mapSelectedPointGroupBox.Size = new System.Drawing.Size(248, 286);
            this.mapSelectedPointGroupBox.TabIndex = 4;
            this.mapSelectedPointGroupBox.TabStop = false;
            this.mapSelectedPointGroupBox.Text = "Propriétés du point sélectionné";
            // 
            // mapSelectedPointPosYNumUpDown
            // 
            this.mapSelectedPointPosYNumUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mapSelectedPointPosYNumUpDown.DecimalPlaces = 2;
            this.mapSelectedPointPosYNumUpDown.Location = new System.Drawing.Point(75, 57);
            this.mapSelectedPointPosYNumUpDown.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.mapSelectedPointPosYNumUpDown.Minimum = new decimal(new int[] {
            9999,
            0,
            0,
            -2147483648});
            this.mapSelectedPointPosYNumUpDown.Name = "mapSelectedPointPosYNumUpDown";
            this.mapSelectedPointPosYNumUpDown.Size = new System.Drawing.Size(167, 20);
            this.mapSelectedPointPosYNumUpDown.TabIndex = 14;
            this.mapSelectedPointPosYNumUpDown.ValueChanged += new System.EventHandler(this.mapSelectedPointPosYNumUpDown_ValueChanged);
            // 
            // mapSelectedPointPosYLabel
            // 
            this.mapSelectedPointPosYLabel.AutoSize = true;
            this.mapSelectedPointPosYLabel.Location = new System.Drawing.Point(6, 59);
            this.mapSelectedPointPosYLabel.Name = "mapSelectedPointPosYLabel";
            this.mapSelectedPointPosYLabel.Size = new System.Drawing.Size(63, 13);
            this.mapSelectedPointPosYLabel.TabIndex = 13;
            this.mapSelectedPointPosYLabel.Text = "Position (Y):";
            // 
            // mapSelectedPointPosXLabel
            // 
            this.mapSelectedPointPosXLabel.AutoSize = true;
            this.mapSelectedPointPosXLabel.Location = new System.Drawing.Point(6, 32);
            this.mapSelectedPointPosXLabel.Name = "mapSelectedPointPosXLabel";
            this.mapSelectedPointPosXLabel.Size = new System.Drawing.Size(63, 13);
            this.mapSelectedPointPosXLabel.TabIndex = 12;
            this.mapSelectedPointPosXLabel.Text = "Position (X):";
            // 
            // mapSelectedPointPosXNumUpDown
            // 
            this.mapSelectedPointPosXNumUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mapSelectedPointPosXNumUpDown.DecimalPlaces = 2;
            this.mapSelectedPointPosXNumUpDown.Location = new System.Drawing.Point(75, 30);
            this.mapSelectedPointPosXNumUpDown.Maximum = new decimal(new int[] {
            9999,
            0,
            0,
            0});
            this.mapSelectedPointPosXNumUpDown.Minimum = new decimal(new int[] {
            9999,
            0,
            0,
            -2147483648});
            this.mapSelectedPointPosXNumUpDown.Name = "mapSelectedPointPosXNumUpDown";
            this.mapSelectedPointPosXNumUpDown.Size = new System.Drawing.Size(167, 20);
            this.mapSelectedPointPosXNumUpDown.TabIndex = 11;
            this.mapSelectedPointPosXNumUpDown.ValueChanged += new System.EventHandler(this.mapSelectedPointPosXNumUpDown_ValueChanged);
            // 
            // mapDefaultBiomeComboBox
            // 
            this.mapDefaultBiomeComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mapDefaultBiomeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.mapDefaultBiomeComboBox.FormattingEnabled = true;
            this.mapDefaultBiomeComboBox.Location = new System.Drawing.Point(97, 23);
            this.mapDefaultBiomeComboBox.Name = "mapDefaultBiomeComboBox";
            this.mapDefaultBiomeComboBox.Size = new System.Drawing.Size(160, 21);
            this.mapDefaultBiomeComboBox.TabIndex = 3;
            this.mapDefaultBiomeComboBox.SelectedIndexChanged += new System.EventHandler(this.mapDefaultBiomeComboBox_SelectedIndexChanged);
            // 
            // mapDefaultBiomeLabel
            // 
            this.mapDefaultBiomeLabel.AutoSize = true;
            this.mapDefaultBiomeLabel.Location = new System.Drawing.Point(6, 26);
            this.mapDefaultBiomeLabel.Name = "mapDefaultBiomeLabel";
            this.mapDefaultBiomeLabel.Size = new System.Drawing.Size(90, 13);
            this.mapDefaultBiomeLabel.TabIndex = 2;
            this.mapDefaultBiomeLabel.Text = "Biome par défaut:";
            // 
            // mapDisplayPanel
            // 
            this.mapDisplayPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.mapDisplayPanel.AutoScroll = true;
            this.mapDisplayPanel.Controls.Add(this.mapDisplayPictureBox);
            this.mapDisplayPanel.Location = new System.Drawing.Point(6, 6);
            this.mapDisplayPanel.Name = "mapDisplayPanel";
            this.mapDisplayPanel.Size = new System.Drawing.Size(487, 373);
            this.mapDisplayPanel.TabIndex = 0;
            // 
            // mapDisplayPictureBox
            // 
            this.mapDisplayPictureBox.Location = new System.Drawing.Point(3, 3);
            this.mapDisplayPictureBox.Name = "mapDisplayPictureBox";
            this.mapDisplayPictureBox.Size = new System.Drawing.Size(481, 367);
            this.mapDisplayPictureBox.TabIndex = 0;
            this.mapDisplayPictureBox.TabStop = false;
            this.mapDisplayPictureBox.Click += new System.EventHandler(this.mapDisplayPictureBox_Click);
            this.mapDisplayPictureBox.DoubleClick += new System.EventHandler(this.mapDisplayPictureBox_DoubleClick);
            this.mapDisplayPictureBox.MouseEnter += new System.EventHandler(this.mapDisplayPictureBox_MouseEnter);
            this.mapDisplayPictureBox.MouseLeave += new System.EventHandler(this.mapDisplayPictureBox_MouseLeave);
            this.mapDisplayPictureBox.MouseMove += new System.Windows.Forms.MouseEventHandler(this.mapDisplayPictureBox_MouseMove);
            // 
            // biomesTabPage
            // 
            this.biomesTabPage.Controls.Add(this.biomePropertiesGroupBox);
            this.biomesTabPage.Controls.Add(this.removeBiomeButton);
            this.biomesTabPage.Controls.Add(this.addBiomeButton);
            this.biomesTabPage.Controls.Add(this.biomeListBox);
            this.biomesTabPage.Location = new System.Drawing.Point(4, 22);
            this.biomesTabPage.Name = "biomesTabPage";
            this.biomesTabPage.Padding = new System.Windows.Forms.Padding(3);
            this.biomesTabPage.Size = new System.Drawing.Size(768, 385);
            this.biomesTabPage.TabIndex = 1;
            this.biomesTabPage.Text = "Biomes";
            this.biomesTabPage.UseVisualStyleBackColor = true;
            // 
            // biomePropertiesGroupBox
            // 
            this.biomePropertiesGroupBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.biomePropertiesGroupBox.Controls.Add(this.biomeChangeInsideWeightNumUpDown);
            this.biomePropertiesGroupBox.Controls.Add(this.biomeChangeIncomingWeightNumUpDown);
            this.biomePropertiesGroupBox.Controls.Add(this.biomeNameLabel);
            this.biomePropertiesGroupBox.Controls.Add(this.biomeChangeColorButton);
            this.biomePropertiesGroupBox.Controls.Add(this.biomeChangeNameTextBox);
            this.biomePropertiesGroupBox.Controls.Add(this.biomeChangeInsideWeightLabel);
            this.biomePropertiesGroupBox.Controls.Add(this.biomeChangeIncomingWeightLabel);
            this.biomePropertiesGroupBox.Controls.Add(this.biomeChangeColorLabel);
            this.biomePropertiesGroupBox.Controls.Add(this.biomeChangeNameLabel);
            this.biomePropertiesGroupBox.Location = new System.Drawing.Point(168, 6);
            this.biomePropertiesGroupBox.Name = "biomePropertiesGroupBox";
            this.biomePropertiesGroupBox.Size = new System.Drawing.Size(594, 373);
            this.biomePropertiesGroupBox.TabIndex = 3;
            this.biomePropertiesGroupBox.TabStop = false;
            this.biomePropertiesGroupBox.Text = "Propriétés du biome";
            // 
            // biomeChangeInsideWeightNumUpDown
            // 
            this.biomeChangeInsideWeightNumUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.biomeChangeInsideWeightNumUpDown.DecimalPlaces = 2;
            this.biomeChangeInsideWeightNumUpDown.Location = new System.Drawing.Point(88, 119);
            this.biomeChangeInsideWeightNumUpDown.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.biomeChangeInsideWeightNumUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.biomeChangeInsideWeightNumUpDown.Name = "biomeChangeInsideWeightNumUpDown";
            this.biomeChangeInsideWeightNumUpDown.Size = new System.Drawing.Size(500, 20);
            this.biomeChangeInsideWeightNumUpDown.TabIndex = 11;
            this.biomeChangeInsideWeightNumUpDown.ValueChanged += new System.EventHandler(this.biomeChangeInsideWeightNumUpDown_ValueChanged);
            // 
            // biomeChangeIncomingWeightNumUpDown
            // 
            this.biomeChangeIncomingWeightNumUpDown.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.biomeChangeIncomingWeightNumUpDown.DecimalPlaces = 2;
            this.biomeChangeIncomingWeightNumUpDown.Location = new System.Drawing.Point(88, 93);
            this.biomeChangeIncomingWeightNumUpDown.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.biomeChangeIncomingWeightNumUpDown.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.biomeChangeIncomingWeightNumUpDown.Name = "biomeChangeIncomingWeightNumUpDown";
            this.biomeChangeIncomingWeightNumUpDown.Size = new System.Drawing.Size(500, 20);
            this.biomeChangeIncomingWeightNumUpDown.TabIndex = 10;
            this.biomeChangeIncomingWeightNumUpDown.ValueChanged += new System.EventHandler(this.biomeChangeIncomingWeightNumUpDown_ValueChanged);
            // 
            // biomeNameLabel
            // 
            this.biomeNameLabel.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.biomeNameLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 11.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.biomeNameLabel.Location = new System.Drawing.Point(6, 16);
            this.biomeNameLabel.Name = "biomeNameLabel";
            this.biomeNameLabel.Size = new System.Drawing.Size(582, 23);
            this.biomeNameLabel.TabIndex = 9;
            this.biomeNameLabel.Text = "BIOME";
            this.biomeNameLabel.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // biomeChangeColorButton
            // 
            this.biomeChangeColorButton.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.biomeChangeColorButton.Location = new System.Drawing.Point(88, 67);
            this.biomeChangeColorButton.Name = "biomeChangeColorButton";
            this.biomeChangeColorButton.Size = new System.Drawing.Size(500, 20);
            this.biomeChangeColorButton.TabIndex = 8;
            this.biomeChangeColorButton.UseVisualStyleBackColor = true;
            this.biomeChangeColorButton.Click += new System.EventHandler(this.biomeChangeColorButton_Click);
            // 
            // biomeChangeNameTextBox
            // 
            this.biomeChangeNameTextBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.biomeChangeNameTextBox.Location = new System.Drawing.Point(88, 42);
            this.biomeChangeNameTextBox.Name = "biomeChangeNameTextBox";
            this.biomeChangeNameTextBox.Size = new System.Drawing.Size(500, 20);
            this.biomeChangeNameTextBox.TabIndex = 4;
            this.biomeChangeNameTextBox.TextChanged += new System.EventHandler(this.biomeChangeNameTextBox_TextChanged);
            // 
            // biomeChangeInsideWeightLabel
            // 
            this.biomeChangeInsideWeightLabel.AutoSize = true;
            this.biomeChangeInsideWeightLabel.Location = new System.Drawing.Point(6, 121);
            this.biomeChangeInsideWeightLabel.Name = "biomeChangeInsideWeightLabel";
            this.biomeChangeInsideWeightLabel.Size = new System.Drawing.Size(76, 13);
            this.biomeChangeInsideWeightLabel.TabIndex = 3;
            this.biomeChangeInsideWeightLabel.Text = "Poids intérieur:";
            // 
            // biomeChangeIncomingWeightLabel
            // 
            this.biomeChangeIncomingWeightLabel.AutoSize = true;
            this.biomeChangeIncomingWeightLabel.Location = new System.Drawing.Point(10, 95);
            this.biomeChangeIncomingWeightLabel.Name = "biomeChangeIncomingWeightLabel";
            this.biomeChangeIncomingWeightLabel.Size = new System.Drawing.Size(72, 13);
            this.biomeChangeIncomingWeightLabel.TabIndex = 2;
            this.biomeChangeIncomingWeightLabel.Text = "Poids entrant:";
            // 
            // biomeChangeColorLabel
            // 
            this.biomeChangeColorLabel.AutoSize = true;
            this.biomeChangeColorLabel.Location = new System.Drawing.Point(36, 71);
            this.biomeChangeColorLabel.Name = "biomeChangeColorLabel";
            this.biomeChangeColorLabel.Size = new System.Drawing.Size(46, 13);
            this.biomeChangeColorLabel.TabIndex = 1;
            this.biomeChangeColorLabel.Text = "Couleur:";
            // 
            // biomeChangeNameLabel
            // 
            this.biomeChangeNameLabel.AutoSize = true;
            this.biomeChangeNameLabel.Location = new System.Drawing.Point(50, 45);
            this.biomeChangeNameLabel.Name = "biomeChangeNameLabel";
            this.biomeChangeNameLabel.Size = new System.Drawing.Size(32, 13);
            this.biomeChangeNameLabel.TabIndex = 0;
            this.biomeChangeNameLabel.Text = "Nom:";
            // 
            // removeBiomeButton
            // 
            this.removeBiomeButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.removeBiomeButton.Location = new System.Drawing.Point(87, 356);
            this.removeBiomeButton.Name = "removeBiomeButton";
            this.removeBiomeButton.Size = new System.Drawing.Size(75, 23);
            this.removeBiomeButton.TabIndex = 2;
            this.removeBiomeButton.Text = "Supprimer";
            this.removeBiomeButton.UseVisualStyleBackColor = true;
            this.removeBiomeButton.Click += new System.EventHandler(this.removeBiomeButton_Click);
            // 
            // addBiomeButton
            // 
            this.addBiomeButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.addBiomeButton.Location = new System.Drawing.Point(6, 356);
            this.addBiomeButton.Name = "addBiomeButton";
            this.addBiomeButton.Size = new System.Drawing.Size(75, 23);
            this.addBiomeButton.TabIndex = 1;
            this.addBiomeButton.Text = "Ajouter";
            this.addBiomeButton.UseVisualStyleBackColor = true;
            this.addBiomeButton.Click += new System.EventHandler(this.addBiomeButton_Click);
            // 
            // biomeListBox
            // 
            this.biomeListBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.biomeListBox.FormattingEnabled = true;
            this.biomeListBox.Location = new System.Drawing.Point(6, 6);
            this.biomeListBox.Name = "biomeListBox";
            this.biomeListBox.Size = new System.Drawing.Size(156, 342);
            this.biomeListBox.TabIndex = 0;
            this.biomeListBox.SelectedIndexChanged += new System.EventHandler(this.biomeListBox_SelectedIndexChanged);
            // 
            // Main
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.mainTabControl);
            this.Controls.Add(this.menuStrip1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Main";
            this.Text = "SAE 2.02 Map Editor";
            this.Load += new System.EventHandler(this.Main_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Main_KeyDown);
            this.Resize += new System.EventHandler(this.Main_Resize);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.mainTabControl.ResumeLayout(false);
            this.mapTabPage.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.mapSelectedPointGroupBox.ResumeLayout(false);
            this.mapSelectedPointGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.mapSelectedPointPosYNumUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.mapSelectedPointPosXNumUpDown)).EndInit();
            this.mapDisplayPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.mapDisplayPictureBox)).EndInit();
            this.biomesTabPage.ResumeLayout(false);
            this.biomePropertiesGroupBox.ResumeLayout(false);
            this.biomePropertiesGroupBox.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.biomeChangeInsideWeightNumUpDown)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.biomeChangeIncomingWeightNumUpDown)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveAsToolStripMenuItem;
        private System.Windows.Forms.TabControl mainTabControl;
        private System.Windows.Forms.TabPage mapTabPage;
        private System.Windows.Forms.TabPage biomesTabPage;
        private System.Windows.Forms.Button removeBiomeButton;
        private System.Windows.Forms.Button addBiomeButton;
        private System.Windows.Forms.ListBox biomeListBox;
        private System.Windows.Forms.GroupBox biomePropertiesGroupBox;
        private System.Windows.Forms.TextBox biomeChangeNameTextBox;
        private System.Windows.Forms.Label biomeChangeInsideWeightLabel;
        private System.Windows.Forms.Label biomeChangeIncomingWeightLabel;
        private System.Windows.Forms.Label biomeChangeColorLabel;
        private System.Windows.Forms.Label biomeChangeNameLabel;
        private System.Windows.Forms.Button biomeChangeColorButton;
        private System.Windows.Forms.Label biomeNameLabel;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.NumericUpDown biomeChangeInsideWeightNumUpDown;
        private System.Windows.Forms.NumericUpDown biomeChangeIncomingWeightNumUpDown;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.ComboBox mapDefaultBiomeComboBox;
        private System.Windows.Forms.Label mapDefaultBiomeLabel;
        private System.Windows.Forms.Panel mapDisplayPanel;
        private System.Windows.Forms.GroupBox mapSelectedPointGroupBox;
        private System.Windows.Forms.NumericUpDown mapSelectedPointPosYNumUpDown;
        private System.Windows.Forms.Label mapSelectedPointPosYLabel;
        private System.Windows.Forms.Label mapSelectedPointPosXLabel;
        private System.Windows.Forms.NumericUpDown mapSelectedPointPosXNumUpDown;
        private System.Windows.Forms.PictureBox mapDisplayPictureBox;
        private System.Windows.Forms.Label mapCoordsYLabel;
        private System.Windows.Forms.Label mapCoordsSeparatorLabel;
        private System.Windows.Forms.Label mapCoordsXLabel;
        private System.Windows.Forms.ToolStripMenuItem toolsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editScaleToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem multiplyAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem generateNodesToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
        private System.Windows.Forms.ToolStripMenuItem exportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportImageToolStripMenuItem;
    }
}

