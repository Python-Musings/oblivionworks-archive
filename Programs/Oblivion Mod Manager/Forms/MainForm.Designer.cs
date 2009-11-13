namespace OblivionModManager {
    partial class MainForm {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.lvEspList = new System.Windows.Forms.ListView();
            this.ch1Esp = new System.Windows.Forms.ColumnHeader();
            this.ch1Belongs = new System.Windows.Forms.ColumnHeader();
            this.EspContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.moveUpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.moveDownToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.moveToTopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.moveToBottomToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.activateAllToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.deactivateAllToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.viewDataFilesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.unlinkToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.lvModList = new System.Windows.Forms.ListView();
            this.omodContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.infoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewReadmeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewScriptToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.visitWebsiteToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.emailAuthorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.viewDataConflictsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.GroupToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.GroupNoneToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aquisitionActivateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.forceDisableToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.cleanToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.convertToArchiveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.extractToFolderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportOmodConversionDataToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.hideToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ConflictImageList = new System.Windows.Forms.ImageList(this.components);
            this.bCreate = new System.Windows.Forms.Button();
            this.OpenDialog = new System.Windows.Forms.OpenFileDialog();
            this.bMoveUp = new System.Windows.Forms.Button();
            this.bMoveDown = new System.Windows.Forms.Button();
            this.bActivate = new System.Windows.Forms.Button();
            this.bLoad = new System.Windows.Forms.Button();
            this.bHelp = new System.Windows.Forms.Button();
            this.cmbEspSortOrder = new System.Windows.Forms.ComboBox();
            this.bAbout = new System.Windows.Forms.Button();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.bLaunch = new System.Windows.Forms.Button();
            this.bEdit = new System.Windows.Forms.Button();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label1 = new System.Windows.Forms.Label();
            this.bSettings = new System.Windows.Forms.Button();
            this.cmbOmodSortOrder = new System.Windows.Forms.ComboBox();
            this.cmbGroupFilter = new System.Windows.Forms.ComboBox();
            this.SaveOmodDialog = new System.Windows.Forms.SaveFileDialog();
            this.PipeFileWatcher = new System.IO.FileSystemWatcher();
            this.bUtilities = new System.Windows.Forms.Button();
            this.cmsUtilitiesMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.conflictReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.conflictReportToolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
            this.saveManagerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bSABrowserToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bSACreatorToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bSAFixerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.archiveInvalidationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dataFileBrowserToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.hiddenOmodSwitcherToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.nifViewerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.BatchContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.activateAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deactivateAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cleanAllToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.cleanFilteredToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.tidyDataFolderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.deleteBackupsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.updateConflictsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.rescanEspHeadersToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.hideInactiveFToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aquisitionActivateFilteredToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.bBatch = new System.Windows.Forms.Button();
            this.bImport = new System.Windows.Forms.Button();
            this.ImportContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.loadOrderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.omodReportToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportOmodListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportLoadOrderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exportOmodGroupInformationToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importOmodListToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importLoadOrderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.importOmodGroupInfoToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.simulateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.EspContextMenu.SuspendLayout();
            this.omodContextMenu.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.PipeFileWatcher)).BeginInit();
            this.cmsUtilitiesMenu.SuspendLayout();
            this.BatchContextMenu.SuspendLayout();
            this.ImportContextMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // lvEspList
            // 
            this.lvEspList.AllowDrop = true;
            this.lvEspList.CheckBoxes = true;
            this.lvEspList.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.ch1Esp,
            this.ch1Belongs});
            this.lvEspList.ContextMenuStrip = this.EspContextMenu;
            this.lvEspList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvEspList.HideSelection = false;
            this.lvEspList.Location = new System.Drawing.Point(0, 0);
            this.lvEspList.MultiSelect = false;
            this.lvEspList.Name = "lvEspList";
            this.lvEspList.ShowItemToolTips = true;
            this.lvEspList.Size = new System.Drawing.Size(290, 320);
            this.lvEspList.TabIndex = 0;
            this.lvEspList.UseCompatibleStateImageBehavior = false;
            this.lvEspList.View = System.Windows.Forms.View.Details;
            this.lvEspList.DragEnter += new System.Windows.Forms.DragEventHandler(this.lvEspList_DragEnter);
            this.lvEspList.DragDrop += new System.Windows.Forms.DragEventHandler(this.lvEspList_DragDrop);
            this.lvEspList.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.lvEspList_ItemCheck);
            this.lvEspList.KeyDown += new System.Windows.Forms.KeyEventHandler(this.lvEspList_KeyDown);
            this.lvEspList.GiveFeedback += new System.Windows.Forms.GiveFeedbackEventHandler(this.lvEspList_GiveFeedback);
            this.lvEspList.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.lvEspList_ItemDrag);
            // 
            // ch1Esp
            // 
            this.ch1Esp.Text = "Esp";
            this.ch1Esp.Width = 138;
            // 
            // ch1Belongs
            // 
            this.ch1Belongs.Text = "Part of";
            this.ch1Belongs.Width = 146;
            // 
            // EspContextMenu
            // 
            this.EspContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.moveUpToolStripMenuItem,
            this.moveDownToolStripMenuItem,
            this.moveToTopToolStripMenuItem,
            this.moveToBottomToolStripMenuItem,
            this.deleteToolStripMenuItem,
            this.activateAllToolStripMenuItem1,
            this.deactivateAllToolStripMenuItem1,
            this.viewDataFilesToolStripMenuItem,
            this.unlinkToolStripMenuItem});
            this.EspContextMenu.Name = "EspContextMenu";
            this.EspContextMenu.Size = new System.Drawing.Size(151, 202);
            // 
            // moveUpToolStripMenuItem
            // 
            this.moveUpToolStripMenuItem.Name = "moveUpToolStripMenuItem";
            this.moveUpToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.moveUpToolStripMenuItem.Text = "Move up";
            this.moveUpToolStripMenuItem.Click += new System.EventHandler(this.bMoveUp_Click);
            // 
            // moveDownToolStripMenuItem
            // 
            this.moveDownToolStripMenuItem.Name = "moveDownToolStripMenuItem";
            this.moveDownToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.moveDownToolStripMenuItem.Text = "Move down";
            this.moveDownToolStripMenuItem.Click += new System.EventHandler(this.bMoveDown_Click);
            // 
            // moveToTopToolStripMenuItem
            // 
            this.moveToTopToolStripMenuItem.Name = "moveToTopToolStripMenuItem";
            this.moveToTopToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.moveToTopToolStripMenuItem.Text = "Move to top";
            this.moveToTopToolStripMenuItem.Click += new System.EventHandler(this.moveToTopToolStripMenuItem_Click);
            // 
            // moveToBottomToolStripMenuItem
            // 
            this.moveToBottomToolStripMenuItem.Name = "moveToBottomToolStripMenuItem";
            this.moveToBottomToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.moveToBottomToolStripMenuItem.Text = "Move to bottom";
            this.moveToBottomToolStripMenuItem.Click += new System.EventHandler(this.moveToBottomToolStripMenuItem_Click);
            // 
            // deleteToolStripMenuItem
            // 
            this.deleteToolStripMenuItem.Name = "deleteToolStripMenuItem";
            this.deleteToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.deleteToolStripMenuItem.Text = "Delete";
            this.deleteToolStripMenuItem.Click += new System.EventHandler(this.deleteToolStripMenuItem_Click);
            // 
            // activateAllToolStripMenuItem1
            // 
            this.activateAllToolStripMenuItem1.Name = "activateAllToolStripMenuItem1";
            this.activateAllToolStripMenuItem1.Size = new System.Drawing.Size(150, 22);
            this.activateAllToolStripMenuItem1.Text = "Activate all";
            this.activateAllToolStripMenuItem1.Click += new System.EventHandler(this.activateAllToolStripMenuItem1_Click);
            // 
            // deactivateAllToolStripMenuItem1
            // 
            this.deactivateAllToolStripMenuItem1.Name = "deactivateAllToolStripMenuItem1";
            this.deactivateAllToolStripMenuItem1.Size = new System.Drawing.Size(150, 22);
            this.deactivateAllToolStripMenuItem1.Text = "Deactivate all";
            this.deactivateAllToolStripMenuItem1.Click += new System.EventHandler(this.deactivateAllToolStripMenuItem1_Click);
            // 
            // viewDataFilesToolStripMenuItem
            // 
            this.viewDataFilesToolStripMenuItem.Name = "viewDataFilesToolStripMenuItem";
            this.viewDataFilesToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.viewDataFilesToolStripMenuItem.Text = "View data files";
            this.viewDataFilesToolStripMenuItem.Click += new System.EventHandler(this.viewDataFilesToolStripMenuItem_Click);
            // 
            // unlinkToolStripMenuItem
            // 
            this.unlinkToolStripMenuItem.Name = "unlinkToolStripMenuItem";
            this.unlinkToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.unlinkToolStripMenuItem.Text = "Unlink";
            this.unlinkToolStripMenuItem.Click += new System.EventHandler(this.unlinkToolStripMenuItem_Click);
            // 
            // lvModList
            // 
            this.lvModList.Alignment = System.Windows.Forms.ListViewAlignment.Left;
            this.lvModList.ContextMenuStrip = this.omodContextMenu;
            this.lvModList.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lvModList.HideSelection = false;
            this.lvModList.Location = new System.Drawing.Point(0, 0);
            this.lvModList.MultiSelect = false;
            this.lvModList.Name = "lvModList";
            this.lvModList.ShowItemToolTips = true;
            this.lvModList.Size = new System.Drawing.Size(321, 320);
            this.lvModList.SmallImageList = this.ConflictImageList;
            this.lvModList.TabIndex = 0;
            this.lvModList.UseCompatibleStateImageBehavior = false;
            this.lvModList.View = System.Windows.Forms.View.List;
            this.lvModList.ItemActivate += new System.EventHandler(this.lvModList_ItemActivate);
            this.lvModList.SelectedIndexChanged += new System.EventHandler(this.lvModList_SelectedIndexChanged);
            // 
            // omodContextMenu
            // 
            this.omodContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.editToolStripMenuItem,
            this.infoToolStripMenuItem,
            this.viewReadmeToolStripMenuItem,
            this.viewScriptToolStripMenuItem,
            this.visitWebsiteToolStripMenuItem,
            this.emailAuthorToolStripMenuItem,
            this.viewDataConflictsToolStripMenuItem,
            this.GroupToolStripMenuItem,
            this.aquisitionActivateToolStripMenuItem,
            this.simulateToolStripMenuItem,
            this.forceDisableToolStripMenuItem,
            this.deleteToolStripMenuItem1,
            this.cleanToolStripMenuItem,
            this.convertToArchiveToolStripMenuItem,
            this.extractToFolderToolStripMenuItem,
            this.exportOmodConversionDataToolStripMenuItem,
            this.hideToolStripMenuItem});
            this.omodContextMenu.Name = "omodContextMenu";
            this.omodContextMenu.Size = new System.Drawing.Size(216, 400);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.editToolStripMenuItem.Text = "Edit";
            this.editToolStripMenuItem.Click += new System.EventHandler(this.editToolStripMenuItem_Click);
            // 
            // infoToolStripMenuItem
            // 
            this.infoToolStripMenuItem.Name = "infoToolStripMenuItem";
            this.infoToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.infoToolStripMenuItem.Text = "Info";
            this.infoToolStripMenuItem.Click += new System.EventHandler(this.infoToolStripMenuItem_Click);
            // 
            // viewReadmeToolStripMenuItem
            // 
            this.viewReadmeToolStripMenuItem.Name = "viewReadmeToolStripMenuItem";
            this.viewReadmeToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.viewReadmeToolStripMenuItem.Text = "View readme";
            this.viewReadmeToolStripMenuItem.Click += new System.EventHandler(this.viewReadmeToolStripMenuItem_Click);
            // 
            // viewScriptToolStripMenuItem
            // 
            this.viewScriptToolStripMenuItem.Name = "viewScriptToolStripMenuItem";
            this.viewScriptToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.viewScriptToolStripMenuItem.Text = "View script";
            this.viewScriptToolStripMenuItem.Click += new System.EventHandler(this.viewScriptToolStripMenuItem_Click);
            // 
            // visitWebsiteToolStripMenuItem
            // 
            this.visitWebsiteToolStripMenuItem.Name = "visitWebsiteToolStripMenuItem";
            this.visitWebsiteToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.visitWebsiteToolStripMenuItem.Text = "Visit website";
            this.visitWebsiteToolStripMenuItem.Click += new System.EventHandler(this.visitWebsiteToolStripMenuItem_Click);
            // 
            // emailAuthorToolStripMenuItem
            // 
            this.emailAuthorToolStripMenuItem.Name = "emailAuthorToolStripMenuItem";
            this.emailAuthorToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.emailAuthorToolStripMenuItem.Text = "Email author";
            this.emailAuthorToolStripMenuItem.Click += new System.EventHandler(this.emailAuthorToolStripMenuItem_Click);
            // 
            // viewDataConflictsToolStripMenuItem
            // 
            this.viewDataConflictsToolStripMenuItem.Name = "viewDataConflictsToolStripMenuItem";
            this.viewDataConflictsToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.viewDataConflictsToolStripMenuItem.Text = "View data conflicts";
            this.viewDataConflictsToolStripMenuItem.Click += new System.EventHandler(this.viewDataConflictsToolStripMenuItem_Click);
            // 
            // GroupToolStripMenuItem
            // 
            this.GroupToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.GroupNoneToolStripMenuItem});
            this.GroupToolStripMenuItem.Name = "GroupToolStripMenuItem";
            this.GroupToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.GroupToolStripMenuItem.Text = "Group";
            // 
            // GroupNoneToolStripMenuItem
            // 
            this.GroupNoneToolStripMenuItem.Name = "GroupNoneToolStripMenuItem";
            this.GroupNoneToolStripMenuItem.Size = new System.Drawing.Size(99, 22);
            this.GroupNoneToolStripMenuItem.Text = "None";
            this.GroupNoneToolStripMenuItem.Click += new System.EventHandler(this.GroupNoneToolStripMenuItem_Click);
            // 
            // aquisitionActivateToolStripMenuItem
            // 
            this.aquisitionActivateToolStripMenuItem.Name = "aquisitionActivateToolStripMenuItem";
            this.aquisitionActivateToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.aquisitionActivateToolStripMenuItem.Text = "Acquisition activate";
            this.aquisitionActivateToolStripMenuItem.Click += new System.EventHandler(this.aquisitionActivateToolStripMenuItem_Click);
            // 
            // forceDisableToolStripMenuItem
            // 
            this.forceDisableToolStripMenuItem.Name = "forceDisableToolStripMenuItem";
            this.forceDisableToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.forceDisableToolStripMenuItem.Text = "Force disable";
            this.forceDisableToolStripMenuItem.Click += new System.EventHandler(this.forceDisableToolStripMenuItem_Click);
            // 
            // deleteToolStripMenuItem1
            // 
            this.deleteToolStripMenuItem1.Name = "deleteToolStripMenuItem1";
            this.deleteToolStripMenuItem1.Size = new System.Drawing.Size(215, 22);
            this.deleteToolStripMenuItem1.Text = "Delete";
            this.deleteToolStripMenuItem1.Click += new System.EventHandler(this.deleteToolStripMenuItem1_Click);
            // 
            // cleanToolStripMenuItem
            // 
            this.cleanToolStripMenuItem.Name = "cleanToolStripMenuItem";
            this.cleanToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.cleanToolStripMenuItem.Text = "Clean";
            this.cleanToolStripMenuItem.Click += new System.EventHandler(this.cleanToolStripMenuItem_Click);
            // 
            // convertToArchiveToolStripMenuItem
            // 
            this.convertToArchiveToolStripMenuItem.Name = "convertToArchiveToolStripMenuItem";
            this.convertToArchiveToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.convertToArchiveToolStripMenuItem.Text = "Convert to archive";
            this.convertToArchiveToolStripMenuItem.Click += new System.EventHandler(this.convertToArchiveToolStripMenuItem_Click);
            // 
            // extractToFolderToolStripMenuItem
            // 
            this.extractToFolderToolStripMenuItem.Name = "extractToFolderToolStripMenuItem";
            this.extractToFolderToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.extractToFolderToolStripMenuItem.Text = "Extract to folder";
            this.extractToFolderToolStripMenuItem.Click += new System.EventHandler(this.extractToFolderToolStripMenuItem_Click);
            // 
            // exportOmodConversionDataToolStripMenuItem
            // 
            this.exportOmodConversionDataToolStripMenuItem.Name = "exportOmodConversionDataToolStripMenuItem";
            this.exportOmodConversionDataToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.exportOmodConversionDataToolStripMenuItem.Text = "Export omod conversion data";
            this.exportOmodConversionDataToolStripMenuItem.Click += new System.EventHandler(this.exportOmodConversionDataToolStripMenuItem_Click);
            // 
            // hideToolStripMenuItem
            // 
            this.hideToolStripMenuItem.Name = "hideToolStripMenuItem";
            this.hideToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.hideToolStripMenuItem.Text = "Hide";
            this.hideToolStripMenuItem.Click += new System.EventHandler(this.hideToolStripMenuItem_Click);
            // 
            // ConflictImageList
            // 
            this.ConflictImageList.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ConflictImageList.ImageStream")));
            this.ConflictImageList.TransparentColor = System.Drawing.Color.Transparent;
            this.ConflictImageList.Images.SetKeyName(0, "blue.bmp");
            this.ConflictImageList.Images.SetKeyName(1, "green.bmp");
            this.ConflictImageList.Images.SetKeyName(2, "orange.bmp");
            this.ConflictImageList.Images.SetKeyName(3, "red.bmp");
            this.ConflictImageList.Images.SetKeyName(4, "black.bmp");
            // 
            // bCreate
            // 
            this.bCreate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bCreate.Location = new System.Drawing.Point(387, 338);
            this.bCreate.Name = "bCreate";
            this.bCreate.Size = new System.Drawing.Size(75, 23);
            this.bCreate.TabIndex = 6;
            this.bCreate.Text = "Create";
            this.bCreate.Click += new System.EventHandler(this.bCreate_Click);
            // 
            // OpenDialog
            // 
            this.OpenDialog.Filter = "Oblivion mod file (*.omod)|*.omod";
            this.OpenDialog.Multiselect = true;
            this.OpenDialog.RestoreDirectory = true;
            this.OpenDialog.Title = "Select mod files to load";
            // 
            // bMoveUp
            // 
            this.bMoveUp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bMoveUp.Location = new System.Drawing.Point(12, 338);
            this.bMoveUp.Name = "bMoveUp";
            this.bMoveUp.Size = new System.Drawing.Size(80, 23);
            this.bMoveUp.TabIndex = 1;
            this.bMoveUp.Text = "Move up";
            this.bMoveUp.Click += new System.EventHandler(this.bMoveUp_Click);
            // 
            // bMoveDown
            // 
            this.bMoveDown.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bMoveDown.Location = new System.Drawing.Point(98, 338);
            this.bMoveDown.Name = "bMoveDown";
            this.bMoveDown.Size = new System.Drawing.Size(80, 23);
            this.bMoveDown.TabIndex = 2;
            this.bMoveDown.Text = "Move down";
            this.bMoveDown.Click += new System.EventHandler(this.bMoveDown_Click);
            // 
            // bActivate
            // 
            this.bActivate.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bActivate.Location = new System.Drawing.Point(306, 338);
            this.bActivate.Name = "bActivate";
            this.bActivate.Size = new System.Drawing.Size(75, 23);
            this.bActivate.TabIndex = 5;
            this.bActivate.Text = "Activate";
            this.bActivate.Click += new System.EventHandler(this.bActivate_Click);
            // 
            // bLoad
            // 
            this.bLoad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bLoad.Location = new System.Drawing.Point(549, 338);
            this.bLoad.Name = "bLoad";
            this.bLoad.Size = new System.Drawing.Size(75, 23);
            this.bLoad.TabIndex = 8;
            this.bLoad.Text = "Load";
            this.bLoad.Click += new System.EventHandler(this.bLoad_Click);
            // 
            // bHelp
            // 
            this.bHelp.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bHelp.Location = new System.Drawing.Point(633, 137);
            this.bHelp.Name = "bHelp";
            this.bHelp.Size = new System.Drawing.Size(120, 23);
            this.bHelp.TabIndex = 10;
            this.bHelp.Text = "Help";
            this.bHelp.Click += new System.EventHandler(this.bHelp_Click);
            // 
            // cmbEspSortOrder
            // 
            this.cmbEspSortOrder.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.cmbEspSortOrder.FormattingEnabled = true;
            this.cmbEspSortOrder.Items.AddRange(new object[] {
            "Load order",
            "Alphabetic",
            "Active",
            "Owner",
            "File size"});
            this.cmbEspSortOrder.Location = new System.Drawing.Point(184, 340);
            this.cmbEspSortOrder.Name = "cmbEspSortOrder";
            this.cmbEspSortOrder.Size = new System.Drawing.Size(95, 21);
            this.cmbEspSortOrder.TabIndex = 3;
            this.cmbEspSortOrder.Text = "Load order";
            this.cmbEspSortOrder.SelectedIndexChanged += new System.EventHandler(this.cmbEspSortOrder_SelectedIndexChanged);
            this.cmbEspSortOrder.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKeyPress);
            // 
            // bAbout
            // 
            this.bAbout.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bAbout.Location = new System.Drawing.Point(633, 166);
            this.bAbout.Name = "bAbout";
            this.bAbout.Size = new System.Drawing.Size(120, 23);
            this.bAbout.TabIndex = 11;
            this.bAbout.Text = "About";
            this.bAbout.Click += new System.EventHandler(this.bAbout_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.splitContainer1.Location = new System.Drawing.Point(12, 12);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.lvEspList);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.lvModList);
            this.splitContainer1.Size = new System.Drawing.Size(615, 320);
            this.splitContainer1.SplitterDistance = 290;
            this.splitContainer1.TabIndex = 0;
            // 
            // bLaunch
            // 
            this.bLaunch.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bLaunch.Location = new System.Drawing.Point(633, 108);
            this.bLaunch.Name = "bLaunch";
            this.bLaunch.Size = new System.Drawing.Size(120, 23);
            this.bLaunch.TabIndex = 9;
            this.bLaunch.Text = "Launch Oblivion";
            this.bLaunch.Click += new System.EventHandler(this.bLaunch_Click);
            // 
            // bEdit
            // 
            this.bEdit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.bEdit.Location = new System.Drawing.Point(468, 338);
            this.bEdit.Name = "bEdit";
            this.bEdit.Size = new System.Drawing.Size(75, 23);
            this.bEdit.TabIndex = 7;
            this.bEdit.Text = "Edit";
            this.bEdit.Click += new System.EventHandler(this.bEdit_Click);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pictureBox1.Location = new System.Drawing.Point(633, 12);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(120, 90);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 13;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(290, 343);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(10, 13);
            this.label1.TabIndex = 4;
            this.label1.Text = "-";
            // 
            // bSettings
            // 
            this.bSettings.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bSettings.Location = new System.Drawing.Point(633, 195);
            this.bSettings.Name = "bSettings";
            this.bSettings.Size = new System.Drawing.Size(120, 23);
            this.bSettings.TabIndex = 12;
            this.bSettings.Text = "Settings";
            this.bSettings.Click += new System.EventHandler(this.bSettings_Click);
            // 
            // cmbOmodSortOrder
            // 
            this.cmbOmodSortOrder.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbOmodSortOrder.FormattingEnabled = true;
            this.cmbOmodSortOrder.Items.AddRange(new object[] {
            "File name",
            "Author",
            "Conflict level",
            "File size",
            "Date packed",
            "Date installed",
            "Group"});
            this.cmbOmodSortOrder.Location = new System.Drawing.Point(633, 340);
            this.cmbOmodSortOrder.Name = "cmbOmodSortOrder";
            this.cmbOmodSortOrder.Size = new System.Drawing.Size(120, 21);
            this.cmbOmodSortOrder.TabIndex = 17;
            this.cmbOmodSortOrder.Text = "File name";
            this.cmbOmodSortOrder.SelectedIndexChanged += new System.EventHandler(this.cmbOmodSortOrder_SelectedIndexChanged);
            this.cmbOmodSortOrder.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKeyPress);
            // 
            // cmbGroupFilter
            // 
            this.cmbGroupFilter.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.cmbGroupFilter.FormattingEnabled = true;
            this.cmbGroupFilter.Items.AddRange(new object[] {
            "All",
            "Unassigned",
            "[Edit groups]"});
            this.cmbGroupFilter.Location = new System.Drawing.Point(633, 313);
            this.cmbGroupFilter.Name = "cmbGroupFilter";
            this.cmbGroupFilter.Size = new System.Drawing.Size(120, 21);
            this.cmbGroupFilter.TabIndex = 16;
            this.cmbGroupFilter.Text = "All";
            this.cmbGroupFilter.SelectedIndexChanged += new System.EventHandler(this.cmbGroupFilter_SelectedIndexChanged);
            this.cmbGroupFilter.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.IgnoreKeyPress);
            // 
            // SaveOmodDialog
            // 
            this.SaveOmodDialog.AddExtension = false;
            this.SaveOmodDialog.Filter = "7-zip (*.7z)|*.7z|zip (*.zip)|*.zip";
            this.SaveOmodDialog.OverwritePrompt = false;
            this.SaveOmodDialog.RestoreDirectory = true;
            this.SaveOmodDialog.Title = "Save omod as";
            // 
            // PipeFileWatcher
            // 
            this.PipeFileWatcher.EnableRaisingEvents = true;
            this.PipeFileWatcher.Filter = "pipe";
            this.PipeFileWatcher.Path = "obmm";
            this.PipeFileWatcher.SynchronizingObject = this;
            this.PipeFileWatcher.Created += new System.IO.FileSystemEventHandler(this.PipeFileWatcher_Created);
            // 
            // bUtilities
            // 
            this.bUtilities.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bUtilities.Location = new System.Drawing.Point(633, 224);
            this.bUtilities.Name = "bUtilities";
            this.bUtilities.Size = new System.Drawing.Size(120, 23);
            this.bUtilities.TabIndex = 13;
            this.bUtilities.Text = "Utilities";
            this.bUtilities.UseVisualStyleBackColor = true;
            this.bUtilities.Click += new System.EventHandler(this.bUtilities_Click);
            // 
            // cmsUtilitiesMenu
            // 
            this.cmsUtilitiesMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.conflictReportToolStripMenuItem,
            this.conflictReportToolStripMenuItem1,
            this.saveManagerToolStripMenuItem,
            this.bSABrowserToolStripMenuItem,
            this.bSACreatorToolStripMenuItem,
            this.bSAFixerToolStripMenuItem,
            this.archiveInvalidationToolStripMenuItem,
            this.dataFileBrowserToolStripMenuItem,
            this.hiddenOmodSwitcherToolStripMenuItem,
            this.nifViewerToolStripMenuItem});
            this.cmsUtilitiesMenu.Name = "cmsUtilitiesMenu";
            this.cmsUtilitiesMenu.Size = new System.Drawing.Size(180, 224);
            // 
            // conflictReportToolStripMenuItem
            // 
            this.conflictReportToolStripMenuItem.Name = "conflictReportToolStripMenuItem";
            this.conflictReportToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.conflictReportToolStripMenuItem.Text = "Old conflict detector";
            this.conflictReportToolStripMenuItem.Click += new System.EventHandler(this.bConflict_Click);
            // 
            // conflictReportToolStripMenuItem1
            // 
            this.conflictReportToolStripMenuItem1.Name = "conflictReportToolStripMenuItem1";
            this.conflictReportToolStripMenuItem1.Size = new System.Drawing.Size(179, 22);
            this.conflictReportToolStripMenuItem1.Text = "New conflict detector";
            this.conflictReportToolStripMenuItem1.Click += new System.EventHandler(this.conflictReportToolStripMenuItem1_Click);
            // 
            // saveManagerToolStripMenuItem
            // 
            this.saveManagerToolStripMenuItem.Name = "saveManagerToolStripMenuItem";
            this.saveManagerToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.saveManagerToolStripMenuItem.Text = "Save manager";
            this.saveManagerToolStripMenuItem.Click += new System.EventHandler(this.bSaves_Click);
            // 
            // bSABrowserToolStripMenuItem
            // 
            this.bSABrowserToolStripMenuItem.Name = "bSABrowserToolStripMenuItem";
            this.bSABrowserToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.bSABrowserToolStripMenuItem.Text = "BSA browser";
            this.bSABrowserToolStripMenuItem.Click += new System.EventHandler(this.bSABrowserToolStripMenuItem_Click);
            // 
            // bSACreatorToolStripMenuItem
            // 
            this.bSACreatorToolStripMenuItem.Name = "bSACreatorToolStripMenuItem";
            this.bSACreatorToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.bSACreatorToolStripMenuItem.Text = "BSA creator";
            this.bSACreatorToolStripMenuItem.Click += new System.EventHandler(this.bSACreatorToolStripMenuItem_Click);
            // 
            // bSAFixerToolStripMenuItem
            // 
            this.bSAFixerToolStripMenuItem.Name = "bSAFixerToolStripMenuItem";
            this.bSAFixerToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.bSAFixerToolStripMenuItem.Text = "BSA un-corrupter";
            this.bSAFixerToolStripMenuItem.Click += new System.EventHandler(this.bSAFixerToolStripMenuItem_Click);
            // 
            // archiveInvalidationToolStripMenuItem
            // 
            this.archiveInvalidationToolStripMenuItem.Name = "archiveInvalidationToolStripMenuItem";
            this.archiveInvalidationToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.archiveInvalidationToolStripMenuItem.Text = "Archive invalidation";
            this.archiveInvalidationToolStripMenuItem.Click += new System.EventHandler(this.archiveInvalidationToolStripMenuItem_Click);
            // 
            // dataFileBrowserToolStripMenuItem
            // 
            this.dataFileBrowserToolStripMenuItem.Name = "dataFileBrowserToolStripMenuItem";
            this.dataFileBrowserToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.dataFileBrowserToolStripMenuItem.Text = "Data file browser";
            this.dataFileBrowserToolStripMenuItem.Click += new System.EventHandler(this.dataFileBrowserToolStripMenuItem_Click);
            // 
            // hiddenOmodSwitcherToolStripMenuItem
            // 
            this.hiddenOmodSwitcherToolStripMenuItem.Name = "hiddenOmodSwitcherToolStripMenuItem";
            this.hiddenOmodSwitcherToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.hiddenOmodSwitcherToolStripMenuItem.Text = "Hidden omod switcher";
            this.hiddenOmodSwitcherToolStripMenuItem.Click += new System.EventHandler(this.hiddenOmodSwitcherToolStripMenuItem_Click);
            // 
            // nifViewerToolStripMenuItem
            // 
            this.nifViewerToolStripMenuItem.Name = "nifViewerToolStripMenuItem";
            this.nifViewerToolStripMenuItem.Size = new System.Drawing.Size(179, 22);
            this.nifViewerToolStripMenuItem.Text = "Nif viewer";
            this.nifViewerToolStripMenuItem.Click += new System.EventHandler(this.nifViewerToolStripMenuItem_Click);
            // 
            // BatchContextMenu
            // 
            this.BatchContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.activateAllToolStripMenuItem,
            this.deactivateAllToolStripMenuItem,
            this.cleanAllToolStripMenuItem,
            this.cleanFilteredToolStripMenuItem,
            this.tidyDataFolderToolStripMenuItem,
            this.deleteBackupsToolStripMenuItem,
            this.updateConflictsToolStripMenuItem,
            this.rescanEspHeadersToolStripMenuItem,
            this.hideInactiveFToolStripMenuItem,
            this.aquisitionActivateFilteredToolStripMenuItem});
            this.BatchContextMenu.Name = "ActivateContextMenu";
            this.BatchContextMenu.Size = new System.Drawing.Size(205, 224);
            // 
            // activateAllToolStripMenuItem
            // 
            this.activateAllToolStripMenuItem.Name = "activateAllToolStripMenuItem";
            this.activateAllToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.activateAllToolStripMenuItem.Text = "Activate filtered";
            this.activateAllToolStripMenuItem.Click += new System.EventHandler(this.activateAllToolStripMenuItem_Click);
            // 
            // deactivateAllToolStripMenuItem
            // 
            this.deactivateAllToolStripMenuItem.Name = "deactivateAllToolStripMenuItem";
            this.deactivateAllToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.deactivateAllToolStripMenuItem.Text = "Deactivate filtered";
            this.deactivateAllToolStripMenuItem.Click += new System.EventHandler(this.deactivateAllToolStripMenuItem_Click);
            // 
            // cleanAllToolStripMenuItem
            // 
            this.cleanAllToolStripMenuItem.Name = "cleanAllToolStripMenuItem";
            this.cleanAllToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.cleanAllToolStripMenuItem.Text = "Deactivate and clean all";
            this.cleanAllToolStripMenuItem.Click += new System.EventHandler(this.cleanAllToolStripMenuItem_Click);
            // 
            // cleanFilteredToolStripMenuItem
            // 
            this.cleanFilteredToolStripMenuItem.Name = "cleanFilteredToolStripMenuItem";
            this.cleanFilteredToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.cleanFilteredToolStripMenuItem.Text = "Clean filtered";
            this.cleanFilteredToolStripMenuItem.Click += new System.EventHandler(this.cleanFilteredToolStripMenuItem_Click);
            // 
            // tidyDataFolderToolStripMenuItem
            // 
            this.tidyDataFolderToolStripMenuItem.Name = "tidyDataFolderToolStripMenuItem";
            this.tidyDataFolderToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.tidyDataFolderToolStripMenuItem.Text = "Tidy data folder";
            this.tidyDataFolderToolStripMenuItem.Click += new System.EventHandler(this.tidyDataFolderToolStripMenuItem_Click);
            // 
            // deleteBackupsToolStripMenuItem
            // 
            this.deleteBackupsToolStripMenuItem.Name = "deleteBackupsToolStripMenuItem";
            this.deleteBackupsToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.deleteBackupsToolStripMenuItem.Text = "Delete backups";
            this.deleteBackupsToolStripMenuItem.Click += new System.EventHandler(this.deleteBackupsToolStripMenuItem_Click);
            // 
            // updateConflictsToolStripMenuItem
            // 
            this.updateConflictsToolStripMenuItem.Name = "updateConflictsToolStripMenuItem";
            this.updateConflictsToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.updateConflictsToolStripMenuItem.Text = "Update conflicts";
            this.updateConflictsToolStripMenuItem.Click += new System.EventHandler(this.updateConflictsToolStripMenuItem_Click);
            // 
            // rescanEspHeadersToolStripMenuItem
            // 
            this.rescanEspHeadersToolStripMenuItem.Name = "rescanEspHeadersToolStripMenuItem";
            this.rescanEspHeadersToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.rescanEspHeadersToolStripMenuItem.Text = "Rescan esp headers";
            this.rescanEspHeadersToolStripMenuItem.Click += new System.EventHandler(this.rescanEspHeadersToolStripMenuItem_Click);
            // 
            // hideInactiveFToolStripMenuItem
            // 
            this.hideInactiveFToolStripMenuItem.Name = "hideInactiveFToolStripMenuItem";
            this.hideInactiveFToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.hideInactiveFToolStripMenuItem.Text = "Hide inactive filtered";
            this.hideInactiveFToolStripMenuItem.Click += new System.EventHandler(this.hideInactiveFToolStripMenuItem_Click);
            // 
            // aquisitionActivateFilteredToolStripMenuItem
            // 
            this.aquisitionActivateFilteredToolStripMenuItem.Name = "aquisitionActivateFilteredToolStripMenuItem";
            this.aquisitionActivateFilteredToolStripMenuItem.Size = new System.Drawing.Size(204, 22);
            this.aquisitionActivateFilteredToolStripMenuItem.Text = "Acquisition activate filtered";
            this.aquisitionActivateFilteredToolStripMenuItem.Click += new System.EventHandler(this.aquisitionActivateFilteredToolStripMenuItem_Click);
            // 
            // bBatch
            // 
            this.bBatch.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bBatch.Location = new System.Drawing.Point(633, 253);
            this.bBatch.Name = "bBatch";
            this.bBatch.Size = new System.Drawing.Size(120, 23);
            this.bBatch.TabIndex = 18;
            this.bBatch.Text = "Batch actions";
            this.bBatch.Click += new System.EventHandler(this.bBatch_Click);
            // 
            // bImport
            // 
            this.bImport.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.bImport.Location = new System.Drawing.Point(633, 282);
            this.bImport.Name = "bImport";
            this.bImport.Size = new System.Drawing.Size(120, 23);
            this.bImport.TabIndex = 19;
            this.bImport.Text = "Import/Export";
            this.bImport.UseVisualStyleBackColor = true;
            this.bImport.Click += new System.EventHandler(this.bImport_Click);
            // 
            // ImportContextMenu
            // 
            this.ImportContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.loadOrderToolStripMenuItem,
            this.omodReportToolStripMenuItem,
            this.exportOmodListToolStripMenuItem,
            this.exportLoadOrderToolStripMenuItem,
            this.exportOmodGroupInformationToolStripMenuItem,
            this.importOmodListToolStripMenuItem,
            this.importLoadOrderToolStripMenuItem,
            this.importOmodGroupInfoToolStripMenuItem});
            this.ImportContextMenu.Name = "ImportContextMenu";
            this.ImportContextMenu.Size = new System.Drawing.Size(188, 180);
            // 
            // loadOrderToolStripMenuItem
            // 
            this.loadOrderToolStripMenuItem.Name = "loadOrderToolStripMenuItem";
            this.loadOrderToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.loadOrderToolStripMenuItem.Text = "View load order";
            this.loadOrderToolStripMenuItem.Click += new System.EventHandler(this.loadOrderToolStripMenuItem_Click);
            // 
            // omodReportToolStripMenuItem
            // 
            this.omodReportToolStripMenuItem.Name = "omodReportToolStripMenuItem";
            this.omodReportToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.omodReportToolStripMenuItem.Text = "Create omod report";
            this.omodReportToolStripMenuItem.Click += new System.EventHandler(this.omodReportToolStripMenuItem_Click);
            // 
            // exportOmodListToolStripMenuItem
            // 
            this.exportOmodListToolStripMenuItem.Name = "exportOmodListToolStripMenuItem";
            this.exportOmodListToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.exportOmodListToolStripMenuItem.Text = "Export active omod list";
            this.exportOmodListToolStripMenuItem.Click += new System.EventHandler(this.exportOmodListToolStripMenuItem_Click);
            // 
            // exportLoadOrderToolStripMenuItem
            // 
            this.exportLoadOrderToolStripMenuItem.Name = "exportLoadOrderToolStripMenuItem";
            this.exportLoadOrderToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.exportLoadOrderToolStripMenuItem.Text = "Export load order";
            this.exportLoadOrderToolStripMenuItem.Click += new System.EventHandler(this.exportLoadOrderToolStripMenuItem_Click);
            // 
            // exportOmodGroupInformationToolStripMenuItem
            // 
            this.exportOmodGroupInformationToolStripMenuItem.Name = "exportOmodGroupInformationToolStripMenuItem";
            this.exportOmodGroupInformationToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.exportOmodGroupInformationToolStripMenuItem.Text = "Export omod group info";
            this.exportOmodGroupInformationToolStripMenuItem.Click += new System.EventHandler(this.exportOmodGroupInformationToolStripMenuItem_Click);
            // 
            // importOmodListToolStripMenuItem
            // 
            this.importOmodListToolStripMenuItem.Name = "importOmodListToolStripMenuItem";
            this.importOmodListToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.importOmodListToolStripMenuItem.Text = "Import active omod list";
            this.importOmodListToolStripMenuItem.Click += new System.EventHandler(this.importOmodListToolStripMenuItem_Click);
            // 
            // importLoadOrderToolStripMenuItem
            // 
            this.importLoadOrderToolStripMenuItem.Name = "importLoadOrderToolStripMenuItem";
            this.importLoadOrderToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.importLoadOrderToolStripMenuItem.Text = "Import load order";
            this.importLoadOrderToolStripMenuItem.Click += new System.EventHandler(this.importLoadOrderToolStripMenuItem_Click);
            // 
            // importOmodGroupInfoToolStripMenuItem
            // 
            this.importOmodGroupInfoToolStripMenuItem.Name = "importOmodGroupInfoToolStripMenuItem";
            this.importOmodGroupInfoToolStripMenuItem.Size = new System.Drawing.Size(187, 22);
            this.importOmodGroupInfoToolStripMenuItem.Text = "Import omod group info";
            this.importOmodGroupInfoToolStripMenuItem.Click += new System.EventHandler(this.importOmodGroupInfoToolStripMenuItem_Click);
            // 
            // simulateToolStripMenuItem
            // 
            this.simulateToolStripMenuItem.Name = "simulateToolStripMenuItem";
            this.simulateToolStripMenuItem.Size = new System.Drawing.Size(215, 22);
            this.simulateToolStripMenuItem.Text = "Simulate";
            this.simulateToolStripMenuItem.Click += new System.EventHandler(this.simulateToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.ClientSize = new System.Drawing.Size(765, 373);
            this.Controls.Add(this.bImport);
            this.Controls.Add(this.bBatch);
            this.Controls.Add(this.cmbGroupFilter);
            this.Controls.Add(this.bUtilities);
            this.Controls.Add(this.cmbOmodSortOrder);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.cmbEspSortOrder);
            this.Controls.Add(this.bEdit);
            this.Controls.Add(this.bSettings);
            this.Controls.Add(this.bCreate);
            this.Controls.Add(this.bLoad);
            this.Controls.Add(this.bLaunch);
            this.Controls.Add(this.bAbout);
            this.Controls.Add(this.bHelp);
            this.Controls.Add(this.bActivate);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.bMoveDown);
            this.Controls.Add(this.bMoveUp);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(773, 400);
            this.Name = "MainForm";
            this.Text = "Oblivion Mod Manager";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.EspContextMenu.ResumeLayout(false);
            this.omodContextMenu.ResumeLayout(false);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.PipeFileWatcher)).EndInit();
            this.cmsUtilitiesMenu.ResumeLayout(false);
            this.BatchContextMenu.ResumeLayout(false);
            this.ImportContextMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ListView lvEspList;
        private System.Windows.Forms.ListView lvModList;
        private System.Windows.Forms.ColumnHeader ch1Esp;
        private System.Windows.Forms.ColumnHeader ch1Belongs;
        private System.Windows.Forms.Button bCreate;
        private System.Windows.Forms.OpenFileDialog OpenDialog;
        private System.Windows.Forms.Button bMoveUp;
        private System.Windows.Forms.Button bMoveDown;
        private System.Windows.Forms.Button bActivate;
        private System.Windows.Forms.Button bLoad;
        private System.Windows.Forms.ContextMenuStrip EspContextMenu;
        private System.Windows.Forms.ContextMenuStrip omodContextMenu;
        private System.Windows.Forms.ToolStripMenuItem moveUpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem moveDownToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cleanToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewReadmeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewScriptToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem visitWebsiteToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem emailAuthorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem infoToolStripMenuItem;
        private System.Windows.Forms.ImageList ConflictImageList;
        private System.Windows.Forms.ComboBox cmbEspSortOrder;
        private System.Windows.Forms.Button bAbout;
        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Button bEdit;
        private System.Windows.Forms.Button bLaunch;
        internal System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button bSettings;
        private System.Windows.Forms.ComboBox cmbOmodSortOrder;
        private System.Windows.Forms.ToolStripMenuItem GroupToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem GroupNoneToolStripMenuItem;
        private System.Windows.Forms.ComboBox cmbGroupFilter;
        private System.Windows.Forms.ToolStripMenuItem moveToTopToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem moveToBottomToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem convertToArchiveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem viewDataConflictsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem forceDisableToolStripMenuItem;
        private System.Windows.Forms.SaveFileDialog SaveOmodDialog;
        private System.IO.FileSystemWatcher PipeFileWatcher;
        private System.Windows.Forms.Button bUtilities;
        private System.Windows.Forms.ContextMenuStrip cmsUtilitiesMenu;
        private System.Windows.Forms.ToolStripMenuItem conflictReportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveManagerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bSABrowserToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip BatchContextMenu;
        private System.Windows.Forms.ToolStripMenuItem activateAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deactivateAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem activateAllToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem deactivateAllToolStripMenuItem1;
        private System.Windows.Forms.Button bBatch;
        private System.Windows.Forms.ToolStripMenuItem cleanAllToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem cleanFilteredToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem tidyDataFolderToolStripMenuItem;
        private System.Windows.Forms.Button bImport;
        private System.Windows.Forms.ContextMenuStrip ImportContextMenu;
        private System.Windows.Forms.ToolStripMenuItem omodReportToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportOmodListToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportLoadOrderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importOmodListToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importLoadOrderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bSACreatorToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem archiveInvalidationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem deleteBackupsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem updateConflictsToolStripMenuItem;
        private System.Windows.Forms.Button bHelp;
        private System.Windows.Forms.ToolStripMenuItem viewDataFilesToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportOmodGroupInformationToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem importOmodGroupInfoToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem bSAFixerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem dataFileBrowserToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem conflictReportToolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem rescanEspHeadersToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem hideToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem hideInactiveFToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem hiddenOmodSwitcherToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem extractToFolderToolStripMenuItem;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.ToolStripMenuItem unlinkToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem nifViewerToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aquisitionActivateFilteredToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aquisitionActivateToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem loadOrderToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exportOmodConversionDataToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem simulateToolStripMenuItem;
    }
}