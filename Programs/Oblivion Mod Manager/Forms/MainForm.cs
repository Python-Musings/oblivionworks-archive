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
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;
using System.IO;

namespace OblivionModManager {
    internal partial class MainForm : Form {
        internal MainForm() {
            InitializeComponent();
            lvEspList.ListViewItemSorter=new EspListSorter();
            lvModList.ListViewItemSorter=new omodListSorter();
            UpdateEspList();
            UpdateOmodList();
            UpdateGroups();
            if(Settings.mfSize!=System.Drawing.Size.Empty) Size=Settings.mfSize;
            if(Settings.mfMaximized) WindowState=System.Windows.Forms.FormWindowState.Maximized;
            if(Settings.mfSplitterPos!=0) splitContainer1.SplitterDistance=Settings.mfSplitterPos;
            if(Settings.EspColWidth1!=-1) lvEspList.Columns[0].Width=Settings.EspColWidth1;
            if(Settings.EspColWidth2!=-1) lvEspList.Columns[1].Width=Settings.EspColWidth2;
            cmbEspSortOrder.SelectedIndex=Settings.EspSortOrder;
            cmbOmodSortOrder.SelectedIndex=Settings.omodSortOrder;
        }

        private void RefreshLists() {
            if(Settings.AutoUpdateConflicts) Conflicts.UpdateConflicts();
            UpdateEspList();
            UpdateOmodList();
        }

        private void UpdateEspList() {
            lvEspList.SuspendLayout();
            lvEspList.Items.Clear();
            int ActiveCount=0;
            foreach(EspInfo ei in Program.Data.Esps) {
                string toolText=ei.FileName+"\nAuthor: "+ei.header.Author;
                ListViewItem lvi=new ListViewItem(new string[] { ei.FileName, ei.BelongsTo });
                if(ei.Active) {
                    lvi.Checked=true;
                    toolText+="\n\nFormID: "+(ActiveCount++).ToString("X").PadLeft(2, '0');
                }
                if(ei.header.Description!=null) {
                    toolText+="\n\n"+ei.header.Description;
                }
                lvi.Tag=ei;
                lvi.ToolTipText=toolText;
                lvEspList.Items.Add(lvi);
            }
            lvEspList.ResumeLayout();
        }

        private void UpdateOmodList() {
            lvModList.OwnerDraw=true;
            lvModList.SuspendLayout();
            int pos;
            if(lvModList.SelectedIndices.Count>0) pos=lvModList.SelectedIndices[0];
            else pos=0;
            lvModList.Clear();
            foreach(omod o in Program.Data.omods) {
                if(o.Hidden) continue;
                if(cmbGroupFilter.SelectedIndex!=0) {
                    if(cmbGroupFilter.SelectedIndex==1) {
                        if(o.group!=0) continue;
                    } else {
                        if((o.group&(ulong)((ulong)0x01<<(cmbGroupFilter.SelectedIndex-2))) == 0) continue;
                    }
                }
                ListViewItem lvi=new ListViewItem(Path.GetFileNameWithoutExtension(o.FileName));
                for(int i=0;i<Settings.omodGroups.Count;i++) {
                    if((o.group&(ulong)((ulong)0x01<<i))!=0) {
                        if(Settings.omodGroups[i].Font!=null) {
                            lvi.Font=Settings.omodGroups[i].Font;
                            lvi.ForeColor=Settings.omodGroups[i].Color;
                            break;
                        }
                    }
                }
                lvi.ToolTipText=o.ModName+"\nAuthor: "+o.Author+"\n"+"Version: "+o.Version+"\n\n"+o.Description;
                lvi.Tag=o;
                switch(o.Conflict) {
                case ConflictLevel.Active:
                    lvi.ImageIndex=0;
                    break;
                case ConflictLevel.NoConflict:
                    lvi.ImageIndex=1;
                    break;
                case ConflictLevel.MinorConflict:
                    lvi.ImageIndex=2;
                    break;
                case ConflictLevel.MajorConflict:
                    lvi.ImageIndex=3;
                    break;
                case ConflictLevel.Unusable:
                    lvi.ImageIndex=4;
                    break;
                default:
                    MessageBox.Show("omod had unrecognised conflict level", "Error");
                    lvi.ImageIndex=1;
                    break;
                }
                lvModList.Items.Add(lvi);
            }
            if(lvModList.Items.Count>pos) lvModList.EnsureVisible(pos);
            lvModList.ResumeLayout();
            lvModList.OwnerDraw=false;
        }

