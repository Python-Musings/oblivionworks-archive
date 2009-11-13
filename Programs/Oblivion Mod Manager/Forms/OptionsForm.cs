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

namespace OblivionModManager {
    internal partial class OptionsForm : Form {
        internal OptionsForm() {
            InitializeComponent();
            toolTip.SetToolTip(cbLockFOV, "Resets the field of view setting in oblivion.ini to 75 each time obmm is closed down");
            toolTip.SetToolTip(cbOmodInfo, "If checked, information about an omod will be displayed when it is double clicked in windows explorer, instead of being automatically installed");
            toolTip.SetToolTip(cbIniWarn, "If checked, omod scripts will require your permission to make modifications to oblivion.ini");
            toolTip.SetToolTip(cbDataWarnings, "Unchecking this disables several miscallanious confirmation dialogs, such as the 'are you sure you want to deactivate this omod' prompt");
            toolTip.SetToolTip(cbWarnings, "If checked, obmm will display warning dialogs if it encounters errors while running omod scripts");
            toolTip.SetToolTip(cbShowEspWarnings, "If checked, obmm will display a confirmation dialog if you try and uncheck an esp which is part of an omod");
            toolTip.SetToolTip(cbCompressionBoost, "If checked, 7-zips created when converting omods to normal archives will use -mx9 compression instead of -mx7, and also omod compression will be slightly greater."+
                Environment.NewLine+"Uncheck this to speed up omod creation");
            toolTip.SetToolTip(cbTrackConflicts, "If checked, obmm will keep track of which omods contain shared data files, and colour code them orange/red/black as appropriate"+
                Environment.NewLine+"Uncheck this to greatly speed up obmm");
            toolTip.SetToolTip(cbUpdateEsps, "If checked, obmm will rescan the TES4 record in all unparented esps each startup."+
                Environment.NewLine+"Uncheck this to speed up obmm's start up time"+
                Environment.NewLine+"Unchecking this may cause the author and description of unparented esps to get out of date");
            toolTip.SetToolTip(cbAutoupdateConflicts, "If checked, does the equivilent of clicking 'Batch actions|Update conflicts' every time an omod is activated or deactivated"+
                Environment.NewLine+"Uncheck this to speed up omod activation/deactivation");
            toolTip.SetToolTip(cbUseKiller, "Enabled the use of obmm's process killer"+
                Environment.NewLine+"Please read obmm's readme before use");
            toolTip.SetToolTip(cbSafeMode, "If checked, obmm operates in its safe mode"+
                Environment.NewLine+"Uncheck this to greatly speed up obmm's start up time"+
                Environment.NewLine+"Unchecking this will cause compatability problem with other utilities such as Wyre Bash"+
                Environment.NewLine+"Do not uncheck this if you intend to add or remove files from your data folder by hand");
            toolTip.SetToolTip(cbAdvSelectMany, "If checked, allows you to use windows explorer style shift/control shortcuts on an obmm scripts SelectMany dialogs");
            toolTip.SetToolTip(cbNewEspsLoadLast, "If checked, esps newly unpacked from omods are added to the very end of your load order"+
                Environment.NewLine+"If unchecked, new esps are timestamped according to the current date/time, and so will load before any esps timestamps in the future");
            toolTip.SetToolTip(cbAllowInsecureScripts, "If checked, omods are allowed to use python, C# or vb for scripting");
            toolTip.SetToolTip(cbNeverModifyLoadOrder, "If checked, obmm will never chance the last modified date on an esp, even if you or a script tries to change the load order");
            SetupForm();
        }

        private void SetupForm() {
            cmbGroups.Items.Clear();
            cmbGroups.Items.Add("None");
            cmbGroups.Items.AddRange(Settings.omodGroups.ToArray());

            cbWarnings.Checked=Settings.ShowScriptWarnings;
            cbIniWarn.Checked=Settings.WarnOnINIEdit;
            cbDataWarnings.Checked=Settings.WarnOnModDelete;
            cbOmodInfo.Checked=Settings.ShowNewModInfo;
            cbLockFOV.Checked=Settings.LockFOV;
            cbShowEspWarnings.Checked=Convert.ToBoolean((byte)Settings.DefaultEspWarn);
            tbCommandLine.Text=Settings.OblivionCommandLine;
            cbCompressionBoost.Checked=Settings.CompressionBoost;
            cbTrackConflicts.Checked=Settings.TrackConflicts;
            cbAutoupdateConflicts.Checked=Settings.AutoUpdateConflicts;
            cbUpdateEsps.Checked=Settings.UpdateEsps;
            InFunction_cbUseKiller_CheckedChanged=true;
            cbUseKiller.Checked=(Settings.UseProcessKiller==1);
            InFunction_cbUseKiller_CheckedChanged=false;
            cbSafeMode.Checked=Settings.SafeMode;
            cbNewEspsLoadLast.Checked=Settings.NewEspsLoadLast;
            cbAdvSelectMany.Checked=Settings.AdvSelectMany;
            cbAllowInsecureScripts.Checked=Settings.AllowInsecureScripts;
            cbNeverModifyLoadOrder.Checked=Settings.NeverTouchLoadOrder;

            if(!cbSafeMode.Checked) cbUpdateEsps.Enabled=false; else cbUpdateEsps.Enabled=true;
            if(!cbTrackConflicts.Checked) {
                cbAutoupdateConflicts.Checked=false;
                cbAutoupdateConflicts.Enabled=false;
            } else cbAutoupdateConflicts.Enabled=true;
        }

