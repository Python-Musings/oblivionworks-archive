# -*- coding: utf-8 -*-
# src/balt/MessageDialogs.py
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

"""This modules implements common modal dialogs used for showing information,
   warnings, and errors."""

# Imports ----------------------------------------------------------------------
import wx
from . import Vista
from src.bolt.Regex import reURL,reURLCode

def VistaDialog(parent,message,title,buttons=[],checkBox=None,icon=None,commandLinks=True,footer='',expander=[],heading=''):
    heading = heading if heading is not None else title
    title = title if heading is not None else 'Wrye Bash'
    dialog = Vista.TaskDialog(title,heading,message,
                              buttons=[x[1] for x in buttons],
                              icon=icon,
                              parenthwnd=parent.GetHandle() if parent else None)
    dialog.bind(Vista.HYPERLINK_CLICKED,Vista.StartURLCallback)
    if footer:
        dialog.set_footer(footer)
    if expander:
        dialog.set_expander(expander,not isinstance(expander,list),not footer)
    if checkBox:
        if isinstance(checkBox,str):
            dialog.set_check_box(checkBox,False)
        else:
            dialog.set_check_box(checkBox[0],checkBox[1])
    result = dialog.show(commandLinks)
    for id,title in buttons:
        if title.startswith('+'): title = title[1:]
        if title == result[0]:
            if checkBox:
                return (id,result[2])
            else:
                return id
    return (None,result[2])

def AskStyled(parent,message,title,style,**kwdargs):
    """Shows a modal MessageDialog"""
    urls = []
    parseURLs = kwdargs.pop('parseURLs',False)
    matches = reURLCode.findall(message)
    for text,url in matches:
        full = '[['+text+'|'+url+']]'
        if parseURLs:
            urls.append((text,url))
        message = message.replace(full,url)
    if parseURLs and not urls:
        matches = reURL.findall(message)
        for url in matches:
            urls.append((url,url))
    if Vista.Available:
        footer = []
        for url in urls:
            footer.append('<A href="%(url)s">%(title)s</A>' % {'url':url[1],
                                                               'title':url[0]})
        if footer:
            footer = (_('Show links'),
                      _('Hide links'),
                      '\n'.join(footer))
        buttons = []
        icon = None
        if style & wx.YES_NO:
            yes = 'yes'
            no = 'no'
            if style & wx.YES_DEFAULT:
                yes = 'Yes'
            elif style & wx.NO_DEFAULT:
                no = 'No'
            buttons.append((wx.ID_YES,yes))
            buttons.append((wx.ID_NO,no))
        if style & wx.OK:
            buttons.append((wx.ID_OK,'ok'))
        if style & wx.CANCEL:
            buttons.append((wx.ID_CANCEL,'cancel'))
        if style & (wx.ICON_HAND|wx.ICON_EXCLAMATION):
            icon = 'error'
        result = VistaDialog(parent,
                             message=message,
                             title=title,
                             icon=icon,
                             buttons=buttons,
                             expander=footer,
                             **kwdargs)
    else:
        dialog = wx.MessageDialog(parent,message,title,style)
        result = dialog.ShowModal()
        dialog.Destroy()
    return result in (wx.ID_OK,wx.ID_YES)

def AskOk(parent,message,title='',**kwdargs):
    """Shows a modal Ok message."""
    return AskStyled(parent,message,title,wx.OK|wx.CANCEL,**kwdargs)

def AskYes(parent,message,title='',default=True,icon=wx.ICON_EXCLAMATION,**kwdargs):
    """Shows a modal warning or question message."""
    style = wx.YES_NO|icon|(wx.YES_DEFAULT if default else wx.NO_DEFAULT)
    return AskStyled(parent,message,title,style,**kwdargs)

def AskWarning(parent,message,title='Warning',**kwdargs):
    """Shows a modal warning message."""
    return AskStyled(parent,message,title,wx.OK|wx.CANCEL|wx.ICON_EXCLAMATION,**kwdargs)

def ShowOk(parent,message,title='',**kwdargs):
    """Shows a modal error message."""
    return AskStyled(parent,message,title,wx.OK,**kwdargs)

def ShowError(parent,message,title='Error',**kwdargs):
    """Shows a modal error message."""
    return AskStyled(parent,message,title,wx.OK|wx.ICON_HAND,**kwdargs)
