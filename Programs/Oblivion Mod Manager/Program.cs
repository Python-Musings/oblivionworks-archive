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
using System.IO;
using System.Collections.Generic;
using Application=System.Windows.Forms.Application;
using MessageBox=System.Windows.Forms.MessageBox;
using Formatter=System.Runtime.Serialization.Formatters.Binary.BinaryFormatter;
using Mutex=System.Threading.Mutex;
using System.Security.Principal;

//TODO: Scripts shouldn't have write access to the root oblivion folder. Make it read/discovery only
//TODO: BSA browser should be able to preview text files

namespace OblivionModManager {
    internal enum LaunchType { None, Oblivion };

    internal static class Program {
        internal const string version="1.1.11";
        internal const byte MajorVersion=1;
        internal const byte MinorVersion=1;
        internal const byte BuildNumber=11;
        internal const byte CurrentOmodVersion=4;

        internal const string CorruptDir=@"obmm\corrupt\";
        internal const string BackupDir=@"obmm\backup\";
        internal const string DataFile=@"obmm\data";
        internal const string SettingsFile=@"obmm\settings2";
        internal const string BSAEditFile=@"obmm\BSAEdits";
        internal const string omodConversionData=@"omod conversion data\";
        internal const string HelpPath="obmm\\obmm.chm";

        internal static readonly string[] BannedFiles={
            @"shaders\shaderpackage001.sdp",
            @"shaders\shaderpackage002.sdp",
            @"shaders\shaderpackage003.sdp",
            @"shaders\shaderpackage004.sdp",
            @"shaders\shaderpackage005.sdp",
            @"shaders\shaderpackage006.sdp",
            @"shaders\shaderpackage007.sdp",
            @"shaders\shaderpackage008.sdp",
            @"shaders\shaderpackage009.sdp",
            @"shaders\shaderpackage010.sdp",
            @"shaders\shaderpackage011.sdp",
            @"shaders\shaderpackage012.sdp",
            @"shaders\shaderpackage013.sdp",
            @"shaders\shaderpackage014.sdp",
            @"shaders\shaderpackage015.sdp",
            @"shaders\shaderpackage016.sdp",
            @"shaders\shaderpackage017.sdp",
            @"shaders\shaderpackage018.sdp",
            @"shaders\shaderpackage019.sdp",
            @"video\2k games.bik",
            @"video\bethesda softworks HD720p.bik",
            @"video\CreditsMenu.bik",
            @"video\game studios.bik",
            @"video\Map loop.bik",
            @"video\Oblivion iv logo.bik",
            @"video\Oblivion Legal.bik",
            @"video\OblivionIntro.bik",
            @"video\OblivionOutro.bik",
            @"oblivion.esm"
        };

