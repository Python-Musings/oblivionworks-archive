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

using System.Collections.Generic;
using Serializable=System.SerializableAttribute;
using Path=System.IO.Path;
using File=System.IO.File;
using Directory=System.IO.Directory;

namespace OblivionModManager {
    internal class omodGroup {
        private string name;
        private System.Drawing.Font font;
        private System.Drawing.Color color;

        internal string Name { get { return name; } }
        internal System.Drawing.Font Font { get { return font; } }
        internal System.Drawing.Color Color { get { return color; } }

        internal omodGroup(string Name) {
            name=Name;
            font=null;
            color=System.Drawing.Color.Black;
        }

        internal void SetFont(System.Drawing.Font Font, System.Drawing.Color Color) {
            font=Font;
            color=Color;
        }

        internal void Rename(string Name) {
            name=Name;
        }

        public override string ToString() {
            return Name;
        }

        internal void Write(System.IO.BinaryWriter bw, System.Runtime.Serialization.Formatters.Binary.BinaryFormatter formatter) {
            bw.Write(name);
            if(Font!=null) {
                bw.Write(true);
                System.IO.MemoryStream ms=new System.IO.MemoryStream(512);
                formatter.Serialize(ms, font);
                bw.Write((int)ms.Length);
                bw.Write(ms.ToArray());
                bw.Write(color.ToArgb());
            } else {
                bw.Write(false);
            }
        }

        internal static omodGroup Read(System.IO.BinaryReader br, System.Runtime.Serialization.Formatters.Binary.BinaryFormatter formatter) {
            omodGroup og=new omodGroup(br.ReadString());
            if(br.ReadBoolean()) {
                System.IO.MemoryStream ms=new System.IO.MemoryStream(br.ReadBytes(br.ReadInt32()));
                og.SetFont((System.Drawing.Font)formatter.Deserialize(ms),System.Drawing.Color.FromArgb(br.ReadInt32()));
            }
            return og;
        }
    }

    [System.Flags]
    internal enum ArchiveInvalidationFlags : uint {
        Textures  = 0x1,
        Meshes    = 0x2,
        Sounds    = 0x4,
        Music     = 0x8,
        TreesLOD  = 0x10,
        Fonts     = 0x20,
        Menus     = 0x40,
        Misc      = 0x80,
        Base      = 0x100,
        IgnoreNormal    = 0x1000,
        BSAOnly         = 0x2000,
        MatchExtensions = 0x4000,
        Universal       = 0x8000,
        EditBSAs        = 0x10000,
        EditAllEntries  = 0x20000,
        HashGenAI       = 0x40000,
        HashWarn        = 0x80000,
        BSARedirection  = 0x100000,
        PackFaceTextures= 0x200000,

        Default = ArchiveInvalidationFlags.BSAOnly|ArchiveInvalidationFlags.MatchExtensions|
                  ArchiveInvalidationFlags.Textures|ArchiveInvalidationFlags.BSARedirection|
                  ArchiveInvalidationFlags.HashGenAI
    }

    internal struct ScriptLabel {
        internal string Label;
        internal int LineNo;

        internal ScriptLabel(string label, int lineNo) {
            Label=label;
            LineNo=lineNo;
        }
    }

    internal struct PluginLoadInfo {
        internal string Plugin;
        internal string Target;
        internal bool LoadAfter;

        internal PluginLoadInfo(string plugin, string target, bool loadAfter) {
            Plugin=plugin;
            Target=target;
            LoadAfter=loadAfter;
        }
    }

    internal struct ScriptEspEdit {
        internal readonly bool IsGMST;
        internal readonly string Plugin;
        internal readonly string EDID;
        internal readonly string Value;

        internal ScriptEspEdit(bool gmst, string plugin, string edid, string value) {
            IsGMST=gmst;
            Plugin=plugin;
            EDID=edid;
            Value=value;
        }
    }

    internal class ScriptReturnData {
        internal readonly List<string> IgnorePlugins=new List<string>();
        internal readonly List<string> InstallPlugins=new List<string>();
        internal bool InstallAllPlugins=true;
        internal readonly List<string> IgnoreData=new List<string>();
        internal readonly List<string> InstallData=new List<string>();
        internal bool InstallAllData=true;
        internal readonly List<PluginLoadInfo> LoadOrderList=new List<PluginLoadInfo>();
        internal readonly List<ConflictData> ConflictsWith=new List<ConflictData>();
        internal readonly List<ConflictData> DependsOn=new List<ConflictData>();
        internal readonly List<string> RegisterBSAList=new List<string>();
        internal bool CancelInstall=false;
        internal readonly List<string> UncheckedPlugins=new List<string>();
        internal readonly List<ScriptEspWarnAgainst> EspDeactivation=new List<ScriptEspWarnAgainst>();
        internal readonly List<ScriptCopyDataFile> CopyDataFiles=new List<ScriptCopyDataFile>();
        internal readonly List<ScriptCopyDataFile> CopyPlugins=new List<ScriptCopyDataFile>();
        internal readonly List<INIEditInfo> INIEdits=new List<INIEditInfo>();
        internal readonly List<SDPEditInfo> SDPEdits=new List<SDPEditInfo>();
        internal readonly List<ScriptEspEdit> EspEdits=new List<ScriptEspEdit>();
        internal readonly List<string> EarlyPlugins=new List<string>();
    }

