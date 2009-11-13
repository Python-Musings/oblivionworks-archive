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
using System.IO;
using System.Windows.Forms;
using System.Collections.Generic;

namespace OblivionModManager {
    internal partial class CreateModForm : Form {
        private omodCreationOptions ops=new omodCreationOptions();
        private bool EditingPlugins=true;

        private string InitialName;

        internal void CreateModFormShared() {
            InitializeComponent();
            DialogResult=DialogResult.Cancel;
            //Set dud context menus
            cmbCompType.ContextMenu=DudMenu;
            cmbDataCompLevel.ContextMenu=DudMenu;
            cmbModCompLevel.ContextMenu=DudMenu;
            //Set the combo box indexes
            cmbCompType.SelectedIndex=0;
            cmbDataCompLevel.SelectedIndex=1;
            cmbModCompLevel.SelectedIndex=0;
            //Init arrays
            ops.espPaths=new string[0];
            ops.esps=new string[0];
            ops.DataFilePaths=new string[0];
            ops.DataFiles=new string[0];
            //cms groups icons
            for(int i=0;i<Settings.omodGroups.Count;i++) {
                ToolStripMenuItem tsmi=new ToolStripMenuItem(Settings.omodGroups[i].Name);
                tsmi.CheckOnClick=true;
                cmsGroups.Items.Add(tsmi);
            }
            try {
                cmbDataCompLevel.SelectedIndex=(byte)Settings.dataCompressionLevel;
                cmbModCompLevel.SelectedIndex=(byte)Settings.omodCompressionLevel;
            } catch {
                MessageBox.Show("Invalid compression level.\n"+
                    "This could be the result of a corrupted save file.\n"+
                    "If you notice any other strange behaviour, use the reset button on the options page to reset your settings to their defaults.", "Error");
                cmbDataCompLevel.SelectedIndex=1;
                cmbModCompLevel.SelectedIndex=0;
            }
        }
        internal CreateModForm() {
            CreateModFormShared();
            //Set some default options
            ops.readme="";
            ops.script="";
            ops.Description="";
            cmbCompType.SelectedIndex=(byte)Settings.dataCompressionType;
            //Make sure the file will generate successfully
            if(File.Exists(Settings.omodDir+tbName.Text+" 1.0.omod")) {
                int i=1;
                while(File.Exists(Settings.omodDir+tbName.Text+i.ToString()+" 1.0.omod")) i++;
                tbName.Text+=i.ToString();
            }
            InitialName=tbName.Text;
        }
        internal CreateModForm(omod o) {
            CreateModFormShared();
            ops.readme=o.GetReadme();
            ops.script=o.GetScript();
            ops.Description=o.Description;
            if(ops.readme==null) ops.readme="";
            if(ops.script==null) ops.script="";
            if(ops.Description==null) ops.Description="";
            ScreenshotPic.Image=o.image;
            ops.Image=o.GetImage();
            tbName.Text=o.ModName;
            tbVersion.Text=o.Version;
            tbAuthor.Text=o.Author;
            tbEmail.Text=o.Email;
            tbWebsite.Text=o.Website;
            cmbCompType.SelectedIndex=(int)o.CompType;
            string s=o.GetPlugins();
            if(s!=null) AddFilesFromFolder(s);
            s=o.GetDataFiles();
            if(s!=null) AddFilesFromFolder(s);
            for(int i=0;i<cmsGroups.Items.Count;i++) {
                if((o.group&(ulong)(1<<i)) > 0) ((ToolStripMenuItem)cmsGroups.Items[i]).Checked=true;
            }
        }

        internal static bool ShowForm() {
            CreateModForm cfm=new CreateModForm();
            if(cfm.ShowDialog()==DialogResult.OK) return true;
            return false;
        }
        internal static bool ShowForm(omod o) {
            CreateModForm cfm=new CreateModForm(o);
            if(cfm.ShowDialog()==DialogResult.OK) return true;
            return false;
        }

        private void DigitKeysOnly(object sender, KeyPressEventArgs e) {
            if(e.KeyChar!='.'&&e.KeyChar!='\b'&&!char.IsDigit(e.KeyChar)) e.Handled=true;
        }

        private void cmbNoKeyPress(object sender, KeyPressEventArgs e) {
            e.Handled=true;
        }

        private void bEdReadme_Click(object sender, EventArgs e) {
            TextEditor te=new TextEditor("Edit readme", ops.readme, true, true);
            if(te.ShowDialog()==DialogResult.Yes) ops.readme=te.Result;
        }

