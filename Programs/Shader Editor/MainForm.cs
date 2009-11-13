using System;
using System.IO;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using Encoding=System.Text.Encoding;

namespace ShaderEdit {
    public partial class MainForm : Form {

        private class Shader {
            internal string name;
            internal char[] name2;
            internal byte[] data;
        }

        private uint unknown;
        private readonly List<Shader> shaders=new List<Shader>();
        private bool ChangedShader=false;
        private bool ChangedFile=false;
        private int Editing=-1;
        private string FileName="";
        private HLSLImporter HLSLImporterForm=new HLSLImporter();

        [DllImport("ShaderDisasm", CharSet=CharSet.Ansi)]
        private unsafe static extern sbyte* Disasm(byte[] data, int len, byte Color);

        [DllImport("ShaderDisasm", CharSet=CharSet.Ansi)]
        private unsafe static extern byte* Asm(byte[] data, int len);

        [DllImport("ShaderDisasm", CharSet=CharSet.Ansi)]
        private unsafe static extern byte* Compile(string data,int len,string EntryPoint,string Profile,byte Debug);

        public MainForm() { InitializeComponent(); }

        private void cmbShaderSelect_KeyPress(object sender, KeyPressEventArgs e) { e.Handled=true; }

        private void bOpen_Click(object sender, EventArgs e) {
            openFileDialog1.Filter="Oblivion shader package (*.sdp)|*.sdp";
            openFileDialog1.Title="Select Shader package to edit";
            if(openFileDialog1.ShowDialog()!=DialogResult.OK) return;
            FileName=Path.GetFileName(openFileDialog1.FileName);
            Text="SDP Editor ("+FileName+")";
            BinaryReader br=new BinaryReader(File.OpenRead(openFileDialog1.FileName), System.Text.Encoding.Default);
            unknown=br.ReadUInt32();
            int num=br.ReadInt32();
            br.ReadInt32();
            for(int i=0;i<num;i++) {
                Shader s=new Shader();
                char[] name=br.ReadChars(0x100);
                s.name="";
                s.name2=name;
                for(int i2=0;i2<100;i2++) { if(name[i2]=='\0') break; s.name+=name[i2]; }
                int size=br.ReadInt32();
                s.data=br.ReadBytes(size);
                shaders.Add(s);
                cmbShaderSelect.Items.Add(s.name);
            }
            br.Close();
            bOpen.Enabled=false;
            bClose.Enabled=true;
            cmbShaderSelect.Enabled=true;
            bSave.Enabled=true;
        }

        private unsafe void cmbShaderSelect_SelectedIndexChanged(object sender, EventArgs e) {
            if(cmbShaderSelect.SelectedIndex==Editing) return;
            if(ChangedShader) {
                if(!Compile()) {
                    if(MessageBox.Show("The current shader could not be compiled. Do you want to discard your changes?", "Question",
                        MessageBoxButtons.YesNo)!=DialogResult.Yes) return;
                }
            }
            Editing=cmbShaderSelect.SelectedIndex;
            sbyte* ptr=Disasm(shaders[Editing].data, shaders[Editing].data.Length, 0);
            string text=new string(ptr);
            text=text.Replace(""+(char)10, Environment.NewLine);
            tbEdit.Text=text;
            bCompile.Enabled=true;
            tbEdit.Enabled=true;
            tbEdit.TextChanged+=new EventHandler(tbEdit_TextChanged);
            bImport.Enabled=true;
        }

        void tbEdit_TextChanged(object sender, EventArgs e) {
            ChangedShader=true;
            Text="SDP Editor ("+FileName+" *)";
            tbEdit.TextChanged-=tbEdit_TextChanged;
        }

        private bool Save() {
            saveFileDialog1.Title="Select file name to save as";
            saveFileDialog1.Filter="Oblivion shader package (*.sdp)|*.sdp";
            if(ChangedShader) {
                if(!Compile()) {
                    if(MessageBox.Show("One of your shaders did not compile. Do you want to save anyway?", "Question", MessageBoxButtons.YesNo)!=
                        DialogResult.Yes) return false;
                }
            }
            if(saveFileDialog1.ShowDialog()!=DialogResult.OK) return false;
            BinaryWriter bw=new BinaryWriter(File.Create(saveFileDialog1.FileName), System.Text.Encoding.Default);
            bw.Write(unknown);
            bw.Write(shaders.Count);
            int tsize=0;
            foreach(Shader s in shaders) tsize+=0x100+4+s.data.Length;
            bw.Write(tsize);
            foreach(Shader s in shaders) {
                bw.Write(s.name2);
                bw.Write(s.data.Length);
                bw.Write(s.data);
            }
            bw.Close();
            ChangedFile=false;
            return true;
        }

        private void bSave_Click(object sender, EventArgs e) { Save(); }

