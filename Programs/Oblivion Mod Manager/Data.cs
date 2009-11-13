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
using MessageBox=System.Windows.Forms.MessageBox;

namespace OblivionModManager {
    [Serializable]
    internal class sData {
        internal readonly List<omod> omods=new List<omod>();

        internal readonly List<DataFileInfo> DataFiles=new List<DataFileInfo>();

        internal readonly List<EspInfo> Esps=new List<EspInfo>();

        internal readonly List<BSA> BSAs=new List<BSA>();

        internal readonly List<INIEditInfo> INIEdits=new List<INIEditInfo>();

        internal bool DoesModExist(string s) {
            s=s.ToLower();
            foreach(omod o in omods) {
                if(o.LowerFileName==s) return true;
            }
            return false;
        }
        internal bool DoesModExist(ConflictData cd, bool Active) {
            foreach(omod o in omods) {
                if(Active&&o.Conflict!=ConflictLevel.Active) continue;
                if(cd==o) return true;
            }
            return false;
        }

        internal omod GetMod(string s) {
            s=s.ToLower();
            foreach(omod o in omods) {
                if(o.LowerFileName==s) return o;
            }
            return null;
        }

        internal bool DoesEspExist(string s) {
            s=s.ToLower();
            foreach(EspInfo ei in Esps) {
                if(ei.LowerFileName==s) return true;
            }
            return false;
        }

        internal EspInfo GetEsp(string s) {
            s=s.ToLower();
            foreach(EspInfo ei in Esps) {
                if(ei.LowerFileName==s) return ei;
            }
            return null;
        }

        internal bool DoesDataFileExist(string s) {
            s=s.ToLower();
            foreach(DataFileInfo sfi in DataFiles) {
                if(sfi.LowerFileName==s) return true;
            }
            return false;
        }
        internal bool DoesDataFileExist(DataFileInfo dfi) {
            foreach(DataFileInfo sfi in DataFiles) {
                if(sfi==dfi) return true;
            }
            return false;
        }

        internal DataFileInfo GetDataFile(string s) {
            s=s.ToLower();
            foreach(DataFileInfo sfi in DataFiles) {
                if(sfi.LowerFileName==s) return sfi;
            }
            return null;
        }
        internal DataFileInfo GetDataFile(DataFileInfo dfi) {
            foreach(DataFileInfo sfi in DataFiles) {
                if(sfi==dfi) return sfi;
            }
            return null;
        }

        internal bool DoesBSAExist(string s) {
            s=s.ToLower();
            foreach(BSA b in BSAs) {
                if(b.LowerFileName==s) return true;
            }
            return false;
        }

        internal BSA GetBSA(string s) {
            s=s.ToLower();
            foreach(BSA b in BSAs) {
                if(b.LowerFileName==s) return b;
            }
            return null;
        }

        private class PluginSorter : IComparer<EspInfo> {
            public int Compare(EspInfo a, EspInfo b) {
                if(Path.GetExtension(a.LowerFileName)!=Path.GetExtension(b.LowerFileName)) {
                    if(Path.GetExtension(a.LowerFileName)==".esm") {
                        return -1;
                    } else {
                        return 1;
                    }
                }
                return DateTime.Compare(a.DateModified, b.DateModified);
            }
        }
        internal void SortEsps() {
            foreach(EspInfo ei in Esps) {
                ei.DateModified=File.GetLastWriteTime("data\\"+ei.FileName);
            }
            Esps.Sort(new PluginSorter());

            if(Settings.NeverTouchLoadOrder) return;

            TimeSpan ts=TimeSpan.FromMinutes(1);
            for(int i=0;i<Esps.Count-1;i++) {
                if(Esps[i].DateModified==Esps[i+1].DateModified) {
                    for(int j=i+1;j<Esps.Count;j++) {
                        Esps[j].DateModified+=ts;
                        File.SetLastWriteTime("data\\"+Esps[j].FileName, Esps[j].DateModified);
                    }
                }
            }
            bool switched=true;
            while(switched) {
                switched=false;
                for(int i=0;i<Esps.Count-1;i++) {
                    if(Esps[i].DateModified>Esps[i+1].DateModified) {
                        DateTime dt=Esps[i].DateModified;
                        Esps[i].DateModified=Esps[i+1].DateModified;
                        Esps[i+1].DateModified=dt;
                        switched=true;
                        File.SetLastWriteTime("data\\"+Esps[i].FileName, Esps[i].DateModified);
                        File.SetLastWriteTime("data\\"+Esps[i+1].FileName, Esps[i+1].DateModified);
                    }
                }
            }
        }