        private void bEdScript_Click(object sender, EventArgs e) {
            Forms.ScriptEditor se=new Forms.ScriptEditor(ops.script);
            if(se.ShowDialog()==DialogResult.Yes) ops.script=se.Result;
        }

        private void bEdDescription_Click(object sender, EventArgs e) {
            TextEditor te=new TextEditor("Edit description", ops.Description, false, true);
            if(te.ShowDialog()==DialogResult.Yes) ops.Description=te.Result;
        }

        private void SaveListboxContents() {
            //Some sanity checking and security
            for(int i=0;i<lvFiles.Items.Count;i++) {
                if(lvFiles.Items[i].Text=="") lvFiles.Items.RemoveAt(i--);
                else if(!Program.IsSafeFileName(lvFiles.Items[i].Text)) lvFiles.Items.RemoveAt(i--);
                lvFiles.Items[i].Text=lvFiles.Items[i].Text.Replace('/', '\\');
            }
            //Update the file lists
            string[] Files=new string[lvFiles.Items.Count];
            string[] Paths=new string[lvFiles.Items.Count];
            for(int i=0;i<lvFiles.Items.Count;i++) {
                Files[i]=lvFiles.Items[i].SubItems[1].Text;
                Paths[i]=lvFiles.Items[i].Text;
            }
            if(EditingPlugins) {
                ops.esps=Files;
                ops.espPaths=Paths;
            } else {
                ops.DataFiles=Files;
                ops.DataFilePaths=Paths;
            }
        }