        private int SelectedGroup=0;

        private void cbWarnings_CheckedChanged(object sender, EventArgs e) {
            Settings.ShowScriptWarnings=cbWarnings.Checked;
        }

        private void cmbGroups_SelectedIndexChanged(object sender, EventArgs e) {
            SelectedGroup=cmbGroups.SelectedIndex;
            bAdd.Enabled=false;
            bRenameGroup.Enabled=false;
            if(cmbGroups.SelectedIndex!=0) {
                bDelete.Enabled=true;
                bMoveToEnd.Enabled=true;
                bSetFont.Enabled=true;
            } else {
                bDelete.Enabled=false;
                bMoveToEnd.Enabled=false;
                bSetFont.Enabled=false;
            }
        }

        private void cmbGroups_TextChanged(object sender, EventArgs e) {
            bAdd.Enabled=true;
            if(SelectedGroup!=0) bRenameGroup.Enabled=true;
            bDelete.Enabled=false;
            bMoveToEnd.Enabled=false;
            bSetFont.Enabled=false;
        }

        private void bMoveToEnd_Click(object sender, EventArgs e) {
            bool[] bools=new bool[Program.Data.omods.Count];
            int index=cmbGroups.SelectedIndex-1;
            for(int i=0;i<Program.Data.omods.Count;i++) {
                if((Program.Data.omods[i].group&(ulong)((ulong)1<<index))>0) bools[i]=true;
                else bools[i]=false;
            }
            string name=cmbGroups.Text;
            bDelete_Click(null, null);
            cmbGroups.Text=name;
            bAdd_Click(null, null);
            index=cmbGroups.Items.Count-2;
            for(int i=0;i<Program.Data.omods.Count;i++) {
                if(bools[i]) {
                    Program.Data.omods[i].group|=(ulong)((ulong)1<<index);
                } else {
                    Program.Data.omods[i].group&=0xFFFFFFFFFFFFFFFF - (ulong)((ulong)1<<index);
                }
            }
        }

        private void bSetFont_Click(object sender, EventArgs e) {
            if(Settings.omodGroups[SelectedGroup-1].Font!=null) {
                GroupFontDialog.Color=Settings.omodGroups[SelectedGroup-1].Color;
                GroupFontDialog.Font=Settings.omodGroups[SelectedGroup-1].Font;
            }
            if(GroupFontDialog.ShowDialog()==DialogResult.OK) {
                Settings.omodGroups[SelectedGroup-1].SetFont(GroupFontDialog.Font,GroupFontDialog.Color);
            }
        }

        private void bDelete_Click(object sender, EventArgs e) {
            int i=cmbGroups.SelectedIndex;
            if(i==-1) return;
            cmbGroups.Items.RemoveAt(i);
            Settings.omodGroups.RemoveAt(i-1);
            bDelete.Enabled=false;
            ulong bit=(ulong)(1<<i);
            foreach(omod o in Program.Data.omods) {
                ulong n=0;
                for(int i2=0;i2<i-1;i2++) {
                    if((o.group&(ulong)((ulong)1<<i2))>0) {
                        n|=(ulong)((ulong)1<<i2);
                    }
                }
                for(int i2=i;i2<64;i2++) {
                    if((o.group&(ulong)((ulong)1<<i2))>0) {
                        n|=(ulong)((ulong)1<<(i2-1));
                    }
                }
                o.group=n;
            }
        }

