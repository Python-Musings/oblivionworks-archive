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
using Formatter=System.Runtime.Serialization.Formatters.Binary.BinaryFormatter;
using MessageBox=System.Windows.Forms.MessageBox;

namespace OblivionModManager {
    internal static class OblivionESP {
        private static readonly string espfile=Program.OblivionESPDir+"plugins.txt";
        private const string bashespfile=@"obmm\loadorder.txt";

        private class PluginSorter : IComparer<string> {
            public int Compare(string a, string b) {
                a=a.ToLower();
                b=b.ToLower();
                if(Path.GetExtension(a)!=Path.GetExtension(b)) return (Path.GetExtension(a)==".esm")?(-1):1;
                FileInfo fia=new FileInfo("data\\"+a);
                FileInfo fib=new FileInfo("data\\"+b);
                return DateTime.Compare(fia.LastWriteTime, fib.LastWriteTime);
            }
        }

        internal static bool CreateList() {
            if(!Directory.Exists(Program.OblivionESPDir)) return false;
            if(File.Exists(espfile)) return true;
            try {
                FileStream fs=File.Create(espfile);
                fs.Close();
            } catch { }
            return File.Exists(espfile);
        }

        internal static bool[] ArePluginsActive(string[] fileNames) {
            string[] FileNames=new string[fileNames.Length];
            bool[] result=new bool[fileNames.Length];
            string[] ActiveEsps=File.ReadAllLines(espfile, System.Text.Encoding.Default);
            for(int i=0;i<result.Length;i++) {
                FileNames[i]=fileNames[i].ToLower();
                result[i]=false;
            }
            int found;
            foreach(string s in ActiveEsps) {
                if(s==""||s.StartsWith("#")) continue;
                if((found=Array.IndexOf(FileNames, s.ToLower()))!=-1) result[found]=true;
            }
            return result;
        }

        internal static void SetActivePlugins() {
            StreamWriter sw=new StreamWriter(espfile, false, System.Text.Encoding.Default);
            sw.WriteLine("# This file is used to tell Oblivion which data files to load.");
            sw.WriteLine("# Use the oblivion launcher or obmm to choose which files you want.");
            sw.WriteLine("# Please do not modify this file by hand.");
            sw.WriteLine();
            sw.WriteLine("# last modified by obmm: "+DateTime.Now.ToShortDateString()+" "+DateTime.Now.ToShortTimeString());
            sw.WriteLine();
            foreach(EspInfo ei in Program.Data.Esps) {
                if(ei.Active) {
                    sw.WriteLine(ei.FileName);
                }
            }
            sw.Close();
            BinaryWriter bw=null;
            try {
                bw=new BinaryWriter(File.Create(bashespfile));
                foreach(EspInfo ei in Program.Data.Esps) {
                    if(ei.Active) {
                        bw.Write(ei.FileName);
                        bw.Write(ei.DateModified.ToBinary());
                    }
                }
            } catch { } finally { if(bw!=null) bw.Close(); }
        }

        internal static string[] GetActivePlugins() {
            if(Program.Data!=null) SetActivePlugins();
            List<string> ActiveEsps=new List<string>(File.ReadAllLines(espfile, System.Text.Encoding.Default));
            for(int i=0;i<ActiveEsps.Count;i++) {
                try {
                    if(ActiveEsps[i].Length==0||ActiveEsps[i][0]=='#'||!File.Exists("data\\"+ActiveEsps[i])) ActiveEsps.RemoveAt(i--);
                } catch { ActiveEsps.RemoveAt(i--); }
            }
            ActiveEsps.Sort(new PluginSorter());
            return ActiveEsps.ToArray();
        }
    }

    internal static class OblivionINI {
        private static readonly string ini=Program.OblivionINIDir+"oblivion.ini";

        internal static bool CreateINI() {
            if(!Directory.Exists(Program.OblivionINIDir)) return false;
            if(File.Exists(ini)) return true;
            try { File.Copy("oblivion_default.ini", ini); } catch { }
            return File.Exists(ini);
        }

        internal static string GetINIValue(string section, string name) {
            string[] ss=GetINISection(section);
            if(ss==null) throw new obmmException("Oblivion.ini section "+section+" does not exist");
            name=name.ToLower();
            foreach(string s in ss) {
                if(s.Trim().ToLower().StartsWith(name+"=")) {
                    string res=s.Substring(s.IndexOf('=')+1).Trim();
                    int i=res.IndexOf(';');
                    if(i!=-1) res=res.Substring(0, i-1);
                    return res;
                }
            }
            return null;
        }

