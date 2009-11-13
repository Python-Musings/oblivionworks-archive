/* This file is part of Oblivion Mod Manager.
 * 
 * Oblivion Mod Manager is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Oblivion Mod Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

using System;
using System.Windows.Forms;

namespace OblivionModManager.Forms {
    internal partial class SelectForm : Form {
        internal SelectForm(string[] items,string title,bool multi,string[] previews,string[] tooltips) {
            InitializeComponent();
            Text=title;
            toolTips=tooltips;
            Multi=multi;
            System.Collections.Generic.List<int> selected=new System.Collections.Generic.List<int>();
            for(int i=0;i<items.Length;i++) {
                if(items[i].Length==0) continue;
                if(items[i][0]=='|') {
                    items[i]=items[i].Substring(1);
                    selected.Add(i);
                    if(!Multi) break;
                }
            }
            lbSelect.Items.AddRange(items);
            if(Multi) {
                if(Settings.AdvSelectMany) {
                    lbSelect.SelectionMode=SelectionMode.MultiExtended;
                } else {
                    lbSelect.SelectionMode=SelectionMode.MultiSimple;
                }
                label1.Text="Select any number of options";
                if(selected.Count>0) foreach(int i in selected) lbSelect.SetSelected(i, true);
            } else {
                if(selected.Count==0) lbSelect.SelectedIndex=0; else lbSelect.SelectedIndex=selected[0];
            }
            if(toolTips==null) bDescription.Visible=false;
            if(previews!=null) {
                Previews=new System.Drawing.Image[previews.Length];
                for(int i=0;i<previews.Length;i++) {
                    if(previews[i]==null) Previews[i]=null;
                    else Previews[i]=System.Drawing.Image.FromFile(previews[i]);
                }
                if(lbSelect.SelectedIndex!=-1&&lbSelect.SelectedIndices.Count==1) bPreview.Enabled=true;
            } else bPreview.Visible=false;
            lbSelect_SelectedIndexChanged(null, null);
        }

        private bool blockClose=true;
        internal int[] SelectedIndex={ 0 };
        private System.Drawing.Image[] Previews=null;
        private string[] toolTips;
        private bool ShowingDesc=false;
        private bool Multi;

        private int selectedIndex;
        private System.Collections.Generic.List<int> selected=new System.Collections.Generic.List<int>();

        private void bOK_Click(object sender, EventArgs e) {
            blockClose=false;
            SelectedIndex=new int[lbSelect.SelectedIndices.Count];
            for(int i=0;i<SelectedIndex.Length;i++) SelectedIndex[i]=lbSelect.SelectedIndices[i];
            if(Previews!=null) {
                foreach(System.Drawing.Image i in Previews) if(i!=null) i.Dispose();
            }
            Close();
        }

        private void SelectForm_FormClosing(object sender, FormClosingEventArgs e) {
            e.Cancel=blockClose;
        }

        private void lbSelect_SelectedIndexChanged(object sender, EventArgs e) {
            if(lbSelect.SelectedItems.Count==0) {
                selected.Clear();
                selectedIndex=-1;
                bDescription.Enabled=false;
                bPreview.Enabled=false;
                return;
            }
            if(selectedIndex!=-1&&!lbSelect.SelectedIndices.Contains(selectedIndex)) selectedIndex=-1;
            for(int i=0;i<selected.Count;i++) { if(!lbSelect.SelectedIndices.Contains(selected[i])) selected.RemoveAt(i--); }
            for(int i=0;i<lbSelect.SelectedIndices.Count;i++) {
                if(!selected.Contains(lbSelect.SelectedIndices[i])) {
                    selectedIndex=lbSelect.SelectedIndices[i];
                    selected.Add(lbSelect.SelectedIndices[i]);
                }
            }
            if(toolTips!=null) {
                if(selectedIndex!=-1&&toolTips[selectedIndex]!=null) {
                    bDescription.Enabled=true;
                } else {
                    bDescription.Enabled=false;
                }
            }
            if(Previews!=null) {
                if(selectedIndex!=-1&&Previews[selectedIndex]!=null) {
                    bPreview.Enabled=true;
                } else {
                    bPreview.Enabled=false;
                }
            }
        }

        private void bPreview_Click(object sender, EventArgs e) {
            (new ImageForm(Previews[selectedIndex])).ShowDialog();
        }

        private void bDescription_Click(object sender, EventArgs e) {
            if(ShowingDesc) {
                lbSelect.Visible=true;
                tbDesc.Visible=false;
                bDescription.Text="Description";
                ShowingDesc=false;
            } else if(toolTips[selectedIndex]!=null) {
                tbDesc.Text=toolTips[selectedIndex];
                tbDesc.Select(0, 0);
                tbDesc.ScrollToCaret();
                tbDesc.Visible=true;
                lbSelect.Visible=false;
                bDescription.Text="Options";
                ShowingDesc=true;
            } else bDescription.Enabled=false;
        }

        private void lbSelect_KeyDown(object sender, KeyEventArgs e) {
            if(e.Control&&e.KeyCode==Keys.A&&Multi) {
                //No function to do this all in one go?
                SuspendLayout();
                lbSelect.SelectedIndices.Clear();
                for(int i=0;i<lbSelect.Items.Count;i++) {
                    lbSelect.SelectedIndices.Add(i);
                }
                ResumeLayout();
            }
        }
    }
}