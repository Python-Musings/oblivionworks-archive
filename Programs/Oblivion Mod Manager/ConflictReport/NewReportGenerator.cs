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
using System.Windows.Forms;
using System.IO;
using Color=System.Drawing.Color;

namespace OblivionModManager.ConflictReport {
    internal partial class NewReportGenerator : Form {
        private class Node : IComparable<Node> {
            internal readonly string name;
            internal readonly Plugin plugin;
            internal readonly string rectype;
            //internal readonly long offset;
            //internal readonly bool compressed;
            internal readonly uint formID;
            //internal readonly Node parent;
            internal readonly List<Node> children;
            internal string description;
            internal byte conflict;

            internal string sFormID { get { return formID.ToString("X").PadLeft(8, '0'); } }
            internal string Name { get { return name==null?sFormID:name; } }
            internal string Description {
                get {
                    if(rectype.Length!=4) {
                        return description;
                    } else if(rectype=="GRUP") {
                        string s;
                        switch(formID) {
                        case 0:
                            s="Group type: Top"+Environment.NewLine+
                                "Contents: "+Base.RecTypeLookup[name.Substring(name.Length-5,4)];
                            break;
                        case 1:
                            s="Group type: World Children";
                            break;
                        case 2:
                            s="Group type: Interior Cell Block";
                            break;
                        case 3:
                            s="Group type: Interior Cell Sub-Block";
                            break;
                        case 4:
                            s="Group type: Exterior Cell Block";
                            break;
                        case 5:
                            s="Group type: Exterior Cell Sub-Block";
                            break;
                        case 6:
                            s="Group type: Cell Children";
                            break;
                        case 7:
                            s="Group type: Topic Children";
                            break;
                        case 8:
                            s="Group type: Cell Persistent Childen";
                            break;
                        case 9:
                            s="Group type: Cell Temporary Children";
                            break;
                        case 10:
                            s="Group type: Cell Visible Distant Children";
                            break;
                        default:
                            s="Warning: Invalid group ID";
                            break;
                        }
                        return s+Environment.NewLine+description;
                    } else {
                        string s="Record type: "+Base.RecTypeLookup[rectype]+" ("+rectype+")"+Environment.NewLine;
                        if(name!=null) s+="EDID: "+name+Environment.NewLine;
                        if(rectype!="GMST") s+="FormID: "+sFormID+Environment.NewLine;
                        return s+Environment.NewLine+description;
                    }
                }
            }

            public int CompareTo(Node b) {
                if(formID!=b.formID) return formID.CompareTo(b.formID);
                else return plugin.FormID.CompareTo(b.plugin.FormID);
            }

            internal Node(string Name, string Rectype, Plugin Plugin, long Offset, bool Compressed, uint FormID, Node Parent) {
                name=Name;
                plugin=Plugin;
                rectype=Rectype;
                //offset=Offset;
                //compressed=Compressed;
                formID=FormID;
                //parent=Parent;
                if(Rectype=="GRUP"||Rectype.Length!=4) children=new List<Node>(); else children=null;
                if(Rectype.Length==4) {
                    description=null;
                    conflict=0;
                }
            }
        }

        private class Plugin {
            internal readonly Node[] IDs;
            internal readonly string path;
            internal readonly string name;
            internal readonly bool IsMaster;
            internal readonly Node baseNode;
            internal readonly byte FormID;

            internal TreeNode[] Conflicts;
            internal TreeNode[] FullStructure;
            internal TreeNode MainNode;

            internal Plugin(string Path, Node BaseNode, byte formID, bool Walk) {
                path=Path;
                name=System.IO.Path.GetFileName(path);
                if(name.ToLower().EndsWith(".esm")) IsMaster=true; else IsMaster=false;
                FormID=formID;
                baseNode=BaseNode;
                if(Walk) IDs=GenIDList();
            }