        internal static void WriteINIValue(string section,string name,string value) {
            List<string> ss=new List<string>(GetINISection(section));
            if(ss==null) throw new obmmException("Oblivion.ini section "+section+" does not exist");
            bool matched=false;
            string lname=name.ToLower();
            for(int i=0;i<ss.Count;i++) {
                string s=ss[i];
                if(s.Trim().ToLower().StartsWith(lname+"=")) {
                    if(value==null) {
                        ss.RemoveAt(i--);
                    } else {
                        ss[i]=name+"="+value;
                     
                    }
                    matched=true;
                    break;
                }
            }
            if(!matched) ss.Add(name+"="+value);
            ReplaceINISection(section,ss.ToArray());
        }

        private static string[] GetINISection(string section) {
            List<string> contents=new List<string>();
            bool InSection=false;
            section=section.ToLower();
            StreamReader sr=new StreamReader(File.OpenRead(ini), System.Text.Encoding.Default);
            try {
                while(sr.Peek()!=-1) {
                    string s=sr.ReadLine();
                    if(InSection) {
                        if(s.Trim().StartsWith("[")&&s.Trim().EndsWith("]")) break;
                        contents.Add(s);
                    } else {
                        if(s.Trim().ToLower()==section) InSection=true;
                    }
                }
            } finally {
                if(sr!=null) sr.Close();
            }
            if(!InSection) return null;
            return contents.ToArray();
        }

        private static void ReplaceINISection(string section, string[] ReplaceWith) {
            List<string> contents=new List<string>();
            StreamReader sr=new StreamReader(File.OpenRead(ini), System.Text.Encoding.Default);
            try {
                section=section.ToLower();
                bool InSection=false;
                while(sr.Peek()!=-1) {
                    string s=sr.ReadLine();
                    if(!InSection) {
                        contents.Add(s);
                        if(s.Trim().ToLower()==section) {
                            InSection=true;
                            contents.AddRange(ReplaceWith);
                        }
                    } else {
                        if(s.Trim().StartsWith("[")&&s.Trim().EndsWith("]")) {
                            contents.Add(s);
                            InSection=false;
                        }
                    }
                }
            } finally {
                if(sr!=null) sr.Close();
            }
            StreamWriter sw=new StreamWriter(File.Create(ini), System.Text.Encoding.Default);
            try {
                foreach(string s in contents) {
                    sw.WriteLine(s);
                }
            } finally {
                if(sw!=null) sw.Close();
            }
        }

        internal static bool IsBSARegistered(string FileName) {
            FileName=FileName.ToLower();
            string[] BSAs=GetINISection("[Archives]");
            for(int i=0;i<BSAs.Length;i++) {
                string s=BSAs[i].Trim().ToLower();
                int index=s.IndexOf(';');
                if(index!=-1) s=s.Remove(index, s.Length-index);
                index=s.IndexOf('=');
                if(index!=-1) s=s.Remove(0, index+1);
                if(s.Trim()==FileName) return true;
            }
            return false;
        }
    }

    internal static class OblivionBSA {
        #region Structs
        private class BSAEditData {
            internal readonly string FolderName;
            internal readonly List<BSAEditFileInfo> Files=new List<BSAEditFileInfo>();

            internal BSAEditData(string folder) {
                FolderName=folder;
            }
        }

        private class BSAEditFileInfo {
            internal readonly ulong Hash;
            internal readonly long HashOffset;
            internal string FileName;
            internal bool Exists=false;

            internal BSAEditFileInfo(ulong hash, long offset) {
                Hash=hash;
                HashOffset=offset;
            }
        }

        [Serializable]
        private class BSAEdit {
            internal readonly string FileName;
            internal readonly long FileSize;
            internal readonly List<ulong> OldData=new List<ulong>();
            internal readonly List<long> Offsets=new List<long>();

            internal BSAEdit(string file, long fileSize) { FileName=file; FileSize=fileSize; }
        }
        #endregion

        private static readonly string InvalidationFile;
        private const string RedirectionPath=@"obmm\BSARedirection.bsa";
        internal static readonly List<string> Archives=new List<string>();
        private static bool UpdatedList=false;

        private static int filesModified;
        private static int entriesModified;
        private static int hashCollisions;
        private static bool NoUpdates=false;
        internal static int FilesModified { get { return filesModified; } }
        internal static int EntriesModified { get { return entriesModified; } }
        internal static int HashCollisions { get { return hashCollisions; } }