        private void bCreate_Click(object sender, EventArgs e) {
            Settings.dataCompressionType=(CompressionType)cmbCompType.SelectedIndex;
            Settings.dataCompressionLevel=(CompressionLevel)cmbDataCompLevel.SelectedIndex;
            Settings.omodCompressionLevel=(CompressionLevel)cmbModCompLevel.SelectedIndex;
            tbVersion_Leave(null, null);
            ops.Name=tbName.Text;
            string omodfname;
            if(cbIncludeVersion.Checked) {
                omodfname=ops.Name+" "+ops.MajorVersion;
                if(ops.MinorVersion!=-1) {
                    omodfname+="."+ops.MinorVersion;
                    if(ops.BuildVersion!=-1) omodfname+="."+ops.BuildVersion;
                }
                omodfname+=".omod";
            } else {
                omodfname=ops.Name+".omod";
            }
            //Sanity check and some warnings
            if(tbName.Text=="") {
                MessageBox.Show("You must enter a mod name", "Error");
                return;
            }
            if(tbName.Text.IndexOfAny(Path.GetInvalidFileNameChars())!=-1) {
                MessageBox.Show("The mod name contains invalid characters\n"+
                    "The mod name may not contain '/', '\\', ':', '*', '?', '\"', '<', '>' or '|'", "Error");
            }
            if(tbEmail.Text.IndexOf('?')!=-1) {
                MessageBox.Show("Illegal character in email address", "Error");
                return;
            }
            if(File.Exists(Settings.omodDir+omodfname)) {
                if(MessageBox.Show("These settings will create a file with the same name as an already existing one.\n"+
                    "Do you wish to overwrite it?", "Error", MessageBoxButtons.YesNo)!=DialogResult.Yes) {
                    return;
                } else {
                    if(File.Exists(Program.BackupDir+omodfname)) File.Delete(Program.BackupDir+omodfname);
                    omod.Remove(omodfname);
                    File.Move(Settings.omodDir+omodfname, Program.BackupDir+omodfname);
                }
            }
            if(tbAuthor.Text=="") {
                if(MessageBox.Show("You haven't entered an author name.\n"+
                    "Are you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) {
                    return;
                }
            }
            if(ops.Description=="") {
                if(MessageBox.Show("You haven't created a description.\n"+
                    "Are you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) {
                    return;
                }
            }
            validateToolStripMenuItem_Click(null, null);
            //if(rbData.Checked) rbPlugins.Checked=true; else rbData.Checked=true;
            validateToolStripMenuItem_Click(null, null);
            SaveListboxContents();
            //Set the rest of the options
            ops.CompressionType=(CompressionType)cmbCompType.SelectedIndex;
            ops.DataFileCompresionLevel=(CompressionLevel)cmbDataCompLevel.SelectedIndex;
            ops.omodCompressionLevel=(CompressionLevel)cmbModCompLevel.SelectedIndex;
            ops.Author=tbAuthor.Text.Trim();
            ops.website=tbWebsite.Text.Trim();
            ops.email=tbEmail.Text.Trim();
            ulong groups=0;
            for(int i=0;i<cmsGroups.Items.Count;i++) {
                if(((ToolStripMenuItem)cmsGroups.Items[i]).Checked) groups|=(ulong)((ulong)0x01<<i);
            }
            try {
                Enabled=false;
                omod.CreateOmod(ops, omodfname, groups);
                MessageBox.Show("omod created successfully", "Message");
                DialogResult=DialogResult.OK;
                Close();
            }catch(obmmCancelException) {
                Enabled=true;
                Focus();
                try { File.Delete(Settings.omodDir+omodfname); } catch { }
            } catch(Exception ex) {
                Enabled=true;
                Focus();
                MessageBox.Show("An error occured while creating the omod file.\n"+ex.Message, "Error");
                try { File.Delete(Settings.omodDir+omodfname); } catch { }
            }
            GC.Collect();
        }

        private void UpdateListView() {
            lvFiles.Items.Clear();
            if(EditingPlugins) {
                for(int i=0;i<ops.esps.Length;i++) {
                    lvFiles.Items.Add(new ListViewItem(new string[] { ops.espPaths[i], ops.esps[i] }));
                }
            } else {
                for(int i=0;i<ops.DataFiles.Length;i++) {
                    lvFiles.Items.Add(new ListViewItem(new string[] { ops.DataFilePaths[i], ops.DataFiles[i] }));
                }
            }
        }

        private void rbCheckedChanged(object sender, EventArgs e) {
            if(!((RadioButton)sender).Checked) return;
            //Causes rediculus slowdown
            //RemoveListBoxDuplicates();
            SaveListboxContents();
            if(rbPlugins.Checked) {
                EditingPlugins=true;
                bUp.Enabled=true;
                bDown.Enabled=true;
            } else {
                EditingPlugins=false;
                bUp.Enabled=false;
                bDown.Enabled=false;
            }
            UpdateListView();
        }

        private void bAdd_Click(object sender, EventArgs e) {
            OpenDialog.Multiselect=true;
            if(rbPlugins.Checked) {
                OpenDialog.Title="Select plugins to add";
                OpenDialog.Filter="plugin files (*.esp,*.esm)|*.esp;*.esm";
            } else {
                OpenDialog.Title="Select other files to add";
                OpenDialog.Filter="All files|*.*";
            }
            if(OpenDialog.ShowDialog()==DialogResult.OK) {
                foreach(string s in OpenDialog.FileNames) {
                    string path=s.ToLower();
                    if(rbData.Checked) {
                        if(path.StartsWith(Program.CurrentDir+"data\\")) {
                            path=s.Substring((Program.CurrentDir+"data\\").Length);
                        } else if(path.StartsWith("data\\")) {
                            path=s.Substring(5);
                        } else {
                            path=Path.GetFileName(s);
                        }
                    } else {
                        path=Path.GetFileName(s);
                    }
                    lvFiles.Items.Add(new ListViewItem(new string[] { path, s }));
                }
            }
        }

        private void AddFilesFromFolder(string folder) {
            if(!Path.IsPathRooted(folder)) folder=Path.GetFullPath(folder);
            string readme=null;
            {
                string[] Files;
                SaveListboxContents();
                //Check for esps in subdirectorys
                foreach(string dir in Directory.GetDirectories(folder)) {
                    if((Directory.GetFiles(dir, "*.esp", SearchOption.AllDirectories).Length+
                        Directory.GetFiles(dir, "*.esm", SearchOption.AllDirectories).Length)>0) {
                        if(MessageBox.Show("Warning - this folder contains esp files in sub directories.\n"+
                            "It is possible that this mod has been packed in a non standard way, or that it contains multiple versions.\n"+
                            "You may have to attach a script to this omod to tell obmm what to do with any optional esps.\n"+
                            "If you do not know how to script, copy the mod to a new folder and set it up as you'd like, and then use 'Add folder'\n"+
                            "Do you wish to continue?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) return;
                        break;
                    }
                }
                //readme
                Files=Directory.GetFiles(folder, "*readme*.txt");
                if(Files.Length==0) Files=Directory.GetFiles(folder, "*readme*.rtf");
                if(Files.Length>0) {
                    if(ops.readme==""||MessageBox.Show("Overwrite current readme with contents of '"+Files[0]+"'?",
                        "Question", MessageBoxButtons.YesNo)==DialogResult.Yes) {
                        readme=Files[0];
                        ops.readme=Program.ReadAllText(readme);
                    }
                } else {
                    Files=Directory.GetFiles(folder, "*.txt");
                    if(Files.Length==0) Files=Directory.GetFiles(folder, "*.rtf");
                    if(Files.Length==1) {
                        if(ops.readme==""||MessageBox.Show("Overwrite current readme with contents of '"+Files[0]+"'?",
                        "Question", MessageBoxButtons.YesNo)==DialogResult.Yes) {
                            readme=Files[0];
                            ops.readme=Program.ReadAllText(readme);
                        }
                    }
                }
                if(Directory.Exists(folder+Program.omodConversionData)) {
                    //script
                    Files=Directory.GetFiles(folder+Program.omodConversionData, "script.txt");
                    if(Files.Length>0) {
                        if(ops.script==""||MessageBox.Show("Overwrite current script with contents of 'script.txt'?",
                                            "Question", MessageBoxButtons.YesNo)==DialogResult.Yes) {
                            ops.script=Program.ReadAllText(Files[0]);
                        }
                    }
                    //screenshot
                    Files=Directory.GetFiles(folder+Program.omodConversionData, "screenshot");
                    if(Files.Length>0) {
                        if(ops.Image==null||MessageBox.Show("Overwrite current screenshot with contents of "+Files[0]+"?",
                            "Question", MessageBoxButtons.YesNo)==DialogResult.Yes) {
                            try {
                                ScreenshotPic.Image=Image.FromFile(Files[0]);
                                ops.Image=Files[0];
                            } catch {
                                MessageBox.Show("The image file appears to be corrupt", "Error");
                            }
                        }
                    }
                    //data
                    Files=Directory.GetFiles(folder+Program.omodConversionData, "config");
                    if(Files.Length>0) {
                        if(MessageBox.Show("omod conversion data is available.\n"+
                    "Would you like to import it?", "Question", MessageBoxButtons.YesNo)==DialogResult.Yes) {
                            BinaryReader br=new BinaryReader(File.OpenRead(folder+Program.omodConversionData+"config"));
                            byte version=br.ReadByte();
                            if(version>Program.CurrentOmodVersion) {
                                MessageBox.Show("This version of obmm is too old to read the omod conversion data\n"+
                                    "Conversion data is from a version "+version+" omod, but this version of obmm can only read up to version "+Program.CurrentOmodVersion,
                                    "Error");
                            } else {
                                tbName.Text=br.ReadString();
                                int MajorVersion=br.ReadInt32();
                                int MinorVersion=br.ReadInt32();
                                tbVersion.Text=MajorVersion.ToString();
                                if(MinorVersion!=-1)tbVersion.Text+="."+MinorVersion.ToString();
                                tbAuthor.Text=br.ReadString();
                                tbEmail.Text=br.ReadString();
                                tbWebsite.Text=br.ReadString();
                                ops.Description=br.ReadString();
                                if(version>=2) br.ReadInt64();//Creation date
                                else br.ReadString();
                                br.ReadByte();  //Don't really want to read in the compression type
                                if(version>=1) {
                                    int i=br.ReadInt32();
                                    if(i!=-1) tbVersion.Text+="."+i;
                                }
                                br.Close();
                            }
                        }
                    }
                }
            }
            {
                List<string> MatchedFiles=new List<string>();
                List<string> MatchedPaths=new List<string>();
                string[] NewFiles;
                string[] NewPaths;
                //esps/esms
                MatchedPaths.AddRange(Directory.GetFiles(folder, "*.esp", SearchOption.AllDirectories));
                MatchedPaths.AddRange(Directory.GetFiles(folder, "*.esm", SearchOption.AllDirectories));
                MatchedFiles.AddRange(MatchedPaths);
                for(int i=0;i<MatchedPaths.Count;i++) MatchedPaths[i] = MatchedPaths[i].Substring(folder.Length);
                NewFiles=new string[ops.esps.Length+MatchedFiles.Count];
                NewPaths=new string[ops.esps.Length+MatchedFiles.Count];
                Array.Copy(ops.esps, NewFiles, ops.esps.Length);
                Array.Copy(MatchedFiles.ToArray(), 0, NewFiles, ops.esps.Length, MatchedFiles.Count);
                Array.Copy(ops.espPaths, NewPaths, ops.esps.Length);
                Array.Copy(MatchedPaths.ToArray(), 0, NewPaths, ops.esps.Length, MatchedFiles.Count);
                ops.esps=NewFiles;
                ops.espPaths=NewPaths;
                //data files
                MatchedPaths.Clear();
                MatchedPaths.AddRange(Directory.GetFiles(folder, "*", SearchOption.AllDirectories));
                foreach(string s in MatchedFiles) MatchedPaths.Remove(s);
                for(int i=0;i<MatchedPaths.Count;i++) {
                    if(MatchedPaths[i].Contains(Program.omodConversionData)||MatchedPaths[i]==readme||
                    System.IO.Path.GetFileName(MatchedPaths[i]).ToLower()=="archiveinvalidation.txt") {
                        MatchedPaths.RemoveAt(i--);
                        continue;
                    }
                    FileInfo fi=new FileInfo(MatchedPaths[i]);
                    if((fi.Attributes&(FileAttributes.System|FileAttributes.Hidden))!=0) {
                        MatchedPaths.RemoveAt(i--);
                        continue;
                    }
                }
                MatchedFiles.Clear();
                MatchedFiles.AddRange(MatchedPaths);
                for(int i=0;i<MatchedPaths.Count;i++) MatchedPaths[i] = MatchedPaths[i].Substring(folder.Length);
                NewFiles=new string[ops.DataFiles.Length+MatchedFiles.Count];
                NewPaths=new string[ops.DataFiles.Length+MatchedFiles.Count];
                Array.Copy(ops.DataFiles, NewFiles, ops.DataFiles.Length);
                Array.Copy(MatchedFiles.ToArray(), 0, NewFiles, ops.DataFiles.Length, MatchedFiles.Count);
                Array.Copy(ops.DataFilePaths, NewPaths, ops.DataFiles.Length);
                Array.Copy(MatchedPaths.ToArray(), 0, NewPaths, ops.DataFiles.Length, MatchedFiles.Count);
                ops.DataFiles=NewFiles;
                ops.DataFilePaths=NewPaths;
            }
            //Clean up
            UpdateListView();
        }

        private void bAddFromFolder_Click(object sender, EventArgs e) {
            while(Settings.omodCreatorFolderBrowserDir.IndexOf('\\')!=-1) {
                if(Directory.Exists(Settings.omodCreatorFolderBrowserDir)) {
                    FolderDialog.SelectedPath=Settings.omodCreatorFolderBrowserDir;
                    break;
                }
                Settings.omodCreatorFolderBrowserDir=Settings.omodCreatorFolderBrowserDir.Substring(0, Settings.omodCreatorFolderBrowserDir.LastIndexOf('\\'));
            }
            if(FolderDialog.ShowDialog()!=DialogResult.OK) return;
            Settings.omodCreatorFolderBrowserDir=FolderDialog.SelectedPath;
            AddFilesFromFolder(FolderDialog.SelectedPath+"\\");
        }

        private void bAddZip_Click(object sender, EventArgs e) {
            OpenDialog.Multiselect=false;
            OpenDialog.Title="Select mod to convert to omod format";
            OpenDialog.Filter="compressed files (*.zip,*.rar,*.7z)|*.zip;*.rar;*.7z|All files|*.*";
            if(OpenDialog.ShowDialog()!=DialogResult.OK) return;
            string Dir=Program.CreateTempDirectory();
            switch(Path.GetExtension(OpenDialog.FileName)) {
            case ".rar":
                if(!File.Exists(@"obmm\unrar.dll")) {
                    MessageBox.Show("Unable to find unrar dll.\n"+
                        "If you did a partial install of obmm but wish to convert rar files, please do a full install instead.",
                        "Error");
                    return;
                }
                Unrar.Unrar rar;
                try {
                    rar=new OblivionModManager.Unrar.Unrar(OpenDialog.FileName);
                } catch {
                    MessageBox.Show("The file was password protected, or was not a valid rar file.", "Error");
                    return;
                }
                try {
                    rar.Open(Unrar.Unrar.OpenMode.Extract);
                    while(rar.ReadHeader()) rar.ExtractToDirectory(Dir);
                } catch {
                    rar.Close();
                    MessageBox.Show("The file was password protected, or was not a valid rar file.", "Error");
                    return;
                }
                rar.Close();
                break;
            case ".zip":
                ICSharpCode.SharpZipLib.Zip.FastZip fz=new ICSharpCode.SharpZipLib.Zip.FastZip();
                try {
                    fz.ExtractZip(OpenDialog.FileName, Dir, null);
                } catch {
                    MessageBox.Show("The file was password protected, or was not a valid zip file.", "Error");
                    return;
                }
                break;
            default:
                if(!File.Exists(@"obmm\7za.exe")) {
                    MessageBox.Show("Unable to find 7za.exe.\n"+
                        "If you did a partial install of obmm but wish to convert 7z files, please do a full install instead.",
                        "Error");
                    return;
                }
                System.Diagnostics.ProcessStartInfo psi=new System.Diagnostics.ProcessStartInfo(@"obmm\7za.exe",
                    "x \""+OpenDialog.FileName+"\" * -o\""+Dir+"\" -aos -y  -r");
                psi.CreateNoWindow=true;
                psi.UseShellExecute=false;
                System.Diagnostics.Process p=System.Diagnostics.Process.Start(psi);
                p.WaitForExit();
                if(Directory.GetFileSystemEntries(Dir).Length==0) {
                    MessageBox.Show("Unable to extract anything from archive", "Error");
                    return;
                }
                break;
            }
            if(Directory.GetFileSystemEntries(Dir).Length==1) {
                string[] folders=Directory.GetDirectories(Dir);
                if(folders.Length==1) {
                    string str;
                    int index=folders[0].LastIndexOf('\\');
                    if(index!=-1) str=folders[0].Substring(index+1); else str=folders[0];
                    switch(str.ToLower()+"\\") {
                    case "lsdata\\":
                    case "meshes\\":
                    case "music\\":
                    case "textures\\":
                    case "shaders\\":
                    case "video\\":
                    case "sound\\":
                    case Program.omodConversionData:
                        break;
                    default:
                        Dir=folders[0]+"\\";
                        break;
                    }

                }
            }
            if(Directory.Exists(Dir+"data")) {
                foreach(string s in Directory.GetFiles(Dir)) {
                    try { File.Move(s, Dir+"data\\"+Path.GetFileName(s)); } catch { }
                }
                Dir+="data\\";
            }
            AddFilesFromFolder(Dir);
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e) {
            try {
                System.Diagnostics.Process.Start(lvFiles.SelectedItems[0].SubItems[1].Text);
            } catch(Exception ex) {
                MessageBox.Show("An error occured attempting to open the file.\n"+
                    ex.Message, "Error");
            }
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e) {
            foreach(ListViewItem lvi in lvFiles.SelectedItems) {
                lvFiles.Items.Remove(lvi);
            }
        }

        private void scanForDataFilesToolStripMenuItem_Click(object sender, EventArgs e) {
            if(!EditingPlugins) return;
            System.Collections.Generic.List<string> toadd=new System.Collections.Generic.List<string>();
            foreach(ListViewItem lvi in lvFiles.SelectedItems) {
                try {
                    toadd.AddRange(ConflictDetector.TesFile.GetDataFileList(lvi.SubItems[1].Text));
                } catch(Exception ex) {
                    MessageBox.Show("An error occurred trying to parse plugin "+lvi.SubItems[1].Text+"\n"+
                        "Error: "+ex.Message, "Error");
                }
            }
            //Remove missing files
            for(int i=0;i<toadd.Count;i++) {
                if(!File.Exists(Path.GetFullPath("Data\\"+toadd[i]))) {
                    toadd.RemoveAt(i--);
                }
            }
            //Remove duplicates
            int index;
            for(int i=0;i<toadd.Count;i++) {
                while((index=toadd.LastIndexOf(toadd[i]))>i) toadd.RemoveAt(index);
            }
            int lower=ops.DataFilePaths.Length;
            Array.Resize<string>(ref ops.DataFilePaths, lower+toadd.Count);
            Array.Resize<string>(ref ops.DataFiles, lower+toadd.Count);
            for(int i=0;i<toadd.Count;i++) {
                ops.DataFilePaths[lower+i]=toadd[i];
                ops.DataFiles[lower+i]=Path.GetFullPath("Data\\"+toadd[i]);
            }
        }

        private void tbVersion_Leave(object sender, EventArgs e) {
            try {
                string[] versions=tbVersion.Text.Split('.');
                if(versions.Length==0||versions.Length>3) throw new obmmException("");
                if(versions[0]=="") ops.MajorVersion=0; else ops.MajorVersion=Convert.ToInt32(versions[0]);
                if(versions.Length>1) {
                    if(versions[1]=="") ops.MinorVersion=0; else ops.MinorVersion=Convert.ToInt32(versions[1]);
                } else ops.MinorVersion=-1;
                if(versions.Length>2) {
                    if(versions[2]=="") ops.BuildVersion=0; else ops.BuildVersion=Convert.ToInt32(versions[2]);
                } else ops.BuildVersion=-1;
            } catch {
                tbVersion.Text="1";
            }
        }

        private void bUp_Click(object sender, EventArgs e) {
            if(lvFiles.SelectedItems.Count==0) return;
            if(lvFiles.SelectedItems.Count>1) {
                MessageBox.Show("Can only move 1 mod at a time", "Error");
                return;
            }
            if(lvFiles.SelectedIndices[0]==0) return;
            ListViewItem lvi=lvFiles.SelectedItems[0];
            int index=lvFiles.SelectedIndices[0];
            lvFiles.Items.RemoveAt(index);
            lvFiles.Items.Insert(index-1, lvi);
        }

        private void bDown_Click(object sender, EventArgs e) {
            if(lvFiles.SelectedItems.Count==0) return;
            if(lvFiles.SelectedItems.Count>1) {
                MessageBox.Show("Can only move 1 mod at a time", "Error");
                return;
            }
            if(lvFiles.SelectedIndices[0]==lvFiles.Items.Count-1) return;
            ListViewItem lvi=lvFiles.SelectedItems[0];
            int index=lvFiles.SelectedIndices[0];
            lvFiles.Items.RemoveAt(index);
            lvFiles.Items.Insert(index+1, lvi);
        }

        private void validateToolStripMenuItem_Click(object sender, EventArgs e) {
            ProgressForm pb=null;
            if(lvFiles.Items.Count>1000) {
                pb=new ProgressForm("Validating", false);
                pb.SetProgressRange(lvFiles.Items.Count);
                pb.Show();
                pb.Activate();
                Enabled=false;
            }
            int InvalidCount=0;
            int DuplicateCount=0;
            System.Collections.Generic.List<string> files=new List<string>(lvFiles.Items.Count);
            for(int i=0;i<lvFiles.Items.Count;i++) {
                if(!File.Exists(lvFiles.Items[i].SubItems[1].Text)) {
                    InvalidCount++;
                    lvFiles.Items.RemoveAt(i--);
                } else {
                    files.Add(lvFiles.Items[i].Text.ToLower());
                }
            }
            for(int i=0;i<files.Count;i++) {
                if(pb!=null&&i%100==0) {
                    pb.UpdateProgress(i);
                    pb.Refresh();
                }
                for(int j=i+1;j<files.Count;j++) {
                    if(files[i]==files[j]) {
                        DuplicateCount++;
                        switch(MessageBox.Show("Two files with duplicate paths were found\n"+
                                "old: "+lvFiles.Items[i].SubItems[1].Text+"\n"+
                                "new: "+lvFiles.Items[j].SubItems[1].Text+"\n\n"+
                                "Do you wish to remove the older file?\n"+
                                "Click no to remove the newer one\n"+
                                "Click cancel to ignore this pair of files (omods created with duplicate files are will be non-functional)", "Duplicate file found",
                            MessageBoxButtons.YesNoCancel)) {
                        case DialogResult.Yes:
                            lvFiles.Items.RemoveAt(i);
                            files.RemoveAt(i);
                            j=i+1;
                            break;
                        case DialogResult.No:
                            files.RemoveAt(j);
                            lvFiles.Items.RemoveAt(j--);
                            break;
                        default:
                            continue;
                        }
                    }
                }
            }
            if(pb!=null) {
                pb.Unblock();
                pb.Close();
                Enabled=true;
                Focus();
            }
            if(sender!=null) {
                MessageBox.Show("Invalid file references removed: "+InvalidCount.ToString()+"\n"+
                    "Duplicate entries removed: "+DuplicateCount.ToString(), "Message");
            }
        }

        private void FilesContextMenu_Opening(object sender, CancelEventArgs e) {
            if(lvFiles.SelectedItems.Count==0) {
                deleteToolStripMenuItem.Enabled=false;
            } else {
                deleteToolStripMenuItem.Enabled=true;
            }
            if(lvFiles.SelectedItems.Count==1) {
                openToolStripMenuItem.Enabled=true;
            } else {
                openToolStripMenuItem.Enabled=false;
            }
            if(EditingPlugins) {
                if(lvFiles.SelectedItems.Count==0) {
                    importModDetailsToolStripMenuItem.Enabled=false;
                    scanForDataFilesToolStripMenuItem.Enabled=false;
                    viewRequiredDataFilesToolStripMenuItem.Enabled=false;
                } else {
                    scanForDataFilesToolStripMenuItem.Enabled=true;
                    viewRequiredDataFilesToolStripMenuItem.Enabled=true;
                    if(lvFiles.SelectedItems.Count==1) {
                        importModDetailsToolStripMenuItem.Enabled=true;
                    } else {
                        importModDetailsToolStripMenuItem.Enabled=false;
                    }
                }
            } else {
                importModDetailsToolStripMenuItem.Enabled=false;
                scanForDataFilesToolStripMenuItem.Enabled=false;
                viewRequiredDataFilesToolStripMenuItem.Enabled=false;
            }
        }

        private void bScreenshot_Click(object sender, EventArgs e) {
            OpenDialog.Title="Choose a screenshot to add";
            OpenDialog.Filter="Image files (bmp, jpg, gif, tif, png)|*.bmp;*.jpg;*.jpeg;*.gif;*.tif;*.tiff;*.png|All files|*.*";
            OpenDialog.Multiselect=false;
            if(OpenDialog.ShowDialog()==DialogResult.OK) {
                try {
                    byte[] buffer=File.ReadAllBytes(OpenDialog.FileName);
                    MemoryStream ms=new MemoryStream(buffer);
                    Image i=Image.FromStream(ms);
                    if(Program.IsImageAnimated(i)) {
                        MessageBox.Show("Animated or multi-resolution images are not supported", "Error");
                    } else {
                        ScreenshotPic.Image=i;
                        ops.Image=OpenDialog.FileName;
                    }
                } catch {
                    MessageBox.Show("The image file appears to be corrupt", "Error");
                }
            }
        }

        private void bRemoveScreenshot_Click(object sender, EventArgs e) {
            ScreenshotPic.Image=null;
            ops.Image=null;
        }

        private void cmbCompLevel1_SelectedIndexChanged(object sender, EventArgs e) {
            if(cmbCompType.SelectedIndex==0&&cmbDataCompLevel.SelectedIndex==0&&Settings.WarnOnModDelete) {
                MessageBox.Show("Using very high compression in 7-zip mode requires 1-2 GB of ram depending on the size of the mod you are trying to compress.\n"+
                    "If omod creation fails with an out of memory error, please switch to a lower level of compression.", "Warning");
            }
        }

        private void bScreenshot_MouseEnter(object sender, EventArgs e) {
            if(ScreenshotPic.Image!=null) {
                ScreenshotPic.Visible=true;
                lvFiles.Visible=false;
            }
        }

        private void bScreenshot_MouseLeave(object sender, EventArgs e) {
            ScreenshotPic.Visible=false;
            lvFiles.Visible=true;
        }

        private void importModDetailsToolStripMenuItem_Click(object sender, EventArgs e) {
            if(!EditingPlugins) return;
            if(lvFiles.SelectedItems.Count!=1) {
                MessageBox.Show("You must select a single esp to import information from", "Error");
                return;
            }
            ConflictDetector.HeaderInfo hi=ConflictDetector.TesFile.GetHeader(lvFiles.SelectedItems[0].SubItems[1].Text);
            if(hi.Author==null||hi.Author=="DEFAULT") {
                if(hi.Description==null) {
                    MessageBox.Show("This esp does not contain author or description information", "Warning");
                } else {
                    MessageBox.Show("This esp does not contain author information", "Warning");
                }
            } else {
                if(hi.Description==null) {
                    MessageBox.Show("This esp does not contain description information", "Warning");
                }
            }
            if(hi.Author!="DEFAULT"&&hi.Author!=null) tbAuthor.Text=hi.Author;
            if(hi.Description!=null) ops.Description=hi.Description;
            if(tbName.Text==InitialName) {
                tbName.Text=Path.GetFileNameWithoutExtension(lvFiles.SelectedItems[0].SubItems[1].Text);
            }
        }

        private void bGroups_Click(object sender, EventArgs e) { cmsGroups.Show(bGroups, 0, 0); }

        private void viewRequiredDataFilesToolStripMenuItem_Click(object sender, EventArgs e) {
            if(!EditingPlugins) return;
            System.Collections.Generic.List<string> toadd=new System.Collections.Generic.List<string>();
            foreach(ListViewItem lvi in lvFiles.SelectedItems) {
                try {
                    toadd.AddRange(ConflictDetector.TesFile.GetDataFileList(lvi.SubItems[1].Text));
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
            string text="Warning: If this mod uses nif's which are packed into a BSA archive, obmm won't be able to extract the list of required texture from them."+
                Environment.NewLine+Environment.NewLine+string.Join(Environment.NewLine, toadd.ToArray());
            (new TextEditor("Required data files", text, false, false)).ShowDialog();
        }

        private void lvFiles_AfterLabelEdit(object sender, LabelEditEventArgs e) {
            if(e==null||e.Label==null) return;
            if(e.Label.Contains("/")) {
                e.CancelEdit=true;
                lvFiles.Items[e.Item].Text=e.Label.Replace('/', '\\');
            }
        }

        private void lvFiles_KeyDown(object sender, KeyEventArgs e) {
            if(e.KeyCode==Keys.Delete||e.KeyCode==Keys.Back) {
                e.Handled=true;
                deleteToolStripMenuItem_Click(null, null);
            }
            if(e.Control&&e.KeyCode==Keys.A) {
                e.Handled=true;
                SuspendLayout();
                lvFiles.SelectedIndices.Clear();
                for(int i=0;i<lvFiles.Items.Count;i++) {
                    lvFiles.SelectedIndices.Add(i);
                }
                ResumeLayout();
            }
        }
    }
}