    internal class ScriptExecutationData {
        internal PluginLoadInfo[] PluginOrder;
        internal string[] UncheckedPlugins;
        internal ScriptEspWarnAgainst[] EspDeactivationWarning;
        internal ScriptEspEdit[] EspEdits;
        internal string[] EarlyPlugins;
    }

    [Serializable]
    internal struct ConflictData {
        internal ConflictLevel level;
        internal string File;
        internal int MinMajorVersion;
        internal int MinMinorVersion;
        internal int MaxMajorVersion;
        internal int MaxMinorVersion;
        internal string Comment;
        internal bool Partial;

        public static bool operator==(ConflictData cd, omod o) {
            if(!cd.Partial&&cd.File!=o.ModName) return false;
            if(cd.Partial) {
                System.Text.RegularExpressions.Regex reg=new System.Text.RegularExpressions.Regex(cd.File, System.Text.RegularExpressions.RegexOptions.IgnoreCase);
                if(!reg.IsMatch(o.ModName)) return false;
            }
            if(cd.MaxMajorVersion!=0||cd.MaxMinorVersion!=0) {
                if(cd.MaxMajorVersion>o.MajorVersion) return false;
                if(cd.MaxMajorVersion==o.MajorVersion&&cd.MaxMinorVersion>o.MinorVersion) return false;
            }
            if(cd.MinMajorVersion!=0||cd.MinMinorVersion!=0) {
                if(cd.MinMajorVersion<o.MajorVersion) return false;
                if(cd.MinMajorVersion==o.MajorVersion&&cd.MinMinorVersion<o.MinorVersion) return false;
            }
            return true;
        }
        public static bool operator!=(ConflictData cd, omod o) {
            return !(cd==o);
        }
        public override int GetHashCode() {
            return base.GetHashCode();
        }
        public override bool Equals(object obj) {
            if(obj is ConflictData) {
                ConflictData cd=(ConflictData)obj;
                if(File==cd.File&&MinMajorVersion==cd.MinMajorVersion&&MinMinorVersion==cd.MinMinorVersion&&
                    MaxMajorVersion==cd.MaxMajorVersion&&MaxMinorVersion==cd.MaxMinorVersion&&Comment==cd.Comment) {
                    return true;
                }
            } else if(obj is omod) {
                return (this==(omod)obj);
            }
            return false;
        }
    }

    internal struct ScriptEspWarnAgainst {
        internal string Plugin;
        internal DeactiveStatus Status;

        internal ScriptEspWarnAgainst(string plugin, DeactiveStatus status) {
            Plugin=plugin;
            Status=status;
        }
    }

    internal struct ScriptCopyDataFile {
        internal readonly string CopyFrom;
        internal readonly string CopyTo;
        internal readonly string hCopyFrom;
        internal readonly string hCopyTo;
        internal ScriptCopyDataFile(string from, string to) {
            CopyFrom=from.ToLower();
            CopyTo=to.ToLower();
            hCopyFrom=from;
            hCopyTo=to;
        }
    }

    internal struct omodCreationOptions {
        internal string Name;
        internal string Author;
        internal string email;
        internal string website;
        internal string Description;
        internal string Image;
        internal int MajorVersion;
        internal int MinorVersion;
        internal int BuildVersion;
        internal CompressionType CompressionType;
        internal CompressionLevel DataFileCompresionLevel;
        internal CompressionLevel omodCompressionLevel;
        internal string[] esps;
        internal string[] espPaths;
        internal string[] DataFiles;
        internal string[] DataFilePaths;
        internal string readme;
        internal string changelog;
        internal string comments;
        internal string credits;
        internal string script;
    }

    [Serializable]
    internal class BSA {
        internal readonly List<string> UsedBy=new List<string>();
        internal readonly string FileName;
        internal readonly string LowerFileName;

        internal BSA(string fileName,string modname) {
            FileName=fileName;
            LowerFileName=FileName.ToLower();
            UsedBy.Add(modname);
        }
    }

    [Serializable]
    internal class EspInfo {
        internal const string UnknownOwner="Unknown";
        internal const string BaseOwner="Base";
        internal static string[] BaseFiles= { "oblivion.esm" };