            private Dictionary<byte, byte> CreateFormIDTranslationDictanory(BinaryReader br, uint size) {
                byte count=0;
                uint read=0;
                Dictionary<byte, byte> temp=new Dictionary<byte, byte>();
                int i;
                while(read<size) {
                    string s=Program.ReadBString(br, 4);
                    ushort size2=br.ReadUInt16();
                    if(s=="MAST") {
                        string master=Program.ReadCString(br).ToLower();
                        i=Array.IndexOf<string>(Base.lActive, master);
                        if(i==-1) throw new obmmException("The master file '"+master+"' required by plugin '"+name+"' is not active");
                        temp.Add(count++, (byte)i);
                    } else br.BaseStream.Position+=size2;
                    read+=(uint)(size2+6);
                }
                temp.Add(count, FormID);
                return temp;
            }

            private void ReadGroup(BinaryReader br, long Size, Dictionary<byte, byte> FormIDLookup, Node Parent, List<Node> EDIDs) {
                long read=0;
                while(read<Size) {
                    string s=Program.ReadBString(br, 4);
                    uint size=br.ReadUInt32();
                    if(s=="GRUP") {
                        string name = "Record Group";
                        string gType=Program.ReadBString(br, 4);
                        uint type=br.ReadUInt32();
                        if(type==0) name+=" ("+gType+")";
                        Node group=new Node(name,"GRUP",this,0,false,type,Parent);
                        Parent.children.Add(group);

                        br.BaseStream.Position+=4;
                        ReadGroup(br, size-20, FormIDLookup, group, EDIDs);
                        read+=size;
                        continue;
                    }
                    uint comp=br.ReadUInt32();
                    uint formID;
                    if(s=="GMST") {
                        br.BaseStream.Position+=8;
                        formID=uint.MaxValue;
                    } else {
                        formID=br.ReadUInt32();
                        byte pluginID=(byte)((formID&0xFF000000)>>24);
                        if(!FormIDLookup.ContainsKey(pluginID)) formID+=0xFF000000;
                        else formID+=(uint)(FormIDLookup[pluginID]<<24);
                        br.BaseStream.Position+=4;
                    }

                    long offset=br.BaseStream.Position;
                    string edid;
                    if(Base.cbEDIDs.Checked||s=="GMST") {
                        if((comp&0x00040000)>0) {
                            ICSharpCode.SharpZipLib.Zip.Compression.Inflater inf;
                            inf=new ICSharpCode.SharpZipLib.Zip.Compression.Inflater();
                            byte[] In=new byte[size-4];
                            byte[] Out=new byte[br.ReadInt32()];
                            br.Read(In, 0, (int)(size-4));
                            inf.SetInput(In);
                            inf.Inflate(Out);
                            if((byte)Out[0]=='E'&&(byte)Out[1]=='D'&&(byte)Out[2]=='I'&&(byte)Out[3]=='D') {
                                edid="";
                                int i=6;
                                while(Out[i]!=0) edid+=(char)Out[i++];
                            } else {
                                edid=null;
                            }
                        } else {
                            string s2=Program.ReadBString(br, 4);
                            if(s2=="EDID") {
                                edid="";
                                byte b;
                                ushort size2=br.ReadUInt16();
                                while((b=br.ReadByte())!=0) edid+=(char)b;
                                br.BaseStream.Position+=size-(6+size2);
                            } else {
                                br.BaseStream.Position+=(long)size-4;
                                edid=null;
                            }
                        }
                    } else {
                        edid=null;
                        br.BaseStream.Position+=size;
                    }
                    read+=size+20;
                    Node n=new Node(edid, s, this, offset, (comp&0x00040000)>0, formID, Parent);
                    EDIDs.Add(n);
                    Parent.children.Add(n);
                }
            }

