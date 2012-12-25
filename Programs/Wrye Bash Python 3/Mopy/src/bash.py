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

def VerifyRequirements():
    """Check to see if all required dependancies are installed."""
    errors = []
    #--wxPython
    try:
        import wx
        haveWx = True
    except ImportError:
        errors.append(_('wxPython is required.  Get wxPhoenix: %(url)s')
                      % {'url':'http://wiki.wxpython.org/ProjectPhoenix'})
        haveWx = False
    #--Python 3.2
    if sys.version_info[0:2] != (3,2):
        errors.append(_('Python 3.2 is required.  Current version %(version)s')
                      % {'version':sys.version_info})
    #--pywin32
    try:
        import win32api
        version = win32api.GetFileVersionInfo(win32api.__file__,'\\')
        version = version['FileVersionLS'] >> 16
        if version < 218:
            errors.append(
                _('pywin32 218 is required.  Current version %(version)s')
                % {'version':version})
    except ImportError:
        errors.append(_('pywin32 218 is required.'))

    if errors:
        # First, try to show the error in the GUI
        msg = '\n\n'.join(errors)
        shown = False
        if haveWx:
            try:
                from src.balt import ShowError
                ShowError(None,msg,'Wrye Bash')
                shown = True
            except Exception as e:
                pass
        if not shown:
            # wx failed, try Tkinter
            try:
                import Tkinter
                root = Tkinter.Tk()
                frame = Tkinter.Frame(root)
                frame.pack()
                button = Tkinter.Button(frame,text=_('Quit'),fg='red',
                                        command=root.destroy,pady=15,borderwidth=15,
                                        relief=Tkinter.GROOVE)
                button.pack(fill=Tkinter.BOTH,expand=1,side=Tkinter.BOTTOM)
                text = Tkinter.Text(frame)
                text.insert(Tkinter.END,msg)
                text.config(state=Tkinter.DISABLED)
                text.pack()
                root.mainloop()
                shown = True
            except:
                pass
        if not shown:
            # even Tkinter failed
            print(msg)
        return False
    return True

def main():
    #--Setup translations
    from src.bolt import Translations
    Translations.Install()
    #--Check for dependencies
    if not VerifyRequirements():
        return
    #--Start wxApp
    import wx
    app = wx.App()
    #--Test for single instance
    from src.bolt import OneInstanceChecker
    if not OneInstanceChecker.Start():
        return
    del OneInstanceChecker
    #--Run the app!
    #  For now we're just using a dummy frame until we flesh this out
    frame = wx.Frame(None,wx.ID_ANY,_('Haha!'))
    frame.Show()
    app.MainLoop()