        internal readonly string FileName;
        internal readonly string LowerFileName;
        internal string BelongsTo;
        internal readonly List<string> MustLoadAfter=new List<string>();
        internal readonly List<string> MustLoadBefore=new List<string>();
        internal bool Active;
        internal System.DateTime DateModified;
        internal DeactiveStatus Deactivatable;
        internal omod Parent;

        internal ConflictDetector.HeaderInfo header;

        internal void GetHeader() {
            if(!System.IO.File.Exists("Data\\"+FileName)) return;
            header=ConflictDetector.TesFile.GetHeader("Data\\"+FileName);
        }

        internal void Unlink() {
            if(Parent==null) return;
            Parent.UnlinkPlugin(this);
            Parent=null;
            Deactivatable=DeactiveStatus.Allow;
            BelongsTo=UnknownOwner;
        }

        internal EspInfo(string fileName,omod parent) {
            FileName=fileName;
            LowerFileName=FileName.ToLower();
            BelongsTo=parent.FileName.Substring(0,parent.FileName.Length-5);
            Parent=parent;
            GetHeader();
            Deactivatable=Settings.DefaultEspWarn;
        }
        internal EspInfo(string fileName) {
            FileName=fileName;
            LowerFileName=FileName.ToLower();
            if(System.Array.IndexOf<string>(BaseFiles, LowerFileName)!=-1) {
                BelongsTo=BaseOwner;
            } else {
                BelongsTo=UnknownOwner;
            }
            Parent=null;
            Deactivatable=DeactiveStatus.Allow;
            GetHeader();
        }

#if DEBUG
        public override string ToString() {
            return FileName;
        }
#endif
    }

    [Serializable]
    internal class DataFileInfo {
        internal readonly string FileName;
        internal readonly string LowerFileName;
        internal uint CRC;
        private readonly List<string> UsedBy=new List<string>();

        public static bool operator==(DataFileInfo a, DataFileInfo b) {
            if(null==(object)a||null==(object)b) {
                if(null!=(object)a||null!=(object)b) return false; else return true;
            }
            return (a.LowerFileName==b.LowerFileName);
        }
        public static bool operator!=(DataFileInfo a, DataFileInfo b) {
            return !(a==b);
        }
        public override bool Equals(object obj) {
            if(!(obj is DataFileInfo)) return false;
            return this==(DataFileInfo)obj;
        }
        public override int GetHashCode() { return LowerFileName.GetHashCode(); }

        //I dont really want this to be here, but .NET does some stupid implicit convertion by calling ToString() it isn't
        /*internal static implicit operator string(DataFileInfo dfi) {
            return dfi.FileName;
        }*/

        public override string ToString() {
            return FileName;
        }

        internal DataFileInfo(string s,uint crc) {
            FileName=s;
            LowerFileName=FileName.ToLower();
            CRC=crc;
        }
        internal DataFileInfo(DataFileInfo orig) {
            FileName=orig.FileName;
            LowerFileName=orig.LowerFileName;
            CRC=orig.CRC;
        }

        internal void AddOwner(omod o) { if(!UsedBy.Contains(o.LowerFileName)) UsedBy.Add(o.LowerFileName); }
        internal void RemoveOwner(omod o) { 
            UsedBy.Remove(o.LowerFileName);
            if(UsedBy.Count==0) {
                Program.Data.DataFiles.Remove(this);
                if(File.Exists(Path.Combine("data", FileName))) File.Delete(Path.Combine("data", FileName));
            }
        }
        internal string Owners { get { return string.Join(", ", UsedBy.ToArray()); } }
    }

    [Serializable]
    internal class INIEditInfo {
        internal readonly string Section;
        internal readonly string Name;
        internal string OldValue;
        internal readonly string NewValue;
        internal omod Plugin;

        internal INIEditInfo(string section, string name, string newvalue) {
            Section=section.ToLower();
            Name=name.ToLower();
            NewValue=newvalue;
        }

        public static bool operator==(INIEditInfo a, INIEditInfo b) { return (a.Section==b.Section) && (a.Name==b.Name); }
        public static bool operator!=(INIEditInfo a, INIEditInfo b) { return (a.Section!=b.Section) || (a.Name!=b.Name); }
        public override bool Equals(object obj) { return this==(obj as INIEditInfo); }
        public override int GetHashCode() { return Section.GetHashCode() + Name.GetHashCode(); }
    }

    [Serializable]
    internal class SDPEditInfo {
        internal readonly byte Package;
        internal readonly string Shader;
        internal string BinaryObject;