        private void bAdd_Click(object sender, EventArgs e) {
            if(cmbGroups.Items.Count>=60) {
                MessageBox.Show("You can only create a maximum of 60 groups", "Error");
                return;
            }
            if(cmbGroups.Items.Contains(cmbGroups.Text)) {
                MessageBox.Show("A group with that name already exists","Error");
                return;
            }
            cmbGroups.Items.Add(cmbGroups.Text);
            Settings.omodGroups.Add(new omodGroup(cmbGroups.Text));
            bAdd.Enabled=false;
        }

        private void bRenameGroup_Click(object sender, EventArgs e) {
            cmbGroups.Items[SelectedGroup]=cmbGroups.Text;
            Settings.omodGroups[SelectedGroup-1].Rename(cmbGroups.Text);
            bRenameGroup.Enabled=false;
        }

        private void bClearGroups_Click(object sender, EventArgs e) {
            cmbGroups.SelectedIndex=-1;
            cmbGroups.Items.Clear();
            cmbGroups.Items.Add("None");
            Settings.omodGroups.Clear();
            foreach(omod o in Program.Data.omods) {
                o.group=0;
            }

        }

        private void cbIniWarn_CheckedChanged(object sender, EventArgs e) {
            Settings.WarnOnINIEdit=cbIniWarn.Checked;
        }

        private void cbDataWarnings_CheckedChanged(object sender, EventArgs e) {
            Settings.WarnOnModDelete=cbDataWarnings.Checked;
        }

        private void cbOmodInfo_CheckedChanged(object sender, EventArgs e) {
            Settings.ShowNewModInfo=cbOmodInfo.Checked;
        }

        private void cbLockFOV_CheckedChanged(object sender, EventArgs e) {
            Settings.LockFOV=cbLockFOV.Checked;
        }