            private Node[] GenIDList() {
                List<Node> ids=new List<Node>();
                BinaryReader br=new BinaryReader(File.OpenRead(path));
                try {
                    string s;
                    uint size;
                    s=Program.ReadBString(br,4);
                    if(s!="TES4") return new Node[0];
                    size=br.ReadUInt32();
                    br.BaseStream.Position+=12;
                    Dictionary<byte, byte> FormIDLookup=CreateFormIDTranslationDictanory(br, size);
                    ReadGroup(br, br.BaseStream.Length-br.BaseStream.Position, FormIDLookup, baseNode, ids);
                } finally {
                    br.Close();
                }
                return ids.ToArray();
            }

            internal void GenTree() {
                baseNode.description="Path: "+path+Environment.NewLine+
                    "FormID: "+FormID.ToString("X").PadLeft(2, '0')+Environment.NewLine+
                    "Is master file: "+(IsMaster?"Yes":"No");
                List<TreeNode> conflicts=new List<TreeNode>();
                WalkTree(baseNode, null, conflicts);
                FullStructure=new TreeNode[MainNode.Nodes.Count];
                MainNode.Nodes.CopyTo(FullStructure, 0);
                Conflicts=conflicts.ToArray();
            }

            private byte WalkTree(Node node, TreeNode Parent, List<TreeNode> conflicts) {
                if(!Base.cbNonConflicting.Checked&&node.children==null&&node.conflict==0) return 0;
                TreeNode tn=new TreeNode(node.Name);
                tn.Tag=node;
                if(node.children!=null) {
                    foreach(Node n in node.children) {
                        byte b=WalkTree(n, tn, conflicts);
                        if(b>node.conflict) node.conflict=b;
                    }
                    if(!Base.cbNonConflicting.Checked&&node.conflict==0&&Parent!=null) return 0;
                } else {
                    if(node.conflict>0) conflicts.Add(tn);
                }
                switch(node.conflict) {
                case 1:
                    tn.ForeColor=Color.Green;
                    break;
                case 2:
                    tn.ForeColor=Color.Orange;
                    break;
                case 3:
                    tn.ForeColor=Color.Red;
                    break;
                }
                if(Parent==null) MainNode=tn; else Parent.Nodes.Add(tn);
                return node.conflict;
            }

            internal void CreateFakeIDs(List<Node> edids) {
                uint last=0;
                for(int i=0;i<edids.Count;i++) {
                    if(edids[i].formID>=0x01000000) break;
                    if(edids[i].formID==last) continue;
                    last=edids[i].formID;
                    edids.Insert(i,new Node(edids[i].name, edids[i].rectype, this, 0, false, edids[i].formID, null));
                    i++;
                }
            }
        }

        private static NewReportGenerator Base;
        private string[] Active;
        private string[] lActive;
        private Dictionary<string, string> RecTypeLookup;

        internal NewReportGenerator() { InitializeComponent(); Base=this; }

