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
    internal static class Settings {
        internal const int CurrentVersion = 18;

        internal static int MaxMemoryStreamSize; //64 mb
        internal static bool ShowScriptWarnings;
        internal static System.Drawing.Size mfSize;
        internal static bool mfMaximized;
        internal static int mfSplitterPos;
        internal static List<omodGroup> omodGroups;
        internal static bool AllowInsecureScripts;
        internal static bool WarnOnINIEdit;
        internal static bool WarnOnModDelete;
        internal static bool ShowNewModInfo;
        internal static bool LockFOV;
        internal static DeactiveStatus DefaultEspWarn;
        internal static string OblivionCommandLine;
        internal static int EspColWidth1;
        internal static int EspColWidth2;
        internal static System.Drawing.Size TextEdSize;
        internal static bool TextEdMaximized;
        internal static string omodDir;
        internal static string tempDir;
        internal static string omodCreatorFolderBrowserDir;
        internal static string BSACreatorFolderBrowserDir;
        internal static bool TrackConflicts;
        internal static bool AutoUpdateConflicts;
        internal static bool UpdateEsps;
        internal static byte UseProcessKiller;
        internal static bool ShowLaunchWarning;
        internal static bool SafeMode;
        internal static bool NewEspsLoadLast;
        internal static bool AdvSelectMany;
        internal static System.Drawing.Size ScriptEdSize;
        internal static bool ScriptEdMaximized;
        internal static bool NeverTouchLoadOrder;

        internal static bool CDShowMajor;
        internal static bool CDShowMinor;
        internal static bool CDShowVeryMinor;
        internal static bool CDIncludeEsp;
        internal static bool CDIgnoreInactiveEsp;
        internal static bool CDIncludeOmod;
        internal static bool CDIgnoreInactiveOmod;

        internal static string[] PKServicesStop;
        internal static string[] PKServicesKeep;
        internal static string[] PKProcessesClose;
        internal static string[] PKProcessesKill;
        internal static string[] PKProcessesKeep;
        internal static uint PKFlags;
        internal static int PKTimeOut;

        internal static int EspSortOrder;
        internal static int omodSortOrder;

        internal static bool UpdateInvalidation;
        internal static ArchiveInvalidationFlags InvalidationFlags;

        internal static bool CompressionBoost;
        internal static CompressionType dataCompressionType;
        internal static CompressionLevel omodCompressionLevel;
        internal static CompressionLevel dataCompressionLevel;

        internal static void SaveSettings() {
            BinaryWriter bw=new BinaryWriter(File.Open(Program.SettingsFile, FileMode.Create));
            Formatter formatter = new Formatter();
            bw.Write(CurrentVersion);
            bw.Write(MaxMemoryStreamSize);
            bw.Write(ShowScriptWarnings);
            bw.Write(mfSize.Width);
            bw.Write(mfSize.Height);
            bw.Write(mfMaximized);
            bw.Write(mfSplitterPos);
            bw.Write((byte)omodGroups.Count);
            foreach(omodGroup og in omodGroups) og.Write(bw, formatter);
            bw.Write(UpdateInvalidation);
            bw.Write(AllowInsecureScripts);
            bw.Write(WarnOnINIEdit);
            bw.Write(WarnOnModDelete);
            bw.Write(TextEdSize.Width);
            bw.Write(TextEdSize.Height);
            bw.Write(TextEdMaximized);
            bw.Write(ShowNewModInfo);
            bw.Write(LockFOV);
            bw.Write(omodDir);
            bw.Write(tempDir);
            bw.Write((byte)DefaultEspWarn);
            bw.Write(OblivionCommandLine);
            bw.Write(EspColWidth1);
            bw.Write(EspColWidth2);
            bw.Write(TrackConflicts);
            bw.Write(AutoUpdateConflicts);
            bw.Write(UpdateEsps);
            bw.Write(UseProcessKiller);
            bw.Write(ShowLaunchWarning);
            bw.Write(SafeMode);
            bw.Write(NeverTouchLoadOrder);

            bw.Write(CDShowMajor);
            bw.Write(CDShowMinor);
            bw.Write(CDShowVeryMinor);
            bw.Write(CDIncludeEsp);
            bw.Write(CDIgnoreInactiveEsp);
            bw.Write(CDIncludeOmod);
            bw.Write(CDIgnoreInactiveOmod);

            bw.Write(PKServicesStop.Length);
            for(int i=0;i<PKServicesStop.Length;i++) bw.Write(PKServicesStop[i]);
            bw.Write(PKServicesKeep.Length);
            for(int i=0;i<PKServicesKeep.Length;i++) bw.Write(PKServicesKeep[i]);
            bw.Write(PKProcessesClose.Length);
            for(int i=0;i<PKProcessesClose.Length;i++) bw.Write(PKProcessesClose[i]);
            bw.Write(PKProcessesKill.Length);
            for(int i=0;i<PKProcessesKill.Length;i++) bw.Write(PKProcessesKill[i]);
            bw.Write(PKProcessesKeep.Length);
            for(int i=0;i<PKProcessesKeep.Length;i++) bw.Write(PKProcessesKeep[i]);
            bw.Write(PKFlags);
            bw.Write(PKTimeOut);

            bw.Write(EspSortOrder);
            bw.Write(omodSortOrder);
            bw.Write(CompressionBoost);
            bw.Write((byte)dataCompressionType);
            bw.Write((byte)omodCompressionLevel);
            bw.Write((byte)dataCompressionLevel);
            bw.Write(omodCreatorFolderBrowserDir);
            bw.Write(BSACreatorFolderBrowserDir);
            bw.Write((uint)InvalidationFlags);

            bw.Write(NewEspsLoadLast);

            bw.Write(AdvSelectMany);
            bw.Write(ScriptEdSize.Width);
            bw.Write(ScriptEdSize.Height);
            bw.Write(ScriptEdMaximized);

            bw.Close();
        }
        internal static void LoadSettings() {
            DefaultSettings();
            if(!File.Exists(Program.SettingsFile)) return;
            BinaryReader br;
            Formatter formatter=new Formatter();
            int count;
            try {
                br=new BinaryReader(File.OpenRead(Program.SettingsFile));
            } catch { return; }
            try {
                int version=br.ReadInt32();
                if(version>CurrentVersion) {
                    MessageBox.Show("Your save file appears to be from a newer version of obmm, and cannot be read", "Error");
                    return;
                }
                MaxMemoryStreamSize=br.ReadInt32();
                ShowScriptWarnings=br.ReadBoolean();
                mfSize.Width=br.ReadInt32();
                mfSize.Height=br.ReadInt32();
                mfMaximized=br.ReadBoolean();
                mfSplitterPos=br.ReadInt32();
                byte b=br.ReadByte();
                if(version>=6) {
                    for(int i=0;i<b;i++) omodGroups.Add(omodGroup.Read(br, formatter));
                } else {
                    for(int i=0;i<b;i++) omodGroups.Add(new omodGroup(br.ReadString()));
                }
                UpdateInvalidation=br.ReadBoolean();
                if(version>=17) {
                    AllowInsecureScripts=br.ReadBoolean();
                }
                WarnOnINIEdit=br.ReadBoolean();
                WarnOnModDelete=br.ReadBoolean();
                TextEdSize.Width=br.ReadInt32();
                TextEdSize.Height=br.ReadInt32();
                TextEdMaximized=br.ReadBoolean();
                ShowNewModInfo=br.ReadBoolean();
                LockFOV=br.ReadBoolean();
                omodDir=br.ReadString();
                if(version>=5) {
                    tempDir=br.ReadString();
                }
                DefaultEspWarn=(DeactiveStatus)br.ReadByte();
                OblivionCommandLine=br.ReadString();
                EspColWidth1=br.ReadInt32();
                EspColWidth2=br.ReadInt32();
                if(version>=16) {
                    TrackConflicts=br.ReadBoolean();
                }
                if(version>=2) {
                    AutoUpdateConflicts=br.ReadBoolean();
                }
                if(version>=4) {
                    UpdateEsps=br.ReadBoolean();
                }
                if(version>=8&&version<=11) {
                    br.ReadBoolean();   //Used to be BackupOrigFiles
                }
                if(version>=9) {
                    UseProcessKiller=br.ReadByte();
                }
                if(version>=11) {
                    ShowLaunchWarning=br.ReadBoolean();
                }
                if(version>=12) {
                    SafeMode=br.ReadBoolean();
                }
                if(version>=18) {
                    NeverTouchLoadOrder=br.ReadBoolean();
                }

                CDShowMajor=br.ReadBoolean();
                CDShowMinor=br.ReadBoolean();
                CDShowVeryMinor=br.ReadBoolean();
                CDIncludeEsp=br.ReadBoolean();
                CDIgnoreInactiveEsp=br.ReadBoolean();
                CDIncludeOmod=br.ReadBoolean();
                CDIgnoreInactiveOmod=br.ReadBoolean();

                if(version>=10) {
                    count=br.ReadInt32();
                    PKServicesStop=new string[count];
                    for(int i=0;i<count;i++) PKServicesStop[i]=br.ReadString();
                    count=br.ReadInt32();
                    PKServicesKeep=new string[count];
                    for(int i=0;i<count;i++) PKServicesKeep[i]=br.ReadString();
                    count=br.ReadInt32();
                    PKProcessesClose=new string[count];
                    for(int i=0;i<count;i++) PKProcessesClose[i]=br.ReadString();
                    count=br.ReadInt32();
                    PKProcessesKill=new string[count];
                    for(int i=0;i<count;i++) PKProcessesKill[i]=br.ReadString();
                    count=br.ReadInt32();
                    PKProcessesKeep=new string[count];
                    for(int i=0;i<count;i++) PKProcessesKeep[i]=br.ReadString();
                    PKFlags=br.ReadUInt32();
                    PKTimeOut=br.ReadInt32();
                }

                EspSortOrder=br.ReadInt32();
                omodSortOrder=br.ReadInt32();
                CompressionBoost=br.ReadBoolean();
                if(version>=3) {
                    dataCompressionType=(CompressionType)br.ReadByte();
                    omodCompressionLevel=(CompressionLevel)br.ReadByte();
                    dataCompressionLevel=(CompressionLevel)br.ReadByte();
                }
                omodCreatorFolderBrowserDir=br.ReadString();
                if(version>=7) {
                    BSACreatorFolderBrowserDir=br.ReadString();
                }
                InvalidationFlags=(ArchiveInvalidationFlags)br.ReadUInt32();
                if(version>=13) {
                    NewEspsLoadLast=br.ReadBoolean();
                }
                if(version>=14) {
                    AdvSelectMany=br.ReadBoolean();
                    ScriptEdSize.Width=br.ReadInt32();
                    ScriptEdSize.Height=br.ReadInt32();
                    ScriptEdMaximized=br.ReadBoolean();
                }

                if(version<15&&(ArchiveInvalidationFlags.EditBSAs&InvalidationFlags)>0) {
                    MessageBox.Show("The recommended archiveinvalidation method is BSA redirection, but this copy of obmm is set up to use BSA alteration.\n"+
                        "Unless there is a specific reason you require BSA alteration, it is recommended that you switch back to the default settings.\n"+
                        "To do this, click 'utilities|Archive invalidation'\n"+
                        "Click 'Reset to defaults'\n"+
                        "Click 'Update now'");
                }
            } catch {
                DefaultSettings();
            } finally {
                if(br!=null) br.Close();
            }
        }
        internal static void DefaultSettings() {
            MaxMemoryStreamSize=67108864; //64 mb
            ShowScriptWarnings=true;
            mfSize=System.Drawing.Size.Empty;
            mfMaximized=false;
            mfSplitterPos=0;
            omodGroups=new List<omodGroup>();
            UpdateInvalidation=false;
            AllowInsecureScripts=true;
            WarnOnINIEdit=true;
            WarnOnModDelete=true;
            ShowNewModInfo=false;
            LockFOV=false;
            DefaultEspWarn=DeactiveStatus.WarnAgainst;
            OblivionCommandLine="";
            EspColWidth1=-1;
            EspColWidth2=-1;
            InvalidationFlags=ArchiveInvalidationFlags.Default;
            TextEdSize=System.Drawing.Size.Empty;
            TextEdMaximized=false;
            TrackConflicts=true;
            AutoUpdateConflicts=false;
            UpdateEsps=false;
            UseProcessKiller=0;
            ShowLaunchWarning=true;
            SafeMode=true;
            AdvSelectMany=false;
            ScriptEdSize=new System.Drawing.Size(0, 0);
            ScriptEdMaximized=true;
            NeverTouchLoadOrder=false;

            CDShowMajor=true;
            CDShowMinor=false;
            CDShowVeryMinor=false;
            CDIncludeEsp=true;
            CDIgnoreInactiveEsp=false;
            CDIncludeOmod=true;
            CDIgnoreInactiveOmod=false;
            EspSortOrder=0;
            omodSortOrder=0;
            CompressionBoost=false;
            dataCompressionType=CompressionType.SevenZip;
            omodCompressionLevel=CompressionLevel.VeryHigh;
            dataCompressionLevel=CompressionLevel.High;

            PKServicesStop=new string[0];
            PKServicesKeep=new string[] { "Windows Audio", "Windows Management Instrumentation" };
            PKProcessesClose=new string[0];
            PKProcessesKill=new string[0];
            PKProcessesKeep=new string[] { "explorer.exe", "services.exe", "svchost.exe", "lsass.exe", "csrss.exe", "smss.exe", 
                "winlogon.exe", "system", "system idle process" };
            PKFlags=0;
            PKTimeOut=1000;

            omodCreatorFolderBrowserDir="";
            BSACreatorFolderBrowserDir="";
            omodDir=@"obmm\mods\";
            tempDir="";

            NewEspsLoadLast=true;
        }
    }
}