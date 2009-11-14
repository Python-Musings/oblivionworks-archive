namespace OblivionModManager {
    partial class CreateModForm {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing) {
            if(disposing && (components != null)) {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent() {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CreateModForm));
            this.bEdReadme = new System.Windows.Forms.Button();
            this.bEdScript = new System.Windows.Forms.Button();
            this.cmbCompType = new System.Windows.Forms.ComboBox();
            this.cmbDataCompLevel = new System.Windows.Forms.ComboBox();
            this.cmbModCompLevel = new System.Windows.Forms.ComboBox();
            this.lvFiles = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.FilesContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.openToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.validateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importModDetailsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.scanForDataFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewRequiredDataFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.tbName = new System.Windows.Forms.TextBox();
            this.label4 = new System.Windows.Forms.Label();
            this.tbVersion = new System.Windows.Forms.TextBox();
            this.label5 = new System.Windows.Forms.Label();
            this.rbPlugins = new System.Windows.Forms.RadioButton();
            this.rbData = new System.Windows.Forms.RadioButton();
            this.bEdDescription = new System.Windows.Forms.Button();
            this.bCreate = new System.Windows.Forms.Button();
            this.OpenDialog = new System.Windows.Forms.OpenFileDialog();
            this.FolderDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.tbAuthor = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.tbEmail = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.bUp = new System.Windows.Forms.Button();
            this.bDown = new System.Windows.Forms.Button();
            this.tbWebsite = new System.Windows.Forms.TextBox();
            this.label8 = new System.Windows.Forms.Label();
            this.bScreenshot = new System.Windows.Forms.Button();
            this.ScreenshotPic = new System.Windows.Forms.PictureBox();
            this.cbIncludeVersion = new System.Windows.Forms.CheckBox();
            this.bGroups = new System.Windows.Forms.Button();
            this.cmsGroups = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.bRemoveScreenshot = new System.Windows.Forms.Button();
            this.bAddZip = new System.Windows.Forms.Button();
            this.bAddFromFolder = new System.Windows.Forms.Button();
            this.bAdd = new System.Windows.Forms.Button();
            this.bEdCredits = new System.Windows.Forms.Button();
            this.bEdChangeLog = new System.Windows.Forms.Button();
            this.bEdComments = new System.Windows.Forms.Button();
            this.FilesContextMenu.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.ScreenshotPic)).BeginInit();
            this.SuspendLayout();
            // 
            // bEdReadme
            // 
            this.bEdReadme.Location = new System.Drawing.Point(12, 177);
            this.bEdReadme.Name = "bEdReadme";
            this.bEdReadme.Size = new System.Drawing.Size(100, 23);
            this.bEdReadme.TabIndex = 17;
            this.bEdReadme.Text = "Edit readme";
            this.bEdReadme.Click += new System.EventHandler(this.bEdReadme_Click);
            // 
            // bEdChangeLog
            // 
            this.bEdChangeLog.Location = new System.Drawing.Point(12, 177);
            this.bEdChangeLog.Name = "bEdChangeLog";
            this.bEdChangeLog.Size = new System.Drawing.Size(100, 23);
            this.bEdChangeLog.TabIndex = 17;
            this.bEdChangeLog.Text = "Edit Changelog";
            this.bEdChangeLog.Click += new System.EventHandler(this.bEdChangeLog_Click);
            // 
            // bEdComments
            // 
            this.bEdComments.Location = new System.Drawing.Point(12, 177);
            this.bEdComments.Name = "bEdComments";
            this.bEdComments.Size = new System.Drawing.Size(100, 23);
            this.bEdComments.TabIndex = 17;
            this.bEdComments.Text = "Edit Comments";
            this.bEdComments.Click += new System.EventHandler(this.bEdComments_Click);
            // 
            // bEdCredits
            // 
            this.bEdCredits.Location = new System.Drawing.Point(12, 177);
            this.bEdCredits.Name = "bEdCredits";
            this.bEdCredits.Size = new System.Drawing.Size(100, 23);
            this.bEdCredits.TabIndex = 17;
            this.bEdCredits.Text = "Edit Credits";
            this.bEdCredits.Click += new System.EventHandler(this.bEdCredits_Click);
            // 
            // bEdScript
            // 
            this.bEdScript.Location = new System.Drawing.Point(118, 177);
            this.bEdScript.Name = "bEdScript";
            this.bEdScript.Size = new System.Drawing.Size(100, 23);
            this.bEdScript.TabIndex = 18;
            this.bEdScript.Text = "Edit script";
            this.bEdScript.Click += new System.EventHandler(this.bEdScript_Click);
            // 
            // cmbCompType
            // 
            this.cmbCompType.Items.AddRange(new object[] {
            "7-zip",
            "zip"});
            this.cmbCompType.Location = new System.Drawing.Point(12, 90);
            this.cmbCompType.Name = "cmbCompType";
            this.cmbCompType.Size = new System.Drawing.Size(121, 21);
            this.cmbCompType.TabIndex = 11;
            this.cmbCompType.SelectedIndexChanged += new System.EventHandler(this.cmbCompLevel1_SelectedIndexChanged);
            this.cmbCompType.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.cmbNoKeyPress);
            // 
            // cmbDataCompLevel
            // 
            this.cmbDataCompLevel.Items.AddRange(new object[] {
            "Very high",
            "High",
            "Medium",
            "Low",
            "Very low"});
            this.cmbDataCompLevel.Location = new System.Drawing.Point(12, 117);
            this.cmbDataCompLevel.Name = "cmbDataCompLevel";
            this.cmbDataCompLevel.Size = new System.Drawing.Size(121, 21);
            this.cmbDataCompLevel.TabIndex = 13;
            this.cmbDataCompLevel.SelectedIndexChanged += new System.EventHandler(this.cmbCompLevel1_SelectedIndexChanged);
            this.cmbDataCompLevel.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.cmbNoKeyPress);
            // 
            // cmbModCompLevel
            // 
            this.cmbModCompLevel.Items.AddRange(new object[] {
            "Very high",
            "High",
            "Medium",
            "Low",
            "Very low",
            "None"});
            this.cmbModCompLevel.Location = new System.Drawing.Point(12, 144);
            this.cmbModCompLevel.Name = "cmbModCompLevel";
            this.cmbModCompLevel.Size = new System.Drawing.Size(121, 21);
            this.cmbModCompLevel.TabIndex = 15;
            this.cmbModCompLevel.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.cmbNoKeyPress);
            // 
            // lvFiles
            // 
            this.lvFiles.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
                        | System.Windows.Forms.AnchorStyles.Left)
                        | System.Windows.Forms.AnchorStyles.Right)));
            this.lvFiles.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
            this.lvFiles.ContextMenuStrip = this.FilesContextMenu;
            this.lvFiles.FullRowSelect = true;
            this.lvFiles.HideSelection = false;
            this.lvFiles.LabelEdit = true;
            this.lvFiles.Location = new System.Drawing.Point(12, 259);
            this.lvFiles.Name = "lvFiles";
            this.lvFiles.Size = new System.Drawing.Size(338, 225);
            this.lvFiles.TabIndex = 23;
            this.lvFiles.UseCompatibleStateImageBehavior = false;
            this.lvFiles.View = System.Windows.Forms.View.Details;
            this.lvFiles.AfterLabelEdit += new System.Windows.Forms.LabelEditEventHandler(this.lvFiles_AfterLabelEdit);
            this.lvFiles.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lvFiles_KeyDown);
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Relative path";
            this.columnHeader1.Width = 163;
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "File path";
            this.columnHeader2.Width = 170;
            // 
            // FilesContextMenu
            // 
            this.FilesContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.validateToolStripMenuItem,
            this.importModDetailsToolStripMenuItem,
            this.scanForDataFilesToolStripMenuItem,
            this.viewRequiredDataFilesToolStripMenuItem});
            this.FilesContextMenu.Name = "FilesContextMenu";
            this.FilesContextMenu.Size = new System.Drawing.Size(197, 136);
            this.FilesContextMenu.Opening += new System.ComponentModel.CancelEventHandler(this.FilesContextMenu_Opening);
            // 
            // openToolStripMenuItem
            // 
            this.openToolStripMenuItem.Name = "openToolStripMenuItem";
            this.openToolStripMenuItem.Size = new System.Drawing.Size(196, 22);
            this.openToolStripMenuItem.Text = "open";
            this.openToolStripMenuItem.Click += new System.EventHandler(this.openToolStripMenuItem_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(196, 22);
            this.deleteToolStripMenuItem.Text = "remove from omod";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // validateToolStripMenuItem
            // 
            this.validateToolStripMenuItem.Name = "validateToolStripMenuItem";
            this.validateToolStripMenuItem.Size = new System.Drawing.Size(196, 22);
            this.validateToolStripMenuItem.Text = "validate";
            this.validateToolStripMenuItem.Click += new System.EventHandler(this.validateToolStripMenuItem_Click);
            // 
            // importModDetailsToolStripMenuItem
            // 
            this.importModDetailsToolStripMenuItem.Name = "importModDetailsToolStripMenuItem";
            this.importModDetailsToolStripMenuItem.Size = new System.Drawing.Size(196, 22);
            this.importModDetailsToolStripMenuItem.Text = "Import mod details";
            this.importModDetailsToolStripMenuItem.Click += new System.EventHandler(this.importModDetailsToolStripMenuItem_Click);
            // 
            // scanForDataFilesToolStripMenuItem
            // 
            this.scanForDataFilesToolStripMenuItem.Name = "scanForDataFilesToolStripMenuItem";
            this.scanForDataFilesToolStripMenuItem.Size = new System.Drawing.Size(196, 22);
            this.scanForDataFilesToolStripMenuItem.Text = "scan for data files";
            this.scanForDataFilesToolStripMenuItem.Click += new System.EventHandler(this.scanForDataFilesToolStripMenuItem_Click);
            // 
            // viewRequiredDataFilesToolStripMenuItem
            // 
            this.viewRequiredDataFilesToolStripMenuItem.Name = "viewRequiredDataFilesToolStripMenuItem";
            this.viewRequiredDataFilesToolStripMenuItem.Size = new System.Drawing.Size(196, 22);
            this.viewRequiredDataFilesToolStripMenuItem.Text = "View required data files";
            this.viewRequiredDataFilesToolStripMenuItem.Click += new System.EventHandler(this.viewRequiredDataFilesToolStripMenuItem_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(139, 93);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(111, 13);
            this.label1.TabIndex = 12;
            this.label1.Text = "data files compression";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(139, 120);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(136, 13);
            this.label2.TabIndex = 14;
            this.label2.Text = "data files compression level";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(139, 147);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(120, 13);
            this.label3.TabIndex = 16;
            this.label3.Text = "omod compression level";
            // 
            // tbName
            // 
            this.tbName.Location = new System.Drawing.Point(12, 12);
            this.tbName.Name = "tbName";
            this.tbName.Size = new System.Drawing.Size(121, 20);
            this.tbName.TabIndex = 0;
            this.tbName.Text = "new mod";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(139, 15);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(35, 13);
            this.label4.TabIndex = 1;
            this.label4.Text = "Name";
            // 
            // tbVersion
            // 
            this.tbVersion.Location = new System.Drawing.Point(12, 38);
            this.tbVersion.MaxLength = 8;
            this.tbVersion.Name = "tbVersion";
            this.tbVersion.Size = new System.Drawing.Size(59, 20);
            this.tbVersion.TabIndex = 3;
            this.tbVersion.Text = "1.0";
            this.tbVersion.Leave += new System.EventHandler(this.tbVersion_Leave);
            this.tbVersion.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.DigitKeysOnly);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(77, 41);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(110, 13);
            this.label5.TabIndex = 4;
            this.label5.Text = "Version number (x.x.x)";
            // 
            // rbPlugins
            // 
            this.rbPlugins.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rbPlugins.Checked = true;
            this.rbPlugins.Location = new System.Drawing.Point(108, 493);
            this.rbPlugins.Name = "rbPlugins";
            this.rbPlugins.Size = new System.Drawing.Size(59, 17);
            this.rbPlugins.TabIndex = 24;
            this.rbPlugins.TabStop = true;
            this.rbPlugins.Text = "Plugins";
            this.rbPlugins.CheckedChanged += new System.EventHandler(this.rbCheckedChanged);
            // 
            // rbData
            // 
            this.rbData.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.rbData.Location = new System.Drawing.Point(108, 522);
            this.rbData.Name = "rbData";
            this.rbData.Size = new System.Drawing.Size(69, 17);
            this.rbData.TabIndex = 25;
            this.rbData.Text = "Data files";
            this.rbData.CheckedChanged += new System.EventHandler(this.rbCheckedChanged);
            // 
            // bEdDescription
            // 
            this.bEdDescription.Location = new System.Drawing.Point(224, 177);
            this.bEdDescription.Name = "bEdDescription";
            this.bEdDescription.Size = new System.Drawing.Size(100, 23);
            this.bEdDescription.TabIndex = 19;
            this.bEdDescription.Text = "Edit description";
            this.bEdDescription.Click += new System.EventHandler(this.bEdDescription_Click);
            // 
            // bCreate
            // 
            this.bCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bCreate.Location = new System.Drawing.Point(270, 519);
            this.bCreate.Name = "bCreate";
            this.bCreate.Size = new System.Drawing.Size(80, 23);
            this.bCreate.TabIndex = 30;
            this.bCreate.Text = "Create omod";
            this.bCreate.Click += new System.EventHandler(this.bCreate_Click);
            // 
            // OpenDialog
            // 
            this.OpenDialog.Multiselect = true;
            this.OpenDialog.RestoreDirectory = true;
            // 
            // FolderDialog
            // 
            this.FolderDialog.Description = "Select mod directory";
            this.FolderDialog.ShowNewFolderButton = false;
            // 
            // tbAuthor
            // 
            this.tbAuthor.Location = new System.Drawing.Point(191, 38);
            this.tbAuthor.Name = "tbAuthor";
            this.tbAuthor.Size = new System.Drawing.Size(116, 20);
            this.tbAuthor.TabIndex = 5;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(312, 41);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(38, 13);
            this.label6.TabIndex = 6;
            this.label6.Text = "Author";
            // 
            // tbEmail
            // 
            this.tbEmail.Location = new System.Drawing.Point(191, 64);
            this.tbEmail.Name = "tbEmail";
            this.tbEmail.Size = new System.Drawing.Size(116, 20);
            this.tbEmail.TabIndex = 9;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(313, 67);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(35, 13);
            this.label7.TabIndex = 10;
            this.label7.Text = "E-mail";
            // 
            // bUp
            // 
            this.bUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bUp.Location = new System.Drawing.Point(184, 490);
            this.bUp.Name = "bUp";
            this.bUp.Size = new System.Drawing.Size(80, 23);
            this.bUp.TabIndex = 26;
            this.bUp.Text = "Move up";
            this.bUp.Click += new System.EventHandler(this.bUp_Click);
            // 
            // bDown
            // 
            this.bDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bDown.Location = new System.Drawing.Point(270, 490);
            this.bDown.Name = "bDown";
            this.bDown.Size = new System.Drawing.Size(80, 23);
            this.bDown.TabIndex = 27;
            this.bDown.Text = "Move down";
            this.bDown.Click += new System.EventHandler(this.bDown_Click);
            // 
            // tbWebsite
            // 
            this.tbWebsite.Location = new System.Drawing.Point(12, 64);
            this.tbWebsite.Name = "tbWebsite";
            this.tbWebsite.Size = new System.Drawing.Size(121, 20);
            this.tbWebsite.TabIndex = 7;
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(139, 67);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(46, 13);
            this.label8.TabIndex = 8;
            this.label8.Text = "Website";
            // 
            // bScreenshot
            // 
            this.bScreenshot.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bScreenshot.Location = new System.Drawing.Point(12, 490);
            this.bScreenshot.Name = "bScreenshot";
            this.bScreenshot.Size = new System.Drawing.Size(90, 23);
            this.bScreenshot.TabIndex = 28;
            this.bScreenshot.Text = "Add image";
            this.bScreenshot.MouseLeave += new System.EventHandler(this.bScreenshot_MouseLeave);
            this.bScreenshot.Click += new System.EventHandler(this.bScreenshot_Click);
            this.bScreenshot.MouseEnter += new System.EventHandler(this.bScreenshot_MouseEnter);
            // 
            // ScreenshotPic
            // 
            this.ScreenshotPic.Location = new System.Drawing.Point(12, 259);
            this.ScreenshotPic.Name = "ScreenshotPic";
            this.ScreenshotPic.Size = new System.Drawing.Size(300, 225);
            this.ScreenshotPic.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.ScreenshotPic.TabIndex = 29;
            this.ScreenshotPic.TabStop = false;
            this.ScreenshotPic.Visible = false;
            // 
            // cbIncludeVersion
            // 
            this.cbIncludeVersion.AutoSize = true;
            this.cbIncludeVersion.Checked = true;
            this.cbIncludeVersion.CheckState = System.Windows.Forms.CheckState.Checked;
            this.cbIncludeVersion.Location = new System.Drawing.Point(191, 14);
            this.cbIncludeVersion.Name = "cbIncludeVersion";
            this.cbIncludeVersion.Size = new System.Drawing.Size(154, 17);
            this.cbIncludeVersion.TabIndex = 2;
            this.cbIncludeVersion.Text = "Include version in file name";
            this.cbIncludeVersion.UseVisualStyleBackColor = true;
            // 
            // bGroups
            // 
            this.bGroups.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.bGroups.Location = new System.Drawing.Point(184, 519);
            this.bGroups.Name = "bGroups";
            this.bGroups.Size = new System.Drawing.Size(80, 23);
            this.bGroups.TabIndex = 29;
            this.bGroups.Text = "Groups";
            this.bGroups.Click += new System.EventHandler(this.bGroups_Click);
            // 
            // cmsGroups
            // 
            this.cmsGroups.Name = "cmsGroups";
            this.cmsGroups.Size = new System.Drawing.Size(61, 4);
            // 
            // bRemoveScreenshot
            // 
            this.bRemoveScreenshot.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bRemoveScreenshot.Location = new System.Drawing.Point(12, 519);
            this.bRemoveScreenshot.Name = "bRemoveScreenshot";
            this.bRemoveScreenshot.Size = new System.Drawing.Size(90, 23);
            this.bRemoveScreenshot.TabIndex = 31;
            this.bRemoveScreenshot.Text = "Remove image";
            this.bRemoveScreenshot.Click += new System.EventHandler(this.bRemoveScreenshot_Click);
            // 
            // bAddZip
            // 
            this.bAddZip.Location = new System.Drawing.Point(224, 235);
            this.bAddZip.Name = "bAddZip";
            this.bAddZip.Size = new System.Drawing.Size(100, 23);
            this.bAddZip.TabIndex = 22;
            this.bAddZip.Text = "Add archive";
            this.bAddZip.Click += new System.EventHandler(this.bAddZip_Click);
            // 
            // bAddFromFolder
            // 
            this.bAddFromFolder.Location = new System.Drawing.Point(118, 235);
            this.bAddFromFolder.Name = "bAddFromFolder";
            this.bAddFromFolder.Size = new System.Drawing.Size(100, 23);
            this.bAddFromFolder.TabIndex = 21;
            this.bAddFromFolder.Text = "Add folder";
            this.bAddFromFolder.Click += new System.EventHandler(this.bAddFromFolder_Click);
            // 
            // bAdd
            // 
            this.bAdd.Location = new System.Drawing.Point(12, 235);
            this.bAdd.Name = "bAdd";
            this.bAdd.Size = new System.Drawing.Size(100, 23);
            this.bAdd.TabIndex = 20;
            this.bAdd.Text = "Add files";
            this.bAdd.Click += new System.EventHandler(this.bAdd_Click);
            // 
            // bEdCredits
            // 
            this.bEdCredits.Location = new System.Drawing.Point(224, 206);
            this.bEdCredits.Name = "bEdCredits";
            this.bEdCredits.Size = new System.Drawing.Size(100, 23);
            this.bEdCredits.TabIndex = 34;
            this.bEdCredits.Text = "Edit Credits";
            this.bEdCredits.Click += new System.EventHandler(this.button1_Click);
            // 
            // bEdChangeLog
            // 
            this.bEdChangeLog.Location = new System.Drawing.Point(12, 206);
            this.bEdChangeLog.Name = "bEdChangeLog";
            this.bEdChangeLog.Size = new System.Drawing.Size(100, 23);
            this.bEdChangeLog.TabIndex = 32;
            this.bEdChangeLog.Text = "Edit Change Log";
            // 
            // bEdComments
            // 
            this.bEdComments.Location = new System.Drawing.Point(118, 206);
            this.bEdComments.Name = "bEdComments";
            this.bEdComments.Size = new System.Drawing.Size(100, 23);
            this.bEdComments.TabIndex = 33;
            this.bEdComments.Text = "Edit Comments";
            this.bEdComments.Click += new System.EventHandler(this.bEdComments_Click);
            // 
            // CreateModForm
            // 
            this.ClientSize = new System.Drawing.Size(362, 554);
            this.Controls.Add(this.bEdCredits);
            this.Controls.Add(this.bEdChangeLog);
            this.Controls.Add(this.bEdComments);
            this.Controls.Add(this.bRemoveScreenshot);
            this.Controls.Add(this.cbIncludeVersion);
            this.Controls.Add(this.label8);
            this.Controls.Add(this.tbWebsite);
            this.Controls.Add(this.bAddZip);
            this.Controls.Add(this.bGroups);
            this.Controls.Add(this.bScreenshot);
            this.Controls.Add(this.tbAuthor);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.tbEmail);
            this.Controls.Add(this.bUp);
            this.Controls.Add(this.bEdDescription);
            this.Controls.Add(this.bDown);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.rbData);
            this.Controls.Add(this.rbPlugins);
            this.Controls.Add(this.bCreate);
            this.Controls.Add(this.ScreenshotPic);
            this.Controls.Add(this.tbVersion);
            this.Controls.Add(this.tbName);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.bAddFromFolder);
            this.Controls.Add(this.lvFiles);
            this.Controls.Add(this.bAdd);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.bEdReadme);
            this.Controls.Add(this.cmbModCompLevel);
            this.Controls.Add(this.cmbDataCompLevel);
            this.Controls.Add(this.cmbCompType);
            this.Controls.Add(this.bEdScript);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(370, 556);
            this.Name = "CreateModForm";
            this.Text = "omod creator";
            this.FilesContextMenu.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.ScreenshotPic)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button bEdReadme;
        private System.Windows.Forms.Button bEdScript;
        private System.Windows.Forms.ComboBox cmbCompType;
        private System.Windows.Forms.ComboBox cmbDataCompLevel;
        private System.Windows.Forms.ComboBox cmbModCompLevel;
        private System.Windows.Forms.ListView lvFiles;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.ColumnHeader columnHeader1;
        private System.Windows.Forms.ColumnHeader columnHeader2;
        private System.Windows.Forms.TextBox tbName;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox tbVersion;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.RadioButton rbPlugins;
        private System.Windows.Forms.RadioButton rbData;
        private System.Windows.Forms.Button bEdDescription;
        private System.Windows.Forms.Button bCreate;
        private System.Windows.Forms.OpenFileDialog OpenDialog;
        private System.Windows.Forms.FolderBrowserDialog FolderDialog;
        private System.Windows.Forms.TextBox tbAuthor;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox tbEmail;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Button bUp;
        private System.Windows.Forms.Button bDown;
        private System.Windows.Forms.TextBox tbWebsite;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.ContextMenuStrip FilesContextMenu;
        private System.Windows.Forms.ToolStripMenuItem openToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem validateToolStripMenuItem;
        private System.Windows.Forms.Button bScreenshot;
        private System.Windows.Forms.ToolStripMenuItem scanForDataFilesToolStripMenuItem;
        private System.Windows.Forms.PictureBox ScreenshotPic;
        private System.Windows.Forms.CheckBox cbIncludeVersion;
        private System.Windows.Forms.ToolStripMenuItem importModDetailsToolStripMenuItem;
        private System.Windows.Forms.Button bGroups;
        private System.Windows.Forms.ContextMenuStrip cmsGroups;
        private System.Windows.Forms.ContextMenu DudMenu=new System.Windows.Forms.ContextMenu();
        private System.Windows.Forms.ToolStripMenuItem viewRequiredDataFilesToolStripMenuItem;
        private System.Windows.Forms.Button bRemoveScreenshot;
        private System.Windows.Forms.Button bAddZip;
        private System.Windows.Forms.Button bAddFromFolder;
        private System.Windows.Forms.Button bAdd;
        private System.Windows.Forms.Button bEdCredits;
        private System.Windows.Forms.Button bEdChangeLog;
        private System.Windows.Forms.Button bEdComments;
    }
}