        static OblivionBSA() {
            try { 
                InvalidationFile=OblivionINI.GetINIValue("[Archive]", "SInvalidationFile");
                if(InvalidationFile==null) InvalidationFile="ArchiveInvalidation.txt";
            } catch { 
                InvalidationFile="ArchiveInvalidation.txt";
            }
        }

        internal static void ReadArchives() {
            string s=null;
            try {
                s=OblivionINI.GetINIValue("[Archive]", "sArchiveList");
            } catch (obmmException) {
                MessageBox.Show("Could not load BSA list", "Warning");
                NoUpdates=true;
                return;
            }
            if(s==null) {
                Archives.AddRange(new string[] { "Oblivion - Meshes.bsa", "Oblivion - Textures - Compressed.bsa",
                    "Oblivion - Sounds.bsa", "Oblivion - Voices1.bsa", "Oblivion - Voices2.bsa",
                    "Oblivion - Misc.bsa" });
            } else {
                Archives.AddRange(s.Split(new char[] { ',' }, StringSplitOptions.RemoveEmptyEntries));
                for(int i=0;i<Archives.Count;i++) Archives[i]=Archives[i].Trim();
            }
        }

        internal static void CommitArchives() {
            if(!NoUpdates&&UpdatedList) OblivionINI.WriteINIValue("[Archive]","sArchiveList",string.Join(", ",Archives.ToArray()));
        }

        internal static bool RegisterBSA(string name) {
            if(NoUpdates) {
                MessageBox.Show("Failed to register BSA "+name+" because the [Archive] section could not be found in oblivion.ini", "Error");
                return false;
            }
            string lname=name.ToLower();
            foreach(string s in Archives) {
                if(s.ToLower()==lname) return true;
            }
            UpdatedList=true;
            if(name==@"..\obmm\BSARedirection.bsa") Archives.Insert(0, name);
            else Archives.Add(name);
            return false;
        }

        internal static void UnregisterBSA(string name) {
            if(NoUpdates) {
                MessageBox.Show("Failed to unregister BSA "+name+" because the [Archive] section could not be found in oblivion.ini", "Error");
                return;
            }
            name=name.ToLower();
            for(int i=0;i<Archives.Count;i++) {
                if(Archives[i].ToLower()==name) {
                    Archives.RemoveAt(i);
                    UpdatedList=true;
                    return;
                }
            }
        }

        internal static string[] GetBSAEntries(string path) {
            List<string> files=new List<string>();
            BinaryReader br=null;
            br=new BinaryReader(File.OpenRead(path));
            if(Program.ReadCString(br)!="BSA") {
                br.Close();
                return new string[0];
            }
            br.ReadUInt32();
            int offset=br.ReadInt32();
            br.ReadUInt32();
            int FolderCount=br.ReadInt32();
            int TotalFileCount=br.ReadInt32();
            int TotalFolderLength=br.ReadInt32();
            int TotalFileNameLength=br.ReadInt32();
            br.ReadUInt32();
            long FileNameBlockStart=offset+TotalFolderLength+16*TotalFileCount+17*FolderCount;
            long folderpos=br.BaseStream.Position;
            for(int i=0;i<FolderCount;i++) {
                br.BaseStream.Position=folderpos+8;
                int filecount=br.ReadInt32();
                br.BaseStream.Position=br.ReadInt32()-TotalFileNameLength+1;
                string fname=Program.ReadCString(br).Replace('/', '\\');
                br.BaseStream.Position=FileNameBlockStart;
                for(int j=0;j<filecount;j++) files.Add(fname+"\\"+Program.ReadCString(br));
                FileNameBlockStart=br.BaseStream.Position;
                folderpos+=16;
            }
            br.Close();
            return files.ToArray();
        }

        internal static string[] GetBSAEntries() {
            List<string> files=new List<string>();
            foreach(string path in Directory.GetFiles("data", "*.bsa")) {
                files.AddRange(GetBSAEntries(path));
            }
            files.Sort();
            return files.ToArray();
        }