        internal SDPEditInfo(byte package, string shader, string binaryObject) {
            Package=package;
            Shader=shader.ToLower();
            BinaryObject=binaryObject.ToLower();
        }
    }

    internal class obmmException : System.ApplicationException {
        internal obmmException(string message) : base(message) { }
    }

    internal enum CompressionType : byte { SevenZip, Zip }
    internal enum CompressionLevel : byte { VeryHigh, High, Medium, Low, VeryLow, None }
    public enum ConflictLevel { Active, NoConflict, MinorConflict, MajorConflict, Unusable }
    public enum DeactiveStatus { Allow, WarnAgainst, Disallow }
    internal enum EspSortOrder { LoadOrder, Alpha, Active, Owner, FileSize, DateCreated }
    internal enum omodSortOrder { Name, Author, ConflictLevel, FileSize, DateCreated, DateInstalled, Group }
    internal enum ScriptType { obmmScript, Python, cSharp, vb, Count }

    internal class EspListSorter : System.Collections.IComparer {
        internal static EspSortOrder order=EspSortOrder.LoadOrder;
        public int Compare(object a, object b) {
            System.Windows.Forms.ListViewItem la=(System.Windows.Forms.ListViewItem)a;
            System.Windows.Forms.ListViewItem lb=(System.Windows.Forms.ListViewItem)b;
            EspInfo ea = (EspInfo)la.Tag;
            EspInfo eb = (EspInfo)lb.Tag;
            switch(order) {
            case EspSortOrder.LoadOrder:
                if(Path.GetExtension(ea.LowerFileName)!=Path.GetExtension(eb.LowerFileName)) {
                    if(Path.GetExtension(ea.LowerFileName)==".esm") {
                        return -1;
                    } else {
                        return 1;
                    }
                }
                if(ea.DateModified==eb.DateModified) return 0;
                if(ea.DateModified>eb.DateModified) return 1; else return -1;
            case EspSortOrder.Alpha:
                return string.Compare(ea.FileName, eb.FileName);  
            case EspSortOrder.Active:
                if(la.Checked==lb.Checked) return 0;
                if(la.Checked) return -1; else return 1;
            case EspSortOrder.Owner:
                return string.Compare(ea.BelongsTo, eb.BelongsTo);
            case EspSortOrder.FileSize:
                long sizea=(new System.IO.FileInfo("data\\"+ea.FileName)).Length;
                long sizeb=(new System.IO.FileInfo("data\\"+eb.FileName)).Length;
                if(sizea==sizeb) return 0;
                if(sizea>sizeb) return -1; else return 1;
            case EspSortOrder.DateCreated:
                System.DateTime da=(new System.IO.FileInfo("data\\"+ea.FileName)).CreationTime;
                System.DateTime db=(new System.IO.FileInfo("data\\"+eb.FileName)).CreationTime;
                return System.DateTime.Compare(da, db);
            default: return 0;
            }
        }
    }
    internal class omodListSorter : System.Collections.IComparer {
        internal static omodSortOrder order=omodSortOrder.Name;
        public int Compare(object a, object b) {
            System.Windows.Forms.ListViewItem la=(System.Windows.Forms.ListViewItem)a;
            System.Windows.Forms.ListViewItem lb=(System.Windows.Forms.ListViewItem)b;
            omod oa = (omod)la.Tag;
            omod ob = (omod)lb.Tag;
            System.DateTime da;
            System.DateTime db;
            switch(order) {
            case omodSortOrder.Name:
                return string.Compare(oa.FileName, ob.FileName);
            case omodSortOrder.Author:
                return string.Compare(oa.Author, ob.Author);
            case omodSortOrder.ConflictLevel:
                if((int)oa.Conflict==(int)ob.Conflict) return 0;
                if((int)oa.Conflict>(int)ob.Conflict) return 1; else return 0;
            case omodSortOrder.FileSize:
                long sizea=(new System.IO.FileInfo(Settings.omodDir+oa.FileName)).Length;
                long sizeb=(new System.IO.FileInfo(Settings.omodDir+ob.FileName)).Length;
                if(sizea==sizeb) return 0;
                if(sizea>sizeb) return -1; else return 1;
            case omodSortOrder.DateCreated:
                //This is deliberately backward
                return System.DateTime.Compare(ob.CreationTime, oa.CreationTime);
            case omodSortOrder.DateInstalled:
                //As is this
                da=(new System.IO.FileInfo(Settings.omodDir+oa.FileName)).CreationTime;
                db=(new System.IO.FileInfo(Settings.omodDir+ob.FileName)).CreationTime;
                return System.DateTime.Compare(db, da);
            case omodSortOrder.Group:
                if(oa.group==ob.group) return 0;
                if(oa.group>ob.group) return -1; else return 1;
            default: return 0;
            }
        }
    }
    
}