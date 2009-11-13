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
using System.Collections.Generic;
using System.Security.Permissions;
using MessageBox=System.Windows.Forms.MessageBox;
using mbButtons=System.Windows.Forms.MessageBoxButtons;
using DialogResult=System.Windows.Forms.DialogResult;
using File=System.IO.File;
using Directory=System.IO.Directory;
using Path=System.IO.Path;
using Regex=System.Text.RegularExpressions.Regex;
using RegexOptions=System.Text.RegularExpressions.RegexOptions;

namespace OblivionModManager.Scripting {

    internal class ScriptFunctions : IScriptFunctions {
        private readonly System.Security.PermissionSet permissions;
        private readonly ScriptReturnData srd;
        private readonly string DataFiles;
        private readonly string Plugins;
        private readonly string[] dataFileList;
        private readonly string[] pluginList;
        private readonly string[] dataFolderList;
        private readonly string[] pluginFolderList;
        private readonly bool testMode;

        internal ScriptFunctions(ScriptReturnData srd, string dataFilesPath, string pluginsPath) {
            this.srd=srd;
            this.DataFiles=dataFilesPath;
            this.Plugins=pluginsPath;
            permissions=new System.Security.PermissionSet(PermissionState.None);
            List<string> paths=new List<string>(4);
            paths.Add(Program.CurrentDir);
            paths.Add(Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "My games\\Oblivion"));
            if(dataFilesPath!=null) paths.Add(dataFilesPath);
            if(pluginsPath!=null) paths.Add(pluginsPath);
            permissions.AddPermission(new FileIOPermission(FileIOPermissionAccess.AllAccess, paths.ToArray()));
            permissions.AddPermission(new UIPermission(UIPermissionWindow.AllWindows));
            testMode=false;
        }

        internal ScriptFunctions(ScriptReturnData srd, string[] dataFiles, string[] plugins) {
            this.srd=srd;
            this.dataFileList=(string[])dataFiles.Clone();
            this.pluginList=(string[])plugins.Clone();

            List<string> df = new List<string>();
            string dir;

            df.Add("");
            for(int i = 0;i < dataFileList.Length;++i) {
                dataFileList[i] = dataFileList[i].ToLower();
                dir = dataFileList[i];
                while(dir.Contains(@"\")) {
                    dir = Path.GetDirectoryName(dir);
                    if(dir != null && dir != "") {
                        if(!df.Contains(dir)) df.Add(dir);
                    } else break;
                }
            }
            dataFolderList=df.ToArray();

            df.Clear();
            df.Add("");
            for(int i = 0;i < pluginList.Length;++i) {
                pluginList[i] = pluginList[i].ToLower();
                dir = pluginList[i];
                while(dir.Contains(@"\")) {
                    dir = Path.GetDirectoryName(dir);
                    if(dir != null && dir != "") {
                        if(!df.Contains(dir)) df.Add(dir);
                    } else break;
                }
            }
            pluginFolderList=df.ToArray();

            string[] paths=new string[2];
            paths[0]=Program.CurrentDir;
            paths[1]=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments), "My games\\Oblivion");
            permissions=new System.Security.PermissionSet(PermissionState.None);
            permissions.AddPermission(new FileIOPermission(FileIOPermissionAccess.PathDiscovery|FileIOPermissionAccess.Read, paths));
            permissions.AddPermission(new UIPermission(UIPermissionWindow.AllWindows));
            testMode=true;
        }

        private bool ExistsIn(string path, string[] files) {
            if(files == null) return false;
            return Array.Exists<string>(files, new Predicate<string>(path.ToLower().Equals));
        }

        private void CheckPathSafty(string path) {
            if(!Program.IsSafeFileName(path)) throw new ScriptingException("Illegal file name: '"+path+"'");
        }
        private void CheckPluginSafty(string path) {
            permissions.Assert();
            if(!Program.IsSafeFileName(path)) throw new ScriptingException("Illegal file name: '"+path+"'");
            if(!(testMode?ExistsIn(path, pluginList):File.Exists(Plugins+path))) throw new ScriptingException("File '"+path+"' not found");
        }
        private void CheckDataSafty(string path) {
            permissions.Assert();
            if(!Program.IsSafeFileName(path)) throw new ScriptingException("Illegal file name: '"+path+"'");
            if(!(testMode?ExistsIn(path, dataFileList):File.Exists(DataFiles+path))) throw new ScriptingException("File '"+path+"' not found");
        }
        private void CheckFolderSafty(string path) {
            if(!Program.IsSafeFolderName(path)) throw new ScriptingException("Illegal folder name: '"+path+"'");
        }
        private void CheckPluginFolderSafty(string path) {
            permissions.Assert();
            if(path.EndsWith("\\")||path.EndsWith("/")) path=path.Remove(path.Length-1);
            if(!Program.IsSafeFolderName(path)) throw new ScriptingException("Illegal folder name: '"+path+"'");
            if(!(testMode?ExistsIn(path, pluginFolderList):Directory.Exists(Plugins+path))) throw new ScriptingException("Folder '"+path+"' not found");
        }
        private void CheckDataFolderSafty(string path) {
            permissions.Assert();
            if(path.EndsWith("\\")||path.EndsWith("/")) path=path.Remove(path.Length-1);
            if(!Program.IsSafeFolderName(path)) throw new ScriptingException("Illegal folder name: '"+path+"'");
            if(!(testMode?ExistsIn(path, dataFolderList):Directory.Exists(DataFiles+path))) throw new ScriptingException("Folder '"+path+"' not found");
        }
        private void CancelCheck() { if(Program.KeyPressed((int)System.Windows.Forms.Keys.Escape)) throw new ExecutionCancelledException(); }

