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
using System.Security.Policy;
using System.CodeDom.Compiler;
using Assembly=System.Reflection.Assembly;
using sList=System.Collections.Generic.List<string>;

namespace OblivionModManager.Scripting {
    internal static class DotNetScriptHandler {
        private static readonly Microsoft.CSharp.CSharpCodeProvider csCompiler=new Microsoft.CSharp.CSharpCodeProvider();
        private static readonly Microsoft.VisualBasic.VBCodeProvider vbCompiler=new Microsoft.VisualBasic.VBCodeProvider();
        private static readonly CompilerParameters cParams;
        private static readonly Evidence evidence;

        private static readonly string ScriptOutputPath=Program.TempDir+"dotnetscript.dll";

        static DotNetScriptHandler() {
            cParams=new CompilerParameters();
            cParams.GenerateExecutable=false;
            cParams.GenerateInMemory=false;
            cParams.IncludeDebugInformation=false;
            cParams.OutputAssembly=ScriptOutputPath;
            cParams.ReferencedAssemblies.Add("OblivionModManager.exe");
            cParams.ReferencedAssemblies.Add("System.dll");
            cParams.ReferencedAssemblies.Add("System.Drawing.dll");
            cParams.ReferencedAssemblies.Add("System.Windows.Forms.dll");
            cParams.ReferencedAssemblies.Add("System.Xml.dll");

            evidence=new Evidence();
            evidence.AddHost(new Zone(System.Security.SecurityZone.Internet));
        }

        private static byte[] Compile(string code, ScriptType language) {
            string[] errors, warnings;
            string stdout;
            return Compile(code, out errors, out warnings, out stdout, language);
        }
        private static byte[] Compile(string code, out string[] errors, out string[] warnings, out string stdout, ScriptType language) {
            CompilerResults results;
            switch(language) {
            case ScriptType.cSharp:
                results=csCompiler.CompileAssemblyFromSource(cParams, code);
                break;
            case ScriptType.vb:
                results=vbCompiler.CompileAssemblyFromSource(cParams, code);
                break;
            default:
                throw new obmmException("Invalid language specified for .NET script compiler");
            }
            stdout="";
            for(int i=0;i<results.Output.Count;i++) stdout+=results.Output[i]+Environment.NewLine;
            if(results.Errors.HasErrors) {
                sList msgs=new sList();
                foreach(CompilerError ce in results.Errors) {
                    if(!ce.IsWarning) msgs.Add("Error on Line " + ce.Line + ": "+ ce.ErrorText);
                }
                errors=msgs.ToArray();
            } else errors=null;
            if(results.Errors.HasWarnings) {
                sList msgs=new sList();
                foreach(CompilerError ce in results.Errors) {
                    if(ce.IsWarning) msgs.Add("Warning on Line " + ce.Line + ": " + ce.ErrorText);
                }
                warnings=msgs.ToArray();
            } else warnings=null;
            if(results.Errors.HasErrors) {
                return null;
            } else {
                byte[] data=System.IO.File.ReadAllBytes(results.PathToAssembly);
                System.IO.File.Delete(results.PathToAssembly);
                return data;
            }
        }

        private static string CheckSyntax(string code, out string stdout, ScriptType language) {
            string[] errors;
            string[] warnings;
            byte[] data;
            string errout = "";

            data = Compile(code, out errors, out warnings, out stdout, language);
            if(data == null) {
                for(int i = 0;i < errors.Length;i++) {
                    errout += errors[i] + Environment.NewLine;
                }
                return errout;
            }
            return "";
        }
        private static void Execute(string script, IScriptFunctions functions, ScriptType language) {
            byte[] data=Compile(script, language);
            if(data==null) {
                System.Windows.Forms.MessageBox.Show("C# script failed to compile", "Error");
                return;
            }
            Assembly asm=AppDomain.CurrentDomain.Load(data, null, evidence);
            IScript s=asm.CreateInstance("Script") as IScript;
            if(s==null) {
                System.Windows.Forms.MessageBox.Show("C# or vb script did not contain a 'Script' class in the root namespace, or IScript was not implemented",
                    "Error");
                functions.FatalError();
                return;
            }
            try {
                s.Execute(functions);
            } catch(ExecutionCancelledException) {
                functions.FatalError();
            } finally {
                Classes.BSAArchive.Clear();
            }
        }

        internal static string CheckSyntaxCS(string code, out string stdout) {
            return CheckSyntax(code, out stdout, ScriptType.cSharp);
        }
        internal static string CheckSyntaxVB(string code, out string stdout) {
            return CheckSyntax(code, out stdout, ScriptType.vb);
        }

        internal static void ExecuteCS(string script, IScriptFunctions functions) {
            Execute(script, functions, ScriptType.cSharp);
        }
        internal static void ExecuteVB(string script, IScriptFunctions functions) {
            Execute(script, functions, ScriptType.vb);
        }
    }
}
