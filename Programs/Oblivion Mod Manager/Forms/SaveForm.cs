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
using System.Drawing;
using System.Drawing.Imaging;
using System.Windows.Forms;
using System.IO;

namespace OblivionModManager {
    internal partial class SaveForm : Form {
        internal static string SaveFolder=null;
        internal int ActiveEsps;

        private struct SaveFile {
            internal DateTime saved;
            internal string FileName;
            internal string Player;
            internal string Location;

            //internal byte[] face;
            //internal int FaceOffset;

            internal byte[] ImageData;
            internal int ImageWidth;
            internal int ImageHeight;
            internal string[] plugins;
            private Bitmap image;
            internal Bitmap Image {
                get {
                    if(image!=null) return image;
                    image=new Bitmap(ImageWidth, ImageHeight,PixelFormat.Format24bppRgb);
                    BitmapData bd=image.LockBits(new Rectangle(0, 0, ImageWidth, ImageHeight), ImageLockMode.WriteOnly, PixelFormat.Format24bppRgb);
                    System.Runtime.InteropServices.Marshal.Copy(ImageData, 0, bd.Scan0, ImageData.Length);
                    image.UnlockBits(bd);
                    return image;
                }
            }
        }

        internal enum SaveSortOrder { Name, Player, Location, Date, FileSize }
        internal class SaveListSorter : System.Collections.IComparer {
            internal static SaveSortOrder order=SaveSortOrder.Name;
            public int Compare(object a, object b) {
                SaveFile sa=(SaveFile)((ListViewItem)a).Tag;
                SaveFile sb=(SaveFile)((ListViewItem)b).Tag;
                switch(order) {
                case SaveSortOrder.Name:
                    return string.Compare(sa.FileName, sb.FileName);
                case SaveSortOrder.Player:
                    return string.Compare(sa.Player, sb.Player);
                case SaveSortOrder.Location:
                    return string.Compare(sa.Location, sb.Location);
                case SaveSortOrder.Date:
                    return DateTime.Compare(sa.saved, sb.saved);
                case SaveSortOrder.FileSize:
                    long sizea=(new System.IO.FileInfo(SaveFolder+sa.FileName)).Length;
                    long sizeb=(new System.IO.FileInfo(SaveFolder+sb.FileName)).Length;
                    if(sizea==sizeb) return 0;
                    if(sizea>sizeb) return -1; else return 1;
                default: return 0;
                }
            }
        }

        private readonly List<SaveFile> saves=new List<SaveFile>();

        private void FindSaveFolder() {
            if(OblivionINI.GetINIValue("[general]", "bUseMyGamesDirectory")=="0") {
                SaveFolder="saves\\";
            } else {
                SaveFolder=Program.OblivionINIDir+OblivionINI.GetINIValue("[general]", "SLocalSavePath");
            }
        }

        private void CountActiveEsps() {
            ActiveEsps=0;
            foreach(EspInfo ei in Program.Data.Esps) {
                if(ei.Active) ActiveEsps++;
            }
        }

        internal SaveForm() {
            if(SaveFolder==null) FindSaveFolder();
            InitializeComponent();
            cmbSort.SelectedIndex=3;
            lvSaves.ListViewItemSorter=new SaveListSorter();
            CountActiveEsps();
            foreach(string file in Directory.GetFiles(SaveFolder)) {
                BinaryReader br=new BinaryReader(File.OpenRead(file));
                if(br.BaseStream.Length<12) {
                    br.Close();
                    continue;
                }
                string s="";
                for(int i=0;i<12;i++) s+=(char)br.ReadByte();
                if(s!="TES4SAVEGAME") {
                    br.Close();
                    continue;
                }
                SaveFile sf=new SaveFile();
                sf.FileName=Path.GetFileName(file);
                br.BaseStream.Position+=30;

                byte b=br.ReadByte();
                sf.Player="";
                for(int i=1;i<b;i++) sf.Player+=(char)br.ReadByte();
                br.ReadByte(); //Read the terminating \0
                br.ReadInt16();
                b=br.ReadByte();
                sf.Location="";
                for(int i=1;i<b;i++) sf.Location+=(char)br.ReadByte();
                br.ReadByte(); //Read the terminating \0
                br.ReadSingle();
                br.ReadInt32();
                short year, month, day, hour, minute, second;
                year=br.ReadInt16();
                month=br.ReadInt16();
                br.ReadInt16();
                day=br.ReadInt16();
                hour=br.ReadInt16();
                minute=br.ReadInt16();
                second=br.ReadInt16();
                br.ReadInt16();
                sf.saved=new DateTime(year, month, day, hour, minute, second);

                br.ReadInt32();
                sf.ImageWidth=br.ReadInt32();
                sf.ImageHeight=br.ReadInt32();
                sf.ImageData=new byte[sf.ImageHeight*sf.ImageWidth*3];
                br.Read(sf.ImageData, 0, sf.ImageData.Length);
                //Flip the blue and red channels
                for(int i=0;i<sf.ImageWidth*sf.ImageHeight;i++) {
                    byte temp=sf.ImageData[i*3];
                    sf.ImageData[i*3]=sf.ImageData[i*3+2];
                    sf.ImageData[i*3+2]=temp;
                }
                sf.plugins=new string[br.ReadByte()];
                for(int i=0;i<sf.plugins.Length;i++) {
                    b=br.ReadByte();
                    sf.plugins[i]="";
                    for(int j=0;j<b;j++) sf.plugins[i]+=(char)br.ReadByte();
                }
                /*{
                    br.ReadInt32();
                    int changerecs=br.ReadInt32();
                    br.BaseStream.Position+=32;
                    br.BaseStream.Position+=br.ReadUInt16()*8;  //globals
                    br.BaseStream.Position+=br.ReadUInt16();    //Class data
                    br.BaseStream.Position+=br.ReadUInt16();    //prcoesses data
                    br.BaseStream.Position+=br.ReadUInt16();    //spectator data
                    br.BaseStream.Position+=br.ReadUInt16();    //weather
                    br.ReadInt32();
                    for(int i=0;i<br.ReadInt32();i++) {
                        s=""+(char)br.ReadByte()+(char)br.ReadByte()+(char)br.ReadByte()+(char)br.ReadByte();
                        if(s=="GRUP") br.BaseStream.Position+=br.ReadInt32()-4;
                        else br.BaseStream.Position+=br.ReadInt32()+12;
                    }
                    br.BaseStream.Position+=br.ReadUInt16();    //Quickkeys
                    br.BaseStream.Position+=br.ReadUInt16();    //HUD
                    br.BaseStream.Position+=br.ReadUInt16();    //Interface
                    br.BaseStream.Position+=br.ReadUInt16();    //Regions
                    for(int i=0;i<6;i++) {
                        int formid=br.ReadInt32();
                        br.BaseStream.Position+=6;
                        //br.BaseStream.Position+=10;
                        br.BaseStream.Position+=br.ReadUInt16();
                    }
                    uint playerid=br.ReadUInt32();
                    byte type=br.ReadByte();
                    br.BaseStream.Position+=5;
                    ushort playersize=br.ReadUInt16();
                    int upto=0;
                    for(int i=0;i<playersize;i++) {
                        if(br.ReadByte()==(byte)sf.Player[upto]) {
                            if(++upto==sf.Player.Length) {
                                sf.FaceOffset=(int)br.BaseStream.Position-(sf.Player.Length+542);
                                br.BaseStream.Position=sf.FaceOffset;
                                sf.face=br.ReadBytes(520);
                            }
                        } else upto=0;
                    }
                }*/
                br.Close();
                saves.Add(sf);
            }
            UpdateSaveList();
        }