        internal static int RestoreBSA() {
            if(!File.Exists(Program.BSAEditFile)) return 0;
            List<BSAEdit> Edits;
            Formatter f=new Formatter();
            Stream s=File.OpenRead(Program.BSAEditFile);
            Edits=(List<BSAEdit>)f.Deserialize(s);
            s.Close();
            int count=0;
            try {
                foreach(BSAEdit be in Edits) {
                    if(!File.Exists(be.FileName)) {
                        MessageBox.Show("Edited BSA '"+be.FileName+"' is missing, and will be ignored","Warning");
                        continue;
                    }
                    FileInfo fi=new FileInfo(be.FileName);
                    if(be.FileSize!=0&&fi.Length!=be.FileSize) {
                        MessageBox.Show("Edited BSA '"+be.FileName+"' has changed since the last time the BSA patcher was run, and will be ignored","Warning");
                        continue;
                    }
                    count+=be.Offsets.Count;
                    DateTime origdate=fi.LastWriteTime;
                    BinaryWriter bw=new BinaryWriter(File.Open(be.FileName, FileMode.Open));
                    try {
                        for(int i=0;i<be.Offsets.Count;i++) {
                            bw.BaseStream.Position=be.Offsets[i];
                            bw.Write(be.OldData[i]);
                        }
                    } finally {
                        if(bw!=null) bw.Close();
                    }
                    fi.LastWriteTime=origdate;
                    
                }
                File.Delete(Program.BSAEditFile);
            } catch (Exception ex) {
                MessageBox.Show("An error occured while trying to undo BSA archive edits\n"+
                    "If you have something else open which may be using the BSA's, (oblivion, the CS, another mod tool, etc.), please close it while using obmm\n"+
                    "Error: "+ex.Message, "Error");
                return -1;
            }
            return count;
        }

        private static bool ShouldFolderBeProcessed(string folder) {
            int index=folder.IndexOf('\\');
            if(index==-1) return (Settings.InvalidationFlags&ArchiveInvalidationFlags.Base)>0;
            folder=folder.Substring(0, index);
            switch(folder) {
            case "textures": return (Settings.InvalidationFlags&ArchiveInvalidationFlags.Textures)>0;
            case "meshes": return (Settings.InvalidationFlags&ArchiveInvalidationFlags.Meshes)>0;
            case "sound": return (Settings.InvalidationFlags&ArchiveInvalidationFlags.Sounds)>0;
            case "music": return (Settings.InvalidationFlags&ArchiveInvalidationFlags.Music)>0;
            case "fonts": return (Settings.InvalidationFlags&ArchiveInvalidationFlags.Fonts)>0;
            case "menus": return (Settings.InvalidationFlags&ArchiveInvalidationFlags.Menus)>0;
            case "trees":
            case "distantlod": return (Settings.InvalidationFlags&ArchiveInvalidationFlags.TreesLOD)>0;
            default: return (Settings.InvalidationFlags&ArchiveInvalidationFlags.Misc)>0;
            }
        }

