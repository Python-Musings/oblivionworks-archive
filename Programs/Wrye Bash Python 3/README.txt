This will be my work on getting Wrye Bash ported to Python 3.

I'm doing a seperate folder rather than a branch for a few reasons:

1) I plan on refactoring as I go, splitting the thousands of lines files
   into many small files.

2) Enough will change simply due to the changes in Python 3 that it won't
   be worth maintaining it as a branch.

3) Wrye Bash for Python 3 will have pretty strict requirements.  Mostly:
   a) wxPython - using wxPhoenix, which is still in dev, and cannot be
      downloaded as an installer.  wxPhoenix requires Python 3.2 (can't
      be higher).  I may try an experimental build at some point for
      Python 3.3, as the advantages of Python 3.3 over 3.2 are pretty good.
   b) py2exe - Pretty much non-existant for Python 3.  So I'll probably have
      to either port py2exe to Python 3, or try an alternate solution (freeze
      or something)


Requirements:
1) Python 3.2 (grab the 32-bit version - wxPhoenix is only available in 32-bit)

     http://www.python.org/download/releases/3.2.3/

2) wxPython Phoenix (grab the latest 3.2 version)

     http://wxpython.org/Phoenix/snapshot-builds/

3) pywin32 (grab latest 32-bit version for 3.2)

     https://sourceforge.net/projects/pywin32/files/pywin32/

Also recommended:
1) Python Launcher for Windows:  Allows you to have multiple version of Python
   installed, and a shebang line at the beginning determines which version
   of Python runs your script (not needed for Linux, already part of the shell).
   This is installed by default with Python 3.3, but not earlier.  Wrye Bash
   trunk and Python 3 versions both have shebang lines so they work with this.

   http://blog.python.org/2011/07/python-launcher-for-windows_11.html