        private void bMoveModFolder_Click(object sender, EventArgs e) {
            if(MessageBox.Show("Please ensure that you have sufficent space in the new folder to move your whole mod collection.\n"+
                "Do you wish to continue?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) return;
            if(omodDirDialog.ShowDialog()==DialogResult.OK) {
                foreach(string s in System.IO.Directory.GetFiles(Settings.omodDir,"*.omod")) {
                    foreach(omod o in Program.Data.omods) {
                        o.Close();
                    }
                    System.IO.File.Move(s, omodDirDialog.SelectedPath+"\\"+System.IO.Path.GetFileName(s));
                }
                Settings.omodDir=omodDirDialog.SelectedPath.ToLower();
                if(!Settings.omodDir.EndsWith("\\")) Settings.omodDir+="\\";
            }
        }

        private void cbShowEspWarnings_CheckedChanged(object sender, EventArgs e) {
            if(cbShowEspWarnings.Checked) {
                Settings.DefaultEspWarn=DeactiveStatus.WarnAgainst;
            } else {
                Settings.DefaultEspWarn=DeactiveStatus.Allow;
            }
        }

        private void tbCommandLine_TextChanged(object sender, EventArgs e) {
            Settings.OblivionCommandLine=tbCommandLine.Text;
        }

        private void cmbArchiveMode_KeyPress(object sender, KeyPressEventArgs e) {
            e.Handled=true;
        }

        private void cbCompressionBoost_CheckedChanged(object sender, EventArgs e) {
            Settings.CompressionBoost=cbCompressionBoost.Checked;
        }

        private void cbAutoupdateConflicts_CheckedChanged(object sender, EventArgs e) {
            Settings.AutoUpdateConflicts=cbAutoupdateConflicts.Checked;
        }

        private void cbUpdateEsps_CheckedChanged(object sender, EventArgs e) {
            Settings.UpdateEsps=cbUpdateEsps.Checked;
        }

        private void bMoveTempFolder_Click(object sender, EventArgs e) {
            string s=omodDirDialog.Description;
            omodDirDialog.Description="Select the folder used for temporary storage of extracted omod files.";
            if(omodDirDialog.ShowDialog()==DialogResult.OK) {
                if(!System.IO.Directory.Exists(omodDirDialog.SelectedPath+"\\obmm")||
                    MessageBox.Show("The contents of '"+omodDirDialog.SelectedPath+"\\obmm' will be overwritten.\n"+
                        "Continue?", "Warning", MessageBoxButtons.YesNo)==DialogResult.Yes) {
                    Program.ClearTempFiles();
                    try { System.IO.Directory.Delete(Program.TempDir); } catch { }
                    Settings.tempDir=omodDirDialog.SelectedPath.ToLower()+"\\obmm\\";
                    Program.ClearTempFiles();
                }
            }
            omodDirDialog.Description=s;
        }

        private void bResetTempFolder_Click(object sender, EventArgs e) { Settings.tempDir=""; }
        
        private static bool InFunction_cbUseKiller_CheckedChanged=false;
        private void cbUseKiller_CheckedChanged(object sender, EventArgs e) {
            if(InFunction_cbUseKiller_CheckedChanged) return;
            InFunction_cbUseKiller_CheckedChanged=true;
            if(!cbUseKiller.Checked) {
                if(Settings.UseProcessKiller==1) Settings.UseProcessKiller=0;
            } else {
                if(Settings.UseProcessKiller==2) {
                    if(Program.KeyPressed(16)&&Program.KeyPressed(17)) Settings.UseProcessKiller=0;
                }
                if(Settings.UseProcessKiller==0) {
                    if(MessageBox.Show("This feature is NOT intended for novice users.\n"+
                        "Make sure you read and _understand_ the appropriate section of obmm's manual before use.\n"+
                        "It is expected that you know what a process and service is, the difference between closing "+
                        "and killing a process, that doing something like shutting down the themes service will cause "+
                        "windows xp to revert to the classic style interface, etc.\n"+
                        "Using this utility incorrectly can cause system instability, loss of data and other serious side effects.\n"+
                        "Also remember that any decent security software will not allow itself to be shut down by an external application.\n"+
                        "If you still wish to use this, please hold shift while clicking OK.\n"+
                        "Clicking OK without holding shift will be taken as an indication that you do not read warning messages, and this "+
                        "utility will be permanently disabled for your own safety.\n"+
                        "Clicking cancel will result in no changes being made.", "Big Huge Massive Warning!", MessageBoxButtons.OKCancel)==
                        DialogResult.OK) {
                        if(Program.KeyPressed(16)) {
                            Settings.UseProcessKiller=1;
                        } else {
                            Settings.UseProcessKiller=2;
                            cbUseKiller.Checked=false;
                        }
                    } else cbUseKiller.Checked=false;
                } else if(Settings.UseProcessKiller==2) {
                    cbUseKiller.Checked=false;
                    MessageBox.Show("You have previously chosen to permenently disable this feature", "Error");
                }
            }
            InFunction_cbUseKiller_CheckedChanged=false;
        }

        private void cbSafeMode_CheckedChanged(object sender, EventArgs e) {
            if(!cbSafeMode.Checked) {
                if(MessageBox.Show(
                    "If you ever make modifications to your data directory by hand, you MUST leave obmm in safe mode.\n"+
                    "Adding or removing files from your data folder while in unsafe mode can cause corruption of obmm's save file, requiring a reinstall.\n"+
                    "Please see the manual for exact information about which checks obmm will not perform while in unsafe mode.\n"+
                    "If you put obmm into unsafe mode but accidently make changes to your data folder, use the '-safe' command line option "+
                        "the next time you start obmm.", "Warning!", MessageBoxButtons.OKCancel)!=DialogResult.OK) {
                    cbSafeMode.Checked=false;
                }
            }
            Settings.SafeMode=cbSafeMode.Checked;
            cbUpdateEsps.Enabled=cbSafeMode.Checked;
        }

        private void bReset_Click(object sender, EventArgs e) {
            if(MessageBox.Show("This will reset all options, including those not displayed on this page.\n"+
                "It will also strip omods of their group information.\n"+
                "Are you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo)==DialogResult.Yes) {
                bClearGroups_Click(null, null);
                Settings.DefaultSettings();
                SetupForm();
            }
        }

        private void cbNewEspsLoadLast_CheckedChanged(object sender, EventArgs e) {
            Settings.NewEspsLoadLast=cbNewEspsLoadLast.Checked;
        }

        private void cbAdvSelectMany_CheckedChanged(object sender, EventArgs e) {
            Settings.AdvSelectMany=cbAdvSelectMany.Checked;
        }

        private void cbTrackConflicts_CheckedChanged(object sender, EventArgs e) {
            Settings.TrackConflicts=cbTrackConflicts.Checked;
            if(cbTrackConflicts.Checked) {
                cbAutoupdateConflicts.Enabled=true;
            } else {
                cbAutoupdateConflicts.Checked=false;
                cbAutoupdateConflicts.Enabled=false;
            }
        }

        private void cbAllowInsecureScripts_CheckedChanged(object sender, EventArgs e) {
            Settings.AllowInsecureScripts=cbAllowInsecureScripts.Checked;
        }

        private void cbNeverModifyLoadOrder_CheckedChanged(object sender, EventArgs e) {
            Settings.NeverTouchLoadOrder=cbNeverModifyLoadOrder.Checked;
        }

    }
}