        private void CreateRecTypeLookup() {
            RecTypeLookup=new Dictionary<string, string>();
            RecTypeLookup["GRUP"]="Record group";
            RecTypeLookup["ACHR"]="Placed NPC";
            RecTypeLookup["ACRE"]="Placed creature";
            RecTypeLookup["ACTI"]="Activator";
            RecTypeLookup["ALCH"]="Potion";
            RecTypeLookup["AMMO"]="Ammo";
            RecTypeLookup["ANIO"]="Animated Object";
            RecTypeLookup["APPA"]="Alchemical Apparatus";
            RecTypeLookup["ARMO"]="Armor";
            RecTypeLookup["BOOK"]="Book";
            RecTypeLookup["BSGN"]="Birthsign";
            RecTypeLookup["CELL"]="Cell";
            RecTypeLookup["CLAS"]="Class";
            RecTypeLookup["CLMT"]="Climate";
            RecTypeLookup["CLOT"]="Clothing";
            RecTypeLookup["CONT"]="Container";
            RecTypeLookup["CREA"]="Creature";
            RecTypeLookup["CSTY"]="Combat Style";
            RecTypeLookup["DIAL"]="Dialog Topic";
            RecTypeLookup["DOOR"]="Door";
            RecTypeLookup["EFSH"]="Effect Shader";
            RecTypeLookup["ENCH"]="Enchantment";
            RecTypeLookup["EYES"]="Eyes";
            RecTypeLookup["FACT"]="Faction";
            RecTypeLookup["FLOR"]="Flora";
            RecTypeLookup["FURN"]="Furniture";
            RecTypeLookup["GLOB"]="Global";
            RecTypeLookup["GMST"]="Game Setting";
            RecTypeLookup["GRAS"]="Grass";
            RecTypeLookup["HAIR"]="Hair";
            RecTypeLookup["IDLE"]="Idle Animations";
            RecTypeLookup["INFO"]="Dialog response";
            RecTypeLookup["INGR"]="Ingredient";
            RecTypeLookup["KEYM"]="Key";
            RecTypeLookup["LAND"]="Land";
            RecTypeLookup["LIGH"]="Light";
            RecTypeLookup["LSCR"]="Load Screen";
            RecTypeLookup["LTEX"]="Land Texture";
            RecTypeLookup["LVLC"]="Leveled Creature";
            RecTypeLookup["LVLI"]="Leveled Item";
            RecTypeLookup["LVSP"]="Leveled Spell";
            RecTypeLookup["MGEF"]="Magic Effect";
            RecTypeLookup["MISC"]="Misc. Item";
            RecTypeLookup["NPC_"]="Non-Player Character";
            RecTypeLookup["PACK"]="AI Package";
            RecTypeLookup["PGRD"]="Path grid";
            RecTypeLookup["QUST"]="Quest";
            RecTypeLookup["RACE"]="Race";
            RecTypeLookup["REFR"]="Placed object";
            RecTypeLookup["REGN"]="Region";
            RecTypeLookup["SBSP"]="Subspace";
            RecTypeLookup["SCPT"]="Script";
            RecTypeLookup["SGST"]="Sigil Stone";
            RecTypeLookup["SKIL"]="Skill";
            RecTypeLookup["SLGM"]="Soul Gem";
            RecTypeLookup["SOUN"]="Sound";
            RecTypeLookup["SPEL"]="Spell";
            RecTypeLookup["STAT"]="Static";
            RecTypeLookup["TES4"]="Tes4 Header";
            RecTypeLookup["TREE"]="Tree";
            RecTypeLookup["WATR"]="Water Type";
            RecTypeLookup["WEAP"]="Weapon";
            RecTypeLookup["WRLD"]="Worldspace";
            RecTypeLookup["WTHR"]="Weather";
            RecTypeLookup["ROAD"]="Unknown";
        }

        private void bActive_Click(object sender, EventArgs e) {
            CreateRecTypeLookup();
            Active=OblivionESP.GetActivePlugins();
            if(Active.Length==0) {
                MessageBox.Show("You have no active mods", "Error");
                return;
            }
            lActive=new string[Active.Length];
            for(int i=0;i<Active.Length;i++) lActive[i]=Active[i].ToLower();
            List<Plugin> plugins=new List<Plugin>();
            for(int i=0;i<Active.Length;i++) {
                try {
                    plugins.Add(new Plugin("data\\"+Active[i], new Node(Active[i], Active[i], null, 0, false, uint.MaxValue, null), (byte)i,
                        (i==0&&!cbShowMain.Checked)?false:true));
                } catch(obmmException ex) {
                    MessageBox.Show("An error occured trying to check '"+Active[i]+"'\n"+ex.Message);
                }
            }
            CheckForConflicts(plugins);
            int i2=0;
            if(!cbShowMain.Checked) i2++;
            for(;i2<plugins.Count;i2++) plugins[i2].GenTree();
            DisplayResults(plugins);
        }

        private void bCancel_Click(object sender, EventArgs e) {
            Close();
        }