        private void UpdateSaveList() {
            lvSaves.SuspendLayout();
            lvSaves.Clear();
            foreach(SaveFile sf in saves) {
                ListViewItem lvi=new ListViewItem(sf.FileName);
                lvi.ToolTipText="Player: "+sf.Player+"\nLocation: "+sf.Location+
                    "\nDate saved: "+sf.saved.ToString()+"\nNumber of plugins: "+sf.plugins.Length.ToString();
                lvi.Tag=sf;
                bool match=false;
                for(int i=0;i<sf.plugins.Length;i++) {
                    EspInfo ei=Program.Data.GetEsp(sf.plugins[i]);
                    if(ei==null||!ei.Active) {
                        lvi.ImageIndex=1;
                        match=true;
                        break;
                    }
                }
                if(!match) {
                    if(ActiveEsps>sf.plugins.Length) lvi.ImageIndex=2;
                    else lvi.ImageIndex=3;
                }
                lvSaves.Items.Add(lvi);
            }
            lvSaves.ResumeLayout();
        }

        private void UpdatePluginList(string[] plugins) {
            lvPlugins.SuspendLayout();
            lvPlugins.Items.Clear();
            foreach(string s in plugins) {
                ListViewItem lvi=new ListViewItem(s);
                EspInfo ei=Program.Data.GetEsp(s);
                if(ei==null) {
                    lvi.ImageIndex=0;
                    lvi.ToolTipText="File not found";
                } else {
                    if(ei.Active) lvi.ImageIndex=3;
                    else lvi.ImageIndex=1;
                    lvi.Tag=ei;
                    lvi.ToolTipText=ei.FileName+"\nAuthor: "+ei.header.Author+"\n\n"+ei.header.Description;
                }
                lvPlugins.Items.Add(lvi);
            }
            lvPlugins.ResumeLayout();
        }

        private void lvSaves_SelectedIndexChanged(object sender, EventArgs e) {
            if(lvSaves.SelectedItems.Count!=1) return;
            SaveFile sf=(SaveFile)lvSaves.SelectedItems[0].Tag;
            lName.Text="Name: "+sf.Player;
            lLocation.Text="Location: "+sf.Location;
            lDate.Text="Date saved: "+sf.saved.ToString();
            UpdatePluginList(sf.plugins);
            pictureBox1.Image=sf.Image;
        }

        private void cmbSort_KeyPress(object sender, KeyPressEventArgs e) {
            e.Handled=true;
        }

        private void cmbSort_SelectedIndexChanged(object sender, EventArgs e) {
            SaveListSorter.order=(SaveSortOrder)cmbSort.SelectedIndex;
            lvSaves.Sort();
        }

        private void bSync_Click(object sender, EventArgs e) {
            if(lvSaves.SelectedItems.Count!=1) return;
            if(lvSaves.SelectedItems[0].ImageIndex==0) {
                if(MessageBox.Show("One or more plugins required by that save could not be found.\n"+
                    "Sync anyway?", "Warning", MessageBoxButtons.YesNo)!=DialogResult.Yes) return;
            }
            foreach(EspInfo ei in Program.Data.Esps) ei.Active=false;
            foreach(string s in ((SaveFile)lvSaves.SelectedItems[0].Tag).plugins) {
                EspInfo ei=Program.Data.GetEsp(s);
                if(ei!=null) ei.Active=true;
            }
            CountActiveEsps();
            UpdatePluginList(((SaveFile)lvSaves.SelectedItems[0].Tag).plugins);
            UpdateSaveList();
        }
    }
}