        private static uint GetFlags() {
            uint result=0;
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Meshes)>0) {
                result|=0x1;
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Textures)>0) {
                result|=0x2;
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Menus)>0) {
                result|=0xC;
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Sounds)>0) {
                result|=0xC;
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Music)>0) {
                result|=0x10;
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.TreesLOD)>0) {
                result|=0x40;
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Fonts)>0) {
                result|=0x80;
            }
            if((Settings.InvalidationFlags&(ArchiveInvalidationFlags.Misc|ArchiveInvalidationFlags.Base))>0) {
                result|=0x120;
            }
            return result;
        }

        private static string[] ProcessFolder(BSAEditData Data, BSAEdit Edits, BinaryWriter bw) {
            List<string> Problems=new List<string>();
            for(int i=1;i<Data.Files.Count-1;i++) {
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.IgnoreNormal)>0) {
                    if(Data.Files[i].FileName.EndsWith("_n.dds")) continue;
                    if(Data.Files[i].FileName.EndsWith("_g.dds")) continue;
                }
                if(Data.Files[i].Exists) {
                    int count=1;
                    bool EditDown=false;
                    bool EditAtAll=true;

                    ulong TempHash=Data.Files[i].Hash+1;
                    while(Data.Files[i+count].Hash==TempHash) {
                        if(!Data.Files[i+count++].Exists) {
                            EditDown=true;
                            break;
                        } else TempHash++;
                    }
                    if(!EditDown) {
                        if(Data.Files[i+count].Hash>TempHash+300) TempHash+=0xCD;
                        else if(Data.Files[i+count].Hash>TempHash+20) TempHash+=0xD;
                    } else {
                        TempHash=Data.Files[i].Hash-1;
                        count=1;
                        while(Data.Files[i-count].Hash==TempHash) {
                            if(!Data.Files[i-count++].Exists) {
                                hashCollisions++;
                                Problems.Add(Data.Files[i].FileName);
                                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.HashWarn)>0) {
                                    System.Windows.Forms.MessageBox.Show("Hash map collision between:\n"+
                                    Data.Files[i-1].FileName+"\n"+
                                    Data.Files[i].FileName+"\n"+
                                    Data.Files[i+1].FileName+"\n\n"+
                                    Data.Files[i].FileName+" may not show up correctly in game", "Warning");
                                }
                                EditAtAll=false;
                                break;
                            } else TempHash--;
                        }
                        if(EditAtAll) {
                            if(Data.Files[i-count].Hash<TempHash-300) TempHash-=0xCD;
                            else if(Data.Files[i-count].Hash<TempHash-20) TempHash-=0xD;
                        }
                    }
                    if(EditAtAll) {
                        entriesModified++;
                        bw.BaseStream.Position=Data.Files[i].HashOffset;
                        bw.Write(TempHash);
                        Edits.Offsets.Add(Data.Files[i].HashOffset);
                        Edits.OldData.Add(Data.Files[i].Hash);
                    }
                }
            }
            return Problems.ToArray();
        }

        internal static void SnipBSA() {
            filesModified=0;
            entriesModified=0;
            hashCollisions=0;
            List<BSAEdit> Edits=new List<BSAEdit>();
            List<string> ProblemFiles=new List<string>();
            string[] Paths=Directory.GetFiles("data", "*.bsa");
            ProgressForm pf=new ProgressForm("", false);
            pf.SetProgressRange(Paths.Length+1);
            pf.Show();
            foreach(string path in Paths) {
                pf.Text="Patching BSA '"+Path.GetFileName(path)+"'";
                pf.UpdateProgress();
                pf.Invalidate();
                pf.Update();
                FileInfo fi=new FileInfo(path);
                BSAEdit be=new BSAEdit(path,fi.Length);
                DateTime origdate=fi.LastWriteTime;
                BinaryReader br;
                try {
                    br=new BinaryReader(File.Open(path, FileMode.Open));
                } catch(Exception ex) {
                    MessageBox.Show("BSA '"+path+"' could not be opened, and has not been edited.\n"+
                        "Make sure that nothing else is using this file, and then try running the BSA patcher again.\n"+
                        "Error: "+ex.Message);
                    continue;
                }
                BinaryWriter bw=new BinaryWriter(br.BaseStream);
                try {
                    if(Program.ReadCString(br)!="BSA") continue;
                    br.ReadUInt32();
                    int offset=br.ReadInt32();
                    br.ReadUInt32();
                    int FolderCount=br.ReadInt32();
                    int TotalFileCount=br.ReadInt32();
                    int TotalFolderLength=br.ReadInt32();
                    int TotalFileNameLength=br.ReadInt32();
                    uint flags=br.ReadUInt32();
                    if((Settings.InvalidationFlags&ArchiveInvalidationFlags.EditAllEntries)==0) {
                        if((flags&GetFlags())==0) continue;
                    }
                    long FileNameBlockStart=offset+TotalFolderLength+16*TotalFileCount+17*FolderCount;
                    long folderpos=br.BaseStream.Position;
                    for(int i=0;i<FolderCount;i++) {
                        br.BaseStream.Position=folderpos+8;
                        int filecount=br.ReadInt32();
                        br.BaseStream.Position=br.ReadInt32()-TotalFileNameLength+1;
                        string fname=Program.ReadCString(br).Replace('/', '\\');
                        if(Directory.Exists("Data\\"+fname)) {
                            BSAEditData EditData=new BSAEditData(fname);
                            EditData.Files.Add(new BSAEditFileInfo(0, 0));
                            for(int j=0;j<filecount;j++) {
                                long pos=br.BaseStream.Position;
                                EditData.Files.Add(new BSAEditFileInfo(br.ReadUInt64(), pos));
                                br.BaseStream.Position+=8;
                            }
                            EditData.Files.Add(new BSAEditFileInfo(ulong.MaxValue, 0));
                            br.BaseStream.Position=FileNameBlockStart;
                            for(int j=0;j<filecount;j++) {
                                string FileName=Program.ReadCString(br);
                                EditData.Files[j+1].FileName=FileName;
                                EditData.Files[j+1].Exists=File.Exists("Data\\"+fname+"\\"+FileName);
                            }
                            FileNameBlockStart=br.BaseStream.Position;
                            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.EditAllEntries)>0||ShouldFolderBeProcessed(fname)) {
                                string[] Problems=ProcessFolder(EditData, be, bw);
                                if(ShouldFolderBeProcessed(fname)) {
                                    foreach(string s in Problems) ProblemFiles.Add(fname+'\\'+s);
                                }
                            }
                        } else {
                            br.BaseStream.Position=FileNameBlockStart;
                            for(int j=0;j<filecount;j++) Program.ReadCString(br);
                            FileNameBlockStart=br.BaseStream.Position;
                        }
                        folderpos+=16;
                    }
                } catch { 
                    continue; 
                } finally {
                    br.Close();
                }
                if(be.Offsets.Count!=0) Edits.Add(be);
                fi.LastWriteTime=origdate;
            }
            pf.Text="Patching BSA's (Saving changes)";
            if(Edits.Count>0) {
                Formatter f=new Formatter();
                Stream s=File.Create(Program.BSAEditFile);
                f.Serialize(s, Edits);
                s.Close();
                filesModified=Edits.Count;
            } else {
                filesModified=0;
                entriesModified=0;
            }
            if(ProblemFiles.Count>0&&(Settings.InvalidationFlags&ArchiveInvalidationFlags.HashGenAI)>0) {
                StreamWriter sw=new StreamWriter(File.Create(InvalidationFile), System.Text.Encoding.Default);
                foreach(string s in ProblemFiles) sw.WriteLine(s.Replace('\\', '/'));
                sw.Close();
            } else {
                try { File.Delete(InvalidationFile); } catch { }
            }
            pf.Unblock();
            pf.Close();
        }

        internal static void UpdateInvalidationFile() {
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.BSARedirection)==0) UnregisterBSA("..\\"+RedirectionPath);
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.EditBSAs)>0) {
                if(RestoreBSA()==-1) return;
                SnipBSA();
                return;
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Universal)>0) {
                StreamWriter sw=new StreamWriter(File.Open(InvalidationFile, FileMode.Create), System.Text.Encoding.Default);
                sw.WriteLine("meshes/ \\s");
                sw.WriteLine("textures/ \\s");
                sw.WriteLine("sound/ \\s");
                sw.Close();
                return;
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.BSARedirection)>0) {
                if(File.Exists(InvalidationFile)) File.Delete(InvalidationFile);
                string path=((Settings.InvalidationFlags&ArchiveInvalidationFlags.PackFaceTextures)>0)?Path.GetFullPath(@"data\textures\faces"):null;
                if(path!=null&&!Directory.Exists(path)) path=null;
                Forms.BSACreator.CreateBSA(RedirectionPath, path, null, 0, 0, true);
                RegisterBSA("..\\"+RedirectionPath);
                return;
            }
            List<string> files=new List<string>();
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.MatchExtensions)>0) {
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Textures)>0) {
                    if(Directory.Exists("Data\\textures")) {
                        List<string> temp=new List<string>();
                        temp.AddRange(Directory.GetFiles("Data\\Textures\\", "*.dds", SearchOption.AllDirectories));
                        temp.AddRange(Directory.GetFiles("Data\\Textures\\", "*.tga", SearchOption.AllDirectories));
                        temp.AddRange(Directory.GetFiles("Data\\Textures\\", "*.hdr", SearchOption.AllDirectories));
                        temp.AddRange(Directory.GetFiles("Data\\Textures\\", "*.bmp", SearchOption.AllDirectories));
                        temp.AddRange(Directory.GetFiles("Data\\Textures\\", "*.jpg", SearchOption.AllDirectories));
                        temp.AddRange(Directory.GetFiles("Data\\Textures\\", "*.jpeg", SearchOption.AllDirectories));
                        if((Settings.InvalidationFlags&ArchiveInvalidationFlags.IgnoreNormal)>0) {
                            for(int i=0;i<temp.Count;i++) {
                                if(Path.GetFileNameWithoutExtension(temp[i]).EndsWith("_n")||
                                Path.GetFileNameWithoutExtension(temp[i]).EndsWith("_g"))
                                    temp.RemoveAt(i--);
                            }
                        }
                        files.AddRange(temp.ToArray());
                    }
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Meshes)>0) {
                    if(Directory.Exists("Data\\meshes"))
                    files.AddRange(Directory.GetFiles("Data\\meshes", "*.nif", SearchOption.AllDirectories));
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Sounds)>0) {
                    if(Directory.Exists("Data\\sound")) {
                        files.AddRange(Directory.GetFiles("Data\\sound", "*.mp3", SearchOption.AllDirectories));
                        files.AddRange(Directory.GetFiles("Data\\sound", "*.wav", SearchOption.AllDirectories));
                    }
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Music)>0) {
                    if(Directory.Exists("Data\\music")) {
                        files.AddRange(Directory.GetFiles("Data\\music", "*.mp3", SearchOption.AllDirectories));
                        files.AddRange(Directory.GetFiles("Data\\music", "*.wav", SearchOption.AllDirectories));
                    }
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Fonts)>0) {
                    if(Directory.Exists("Data\\fonts")) {
                        files.AddRange(Directory.GetFiles("Data\\Fonts", "*.fnt", SearchOption.AllDirectories));
                        files.AddRange(Directory.GetFiles("Data\\Fonts", "*.tex", SearchOption.AllDirectories));
                    }
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Menus)>0) {
                    if(Directory.Exists("Data\\menus")) {
                        files.AddRange(Directory.GetFiles("Data\\menus", "*.xml", SearchOption.AllDirectories));
                        files.AddRange(Directory.GetFiles("Data\\menus", "*.txt", SearchOption.AllDirectories));
                    }
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Misc)>0) {
                    if(Directory.Exists("Data\\video"))
                        files.AddRange(Directory.GetFiles("Data\\video", "*.bik", SearchOption.AllDirectories));
                    if(Directory.Exists("Data\\shaders")) {
                        files.AddRange(Directory.GetFiles("Data\\shaders", "*.fx", SearchOption.AllDirectories));
                        files.AddRange(Directory.GetFiles("Data\\shaders", "*.sdp", SearchOption.AllDirectories));
                    }
                    if(Directory.Exists("Data\\lsdata"))
                        files.AddRange(Directory.GetFiles("Data\\lsdata", "*.dat", SearchOption.AllDirectories));
                    if(Directory.Exists("Data\\facegen"))
                    files.AddRange(Directory.GetFiles("Data\\facegen", "*.ctl", SearchOption.AllDirectories));
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.TreesLOD)>0) {
                    if(Directory.Exists("Data\\trees"))
                        files.AddRange(Directory.GetFiles("Data\\trees", "*.spt", SearchOption.AllDirectories));
                    if(Directory.Exists("Data\\distantlod")) {
                        files.AddRange(Directory.GetFiles("Data\\distantlod", "*.lod", SearchOption.AllDirectories));
                        files.AddRange(Directory.GetFiles("Data\\distantlod", "*.cmp", SearchOption.AllDirectories));
                    }
                }
            } else {
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Textures)>0) {
                    if(Directory.Exists("Data\\textures")) {
                        List<string> temp=new List<string>();
                        temp.AddRange(Directory.GetFiles("Data\\Textures\\", "*", SearchOption.AllDirectories));
                        if((Settings.InvalidationFlags&ArchiveInvalidationFlags.IgnoreNormal)>0) {
                            for(int i=0;i<temp.Count;i++) {
                                if(Path.GetFileNameWithoutExtension(temp[i]).EndsWith("_n")||
                                Path.GetFileNameWithoutExtension(temp[i]).EndsWith("_g"))
                                    temp.RemoveAt(i--);
                            }
                        }
                        files.AddRange(temp.ToArray());
                    }
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Meshes)>0) {
                    if(Directory.Exists("Data\\meshes"))
                        files.AddRange(Directory.GetFiles("Data\\meshes", "*", SearchOption.AllDirectories));
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Sounds)>0) {
                    if(Directory.Exists("Data\\sound"))
                        files.AddRange(Directory.GetFiles("Data\\sound", "*", SearchOption.AllDirectories));
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Music)>0) {
                    if(Directory.Exists("Data\\music"))
                        files.AddRange(Directory.GetFiles("Data\\music", "*", SearchOption.AllDirectories));
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Fonts)>0) {
                    if(Directory.Exists("Data\\fonts"))
                        files.AddRange(Directory.GetFiles("Data\\fonts", "*", SearchOption.AllDirectories));
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Menus)>0) {
                    if(Directory.Exists("Data\\menus"))
                        files.AddRange(Directory.GetFiles("Data\\menus", "*", SearchOption.AllDirectories));
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Misc)>0) {
                    if(Directory.Exists("Data\\video"))
                        files.AddRange(Directory.GetFiles("Data\\video", "*", SearchOption.AllDirectories));
                    if(Directory.Exists("Data\\shaders"))
                        files.AddRange(Directory.GetFiles("Data\\shaders", "*", SearchOption.AllDirectories));
                    if(Directory.Exists("Data\\lsdata"))
                        files.AddRange(Directory.GetFiles("Data\\lsdata", "*", SearchOption.AllDirectories));
                    if(Directory.Exists("Data\\facegen"))
                        files.AddRange(Directory.GetFiles("Data\\facegen", "*", SearchOption.AllDirectories));
                }
                if((Settings.InvalidationFlags&ArchiveInvalidationFlags.TreesLOD)>0) {
                    if(Directory.Exists("Data\\trees"))
                        files.AddRange(Directory.GetFiles("Data\\trees", "*", SearchOption.AllDirectories));
                    if(Directory.Exists("Data\\distantlod"))
                        files.AddRange(Directory.GetFiles("Data\\distantlod", "*", SearchOption.AllDirectories));
                }
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.Base)>0) {
                List<string> temp=new List<string>();
                temp.AddRange(Directory.GetFiles("Data", "*"));
                for(int i=0;i<temp.Count;i++) {
                    switch(Path.GetExtension(temp[i]).ToLower()) {
                    case ".esp":
                    case ".esm":
                    case ".bsa":
                    case ".txt":
                        temp.RemoveAt(i--);
                        break;
                    }
                }
                files.AddRange(temp.ToArray());
            }
            for(int i=0;i<files.Count;i++) {
                files[i]=files[i].ToLower();
                if(files[i].StartsWith(Program.CurrentDir)) files[i]=files[i].Substring(Program.CurrentDir.Length);
                if(files[i].StartsWith("data")) files[i]=files[i].Substring(5);
            }
            if((Settings.InvalidationFlags&ArchiveInvalidationFlags.BSAOnly)>0) {
                string[] entries=GetBSAEntries();
                files.Sort();
                int upto=0;
                for(int i=0;i<files.Count;) {
                    if(upto==entries.Length) {
                        files.RemoveRange(i, files.Count-i);
                        break;
                    }
                    int result=string.Compare(entries[upto], files[i]);
                    if(result>0) files.RemoveAt(i);
                    else if(result<0) upto++;
                    else i++;
                }
            }
            entriesModified=files.Count;
            if(files.Count>0) {
                StreamWriter sw=new StreamWriter(File.Open(InvalidationFile, FileMode.Create), System.Text.Encoding.Default);
                foreach(string s in files) sw.WriteLine(s.Replace('\\','/'));
                sw.Close();
            } else {
                File.Delete(InvalidationFile);
            }
        }

        internal static ulong GenHash(string s) {
            string extension="";
            int i=s.LastIndexOf('.');
            if(i!=-1) {
                extension=s.Substring(i);
                s=s.Remove(i);
            }
            return GenHash(s, extension);
        }

        internal static ulong GenHash(string file, string ext) {
            file=file.ToLower();
            ext=ext.ToLower();
            ulong hash=0;
            if(file.Length>0) {
                hash=(ulong)(
                        (((byte)file[file.Length-1])*0x1)+
                        ((file.Length>2?(byte)file[file.Length-2]:(byte)0)*0x100)+
                        (file.Length*0x10000)+
                        (((byte)file[0])*0x1000000)
                     );
            }
            if(file.Length>3) hash+=(ulong)(GenHash2(file.Substring(1, file.Length-3))*0x100000000);
            if(ext.Length>0) {
                hash+=(ulong)(GenHash2(ext)*0x100000000);
                byte i=0;
                switch(ext) {
                case ".nif": i=1; break;
                case ".kf": i=2; break;
                case ".dds": i=3; break;
                case ".wav": i=4; break;
                }
                if(i!=0) {
                    byte a=(byte)(((i&0xfc)<<5)+(byte)((hash&0xff000000)>>24));
                    byte b=(byte)(((i&0xfe)<<6)+(byte)(hash&0xff));
                    byte c=(byte)((i<<7)+(byte)((hash&0xff00)>>8));
                    hash-=hash&0xFF00FFFF;
                    hash+=(uint)((a<<24)+b+(c<<8));
                }
            }
            return hash;
        }

        internal static uint GenHash2(string s) {
            uint hash=0;
            for(int i=0;i<s.Length;i++) {
                hash*=0x1003f;
                hash+=(byte)s[i];
            }
            return hash;
        }
    }

    internal static class OblivionRenderInfo {
        private static readonly string info=Program.OblivionINIDir+"RendererInfo.txt";

        internal static string GetInfo(string s) {
            s=s.ToLower();
            try {
                string[] lines=File.ReadAllLines(info);
                for(int i=0;i<lines.Length;i++) {
                    lines[i]=lines[i].Trim().ToLower();
                    if(lines[i].StartsWith(s)) {
                        lines=lines[i].Split(':');
                        if(lines.Length!=2) return "-1";
                        return lines[1].Trim();
                    }
                }
                return "Value '"+s+"' not found";
            } catch (Exception e) { return e.Message; }
        }

    }

}