        internal void InsertESP(EspInfo ei, PluginLoadInfo[] plis, bool early) {
            if(Settings.NeverTouchLoadOrder) {
                SortEsps();
                return;
            }

            DateTime MinTime;
            DateTime MaxTime;
            DateTime InsertAt=DateTime.Now;
            if(early&&Esps.Count>0) {
                MaxTime=File.GetLastWriteTime("Data\\"+Esps[0].FileName);
                if(InsertAt+TimeSpan.FromMinutes(1)>=MaxTime) InsertAt=MaxTime-TimeSpan.FromMinutes(1);
            } else if(Settings.NewEspsLoadLast&&Esps.Count>0) {
                MaxTime=File.GetLastWriteTime("Data\\"+(Esps[Esps.Count-1]).FileName);
                if(InsertAt-TimeSpan.FromMinutes(1)<=MaxTime) InsertAt=MaxTime+TimeSpan.FromMinutes(1);
            }
            MinTime=DateTime.MinValue;
            MaxTime=DateTime.MaxValue;
            foreach(EspInfo esp in Esps) {
                foreach(string target in esp.MustLoadAfter) {
                    if(target!=ei.LowerFileName) continue;
                    DateTime temp=File.GetLastWriteTime("Data\\"+esp.FileName);
                    if(temp<MaxTime) MaxTime=temp;
                }
                foreach(string target in esp.MustLoadBefore) {
                    if(target!=ei.LowerFileName) continue;
                    DateTime temp=File.GetLastWriteTime("Data\\"+esp.FileName);
                    if(temp>MinTime) MinTime=temp;
                }
            }
            foreach(PluginLoadInfo pli in plis) {
                if(pli.Plugin.ToLower()!=ei.LowerFileName) continue;
                EspInfo target=GetEsp(pli.Target);
                if(pli.LoadAfter) {
                    ei.MustLoadAfter.Add(pli.Target.ToLower());
                    if(target!=null) {
                        DateTime temp=File.GetLastWriteTime("Data\\"+target.FileName);
                        if(temp>MinTime) MinTime=temp;
                    }
                } else {
                    ei.MustLoadBefore.Add(pli.Target.ToLower());
                    if(target!=null) {
                        DateTime temp=File.GetLastWriteTime("Data\\"+target.FileName);
                        if(temp<MaxTime) MaxTime=temp;
                    }
                }
            }
            if(MinTime>DateTime.MinValue&&MaxTime<DateTime.MaxValue&&MinTime>=MaxTime+TimeSpan.FromSeconds(2)) {
                MessageBox.Show("Unable to correctly insert plugin "+ei.FileName+" because of conflicting load order information.\n"+
                    "This is most likely to occur if two existing mods are already loading in the wrong order.\n"+
                    "Please check the readmes of other mods to ensure you have installed them correctly.\n"+
                    ei.FileName+" has been added to the end of the load order.", "Error");
            } else {
                if(InsertAt>MaxTime) InsertAt=MaxTime-TimeSpan.FromMinutes(2);
                while(InsertAt<MinTime+TimeSpan.FromSeconds(1)) InsertAt+=TimeSpan.FromSeconds(1);
                File.SetLastWriteTime("Data\\"+ei.FileName, InsertAt);
            }
            ei.Active=true;
            Esps.Add(ei);
            SortEsps();
        }
    }
}