        private unsafe bool Compile() {
            byte[] b=new byte[tbEdit.Text.Length];
            for(int i=0;i<tbEdit.Text.Length;i++) b[i]=(byte)tbEdit.Text[i];
            byte* data=Asm(b, b.Length);
            int size=(data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0];
            if(size==0) {
                string error="";
                for(int i=4;i<0x10000;i++) { if(data[i]=='\0') break; error+=(char)data[i]; }
                MessageBox.Show("Shader assembly failed: "+Environment.NewLine+error.Replace(""+(char)10, Environment.NewLine));
                return false;
            } else {
                shaders[Editing].data=new byte[size];
                byte[] newdata=new byte[size];
                for(int i=0;i<size;i++) newdata[i]=data[i+4];
                Array.Copy(newdata, 0, shaders[Editing].data, 0, size);
                ChangedFile=true;
                ChangedShader=false;
                Text="SDP Editor ("+FileName+")";
                return true;
            }
        }

        private void bCompile_Click(object sender, EventArgs e) { Compile(); }

        private void bClose_Click(object sender, EventArgs e) {
            if(ChangedShader||ChangedFile) {
                switch(MessageBox.Show("Do you want to save your changes?", "Question", MessageBoxButtons.YesNoCancel)) {
                case DialogResult.Yes:
                    if(!Save()) return;
                    break;
                case DialogResult.No:
                    break;
                default: return;
                }
            }
            unknown=0;
            shaders.Clear();
            cmbShaderSelect.Enabled=false;
            bOpen.Enabled=true;
            bClose.Enabled=false;
            bSave.Enabled=false;
            bCompile.Enabled=false;
            ChangedFile=false;
            ChangedShader=false;
            Editing=-1;
            cmbShaderSelect.Items.Clear();
            cmbShaderSelect.Text="";
            tbEdit.Text="";
            tbEdit.Enabled=false;
            bImport.Enabled=false;
        }

        private void bImport_Click(object sender, EventArgs e) {
            ImportMenu.Show(PointToScreen(bImport.Location));
        }

        private unsafe void importHLSLToolStripMenuItem_Click(object sender, EventArgs e) {
            openFileDialog1.Filter="HLSL text file|*.*";
            openFileDialog1.Title="Select HLSL file to import";
            if(openFileDialog1.ShowDialog()!=DialogResult.OK) return;
            if(HLSLImporterForm.ShowDialog()!=DialogResult.OK) return;
            string text=File.ReadAllText(openFileDialog1.FileName, Encoding.Default);
            byte* data=Compile(text, text.Length, HLSLImporterForm.EntryPoint, HLSLImporterForm.Profile, HLSLImporterForm.Debug);
            int size=(data[3]<<24)+(data[2]<<16)+(data[1]<<8)+data[0];
            if(size==0) {
                string error="";
                for(int i=4;i<0x10000;i++) { if(data[i]=='\0') break; error+=(char)data[i]; }
                MessageBox.Show("Shader compilation failed: "+Environment.NewLine+error.Replace(""+(char)10, Environment.NewLine));
            } else {
                shaders[Editing].data=new byte[size];
                byte[] newdata=new byte[size];
                for(int i=0;i<size;i++) newdata[i]=data[i+4];
                Array.Copy(newdata, 0, shaders[Editing].data, 0, size);
                ChangedFile=true;
                ChangedShader=false;
                Text="SDP Editor ("+FileName+")";
                Editing=-1;
                cmbShaderSelect_SelectedIndexChanged(null, null);
            }
        }

        private unsafe void importBinaryToolStripMenuItem_Click(object sender, EventArgs e) {
            openFileDialog1.Filter="Compiled shader (*.vso,*.pso)|*.vso;*.pso";
            openFileDialog1.Title="Select HLSL file to import";
            if(openFileDialog1.ShowDialog()!=DialogResult.OK) return;
            byte[] b=File.ReadAllBytes(openFileDialog1.FileName);
            sbyte* result=Disasm(b, b.Length,0);
            if(new IntPtr(result)==IntPtr.Zero) {
                MessageBox.Show("An error occured during shader disassembly", "Error");
            } else {
                shaders[Editing].data=b;
                string text=new string(result);
                tbEdit.Text=text.Replace(""+(char)10, Environment.NewLine);
            }
        }

        private void exportBinaryToolStripMenuItem_Click(object sender, EventArgs e) {
            if(ChangedShader&&!Compile()) return;
            saveFileDialog1.Title="Select file name to save as";
            saveFileDialog1.Filter="Compiled shader (*.vso,*.pso)|*.vso;*.pso";
            if(saveFileDialog1.ShowDialog()!=DialogResult.OK) return;
            File.WriteAllBytes(saveFileDialog1.FileName,shaders[Editing].data);
        }
    }
}