# GPL License and Copyright Notice ============================================
#  This file is part of Wrye Bash.
#
#  Wrye Bash is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  Wrye Bash is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Wrye Bash; if not, write to the Free Software Foundation,
#  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
#  Wrye Bash copyright (C) 2005, 2006, 2007, 2008, 2009 Wrye
#
# =============================================================================

"""This module starts the Wrye Bash application. Basically, it runs some
initialization functions, and then starts the main application loop.

bash [-o OblivionPath] [-u userPath] [-p personalPath] [-l localAppDataPath] [-d] [0]
----
For all arguments:
Note that Python reads the backslash "\" as an escape character,
(that is, the backslash itself is ignored and the following character is read literally)
so for any paths you'll want to either use two backslashes (C:\\Folder\\)
or a forwardslash (C:/Folder/).

All arguments except the -d Debug can be set in the .ini file.
Arguments have precedence over ini settings.
You can use a mix of arguments and ini settings.
Ini settings don't require a double backslash and can have relative paths.
----
Oblivion directory argument (-o).
-o OblivionPath: Specify Oblivion directory (containing Oblivion.exe).
Use this argument if Bash is located outside of the Oblivion directory.
Example: -o "C:\\Games\\Oblivion\\"
----
User directory arguments (-u, -p, and -l).
These arguments allow you to specify your user directories in several ways. These
are only useful if the regular procedure for getting the user directory fails.
And even in that case, the user is probably better off installing win32com.
However, the arguments are:

-u userPath: Specify the user profile path. May help if HOMEDRIVE and/or HOMEPATH
are missing from the user's environgment.
Example: -u "C:\\Documents and Settings\\Wrye"

-p personalPath: Specify the user's personal directory.
If you need to set this then you probably need to set -l too.
Example: -p "C:\\Documents and Settings\\Wrye\\My Documents"

-l localAppDataPath: Specify the user's local application data directory. 
If you need to set this then you probably need to set -p too.
Example: -l "C:\\Documents and Settings\\Wrye\\Local Settings\\Application Data"
----
Debug argument:
-d Send debug text to the console rather than to a newly created debug window.
Useful if bash is crashing on startup or if you want to print a lot of
information (e.g. while developing or debugging).
"""

# Imports ---------------------------------------------------------------------
import getopt
import os
import sys
if sys.version[:3] == '2.4':
    import wxversion
    wxversion.select("2.5.3.1")
import barg
import bosh

#--Parse arguments
opts, args = barg.ParseArgs()

#--Initialize Directories and some settings
#  required before the rest has imported
opts = dict(optlist)
barg.SetUserPath('bash.ini',opts.get('-u'))
personal = opts.get('-p')
localAppData = opts.get('-l')
oblivionPath = opts.get('-o')

bosh.initBosh(personal,localAppData,oblivionPath)
pidpath = bosh.dirs['mopy'].join('pidfile.tmp')
import basher
import bolt
import atexit

def exit():
    pidpath.remove()

def oneInstanceChecker():
    if not os.path.isfile(pidpath.s):
        pidfile = pidpath.open('w')
        pidfile.write(str(os.getpid()))
        return True
    processlist = bosh.dirs['mopy'].join('temp.tmp')
    pidfile = pidpath.open('r')
    pidlist = pidfile.readlines()
    os.system(r'tasklist > "%s"' % processlist.s)
    processlistfile = processlist.open('r')
    processlistcontents = processlistfile.readlines()
    for pid in pidlist:
        for line in processlistcontents:
            if pid in line:
                print 'already started'
                return False
    pidfile.close()
    pidpath.open('w').write(' %s ' % str(os.getpid()))
    processlistfile.close()
    processlist.remove()
    return True
    
# Main ------------------------------------------------------------------------
def main():
    #import warnings
    #warnings.filterwarnings('error')
    #--More Initialization
    if not oneInstanceChecker():
        return False
    atexit.register(exit)
    basher.InitSettings()
    basher.InitLinks()
    basher.InitImages()
    #--Start application
    if '-d' in opts or (args and args[0] == '0'):
        app = basher.BashApp(False)
        bolt.deprintOn = True
    else:
        app = basher.BashApp()

    app.Init()
    app.MainLoop()

if __name__ == '__main__':
    try:
        args = sys.argv[1:]
        if '-d' not in args and '0' not in args:
            import psyco
            psyco.full()
    except:
        pass
    main()
