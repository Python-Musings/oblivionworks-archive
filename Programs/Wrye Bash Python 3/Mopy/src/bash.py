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

def main():
    #--Setup translations
    from src.bolt import Translations
    Translations.Install()
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