        private void lvEspList_ItemCheck(object sender, ItemCheckEventArgs e) {
            if(((EspInfo)(lvEspList.Items[e.Index].Tag)).Active==(e.NewValue==CheckState.Checked)) return;
            if(e.NewValue==CheckState.Checked) {
                ((EspInfo)(lvEspList.Items[e.Index].Tag)).Active=true;
                return;
            }
            EspInfo ei=(EspInfo)lvEspList.Items[e.Index].Tag;
            if(ei.Parent!=null) {
                if(ei.Deactivatable==DeactiveStatus.Disallow) {
                    MessageBox.Show("That plugin belongs to "+ei.BelongsTo+".\n"+
                    "You must disable the entire mod instead of just this single file.", "Error");
                    e.NewValue=e.CurrentValue;
                    return;
                } else if(ei.Deactivatable==DeactiveStatus.WarnAgainst) {
                    if(MessageBox.Show("That plugin belongs to "+ei.BelongsTo+".\n"+
                    "It is recommended that you disable the entire mod instead of just this single file.\n"+
                    "Are you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) {
                        e.NewValue=e.CurrentValue;
                        return;
                    }
                }
            }
            ((EspInfo)(lvEspList.Items[e.Index].Tag)).Active=false;
        }

        private void lvModList_ItemActivate(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            ActivateOmod((omod)lvModList.SelectedItems[0].Tag, false);
            RefreshLists();
        }

        private void bCreate_Click(object sender, EventArgs e) {
            if(CreateModForm.ShowForm()) {
                UpdateOmodList();
            }
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvEspList.SelectedItems.Count!=1) return;
            EspInfo ei=(EspInfo)lvEspList.SelectedItems[0].Tag;
            if(ei.BelongsTo!=EspInfo.UnknownOwner) {
                if(ei.BelongsTo==EspInfo.BaseOwner) {
                    MessageBox.Show("That plugin is an essental part of Oblivion and cannot be removed", "Error");
                } else {
                    MessageBox.Show("That plugin is part of "+ei.BelongsTo+" and cannot be removed", "Error");
                }
                return;
            }
            if(Settings.WarnOnModDelete&&MessageBox.Show("This will delete "+ei.FileName+" permenently.\n"+
                "Are you sure you wish to do this?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) {
                return;
            }
            File.Delete("data\\"+ei.FileName);
            Program.Data.Esps.Remove(ei);
            lvEspList.Items.RemoveAt(lvEspList.SelectedIndices[0]);
        }

        private void cleanToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            if(o.Conflict==ConflictLevel.Active) {
                MessageBox.Show(o.ModName+" must be deactivated before it can be cleaned.", "Error");
                return;
            }
            if(Settings.WarnOnModDelete&&MessageBox.Show("If you clean this mod some files may be permenently deleted.\n"+
                "Are you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) {
                return;
            }
            int DeletedCount=0;
            int SkippedCount=0;
            int NotFoundCount=0;
            o.Clean(ref DeletedCount, ref SkippedCount, ref NotFoundCount);
            Conflicts.UpdateConflicts();
            UpdateEspList();
            UpdateOmodList();
            MessageBox.Show("Files deleted: "+DeletedCount.ToString()+"\n"+
                "Files skipped: "+SkippedCount.ToString()+"\n"+
                "Files not found: "+NotFoundCount.ToString(), "Message");
        }

        private void deleteToolStripMenuItem1_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            if(MessageBox.Show("This will delete "+o.FileName+" permenently.\n"+
                "Are you sure you wish to do this?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) {
                return;
            }
            if(o.Conflict==ConflictLevel.Active) {
                if(MessageBox.Show(o.ModName+" is still active.\n"+
                    "A forced disable will be carried out before deletion, but may have side effects.\n"+
                    "It is recommended that you disable the omod normally before attempting to delete it.\n"+
                    "Are you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes)
                    return;
                o.DeletionDeactivate();
            }
            o.Close();
            Program.Data.omods.Remove(o);
            File.Delete(Path.Combine(Settings.omodDir,o.FileName));
            lvModList.Items.RemoveAt(lvModList.SelectedIndices[0]);
        }

        private void viewReadmeToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            try {
                string s=o.GetReadme();
                TextEditor te=new TextEditor("View readme", s, true, true);
                if(te.ShowDialog()==DialogResult.Yes) {
                    o.ReplaceReadme(te.Result);

                }
            } catch(Exception ex) {
                MessageBox.Show("Error opening readme.\n"+ex.Message, "Error");
            }
        }

        private void viewScriptToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            try {
                string s=o.GetScript();
                Forms.ScriptEditor se=new OblivionModManager.Forms.ScriptEditor(s,o.AllPlugins,o.AllDataFiles);
                if(se.ShowDialog()==DialogResult.Yes) o.ReplaceScript(se.Result);
            } catch(Exception ex) {
                MessageBox.Show("Error opening script.\n"+ex.Message, "Error");
            }
        }

        private bool SwitchEsps(int index) {
            if(Settings.NeverTouchLoadOrder) {
                MessageBox.Show("You cannot modify load order with the 'never modify load order' option checked", "Error");
                return false;
            }
            if(EspListSorter.order!=EspSortOrder.LoadOrder) {
                MessageBox.Show("Esps must be sorted by load order before you can switch them", "Error");
                return false;
            }
            EspInfo bottom=(EspInfo)lvEspList.Items[index].Tag;
            EspInfo top=(EspInfo)lvEspList.Items[index+1].Tag;
            if(bottom.MustLoadBefore.IndexOf(top.LowerFileName)!=-1) {
                MessageBox.Show(bottom.FileName+" must load before "+top.FileName, "Error");
                return false;
            }
            if(top.MustLoadAfter.IndexOf(bottom.LowerFileName)!=-1) {
                MessageBox.Show(top.FileName+" must load after "+bottom.FileName, "Error");
                return false;
            }
            if(top.LowerFileName.EndsWith(".esp")&&bottom.LowerFileName.EndsWith(".esm")) {
                MessageBox.Show("An esp cannot be loaded before an esm", "Error");
                return false;
            }
            DateTime oldtime=File.GetLastWriteTime("data\\"+bottom.FileName);
            File.SetLastWriteTime("data\\"+bottom.FileName, File.GetLastWriteTime("data\\"+top.FileName));
            File.SetLastWriteTime("data\\"+top.FileName, oldtime);
            Program.Data.SortEsps();
            ListViewItem lvi=lvEspList.Items[index];
            lvEspList.Items.RemoveAt(index);
            lvEspList.Items.Insert(index+1, lvi);
            return true;
        }

        private void bMoveUp_Click(object sender, EventArgs e) {
            if(lvEspList.SelectedItems.Count!=1) return;
            if(lvEspList.SelectedIndices[0]==0) return;
            SwitchEsps(lvEspList.SelectedIndices[0]-1);
        }

        private void moveToTopToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvEspList.SelectedItems.Count!=1) return;
            int position=lvEspList.SelectedIndices[0];
            while(position!=0) {
                if(!SwitchEsps(--position)) break;
            }
        }

        private void bMoveDown_Click(object sender, EventArgs e) {
            if(lvEspList.SelectedItems.Count!=1) return;
            if(lvEspList.SelectedIndices[0]==lvEspList.Items.Count-1) return;
            SwitchEsps(lvEspList.SelectedIndices[0]);
        }

