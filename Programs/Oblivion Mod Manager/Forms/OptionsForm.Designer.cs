namespace OblivionModManager {
    partial class OptionsForm {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(OptionsForm));
            this.cbWarnings = new System.Windows.Forms.CheckBox();
            this.cmbGroups = new System.Windows.Forms.ComboBox();
            this.bDelete = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.bAdd = new System.Windows.Forms.Button();
            this.cbIniWarn = new System.Windows.Forms.CheckBox();
            this.cbDataWarnings = new System.Windows.Forms.CheckBox();
            this.cbOmodInfo = new System.Windows.Forms.CheckBox();
            this.cbLockFOV = new System.Windows.Forms.CheckBox();
            this.bMoveModFolder = new System.Windows.Forms.Button();
            this.omodDirDialog = new System.Windows.Forms.FolderBrowserDialog();
            this.cbShowEspWarnings = new System.Windows.Forms.CheckBox();
            this.label2 = new System.Windows.Forms.Label();
            this.tbCommandLine = new System.Windows.Forms.TextBox();
            this.DudMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.bRenameGroup = new System.Windows.Forms.Button();
            this.bClearGroups = new System.Windows.Forms.Button();
            this.bMoveToEnd = new System.Windows.Forms.Button();
            this.cbCompressionBoost = new System.Windows.Forms.CheckBox();
            this.cbAutoupdateConflicts = new System.Windows.Forms.CheckBox();
            this.cbUpdateEsps = new System.Windows.Forms.CheckBox();
            this.bResetTempFolder = new System.Windows.Forms.Button();
            this.bMoveTempFolder = new System.Windows.Forms.Button();
            this.bSetFont = new System.Windows.Forms.Button();
            this.GroupFontDialog = new System.Windows.Forms.FontDialog();
            this.cbUseKiller = new System.Windows.Forms.CheckBox();
            this.cbSafeMode = new System.Windows.Forms.CheckBox();
            this.bReset = new System.Windows.Forms.Button();
            this.cbNewEspsLoadLast = new System.Windows.Forms.CheckBox();
            this.cbAdvSelectMany = new System.Windows.Forms.CheckBox();
            this.cbTrackConflicts = new System.Windows.Forms.CheckBox();
            this.cbAllowInsecureScripts = new System.Windows.Forms.CheckBox();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.cbNeverModifyLoadOrder = new System.Windows.Forms.CheckBox();
            this.SuspendLayout();
            // 
            // cbWarnings
            // 
            this.cbWarnings.AutoSize = true;
            this.cbWarnings.Location = new System.Drawing.Point(12, 58);
            this.cbWarnings.Name = "cbWarnings";
            this.cbWarnings.Size = new System.Drawing.Size(205, 17);
            this.cbWarnings.TabIndex = 4;
            this.cbWarnings.Text = "Display warnings when running scripts";
            this.cbWarnings.UseVisualStyleBackColor = true;
            this.cbWarnings.CheckedChanged += new System.EventHandler(this.cbWarnings_CheckedChanged);
            // 
            // cmbGroups
            // 
            this.cmbGroups.FormattingEnabled = true;
            this.cmbGroups.Items.AddRange(new object[] {
            "None"});
            this.cmbGroups.Location = new System.Drawing.Point(18, 219);
            this.cmbGroups.Name = "cmbGroups";
            this.cmbGroups.Size = new System.Drawing.Size(186, 21);
            this.cmbGroups.TabIndex = 17;
            this.cmbGroups.Text = "None";
            this.cmbGroups.SelectedIndexChanged += new System.EventHandler(this.cmbGroups_SelectedIndexChanged);
            this.cmbGroups.TextChanged += new System.EventHandler(this.cmbGroups_TextChanged);
            // 
            // bDelete
            // 
            this.bDelete.Enabled = false;
            this.bDelete.Location = new System.Drawing.Point(18, 246);
            this.bDelete.Name = "bDelete";
            this.bDelete.Size = new System.Drawing.Size(90, 23);
            this.bDelete.TabIndex = 18;
            this.bDelete.Text = "Delete";
            this.bDelete.UseVisualStyleBackColor = true;
            this.bDelete.Click += new System.EventHandler(this.bDelete_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(72, 193);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(68, 13);
            this.label1.TabIndex = 15;
            this.label1.Text = "omod groups";
            // 
            // bAdd
            // 
            this.bAdd.Enabled = false;
            this.bAdd.Location = new System.Drawing.Point(114, 246);
            this.bAdd.Name = "bAdd";
            this.bAdd.Size = new System.Drawing.Size(90, 23);
            this.bAdd.TabIndex = 19;
            this.bAdd.Text = "Add";
            this.bAdd.UseVisualStyleBackColor = true;
            this.bAdd.Click += new System.EventHandler(this.bAdd_Click);
            // 
            // cbIniWarn
            // 
            this.cbIniWarn.AutoSize = true;
            this.cbIniWarn.Location = new System.Drawing.Point(12, 35);
            this.cbIniWarn.Name = "cbIniWarn";
            this.cbIniWarn.Size = new System.Drawing.Size(128, 17);
            this.cbIniWarn.TabIndex = 2;
            this.cbIniWarn.Text = "Warn on script ini edit";
            this.cbIniWarn.UseVisualStyleBackColor = true;
            this.cbIniWarn.CheckedChanged += new System.EventHandler(this.cbIniWarn_CheckedChanged);
            // 
            // cbDataWarnings
            // 
            this.cbDataWarnings.AutoSize = true;
            this.cbDataWarnings.Location = new System.Drawing.Point(227, 35);
            this.cbDataWarnings.Name = "cbDataWarnings";
            this.cbDataWarnings.Size = new System.Drawing.Size(122, 17);
            this.cbDataWarnings.TabIndex = 3;
            this.cbDataWarnings.Text = "Show misc warnings";
            this.cbDataWarnings.UseVisualStyleBackColor = true;
            this.cbDataWarnings.CheckedChanged += new System.EventHandler(this.cbDataWarnings_CheckedChanged);
            // 
            // cbOmodInfo
            // 
            this.cbOmodInfo.AutoSize = true;
            this.cbOmodInfo.Location = new System.Drawing.Point(227, 12);
            this.cbOmodInfo.Name = "cbOmodInfo";
            this.cbOmodInfo.Size = new System.Drawing.Size(194, 17);
            this.cbOmodInfo.TabIndex = 1;
            this.cbOmodInfo.Text = "Display omod info before installation";
            this.cbOmodInfo.UseVisualStyleBackColor = true;
            this.cbOmodInfo.CheckedChanged += new System.EventHandler(this.cbOmodInfo_CheckedChanged);
            // 
            // cbLockFOV
            // 
            this.cbLockFOV.AutoSize = true;
            this.cbLockFOV.Location = new System.Drawing.Point(12, 12);
            this.cbLockFOV.Name = "cbLockFOV";
            this.cbLockFOV.Size = new System.Drawing.Size(101, 17);
            this.cbLockFOV.TabIndex = 0;
            this.cbLockFOV.Text = "Lock FOV at 75";
            this.cbLockFOV.UseVisualStyleBackColor = true;
            this.cbLockFOV.CheckedChanged += new System.EventHandler(this.cbLockFOV_CheckedChanged);
            // 
            // bMoveModFolder
            // 
            this.bMoveModFolder.Location = new System.Drawing.Point(224, 246);
            this.bMoveModFolder.Name = "bMoveModFolder";
            this.bMoveModFolder.Size = new System.Drawing.Size(194, 23);
            this.bMoveModFolder.TabIndex = 25;
            this.bMoveModFolder.Text = "Move omod directory";
            this.bMoveModFolder.UseVisualStyleBackColor = true;
            this.bMoveModFolder.Click += new System.EventHandler(this.bMoveModFolder_Click);
            // 
            // omodDirDialog
            // 
            this.omodDirDialog.Description = "Select a new home for your omods";
            // 
            // cbShowEspWarnings
            // 
            this.cbShowEspWarnings.AutoSize = true;
            this.cbShowEspWarnings.Location = new System.Drawing.Point(227, 58);
            this.cbShowEspWarnings.Name = "cbShowEspWarnings";
            this.cbShowEspWarnings.Size = new System.Drawing.Size(179, 17);
            this.cbShowEspWarnings.TabIndex = 5;
            this.cbShowEspWarnings.Text = "Show esp deactivation warnings";
            this.cbShowEspWarnings.UseVisualStyleBackColor = true;
            this.cbShowEspWarnings.CheckedChanged += new System.EventHandler(this.cbShowEspWarnings_CheckedChanged);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(267, 193);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(113, 13);
            this.label2.TabIndex = 16;
            this.label2.Text = "Oblivion command line";
            // 
            // tbCommandLine
            // 
            this.tbCommandLine.Location = new System.Drawing.Point(227, 219);
            this.tbCommandLine.Name = "tbCommandLine";
            this.tbCommandLine.Size = new System.Drawing.Size(194, 20);
            this.tbCommandLine.TabIndex = 24;
            this.tbCommandLine.TextChanged += new System.EventHandler(this.tbCommandLine_TextChanged);
            // 
            // DudMenu
            // 
            this.DudMenu.Name = "DudMenu";
            this.DudMenu.Size = new System.Drawing.Size(61, 4);
            // 
            // bRenameGroup
            // 
            this.bRenameGroup.Enabled = false;
            this.bRenameGroup.Location = new System.Drawing.Point(18, 275);
            this.bRenameGroup.Name = "bRenameGroup";
            this.bRenameGroup.Size = new System.Drawing.Size(90, 23);
            this.bRenameGroup.TabIndex = 20;
            this.bRenameGroup.Text = "Rename";
            this.bRenameGroup.UseVisualStyleBackColor = true;
            this.bRenameGroup.Click += new System.EventHandler(this.bRenameGroup_Click);
            // 
            // bClearGroups
            // 
            this.bClearGroups.Location = new System.Drawing.Point(114, 275);
            this.bClearGroups.Name = "bClearGroups";
            this.bClearGroups.Size = new System.Drawing.Size(90, 23);
            this.bClearGroups.TabIndex = 21;
            this.bClearGroups.Text = "Clear";
            this.bClearGroups.UseVisualStyleBackColor = true;
            this.bClearGroups.Click += new System.EventHandler(this.bClearGroups_Click);
            // 
            // bMoveToEnd
            // 
            this.bMoveToEnd.Enabled = false;
            this.bMoveToEnd.Location = new System.Drawing.Point(18, 304);
            this.bMoveToEnd.Name = "bMoveToEnd";
            this.bMoveToEnd.Size = new System.Drawing.Size(90, 23);
            this.bMoveToEnd.TabIndex = 22;
            this.bMoveToEnd.Text = "Move to end";
            this.bMoveToEnd.UseVisualStyleBackColor = true;
            this.bMoveToEnd.Click += new System.EventHandler(this.bMoveToEnd_Click);
            // 
            // cbCompressionBoost
            // 
            this.cbCompressionBoost.AutoSize = true;
            this.cbCompressionBoost.Location = new System.Drawing.Point(12, 81);
            this.cbCompressionBoost.Name = "cbCompressionBoost";
            this.cbCompressionBoost.Size = new System.Drawing.Size(115, 17);
            this.cbCompressionBoost.TabIndex = 6;
            this.cbCompressionBoost.Text = "Compression boost";
            this.cbCompressionBoost.UseVisualStyleBackColor = true;
            this.cbCompressionBoost.CheckedChanged += new System.EventHandler(this.cbCompressionBoost_CheckedChanged);
            // 
            // cbAutoupdateConflicts
            // 
            this.cbAutoupdateConflicts.AutoSize = true;
            this.cbAutoupdateConflicts.Location = new System.Drawing.Point(227, 104);
            this.cbAutoupdateConflicts.Name = "cbAutoupdateConflicts";
            this.cbAutoupdateConflicts.Size = new System.Drawing.Size(123, 17);
            this.cbAutoupdateConflicts.TabIndex = 9;
            this.cbAutoupdateConflicts.Text = "Autoupdate conflicts";
            this.cbAutoupdateConflicts.UseVisualStyleBackColor = true;
            this.cbAutoupdateConflicts.CheckedChanged += new System.EventHandler(this.cbAutoupdateConflicts_CheckedChanged);
            // 
            // cbUpdateEsps
            // 
            this.cbUpdateEsps.AutoSize = true;
            this.cbUpdateEsps.Location = new System.Drawing.Point(12, 104);
            this.cbUpdateEsps.Name = "cbUpdateEsps";
            this.cbUpdateEsps.Size = new System.Drawing.Size(179, 17);
            this.cbUpdateEsps.TabIndex = 8;
            this.cbUpdateEsps.Text = "Update unparented esp headers";
            this.cbUpdateEsps.UseVisualStyleBackColor = true;
            this.cbUpdateEsps.CheckedChanged += new System.EventHandler(this.cbUpdateEsps_CheckedChanged);
            // 
            // bResetTempFolder
            // 
            this.bResetTempFolder.Location = new System.Drawing.Point(224, 304);
            this.bResetTempFolder.Name = "bResetTempFolder";
            this.bResetTempFolder.Size = new System.Drawing.Size(194, 23);
            this.bResetTempFolder.TabIndex = 27;
            this.bResetTempFolder.Text = "Use default temp folder";
            this.bResetTempFolder.UseVisualStyleBackColor = true;
            this.bResetTempFolder.Click += new System.EventHandler(this.bResetTempFolder_Click);
            // 
            // bMoveTempFolder
            // 
            this.bMoveTempFolder.Location = new System.Drawing.Point(224, 275);
            this.bMoveTempFolder.Name = "bMoveTempFolder";
            this.bMoveTempFolder.Size = new System.Drawing.Size(194, 23);
            this.bMoveTempFolder.TabIndex = 26;
            this.bMoveTempFolder.Text = "Move temp directory";
            this.bMoveTempFolder.UseVisualStyleBackColor = true;
            this.bMoveTempFolder.Click += new System.EventHandler(this.bMoveTempFolder_Click);
            // 
            // bSetFont
            // 
            this.bSetFont.Enabled = false;
            this.bSetFont.Location = new System.Drawing.Point(114, 305);
            this.bSetFont.Name = "bSetFont";
            this.bSetFont.Size = new System.Drawing.Size(90, 23);
            this.bSetFont.TabIndex = 23;
            this.bSetFont.Text = "Set font";
            this.bSetFont.UseVisualStyleBackColor = true;
            this.bSetFont.Click += new System.EventHandler(this.bSetFont_Click);
            // 
            // GroupFontDialog
            // 
            this.GroupFontDialog.FontMustExist = true;
            this.GroupFontDialog.ShowColor = true;
            // 
            // cbUseKiller
            // 
            this.cbUseKiller.AutoSize = true;
            this.cbUseKiller.Location = new System.Drawing.Point(12, 127);
            this.cbUseKiller.Name = "cbUseKiller";
            this.cbUseKiller.Size = new System.Drawing.Size(169, 17);
            this.cbUseKiller.TabIndex = 10;
            this.cbUseKiller.Text = "Use background process killer";
            this.cbUseKiller.UseVisualStyleBackColor = true;
            this.cbUseKiller.CheckedChanged += new System.EventHandler(this.cbUseKiller_CheckedChanged);
            // 
            // cbSafeMode
            // 
            this.cbSafeMode.AutoSize = true;
            this.cbSafeMode.Location = new System.Drawing.Point(227, 127);
            this.cbSafeMode.Name = "cbSafeMode";
            this.cbSafeMode.Size = new System.Drawing.Size(77, 17);
            this.cbSafeMode.TabIndex = 11;
            this.cbSafeMode.Text = "Safe mode";
            this.cbSafeMode.UseVisualStyleBackColor = true;
            this.cbSafeMode.CheckedChanged += new System.EventHandler(this.cbSafeMode_CheckedChanged);
            // 
            // bReset
            // 
            this.bReset.Location = new System.Drawing.Point(134, 344);
            this.bReset.Name = "bReset";
            this.bReset.Size = new System.Drawing.Size(162, 23);
            this.bReset.TabIndex = 28;
            this.bReset.Text = "Reset all settings to defaults";
            this.bReset.UseVisualStyleBackColor = true;
            this.bReset.Click += new System.EventHandler(this.bReset_Click);
            // 
            // cbNewEspsLoadLast
            // 
            this.cbNewEspsLoadLast.AutoSize = true;
            this.cbNewEspsLoadLast.Location = new System.Drawing.Point(227, 150);
            this.cbNewEspsLoadLast.Name = "cbNewEspsLoadLast";
            this.cbNewEspsLoadLast.Size = new System.Drawing.Size(150, 17);
            this.cbNewEspsLoadLast.TabIndex = 13;
            this.cbNewEspsLoadLast.Text = "New esps always load last";
            this.cbNewEspsLoadLast.UseVisualStyleBackColor = true;
            this.cbNewEspsLoadLast.CheckedChanged += new System.EventHandler(this.cbNewEspsLoadLast_CheckedChanged);
            // 
            // cbAdvSelectMany
            // 
            this.cbAdvSelectMany.AutoSize = true;
            this.cbAdvSelectMany.Location = new System.Drawing.Point(12, 150);
            this.cbAdvSelectMany.Name = "cbAdvSelectMany";
            this.cbAdvSelectMany.Size = new System.Drawing.Size(195, 17);
            this.cbAdvSelectMany.TabIndex = 12;
            this.cbAdvSelectMany.Text = "Allow shift/ctrl on SelectMany menu";
            this.cbAdvSelectMany.UseVisualStyleBackColor = true;
            this.cbAdvSelectMany.CheckedChanged += new System.EventHandler(this.cbAdvSelectMany_CheckedChanged);
            // 
            // cbTrackConflicts
            // 
            this.cbTrackConflicts.AutoSize = true;
            this.cbTrackConflicts.Location = new System.Drawing.Point(227, 81);
            this.cbTrackConflicts.Name = "cbTrackConflicts";
            this.cbTrackConflicts.Size = new System.Drawing.Size(137, 17);
            this.cbTrackConflicts.TabIndex = 7;
            this.cbTrackConflicts.Text = "Enable conflict tracking";
            this.cbTrackConflicts.UseVisualStyleBackColor = true;
            this.cbTrackConflicts.CheckedChanged += new System.EventHandler(this.cbTrackConflicts_CheckedChanged);
            // 
            // cbAllowInsecureScripts
            // 
            this.cbAllowInsecureScripts.AutoSize = true;
            this.cbAllowInsecureScripts.Location = new System.Drawing.Point(12, 173);
            this.cbAllowInsecureScripts.Name = "cbAllowInsecureScripts";
            this.cbAllowInsecureScripts.Size = new System.Drawing.Size(155, 17);
            this.cbAllowInsecureScripts.TabIndex = 14;
            this.cbAllowInsecureScripts.Text = "Allow additional script types";
            this.cbAllowInsecureScripts.UseVisualStyleBackColor = true;
            this.cbAllowInsecureScripts.CheckedChanged += new System.EventHandler(this.cbAllowInsecureScripts_CheckedChanged);
            // 
            // cbNeverModifyLoadOrder
            // 
            this.cbNeverModifyLoadOrder.AutoSize = true;
            this.cbNeverModifyLoadOrder.Location = new System.Drawing.Point(227, 173);
            this.cbNeverModifyLoadOrder.Name = "cbNeverModifyLoadOrder";
            this.cbNeverModifyLoadOrder.Size = new System.Drawing.Size(138, 17);
            this.cbNeverModifyLoadOrder.TabIndex = 29;
            this.cbNeverModifyLoadOrder.Text = "Never modify load order";
            this.cbNeverModifyLoadOrder.UseVisualStyleBackColor = true;
            this.cbNeverModifyLoadOrder.CheckedChanged += new System.EventHandler(this.cbNeverModifyLoadOrder_CheckedChanged);
            // 
            // OptionsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(442, 380);
            this.Controls.Add(this.cbNeverModifyLoadOrder);
            this.Controls.Add(this.cbAllowInsecureScripts);
            this.Controls.Add(this.cbTrackConflicts);
            this.Controls.Add(this.cbAdvSelectMany);
            this.Controls.Add(this.bReset);
            this.Controls.Add(this.cbNewEspsLoadLast);
            this.Controls.Add(this.cbSafeMode);
            this.Controls.Add(this.cbUseKiller);
            this.Controls.Add(this.cbUpdateEsps);
            this.Controls.Add(this.bSetFont);
            this.Controls.Add(this.bMoveTempFolder);
            this.Controls.Add(this.bResetTempFolder);
            this.Controls.Add(this.cbCompressionBoost);
            this.Controls.Add(this.cbAutoupdateConflicts);
            this.Controls.Add(this.cbShowEspWarnings);
            this.Controls.Add(this.cbLockFOV);
            this.Controls.Add(this.cbOmodInfo);
            this.Controls.Add(this.cbDataWarnings);
            this.Controls.Add(this.bMoveToEnd);
            this.Controls.Add(this.cbIniWarn);
            this.Controls.Add(this.bClearGroups);
            this.Controls.Add(this.bRenameGroup);
            this.Controls.Add(this.cbWarnings);
            this.Controls.Add(this.tbCommandLine);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.bAdd);
            this.Controls.Add(this.bDelete);
            this.Controls.Add(this.bMoveModFolder);
            this.Controls.Add(this.cmbGroups);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "OptionsForm";
            this.Text = "Settings";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.CheckBox cbWarnings;
        private System.Windows.Forms.ComboBox cmbGroups;
        private System.Windows.Forms.Button bDelete;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button bAdd;
        private System.Windows.Forms.CheckBox cbIniWarn;
        private System.Windows.Forms.CheckBox cbDataWarnings;
        private System.Windows.Forms.CheckBox cbOmodInfo;
        private System.Windows.Forms.CheckBox cbLockFOV;
        private System.Windows.Forms.Button bMoveModFolder;
        private System.Windows.Forms.FolderBrowserDialog omodDirDialog;
        private System.Windows.Forms.CheckBox cbShowEspWarnings;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.TextBox tbCommandLine;
        private System.Windows.Forms.ContextMenuStrip DudMenu;
        private System.Windows.Forms.Button bRenameGroup;
        private System.Windows.Forms.Button bClearGroups;
        private System.Windows.Forms.Button bMoveToEnd;
        private System.Windows.Forms.CheckBox cbCompressionBoost;
        private System.Windows.Forms.CheckBox cbAutoupdateConflicts;
        private System.Windows.Forms.CheckBox cbUpdateEsps;
        private System.Windows.Forms.Button bResetTempFolder;
        private System.Windows.Forms.Button bMoveTempFolder;
        private System.Windows.Forms.Button bSetFont;
        private System.Windows.Forms.FontDialog GroupFontDialog;
        private System.Windows.Forms.CheckBox cbUseKiller;
        private System.Windows.Forms.CheckBox cbSafeMode;
        private System.Windows.Forms.Button bReset;
        private System.Windows.Forms.CheckBox cbNewEspsLoadLast;
        private System.Windows.Forms.CheckBox cbAdvSelectMany;
        private System.Windows.Forms.CheckBox cbTrackConflicts;
        private System.Windows.Forms.CheckBox cbAllowInsecureScripts;
        private System.Windows.Forms.ToolTip toolTip;
        private System.Windows.Forms.CheckBox cbNeverModifyLoadOrder;
    }
}