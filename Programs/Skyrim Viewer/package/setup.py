## Setup script for Skyrim Viewer

try:
    try:
        import py2exe.mf as modulefinder
    except ImportError:
        import modulefinder
    import win32com, sys
    for p in win32com.__path__[1:]:
        modulefinder.AddPackagePath('win32com', p)
    for extra in ['win32com.shell']:
        __import__(extra)
        m = sys.modules[extra]
        for p in m.__path__[1:]:
            modulefinder.AddPackagePath(extra, p)
except ImportError:
    pass

from distutils.core import setup
import py2exe, os, subprocess, platform

manifest = '''<assembly xmlns="urn:schemas-microsoft-com:asm.v1" manifestVersion="1.0">
  <trustInfo xmlns="urn:schemas-microsoft-com:asm.v3">
    <security>
      <requestedPrivileges>
        <requestedExecutionLevel
          level="asInvoker"
          uiAccess="false"
        />
      </requestedPrivileges>
    </security>
  </trustInfo>
  <dependency>
    <dependentAssembly>
      <assemblyIdentity
        type="win32"
        name="Microsoft.VC90.CRT"
        version="9.0.21022.8"
        processorArchitecture="*"
        publicKeyToken="1fc8b3b9a1e18e3b"
      />
    </dependentAssembly>
  </dependency>
  <dependency>
    <dependentAssembly>
      <assemblyIdentity
          type="win32"
          name="Microsoft.Windows.Common-Controls"
          version="6.0.0.0"
          processorArchitecture="*"
          publicKeyToken="6595b64144ccf1df"
          language="*"
      />
    </dependentAssembly>
  </dependency>
</assembly>
'''
RT_MANIFEST = 24

class Target:
    def __init__(self, script):
        self.name = 'Skyrim View'
        self.description = 'Skyrim View'
        self.version = '0.3.5'
        self.author = 'Lojack'
        self.url = 'http://www.skyrimnexus.com/downloads/file.php?id=165'
        self.download_url = self.url
        self.script = script
        if manifest is not None:
            self.other_resources = [(RT_MANIFEST, 1, manifest)]
windows = Target('Skyrim Viewer.pyw')

if '64bit' in platform.architecture():
    bundle = 3
else:
    bundle = 2

setup(
    windows = [windows],
    options = {
        'py2exe': {
            'dll_excludes': [
                # Make sure the MSVC 2008 Redist doesn't get included.
                'MSVCP90.dll',
                'MSVCR90.dll',
                'MSVCM90.dll',
                'mswsock.dll',  # Prevent x64 versions (for win32api)
                'powrprof.dll', # Prevent x64 versions (for win32api)
                ],
            #'excludes': [
            #    '_ssl',     # Suggested in the py2exe tutorial
            #    'doctest',  # Suggested in the py2exe tutorial
            #    'pdb',      # Suggested in the py2exe tutorial
            #    'unittest', # Suggested in the py2exe tutorial
            #    'difflib',  # Suggested in the py2exe tutorial
            #    'pywin',                # Suggested in the wxPython example
            #    'pywin.debugger',       # Suggested in the wxPython example
            #    'pywin.debugger.dbgcon',# Suggested in the wxPython example
            #    'pywin.dialogs',        # Suggested in the wxPython example
            #    'pywin.dialogs.list',   # Suggested in the wxPython example
            #    ],
            'ignores': [
                'psyco'
                ],
            'bundle_files': bundle, # 1 = bundle in the exe
            'optimize': 2,          # 2 = full code optimization
            'compressed': True,     # Compress the data files
            },
        },
    zipfile = None,     # Don't include data files in a zip along side
    )