        private void moveToBottomToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvEspList.SelectedItems.Count!=1) return;
            int position=lvEspList.SelectedIndices[0];
            while(position!=lvEspList.Items.Count-1) {
                if(!SwitchEsps(position++)) break;
            }
        }

        private void viewDataFilesToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvEspList.SelectedItems.Count==0) return;
            System.Collections.Generic.List<string> toadd=new System.Collections.Generic.List<string>();
            foreach(ListViewItem lvi in lvEspList.SelectedItems) {
                try {
                    toadd.AddRange(ConflictDetector.TesFile.GetDataFileList("data\\"+lvi.Text));
                } catch(Exception ex) {
                    MessageBox.Show("An error occurred trying to parse plugin "+lvi.SubItems[1].Text+"\n"+
                        "Error: "+ex.Message, "Error");
                }
            }
            //Remove duplicates
            int index;
            for(int i=0;i<toadd.Count;i++) {
                while((index=toadd.LastIndexOf(toadd[i]))>i) toadd.RemoveAt(index);
            }
            string text=string.Join(Environment.NewLine, toadd.ToArray());
            (new TextEditor("Required data files", text, false, false)).ShowDialog();
        }

        private void unlinkToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvEspList.SelectedItems.Count==0) return;
            bool MadeChanges=false;
            foreach(ListViewItem lvi in lvEspList.SelectedItems) {
                EspInfo ei=(EspInfo)lvi.Tag;
                if(ei.Parent!=null) {
                    ei.Unlink();
                    MadeChanges=true;
                }
            }
            if(MadeChanges) UpdateEspList();
        }

        private void lvEspList_KeyDown(object sender, KeyEventArgs e) {
            if(e.Alt) {
                if(e.KeyCode==Keys.Up) {
                    bMoveUp_Click(null, null);
                    e.Handled=true;
                } else if(e.KeyCode==Keys.Down) {
                    bMoveDown_Click(null, null);
                    e.Handled=true;
                }
            }
        }

        private void lvModList_SelectedIndexChanged(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            //group filter
            foreach(ToolStripMenuItem tsmi in GroupToolStripMenuItem.DropDownItems) {
                if(tsmi==GroupNoneToolStripMenuItem) continue;
                if((o.group&(ulong)((ulong)0x01<<(int)tsmi.Tag))>0) {
                    tsmi.Checked=true;
                } else {
                    tsmi.Checked=false;
                }
            }
            //menus
            if(o.DoesReadmeExist()) viewReadmeToolStripMenuItem.Text="View readme";
            else viewReadmeToolStripMenuItem.Text="Create readme";
            if(o.DoesScriptExist()) viewScriptToolStripMenuItem.Text="View script";
            else viewScriptToolStripMenuItem.Text="Create script";
            if(o.Website=="") visitWebsiteToolStripMenuItem.Enabled=false;
            else visitWebsiteToolStripMenuItem.Enabled=true;
            if(o.Email=="") emailAuthorToolStripMenuItem.Enabled=false;
            else emailAuthorToolStripMenuItem.Enabled=true;
            //image
            pictureBox1.Image=o.image;
            //activation button
            switch(o.Conflict) {
            case ConflictLevel.Active:
                bActivate.Text="Deactivate";
                bActivate.Enabled=true;
                break;
            /*case ConflictLevel.Unusable:
                bActivate.Text="Activate";
                bActivate.Enabled=false;
                break;*/
            default:
                bActivate.Text="Activate";
                bActivate.Enabled=true;
                break;
            }
        }

        private void bActivate_Click(object sender, EventArgs e) {
            lvModList_ItemActivate(null, null);
        }

        private void bConflict_Click(object sender, EventArgs e) {
            //(new TextEditor("Conflict report", Conflicts.UpdateConflictsWithReport(), false, false)).ShowDialog();
            ConflictDetector.ReportGenerator.GenerateReport();
        }

        private void infoToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            string report=o.GetInfo();
            (new TextEditor(o.FileName+" info", report, false, false)).ShowDialog();
        }

        private void bLoad_Click(object sender, EventArgs e) {
            OpenDialog.Title="Select mod files to load";
            OpenDialog.Filter="Oblivion mod file (*.omod)|*.omod";
            OpenDialog.Multiselect=true;
            if(OpenDialog.ShowDialog()==DialogResult.OK) {
                foreach(string s in OpenDialog.FileNames) Program.LoadNewOmod(s);
                UpdateOmodList();
            }
        }

        private void visitWebsiteToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            if(o.Website=="") {
                MessageBox.Show(o.FileName+" does not have a website", "Error");
                return;
            }
            string s=o.Website;
            if(!s.ToLower().StartsWith("http://")) s="http://"+s;
            System.Diagnostics.Process p=System.Diagnostics.Process.Start(s);
            if(p!=null) p.Close();
        }

        private void emailAuthorToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            if(o.Email=="") {
                MessageBox.Show(o.FileName+" does not have an email address", "Error");
                return;
            }
            string s="mailto:"+o.Email+"?subject="+o.FileName;
            System.Diagnostics.Process p=System.Diagnostics.Process.Start(s);
            if(p!=null) p.Close();
        }

        private void bHelp_Click(object sender, EventArgs e) {
            try {
                System.Diagnostics.Process.Start(Program.HelpPath);
            } catch(Exception ex) {
                MessageBox.Show("An error occured while trying to open the help file.\n"+ex.Message, "Error");
            }
        }

        private void cmbEspSortOrder_SelectedIndexChanged(object sender, EventArgs e) {
            EspListSorter.order=(EspSortOrder)cmbEspSortOrder.SelectedIndex;
            if(EspListSorter.order!=EspSortOrder.LoadOrder) {
                bMoveDown.Enabled=false;
                bMoveUp.Enabled=false;
                moveDownToolStripMenuItem.Enabled=false;
                moveUpToolStripMenuItem.Enabled=false;
            } else {
                bMoveDown.Enabled=true;
                bMoveUp.Enabled=true;
                moveDownToolStripMenuItem.Enabled=true;
                moveUpToolStripMenuItem.Enabled=true;
            }
            lvEspList.Sort();
        }

        private void bAbout_Click(object sender, EventArgs e) {
            (new About()).ShowDialog();
            if(Program.Launch!=LaunchType.None) Close();
        }

        private void bLaunch_Click(object sender, EventArgs e) {
            if(Settings.ShowLaunchWarning) {
                MessageBox.Show(
                    "This is a internal service announcment, due to the huge number of people who completely missunderstand what this button is for\n"+
                    "Basically, YOU DO NOT HAVE TO USE THIS BUTTON FOR OBMM TO WORK!!!\n"+
                    "Launching oblivion via the normal launcher will not magically make obmm lose it's load order or anything.\n"+
                    "If you have obse installed, this button will launch obse instead of oblivion, so the two are not incompatable.\n"+
                    "If you want to override this behaviour, or lanch a custom program, you can just edit the command line box in the settings\n"+
                    "To prove that you've read this, hold down shift while clicking OK so that this message will not appear again.", "Message");
                if(Program.KeyPressed(16)) Settings.ShowLaunchWarning=false;
                else return;
            }
            Program.Launch=LaunchType.Oblivion;
            Close();
        }

        private void bEdit_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            pictureBox1.Image=null;
            if(CreateModForm.ShowForm((omod)lvModList.SelectedItems[0].Tag)) {
                UpdateOmodList();
            }
        }

        private void pictureBox1_Click(object sender, EventArgs e) {
            if(pictureBox1.Image!=null) (new ImageForm(pictureBox1.Image)).ShowDialog();
        }

        private void TidyDataFolder() {
            System.Collections.Generic.List<DirectoryInfo> dis=new System.Collections.Generic.List<DirectoryInfo>();
            bool match=true;
            while(match) {
                dis.Clear();
                dis.Add(new DirectoryInfo("data"));
                match=false;
                for(int i=0;i<dis.Count;i++) {
                    DirectoryInfo[] temp=dis[i].GetDirectories();
                    if(temp.Length!=0) {
                        dis.AddRange(temp);
                    } else {
                        if(dis[i].GetFileSystemInfos().Length==0) {
                            dis[i].Delete();
                            dis.RemoveAt(i--);
                            match=true;
                        }
                    }
                }
            }
        }

        private void MainForm_FormClosing(object sender, FormClosingEventArgs e) {
            TidyDataFolder();
            Settings.mfSize=Size;
            if(WindowState==FormWindowState.Maximized) Settings.mfMaximized=true;
            else Settings.mfMaximized=false;
            Settings.mfSplitterPos=splitContainer1.SplitterDistance;
            Settings.EspColWidth1=lvEspList.Columns[0].Width;
            Settings.EspColWidth2=lvEspList.Columns[1].Width;
            Settings.EspSortOrder=cmbEspSortOrder.SelectedIndex;
            Settings.omodSortOrder=cmbOmodSortOrder.SelectedIndex;
        }

        private void bSettings_Click(object sender, EventArgs e) {
            (new OptionsForm()).ShowDialog();
            UpdateGroups();
        }

        private void IgnoreKeyPress(object sender, KeyPressEventArgs e) {
            e.Handled=true;
        }

        private void cmbOmodSortOrder_SelectedIndexChanged(object sender, EventArgs e) {
            omodListSorter.order=(omodSortOrder)cmbOmodSortOrder.SelectedIndex;
            lvModList.Sort();
        }

        private void cmbGroupFilter_SelectedIndexChanged(object sender, EventArgs e) {
            if(cmbGroupFilter.SelectedIndex==cmbGroupFilter.Items.Count-1) {
                cmbGroupFilter.SelectedIndex=0;
                bSettings_Click(null, null);
            } else {
                UpdateOmodList();
            }
        }

        private void UpdateGroups() {
            GroupToolStripMenuItem.DropDownItems.Clear();
            GroupToolStripMenuItem.DropDownItems.Add(GroupNoneToolStripMenuItem);
            for(int i=0;i<Settings.omodGroups.Count;i++) {
                ToolStripMenuItem tsmi=new ToolStripMenuItem(Settings.omodGroups[i].Name);
                tsmi.Tag=i;
                tsmi.Click+=GroupNoneToolStripMenuItem_Click;
                GroupToolStripMenuItem.DropDownItems.Add(tsmi);
            }
            cmbGroupFilter.Items.Clear();
            cmbGroupFilter.Items.Add("All");
            cmbGroupFilter.Items.Add("Unassigned");
            cmbGroupFilter.Items.AddRange(Settings.omodGroups.ToArray());
            cmbGroupFilter.Items.Add("[Edit groups]");
            cmbGroupFilter.SelectedIndex=0;
        }

        private void GroupNoneToolStripMenuItem_Click(object _sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            ToolStripMenuItem sender=(ToolStripMenuItem)_sender;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            if(sender==GroupNoneToolStripMenuItem) {
                o.group=0;
                lvModList_SelectedIndexChanged(null, null);
                return;
            }
            sender.Checked=!sender.Checked;
            if(sender.Checked) {
                o.group|=(ulong)((ulong)1<<(int)sender.Tag);
            } else {
                o.group&=0xFFFFFFFFFFFFFFFF - (ulong)((ulong)1<<(int)sender.Tag);
            }
        }

        private void aquisitionActivateToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            if(o.Conflict==ConflictLevel.Active) return;
            o.AquisitionActivate(true);
            UpdateOmodList();
        }

        private void simulateToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            if(o.Conflict==ConflictLevel.Active) return;

            string[] plugins=o.AllPlugins;
            DataFileInfo[] dfis=o.AllDataFiles;
            string[] dataFiles=new string[dfis.Length];
            for(int i=0;i<dfis.Length;i++) dataFiles[i]=dfis[i].FileName;
            ScriptReturnData srd;
            try {
                srd=Scripting.ScriptRunner.SimulateScript(o.GetScript(), dataFiles, plugins);
            } catch(Exception ex) {
                MessageBox.Show("The script did not complete successfully\n"+ex.Message);
                return;
            }
            Forms.SimResults simForm=new Forms.SimResults(srd, plugins, dataFiles);
            simForm.ShowDialog();
        }

        private void forceDisableToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            if(o.Conflict==ConflictLevel.Active) o.Deactivate(true);
            RefreshLists();
        }

        private void viewDataConflictsToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            (new TextEditor("data file and script conflicts", Conflicts.ConflictReport(o), false, false)).ShowDialog();
        }

        private void convertToArchiveToolStripMenuItem_Click(object sender, EventArgs e) {
            SaveOmodDialog.Title="Save omod as";
            SaveOmodDialog.Filter="7-zip (*.7z)|*.7z|zip (*.zip)|*.zip";
            SaveOmodDialog.AddExtension=false;
            SaveOmodDialog.OverwritePrompt=false;
            try {
                if(lvModList.SelectedItems.Count!=1) return;
                omod o=(omod)lvModList.SelectedItems[0].Tag;
                SaveOmodDialog.FileName=Path.GetFileNameWithoutExtension(o.FileName);
                if(SaveOmodDialog.ShowDialog()!=DialogResult.OK) return;
                string ZipName=SaveOmodDialog.FileName;
                if(SaveOmodDialog.FilterIndex==1) {
                    if(Path.GetExtension(ZipName).ToLower()!=".7z") ZipName+=".7z";
                } else {
                    if(Path.GetExtension(ZipName).ToLower()!=".zip") ZipName+=".zip";
                }
                if(File.Exists(ZipName)&&MessageBox.Show("File '"+ZipName+"' already exists.\n"+
                    "Do you wish to overwrite?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) return;
                bool CreateConversionData=MessageBox.Show("Create omod conversion information?",
                    "Question", MessageBoxButtons.YesNo)==DialogResult.Yes;
                string path=o.GetDataFiles();
                string temppath=null;
                if(path==null) {
                    path=o.GetPlugins();
                    if(path==null) path=Program.CreateTempDirectory();
                } else {
                    temppath=o.GetPlugins();
                    if(temppath!=null) {
                        foreach(string s in Directory.GetFiles(temppath, "*", SearchOption.AllDirectories)) {
                            string esppath=s.Substring(temppath.Length);
                            Directory.CreateDirectory(Path.Combine(path, Path.GetDirectoryName(esppath)));
                            File.Move(s, Path.Combine(path, esppath));
                        }
                    }
                }
                if(o.DoesReadmeExist()) {
                    File.WriteAllText(path+o.ModName+"_readme.rtf", o.GetReadme());
                }
                if(CreateConversionData) {
                    Directory.CreateDirectory(path+Program.omodConversionData);
                    if(o.DoesScriptExist()) {
                        File.WriteAllText(path+Program.omodConversionData+"script.txt", o.GetScript());
                    }
                    File.WriteAllText(path+Program.omodConversionData+"info.txt", o.GetInfo());
                    temppath=o.GetImage();
                    if(temppath!=null) {
                        System.Drawing.Imaging.ImageFormat imf=o.image.RawFormat;
                        File.Move(temppath, path+Program.omodConversionData+"screenshot");
                    }
                    temppath=o.GetConfig();
                    File.Move(temppath, path+Program.omodConversionData+"config");
                }
                if(SaveOmodDialog.FilterIndex==1) {
                    System.Diagnostics.ProcessStartInfo psi=new System.Diagnostics.ProcessStartInfo("obmm\\7za.exe");
                    psi.Arguments="a -t7z -mx="+(Settings.CompressionBoost?"9":"7")+" \""+ZipName+"\" \""+path+"*\"";
                    psi.CreateNoWindow=true;
                    psi.UseShellExecute=false;
                    System.Diagnostics.Process p=System.Diagnostics.Process.Start(psi);
                    p.WaitForExit();
                } else {
                    ICSharpCode.SharpZipLib.Zip.FastZip fs=new ICSharpCode.SharpZipLib.Zip.FastZip();
                    fs.CreateZip(ZipName, path, true, null);
                }
            } catch(Exception ex) {
                MessageBox.Show("Conversion failed\nError: "+ex.Message, "Error");
                return;
            }
            MessageBox.Show("Conversion sucessful", "Message");
        }

        private void extractToFolderToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            folderBrowserDialog1.Description="Select the path to extract "+o.FileName+" to";
            if(folderBrowserDialog1.ShowDialog()!=DialogResult.OK) return;
            if(Directory.Exists(folderBrowserDialog1.SelectedPath+"\\"+o.ModName)) {
                MessageBox.Show("Unable to extract to that directory.\n"+
                    "The path '"+folderBrowserDialog1.SelectedPath+"\\"+o.ModName+"' already exists", "Error");
                return;
            }
            try {
                bool CreateConversionData=MessageBox.Show("Create omod conversion information?",
                    "Question", MessageBoxButtons.YesNo)==DialogResult.Yes;
                string path=o.GetDataFiles();
                string temppath=null;
                if(path==null) {
                    path=o.GetPlugins();
                    if(path==null) path=Program.CreateTempDirectory();
                } else {
                    temppath=o.GetPlugins();
                    if(temppath!=null) {
                        foreach(string s in Directory.GetFiles(temppath, "*", SearchOption.AllDirectories)) {
                            string esppath=s.Substring(temppath.Length);
                            Directory.CreateDirectory(Path.Combine(path,Path.GetDirectoryName(esppath)));
                            File.Move(s, Path.Combine(path,esppath));
                        }
                    }
                }
                if(o.DoesReadmeExist()) {
                    File.WriteAllText(path+o.ModName+"_readme.rtf", o.GetReadme());
                }
                if(CreateConversionData) {
                    Directory.CreateDirectory(path+Program.omodConversionData);
                    if(o.DoesScriptExist()) {
                        File.WriteAllText(path+Program.omodConversionData+"script.txt", o.GetScript());
                    }
                    File.WriteAllText(path+Program.omodConversionData+"info.txt", o.GetInfo());
                    temppath=o.GetImage();
                    if(temppath!=null) {
                        System.Drawing.Imaging.ImageFormat imf=o.image.RawFormat;
                        File.Move(temppath, path+Program.omodConversionData+"screenshot");
                    }
                    temppath=o.GetConfig();
                    File.Move(temppath, path+Program.omodConversionData+"config");
                }
                Program.RecersiveDirectoryMove(path, folderBrowserDialog1.SelectedPath+"\\"+o.ModName, false);
                MessageBox.Show("Done");
            } catch(Exception ex) {
                MessageBox.Show("Extraction failed\nError: "+ex.Message, "Error");
            }
        }

        private void exportOmodConversionDataToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            folderBrowserDialog1.Description="Select the path to extract "+o.FileName+" to";
            if(folderBrowserDialog1.ShowDialog()!=DialogResult.OK) return;
            if(Directory.Exists(folderBrowserDialog1.SelectedPath+"\\"+folderBrowserDialog1.SelectedPath)) {
                MessageBox.Show("Unable to extract to that directory.\n"+
                    "The path '"+folderBrowserDialog1.SelectedPath+"\\"+folderBrowserDialog1.SelectedPath+"' already exists", "Error");
                return;
            }
            try {
                string path=Path.Combine(folderBrowserDialog1.SelectedPath, Program.omodConversionData)+"\\";
                string temppath;
                Directory.CreateDirectory(path);
                if(o.DoesScriptExist()) {
                    File.WriteAllText(path+"script.txt", o.GetScript());
                }
                File.WriteAllText(path+"info.txt", o.GetInfo());
                temppath=o.GetImage();
                if(temppath!=null) {
                    System.Drawing.Imaging.ImageFormat imf=o.image.RawFormat;
                    File.Move(temppath, path+"screenshot");
                }
                temppath=o.GetConfig();
                File.Move(temppath, path+"config");
            } catch(Exception ex) {
                MessageBox.Show("Extraction failed\nError: "+ex.Message, "Error");
            }
        }

        private void bSaves_Click(object sender, EventArgs e) {
            (new SaveForm()).ShowDialog();
            UpdateEspList();
        }

        private void PipeFileWatcher_Created(object sender, FileSystemEventArgs e) {
            System.Threading.Thread.Sleep(100); //Give the other process a chance to close the file
            if(Application.OpenForms.Count>1) {
                MessageBox.Show("You can only load new omods when obmm is displaying the data file list", "Error");
            } else {
                string[] lines=File.ReadAllLines("obmm\\pipe");
                Program.RunCommandLine(lines);
                //foreach(string s in mods) Program.LoadNewOmod(s);
                UpdateOmodList();
            }
            File.Delete("obmm\\pipe");
        }

        private void bUtilities_Click(object sender, EventArgs e) {
            cmsUtilitiesMenu.Show(bUtilities, 0, 22);
        }

        private void bSABrowserToolStripMenuItem_Click(object sender, EventArgs e) {
            (new BSABrowser()).ShowDialog();
        }

        private void loadOrderToolStripMenuItem_Click(object sender, EventArgs e) {
            System.Text.StringBuilder sb=new System.Text.StringBuilder();
            foreach(EspInfo ei in Program.Data.Esps) {
                if(ei.Active) sb.Append(ei.FileName+Environment.NewLine);
            }
            new TextEditor("Load order", sb.ToString(), false, false).ShowDialog();
        }

        private void omodReportToolStripMenuItem_Click(object sender, EventArgs e) {
            SaveOmodDialog.Title="Save report as";
            SaveOmodDialog.Filter="text file|*.txt";
            SaveOmodDialog.AddExtension=true;
            SaveOmodDialog.OverwritePrompt=true;
            if(SaveOmodDialog.ShowDialog()==DialogResult.OK) {
                StreamWriter sw=new StreamWriter(SaveOmodDialog.FileName);
                char t='\t';
                foreach(omod o in Program.Data.omods) {
                    sw.WriteLine(o.FileName+t+o.ModName+t+o.Version+t+
                        o.Author+t+o.Email+t+o.Website+t+o.CreationTime.ToString());
                }
                sw.Close();
            }
        }

        private void ActivateOmod(omod mod, bool force) {
            if(mod.Conflict!=ConflictLevel.Active) Conflicts.UpdateConflict(mod);
            bool warn=!force;
            switch(mod.Conflict) {
            case ConflictLevel.Active:
                if(!force) {
                    if(!Settings.WarnOnModDelete||MessageBox.Show("Deactivate "+mod.FileName+"?", "Question", MessageBoxButtons.YesNo)==DialogResult.Yes) {
                        mod.Deactivate(false);
                    }
                }
                return;
            case ConflictLevel.MajorConflict:
                if(!force) {
                    switch(MessageBox.Show(mod.FileName+" conflicts with an already active mod.\n"+
                        "Do you wish to be warned about any file conflicts?", "Warning", MessageBoxButtons.YesNoCancel)) {
                    case DialogResult.Yes: warn=true; break;
                    case DialogResult.No: warn=false; break;
                    case DialogResult.Cancel: return;
                    }
                }
                break;
            case ConflictLevel.Unusable:
                if(!force) {
                    switch(MessageBox.Show(mod.FileName+" will overwrite one or more existing esps.\n"+
                        "Activating this mod could cause serious problems with your game.\n"+
                        "Please use the 'view data conflicts' option to ensure this mod is not doing anything you don't expect it to before you continue\n"+
                        "Do you wish to be warned about any file conflicts?", "Warning", MessageBoxButtons.YesNoCancel)) {
                    case DialogResult.Yes: warn=true; break;
                    case DialogResult.No: warn=false; break;
                    case DialogResult.Cancel: return;
                    }
                }
                break;
            }
            try {
                mod.Activate(warn);
            } catch(Exception ex) {
                MessageBox.Show("An error occured while activating the mod.\n"+
                    "It may not have been activated completely.\n"+
                    "Error: "+ex.Message, "Error");
            }
        }

        private void activateAllToolStripMenuItem_Click(object sender, EventArgs e) {
            foreach(ListViewItem lvi in lvModList.Items) {
                ActivateOmod((omod)lvi.Tag, true);
            }
            RefreshLists();
        }

        private void deactivateAllToolStripMenuItem_Click(object sender, EventArgs e) {
            foreach(ListViewItem lvi in lvModList.Items) {
                ((omod)lvi.Tag).Deactivate(true);
            }
            RefreshLists();
        }

        private void activateAllToolStripMenuItem1_Click(object sender, EventArgs e) {
            foreach(ListViewItem lvi in lvEspList.Items) {
                ((EspInfo)lvi.Tag).Active=true;
                lvi.Checked=true;
            }
            UpdateEspList();
        }

        private void deactivateAllToolStripMenuItem1_Click(object sender, EventArgs e) {
            foreach(ListViewItem lvi in lvEspList.Items) {
                if(((EspInfo)lvi.Tag).BelongsTo==EspInfo.BaseOwner) continue;
                if(((EspInfo)lvi.Tag).Deactivatable==DeactiveStatus.Disallow) continue;
                ((EspInfo)lvi.Tag).Active=false;
                lvi.Checked=false;
            }
        }

        private void cleanAllToolStripMenuItem_Click(object sender, EventArgs e) {
            if(MessageBox.Show("This option will disable all mods and permenently delete all data files and esps which are linked to omods.\n"+
                "Are you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) {
                return;
            }
            //Deactivate everything
            foreach(omod o in Program.Data.omods) o.Deactivate(true);
            //Clear up any left over obmm stuff
            Program.Data.BSAs.Clear();
            Program.Data.DataFiles.Clear();
            Program.Data.INIEdits.Clear();
            for(int i=0;i<Program.Data.Esps.Count;i++) {
                if(Program.Data.Esps[i].Parent!=null) Program.Data.Esps.RemoveAt(i--);
            }
            //Clear out the data folder
            foreach(omod o in Program.Data.omods) o.Clean();
            TidyDataFolder();
            RefreshLists();
        }

        private void cleanFilteredToolStripMenuItem_Click(object sender, EventArgs e) {
            int DeletedCount=0;
            int SkippedCount=0;
            int NotFoundCount=0;
            foreach(ListViewItem lvi in lvModList.Items) {
                ((omod)lvi.Tag).Clean(ref DeletedCount, ref SkippedCount, ref NotFoundCount);
            }
            RefreshLists();
            MessageBox.Show("Files deleted: "+DeletedCount.ToString()+"\n"+
                "Files skipped: "+SkippedCount.ToString()+"\n"+
                "Files not found: "+NotFoundCount.ToString(), "Message");
        }

        private void tidyDataFolderToolStripMenuItem_Click(object sender, EventArgs e) { TidyDataFolder(); }

        private void bBatch_Click(object sender, EventArgs e) {
            BatchContextMenu.Show(bBatch, 0, 0);
        }

        private void bImport_Click(object sender, EventArgs e) {
            ImportContextMenu.Show(bImport, 0, 0);
        }

        private void exportOmodListToolStripMenuItem_Click(object sender, EventArgs e) {
            SaveOmodDialog.Title="Save active omod list as";
            SaveOmodDialog.Filter="Active omod list (*.oaml)|*.oaml";
            SaveOmodDialog.AddExtension=false;
            SaveOmodDialog.OverwritePrompt=false;
            if(SaveOmodDialog.ShowDialog()!=DialogResult.OK) return;
            if(Path.GetExtension(SaveOmodDialog.FileName).ToLower()!=".oaml") SaveOmodDialog.FileName+=".oaml";
            if(File.Exists(SaveOmodDialog.FileName)&&MessageBox.Show("File '"+SaveOmodDialog.FileName+"' already exists.\n"+
                    "Do you wish to overwrite?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) return;
            BinaryWriter bw=new BinaryWriter(File.Create(SaveOmodDialog.FileName));
            foreach(omod o in Program.Data.omods) {
                if(o.Conflict==ConflictLevel.Active) bw.Write(o.CRC);
            }
            bw.Close();
        }

        private void exportLoadOrderToolStripMenuItem_Click(object sender, EventArgs e) {
            SaveOmodDialog.Title="Save load order data as";
            SaveOmodDialog.Filter="Load order data (*.olod)|*.olod";
            SaveOmodDialog.AddExtension=false;
            SaveOmodDialog.OverwritePrompt=false;
            if(SaveOmodDialog.ShowDialog()!=DialogResult.OK) return;
            if(Path.GetExtension(SaveOmodDialog.FileName).ToLower()!=".olod") SaveOmodDialog.FileName+=".olod";
            if(File.Exists(SaveOmodDialog.FileName)&&MessageBox.Show("File '"+SaveOmodDialog.FileName+"' already exists.\n"+
                    "Do you wish to overwrite?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) return;
            BinaryWriter bw=new BinaryWriter(File.Create(SaveOmodDialog.FileName));
            foreach(EspInfo ei in Program.Data.Esps) {
                bw.Write(ei.LowerFileName);
                bw.Write(ei.DateModified.ToBinary());
            }
            bw.Close();
        }

        private void importOmodListToolStripMenuItem_Click(object sender, EventArgs e) {
            OpenDialog.Title="Select active omod list to import";
            OpenDialog.Filter="Active omod list (*.oaml)|*.oaml";
            OpenDialog.Multiselect=false;
            if(OpenDialog.ShowDialog()!=DialogResult.OK) return;
            BinaryReader br=new BinaryReader(File.OpenRead(OpenDialog.FileName));
            uint[] crcs=new uint[br.BaseStream.Length/4];
            int i=0;
            while(br.BaseStream.Position<br.BaseStream.Length) {
                crcs[i++]=br.ReadUInt32();
            }
            br.Close();
            foreach(omod o in Program.Data.omods) {
                if(o.Conflict==ConflictLevel.Active) {
                    if(Array.IndexOf<uint>(crcs, o.CRC)==-1) o.Deactivate(true);
                }
            }
            foreach(omod o in Program.Data.omods) {
                if(o.Conflict!=ConflictLevel.Active) {
                    try {
                        if(Array.IndexOf<uint>(crcs, o.CRC)!=-1) o.Activate(false);
                    } catch {
                        MessageBox.Show("An error occured activating "+o.FileName, "Error");
                    }
                }
            }
            RefreshLists();
        }

        private void importLoadOrderToolStripMenuItem_Click(object sender, EventArgs e) {
            OpenDialog.Title="Select load order data file to import";
            OpenDialog.Filter="Load order data (*.olod)|*.olod";
            OpenDialog.Multiselect=false;
            if(OpenDialog.ShowDialog()!=DialogResult.OK) return;
            BinaryReader br=new BinaryReader(File.OpenRead(OpenDialog.FileName));
            while(br.BaseStream.Position<br.BaseStream.Length) {
                string file=br.ReadString();
                DateTime pos=DateTime.FromBinary(br.ReadInt64());
                EspInfo ei=Program.Data.GetEsp(file);
                if(ei!=null) {
                    ei.DateModified=pos;
                    FileInfo fi=new FileInfo("data\\"+file);
                    fi.LastWriteTime=pos;
                } else {
                    MessageBox.Show("esp '"+file+"' was not found", "Error");
                }
            }
            br.Close();
            Program.Data.SortEsps();
            UpdateEspList();
        }

        private void bSACreatorToolStripMenuItem_Click(object sender, EventArgs e) {
            //MessageBox.Show("The BSA creator is incomplete, and does not generate working BSAs", "Warning");
            (new Forms.BSACreator()).ShowDialog();
        }

        private void archiveInvalidationToolStripMenuItem_Click(object sender, EventArgs e) {
            (new Forms.ArchiveInvalidation()).ShowDialog();
        }

        private void deleteBackupsToolStripMenuItem_Click(object sender, EventArgs e) {
            bool error=false;
            foreach(string s in Directory.GetFiles(Program.BackupDir, "*.omod")) {
                try { File.Delete(s); } catch { error=true; }
            }
            foreach(string s in Directory.GetFiles(Program.CorruptDir, "*.omod")) {
                try { File.Delete(s); } catch { error=true; }
            }
            if(error) {
                MessageBox.Show("An error occured, and one or more files could not be deleted.\n"+
                    "Make sure that none of the files in the backup or corrupt folders are write protected.",
                    "Error");
            }
        }

        private void editToolStripMenuItem_Click(object sender, EventArgs e) {
            bEdit_Click(null, null);
        }

        private void updateConflictsToolStripMenuItem_Click(object sender, EventArgs e) {
            Conflicts.UpdateConflicts();
            UpdateEspList();
            UpdateOmodList();
        }

        private void exportOmodGroupInformationToolStripMenuItem_Click(object sender, EventArgs e) {
            SaveOmodDialog.Title="Save group information as";
            SaveOmodDialog.Filter="omod group information (*.ogi)|*.ogi";
            SaveOmodDialog.AddExtension=true;
            SaveOmodDialog.OverwritePrompt=true;
            if(SaveOmodDialog.ShowDialog()!=DialogResult.OK) return;

            System.Runtime.Serialization.Formatters.Binary.BinaryFormatter formatter;
            formatter=new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();

            BinaryWriter bw=new BinaryWriter(File.Create(SaveOmodDialog.FileName));
            bw.Write(Settings.omodGroups.Count);
            foreach(omodGroup og in Settings.omodGroups) { og.Write(bw, formatter); }
            bw.Write(Program.Data.omods.Count);
            foreach(omod o in Program.Data.omods) { bw.Write(o.LowerFileName); bw.Write(o.group); }
            bw.Close();
        }

        private void importOmodGroupInfoToolStripMenuItem_Click(object sender, EventArgs e) {
            OpenDialog.Title="Select omod group information to import";
            OpenDialog.Filter="Active omod list (*.ogi)|*.ogi";
            OpenDialog.Multiselect=false;
            if(OpenDialog.ShowDialog()!=DialogResult.OK) return;
            Settings.omodGroups.Clear();
            foreach(omod o in Program.Data.omods) {
                o.group=0;
            }

            BinaryReader br=new BinaryReader(File.OpenRead(OpenDialog.FileName));
            System.Runtime.Serialization.Formatters.Binary.BinaryFormatter formatter;
            formatter=new System.Runtime.Serialization.Formatters.Binary.BinaryFormatter();
            int count;

            count=br.ReadInt32();
            for(int i=0;i<count;i++) Settings.omodGroups.Add(omodGroup.Read(br, formatter));
            count=br.ReadInt32();
            for(int i=0;i<count;i++) {
                omod o=Program.Data.GetMod(br.ReadString());
                ulong group=br.ReadUInt64();
                if(o!=null) o.group=group;
            }
            br.Close();

            UpdateOmodList();
            UpdateGroups();
        }

        private void bSAFixerToolStripMenuItem_Click(object sender, EventArgs e) {
            OpenDialog.Title="Select BSA(s) to check";
            OpenDialog.Filter="BethesdaSoftworks archive (*.bsa)|*.bsa";
            OpenDialog.Multiselect=true;
            if(OpenDialog.ShowDialog()!=DialogResult.OK) return;
            Classes.BSAUncorrupter.BSAFixResult result=Classes.BSAUncorrupter.ScanBSA(OpenDialog.FileNames);
            string message;
            if(OpenDialog.FileNames.Length>1) message=OpenDialog.FileNames.Length.ToString()+" files checked";
            else message="'"+OpenDialog.FileName+"' checked";
            message+="\nFolder found: "+result.foldersFound+
                "\nCorrupted folders found and fixed: "+result.foldersFixed+
                "\nFiles found: "+result.filesFound+
                "\nCorrupted files found and fixed: "+result.filesFixed+
                "\nFiles possibly still corrupted: "+result.Failed;
            MessageBox.Show(message, "Results");
        }

        private void dataFileBrowserToolStripMenuItem_Click(object sender, EventArgs e) {
            bool b=MessageBox.Show("Do you want to include files packed inside BSAs?\n"+
                "Warning: Choosing 'Yes' will drastically slow down this utility", "Question", MessageBoxButtons.YesNo)==DialogResult.Yes;
            (new Forms.DataFileBrowser(b)).ShowDialog();
        }

        private void conflictReportToolStripMenuItem1_Click(object sender, EventArgs e) {
            (new ConflictReport.NewReportGenerator()).ShowDialog();
        }

        private void rescanEspHeadersToolStripMenuItem_Click(object sender, EventArgs e) {
            foreach(EspInfo ei in Program.Data.Esps) { if(ei.Parent==null) { ei.GetHeader(); } }
            UpdateEspList();
        }

        private void hideInactiveFToolStripMenuItem_Click(object sender, EventArgs e) {
            pictureBox1.Image=null;
            foreach(ListViewItem lvi in lvModList.Items) {
                omod o=(omod)lvi.Tag;
                if(o.Conflict!=ConflictLevel.Active) o.Hide();
            }
            UpdateOmodList();
        }

        private void aquisitionActivateFilteredToolStripMenuItem_Click(object sender, EventArgs e) {
            foreach(ListViewItem lvi in lvModList.Items) {
                omod o=(omod)lvi.Tag;
                if(o.Conflict!=ConflictLevel.Active) o.AquisitionActivate(false);
            }
            UpdateOmodList();
        }

        private void hiddenOmodSwitcherToolStripMenuItem_Click(object sender, EventArgs e) {
            pictureBox1.Image=null;
            new Forms.omodEnabler().ShowDialog();
            UpdateOmodList();
        }

        private void hideToolStripMenuItem_Click(object sender, EventArgs e) {
            if(lvModList.SelectedItems.Count!=1) return;
            pictureBox1.Image=null;
            omod o=(omod)lvModList.SelectedItems[0].Tag;
            o.Hide();
            UpdateOmodList();
        }

        private void nifViewerToolStripMenuItem_Click(object sender, EventArgs e) {
            if(!File.Exists("obmm\\NifViewer.exe")) {
                MessageBox.Show("The nifviewer utility could not be found","Error");
                return;
            }
            /*try {
                System.Reflection.Assembly asm=System.Reflection.Assembly.ReflectionOnlyLoad(@"Microsoft.DirectX.Direct3DX, Version=1.0.2911.0, Culture=neutral, PublicKeyToken=31bf3856ad364e35");
            } catch {
                MessageBox.Show("You do not have the required version of MDX installed.\n"+
                    "Please install the october 2005 DirectX update or newer, and ensure the MDX component is selected to be installed.", "Error");
            }*/
            System.Diagnostics.Process.Start("obmm\\NifViewer.exe");
        }

        private void lvEspList_GiveFeedback(object sender, GiveFeedbackEventArgs e) {
            Point p=lvEspList.PointToClient(Form.MousePosition);
            ListViewItem lvi=lvEspList.GetItemAt(p.X, p.Y);
            if(lvi==null) lvEspList.SelectedIndices.Clear();
            else lvi.Selected=true;
        }

        private void lvEspList_DragDrop(object sender, DragEventArgs e) {
            if(lvEspList.SelectedIndices.Count!=1) return;
            int toswap=(int)e.Data.GetData(typeof(int)) - 1;
            if(toswap==-1) return;
            int swapwith=lvEspList.SelectedIndices[0];
            if(toswap==swapwith) return;
            if(swapwith>toswap) {
                for(int i=0;i<swapwith-toswap;i++) {
                    if(!SwitchEsps(toswap+i)) break;
                }
            } else {
                for(int i=0;i<toswap-swapwith;i++) {
                    if(!SwitchEsps(toswap-(i+1))) break;
                }
            }
        }
        
        private bool DragDropInProgress=false;
        private void lvEspList_ItemDrag(object sender, ItemDragEventArgs e) {
            if(lvEspList.SelectedIndices.Count!=1||e.Button!=MouseButtons.Left) return;
            if(EspListSorter.order!=EspSortOrder.LoadOrder) return;
            DragDropInProgress=true;
            lvEspList.DoDragDrop(lvEspList.SelectedIndices[0]+1, DragDropEffects.Move);
        }

        private void lvEspList_DragEnter(object sender, DragEventArgs e) {
            if(!DragDropInProgress) return;
            e.Effect=DragDropEffects.Move;
            DragDropInProgress=false;
        }
    }
}