        private string[] SimulateFSOutput(string[] fsList, string path, string pattern, bool recurse) {
            pattern = "^" + (pattern == "" ? ".*" : pattern.Replace("[", @"\[").Replace(@"\", "\\").Replace("^", @"\^").Replace("$", @"\$").
                Replace("|", @"\|").Replace("+", @"\+").Replace("(", @"\(").Replace(")", @"\)").
                Replace(".", @"\.").Replace("*", ".*").Replace("?", ".{0,1}")) + "$";
            return Array.FindAll(fsList, delegate(string value)
            {
                if((path.Length > 0 && value.StartsWith(path.ToLower() + @"\")) || path.Length == 0) {
                    if(value == "" || (!recurse && Regex.Matches(value.Substring(path.Length), @"\\", RegexOptions.None).Count > 1)) return false;
                    if(Regex.IsMatch(value.Substring(value.LastIndexOf('\\') + 1), pattern)) return true;
                }
                return false;
            });
        }
        private string[] GetFilePaths(string path, string pattern, bool recurse) {
            permissions.Assert();
            return Directory.GetFiles(path, (pattern != ""&&pattern!=null) ? pattern : "*", recurse ? System.IO.SearchOption.AllDirectories : System.IO.SearchOption.TopDirectoryOnly);
        }
        private string[] GetDirectoryPaths(string path, string pattern, bool recurse) {
            permissions.Assert();
            return Directory.GetDirectories(path, (pattern != ""&&pattern!=null) ? pattern : "*", recurse ? System.IO.SearchOption.AllDirectories : System.IO.SearchOption.TopDirectoryOnly);
        }
        private string[] StripPathList(string[] paths, int baseLength) {
            for(int i=0;i<paths.Length;i++) if(Path.IsPathRooted(paths[i])) paths[i]=paths[i].Substring(baseLength);
            return paths;
        }

        public bool GetDisplayWarnings() { return Settings.ShowScriptWarnings; }

        public bool DialogYesNo(string msg) { return DialogYesNo(msg, "Question"); }
        public bool DialogYesNo(string msg, string title) {
            return MessageBox.Show(msg, title, System.Windows.Forms.MessageBoxButtons.YesNo)==System.Windows.Forms.DialogResult.Yes;
        }
        public bool DataFileExists(string path) {
            CheckPathSafty(path);
            permissions.Assert();
            return File.Exists("data\\"+path);
        }
        public Version GetOBMMVersion() {
            return new Version(Program.MajorVersion, Program.MinorVersion, Program.BuildNumber, 0);
        }
        public Version GetOBSEVersion() {
            permissions.Assert();
            if(!File.Exists("obse_loader.exe")) return null;
            else return new Version(System.Diagnostics.FileVersionInfo.GetVersionInfo("obse_loader.exe").FileVersion.Replace(", ", "."));
        }
        public Version GetOBGEVersion() {
            permissions.Assert();
            if(!File.Exists("data\\obse\\plugins\\obge.dll")) return null;
            else return new Version(System.Diagnostics.FileVersionInfo.GetVersionInfo("data\\obse\\plugins\\obge.dll").FileVersion.Replace(", ", "."));
        }
        public Version GetOblivionVersion() {
            permissions.Assert();
            return new Version(System.Diagnostics.FileVersionInfo.GetVersionInfo("oblivion.exe").FileVersion.Replace(", ", "."));
        }
        public Version GetOBSEPluginVersion(string plugin) {
            plugin=Path.ChangeExtension(Path.Combine("data\\obse\\plugins", plugin), ".dll");
            CheckPathSafty(plugin);
            permissions.Assert();
            if(!File.Exists(plugin)) return null;
            else return new Version(System.Diagnostics.FileVersionInfo.GetVersionInfo(plugin).FileVersion.Replace(", ", "."));
        }

        public string[] GetPlugins(string path, string pattern, bool recurse) {
            CheckPluginFolderSafty(path);
            return testMode ? SimulateFSOutput(pluginList, path, pattern, recurse)
                : StripPathList(GetFilePaths(Plugins + path, pattern, recurse), Plugins.Length);
        }
        public string[] GetDataFiles(string path, string pattern, bool recurse) {
            CheckDataFolderSafty(path);
            return testMode ? SimulateFSOutput(dataFileList, path, pattern, recurse)
                : StripPathList(GetFilePaths(DataFiles + path, pattern, recurse), DataFiles.Length);
        }
        public string[] GetPluginFolders(string path, string pattern, bool recurse) {
            CheckPluginFolderSafty(path);
            return testMode ? SimulateFSOutput(pluginFolderList, path, pattern, recurse)
                : StripPathList(GetDirectoryPaths(Plugins + path, pattern, recurse), Plugins.Length);
        }
        public string[] GetDataFolders(string path, string pattern, bool recurse) {
            CheckDataFolderSafty(path);
            return testMode ? SimulateFSOutput(dataFolderList, path, pattern, recurse)
                : StripPathList(GetDirectoryPaths(DataFiles + path, pattern, recurse), DataFiles.Length);
        }

        public string[] GetExistingEspNames() {
            permissions.Assert();
            Program.Data.SortEsps();
            string[] names=new string[Program.Data.Esps.Count];
            for(int i=0;i<names.Length;i++) names[i]=Program.Data.Esps[i].FileName;
            return names;
        }
        public string[] GetActiveOmodNames() {
            string[] names=new string[Program.Data.omods.Count];
            for(int i=0;i<names.Length;i++) names[i]=Program.Data.omods[i].ModName;
            return names;
        }

        public string[] Select(string[] items, string[] previews, string[] descs, string title, bool many) {
            permissions.Assert();
            if(previews!=null) {
                for(int i=0;i<previews.Length;i++) {
                    if(previews[i]!=null) {
                        CheckDataSafty(previews[i]);
                        previews[i]=DataFiles+previews[i];
                    }
                }
            }
            Forms.SelectForm sf=new OblivionModManager.Forms.SelectForm(items, title, many, previews, descs);
            sf.ShowDialog();
            CancelCheck();
            string[] result=new string[sf.SelectedIndex.Length];
            for(int i=0;i<sf.SelectedIndex.Length;i++) {
                result[i]=items[sf.SelectedIndex[i]];
            }
            return result;
        }

        public void Message(string msg) {
            MessageBox.Show(msg);
        }
        public void Message(string msg, string title) {
            MessageBox.Show(msg, title);
        }
        public void DisplayImage(string path) { DisplayImage(path, null); }
        public void DisplayImage(string path, string title) {
            CheckDataSafty(path);
            permissions.Assert();
            System.Drawing.Image image=System.Drawing.Image.FromFile(DataFiles+path);
            new ImageForm(image, (title!=null)?title:path).ShowDialog();
            image.Dispose();
        }
        public void DisplayText(string path) { DisplayText(path, null); }
        public void DisplayText(string path, string title) {
            CheckDataSafty(path);
            permissions.Assert();
            string s=File.ReadAllText(DataFiles+path, System.Text.Encoding.Default);
            new TextEditor((title!=null)?title:path, s, true, false).ShowDialog();
        }

        public void LoadEarly(string plugin) {
            CheckPathSafty(plugin);
            plugin=plugin.ToLower();
            if(srd.EarlyPlugins.Contains(plugin)) return;
            srd.EarlyPlugins.Add(plugin);
        }
        public void LoadBefore(string plugin1, string plugin2) { ModLoadOrder(plugin1, plugin2, false); }
        public void LoadAfter(string plugin1, string plugin2) { ModLoadOrder(plugin1, plugin2, true); }
        private void ModLoadOrder(string plugin1, string plugin2, bool after) {
            CheckPathSafty(plugin1);
            string path1=plugin1.ToLower();
            bool found=false;
            for(int i=0;i<srd.CopyPlugins.Count;i++) {
                if(srd.CopyPlugins[i].CopyTo==path1) {
                    found=true;
                    break;
                }
            }
            if(!found) CheckPluginSafty(plugin1);
            CheckPathSafty(plugin2);
            plugin1=plugin1.ToLower();
            plugin2=plugin2.ToLower();
            for(int i=0;i<srd.LoadOrderList.Count;i++) {
                if(plugin1==srd.LoadOrderList[i].Plugin&&plugin2==srd.LoadOrderList[i].Target) srd.LoadOrderList.RemoveAt(i--);
            }
            srd.LoadOrderList.Add(new PluginLoadInfo(plugin1, plugin2, after));
        }
        public void SetNewLoadOrder(string[] plugins) {
            if(plugins.Length!=Program.Data.Esps.Count) throw new ScriptingException("SetNewLoadOrder was called with an invalid plugin list");
            permissions.Assert();
            for(int i=0;i<plugins.Length;i++) {
                CheckPathSafty(plugins[i]);
                plugins[i]=Path.Combine("data\\", plugins[i]);
                if(!File.Exists(plugins[i])) throw new ScriptingException("Plugin '"+plugins[i]+"' does not exist");
            }
            for(int i=1;i<=plugins.Length;i++) {
                new System.IO.FileInfo(plugins[i-1]).LastWriteTime=new DateTime(2005, 1+(i-i%28)/28, i%28);
            }
            Program.Data.SortEsps();
        }

        public void UncheckEsp(string plugin) {
            CheckPluginSafty(plugin);
            plugin=plugin.ToLower();
            if(!srd.UncheckedPlugins.Contains(plugin)) srd.UncheckedPlugins.Add(plugin);
        }
        public void SetDeactivationWarning(string plugin, DeactiveStatus warning) {
            CheckPluginSafty(plugin);
            plugin=plugin.ToLower();
            for(int i=0;i<srd.EspDeactivation.Count;i++) {
                if(srd.EspDeactivation[i].Plugin==plugin) srd.EspDeactivation.RemoveAt(i--);
            }
            srd.EspDeactivation.Add(new ScriptEspWarnAgainst(plugin, warning));
        }

        public void ConflictsWith(string filename) { ConflictsWith(filename, 0, 0, 0, 0, null, ConflictLevel.MajorConflict, false); }
        public void ConslictsWith(string filename, string comment) { ConflictsWith(filename, 0, 0, 0, 0, comment, ConflictLevel.MajorConflict, false); }
        public void ConflictsWith(string filename, string comment, ConflictLevel level) { ConflictsWith(filename, 0, 0, 0, 0, comment, level, false); }
        public void ConflictsWith(string name, int minMajorVersion, int minMinorVersion, int maxMajorVersion, int maxMinorVersion) {
            ConflictsWith(name, minMajorVersion, minMinorVersion, maxMajorVersion, maxMinorVersion, null, ConflictLevel.MajorConflict, false);
        }
        public void ConflictsWith(string name, int minMajorVersion, int minMinorVersion, int maxMajorVersion, int maxMinorVersion, string comment) {
            ConflictsWith(name, minMajorVersion, minMinorVersion, maxMajorVersion, maxMinorVersion, comment, ConflictLevel.MajorConflict, false);
        }
        public void ConflictsWith(string name, int minMajorVersion, int minMinorVersion, int maxMajorVersion, int maxMinorVersion, string comment, ConflictLevel level) {
            ConflictsWith(name, minMajorVersion, minMinorVersion, maxMajorVersion, maxMinorVersion, comment, level, false);
        }
        public void ConflictsWith(string name, int minMajorVersion, int minMinorVersion, int maxMajorVersion, int maxMinorVersion, string comment, ConflictLevel level, bool regex) {
            ConflictData cd=new ConflictData();
            cd.File=name;
            cd.Comment=comment;
            cd.level=level;
            cd.MinMajorVersion=minMajorVersion;
            cd.MinMinorVersion=minMinorVersion;
            cd.MaxMajorVersion=maxMajorVersion;
            cd.MaxMinorVersion=maxMinorVersion;
            cd.Partial=regex;
            srd.ConflictsWith.Add(cd);
        }
        public void DependsOn(string filename) { DependsOn(filename, 0, 0, 0, 0, null, false); }
        public void DependsOn(string filename, string comment) { DependsOn(filename, 0, 0, 0, 0, comment, false); }
        public void DependsOn(string name, int minMajorVersion, int minMinorVersion, int maxMajorVersion, int maxMinorVersion) {
            DependsOn(name, minMajorVersion, minMinorVersion, maxMajorVersion, maxMinorVersion, null, false);
        }
        public void DependsOn(string name, int minMajorVersion, int minMinorVersion, int maxMajorVersion, int maxMinorVersion, string comment) {
            DependsOn(name, minMajorVersion, minMinorVersion, maxMajorVersion, maxMinorVersion, comment, false);
        }
        public void DependsOn(string name, int minMajorVersion, int minMinorVersion, int maxMajorVersion, int maxMinorVersion, string comment, bool regex) {
            ConflictData cd=new ConflictData();
            cd.File=name;
            cd.Comment=comment;
            cd.MinMajorVersion=minMajorVersion;
            cd.MinMinorVersion=minMinorVersion;
            cd.MaxMajorVersion=maxMajorVersion;
            cd.MaxMinorVersion=maxMinorVersion;
            cd.Partial=regex;
            srd.DependsOn.Add(cd);
        }

        public void DontInstallAnyPlugins() { srd.InstallAllPlugins=false; }
        public void DontInstallAnyDataFiles() { srd.InstallAllData=false; }
        public void InstallAllPlugins() { srd.InstallAllPlugins=true; }
        public void InstallAllDataFiles() { srd.InstallAllData=true; }

        public void DontInstallPlugin(string name) {
            CheckPluginSafty(name);
            Program.strArrayRemove(srd.InstallPlugins, name);
            if(!Program.strArrayContains(srd.IgnorePlugins, name)) srd.IgnorePlugins.Add(name);
        }
        public void DontInstallDataFile(string name) {
            CheckDataSafty(name);
            Program.strArrayRemove(srd.InstallData, name);
            if(!Program.strArrayContains(srd.IgnoreData, name)) srd.IgnoreData.Add(name);
        }
        public void DontInstallDataFolder(string folder, bool recurse) {
            CheckDataFolderSafty(folder);
            if(testMode) {
                folder=folder.ToLower();
                if(folder.EndsWith("\\")||folder.EndsWith("/")) folder=folder.Remove(folder.Length-1);
                foreach(string s in dataFileList) {
                    if(s.StartsWith(folder)&&(recurse||s.IndexOf('\\', folder.Length+1)==-1)) {
                        Program.strArrayRemove(srd.InstallData, s);
                        if(!Program.strArrayContains(srd.IgnoreData, s)) srd.IgnoreData.Add(s);
                    }
                }
            } else {
                permissions.Assert();
                foreach(string path in Directory.GetFiles(DataFiles+folder, "*", recurse?System.IO.SearchOption.AllDirectories:System.IO.SearchOption.TopDirectoryOnly)) {
                    string file=Path.GetFullPath(path).Substring(DataFiles.Length);
                    Program.strArrayRemove(srd.InstallData, file);
                    if(!Program.strArrayContains(srd.IgnoreData, file)) srd.IgnoreData.Add(file);
                }
            }
        }
        public void InstallPlugin(string name) {
            CheckPluginSafty(name);
            Program.strArrayRemove(srd.IgnorePlugins, name);
            if(!Program.strArrayContains(srd.InstallPlugins, name)) srd.InstallPlugins.Add(name);
        }
        public void InstallDataFile(string name) {
            CheckDataSafty(name);
            Program.strArrayRemove(srd.IgnoreData, name);
            if(!Program.strArrayContains(srd.InstallData, name)) srd.InstallData.Add(name);
        }
        public void InstallDataFolder(string folder, bool recurse) {
            CheckDataFolderSafty(folder);
            if(testMode) {
                folder=folder.ToLower();
                if(folder.EndsWith("\\")||folder.EndsWith("/")) folder=folder.Remove(folder.Length-1);
                foreach(string s in dataFileList) {
                    if(s.StartsWith(folder)&&(recurse||s.IndexOf('\\', folder.Length+1)==-1)) {
                        Program.strArrayRemove(srd.IgnoreData, s);
                        if(!Program.strArrayContains(srd.InstallData, s)) srd.InstallData.Add(s);
                    }
                }
            } else {
                permissions.Assert();
                foreach(string path in Directory.GetFiles(DataFiles+folder, "*", recurse?System.IO.SearchOption.AllDirectories:System.IO.SearchOption.TopDirectoryOnly)) {
                    string file=Path.GetFullPath(path).Substring(DataFiles.Length);
                    Program.strArrayRemove(srd.IgnoreData, file);
                    if(!Program.strArrayContains(srd.InstallData, file)) srd.InstallData.Add(file);
                }
            }
        }

        public void CopyPlugin(string from, string to) {
            CheckPluginSafty(from);
            CheckPathSafty(to);
            string lto=to.ToLower();
            if(!lto.EndsWith(".esp")&&!lto.EndsWith(".esm")) throw new ScriptingException("Copied plugins must have a .esp or .esm file extension");
            if(to.Contains("\\")||to.Contains("/")) throw new ScriptingException("Cannot copy a plugin to a subdirectory of the data folder");
            for(int i=0;i<srd.CopyPlugins.Count;i++) {
                if(srd.CopyPlugins[i].CopyTo==lto) srd.CopyPlugins.RemoveAt(i--);
            }
            srd.CopyPlugins.Add(new ScriptCopyDataFile(from, to));
        }
        public void CopyDataFile(string from, string to) {
            CheckDataSafty(from);
            CheckPathSafty(to);
            string lto=to.ToLower();
            if(lto.EndsWith(".esm")||lto.EndsWith(".esp")) throw new ScriptingException("Copied data files cannot have a .esp or .esm file extension");
            for(int i=0;i<srd.CopyDataFiles.Count;i++) {
                if(srd.CopyDataFiles[i].CopyTo==lto) srd.CopyDataFiles.RemoveAt(i--);
            }
            srd.CopyDataFiles.Add(new ScriptCopyDataFile(from, to));
        }
        public void CopyDataFolder(string from, string to, bool recurse) {
            CheckDataFolderSafty(from);
            CheckFolderSafty(to);

            if(testMode) {
                from=from.ToLower();
                to=to.ToLower();
                if(from.EndsWith("\\")||from.EndsWith("/")) from=from.Remove(from.Length-1);
                if(to.EndsWith("\\")||to.EndsWith("/")) to=to.Remove(to.Length-1);
                foreach(string s in dataFileList) {
                    if(s.StartsWith(from)&&(recurse||s.IndexOf('\\', from.Length+1)==-1)) {
                        string lto=Path.Combine(to, s.Substring(s.IndexOf('\\', from.Length-1)+1));
                        for(int i=0;i<srd.CopyDataFiles.Count;i++) {
                            if(srd.CopyDataFiles[i].CopyTo==lto) srd.CopyDataFiles.RemoveAt(i--);
                        }
                        srd.CopyDataFiles.Add(new ScriptCopyDataFile(s, lto));
                    }
                }
            } else {
                permissions.Assert();
                from=Path.GetFullPath(Path.Combine(DataFiles, from));
                foreach(string path in Directory.GetFiles(from, "*", recurse?System.IO.SearchOption.AllDirectories:System.IO.SearchOption.TopDirectoryOnly)) {
                    string filefrom=Path.GetFullPath(path).Substring(DataFiles.Length);
                    string fileto=Path.GetFullPath(path).Substring(from.Length);
                    if(fileto.StartsWith(""+Path.DirectorySeparatorChar)||fileto.StartsWith(""+Path.AltDirectorySeparatorChar)) fileto=fileto.Substring(1);
                    fileto=Path.Combine(to, fileto);
                    string lto=fileto.ToLower();
                    for(int i=0;i<srd.CopyDataFiles.Count;i++) {
                        if(srd.CopyDataFiles[i].CopyTo==lto) srd.CopyDataFiles.RemoveAt(i--);
                    }
                    srd.CopyDataFiles.Add(new ScriptCopyDataFile(filefrom, fileto));
                }
            }
        }

        public void PatchPlugin(string from, string to, bool create) {
            CheckPluginSafty(from);
            CheckPathSafty(to);
            string lto=to.ToLower();
            if(!lto.EndsWith(".esp")&&!lto.EndsWith(".esm")) throw new ScriptingException("Copied plugins must have a .esp or .esm file extension");
            if(to.Contains("\\")||to.Contains("/")) throw new ScriptingException("Cannot copy a plugin to a subdirectory of the data folder");
            to="Data\\"+to;

            permissions.Assert();

            if(testMode) {
                if(File.Exists(to)&&!Program.Data.DoesEspExist(to))
                    throw new ScriptingException("Cannot patch file '"+to+"' because it already exists but is not parented to an omod");
                return;
            }

            DateTime timestamp=File.GetLastWriteTime(Plugins+from);
            if(File.Exists(to)) {
                if(!Program.Data.DoesEspExist(to)) {
                    throw new ScriptingException("Cannot patch file '"+to+"' because it already exists but is not parented to an omod");
                }
                timestamp=File.GetLastWriteTime(to);
                File.Delete(to);
            } else if(!create) return;
            File.Move(Plugins+from, to);
            File.SetLastWriteTime(to, timestamp);
        }
        public void PatchDataFile(string from, string to, bool create) {
            CheckDataSafty(from);
            CheckPathSafty(to);
            string lto=to.ToLower();
            if(lto.EndsWith(".esp")||lto.EndsWith(".esm")) throw new ScriptingException("Copied data files must not have a .esp or .esm file extension");
            to="Data\\"+to;

            permissions.Assert();

            if(testMode) {
                if(File.Exists(to)&&!Program.Data.DoesDataFileExist(to))
                    throw new ScriptingException("Cannot patch file '"+to+"' because it already exists but is not parented to an omod");
                return;
            }

            DateTime timestamp=File.GetLastWriteTime(DataFiles+from);
            if(File.Exists(to)) {
                if(!Program.Data.DoesDataFileExist(to)) {
                    throw new ScriptingException("Cannot patch file '"+to+"' because it already exists but is not parented to an omod");
                }
                timestamp=File.GetLastWriteTime(to);
                File.Delete(to);
            } else if(!create) return;
            File.Move(DataFiles+from, to);
            File.SetLastWriteTime(to, timestamp);
        }

        public void RegisterBSA(string path) {
            CheckDataSafty(path);
            if(path.Contains(";")||path.Contains(",")||path.Contains("=")) throw new ScriptingException("BSA path cannot contain the characters ',', '=' or ';'");
            if(!srd.RegisterBSAList.Contains(path)) srd.RegisterBSAList.Add(path);
        }
        public void UnregisterBSA(string path) {
            CheckDataSafty(path);
            if(path.Contains(";")||path.Contains(",")||path.Contains("=")) throw new ScriptingException("BSA path cannot contain the characters ',', '=' or ';'");
            if(srd.RegisterBSAList.Contains(path)) srd.RegisterBSAList.Remove(path);
        }

        public void EditINI(string section, string key, string value) {
            srd.INIEdits.Add(new INIEditInfo(section, key, value));
        }
        public void EditShader(byte package, string name, string path) {
            CheckDataSafty(path);
            srd.SDPEdits.Add(new SDPEditInfo(package, name, DataFiles+path));
        }

        public void FatalError() { srd.CancelInstall=true; }

        public void SetGMST(string file, string edid, string value) {
            CheckPluginSafty(file);
            srd.EspEdits.Add(new ScriptEspEdit(false, file.ToLower(), edid, value));
        }
        public void SetGlobal(string file, string edid, string value) {
            CheckPluginSafty(file);
            srd.EspEdits.Add(new ScriptEspEdit(false, file.ToLower(), edid, value));
        }

        public void SetPluginByte(string file, long offset, byte value) {
            CheckPluginSafty(file);
            if(testMode) return;
            permissions.Assert();
            System.IO.FileStream fs=File.OpenWrite(Plugins+file);
            fs.Position=offset;
            fs.WriteByte(value);
            fs.Close();
        }
        public void SetPluginShort(string file, long offset, short value) {
            CheckPluginSafty(file);
            if(testMode) return;
            permissions.Assert();
            byte[] data=BitConverter.GetBytes(value);
            System.IO.FileStream fs=File.OpenWrite(Plugins+file);
            fs.Position=offset;
            fs.Write(data, 0, 2);
            fs.Close();
        }
        public void SetPluginInt(string file, long offset, int value) {
            CheckPluginSafty(file);
            if(testMode) return;
            permissions.Assert();
            byte[] data=BitConverter.GetBytes(value);
            System.IO.FileStream fs=File.OpenWrite(Plugins+file);
            fs.Position=offset;
            fs.Write(data, 0, 4);
            fs.Close();
        }
        public void SetPluginLong(string file, long offset, long value) {
            CheckPluginSafty(file);
            if(testMode) return;
            permissions.Assert();
            byte[] data=BitConverter.GetBytes(value);
            System.IO.FileStream fs=File.OpenWrite(Plugins+file);
            fs.Position=offset;
            fs.Write(data, 0, 8);
            fs.Close();
        }
        public void SetPluginFloat(string file, long offset, float value) {
            CheckPluginSafty(file);
            if(testMode) return;
            permissions.Assert();
            byte[] data=BitConverter.GetBytes(value);
            System.IO.FileStream fs=File.OpenWrite(Plugins+file);
            fs.Position=offset;
            fs.Write(data, 0, 4);
            fs.Close();
        }

        public string InputString() { return InputString("", ""); }
        public string InputString(string title) { return InputString(title, ""); }
        public string InputString(string title, string initial) {
            permissions.Assert();
            TextEditor te=new TextEditor(title, initial, false, true);
            te.ShowDialog();
            if(te.DialogResult!=DialogResult.Yes) return "";
            else return te.Result;
        }

        public string ReadINI(string section, string value) {
            permissions.Assert();
            return OblivionINI.GetINIValue(section, value);
        }
        public string ReadRendererInfo(string value) {
            permissions.Assert();
            return OblivionRenderInfo.GetInfo(value);
        }

        public void EditXMLLine(string file, int line, string value) {
            CheckDataSafty(file);
            string ext=Path.GetExtension(file).ToLower();
            if(ext!=".txt"&&ext!=".xml"&&ext!=".bat"&&ext!=".ini") throw new ScriptingException("Can only edit files with a .xml, .ini, .bat or .txt extension");
            permissions.Assert();
            string[] lines=File.ReadAllLines(DataFiles+file);
            if(line<0||line>=lines.Length) throw new ScriptingException("Invalid line number");
            lines[line]=value;
            File.WriteAllLines(DataFiles+file, lines);
        }
        public void EditXMLReplace(string file, string find, string replace) {
            CheckDataSafty(file);
            string ext=Path.GetExtension(file).ToLower();
            if(ext!=".txt"&&ext!=".xml"&&ext!=".bat"&&ext!=".ini") throw new ScriptingException("Can only edit files with a .xml, .ini, .bat or .txt extension");
            permissions.Assert();
            string text=File.ReadAllText(DataFiles+file);
            text=text.Replace(find, replace);
            File.WriteAllText(DataFiles+file, text);
        }

        public System.Windows.Forms.Form CreateCustomDialog() {
            permissions.Assert();
            return new System.Windows.Forms.Form();
        }

        public byte[] ReadDataFile(string file) {
            if(testMode) throw new ScriptingException("ReadDataFile cannot be used in a script simulation");
            CheckDataSafty(file);
            permissions.Assert();
            return File.ReadAllBytes(Path.Combine(DataFiles, file));
        }
        public byte[] ReadExistingDataFile(string file) {
            CheckPathSafty(file);
            permissions.Assert();
            return File.ReadAllBytes("data\\"+file);
        }
        public byte[] GetDataFileFromBSA(string file) {
            CheckPathSafty(file);
            permissions.Assert();
            return Classes.BSAArchive.GetFileFromBSA(file);
        }
        public byte[] GetDataFileFromBSA(string bsa, string file) {
            CheckPathSafty(file);
            permissions.Assert();
            return Classes.BSAArchive.GetFileFromBSA(bsa, file);
        }

        public void GenerateNewDataFile(string file, byte[] data) {
            if(testMode) throw new ScriptingException("GenerateNewDataFile cannot be used in a script simulation");
            CheckPathSafty(file);
            permissions.Assert();
            string file2=Path.Combine(DataFiles, file);
            if(!File.Exists(file2)) {
                string lto=file.ToLower();
                if(lto.EndsWith(".esm")||lto.EndsWith(".esp")) throw new ScriptingException("Copied data files cannot have a .esp or .esm file extension");
                for(int i=0;i<srd.CopyDataFiles.Count;i++) {
                    if(srd.CopyDataFiles[i].CopyTo==lto) srd.CopyDataFiles.RemoveAt(i--);
                }
                srd.CopyDataFiles.Add(new ScriptCopyDataFile(file, file));
            }
            if(!Directory.Exists(Path.GetDirectoryName(file2))) Directory.CreateDirectory(Path.GetDirectoryName(file2));
            File.WriteAllBytes(file2, data);
        }
        public void CancelDataFileCopy(string file) {
            if(testMode) throw new ScriptingException("CancelDataFileCopy cannot be used in a script simulation");
            CheckPathSafty(file);
            permissions.Assert();
            string file2=Path.Combine(DataFiles, file);
            string lto=file.ToLower();
            for(int i=0;i<srd.CopyDataFiles.Count;i++) {
                if(srd.CopyDataFiles[i].CopyTo==lto) srd.CopyDataFiles.RemoveAt(i--);
            }
            File.Delete(file2);
        }
        public void CancelDataFolderCopy(string folder) {
            if(testMode) throw new ScriptingException("CancelDataFolderCopy cannot be used in a script simulation");
            CheckPathSafty(folder);
            permissions.Assert();
            string lto=folder.ToLower();
            for(int i=0;i<srd.CopyDataFiles.Count;i++) {
                if(srd.CopyDataFiles[i].CopyTo.StartsWith(lto)) {
                    File.Delete(Path.Combine(DataFiles, srd.CopyDataFiles[i].CopyTo));
                    srd.CopyDataFiles.RemoveAt(i--);
                }
            }
        }
        public void GenerateBSA(string file, string path, string prefix, int cRatio, int cLevel) {
            if(testMode) throw new ScriptingException("GenerateBSA cannot be used in a script simulation");
            CheckPathSafty(file);
            CheckDataFolderSafty(path);
            permissions.Assert();
            path=Path.Combine(DataFiles, path);
            string file2=Path.Combine(DataFiles, file);
            if(!File.Exists(file2)) {
                string lto=file.ToLower();
                if(lto.EndsWith(".esm")||lto.EndsWith(".esp")) throw new ScriptingException("Copied data files cannot have a .esp or .esm file extension");
                for(int i=0;i<srd.CopyDataFiles.Count;i++) {
                    if(srd.CopyDataFiles[i].CopyTo==lto) srd.CopyDataFiles.RemoveAt(i--);
                }
                srd.CopyDataFiles.Add(new ScriptCopyDataFile(file, file));
            }
            Forms.BSACreator.CreateBSA(file2, path, prefix, cRatio, cLevel, false);
        }

        public bool IsSimulation() { return testMode; }
    }

}