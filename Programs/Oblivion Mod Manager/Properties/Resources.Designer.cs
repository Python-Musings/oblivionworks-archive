﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version:2.0.50727.42
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace OblivionModManager.Properties {
    using System;
    
    
    /// <summary>
    ///   A strongly-typed resource class, for looking up localized strings, etc.
    /// </summary>
    // This class was auto-generated by the StronglyTypedResourceBuilder
    // class via a tool like ResGen or Visual Studio.
    // To add or remove a member, edit your .ResX file then rerun ResGen
    // with the /str option, or rebuild your VS project.
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("System.Resources.Tools.StronglyTypedResourceBuilder", "2.0.0.0")]
    [global::System.Diagnostics.DebuggerNonUserCodeAttribute()]
    [global::System.Runtime.CompilerServices.CompilerGeneratedAttribute()]
    internal class Resources {
        
        private static global::System.Resources.ResourceManager resourceMan;
        
        private static global::System.Globalization.CultureInfo resourceCulture;
        
        [global::System.Diagnostics.CodeAnalysis.SuppressMessageAttribute("Microsoft.Performance", "CA1811:AvoidUncalledPrivateCode")]
        internal Resources() {
        }
        
        /// <summary>
        ///   Returns the cached ResourceManager instance used by this class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Resources.ResourceManager ResourceManager {
            get {
                if (object.ReferenceEquals(resourceMan, null)) {
                    global::System.Resources.ResourceManager temp = new global::System.Resources.ResourceManager("OblivionModManager.Properties.Resources", typeof(Resources).Assembly);
                    resourceMan = temp;
                }
                return resourceMan;
            }
        }
        
        /// <summary>
        ///   Overrides the current thread's CurrentUICulture property for all
        ///   resource lookups using this strongly typed resource class.
        /// </summary>
        [global::System.ComponentModel.EditorBrowsableAttribute(global::System.ComponentModel.EditorBrowsableState.Advanced)]
        internal static global::System.Globalization.CultureInfo Culture {
            get {
                return resourceCulture;
            }
            set {
                resourceCulture = value;
            }
        }
        
        internal static byte[] cSharp {
            get {
                object obj = ResourceManager.GetObject("cSharp", resourceCulture);
                return ((byte[])(obj));
            }
        }
        
        internal static byte[] IronPythonInterface {
            get {
                object obj = ResourceManager.GetObject("IronPythonInterface", resourceCulture);
                return ((byte[])(obj));
            }
        }
        
        /// <summary>
        ///   Looks up a localized string similar to &lt;?xml version=&quot;1.0&quot; encoding=&quot;utf-8&quot; ?&gt;
        ///&lt;xsd:schema xmlns:xsd=&quot;http://www.w3.org/2001/XMLSchema&quot; &gt;
        ///	&lt;xsd:annotation&gt;
        ///		&lt;xsd:documentation&gt;
        ///		This schema defines the syntax for mode definitions in SharpDevelop.
        ///		The schema can be simplified quite a bit but it does the job as is.
        ///		
        ///		
        ///		If you are using this file as a reference it is probably easiest to scroll to
        ///		the botton to find the definition of the root element called SyntaxDefinition and
        ///		then unwind the different type definitions and ref [rest of string was truncated]&quot;;.
        /// </summary>
        internal static string Mode {
            get {
                return ResourceManager.GetString("Mode", resourceCulture);
            }
        }
        
        internal static byte[] obmmScript {
            get {
                object obj = ResourceManager.GetObject("obmmScript", resourceCulture);
                return ((byte[])(obj));
            }
        }
        
        internal static byte[] python {
            get {
                object obj = ResourceManager.GetObject("python", resourceCulture);
                return ((byte[])(obj));
            }
        }
        
        internal static byte[] RightArrow {
            get {
                object obj = ResourceManager.GetObject("RightArrow", resourceCulture);
                return ((byte[])(obj));
            }
        }
        
        internal static byte[] vb {
            get {
                object obj = ResourceManager.GetObject("vb", resourceCulture);
                return ((byte[])(obj));
            }
        }
    }
}