        internal static string TempDir {
            get {
                if(Settings.tempDir!="") return Settings.tempDir;
                return Path.GetTempPath()+@"obmm\";
            }
        }
        internal static readonly string CurrentDir=(Path.GetDirectoryName(Application.ExecutablePath)+"\\").ToLower();
        internal static readonly string OblivionINIDir=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments),"My games\\oblivion\\");
        internal static readonly string OblivionESPDir=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData),"oblivion\\");
        internal static readonly string VistaVirtualStore=Path.Combine(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "VirtualStore\\"), CurrentDir.Remove(0,3));

        internal static sData Data;
        internal static bool IsLimited=false;
        private static Mutex mutex=null;
        internal static LaunchType Launch=LaunchType.None;

        [STAThread]
        internal static void Main(string[] args) {
            if(!PreInit(args)) return;
            if(Launch!=LaunchType.None) {
                PostExit();
                return;
            }
            if(!Init()) return;
            RunCommandLine(args);
            if(Settings.SafeMode) PostInit();
            //Run!
            if(IsLimited) {
                MessageBox.Show("You appear to be running as a limited user.\n"+
                    "Most of obmm's functionality will be unavailable.\n"+
                    "If using windows vista, remember to check the 'run as administrator' checkbox in obmm's compatability settings (see note 16 in the FAQ)", "Warning");
                Forms.LimitedUserForm mf=new Forms.LimitedUserForm();
                Application.Run(mf);
            } else {
                Conflicts.UpdateConflicts();
                MainForm mf=new MainForm();
                Application.Run(mf);
            }
            Exit();
            PostExit();
        }

        internal static void RunCommandLine(string[] args) {
            bool DisplayedWarning=false;
            foreach(string s in args) {
                switch(Path.GetExtension(s).ToLower()) {
                case ".omod":
                    if(IsLimited) {
                        if(!DisplayedWarning) {
                            MessageBox.Show("Limited users cannot install new mods.\n"+
                                            "Please log on as an administrator.", "Error");
                        }
                        break;
                    }
                    LoadNewOmod(s);
                    break;
                case ".bsa":
                    try {
                        (new BSABrowser(s)).ShowDialog();
                    } catch(Exception e) {
                        MessageBox.Show("An error occured\n"+e.Message);
                    }
                    break;
                default:
                    if(!s.StartsWith("-")) MessageBox.Show("Cannot open '"+s+"'\nUnsupported file type.");
                    break;
                }
            }
        }

        private static bool PreInit(string[] args) {
            //Initial setup
            Directory.SetCurrentDirectory(Path.GetDirectoryName(Application.ExecutablePath));
            Application.EnableVisualStyles();
            AppDomain.CurrentDomain.UnhandledException+=new UnhandledExceptionEventHandler(CurrentDomain_UnhandledException);
            Application.ThreadException+=new System.Threading.ThreadExceptionEventHandler(Application_ThreadException);
            //Check for mutex
            bool creatednew;
            mutex=new Mutex(false, "obmm_mutex", out creatednew);
            if(!creatednew) {
                if(args.Length>0) {
                    File.WriteAllLines("obmm\\pipe", args);
                    return false;
                } else {
                    MessageBox.Show("Only one instance of obmm may be open at any one time.", "Error");
                    return false;
                }
            }
            //check it's being run from the oblivion directory
            if(!Directory.Exists("data")||!File.Exists("oblivion.exe")||!File.Exists("oblivion_default.ini")) {
                MessageBox.Show("Oblivion mod manager must be installed to oblivion's base directory.\n\n"+
                    "If you have moved or renamed 'oblivion_default.ini', please replace it.", "Error");
                return false;
            }
            //Load settings
            Settings.LoadSettings();
            //Run arguments
            if(args.Length==1) {
                switch(args[0].ToLower()) {
                case "-safe":
                    Settings.SafeMode=true;
                    return true;
                case "-bsa-creator":
                    new Forms.BSACreator().ShowDialog();
                    return false;
                case "-bsa-browser":
                    new BSABrowser().ShowDialog();
                    return false;
                case "-conflict-detector":
                    new ConflictReport.NewReportGenerator().ShowDialog();
                    return false;
                case "-launch":
                    Launch=LaunchType.Oblivion;
                    return true;
                }
            }
            return true;
        }

        private static bool Init() {
            //Clear out any tempory files obmm may have created
            ClearTempFiles();
            //Check shader packages are not read only
            if(Directory.Exists("data\\shaders")) {
                foreach(FileInfo fi in new DirectoryInfo("data\\shaders").GetFiles("*.sdp")) {
                    if((fi.Attributes&FileAttributes.ReadOnly)>0) fi.Attributes^=FileAttributes.ReadOnly;
                }
            }
            //Create required directories
            try {
                if(!Directory.Exists("obmm")) Directory.CreateDirectory("obmm");
                if(!Directory.Exists(Settings.omodDir)) Directory.CreateDirectory(Settings.omodDir);
                if(!Directory.Exists(CorruptDir)) Directory.CreateDirectory(CorruptDir);
                if(!Directory.Exists(BackupDir)) Directory.CreateDirectory(BackupDir);
                File.Delete("obmm\\pipe");
            } catch(Exception ex) {
                MessageBox.Show("One or more of Oblivion mod managers directories do not exist and cannot be created.\n"+
                    "Error: "+ex.Message, "Error");
                return false;
            }
            try {
                if(!Directory.Exists(OblivionINIDir)) Directory.CreateDirectory(OblivionINIDir);
                if(!Directory.Exists(OblivionESPDir)) Directory.CreateDirectory(OblivionESPDir);
            } catch(Exception ex) {
                MessageBox.Show("One or more of Oblivion's game directories do not exist and cannot be created.\n"+
                    "Error: "+ex.Message, "Error");
                return false;
            }
            if(!OblivionESP.CreateList()) {
                MessageBox.Show("Oblivion's active esp list is missing and could not be created.", "Error");
                return false;
            }
            if(!OblivionINI.CreateINI()) {
                MessageBox.Show("Oblivion's ini file is missing and could not be created.", "Error");
                return false;
            }
            
            try {
                File.Delete("obmm\\limited");
                if(File.Exists(VistaVirtualStore+"obmm\\limited")) File.Delete(VistaVirtualStore+"obmm\\limited");
                FileStream fs=File.Create("obmm\\limited");
                fs.Close();
                if(File.Exists(VistaVirtualStore+"obmm\\limited")) {
                    File.Delete("obmm\\limited");
                    throw new Exception();
                }
                File.Delete("obmm\\limited");
            } catch {
                IsLimited=true;
                Data=new sData();
                return true;
            }
            //If using windows vista, check that the virtual store is empty
            if(Directory.Exists(VistaVirtualStore)) {
                if(new WindowsPrincipal(WindowsIdentity.GetCurrent()).IsInRole(WindowsBuiltInRole.Administrator)) {
                    if(MessageBox.Show("Vista appears to have moved some of oblivions files to the virtual store.\n"+
                        "Do you wish to move them back?\n"+
                        "If you don't know what the virtual store is, please read entry 16 in the FAQ.",
                        "Warning", System.Windows.Forms.MessageBoxButtons.YesNo)==System.Windows.Forms.DialogResult.Yes) {
                        RecersiveDirectoryMove(VistaVirtualStore, CurrentDir, true);
                        Directory.Delete(VistaVirtualStore, true);
                    }
                } else {
                    MessageBox.Show("Vista appears to have moved some of oblivions files to the virtual store.\n"+
                        "Since obmm doesn't have administrative privilages, it can't move them back\n"+
                        "If you have problems with mods not showing up in game, or omods vanishing from obmm, please read entry 16 in the FAQ.",
                        "Warning");
                }
            }
            //Delete old save files
            if(File.Exists("obmm\\settings")) {
                MessageBox.Show("obmm 0.8 cannot read save files from 0.7.x or earlier.\n"+
                    "If you did not deactivate your omods before upgrading, it may be necessary to use 'clean all' to tidy up the mess", "Warning");
                File.Delete("obmm\\settings");
                File.Delete(SettingsFile);
                File.Delete(DataFile);
            }
            //Load more saved settings
            OblivionBSA.ReadArchives();
            if(!File.Exists(DataFile)) {
                Data=new sData();
            } else {
                Formatter f=new Formatter();
                Stream s=File.OpenRead(DataFile);
                try {
                    Data=(sData)f.Deserialize(s);
                } catch(Exception ex) {
                    MessageBox.Show("Could not load obmm save data.\n"+
                        "If you weren't expecting this message, please read entry 15 in the FAQ.\n"+
                        "Error: "+ex.Message, "Error");
                    Data=new sData();
                }
                s.Close();
            }
            //recreate the unserialized data in omods
            foreach(omod o in Data.omods) o.RecreatePrivateData();
            //finished sucessfully
            return true;
        }

        private static void PostInit() {
            //Remove any omods that have gone missing from the omod directory
            if(!IsLimited) {
                for(int i=0;i<Data.omods.Count;i++) {
                    if(!File.Exists(Settings.omodDir+((omod)Data.omods[i]).FileName)) {
                        if(Data.omods[i].Conflict==ConflictLevel.Active) Data.omods[i].DeletionDeactivate();
                        else Data.omods.RemoveAt(i);
                        i--;
                    }
                }
                //Load any omods manually placed in the mods directory
                foreach(string s in Directory.GetFiles(Settings.omodDir, "*.omod")) {
                    if(!Data.DoesModExist(Path.GetFileName(s))) LoadNewOmod(Settings.omodDir+Path.GetFileName(s));
                }
            }
            //Rescan esp headers
            if(Settings.UpdateEsps) { foreach(EspInfo ei in Data.Esps) { if(ei.Parent==null) { ei.GetHeader(); } } }
            //Search for any esps the have appeared since the last time this was run
            List<string> Plugins=new List<string>();
            foreach(string s in Directory.GetFiles("data")) {
                if(Path.GetExtension(s)!=".esp"&&Path.GetExtension(s)!=".esm") continue;
                if(!Data.DoesEspExist(Path.GetFileName(s))) {
                    Plugins.Add(Path.GetFileName(s));
                }
                FileInfo fi=new FileInfo(s);
                if((fi.Attributes&FileAttributes.ReadOnly)>0) fi.Attributes^=FileAttributes.ReadOnly;
            }
            if(Plugins.Count>0) {
                string[] files=Plugins.ToArray();
                for(int i=0;i<files.Length;i++) {
                    EspInfo ei;
                    try {
                        ei=new EspInfo(files[i]);
                    } catch {
                        MessageBox.Show("File "+files[i]+" does not appear to be a valid TES4 plugin", "Error");
                        continue;
                    }
                    ei.Active=false;
                    Data.Esps.Add(ei);
                }
            }
            //Check that the plugins we think are active are active
            Plugins.Clear();
            foreach(EspInfo ei in Data.Esps) Plugins.Add(ei.FileName);
            bool[] b=OblivionESP.ArePluginsActive(Plugins.ToArray());
            for(int i=0;i<b.Length;i++) Data.Esps[i].Active=b[i];
            if(!IsLimited) {
                //Remove any plugins which have been deleted since the last time obmm was run
                for(int i=0;i<Data.Esps.Count;i++) {
                    if(!File.Exists("data\\"+Data.Esps[i].FileName)) {
                        Data.Esps.RemoveAt(i--);
                    }
                }
                //Remove any data files which have been deleted since the last time obmm was run
                for(int i=0;i<Data.DataFiles.Count;i++) {
                    if(!File.Exists("data\\"+Data.DataFiles[i].FileName)) {
                        foreach(omod o in Data.omods) {
                            if(o.Conflict==ConflictLevel.Active) {
                                int j=Array.IndexOf<DataFileInfo>(o.DataFiles, Data.DataFiles[i]);
                                if(j!=-1) {
                                    for(int k=j;k<o.DataFiles.Length-1;k++) {
                                        o.DataFiles[k]=o.DataFiles[k+1];
                                    }
                                    Array.Resize<DataFileInfo>(ref o.DataFiles, o.DataFiles.Length-1);
                                }
                            }
                        }
                        Data.DataFiles.RemoveAt(i--);
                    }
                }
            }
            Data.SortEsps();
        }

        private static void Exit() {
            ClearTempFiles();
            if(Settings.LockFOV) {
                try {
                    OblivionINI.WriteINIValue("[Display]", "fDefaultFOV", "75.0000");
                } catch(UnauthorizedAccessException) {
                    MessageBox.Show("Unable to set FOV to 75\n"+
                        "You appear to have oblivion.ini write protected\n"+
                        "The lock FOV option has been disabled, and can be reenabled from the options menu", "Error");
                    Settings.LockFOV=false;
                }
            }
            OblivionESP.SetActivePlugins();
            if(!IsLimited) {
                try {
                    OblivionBSA.CommitArchives();
                } catch(UnauthorizedAccessException) {
                    MessageBox.Show("obmm was unable to commit the BSA list to oblivion.ini\n"+
                    "You appear to have oblivion.ini write protected\n"+
                    "The BSA list will now be incorrect, and some mods may not work as intended", "Error");
                }
                if(Settings.UpdateInvalidation) {
                    try {
                        OblivionBSA.UpdateInvalidationFile();
                    } catch(UnauthorizedAccessException) {
                        MessageBox.Show("Unable to update archiveinvalidation file\n"+
                        "The file appears to be write protected\n"+
                        "The autoupdate invalidation file option has been disabled, and can be reenabled from the options menu", "Error");
                        Settings.UpdateInvalidation=false;

                    }
                }
                //Save settings,
                Settings.SaveSettings();
                Stream s=File.Open(DataFile, FileMode.Create);
                Formatter f=new Formatter();
                f.Serialize(s, Data);
                s.Close();
            }
        }

        internal static void PostExit() {
            switch(Launch) {
            case LaunchType.Oblivion:
                if(Settings.UseProcessKiller==1) {
                    if((new Forms.ProcessKiller()).ShowDialog()!=System.Windows.Forms.DialogResult.Yes) break;
                    Settings.SaveSettings();
                }
                try {
                    if(Settings.OblivionCommandLine!="") {
                        System.Diagnostics.Process.Start(Settings.OblivionCommandLine);
                    } else if(File.Exists("obse_loader.exe")) {
                        System.Diagnostics.Process.Start("obse_loader.exe");
                    } else {
                        System.Diagnostics.Process.Start("oblivion.exe");
                    }
                } catch(Exception ex) {
                    MessageBox.Show("An error occurred attempting to start oblivion.\n"+
                    ex.Message, "Error");
                }
                break;
            }
            mutex.Close();
        }

        internal static bool IsSafeFileName(string s) {
            s=s.Replace('/', '\\');
            if(s.IndexOfAny(Path.GetInvalidPathChars())!=-1) return false;
            if(Path.IsPathRooted(s)) return false;
            if(s.StartsWith(".")||Array.IndexOf<char>(Path.GetInvalidFileNameChars(), s[0])!=-1) return false;
            if(s.Contains("\\..\\")) return false;
            if(s.EndsWith(".")||Array.IndexOf<char>(Path.GetInvalidFileNameChars(), s[s.Length-1])!=-1) return false;
            return true;
        }

        internal static bool IsSafeFolderName(string s) {
            if(s.Length==0) return true;
            s=s.Replace('/', '\\');
            if(s.IndexOfAny(Path.GetInvalidPathChars())!=-1) return false;
            if(Path.IsPathRooted(s)) return false;
            if(s.StartsWith(".")||Array.IndexOf<char>(Path.GetInvalidFileNameChars(), s[0])!=-1) return false;
            if(s.Contains("\\..\\")) return false;
            if(s.EndsWith(".")) return false;
            return true;
        }

        internal static string MakeValidFolderPath(string s) {
            s=s.Replace('/', '\\');
            if(!s.EndsWith("\\")) s+="\\";
            return s;
        }

        static void Application_ThreadException(object sender, System.Threading.ThreadExceptionEventArgs e) {
            UnhandledExceptionEventArgs e2=new UnhandledExceptionEventArgs(e.Exception, false);
            CurrentDomain_UnhandledException(sender, e2);
        }

        static void CurrentDomain_UnhandledException(object sender, UnhandledExceptionEventArgs e) {
            string s1=
                "An unhandled exception occurred."+
                Environment.NewLine+
                "Extra information should have been saved to 'obmm_crashdump.txt' in oblivion's base directory."+
                Environment.NewLine+Environment.NewLine+
                "Error message: "+((Exception)e.ExceptionObject).Message+Environment.NewLine;

            string s2=
                DateTime.Now.ToLongDateString()+" - "+DateTime.Now.ToLongTimeString()+Environment.NewLine+
                version+(Settings.SafeMode?" (Safe mode)":"")+Environment.NewLine+"OS version: "+Environment.OSVersion.ToString()+
                Environment.NewLine+Environment.NewLine;

            Exception ex=(Exception)e.ExceptionObject;
            while(ex!=null) {
                s2+=
                    "Type: "+ex.GetType().ToString()+Environment.NewLine+
                    "Error message: "+ex.Message+Environment.NewLine+
                    "Stack trace: "+ex.StackTrace+Environment.NewLine;

                ex=ex.InnerException;
                if(ex!=null) s2+=Environment.NewLine+"Inner exception: "+Environment.NewLine;
            }
            MessageBox.Show(s1, "Fatal error");
            try {
                File.WriteAllText("obmm_crashdump.txt", s2);
            } catch { }
        }

        internal static bool strArrayContains(List<string> a, string s) {
            s=s.ToLower();
            foreach(string s2 in a) {
                if(s2.ToLower()==s) return true;
            }
            return false;
        }
        internal static bool strArrayContains(List<DataFileInfo> a, string s) {
            s=s.ToLower();
            for(int i=0;i<a.Count;i++) {
                if(a[i].LowerFileName==s) return true;
            }
            return false;
        }

        internal static DataFileInfo strArrayGet(DataFileInfo[] a, string s) {
            s=s.ToLower();
            for(int i=0;i<a.Length;i++) {
                if(a[i].LowerFileName==s) return a[i];
            }
            return null;
        }

        internal static void strArrayRemove(List<string> a, string s) {
            s=s.ToLower();
            for(int i=0;i<a.Count;i++) {
                if(a[i].ToLower()==s) {
                    a.RemoveAt(i);
                    return;
                }
            }
        }
        internal static void strArrayRemove(List<DataFileInfo> a, string s) {
            s=s.ToLower();
            for(int i=0;i<a.Count;i++) {
                if(a[i].LowerFileName==s) {
                    a.RemoveAt(i);
                    return;
                }
            }
        }

        internal static void ArrayRemoveAt<T>(ref T[] array, int pos) {
            for(int i=pos+1;i<array.Length;i++) array[i-1]=array[i];
            Array.Resize<T>(ref array, array.Length-1);
        }

        internal static omod LoadNewOmod(string s) {
            //Perform some sanity checking before trying to load the omod
            if(!File.Exists(s)) {
                MessageBox.Show("Unable to find file '"+s+"'", "Error");
                return null;
            }
            if(Path.GetExtension(s)!=".omod") {
                MessageBox.Show("Unrecognized file type. Oblivion mod manager only supports '.omod' "+
                                "files", "Error");
                return null;
            }
            if(Path.GetDirectoryName(s).ToLower()+"\\"==CurrentDir+Settings.omodDir) {
                return null;
            }
            if(Data.DoesModExist(Path.GetFileName(s))) {
                MessageBox.Show("The mod '"+Path.GetFileName(s)+"' appears to already be installed.",
                    "Error");
                return null;
            }
            if(s.ToLower()==Settings.omodDir+Path.GetFileName(s).ToLower()) {
                //Register a newly created omod
                omod newmod=null;
                try {
                    newmod=new omod(Path.GetFileName(s),true);
                    FileInfo fi=new FileInfo(Settings.omodDir+Path.GetFileName(s));
                    fi.CreationTime=DateTime.Now;
                    Data.omods.Add(newmod);
                    Conflicts.UpdateConflict(newmod);
                    return newmod;
                } catch(Exception ex) {
                    if(newmod!=null) newmod.Close();
                    MessageBox.Show("Error loading '"+s+"'\n"+ex.Message, "Error");
                    try { File.Move(s, CorruptDir+Path.GetFileName(s)); } catch { }
                    return null;
                }
            } else {
                //Register a newly downloaded omod
                try {
                    if(Settings.ShowNewModInfo) {
                        if(MessageBox.Show("Show omod info for '"+s+"'?", "Question", System.Windows.Forms.MessageBoxButtons.YesNo)==System.Windows.Forms.DialogResult.Yes) {
                            (new TextEditor(s, new omod(s,false).GetInfo(), false, false)).ShowDialog();
                        }
                        if(MessageBox.Show("Install '"+s+"'?", "Question", System.Windows.Forms.MessageBoxButtons.YesNo)!=System.Windows.Forms.DialogResult.Yes) {
                            return null;
                        }
                    }
                    File.Copy(s, Settings.omodDir+Path.GetFileName(s));
                    omod newmod=new omod(Path.GetFileName(s),true);
                    
                    newmod.Close();
                    FileInfo fi=new FileInfo(Settings.omodDir+Path.GetFileName(s));
                    if((fi.Attributes&FileAttributes.ReadOnly)>0) fi.Attributes^=FileAttributes.ReadOnly;
                    fi.CreationTime=DateTime.Now;
                    Data.omods.Add(newmod);
                    Conflicts.UpdateConflict(newmod);
                    return newmod;
                } catch(Exception ex) {
                    MessageBox.Show("Error loading '"+s+"'\n"+ex.Message, "Error");
                    try { File.Delete(Settings.omodDir+Path.GetFileName(s)); } catch { }
                    return null;
                }
            }
        }

        internal static FileStream CreateTempFile() {
            string s;
            return CreateTempFile(out s);
        }
        internal static FileStream CreateTempFile(out string path) {
            int i=0;
            for(i=0;i<32000;i++) {
                if(!File.Exists(TempDir+"tmp"+i.ToString())) {
                    path=TempDir+"tmp"+i.ToString();
                    return File.Create(path);
                }
            }
            throw new obmmException("Could not create temp file because directory is full");
        }

        internal static string CreateTempDirectory() {
            for(int i=0;i<32000;i++) {
                if(!Directory.Exists(TempDir+i.ToString())) {
                    Directory.CreateDirectory(TempDir+i.ToString()+"\\");
                    return TempDir+i.ToString()+"\\";
                }
            }
            throw new obmmException("Could not create temp folder because directory is full");
        }

        internal static void ClearTempFiles() { ClearTempFiles(""); }
        internal static void ClearTempFiles(string subfolder) {
            if(!Directory.Exists(TempDir)) Directory.CreateDirectory(TempDir);
            if(!Directory.Exists(TempDir+subfolder)) return;
            foreach(string file in Directory.GetFiles(TempDir+subfolder)) {
                try { File.Delete(file); } catch { }
            }
            try { Directory.Delete(TempDir+subfolder, true); } catch { }
            if(!Directory.Exists(TempDir)) Directory.CreateDirectory(TempDir);
        }
        
        internal static string ReadAllText(string file) {
        	if(!File.Exists(file)) return null;
        	return File.ReadAllText(file, System.Text.Encoding.Default);
        }

        internal static string ReadBString(BinaryReader br, int len) {
            string s="";
            byte[] bs=br.ReadBytes(len);
            foreach(byte b in bs) s+=(char)b;
            return s;
        }
        internal static string ReadCString(BinaryReader br) {
            string s="";
            while(true) {
                byte b=br.ReadByte();
                if(b==0) return s;
                s+=(char)b;
            }
        }

        internal static void RecersiveDirectoryMove(string from, string to, bool overwrite) {
            from=Path.GetFullPath(from);
            to=Path.GetFullPath(to);
            foreach(string s in Directory.GetFiles(from, "*", SearchOption.AllDirectories)) {
                string subpath=s.Substring(from.Length);
                string topath=Path.Combine(to, subpath);
                string todir=Path.GetDirectoryName(topath);
                if(!Directory.Exists(todir)) Directory.CreateDirectory(todir);
                if(File.Exists(topath)) File.Delete(topath);
                File.Move(s, topath);
            }
        }

        internal static bool KeyPressed(int code) {
            GetAsyncKeyState(code);
            return GetAsyncKeyState(code) != 0;
        }

        [System.Security.SuppressUnmanagedCodeSecurity]
        [System.Runtime.InteropServices.DllImport("user32.dll", EntryPoint="GetAsyncKeyState")]
        private static extern short GetAsyncKeyState(int code);

        internal static bool IsImageAnimated(System.Drawing.Image image) {
            if(image.FrameDimensionsList.Length>0) {
                if(Array.IndexOf<Guid>(image.FrameDimensionsList,System.Drawing.Imaging.FrameDimension.Page.Guid)!=-1) {
                    if(image.GetFrameCount(System.Drawing.Imaging.FrameDimension.Page)!=1) return true;
                }
                if(Array.IndexOf<Guid>(image.FrameDimensionsList, System.Drawing.Imaging.FrameDimension.Resolution.Guid)!=-1) {
                    if(image.GetFrameCount(System.Drawing.Imaging.FrameDimension.Resolution)!=1) return true;
                }
                if(Array.IndexOf<Guid>(image.FrameDimensionsList, System.Drawing.Imaging.FrameDimension.Time.Guid)!=-1) {
                    if(image.GetFrameCount(System.Drawing.Imaging.FrameDimension.Time)!=1) return true;
                }
            }
            return false;
        }
    }
}
