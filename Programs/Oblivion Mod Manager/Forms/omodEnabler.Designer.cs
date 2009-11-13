namespace OblivionModManager.Forms {
    partial class omodEnabler {
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(omodEnabler));
            this.lbSelect = new System.Windows.Forms.ListBox();
            this.SuspendLayout();
            // 
            // lbSelect
            // 
            this.lbSelect.Dock = System.Windows.Forms.DockStyle.Fill;
            this.lbSelect.FormattingEnabled = true;
            this.lbSelect.IntegralHeight = false;
            this.lbSelect.Location = new System.Drawing.Point(0, 0);
            this.lbSelect.Name = "lbSelect";
            this.lbSelect.SelectionMode = System.Windows.Forms.SelectionMode.MultiSimple;
            this.lbSelect.Size = new System.Drawing.Size(292, 273);
            this.lbSelect.TabIndex = 0;
            // 
            // omodEnabler
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(292, 273);
            this.Controls.Add(this.lbSelect);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "omodEnabler";
            this.Text = "Choose hidden omods";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.omodEnabler_FormClosing);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox lbSelect;
    }
}