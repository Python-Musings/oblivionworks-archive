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

"""This module does the real working of starting Wrye Bash."""

# Imports ----------------------------------------------------------------------
#--Standard
import os
import sys
import builtins
import traceback
#--Local
import src.barg as barg
import src.bass as bass

# Install dummy translation function
builtins.__dict__['_'] = lambda x: x

def ErrorMessage(message):
    """Show an error message using GUI if possible."""
    try:
        # Try wx first
        import wx
        from src.balt import ShowError
        ShowError(None,message,'Wrye Bash - Error',parseURLs=True)
    except:
        # That failed, try Tkinter
        try:
            import tkinter
            import tkinter.ttk as ttk
            root = tkinter.Tk()
            root.title('Wrye Bash')
            frame = ttk.Frame(root,padding='3 3 3 3')
            frame.pack()
            style = ttk.Style()
            style.configure('TButton')
            button = ttk.Button(text=_('Quit'),command=root.destroy)
            button.pack()#side=tkinter.BOTTOM)
            text = tkinter.Text(frame,wrap='word')
            text.insert(1.0,msg)
            text.config(state=tkinter.DISABLED)
            text.pack()
            root.mainloop()
        except:
            # That failed, print to console
            print('Error:', message)


def VerifyRequirements():
    """Check to see if all required dependencies are installed."""
    errors = []
    #--wxPython
    try:
        import wx
        haveWx = True
    except ImportError:
        errors.append(_('wxPython is required.  None detected.  Get it from:') +
                      '\n    http://wiki.wxpython.org/ProjectPhoenix')
        haveWx = False
    #--Python 3.2
    if sys.version_info[0:2] != (3,2):
        url = 'http://www.python.org/download/releases/3.2.3/'
        if haveWx:
            url = '[[Python 3.2|'+url+']]'
        errors.append(
            (_('Python 3.2 is required.  Current version is %(version)s.  Get it from:')
             % {'version':'.'.join(map(str,sys.version_info[0:3]))})
            + '\n    %s' % url)
    #--pywin32
    url = 'https://sourceforge.net/projects/pywin32/files/pywin32/'
    if haveWx:
        url = '[[pywin32 218|'+url+']]'
    try:
        import win32api
        version = win32api.GetFileVersionInfo(win32api.__file__,'\\')
        version = version['FileVersionLS'] >> 16
        if version < 218:
            errors.append(
                (_('pywin32 218 is required.  Current version is %(version)s.  Get it from:')
                 % {'version':version})
                + '\n    %s' % url)
    except ImportError:
        errors.append(_('pywin32 218 is required.  None detected.  Get it from:')
                      + '\n    %s' % url)

    if errors:
        msg = (_('Cannot start Wrye Bash!  Please ensure that Python dependencies are installed correctly.')
               + '\n\n' +
               '\n\n'.join(errors))
        ErrorMessage(msg)
        return False
    return True

def main():
    try:
        #--Parse command line
        barg.parse()
        #--Initialize directories
        import src.dirs
        src.dirs.InitDirs()
        #--Setup translations
        try:
            from src.bolt import Translations
            Translations.Install(pathRead=bass.dirs['l10n'],
                                 pathWrite=bass.dirs['l10n.compiled'])
        except Exception as e:
            # Translations fail.  Install a NULL Translations
            # _ function so at least we don't get errors there.
            # TODO: use logging to print
            pass
        #--Check for dependencies
        if not VerifyRequirements():
            return
        #--Start wxApp
        import wx
        app = wx.App()
        #--Test for single instance
        from src.bolt import OneInstanceChecker
        if bass.opts.portable:
            oicDir = None # Use defalt
        else:
            oicDir = bass.dirs['appdata'].s
        if not OneInstanceChecker.Start(oicDir):
            return
        del OneInstanceChecker
        #--Run the app!
        #  For now we're just using a dummy frame until we flesh this out
        frame = wx.Frame(None,wx.ID_ANY,_('Haha!'))
        frame.Show()
        frame.SetIcon(wx.Icon('bash.ico'))
        app.MainLoop()
    except Exception as e:
        #--Something bad happened, try to show it in GUI mode.
        import io
        o = io.StringIO()
        o.write(_('Could not start Wrye Bash:'))
        o.write('\n\n')
        traceback.print_exc(file=o)
        ErrorMessage(o.getvalue())
        raise
