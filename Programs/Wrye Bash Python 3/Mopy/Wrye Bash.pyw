#!/usr/bin/env python3.2
# -*- coding: utf-8 -*-
#
# GPL License and Copyright Notice =============================================
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
# ==============================================================================

"""This module starts Wrye Bash in GUI mode."""

# Import Hooks -----------------------------------------------------------------
import sys
if not hasattr(sys,'frozen'):
    import imp,os
    # When a Python application/module is located in a directory with unicode
    # characters, this causes problems with imports.  The full path is encoded
    # to MBCS before passing to the filesystem, so any characters that do not
    # encode properly to MBCS make Python unable to find the path for importing.
    # This doesn't normally affect standard or 3rd party modules, but it does
    # affect any module provided with said application/module.  To work around
    # this, we install an import hook that accesses the appropriate file
    # directly and loads it into Python.
    # Currently this is only implemented for Python mode, not frozen apps.
    class UnicodeImporter(object):
        def find_module(self,fullname,path=None):
            fullname = fullname.replace('.','\\')
            if os.path.exists(fullname) and os.path.isdir(fullname):
                return self
            elif os.path.exists(fullname+'.py'):
                return self

        def load_module(self,fullname):
            if fullname in sys.modules:
                return sys.modules[fullname]
            else:
                sys.modules[fullname] = imp.new_module(fullname)
            filename = fullname.replace('.','\\')
            # File (module) import
            if os.path.exists(filename+'.py'):
                try:
                    with open(filename+'.py','U') as fp:
                        mod = imp.load_source(fullname,filename+'.py',fp)
                        sys.modules[fullname] = mod
                        mod.__loader__ = self
                        mod.__file__ = filename+'.py'
                        mod.__package__ = fullname.rpartition('.')[0]
                        return mod
                except Exception as e:
                    print('UnicodeImporter failed importing %s: %s' % (filename+'.py',e))
                    if fullname in sys.modules:
                          del sys.modules[fullname]
                    raise
            else:
                # Directory (package) import
                mod = sys.modules[fullname]
                mod.__loader__ = self
                mod.__file__ = filename
                mod.__name__ = filename
                mod.__path__ = [filename]
                mod.__package__ = fullname
                # __init__ file
                initfile = os.path.join(filename,'__init__.py')
                if os.path.exists(initfile):
                    with open(initfile,'U') as fp:
                        code = fp.read()
                    exec(code,mod.__dict__)
                return mod

    sys.meta_path.append(UnicodeImporter())

# Start Wrye Bash --------------------------------------------------------------
if __name__ == '__main__':
    from src import bash
    bash.main()