        private string StrCat(List<Node> items,int ignore, int start, int count) {
            string s="";
            Node temp=null;
            if(ignore!=-1) {
                temp=items[start+ignore];
                items.RemoveAt(start+ignore);
                count--;
            }
            if(count>2) for(int i=0;i<count-2;i++) s+=items[start+i].plugin.name+", ";
            if(count>1) s+=items[start+count-2].plugin.name+" and ";
            s+=items[start+count-1].plugin.name;
            if(ignore!=-1) {
                items.Insert(start+ignore, temp);
            }
            return s;
        }
        
        private void DetailConflicts(List<Node> edids, int start, int count) {
            edids[start].description+="Overridden in '"+edids[start+count-1].plugin.name+"'";
            edids[start].conflict=1;
            for(int i=1;i<count;i++) {
                edids[start+i].description+="Overrides original entry in '"+edids[start].plugin.name+"'"+Environment.NewLine;
            }
            if(count>2) {
                for(int i=1;i<count;i++) {
                    if(edids[start].rectype=="DIAL"||edids[start].rectype=="WRLD"||edids[start].rectype=="CELL") {
                        edids[start+i].conflict=1;
                        edids[start+i].description+="Overlaps with "+StrCat(edids, i-1, start+1, count-1)+Environment.NewLine;
                    } else {
                        edids[start+i].conflict=3;
                        edids[start+i].description+="Conflicts with "+StrCat(edids, i-1, start+1, count-1)+Environment.NewLine;
                    }
                }
            }
            if(!edids[start].plugin.IsMaster) {
                for(int i=1;i<count;i++) {
                    edids[start+i].description+="Warning: This FormID is specified in multiple esps, but is not defined in any active master files"+Environment.NewLine;
                    edids[start+i].conflict=3;
                }
            }
        }

        private void CheckForConflicts(List<Plugin> plugins) {
            List<Node> edids=new List<Node>();
            int i=0;
            if(!cbShowMain.Checked) i=1;
            for(;i<plugins.Count;i++) edids.AddRange(plugins[i].IDs);
            edids.Sort();
            if(!cbShowMain.Checked) plugins[0].CreateFakeIDs(edids);
            
            for(i=0;i<edids.Count;i++) {
                if(edids[i].formID!=0) break;
            }
            for(;i<edids.Count-1;i++) {
                if(edids[i].formID==uint.MaxValue) break;
                int ConflictCount=0;
                while(i+ConflictCount<edids.Count-1 && edids[i+ConflictCount+1].formID==edids[i].formID) ConflictCount++;
                if(ConflictCount>0) {
                    DetailConflicts(edids, i, ConflictCount+1);
                    i+=ConflictCount;
                }
            }
            for(;i<edids.Count-1;i++) {
                int ConflictCount=0;
                while(i+ConflictCount<edids.Count-1 && edids[i+ConflictCount+1].name==edids[i].name) ConflictCount++;
                if(ConflictCount>0) {
                    DetailConflicts(edids, i, ConflictCount+1);
                    i+=ConflictCount;
                }
            }
        }

        private void DisplayResults(List<Plugin> plugins) {
            SuspendLayout();
            this.FormBorderStyle=FormBorderStyle.Sizable;
            this.Width=450;
            splitContainer1.Panel2Collapsed=false;
            treeView.Visible=true;
            bActive.Visible=false;
            cbEDIDs.Visible=false;
            cbShowMain.Visible=false;
            cbNonConflicting.Visible=false;
            bCancel.Visible=false;
            int i=0;
            if(!cbShowMain.Checked) i++;
            for(;i<plugins.Count;i++) {
                treeView.Nodes.Add(plugins[i].MainNode);
            }
            ResumeLayout();
        }

        private void treeView_AfterSelect(object sender, TreeViewEventArgs e) {
            if(e.Action!=TreeViewAction.ByKeyboard&e.Action!=TreeViewAction.ByMouse) return;
            if(e.Node.Tag is string) tbDesc.Text=(string)e.Node.Tag;
            else tbDesc.Text=((Node)e.Node.Tag).Description;
        }
    }
}