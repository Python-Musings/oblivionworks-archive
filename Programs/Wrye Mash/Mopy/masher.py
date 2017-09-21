# Imports ---------------------------------------------------------------------
#--Localization
#..Handled by mosh, so import that.
import mosh
from mosh import _
from mosh import AbstractError, ArgumentError, StateError, UncodedError
from mosh import formatInteger,formatDate
bosh = mosh #--Cheap compatibility for imported code.

#--Python
import cStringIO
import os
import re
import shutil
import string
import struct
import sys
import textwrap
import time
from types import *

import bolt
from bolt import LString,GPath, SubProgress

#--wxPython
import wx
from wx.lib.mixins.listctrl import ListCtrlAutoWidthMixin
from wx.lib.evtmgr import eventManager

#--Balt
import balt
from balt import tooltip, fill, bell
from balt import bitmapButton, button, toggleButton, checkBox, staticText, spinCtrl
from balt import leftSash, topSash
from balt import spacer, hSizer, vSizer, hsbSizer, vsbSizer
from balt import colors, images, Image
from balt import Links, Link, SeparatorLink, MenuLink

#--Internet Explorer 
#  - Make sure that python root directory is in PATH, so can access dll's.
if sys.prefix not in set(os.environ['PATH'].split(';')):
    os.environ['PATH'] += ';'+sys.prefix
import wx.lib.iewin

# Singletons ------------------------------------------------------------------
statusBar = None
modList = None
modDetails = None
saveList = None
saveDetails = None
mashFrame = None
docBrowser = None
journalBrowser = None
helpBrowser = None
gInstallers = None

# Settings --------------------------------------------------------------------
settings = None

#--Load config/defaults
settingDefaults = {
    #--Morrowind Directory
    'mwDir': r'C:\Program Files\Bethesda Softworks\Morrowind',
    #--Wrye Mash
    'mash.version': 0,
    'mash.readme': (0,'0'),
    'mash.framePos': (-1,-1),
    'mash.frameSize': (600,500),
    'mash.frameSize.min': (400,500),
    'mash.page':0,
    #--Wrye Mash: Windows
    'mash.window.sizes': {},
    #--Wrye Mash: Load Lists
    'mash.loadLists.data': {
        'Bethesda ESMs': [
            'Morrowind.esm',
            'Tribunal.esm',
            'Bloodmoon.esm',
            ],
        },
    #--Wrye Mash: Statistics
    'mash.fileStats.cols': ['Type','Count','Size'],
    'mash.fileStats.sort': 'Type',
    'mash.fileStats.colReverse': {
        'Count':1,
        'Size':1,
        },
    'mash.fileStats.colWidths': {
        'Type':50,
        'Count':50,
        'Size':75,
        },
    'mash.fileStats.colAligns': {
        'Count':1,
        'Size':1,
        },
    #--Installers
    'bash.installers.page':0,
    'bash.installers.enabled': True,
    'bash.installers.autoAnneal': True,
    'bash.installers.fastStart': True,
    'bash.installers.removeEmptyDirs':True,
    'bash.installers.skipDistantLOD':False,
    'bash.installers.sortProjects':True,
    'bash.installers.sortActive':False,
    'bash.installers.sortStructure':False,
    'bash.installers.conflictsReport.showLower':True,
    'bash.installers.conflictsReport.showInactive':False,
    #--Wrye Bash: Screens
    'bash.screens.cols': ['File'],
    'bash.screens.sort': 'File',
    'bash.screens.colReverse': {
        'Modified':1,
        },
    'bash.screens.colWidths': {
        'File':150,
        'Modified':150,
        'Size':75,
        },
    'bash.screens.colAligns': {},
    #--Wrye Mash: Group and Rating
    'mash.mods.groups': ['Body','Bethesda','Clothes','Creature','Fix','Last','Test','Game','GFX','Location','Misc.','NPC','Quest','Race','Resource','Sound'],
    'mash.mods.ratings': ['+','1','2','3','4','5','=','~'],
    #--Wrye Mash: RefRemovers
    'mash.refRemovers.data': {
        },
    'mash.refRemovers.safeCells': [
        _("Balmora, Caius Cosades' House"),
        _("Indarys Manor"),
        _("Raven Rock, Factor's Estate"),
        _("Rethan Manor"),
        _("Skaal Village, The Blodskaal's House"),
        _("Solstheim, Thirsk"),
        _("Tel Uvirith, Tower Lower"),
        _("Tel Uvirith, Tower Upper"),
        ],
    #--Wrye Mash: RefReplacers
    'mash.refReplacers.data': {
        },
    #--Wrye Mash: Col (Sort) Names
    'mash.colNames': {
        'Author': _('Author'),
        'Cell': _('Cell'),
        'Count': _('Count'),
        'Day': _('Day'),
        'File': _('File'),
        'Rating': _('Rating'),
        'Group': _('Group'),
        'Load Order': _('Load Order'),
        'Modified': _('Modified'),
        'Num': _('Num'),
        'Player': _('Player'),
        'Rating':_('Rating'),
        'Save Name': _('Save Name'),
        'Size': _('Size'),
        'Status': _('Status'),
        'Type': _('Type'),
        'Version': _('Version'),
        },
    #--Wrye Mash: Masters
    'mash.masters.cols': ['File','Num'],
    'mash.masters.esmsFirst': 1,
    'mash.masters.selectedFirst': 0,
    'mash.masters.sort': 'Load Order',
    'mash.masters.colReverse': {},
    'mash.masters.colWidths': {
        'File':80,
        'Num':20,
        },
    'mash.masters.colAligns': {
        'Num':1,
        },
    #--Wrye Mash: Mod Notes
    'mash.modNotes.show': False,
    'mash.modNotes.size': (200,300),
    'mash.modNotes.pos': wx.DefaultPosition,
    #--Wrye Mash: Mod Docs
    'mash.modDocs.show': False,
    'mash.modDocs.size': (300,400),
    'mash.modDocs.pos': wx.DefaultPosition,
    'mash.modDocs.dir': None,
    #--Wrye Mash: Mods
    'mash.mods.cols': ['File','Rating','Group','Modified','Size','Author'],
    'mash.mods.esmsFirst': 1,
    'mash.mods.selectedFirst': 0,
    'mash.mods.sort': 'File',
    'mash.mods.colReverse': {},
    'mash.mods.colWidths': {
        'File':200,
        'Rating':20,
        'Group':20,
        'Rating':20,
        'Modified':150,
        'Size':75,
        'Author':100,
        },
    'mash.mods.colAligns': {
        'Size':1,
        },
    'mash.mods.renames': {},
    #--Wrye Mash: Journal
    'mash.journal.show': False,
    'mash.journal.size': (300,400),
    'mash.journal.pos': wx.DefaultPosition,
    #--Wrye Mash: Save Sets
    'mash.saves.sets': [],
    #--Wrye Mash: Saves
    'mash.saves.cols': ['File','Modified','Size','Save Name','Player','Cell'],
    'mash.saves.sort': 'Modified',
    'mash.saves.colReverse': {
        'Modified':1,
        },
    'mash.saves.colWidths': {
        'File':150,
        'Modified':150,
        'Size':75,
        'Save Name':100,
        'Player':100,
        'Cell':150,
        'Day':30,
        },
    'mash.saves.colAligns': {
        'Size':1,
        },
    #--Wrye Mash: World Map Reapir
    'mash.worldMap.gridLines':True,
    }

#InitSettings()
def InitSettings():
# Initialization fo mosh things--------------------------------------------------------------
    """Initialize settings (configuration store). First, read from file, then
    load defaults (defaults will not overwrite items extracted from file)."""
    mosh.initSettings()
    global settings
    settings = mosh.settings
    settings.loadDefaults(settingDefaults)
    mosh.initDirs()
# Exceptions ------------------------------------------------------------------

class MashError(mosh.MoshError):
    pass

class InterfaceError(MashError):
    pass

# Gui Ids ---------------------------------------------------------------------
#------------------------------------------------------------------------------
class IdListIterator:
    """Iterator for IdList object."""
    def __init__(self, idList):
        """Initialize."""
        self.idList = idList
        self.prevId = idList.baseId - 1
        self.lastId = idList.baseId + idList.size - 1

    def __iter__(self):
        """Iterator method."""
        return self
    
    def next(self):
        """Iterator method."""
        if self.prevId >= self.lastId:
            raise StopIteration
        self.prevId += 1
        return self.prevId

#------------------------------------------------------------------------------
class IdList:
    """List of ids."""
    def __init__(self,baseId,size,*extras):
        self.BASE = baseId
        self.MAX = baseId + size -1
        self.baseId = baseId
        self.size = size
        #--Extra
        nextId = baseId + size
        for extra in extras:
            setattr(self,extra,nextId)
            nextId += 1

    def __iter__(self):
        """Return iterator."""
        return IdListIterator(self)

#------------------------------------------------------------------------------
# Constants
#--Indexed
wxListAligns = [wx.LIST_FORMAT_LEFT, wx.LIST_FORMAT_RIGHT, wx.LIST_FORMAT_CENTRE]

#--Generic
ID_RENAME = 6000
ID_SET    = 6001
ID_SELECT = 6002
ID_BROWSER = 6003
ID_NOTES  = 6004
ID_EDIT   = 6005
ID_BACK   = 6006
ID_NEXT   = 6007

#--File Menu
ID_REVERT_BACKUP = 6100
ID_REVERT_FIRST  = 6101
ID_BACKUP_NOW    = 6102

#--Label Menus
ID_LOADERS   = IdList(10000,90,'SAVE','EDIT','NONE') 
ID_REMOVERS  = IdList(10100,90,'EDIT','EDIT_CELLS')
ID_REPLACERS = IdList(10200,90,'EDIT')
ID_GROUPS    = IdList(10300,90,'EDIT','NONE')
ID_RATINGS   = IdList(10400,90,'EDIT','NONE')
ID_PROFILES  = IdList(10500,90,'EDIT')

# Message Dialogs -------------------------------------------------------------
def Message(parent,message,title='',style=wx.OK):
    """Shows a modal MessageDialog. 
    Use ErrorMessage, WarningMessage or InfoMessage."""
    dialog = wx.MessageDialog(parent,message,title,style)
    result = dialog.ShowModal()
    dialog.Destroy()
    return result

def ErrorMessage(parent,message,title=_('Error'),style=(wx.OK|wx.ICON_HAND)):
    """Shows a modal error message."""
    return Message(parent,message,title,style)

def WarningMessage(parent,message,title=_('Warning'),style=(wx.OK|wx.ICON_EXCLAMATION)):
    """Shows a modal warning message."""
    return Message(parent,message,title,style)
    
def WarningQuery(parent,message,title='',style=(wx.YES_NO|wx.ICON_EXCLAMATION)):
    """Shows a modal warning message."""
    return Message(parent,message,title,style)
    
def InfoMessage(parent,message,title=_('Information'),style=(wx.OK|wx.ICON_INFORMATION)):
    """Shows a modal information message."""
    return Message(parent,message,title,style)

def LogMessage(parent,message,logText,title='',style=0,asDialog=True):
    #--Query Dialog
    pos = settings.get('mash.message.log.pos',wx.DefaultPosition)
    size = settings.get('mash.message.log.size',(400,400))
    if asDialog:
        window = wx.Dialog(parent,-1,title,pos=pos,size=size,
            style=wx.DEFAULT_DIALOG_STYLE|wx.RESIZE_BORDER)
    else:
        window = wx.Frame(parent,-1,title,pos=pos,size=(200,300),
            style= (wx.RESIZE_BORDER | wx.CAPTION | wx.SYSTEM_MENU | wx.CLOSE_BOX | wx.CLIP_CHILDREN))
        window.SetIcons(images['mash.icons2'].GetIconBundle())
    window.SetSizeHints(200,200)
    sizer = wx.BoxSizer(wx.VERTICAL)
    if message:
        sizer.Add(wx.StaticText(window,-1,message),0,wx.EXPAND|wx.LEFT|wx.RIGHT|wx.TOP,6)
    textCtrl = wx.TextCtrl(window,-1,logText,style=wx.TE_READONLY|wx.TE_MULTILINE)
    sizer.Add(textCtrl,1,wx.EXPAND)#|wx.ALL,6)
    window.SetSizer(sizer)
    if asDialog:
        window.ShowModal()
        #--Done
        settings['mash.message.log.pos'] = window.GetPosition()
        settings['mash.message.log.size'] = window.GetSizeTuple()
        window.Destroy()
    else:
        window.Show()

def ContinueQuery(parent,message,continueKey,title=_('Warning')):
    """Shows a modal continue query if value of continueKey is false. Returns True to continue.
    Also provides checkbox "Don't show this in future." to set continueKey to true."""
    #--ContinueKey set?
    if settings.get(continueKey): return wx.ID_OK
    #--Generate/show dialog
    dialog = wx.Dialog(parent,-1,title,size=(350,200),style=wx.DEFAULT_DIALOG_STYLE|wx.RESIZE_BORDER)
    sizer = wx.BoxSizer(wx.VERTICAL)
    staticText = wx.StaticText(dialog,-1,message,style=wx.ST_NO_AUTORESIZE)
    sizer.Add(staticText,1,wx.EXPAND|wx.ALL,6)
    checkBox = wx.CheckBox(dialog,-1,_("Don't show this in the future."))
    sizer.Add(checkBox,0,wx.EXPAND|wx.LEFT|wx.RIGHT|wx.BOTTOM,6)
    #--Save/Cancel
    sizer_h1 = wx.BoxSizer(wx.HORIZONTAL)
    sizer_h1.Add((0,0),1)
    sizer_h1.Add(wx.Button(dialog,wx.ID_OK))
    sizer_h1.Add(wx.Button(dialog,wx.ID_CANCEL),0,wx.LEFT,4)
    sizer.Add(sizer_h1,0,wx.EXPAND|wx.LEFT|wx.RIGHT|wx.BOTTOM,6)
    dialog.SetSizer(sizer)
    #--Get continue key setting and return
    result = dialog.ShowModal()
    if checkBox.GetValue():
        settings[continueKey] = 1
    return result

def DirDialog(parent,message=_('Choose a directory.'),defaultPath=''):
    """Shows a modal directory dialog and return the resulting path, or None if canceled."""
    dialog = wx.DirDialog(parent,message,defaultPath,style=wx.DD_NEW_DIR_BUTTON)
    if dialog.ShowModal() != wx.ID_OK:
        dialog.Destroy()
        return None
    else:
        path = dialog.GetPath()
        dialog.Destroy()
        return path

def TextEntry(parent,message,default=''):
    """Shows a text entry dialog and returns result or None if canceled."""
    dialog = wx.TextEntryDialog(parent,message,default)
    if dialog.ShowModal() != wx.ID_OK:
        dialog.Destroy()
        return None
    else:
        value = dialog.GetValue()
        dialog.Destroy()
        return value

#------------------------------------------------------------------------------
class ProgressDialog(mosh.Progress):
    """Prints progress to file (stdout by default)."""
    def __init__(self,title=_('Progress'),message='',parent=None,
        style=wx.PD_APP_MODAL|wx.PD_ELAPSED_TIME,interval=0.1):
        self.dialog = wx.ProgressDialog(title,message,100,parent,style)
        mosh.Progress.__init__(self,interval)
        self.isDestroyed = False

    def doProgress(self,progress,message):
        if self.dialog:
            self.dialog.Update(int(progress*100),message)
        else:
            raise InterfaceError,_('Dialog already destroyed.')

    def Destroy(self):
        if self.dialog:
            self.dialog.Destroy()
            self.dialog = None

# Images ----------------------------------------------------------------------
images = {}

#------------------------------------------------------------------------------
class Checkboxes(balt.ImageList):
    """Checkboxes ImageList. Used by several List classes."""
    def __init__(self):
        balt.ImageList.__init__(self,16,16)
        for status in ('on','off'):
            for color in ('purple','blue','green','orange','yellow','red'):
                shortKey = color+'.'+status
                imageKey = 'checkbox.'+shortKey
                file = r'images\checkbox_'+color+'_'+status+'.png'
                image = images[imageKey] = Image(file,wx.BITMAP_TYPE_PNG)
                self.Add(image,shortKey)

    def Get(self,status,on):
        self.GetImageList()
        if on:
            if status   <= -20: shortKey = 'purple.on'
            elif status <= -10: shortKey = 'blue.on'
            elif status <= 0: shortKey = 'green.on'
            elif status <=10: shortKey = 'yellow.on'
            elif status <=20: shortKey = 'orange.on'
            else: shortKey = 'red.on'
        else:
            if status   <= -20: shortKey = 'purple.off'
            elif status <= -10: shortKey = 'blue.off'
            elif status == 0: shortKey = 'green.off'
            elif status <=10: shortKey = 'yellow.off'
            elif status <=20: shortKey = 'orange.off'
            else: shortKey = 'red.off'
        return self.indices[shortKey]

# Icons------------------------------------------------------------------------
installercons = balt.ImageList(16,16)
installercons.data.extend({
    #--Off/Archive
    'off.green':  Image(r'images/checkbox_green_off.png',wx.BITMAP_TYPE_PNG),
    'off.grey':   Image(r'images/checkbox_grey_off.png',wx.BITMAP_TYPE_PNG),
    'off.red':    Image(r'images/checkbox_red_off.png',wx.BITMAP_TYPE_PNG),
    'off.white':  Image(r'images/checkbox_white_off.png',wx.BITMAP_TYPE_PNG),
    'off.orange': Image(r'images/checkbox_orange_off.png',wx.BITMAP_TYPE_PNG),
    'off.yellow': Image(r'images/checkbox_yellow_off.png',wx.BITMAP_TYPE_PNG),
    #--On/Archive
    'on.green':  Image(r'images/checkbox_green_inc.png',wx.BITMAP_TYPE_PNG),
    'on.grey':   Image(r'images/checkbox_grey_inc.png',wx.BITMAP_TYPE_PNG),
    'on.red':    Image(r'images/checkbox_red_inc.png',wx.BITMAP_TYPE_PNG),
    'on.white':  Image(r'images/checkbox_white_inc.png',wx.BITMAP_TYPE_PNG),
    'on.orange': Image(r'images/checkbox_orange_inc.png',wx.BITMAP_TYPE_PNG),
    'on.yellow': Image(r'images/checkbox_yellow_inc.png',wx.BITMAP_TYPE_PNG),
    #--Off/Directory
    'off.green.dir':  Image(r'images/diamond_green_off.png',wx.BITMAP_TYPE_PNG),
    'off.grey.dir':   Image(r'images/diamond_grey_off.png',wx.BITMAP_TYPE_PNG),
    'off.red.dir':    Image(r'images/diamond_red_off.png',wx.BITMAP_TYPE_PNG),
    'off.white.dir':  Image(r'images/diamond_white_off.png',wx.BITMAP_TYPE_PNG),
    'off.orange.dir': Image(r'images/diamond_orange_off.png',wx.BITMAP_TYPE_PNG),
    'off.yellow.dir': Image(r'images/diamond_yellow_off.png',wx.BITMAP_TYPE_PNG),
    #--On/Directory
    'on.green.dir':  Image(r'images/diamond_green_inc.png',wx.BITMAP_TYPE_PNG),
    'on.grey.dir':   Image(r'images/diamond_grey_inc.png',wx.BITMAP_TYPE_PNG),
    'on.red.dir':    Image(r'images/diamond_red_inc.png',wx.BITMAP_TYPE_PNG),
    'on.white.dir':  Image(r'images/diamond_white_inc.png',wx.BITMAP_TYPE_PNG),
    'on.orange.dir': Image(r'images/diamond_orange_inc.png',wx.BITMAP_TYPE_PNG),
    'on.yellow.dir': Image(r'images/diamond_yellow_inc.png',wx.BITMAP_TYPE_PNG),
    #--Broken
    'corrupt':   Image(r'images/red_x.png',wx.BITMAP_TYPE_PNG),
    }.items())
# Windows ---------------------------------------------------------------------
#------------------------------------------------------------------------------
class NotebookPanel(wx.Panel):
    """Parent class for notebook panels."""

    def SetStatusCount(self):
        """Sets status bar count field."""
        statusBar.SetStatusText('',2)

    def OnShow(self):
        """To be called when particular panel is changed to and/or shown for first time.
        Default version does nothing, but derived versions might update data."""
        self.SetStatusCount()

    def OnCloseWindow(self):
        """To be called when containing frame is closing. Use for saving data, scrollpos, etc."""
        pass

#------------------------------------------------------------------------------
class SashTankPanel(NotebookPanel):
    """Subclass of a notebook panel designed for a two pane tank panel."""
    def __init__(self,data,parent):
        """Initialize."""
        wx.Panel.__init__(self, parent,-1)
        self.data = data
        self.detailsItem = None
        sashPos = data.getParam('sashPos',200)
        self.left = leftSash(self,defaultSize=(sashPos,100),onSashDrag=self.OnSashDrag)
        self.right = wx.Panel(self,style=wx.NO_BORDER)
        #--Events
        self.Bind(wx.EVT_SIZE,self.OnSize)

    def OnShow(self):
        """Panel is shown. Update self.data."""
        if self.gList.data.refresh():
            self.gList.RefreshUI()
        self.SetStatusCount()

    def OnSashDrag(self,event):
        """Handle sash moved."""
        wMin,wMax = 80,self.GetSizeTuple()[0]-80
        sashPos = max(wMin,min(wMax,event.GetDragRect().width))
        self.left.SetDefaultSize((sashPos,10))
        wx.LayoutAlgorithm().LayoutWindow(self, self.right)
        self.data.setParam('sashPos',sashPos)

    def OnSize(self,event=None):
        wx.LayoutAlgorithm().LayoutWindow(self, self.right)

    def OnCloseWindow(self):
        """To be called when containing frame is closing. Use for saving data, scrollpos, etc."""
        self.SaveDetails()
        self.data.save()

    def GetDetailsItem(self):
        """Returns item currently being shown in details view."""
        return self.detailsItem

#------------------------------------------------------------------------------
class ListEditorData:
    """Data capsule for ListEditorDialog. [Abstract]"""
    def __init__(self,parent):
        """Initialize."""
        self.parent = parent #--Parent window.
        self.showAdd = False
        self.showEdit = False
        self.showRename = False
        self.showRemove = False
    def getItemList(self):
        """Returns item list in correct order."""
        raise mosh.AbstractError
        return []
    def add(self):
        """Peforms add operation. Return new item on success."""
        raise mosh.AbstractError
        return None
    def edit(self,item=None):
        """Edits specified item. Return true on success."""
        raise mosh.AbstractError
        return False
    def rename(self,oldItem,newItem):
        """Renames oldItem to newItem. Return true on success."""
        raise mosh.AbstractError
        return False
    def remove(self,item):
        """Removes item. Return true on success."""
        raise mosh.AbstractError
        return False
    #--Checklist
    def getChecks(self):
        """Returns checked state of items as array of True/False values matching Item list."""
        raise mosh.AbstractError
        return []
    def check(self,item):
        """Checks items. Return true on success."""
        raise mosh.AbstractError
        return False
    def uncheck(self,item):
        """Unchecks item. Return true on success."""
        raise mosh.AbstractError
        return False

#------------------------------------------------------------------------------
class ListEditorDialog(wx.Dialog):
    """Dialog for editing lists."""
    def __init__(self,parent,id,title,data,type='list'):
        #--Data
        self.data = data #--Should be subclass of ListEditorData
        self.items = data.getItemList()
        #--GUI
        wx.Dialog.__init__(self,parent,id,title,
            style=wx.DEFAULT_DIALOG_STYLE|wx.RESIZE_BORDER)
        wx.EVT_CLOSE(self, self.OnCloseWindow)
        #--List Box
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        if type == 'checklist':
            self.list = wx.CheckListBox(self,-1,choices=self.items,style=wx.LB_SINGLE)
            for index,checked in enumerate(self.data.getChecks()):
                self.list.Check(index,checked)
            self.Bind(wx.EVT_CHECKLISTBOX, self.DoCheck, self.list)
        else:
            self.list = wx.ListBox(self,-1,choices=self.items,style=wx.LB_SINGLE)
        self.list.SetSizeHints(125,150)
        sizer.Add(self.list,1,wx.EXPAND|wx.TOP,4)
        #--Buttons and Events
        if data.showAdd or data.showEdit or data.showRename or data.showRemove:
            sizer_v1 = wx.BoxSizer(wx.VERTICAL)
            if data.showAdd:
                sizer_v1.Add(wx.Button(self,wx.ID_NEW,_('Add')),0,wx.LEFT|wx.TOP,4)
                wx.EVT_BUTTON(self,wx.ID_NEW,self.DoAdd)
            if data.showEdit:
                sizer_v1.Add(wx.Button(self,wx.ID_REPLACE,_('Edit')),0,wx.LEFT|wx.TOP,4)
                wx.EVT_BUTTON(self,wx.ID_REPLACE,self.DoEdit)
            if data.showRename:
                sizer_v1.Add(wx.Button(self,ID_RENAME,_('Rename')),0,wx.LEFT|wx.TOP,4)
                wx.EVT_BUTTON(self,ID_RENAME,self.DoRename)
            if data.showRemove:
                sizer_v1.Add(wx.Button(self,wx.ID_DELETE,_('Remove')),0,wx.LEFT|wx.TOP,4)
                wx.EVT_BUTTON(self,wx.ID_DELETE,self.DoRemove)
            sizer.Add(sizer_v1,0,wx.EXPAND)
        #--Done
        if data.__class__ in settings['mash.window.sizes']:
            self.SetSizer(sizer)
            self.SetSize(settings['mash.window.sizes'][data.__class__])
        else:
            self.SetSizerAndFit(sizer)

    def GetSelected(self):
        return self.list.GetNextItem(-1,wx.LIST_NEXT_ALL,wx.LIST_STATE_SELECTED)

    #--Checklist commands
    def DoCheck(self,event):
        """Handles check/uncheck of listbox item."""
        index = event.GetSelection()
        item = self.items[index]
        if self.list.IsChecked(index):
            self.data.check(item)
        else:
            self.data.uncheck(item)
        #self.list.SetSelection(index)

    #--List Commands
    def DoAdd(self,event):
        """Adds a new item."""
        newItem = self.data.add()
        if newItem and newItem not in self.items:
            self.items = self.data.getItemList()
            index = self.items.index(newItem)
            self.list.InsertItems([newItem],index)

    def DoEdit(self,event):
        """Edits the selected item."""
        raise mosh.UncodedError

    def DoRename(self,event):
        """Renames selected item."""
        selections = self.list.GetSelections()
        if not selections:
            wx.Bell()
            return
        #--Rename it
        itemDex = selections[0]
        curName = self.list.GetString(itemDex)
        #--Dialog
        dialog = wx.TextEntryDialog(self,_('Rename to:'),_('Rename'),curName)
        result = dialog.ShowModal()
        #--Okay?
        if result != wx.ID_OK:
            dialog.Destroy()
            return
        newName = dialog.GetValue()
        dialog.Destroy()
        if newName == curName:
            pass
        elif newName in self.items:
            ErrorMessage(self,_('Name must be unique.'))
        elif self.data.rename(curName,newName):
            self.items[itemDex] = newName
            self.list.SetString(itemDex,newName)

    def DoRemove(self,event):
        """Removes selected item."""
        selections = self.list.GetSelections()
        if not selections:
            wx.Bell()
            return
        #--Data
        itemDex = selections[0]
        item = self.items[itemDex]
        if not self.data.remove(item): return 
        #--GUI
        del self.items[itemDex]
        self.list.Delete(itemDex)
        
    #--Window Closing
    def OnCloseWindow(self, event):
        """Handle window close event.
        Remember window size, position, etc."""
        sizes = settings.getChanged('mash.window.sizes')
        sizes[self.data.__class__] = self.GetSizeTuple()
        self.Destroy()

#------------------------------------------------------------------------------
class ListCtrl(wx.ListCtrl, ListCtrlAutoWidthMixin):
    def __init__(self, parent, ID, pos=wx.DefaultPosition,
                 size=wx.DefaultSize, style=0):
        wx.ListCtrl.__init__(self, parent, ID, pos, size, style=style)
        ListCtrlAutoWidthMixin.__init__(self)

#------------------------------------------------------------------------------
class List(wx.Panel):
    def __init__(self,parent,id=-1,ctrlStyle=(wx.LC_REPORT | wx.LC_SINGLE_SEL)):
        wx.Panel.__init__(self,parent,id, style=wx.WANTS_CHARS)
        sizer = wx.BoxSizer(wx.VERTICAL)
        self.SetSizer(sizer)
        self.SetSizeHints(-1,50)
        #--ListCtrl
        listId = self.listId = wx.NewId()
        self.list = ListCtrl(self, listId, style=ctrlStyle)
        self.checkboxes = images['mash.checkboxes']
        #--Columns
        self.PopulateColumns()
        #--Items
        self.sortDirty = 0
        self.PopulateItems()
        #--Events
        wx.EVT_SIZE(self, self.OnSize)
        #--Events: Items
        self.hitIcon = 0
        wx.EVT_LEFT_DOWN(self.list,self.OnLeftDown)
        wx.EVT_COMMAND_RIGHT_CLICK(self.list, listId, self.DoItemMenu)
        #--Events: Columns
        wx.EVT_LIST_COL_CLICK(self, listId, self.DoItemSort)
        wx.EVT_LIST_COL_RIGHT_CLICK(self, listId, self.DoColumnMenu)
        wx.EVT_LIST_COL_END_DRAG(self,listId, self.OnColumnResize)
    
    #--Items ----------------------------------------------
    #--Populate Columns
    def PopulateColumns(self):
        """Create/name columns in ListCtrl."""
        cols = self.cols
        self.numCols = len(cols)
        for colDex in range(self.numCols):
            colKey = cols[colDex]
            colName = self.colNames.get(colKey,colKey)
            wxListAlign = wxListAligns[self.colAligns.get(colKey,0)]
            self.list.InsertColumn(colDex,colName,wxListAlign)
            self.list.SetColumnWidth(colDex, self.colWidths.get(colKey,30))

    def PopulateItem(self,itemDex,mode=0,selected=set()):
        """Populate ListCtrl for specified item. [ABSTRACT]"""
        raise mosh.AbstractError

    def GetItems(self):
        """Set and return self.items."""
        self.items = self.data.keys()
        return self.items

    def PopulateItems(self,col=None,reverse=-2,selected='SAME'):
        """Sort items and populate entire list."""
        #--Sort Dirty?
        if self.sortDirty:
            self.sortDirty = 0
            (col, reverse) = (None,-1)
        #--Items to select afterwards. (Defaults to current selection.)
        if selected == 'SAME': selected = set(self.GetSelected())
        #--Reget items
        self.GetItems()
        self.SortItems(col,reverse)
        #--Delete Current items
        listItemCount = self.list.GetItemCount()
        #--Populate items
        for itemDex in range(len(self.items)):
            mode = int(itemDex >= listItemCount)
            self.PopulateItem(itemDex,mode,selected)
        #--Delete items?
        while self.list.GetItemCount() > len(self.items):
            self.list.DeleteItem(self.list.GetItemCount()-1)

    def ClearSelected(self):
        for itemDex in range(self.list.GetItemCount()):
            self.list.SetItemState(itemDex, 0, wx.LIST_STATE_SELECTED)

    def GetSelected(self):
        """Return list of items selected (hilighted) in the interface."""
        #--No items?
        if not 'items' in self.__dict__: return []
        selected = []
        itemDex = -1
        while True:
            itemDex = self.list.GetNextItem(itemDex,
                wx.LIST_NEXT_ALL,wx.LIST_STATE_SELECTED)
            if itemDex == -1: 
                break
            else:
                selected.append(self.items[itemDex])
        return selected

    def GetSortSettings(self,col,reverse):
        """Return parsed col, reverse arguments. Used by SortSettings.
        col: sort variable. 
          Defaults to last sort. (self.sort)
        reverse: sort order
          1: Descending order
          0: Ascending order
         -1: Use current reverse settings for sort variable, unless
             last sort was on same sort variable -- in which case, 
             reverse the sort order. 
         -2: Use current reverse setting for sort variable.
        """
        #--Sort Column
        if not col:
            col = self.sort
        #--Reverse
        oldReverse = self.colReverse.get(col,0)
        if col == 'Load Order': #--Disallow reverse for load
            reverse = 0
        elif reverse == -1 and col == self.sort:
            reverse = not oldReverse
        elif reverse < 0:
            reverse = oldReverse
        #--Done
        self.sort = col
        self.colReverse[col] = reverse
        return (col,reverse)

    #--Event Handlers -------------------------------------
    #--Column Menu
    def DoColumnMenu(self,event):
        if not self.mainMenu: return
        #--Build Menu
        column = event.GetColumn()
        menu = wx.Menu()
        for link in self.mainMenu:
            link.AppendToMenu(menu,self,column)
        #--Show/Destroy Menu
        self.PopupMenu(menu)
        menu.Destroy()

    #--Column Resize
    def OnColumnResize(self,event):
        pass

    #--Item Sort
    def DoItemSort(self, event):
        self.PopulateItems(self.cols[event.GetColumn()],-1)

    #--Item Menu
    def DoItemMenu(self,event):
        selected = self.GetSelected()
        if not selected: return
        #--Build Menu
        menu = wx.Menu()
        for link in self.itemMenu:
            link.AppendToMenu(menu,self,selected)
        #--Show/Destroy Menu
        self.PopupMenu(menu)
        menu.Destroy()

    #--Size Change
    def OnSize(self, event):
        size = self.GetClientSizeTuple()
        #print self,size
        self.list.SetSize(size)

    #--Event: Left Down
    def OnLeftDown(self,event):
        #self.hitTest = self.list.HitTest((event.GetX(),event.GetY()))
        event.Skip()
    
#------------------------------------------------------------------------------
class MasterList(List):
    mainMenu = []
    itemMenu = []

    def __init__(self,parent,fileInfo):
        self.parent = parent
        #--Columns
        self.cols = settings['mash.masters.cols']
        self.colNames = settings['mash.colNames']
        self.colWidths = settings['mash.masters.colWidths']
        self.colAligns = settings['mash.masters.colAligns']
        self.colReverse = settings['mash.masters.colReverse'].copy()
        #--Data/Items
        self.edited = False
        self.fileInfo = fileInfo
        self.fileIsMod = True
        self.prevId = -1
        self.data = {}
        self.items = [] #--These are id numbers
        self.oldMasters = []
        self.newMasters = []
        self.allMasters = [] #--Used for sorting
        self.sort = settings['mash.masters.sort']
        self.esmsFirst = settings['mash.masters.esmsFirst']
        self.selectedFirst = settings['mash.masters.selectedFirst']
        #--Links
        self.mainMenu = MasterList.mainMenu
        self.itemMenu = MasterList.itemMenu
        #--Parent init
        List.__init__(self,parent,-1,ctrlStyle=(wx.LC_REPORT|wx.LC_SINGLE_SEL|wx.LC_EDIT_LABELS))
        wx.EVT_LIST_END_LABEL_EDIT(self,self.listId,self.OnLabelEdited)
        #--Image List
        checkboxesIL = self.checkboxes.GetImageList()
        self.list.SetImageList(checkboxesIL,wx.IMAGE_LIST_SMALL)

    #--Label Edited
    def OnLabelEdited(self,event):
        itemDex = event.m_itemIndex
        newName = event.GetText()
        #--No change?
        if newName in mosh.modInfos:
            masterInfo = self.data[self.items[itemDex]]
            oldName = masterInfo.name
            masterInfo.setName(newName)
            if newName not in self.newMasters: 
                self.newMasters.append(newName)
            if (oldName in self.newMasters) and (not self.getMasterInfos(oldName)):
                self.newMasters.remove(oldName)
            if newName not in self.allMasters:
                self.allMasters.append(newName)
            self.ReList()
            self.PopulateItem(itemDex)
            settings.getChanged('mash.mods.renames')[masterInfo.oldName] = newName
        elif newName == '':
            event.Veto()
        else:
            ErrorMessage(self,_('File "%s" does not exist.') % (newName,))
            event.Veto()

    #--NewItemNum
    def newId(self):
        self.prevId += 1
        return self.prevId

    #--Set ModInfo
    def SetFileInfo(self,fileInfo):
        self.ClearSelected()
        self.edited = False
        self.fileInfo = fileInfo
        self.fileIsMod = fileInfo and fileInfo.isMod()
        self.prevId = -1
        self.data.clear()
        del self.items[:]
        del self.oldMasters[:]
        del self.newMasters[:]
        del self.allMasters[:]
        #--Null fileInfo?
        if not fileInfo:
            self.PopulateItems()
            return
        #--Fill data and populate
        for (masterName,size) in fileInfo.tes3.masters:
            item = self.newId()
            masterInfo = mosh.MasterInfo(masterName,size)
            self.data[item] = masterInfo
            self.items.append(item)
            self.oldMasters.append(masterName)
        self.newMasters.extend(mosh.modInfos.getLoadOrder(self.oldMasters,False))
        self.allMasters.extend(self.newMasters)
        self.PopulateItems()

    #--Get Master Status
    def GetMasterStatus(self,item):
        masterInfo = self.data[item]
        masterName = masterInfo.name
        status = masterInfo.getStatus()
        if status == 30 or masterName not in self.newMasters:
            return status
        newIndex = self.newMasters.index(masterName)
        mwIniLoadOrder = mosh.mwIniFile.loadOrder
        if (not self.edited) and (newIndex != self.oldMasters.index(masterName)):
            return 20
        elif status > 0 or self.fileIsMod:
            return status
        elif ((newIndex < len(mwIniLoadOrder)) and 
            (mwIniLoadOrder[newIndex] == masterName)):
            return -10
        else:
            return status

    #--Get Items
    def GetItems(self):
        return self.items

    #--Populate Item
    def PopulateItem(self,itemDex,mode=0,selected=set()):
        itemId = self.items[itemDex]
        masterInfo = self.data[itemId]
        masterName = masterInfo.name
        cols = self.cols
        for colDex in range(self.numCols):
            #--Value
            col = cols[colDex]
            if col == 'File':
                value = masterName
            elif col == 'Num':
                value = `self.allMasters.index(masterName)+1`
            #--Insert/Set Value
            if mode and (colDex == 0):
                self.list.InsertStringItem(itemDex, value)
            else:
                self.list.SetStringItem(itemDex, colDex, value)
        #--Text BG
        if not mosh.mwIniFile.isWellOrdered(masterName):
            self.list.SetItemBackgroundColour(itemDex,colors['mash.doubleTime.load'])
        elif masterInfo.getObjectMap():
            self.list.SetItemBackgroundColour(itemDex,colors['mash.masters.remapped'])
        elif masterInfo.hasChanged():
            self.list.SetItemBackgroundColour(itemDex,colors['mash.masters.changed'])
        elif masterInfo.isExOverLoaded():
            self.list.SetItemBackgroundColour(itemDex,colors['mash.exOverLoaded'])
        elif not masterInfo.isWellOrdered():
            self.list.SetItemBackgroundColour(itemDex,colors['mash.doubleTime.exists'])
        elif masterName[-1].lower() == 'm':
            self.list.SetItemBackgroundColour(itemDex,colors['mash.esm'])
        else:
            self.list.SetItemBackgroundColour(itemDex,colors['mash.doubleTime.not'])
        #--Image
        status = self.GetMasterStatus(itemId)
        on = masterInfo.isLoaded
        self.list.SetItemImage(itemDex,self.checkboxes.Get(status,on))
        #--Selection State [NOT USED]

    #--Sort Items
    def SortItems(self,col=None,reverse=-2):
        (col, reverse) = self.GetSortSettings(col,reverse)
        #--Sort
        data = self.data
        #--Start with sort by type
        self.items.sort(key=lambda a: data[a].name[:-4].lower())
        if col == 'File':
            pass #--Done by default
        elif col == 'Rating':
            self.items.sort(key=lambda a: mosh.modInfos.table.getItem(a,'rating',''))
        elif col == 'Group':
            self.items.sort(key=lambda a: mosh.modInfos.table.getItem(a,'group',''))
        elif col == 'Modified':
            self.items.sort(key=lambda a: data[a].mtime)
        elif col == 'Load Order':
            allMasters = self.allMasters
            data = self.data
            self.items.sort(key=lambda a: allMasters.index(data[a].name))
        elif col == 'Size':
            self.items.sort(lambda a,b: cmp(data[a].size,data[b].size))
        elif col == 'Status':
            self.items.sort(lambda a,b: cmp(self.GetMasterStatus(a),self.GetMasterStatus(b)))
        elif col == 'Author':
            self.items.sort(lambda a,b: cmp(data[a].author.lower(),data[b].author.lower()))
        else:
            raise MashError, _('Unrecognized sort key: ')+col
        #--Ascending
        if reverse: self.items.reverse()
        #--ESMs First?
        settings['mash.masters.esmsFirst'] = self.esmsFirst
        if self.esmsFirst or col == 'Load Order':
            self.items.sort(key=lambda a:data[a].name[-1].lower())

    #--Get instances
    def getMasterInfos(self,masterName):
        masterInfos = []
        for masterInfo in self.data.values():
            if masterInfo.name == masterName:
                masterInfos.append(masterInfo)
        return masterInfos

    #--Selection (exists and is selected)
    def isLoaded(self,masterInfos):
        if not masterInfos:
            return False
        for masterInfo in masterInfos:
            if masterInfo.isLoaded == False:
                return False
        #--Else Okay
        return True
    
    def load(self,masterName):
        masterInfos = self.getMasterInfos(masterName)
        #--Already selected?
        if self.isLoaded(masterInfos): 
            return True
        #--Already at max masters?
        elif len(self.newMasters) == 255:
            ErrorMessage(self, _('Unable to select %s because file already has maximum number of masters.') 
                % (masterName,))
            return False
        #--New master?
        elif not masterInfos:
            modInfo = mosh.modInfos.get(masterName,None)
            if not modInfo:
                ErrorMessage(self,_('Unable to add %s because file doesn\'t exist.' )
                    % (masterName,))
                return False
            itemId = self.newId()
            masterInfo = mosh.MasterInfo(masterName,modInfo.size)
            masterInfo.isNew = True
            self.data[itemId] = masterInfo
            self.items.append(itemId)
            masterInfos.append(masterInfo)
            self.newMasters.append(masterName)
            if masterName not in self.allMasters: 
                self.allMasters.append(masterName)
            self.ReList()
        #--File exists?
        if not masterName in mosh.modInfos.keys():
            wx.Bell()
            return
        #--Select master's masters
        for mmName in masterInfos[0].masterNames:
            if not self.load(mmName): 
                return False
        #--Select master
        for masterInfo in masterInfos:
            masterInfo.isLoaded = True
        if masterName not in self.newMasters: 
            self.newMasters.append(masterName)
            if masterName not in self.allMasters:
                self.allMasters.append(masterName)
            self.ReList()
        #--Done
        return True

    def unload(self,masterName):
        #--Unselect self
        masterInfos = self.getMasterInfos(masterName)
        for masterInfo in masterInfos:
            masterInfo.isLoaded = False
        if masterName in self.newMasters: 
            self.newMasters.remove(masterName)
        #--Unselect dependents
        for itemId in self.items:
            otherMasterInfo = self.data[itemId]
            if not otherMasterInfo.isLoaded: continue
            if masterName in otherMasterInfo.masterNames:
                self.unload(otherMasterInfo.name)

    #--Relist
    def ReList(self):
        self.newMasters = mosh.modInfos.getLoadOrder(self.newMasters,False)
        self.allMasters = mosh.modInfos.getLoadOrder(self.allMasters,False)

    #--InitEdit
    def InitEdit(self):
        #--Pre-clean
        for itemId in self.items:
            masterInfo = self.data[itemId]
            #--Missing Master?
            if not masterInfo.modInfo:
                masterName = masterInfo.name
                newName = settings['mash.mods.renames'].get(masterName,None)
                #--Rename?
                if newName and mosh.modInfos.has_key(newName):
                    masterInfo.setName(newName)
                    if newName not in self.newMasters: 
                        self.newMasters.append(newName)
                    if newName not in self.allMasters: 
                        self.allMasters.append(newName)
                #--Unselect?
                else:
                    #self.unload(masterName)
                    masterInfo.isLoaded = False
                if masterName in self.newMasters: self.newMasters.remove(masterName)
            #--Fix size
            if masterInfo.modInfo:
                masterInfo.size = masterInfo.modInfo.size
            else:
                masterInfo.size = 0
        #--Done
        self.edited = True
        self.ReList()
        self.PopulateItems()
        self.parent.SetEdited()

    #--Item Sort
    def DoItemSort(self, event):
        pass #--Don't do column head sort.

    #--Column Menu
    def DoColumnMenu(self,event):
        if not self.fileInfo: return
        List.DoColumnMenu(self,event)

    #--Item Menu
    def DoItemMenu(self,event):
        if not self.edited:
            self.OnLeftDown(event)
        else:
            List.DoItemMenu(self,event)

    #--Column Resize
    def OnColumnResize(self,event):
        colDex = event.GetColumn()
        colName = self.cols[colDex]
        self.colWidths[colName] = self.list.GetColumnWidth(colDex)
        settings.setChanged('mash.masters.colWidths')

    #--Event: Left Down
    def OnLeftDown(self,event):
        #--Not edited yet?
        if not self.edited:
            message = (_("Edit/update the masters list? Note that the update process will automatically fix some problems. Be sure to review the changes before saving."))
            if ContinueQuery(self,message,'mash.masters.update',_('Update Masters')) != wx.ID_OK:
                return
            self.InitEdit()
            return
        (hitItem,hitFlag) = self.list.HitTest((event.GetX(),event.GetY()))
        if hitFlag == 32:
            itemId = self.items[hitItem]
            masterInfo = self.data[itemId]
            #--Unselect?
            if masterInfo.isLoaded:
                self.unload(masterInfo.name)
            #--Select?
            else:
                self.load(masterInfo.name)
            #--Repopulate
            self.ReList()
            self.PopulateItems()
        #--Pass event on (for label editing)
        if self.edited: 
            event.Skip()

    #--GetMasters
    def GetNewMasters(self):
        newMasters = []
        for newMaster in self.newMasters:
            newMasters.append((newMaster,mosh.modInfos[newMaster].size))
        return newMasters
    
    #--Get ModMap
    def GetMaps(self):
        modMap = {}
        objMaps = []
        for itemId in range(len(self.oldMasters)):
            masterInfo = self.data[itemId]
            #--Delete?
            oldMod = itemId + 1
            if not masterInfo.modInfo or not masterInfo.isLoaded:
                modMap[oldMod] = -1 #--Delete
            else:
                masterName = masterInfo.name
                if masterName not in self.newMasters:
                    raise mosh.MoshError, _("Missing master: ")+masterName
                newMod = self.newMasters.index(masterName) + 1
                if newMod != oldMod:
                    modMap[oldMod] = newMod
                #--Object map?
                objMap = masterInfo.getObjectMap()
                if objMap:
                    objMaps.append((oldMod,objMap))
        return (modMap,objMaps)

    #--Get ObjMaps
    def getObjMaps(self):
        objMaps = {}
        for itemId in range(len(self.oldMasters)):
            masterInfo = self.data[itemId]
        return objMaps

#------------------------------------------------------------------------------
class ModList(List):
    #--Class Data
    mainMenu = [] #--Column menu
    itemMenu = [] #--Single item menu

    def __init__(self,parent):
        #--Columns
        self.cols = settings['mash.mods.cols']
        self.colAligns = settings['mash.mods.colAligns']
        self.colNames = settings['mash.colNames']
        self.colReverse = settings.getChanged('mash.mods.colReverse')
        self.colWidths = settings['mash.mods.colWidths']
        #--Data/Items
        self.data = data = mosh.modInfos
        self.details = None #--Set by panel
        self.sort = settings['mash.mods.sort']
        self.esmsFirst = settings['mash.mods.esmsFirst']
        self.selectedFirst = settings['mash.mods.selectedFirst']
        #--Links
        self.mainMenu = ModList.mainMenu
        self.itemMenu = ModList.itemMenu
        #--Parent init
        List.__init__(self,parent,-1,ctrlStyle=(wx.LC_REPORT))#|wx.SUNKEN_BORDER))
        #--Image List
        checkboxesIL = images['mash.checkboxes'].GetImageList()
        self.list.SetImageList(checkboxesIL,wx.IMAGE_LIST_SMALL)
        #--Events
        wx.EVT_LIST_ITEM_SELECTED(self,self.listId,self.OnItemSelected)
        self.list.Bind(wx.EVT_LEFT_DCLICK, self.OnDoubleClick)

    def Refresh(self,files='ALL',detail='SAME'):
        """Refreshes UI for specified file. Also calls saveList.Refresh()!"""
        #--Details
        if detail == 'SAME':
            selected = set(self.GetSelected())
        else:
            selected = set([detail])
        #--Populate
        if files == 'ALL':
            self.PopulateItems(selected=selected)
        elif isinstance(files,StringTypes):
            self.PopulateItem(files,selected=selected)
        else: #--Iterable
            for file in files:
                self.PopulateItem(file,selected=selected)
        modDetails.SetFile(detail)
        #--Saves
        saveList.Refresh()

    #--Populate Item
    def PopulateItem(self,itemDex,mode=0,selected=set()):
        #--String name of item?
        if not isinstance(itemDex,int):
            itemDex = self.items.index(itemDex)
        fileName = self.items[itemDex]
        fileInfo = self.data[fileName]
        cols = self.cols
        for colDex in range(self.numCols):
            col = cols[colDex]
            #--Get Value
            if col == 'File':
                value = fileName
            elif col == 'Rating':
                value = mosh.modInfos.table.getItem(fileName,'rating','')
            elif col == 'Group':
                value = mosh.modInfos.table.getItem(fileName,'group','')
            elif col == 'Modified':
                value = formatDate(fileInfo.mtime)
            elif col == 'Size':
                value = formatInteger(fileInfo.size/1024)+' KB'
            elif col == 'Author' and fileInfo.tes3:
                value = fileInfo.tes3.hedr.author
            else:
                value = '-'
            #--Insert/SetString
            if mode and (colDex == 0):
                self.list.InsertStringItem(itemDex, value)
            else:
                self.list.SetStringItem(itemDex, colDex, value)
        #--Text BG
        if not mosh.mwIniFile.isWellOrdered(fileName):
            self.list.SetItemBackgroundColour(itemDex,colors['mash.doubleTime.load'])
        elif fileInfo.isExOverLoaded():
            self.list.SetItemBackgroundColour(itemDex,colors['mash.exOverLoaded'])
        elif not fileInfo.isWellOrdered():
            self.list.SetItemBackgroundColour(itemDex,colors['mash.doubleTime.exists'])
        elif fileName[-1].lower() == 'm':
            self.list.SetItemBackgroundColour(itemDex,colors['mash.esm'])
        else:
            self.list.SetItemBackgroundColour(itemDex,colors['mash.doubleTime.not'])
        #--Image
        status = fileInfo.getStatus()
        on = fileInfo.name in mosh.mwIniFile.loadFiles
        self.list.SetItemImage(itemDex,self.checkboxes.Get(status,on))
        #--Selection State
        if fileName in selected:
            self.list.SetItemState(itemDex,wx.LIST_STATE_SELECTED,wx.LIST_STATE_SELECTED)
        else:
            self.list.SetItemState(itemDex,0,wx.LIST_STATE_SELECTED)

    #--Sort Items
    def SortItems(self,col=None,reverse=-2):
        (col, reverse) = self.GetSortSettings(col,reverse)
        settings['mash.mods.sort'] = col
        loadFiles = mosh.mwIniFile.loadFiles
        data = self.data
        #--Start with sort by name
        self.items.sort( key= lambda a: a[:-4].lower())
        if col == 'File':
            pass #--Done by default
        elif col == 'Author':
            self.items.sort(lambda a,b: cmp(
                data[a].tes3.hedr.author.lower(),
                data[b].tes3.hedr.author.lower()))
        elif col == 'Rating':
            self.items.sort(key=lambda a: mosh.modInfos.table.getItem(a,'rating',''))
        elif col == 'Group':
            self.items.sort(key=lambda a: mosh.modInfos.table.getItem(a,'group',''))
        elif col == 'Load Order':
            self.items = mosh.modInfos.getLoadOrder(self.items,False)
        elif col == 'Modified':
            self.items.sort(key=lambda a: data[a].mtime)
        elif col == 'Size':
            self.items.sort(key=lambda a: data[a].size)
        elif col == 'Status':
            self.items.sort(key=lambda a: data[a].getStatus())
        elif col == 'Version':
            self.items.sort(key=lambda a: data[a].tes3.hedr.version)
        else:
            raise MashError, _('Unrecognized sort key: ')+col
        #--Ascending
        if reverse: self.items.reverse()
        #--ESMs First?
        settings['mash.mods.esmsFirst'] = self.esmsFirst
        if self.esmsFirst or col == 'Load Order':
            self.items.sort(lambda a,b: cmp(a[-4:].lower(),b[-4:].lower()))
        #--Selected First?
        settings['mash.mods.selectedFirst'] = self.selectedFirst
        if self.selectedFirst:
            self.items.sort(lambda a,b: cmp(b in loadFiles, a in loadFiles))

    #--Events ---------------------------------------------
    def OnDoubleClick(self,event):
        """Handle doubclick event."""
        (hitItem,hitFlag) = self.list.HitTest(event.GetPosition())
        if hitItem < 0: return
        fileInfo = self.data[self.items[hitItem]]
        if not docBrowser: 
            DocBrowser().Show()
            settings['mash.modDocs.show'] = True
        docBrowser.SetMod(fileInfo.name)
        docBrowser.Raise()

    #--Column Resize
    def OnColumnResize(self,event):
        colDex = event.GetColumn()
        colName = self.cols[colDex]
        self.colWidths[colName] = self.list.GetColumnWidth(colDex)
        settings.setChanged('mash.mods.colWidths')

    #--Event: Left Down
    def OnLeftDown(self,event):
        (hitItem,hitFlag) = self.list.HitTest((event.GetX(),event.GetY()))
        if hitFlag == 32:
            oldDTFiles = mosh.mwIniFile.getDoubleTimeFiles()
            oldFiles = mosh.mwIniFile.loadFiles[:]
            fileName = self.items[hitItem]
            #--Unselect?
            if self.data.isLoaded(fileName):
                self.data.unload(fileName)
                changedFiles = mosh.listSubtract(oldFiles,mosh.mwIniFile.loadFiles)
            #--Select?
            else:
                try:
                    self.data.load(fileName)
                    changedFiles = mosh.listSubtract(mosh.mwIniFile.loadFiles,oldFiles)
                except mosh.MaxLoadedError:
                    ErrorMessage(self,_("Unable to add mod %s because load list is full." )
                        % (fileName,))
                    return
            newDTFiles = mosh.mwIniFile.getDoubleTimeFiles()
            #--Refresh changed files
            #self.Refresh(changedFiles + oldDTFiles + newDTFiles)
            self.Refresh()
            #--Mark sort as dirty
            if self.selectedFirst:
                self.sortDirty = 1
        #--Pass Event onward
        event.Skip()

    def OnItemSelected(self,event):
        modName = self.items[event.m_itemIndex]
        self.details.SetFile(modName)
        if docBrowser: 
            docBrowser.SetMod(modName)

#------------------------------------------------------------------------------
class ModDetails(wx.Window):
    def __init__(self,parent):
        wx.Window.__init__(self, parent, -1, style=wx.TAB_TRAVERSAL)
        #--Singleton
        global modDetails
        modDetails = self
        #--Data
        self.modInfo = None
        self.edited = False
        textWidth = 200
        #--Sizer
        sizer = wx.BoxSizer(wx.VERTICAL)
        self.SetSizer(sizer)
        #--File/Version Static Text
        sizer_h0 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_h0.Add(wx.StaticText(self,-1,_("Morrowind Mod File")),0,wx.TOP,4)
        sizer_h0.Add((0,0),1) #--Spacer
        self.version = wx.StaticText(self,-1,'v0.0')
        sizer_h0.Add(self.version,0,wx.TOP|wx.RIGHT,4)
        #self.readMe = wx.BitmapButton(self,ID_BROWSER,images['doc.on'].GetBitmap(),style=wx.NO_BORDER)
        #self.readMe.SetToolTip(wx.ToolTip(_("Show Doc Browser")))
        #wx.EVT_BUTTON(self,ID_BROWSER,self.OnBrowser)
        #sizer_h0.Add(self.readMe,0,wx.TOP,4)
        #self.version.SetSizeHints(25,-1)
        #sizer_h0.Add((0,0),1) #--Spacer
        sizer.Add(sizer_h0,0,wx.EXPAND)
        #--File Name
        id = self.fileId = wx.NewId()
        self.file = wx.TextCtrl(self,id,"",size=(textWidth,-1))
        self.file.SetMaxLength(200)
        sizer.Add(self.file)
        wx.EVT_KILL_FOCUS(self.file,self.OnEditFile)
        wx.EVT_TEXT(self.file,id,self.OnTextEdit)
        #--Author
        id = self.authorId = wx.NewId()
        #sizer.Add(wx.StaticText(self,-1,_("Author:")),0,wx.TOP,4)
        self.author = wx.TextCtrl(self,id,"",size=(textWidth,-1))
        self.author.SetMaxLength(32)
        sizer.Add(self.author)
        wx.EVT_KILL_FOCUS(self.author,self.OnEditAuthor)
        wx.EVT_TEXT(self.author,id,self.OnTextEdit)
        #--Modified
        id = self.modifiedId = wx.NewId()
        #sizer.Add(wx.StaticText(self,-1,_("Modified:")),0,wx.TOP,4)
        self.modified = wx.TextCtrl(self,id,"",size=(textWidth,-1))
        self.modified.SetMaxLength(32)
        sizer.Add(self.modified)
        wx.EVT_KILL_FOCUS(self.modified,self.OnEditModified)
        wx.EVT_TEXT(self.modified,id,self.OnTextEdit)
        #--Description
        #sizer_h2 = wx.BoxSizer(wx.HORIZONTAL)
        #sizer_h2.Add(wx.StaticText(self,-1,_("Description:")),0,wx.TOP,4)
        #sizer_h2.Add((0,0),1)
        #sizer.Add(sizer_h2,0,wx.EXPAND)
        #--
        id = self.descriptionId = wx.NewId()
        self.description = (
            wx.TextCtrl(self,id,"",size=(textWidth,150),style=wx.TE_MULTILINE))
        self.description.SetMaxLength(256)
        sizer.Add(self.description)
        wx.EVT_KILL_FOCUS(self.description,self.OnEditDescription)
        wx.EVT_TEXT(self.description,id,self.OnTextEdit)
        #--Masters
        id = self.mastersId = wx.NewId()
        #sizer.Add(wx.StaticText(self,-1,_("Masters:")),0,wx.TOP,4)
        self.masters = MasterList(self,None)
        sizer.Add(self.masters,1,wx.EXPAND)
        #--Save/Cancel
        sizer_h1 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_h1.Add((0,0),1)
        self.save = wx.Button(self,wx.ID_SAVE)
        self.cancel = wx.Button(self,wx.ID_CANCEL)
        self.save.Disable()
        self.cancel.Disable()
        sizer_h1.Add(self.save)
        sizer_h1.Add(self.cancel,0,wx.LEFT,4)
        sizer.Add(sizer_h1,0,wx.EXPAND|wx.TOP,4)
        wx.EVT_BUTTON(self,wx.ID_SAVE,self.OnSave)
        wx.EVT_BUTTON(self,wx.ID_CANCEL,self.OnCancel)

    def SetFile(self,fileName='SAME'):
        #--Reset?
        if fileName == 'SAME': 
            if not self.modInfo or self.modInfo.name not in mosh.modInfos:
                fileName = None
            else:
                fileName = self.modInfo.name
        #--Empty?
        if not fileName:
            modInfo = self.modInfo = None
            self.fileStr = ''
            self.authorStr = ''
            self.modifiedStr = ''
            self.descriptionStr = ''
            self.versionStr = ''
        #--Valid fileName?
        else:
            modInfo = self.modInfo = mosh.modInfos[fileName]
            #--Remember values for edit checks
            self.fileStr = modInfo.name
            self.authorStr = modInfo.tes3.hedr.author
            self.modifiedStr = formatDate(modInfo.mtime)
            self.descriptionStr = modInfo.tes3.hedr.description
            self.versionStr = 'v%0.1f' % (modInfo.tes3.hedr.version,)
        #--Set fields
        self.file.SetValue(self.fileStr)
        self.author.SetValue(self.authorStr)
        self.modified.SetValue(self.modifiedStr)
        self.description.SetValue(self.descriptionStr)
        self.version.SetLabel(self.versionStr)
        self.masters.SetFileInfo(modInfo)
        #--Edit State
        self.edited = 0
        self.save.Disable()
        self.cancel.Disable()

    def SetEdited(self):
        self.edited = True
        self.save.Enable()
        self.cancel.Enable()
    
    def OnBrowser(self,event):
        """Event: Clicked Doc Browser button."""
        if not docBrowser: 
            DocBrowser().Show()
            settings['mash.modDocs.show'] = True
        if self.modInfo:
            docBrowser.SetMod(self.modInfo.name)
        docBrowser.Raise()

    def OnTextEdit(self,event):
        if self.modInfo and not self.edited:
            if ((self.fileStr != self.file.GetValue()) or
                (self.authorStr != self.author.GetValue()) or
                (self.modifiedStr != self.modified.GetValue()) or
                (self.descriptionStr != self.description.GetValue()) ):
                self.SetEdited()
        event.Skip()

    def OnEditFile(self,event):
        if not self.modInfo: return
        #--Changed?
        fileStr = self.file.GetValue()
        if fileStr == self.fileStr: return
        #--Extension Changed?
        if fileStr[-4:].lower() != self.fileStr[-4:].lower():
            ErrorMessage(self,_("Incorrect file extension: ")+fileStr[-3:])
            self.file.SetValue(self.fileStr)
        #--Else file exists?
        elif os.path.exists(os.path.join(self.modInfo.dir,fileStr)):
            ErrorMessage(self,_("File %s already exists.") % (fileStr,))
            self.file.SetValue(self.fileStr)
        #--Okay?
        else:
            self.fileStr = fileStr
            self.SetEdited()

    def OnEditAuthor(self,event):
        if not self.modInfo: return
        authorStr = self.author.GetValue()
        if authorStr != self.authorStr:
            self.authorStr = authorStr
            self.SetEdited()

    def OnEditModified(self,event):
        if not self.modInfo: return
        modifiedStr = self.modified.GetValue()
        if modifiedStr == self.modifiedStr: return
        try:
            newTimeTup = time.strptime(modifiedStr,'%c')
            time.mktime(newTimeTup)
        except ValueError:
            ErrorMessage(self,_('Unrecognized date: ')+modifiedStr)
            self.modified.SetValue(self.modifiedStr)
            return
        except OverflowError:
            ErrorMessage(self,_('Mash cannot handle files dates greater than January 19, 2038.)'))
            self.modified.SetValue(self.modifiedStr)
            return
        #--Normalize format
        modifiedStr = time.strftime('%c',newTimeTup)
        self.modifiedStr = modifiedStr
        self.modified.SetValue(modifiedStr) #--Normalize format
        self.SetEdited()

    def OnEditDescription(self,event):
        if not self.modInfo: return
        descriptionStr = self.description.GetValue()
        if descriptionStr != self.descriptionStr:
            self.descriptionStr = descriptionStr
            self.SetEdited()
    
    def OnSave(self,event):
        modInfo = self.modInfo
        #--Change Tests
        changeName = (self.fileStr != modInfo.name)
        changeDate = (self.modifiedStr != formatDate(modInfo.mtime))
        changeHedr = ((self.authorStr != modInfo.tes3.hedr.author) or 
            (self.descriptionStr != modInfo.tes3.hedr.description ))
        changeMasters = self.masters.edited
        #--Only change date?
        if changeDate and not (changeName or changeHedr):
            newTimeTup = time.strptime(self.modifiedStr,'%c')
            newTimeInt = int(time.mktime(newTimeTup))
            modInfo.setMTime(newTimeInt)
            self.SetFile(self.modInfo.name)
            mosh.modInfos.refreshDoubleTime()
            modList.Refresh()
            return
        #--Backup
        modInfo.makeBackup()
        #--Change Name?
        fileName = modInfo.name
        if changeName:
            (oldName,newName) = (modInfo.name,self.fileStr.strip())
            modList.items[modList.items.index(oldName)] = newName
            settings.getChanged('mash.mods.renames')[oldName] = newName
            mosh.modInfos.rename(oldName,newName)
            fileName = newName
        #--Change hedr?
        if changeHedr:
            modInfo.tes3.hedr.author = self.authorStr.strip()
            modInfo.tes3.hedr.description = self.descriptionStr.strip()
            modInfo.tes3.hedr.changed = True
            modInfo.writeHedr()
        #--Change masters?
        if changeMasters:
            newMasters = self.masters.GetNewMasters()
            (modMap,objMaps) = self.masters.GetMaps()
            #--Create and use FileRefs
            progress = None
            try:
                progress = ProgressDialog(_('Saving'))
                fileRefs = mosh.FileRefs(modInfo,progress=progress)
                progress.setBaseScale(0.0,0.67)
                fileRefs.load()
                progress(1.0,_('Remap Masters'))
                fileRefs.remap(newMasters,modMap,objMaps)
                progress.setBaseScale(0.67,0.33)
                fileRefs.safeSave()
            finally:
                progress = progress.Destroy()
        #--Change date?
        if (changeDate or changeHedr or changeMasters):
            newTimeTup = time.strptime(self.modifiedStr,'%c')
            newTimeInt = int(time.mktime(newTimeTup))
            modInfo.setMTime(newTimeInt)
        #--Done
        try:
            mosh.modInfos.refreshFile(fileName)
            self.SetFile(fileName)
        except mosh.Tes3Error:
            ErrorMessage(self,_('File corrupted on save!'))
            self.SetFile(None)
        modList.Refresh()

    def OnCancel(self,event):
        self.SetFile(self.modInfo.name)

#------------------------------------------------------------------------------
class ModPanel(NotebookPanel):
    def __init__(self,parent):
        wx.Panel.__init__(self, parent, -1)
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        global modList
        modList = ModList(self)
        sizer.Add(modList,1,wx.GROW)
        sizer.Add((4,-1),0)
        self.modDetails = ModDetails(self)
        modList.details = self.modDetails
        sizer.Add(self.modDetails,0,wx.EXPAND)
        self.SetSizer(sizer)
        self.modDetails.Fit()
        #--Events
        wx.EVT_SIZE(self,self.OnSize)

    def SetStatusCount(self):
        """Sets mod count in last field."""
        text = _("Mods: %d/%d") % (len(mosh.mwIniFile.loadFiles),len(mosh.modInfos.data))
        statusBar.SetStatusText(text,2)

    def OnSize(self,event):
        wx.Window.Layout(self)
        modList.Layout()
        self.modDetails.Layout()

#------------------------------------------------------------------------------
class SaveList(List):
    #--Class Data
    mainMenu = [] #--Column menu
    itemMenu = [] #--Single item menu

    def __init__(self,parent):
        #--Columns
        self.cols = settings['mash.saves.cols']
        self.colAligns = settings['mash.saves.colAligns']
        self.colNames = settings['mash.colNames']
        self.colReverse = settings.getChanged('mash.saves.colReverse')
        self.colWidths = settings['mash.saves.colWidths']
        #--Data/Items
        self.data = data = mosh.saveInfos
        self.details = None #--Set by panel
        self.sort = settings['mash.saves.sort']
        #--Links
        self.mainMenu = SaveList.mainMenu
        self.itemMenu = SaveList.itemMenu
        #--Parent init
        List.__init__(self,parent,-1,ctrlStyle=(wx.LC_REPORT|wx.SUNKEN_BORDER))
        #--Image List
        checkboxesIL = self.checkboxes.GetImageList()
        self.list.SetImageList(checkboxesIL,wx.IMAGE_LIST_SMALL)
        #--Events
        wx.EVT_LIST_ITEM_SELECTED(self,self.listId,self.OnItemSelected)

    def Refresh(self,files='ALL',detail='SAME'):
        """Refreshes UI for specified files."""
        #--Details
        if detail == 'SAME':
            selected = set(self.GetSelected())
        else:
            selected = set([detail])
        #--Populate
        if files == 'ALL':
            self.PopulateItems(selected=selected)
        elif isinstance(files,StringTypes):
            self.PopulateItem(files,selected=selected)
        else: #--Iterable
            for file in files:
                self.PopulateItem(file,selected=selected)
        saveDetails.SetFile(detail)

    #--Populate Item
    def PopulateItem(self,itemDex,mode=0,selected=set()):
        #--String name of item?
        if not isinstance(itemDex,int):
            itemDex = self.items.index(itemDex)
        fileName = self.items[itemDex]
        fileInfo = self.data[fileName]
        cols = self.cols
        for colDex in range(self.numCols):
            col = cols[colDex]
            if col == 'File':
                value = fileName
            elif col == 'Modified':
                value = formatDate(fileInfo.mtime)
            elif col == 'Size':
                value = formatInteger(fileInfo.size/1024)+' KB'
            elif col == 'Save Name' and fileInfo.tes3:
                value = fileInfo.tes3.hedr.description
            elif col == 'Player' and fileInfo.tes3 and fileInfo.tes3.gmdt:
                value = fileInfo.tes3.gmdt.playerName
            elif col == 'Cell' and fileInfo.tes3 and fileInfo.tes3.gmdt:
                value = fileInfo.tes3.gmdt.curCell
            else:
                value = '-'
            if mode and (colDex == 0):
                self.list.InsertStringItem(itemDex, value)
            else:
                self.list.SetStringItem(itemDex, colDex, value)
        #--Image
        status = fileInfo.getStatus()
        self.list.SetItemImage(itemDex,self.checkboxes.Get(status,False))
        #--Selection State
        if fileName in selected:
            self.list.SetItemState(itemDex,wx.LIST_STATE_SELECTED,wx.LIST_STATE_SELECTED)
        else:
            self.list.SetItemState(itemDex,0,wx.LIST_STATE_SELECTED)

    #--Sort Items
    def SortItems(self,col=None,reverse=-2):
        (col, reverse) = self.GetSortSettings(col,reverse)
        settings['mash.saves.sort'] = col
        data = self.data
        #--Start with sort by name
        self.items.sort(lambda a,b: cmp(a.lower(),b.lower()))
        if col == 'File':
            pass #--Done by default
        elif col == 'Modified':
            self.items.sort(lambda a,b: cmp(data[a].mtime,data[b].mtime))
        elif col == 'Size':
            self.items.sort(lambda a,b: cmp(data[a].size,data[b].size))
        elif col == 'Save Name':
            self.items.sort(lambda a,b: cmp(
                data[a].tes3.hedr.description.lower(),
                data[b].tes3.hedr.description.lower()))
        elif col == 'Status':
            self.items.sort(lambda a,b: cmp(data[a].getStatus(),data[b].getStatus()))
        elif col == 'Player':
            self.items.sort(lambda a,b: cmp(
                data[a].tes3.gmdt.playerName.lower(),
                data[b].tes3.gmdt.playerName.lower()))
        elif col == 'Cell':
            self.items.sort(lambda a,b: cmp(
                data[a].tes3.gmdt.curCell.lower(),
                data[b].tes3.gmdt.curCell.lower()))
        else:
            raise MashError, _('Unrecognized sort key: ')+col
        #--Ascending
        if reverse: self.items.reverse()

    #--Events ---------------------------------------------
    #--Column Resize
    def OnColumnResize(self,event):
        colDex = event.GetColumn()
        colName = self.cols[colDex]
        self.colWidths[colName] = self.list.GetColumnWidth(colDex)
        settings.setChanged('mash.saves.colWidths')

    def OnItemSelected(self,event=None):
        saveName = self.items[event.m_itemIndex]
        self.details.SetFile(saveName)
        if journalBrowser: 
            journalBrowser.SetSave(saveName)

#------------------------------------------------------------------------------
class SaveDetails(wx.Window):
    """Savefile details panel."""
    def __init__(self,parent):
        """Initialize."""
        wx.Window.__init__(self, parent, -1, style=wx.TAB_TRAVERSAL)
        readOnlyColour = self.GetBackgroundColour()
        #--Singleton
        global saveDetails
        saveDetails = self
        #--Data
        self.saveInfo = None
        self.edited = False
        textWidth = 200
        #--Sizer
        sizer = wx.BoxSizer(wx.VERTICAL)
        self.SetSizer(sizer)
        #--File/Version Static Text
        sizer_h0 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_h0.Add(wx.StaticText(self,-1,_("Morrowind Save File")),0,wx.TOP,4)
        sizer_h0.Add((0,0),1) #--Spacer
        self.version = wx.StaticText(self,-1,'v0.0')
        sizer_h0.Add(self.version,0,wx.TOP|wx.RIGHT,4)
        sizer.Add(sizer_h0,0,wx.EXPAND)
        #--File Name
        id = self.fileId = wx.NewId()
        self.file = wx.TextCtrl(self,id,"",size=(textWidth,-1))
        self.file.SetMaxLength(256)
        sizer.Add(self.file)
        wx.EVT_KILL_FOCUS(self.file,self.OnEditFile)
        wx.EVT_TEXT(self.file,id,self.OnTextEdit)
        #--Save Name
        id = self.saveNameId = wx.NewId()
        #sizer.Add(wx.StaticText(self,-1,_("Save Name:")),0,wx.TOP,4)
        self.saveName = wx.TextCtrl(self,id,"",size=(textWidth,-1))
        self.saveName.SetMaxLength(32)
        sizer.Add(self.saveName)
        wx.EVT_KILL_FOCUS(self.saveName,self.OnEditSaveName)
        wx.EVT_TEXT(self.saveName,id,self.OnTextEdit)
        #--Player Name
        sizer_h2 = wx.BoxSizer(wx.HORIZONTAL)
        #sizer_h2.Add(wx.StaticText(self,-1,_("Player Name:")),0,wx.TOP,4)
        #sizer_h2.Add((0,0),1)
        #self.readMe = wx.BitmapButton(self,ID_BROWSER,images['doc.on'].GetBitmap(),style=wx.NO_BORDER)
        #self.readMe.SetToolTip(wx.ToolTip(_("Show Journal")))
        #wx.EVT_BUTTON(self,ID_BROWSER,self.OnBrowser)
        #sizer_h2.Add(self.readMe,0,wx.TOP,4)
        #sizer.Add(sizer_h2,0,wx.EXPAND)
        id = self.descriptionId = wx.NewId()
        self.playerName = wx.TextCtrl(self,id,"",size=(textWidth,-1),style=wx.TE_READONLY)
        self.playerName.SetBackgroundColour(readOnlyColour)
        sizer.Add(self.playerName)
        #--Cell
        id = self.curCellId = wx.NewId()
        #sizer.Add(wx.StaticText(self,-1,_("Cell:")),0,wx.TOP,4)
        self.curCell = wx.TextCtrl(self,id,"",size=(textWidth,-1),style=wx.TE_READONLY)
        self.curCell.SetBackgroundColour(readOnlyColour)
        sizer.Add(self.curCell)
        #--Picture
        self.picture = balt.Picture(self,textWidth,128)
        sizer.Add(self.picture,0,wx.TOP|wx.BOTTOM,4)
        #--Masters
        id = self.mastersId = wx.NewId()
        #sizer.Add(wx.StaticText(self,-1,_("Masters:")),0,wx.TOP,4)
        self.masters = MasterList(self,None)
        sizer.Add(self.masters,1,wx.EXPAND)
        #--Save/Cancel
        sizer_h1 = wx.BoxSizer(wx.HORIZONTAL)
        sizer_h1.Add((0,0),1)
        self.save = wx.Button(self,wx.ID_SAVE)
        self.cancel = wx.Button(self,wx.ID_CANCEL)
        self.save.Disable()
        self.cancel.Disable()
        sizer_h1.Add(self.save)
        sizer_h1.Add(self.cancel,0,wx.LEFT,4)
        sizer.Add(sizer_h1,0,wx.EXPAND|wx.TOP,4)
        wx.EVT_BUTTON(self,wx.ID_SAVE,self.OnSave)
        wx.EVT_BUTTON(self,wx.ID_CANCEL,self.OnCancel)
    
    def SetFile(self,fileName='SAME'):
        """Set file to be viewed."""
        #--Reset?
        if fileName == 'SAME': 
            if not self.saveInfo or self.saveInfo.name not in mosh.saveInfos:
                fileName = None
            else:
                fileName = self.saveInfo.name
        #--Null fileName?
        if not fileName:
            saveInfo = self.saveInfo = None
            self.fileStr = ''
            self.saveNameStr = ''
            self.playerNameStr = ''
            self.curCellStr = ''
            self.versionStr = ''
            self.picData = None
        #--Valid fileName?
        else:
            saveInfo = self.saveInfo = mosh.saveInfos[fileName]
            #--Remember values for edit checks
            self.fileStr = saveInfo.name
            self.saveNameStr = saveInfo.tes3.hedr.description
            self.playerNameStr = saveInfo.tes3.gmdt.playerName
            self.curCellStr = saveInfo.tes3.gmdt.curCell
            self.versionStr = 'v%0.1f' % (saveInfo.tes3.hedr.version,)
            self.picData = self.saveInfo.getScreenshot()
        #--Set Fields
        self.file.SetValue(self.fileStr)
        self.saveName.SetValue(self.saveNameStr)
        self.playerName.SetValue(self.playerNameStr)
        self.curCell.SetValue(self.curCellStr)
        self.version.SetLabel(self.versionStr)
        self.masters.SetFileInfo(saveInfo)
        #--Picture
        if not self.picData:
            self.picture.SetBitmap(None)
        else:
            image = wx.EmptyImage(128,128)
            image.SetData(self.picData)
            image = image.Scale(171,128)
            self.picture.SetBitmap(image.ConvertToBitmap())
        #--Edit State
        self.edited = 0
        self.save.Disable()
        self.cancel.Disable()

    def SetEdited(self):
        """Mark as edited."""
        self.edited = True
        self.save.Enable()
        self.cancel.Enable()
    
    def OnBrowser(self,event):
        """Event: Clicked Journal Browser button."""
        if not journalBrowser: 
            JournalBrowser().Show()
            settings['mash.journal.show'] = True
        if self.saveInfo:
            journalBrowser.SetSave(self.saveInfo.name)
        journalBrowser.Raise()

    def OnTextEdit(self,event):
        """Event: Editing file or save name text."""
        if self.saveInfo and not self.edited:
            if ((self.fileStr != self.file.GetValue()) or
                (self.saveNameStr != self.saveName.GetValue())):
                self.SetEdited()
        event.Skip()

    def OnEditFile(self,event):
        """Event: Finished editing file name."""
        if not self.saveInfo: return
        #--Changed?
        fileStr = self.file.GetValue()
        if fileStr == self.fileStr: return
        #--Extension Changed?
        if fileStr[-4:].lower() != self.fileStr[-4:].lower():
            ErrorMessage(self,"Incorrect file extension: "+fileStr[-3:])
            self.file.SetValue(self.fileStr)
        #--Else file exists?
        elif os.path.exists(os.path.join(self.saveInfo.dir,fileStr)):
            ErrorMessage(self,"File %s already exists." % (fileStr,))
            self.file.SetValue(self.fileStr)
        #--Okay?
        else:
            self.fileStr = fileStr
            self.SetEdited()

    def OnEditSaveName(self,event):
        """Event: Finished editing save name."""
        if not self.saveInfo: return
        saveNameStr = self.saveName.GetValue()
        if saveNameStr != self.saveNameStr:
            self.saveNameStr = saveNameStr
            self.SetEdited()
    
    def OnSave(self,event):
        """Event: Clicked Save button."""
        saveInfo = self.saveInfo
        #--Change Tests
        changeName = (self.fileStr != saveInfo.name)
        changeHedr = (self.saveNameStr != saveInfo.tes3.hedr.description )
        changeMasters = self.masters.edited
        #--Backup
        saveInfo.makeBackup()
        prevMTime = saveInfo.mtime
        #--Change Name?
        if changeName:
            (oldName,newName) = (saveInfo.name,self.fileStr.strip())
            saveList.items[saveList.items.index(oldName)] = newName
            mosh.saveInfos.rename(oldName,newName)
        #--Change hedr?
        if changeHedr:
            saveInfo.tes3.hedr.description = self.saveNameStr.strip()
            saveInfo.tes3.hedr.changed = True
            saveInfo.writeHedr()
        #--Change masters?
        if changeMasters:
            newMasters = self.masters.GetNewMasters()
            (modMap,objMaps) = self.masters.GetMaps()
            #--Create and use FileRefs
            progress = None
            try:
                progress = ProgressDialog(_('Saving'))
                fileRefs = mosh.FileRefs(saveInfo,progress=progress)
                progress.setBaseScale(0.0,0.67)
                fileRefs.load()
                progress(1.0,_('Remap Masters'))
                fileRefs.remap(newMasters,modMap,objMaps)
                progress.setBaseScale(0.67,0.33)
                fileRefs.safeSave()
            finally:
                if progress != None: progress = progress.Destroy()
        #--Restore Date?
        if (changeHedr or changeMasters):
            saveInfo.setMTime(prevMTime)
        #--Done
        try:
            mosh.saveInfos.refreshFile(saveInfo.name)
            self.SetFile(self.saveInfo.name)
        except mosh.Tes3Error:
            ErrorMessage(self,_('File corrupted on save!'))
            self.SetFile(None)
        self.SetFile(self.saveInfo.name)
        saveList.Refresh(saveInfo.name)

    def OnCancel(self,event):
        """Event: Clicked cancel button."""
        self.SetFile(self.saveInfo.name)

#------------------------------------------------------------------------------
class SavePanel(NotebookPanel):
    def __init__(self,parent):
        wx.Panel.__init__(self, parent, -1)
        sizer = wx.BoxSizer(wx.HORIZONTAL)
        global saveList
        saveList = SaveList(self)
        sizer.Add(saveList,1,wx.GROW)
        sizer.Add((4,-1),0)
        self.saveDetails = SaveDetails(self)
        saveList.details = self.saveDetails
        sizer.Add(self.saveDetails,0,wx.EXPAND)
        self.SetSizer(sizer)
        self.saveDetails.Fit()
        #--Events
        wx.EVT_SIZE(self,self.OnSize)

    def SetStatusCount(self):
        """Sets mod count in last field."""
        text = _("Saves: %d") % (len(mosh.saveInfos.data))
        statusBar.SetStatusText(text,2)

    def OnSize(self,event=None):
        wx.Window.Layout(self)
        saveList.Layout()
        self.saveDetails.Layout()

#------------------------------------------------------------------------------
class InstallersPanel(SashTankPanel):
    """Panel for InstallersTank."""
    mainMenu = Links()
    itemMenu = Links()

    def __init__(self,parent):
        """Initialize."""
        global gInstallers
        gInstallers = self
        data = mosh.InstallersData()
        SashTankPanel.__init__(self,data,parent)
        left,right = self.left,self.right
        #--Refreshing
        self.refreshed = False
        self.refreshing = False
        self.frameActivated = False
        self.fullRefresh = False
        #--Contents
        self.gList = balt.Tank(left,data, 
            installercons, InstallersPanel.mainMenu, InstallersPanel.itemMenu,
            details=self, style=wx.LC_REPORT)
        self.gList.SetSizeHints(100,100)
        #--Package
        self.gPackage = wx.TextCtrl(right,-1,style=wx.TE_READONLY|wx.NO_BORDER)
        self.gPackage.SetBackgroundColour(self.GetBackgroundColour())
        #--Info Tabs
        self.gNotebook = wx.Notebook(right,style=wx.NB_MULTILINE)
        self.infoPages = []
        infoTitles = (
            ('gGeneral',_("General")),
            ('gMatched',_("Matched")),
            ('gMissing',_("Missing")),
            ('gMismatched',_("Mismatched")),
            ('gConflicts',_("Conflicts")),
            ('gUnderrides',_("Underridden")),
            ('gDirty',_("Dirty")),
            ('gSkipped',_("Skipped")),
            )
        for name,title in infoTitles:
            gPage = wx.TextCtrl(self.gNotebook,-1,style=wx.TE_MULTILINE|wx.TE_READONLY|wx.HSCROLL,name=name)
            self.gNotebook.AddPage(gPage,title)
            self.infoPages.append([gPage,False])
        self.gNotebook.SetSelection(settings['bash.installers.page'])
        self.gNotebook.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED,self.OnShowInfoPage)
        #--Sub-Intallers
        self.gSubList = wx.CheckListBox(right,-1)
        self.gSubList.Bind(wx.EVT_CHECKLISTBOX,self.OnCheckSubItem)
        #--Espms
        self.espms = []
        self.gEspmList = wx.CheckListBox(right,-1)
        self.gEspmList.Bind(wx.EVT_CHECKLISTBOX,self.OnCheckEspmItem)
        #--Comments
        self.gComments = wx.TextCtrl(right,-1,style=wx.TE_MULTILINE)
        #--Events
        self.Bind(wx.EVT_SIZE,self.OnSize)
        #--Layout
        right.SetSizer(vSizer(
            (self.gPackage,0,wx.GROW|wx.TOP|wx.LEFT,4),
            (self.gNotebook,2,wx.GROW|wx.TOP,0),
            (hSizer(
                (vSizer(
                    (staticText(right,_('Sub-Packages')),),
                    (self.gSubList,1,wx.GROW|wx.TOP,4),
                    ),1,wx.GROW),
                (vSizer(
                    (staticText(right,_('Esp/m Filter')),),
                    (self.gEspmList,1,wx.GROW|wx.TOP,4),
                    ),1,wx.GROW|wx.LEFT,2),
                ),1,wx.GROW|wx.TOP,4),
            (staticText(right,_('Comments')),0,wx.TOP,4),
            (self.gComments,1,wx.GROW|wx.TOP,4),
            ))
        wx.LayoutAlgorithm().LayoutWindow(self, right)

    def OnShow(self):
        """Panel is shown. Update self.data."""
        if settings.get('bash.installers.isFirstRun',True):
            settings['bash.installers.isFirstRun'] = False
            message = _("Do you want to enable Installers If you do, Bash will first need to initialize some data. If you have many mods installed, this can take on the order of five minutes.\n\nIf you prefer to not enable Installers at this time, you can always enable it later from the column header context menu.")
            settings['bash.installers.enabled'] = balt.askYes(self,fill(message,80),self.data.title)
        if not settings['bash.installers.enabled']: return
        if self.refreshing: return
        data = self.gList.data
        if not self.refreshed or (self.frameActivated and (
            data.refreshRenamedNeeded() or data.refreshInstallersNeeded())
            ):
            self.refreshing = True
            progress = balt.Progress(_("Refreshing Installers..."),'\n'+' '*60)
            try:
                what = ('DIS','I')[self.refreshed]
                if data.refresh(progress,what,self.fullRefresh):
                    self.gList.RefreshUI()
                self.fullRefresh = False
                self.frameActivated = False
                self.refreshing = False
                self.refreshed = True
            finally:
                if progress != None: progress.Destroy()
        self.SetStatusCount()

    def OnShowInfoPage(self,event):
        """A specific info page has been selected."""
        if event.GetId() == self.gNotebook.GetId():
            index = event.GetSelection()
            gPage,initialized = self.infoPages[index]
            if self.detailsItem and not initialized:
                self.RefreshInfoPage(index,self.data[self.detailsItem])
            event.Skip()

    def SetStatusCount(self):
        """Sets status bar count field."""
        active = len([x for x in self.data.itervalues() if x.isActive])
        text = _('Packages: %d/%d') % (active,len(self.data.data))
        statusBar.SetStatusText(text,2)

    #--Details view (if it exists)
    def SaveDetails(self):
        """Saves details if they need saving."""
        settings['bash.installers.page'] = self.gNotebook.GetSelection()
        if not self.detailsItem: return
        if not self.gComments.IsModified(): return
        installer = self.data[self.detailsItem]
        installer.comments = self.gComments.GetValue()
        self.data.setChanged()

    def RefreshUIMods(self):
        """Refresh UI plus refresh mods state."""
        self.gList.RefreshUI()
        if mosh.modInfos.refresh():
            del mosh.modInfos.mtimesReset[:]
            modList.Refresh('ALL')

    def RefreshDetails(self,item=None):
        """Refreshes detail view associated with data from item."""
        if item not in self.data: item = None
        self.SaveDetails() #--Save previous details
        self.detailsItem = item
        del self.espms[:]
        if item:
            installer = self.data[item]
            #--Name
            self.gPackage.SetValue(item.s)
            #--Info Pages
            currentIndex = self.gNotebook.GetSelection()
            for index,(gPage,state) in enumerate(self.infoPages):
                self.infoPages[index][1] = False
                if (index == currentIndex): self.RefreshInfoPage(index,installer)
                else: gPage.SetValue('')
            #--Sub-Packages
            self.gSubList.Clear()
            if len(installer.subNames) <= 2:
                self.gSubList.Clear()
            else:
                balt.setCheckListItems(self.gSubList, installer.subNames[1:],installer.subActives[1:])
            #--Espms
            if not installer.espms:
                self.gEspmList.Clear()
            else:
                names = self.espms = sorted(installer.espms)
                names.sort(key=lambda x: x.cext != '.esm')
                balt.setCheckListItems(self.gEspmList, [x.s for x in names],
                    [x not in installer.espmNots for x in names])
            #--Comments
            self.gComments.SetValue(installer.comments)
        else:
            self.gPackage.SetValue('')
            for index,(gPage,state) in enumerate(self.infoPages):
                self.infoPages[index][1] = True
                gPage.SetValue('')
            self.gSubList.Clear()
            self.gEspmList.Clear()
            self.gComments.SetValue('')
    
    def RefreshInfoPage(self,index,installer):
        """Refreshes notebook page."""
        gPage,initialized = self.infoPages[index]
        if initialized: return
        else: self.infoPages[index][1] = True
        pageName = gPage.GetName()
        sNone = _('[None]')
        def sortKey(file):
            dirFile = file.lower().rsplit('\\',1)
            if len(dirFile) == 1: dirFile.insert(0,'')
            return dirFile
        def dumpFiles(files,default='',header='',isPath=False):
            if files:
                buff = cStringIO.StringIO()
                if isPath: files = [x.s for x in files]
                else: files = list(files)
                sortKeys = dict((x,sortKey(x)) for x in files)
                files.sort(key=lambda x: sortKeys[x])
                if header: buff.write(header+'\n')
                for file in files:
                    buff.write(file)
                    buff.write('\n')
                return buff.getvalue()
            elif header:
                return header+'\n'
            else:
                return ''
        if pageName == 'gGeneral':
            info = _("== Overview\n")
            info += _("Type: ") 
            info += (_('Archive'),_('Project'))[isinstance(installer,mosh.InstallerProject)] 
            info += '\n'
            if installer.type == 1:
                info += _("Structure: Simple\n")
            elif installer.type == 2:
                if len(installer.subNames) == 2:
                    info += _("Structure: Complex/Simple\n")
                else:
                    info += _("Structure: Complex\n")
            elif installer.type < 0:
                 info += _("Structure: Corrupt/Incomplete\n")
            else:
                info += _("Structure: Unrecognized\n")
            nConfigured = len(installer.data_sizeCrc)
            nMissing = len(installer.missingFiles)
            nMismatched = len(installer.mismatchedFiles)
            info += _("Compressed: %s kb\n") % formatInteger(installer.size/1024)
            info += _("Files: %s\n") % formatInteger(len(installer.fileSizeCrcs))
            info += _("Configured: %s (%s kb)\n") % (
                formatInteger(nConfigured), formatInteger(installer.unSize/1024))
            info += _("  Matched: %s\n") % formatInteger(nConfigured-nMissing-nMismatched)
            info += _("  Missing: %s\n") % formatInteger(nMissing)
            info += _("  Conflicts: %s\n") % formatInteger(nMismatched)
            info += '\n'
            #--Infoboxes
            gPage.SetValue(info+dumpFiles(installer.data_sizeCrc,sNone,
                _("== Configured Files"),isPath=True))
        elif pageName == 'gMatched':
            gPage.SetValue(dumpFiles(set(installer.data_sizeCrc) 
                - installer.missingFiles - installer.mismatchedFiles,isPath=True))
        elif pageName == 'gMissing':
            gPage.SetValue(dumpFiles(installer.missingFiles,isPath=True))
        elif pageName == 'gMismatched':
            gPage.SetValue(dumpFiles(installer.mismatchedFiles,sNone,isPath=True))
        elif pageName == 'gConflicts':
            gPage.SetValue(self.data.getConflictReport(installer,'OVER'))
        elif pageName == 'gUnderrides':
            gPage.SetValue(self.data.getConflictReport(installer,'UNDER'))
        elif pageName == 'gDirty':
            gPage.SetValue(dumpFiles(installer.dirty_sizeCrc,isPath=True))
        elif pageName == 'gSkipped':
            gPage.SetValue('\n'.join((
                dumpFiles(installer.skipExtFiles,sNone,_('== Skipped (Extension)')),
                dumpFiles(installer.skipDirFiles,sNone,_('== Skipped (Dir)')),
                )) or sNone)

    #--Config
    def refreshCurrent(self,installer):
        """Refreshes current item while retaining scroll positions."""
        installer.refreshDataSizeCrc()
        installer.refreshStatus(self.data)
        subScrollPos  = self.gSubList.GetScrollPos(wx.VERTICAL)
        espmScrollPos = self.gEspmList.GetScrollPos(wx.VERTICAL)
        self.gList.RefreshUI(self.detailsItem)
        self.gSubList.ScrollLines(subScrollPos)
        self.gEspmList.ScrollLines(espmScrollPos)

    def OnCheckSubItem(self,event):
        """Handle check/uncheck of item."""
        installer = self.data[self.detailsItem]
        for index in range(self.gSubList.GetCount()):
            installer.subActives[index+1] = self.gSubList.IsChecked(index)
        self.refreshCurrent(installer)

    def OnCheckEspmItem(self,event):
        """Handle check/uncheck of item."""
        installer = self.data[self.detailsItem]
        espmNots = installer.espmNots
        for index,espm in enumerate(self.espms):
            if self.gEspmList.IsChecked(index):
                espmNots.discard(espm)
            else: 
                espmNots.add(espm)
        self.refreshCurrent(installer)

#------------------------------------------------------------------------------
class ScreensList(List):
    #--Class Data
    mainMenu = Links() #--Column menu
    itemMenu = Links() #--Single item menu

    def __init__(self,parent):
        #--Columns
        self.cols = settings['bash.screens.cols']
        self.colAligns = settings['bash.screens.colAligns']
        self.colNames = settings['mash.colNames']
        self.colReverse = settings.getChanged('bash.screens.colReverse')
        self.colWidths = settings['bash.screens.colWidths']
        #--Data/Items
        self.data = mosh.screensData = mosh.ScreensData()
        self.sort = settings['bash.screens.sort']
        #--Links
        self.mainMenu = ScreensList.mainMenu
        self.itemMenu = ScreensList.itemMenu
        #--Parent init
        List.__init__(self,parent,-1,ctrlStyle=(wx.LC_REPORT|wx.SUNKEN_BORDER))
        #--Events
        wx.EVT_LIST_ITEM_SELECTED(self,self.listId,self.OnItemSelected)

    def RefreshUI(self,files='ALL',detail='SAME'):
        """Refreshes UI for specified files."""
        #--Details
        if detail == 'SAME':
            selected = set(self.GetSelected())
        else:
            selected = set([detail])
        #--Populate
        if files == 'ALL':
            self.PopulateItems(selected=selected)
        elif isinstance(files,StringTypes):
            self.PopulateItem(files,selected=selected)
        else: #--Iterable
            for file in files:
                self.PopulateItem(file,selected=selected)
        mashFrame.SetStatusCount()

    #--Populate Item
    def PopulateItem(self,itemDex,mode=0,selected=set()):
        #--String name of item?
        if not isinstance(itemDex,int):
            itemDex = self.items.index(itemDex)
        fileName = GPath(self.items[itemDex])
        fileInfo = self.data[fileName]
        cols = self.cols
        for colDex in range(self.numCols):
            col = cols[colDex]
            if col == 'File':
                value = fileName.s
            elif col == 'Modified':
                value = formatDate(fileInfo[1])
            else:
                value = '-'
            if mode and (colDex == 0):
                self.list.InsertStringItem(itemDex, value)
            else:
                self.list.SetStringItem(itemDex, colDex, value)
        #--Image
        #--Selection State
        if fileName in selected:
            self.list.SetItemState(itemDex,wx.LIST_STATE_SELECTED,wx.LIST_STATE_SELECTED)
        else:
            self.list.SetItemState(itemDex,0,wx.LIST_STATE_SELECTED)

    #--Sort Items
    def SortItems(self,col=None,reverse=-2):
        (col, reverse) = self.GetSortSettings(col,reverse)
        settings['bash.screens.sort'] = col
        data = self.data
        #--Start with sort by name
        self.items.sort()
        if col == 'File':
            pass #--Done by default
        elif col == 'Modified':
            self.items.sort(key=lambda a: data[a][1])
        else:
            raise BashError(_('Unrecognized sort key: ')+col)
        #--Ascending
        if reverse: self.items.reverse()

    #--Events ---------------------------------------------
    #--Column Resize
    def OnColumnResize(self,event):
        colDex = event.GetColumn()
        colName = self.cols[colDex]
        self.colWidths[colName] = self.list.GetColumnWidth(colDex)
        settings.setChanged('bash.screens.colWidths')

    def OnItemSelected(self,event=None):
        fileName = self.items[event.m_itemIndex]
        filePath = mosh.screensData.dir.join(fileName)
        bitmap = (filePath.exists() and wx.Bitmap(filePath.s)) or None
        self.picture.SetBitmap(bitmap)

#------------------------------------------------------------------------------
class ScreensPanel(NotebookPanel):
    """Screenshots tab."""
    def __init__(self,parent):
        """Initialize."""
        wx.Panel.__init__(self, parent, -1)
        #--Left
        sashPos = settings.get('bash.screens.sashPos',120)
        left = self.left = leftSash(self,defaultSize=(sashPos,100),onSashDrag=self.OnSashDrag)
        right = self.right =  wx.Panel(self,style=wx.NO_BORDER)
        #--Contents
        global screensList
        screensList = ScreensList(left)
        screensList.SetSizeHints(100,100)
        screensList.picture = balt.Picture(right,256,192)
        #--Events
        self.Bind(wx.EVT_SIZE,self.OnSize)
        #--Layout
        #left.SetSizer(hSizer((screensList,1,wx.GROW),((10,0),0)))
        right.SetSizer(hSizer((screensList.picture,1,wx.GROW)))
        wx.LayoutAlgorithm().LayoutWindow(self, right)

    def SetStatusCount(self):
        """Sets status bar count field."""
        text = _('Screens: %d') % (len(screensList.data.data),)
        statusBar.SetStatusText(text,2)

    def OnSashDrag(self,event):
        """Handle sash moved."""
        wMin,wMax = 80,self.GetSizeTuple()[0]-80
        sashPos = max(wMin,min(wMax,event.GetDragRect().width))
        self.left.SetDefaultSize((sashPos,10))
        wx.LayoutAlgorithm().LayoutWindow(self, self.right)
        screensList.picture.Refresh()
        settings['bash.screens.sashPos'] = sashPos

    def OnSize(self,event=None):
        wx.LayoutAlgorithm().LayoutWindow(self, self.right)

    def OnShow(self):
        """Panel is shown. Update self.data."""
        if mosh.screensData.refresh():
            screensList.RefreshUI()
            #self.Refresh()
        self.SetStatusCount()

#------------------------------------------------------------------------------
class MashNotebook(wx.Notebook):
    def __init__(self, parent, id):
        wx.Notebook.__init__(self, parent, id)
        self.AddPage(InstallersPanel(self),_("Installers"))
        self.AddPage(ModPanel(self),_("Mods"))
        self.AddPage(SavePanel(self),_("Saves"))
        self.AddPage(ScreensPanel(self),_("Screenshots"))
        #--Selection
        pageIndex = settings['mash.page']
        if settings['bash.installers.fastStart'] and pageIndex == 0:
            pageIndex = 1
        self.SetSelection(pageIndex)
        #--Events
        self.Bind(wx.EVT_NOTEBOOK_PAGE_CHANGED,self.OnShowPage)

    def OnShowPage(self,event):
        """Call page's OnShow command."""
        if event.GetId() == self.GetId():
            self.GetPage(event.GetSelection()).OnShow()
            event.Skip()

#------------------------------------------------------------------------------
class MashStatusBar(wx.StatusBar):
    #--Class Data
    buttons = Links()

    def __init__(self, parent):
        wx.StatusBar.__init__(self, parent, -1)
        global statusBar
        statusBar = self
        self.SetFieldsCount(3)
        buttons = MashStatusBar.buttons
        self.size = int(mosh.inisettings['iconSize'])
        self.size += 8
        self.buttons = []
        for link in buttons:
            gButton = link.GetBitmapButton(self,style=wx.NO_BORDER)
            if gButton: self.buttons.append(gButton)
        self.SetStatusWidths([self.size*len(self.buttons),-1, 120])
        self.SetSize((-1, self.size))
        self.GetParent().SendSizeEvent()
        self.OnSize() #--Position buttons
        wx.EVT_SIZE(self,self.OnSize)
        #--Clear text notice
        self.Bind(wx.EVT_TIMER, self.OnTimer)

    def OnSize(self,event=None):
        rect = self.GetFieldRect(0)
        (xPos,yPos) = (rect.x+1,rect.y+1)
        for button in self.buttons:
            button.SetPosition((xPos,yPos))
            xPos += self.size  
        if event: event.Skip()

    def SetText(self,text="",timeout=5):
        """Set's display text as specified. Empty string clears the field."""
        self.SetStatusText(text,1)
        if timeout > 0:
            wx.Timer(self).Start(timeout*1000,wx.TIMER_ONE_SHOT)

    def OnTimer(self,evt):
        """Clears display text as specified. Empty string clears the field."""
        self.SetStatusText("",1)

#------------------------------------------------------------------------------
class MashFrame(wx.Frame):
    """Main application frame."""
    def __init__(self, parent=None,pos=wx.DefaultPosition,size=(400,500),
             style = wx.DEFAULT_FRAME_STYLE):
        """Initialization."""
        #--Singleton
        global mashFrame
        mashFrame = self
        #--Window
        wx.Frame.__init__(self, parent, -1, 'Wrye Mash', pos, size,style)
        minSize = settings['mash.frameSize.min']
        self.SetSizeHints(minSize[0],minSize[1])
        self.SetTitle()
        #--Application Icons
        self.SetIcons(images['mash.icons'].GetIconBundle())
        #--Status Bar
        self.SetStatusBar(MashStatusBar(self))
        #--Sizer
        self.sizer = wx.BoxSizer(wx.VERTICAL)
        #--Notebook panel
        self.notebook = notebook = MashNotebook(self,-1)
        #self.sizer.Add(wx.NotebookSizer(notebook),1,wx.GROW)
        self.sizer.Add(notebook,1,wx.GROW)
        #--Layout
        self.SetSizer(self.sizer)
        #--Events
        wx.EVT_CLOSE(self, self.OnCloseWindow)
        wx.EVT_ACTIVATE(self, self.RefreshData)
        #--Data
        self.knownCorrupted = set() 

    def SetTitle(self,title=None):
        """Set title. Set to default if no title supplied."""
        if not title:
            title = "Wrye Mash %s" % (settings['mash.readme'][1],)
            if 'mash.profile' in settings:
                title += ': ' + settings['mash.profile']
        wx.Frame.SetTitle(self,title)

    def SetStatusCount(self):
        """Sets the status bar count field. Actual work is done by current panel."""
        if hasattr(self,'notebook'): #--Hack to get around problem with screens tab.
            self.notebook.GetPage(self.notebook.GetSelection()).SetStatusCount()

    #--Events ---------------------------------------------
    def RefreshData(self, event=None):
        """Refreshes all data. Can be called manually, but is also triggered by window activation event."""
        #--Ignore deactivation events.
        if event and not event.GetActive(): return
        #--UPDATES-----------------------------------------
        popMods = popSaves = None
        #--Check morrowind.ini and mods directory...
        if mosh.mwIniFile.refresh() | mosh.modInfos.refresh():
            mosh.mwIniFile.refreshDoubleTime()
            popMods = 'ALL'
        #--Have any mtimes been reset?
        if mosh.modInfos.mtimesReset:
            resetList = '\n* '.join(mosh.modInfos.mtimesReset)
            del mosh.modInfos.mtimesReset[:]
            InfoMessage(self,_('Modified dates have been reset for some mod files.\n* ')+resetList)
            popMods = 'ALL'
        #--Check savegames directory...
        if mosh.saveInfos.refresh():
            popSaves = 'ALL'
        #--Repopulate
        if popMods:
            modList.Refresh(popMods) #--Will repop saves too.
        elif popSaves:
            saveList.Refresh(popSaves)
        #--Current notebook panel
        if gInstallers: gInstallers.frameActivated = True
        self.notebook.GetPage(self.notebook.GetSelection()).OnShow()
        #--WARNINGS----------------------------------------
        #--Does morrowind.ini have any bad or missing files?
        if mosh.mwIniFile.loadFilesBad:
            message = (_("Missing files have been removed from load list. (%s)") 
                % (', '.join(mosh.mwIniFile.loadFilesBad),))
            mosh.mwIniFile.safeSave()
            WarningMessage(self,message)
        #--Was load list too long?
        if mosh.mwIniFile.loadFilesExtra:
            message = (_("Load list has been truncated because it was too long. (%s)") 
                % (', '.join(mosh.mwIniFile.loadFilesExtra),))
            mosh.mwIniFile.safeSave()
            WarningMessage(self,message)
        #--Any new corrupted files?
        message = ''
        corruptMods = set(mosh.modInfos.corrupted.keys())
        if not corruptMods <= self.knownCorrupted:
            message += _("The following mod files have corrupted headers: ")
            message += ','.join(sorted(corruptMods))+'.'
            self.knownCorrupted |= corruptMods
        corruptSaves = set(mosh.saveInfos.corrupted.keys())
        if not corruptSaves <= self.knownCorrupted:
            if message: message += '\n'
            message += _("The following save files have corrupted headers: ")
            message += ','.join(sorted(corruptSaves))+'.'
            self.knownCorrupted |= corruptSaves
        if message: WarningMessage(self,message)
        #--Any Y2038 Resets?
        if mosh.y2038Resets:
            message = (_("Mash cannot handle dates greater than January 19, 2038. Accordingly, the dates for the following files have been reset to an earlier date: ") +
                ', '.join(sorted(mosh.y2038Resets))+'.')
            del mosh.y2038Resets[:]
            WarningMessage(self,message)

    def OnCloseWindow(self, event):
        """Handle Close event. Save application data."""
        self.CleanSettings()
        if docBrowser: docBrowser.DoSave()
        if not self.IsIconized() and not self.IsMaximized():
            settings['mash.framePos'] = self.GetPosition()
            settings['mash.frameSize'] = self.GetSizeTuple()
        settings['mash.page'] = self.notebook.GetSelection()
        mosh.modInfos.table.save()
        for index in range(self.notebook.GetPageCount()):
            self.notebook.GetPage(index).OnCloseWindow()
        settings.save()
        self.Destroy()

    def CleanSettings(self):
        """Cleans junk from settings before closing."""
        #--Clean rename dictionary.
        modNames = set(mosh.modInfos.data.keys())
        modNames.update(mosh.modInfos.table.data.keys())
        renames = mosh.settings.getChanged('mash.mods.renames')
        for key,value in renames.items():
            if value not in modNames:
                del renames[key]
        #--Clean backup
        for fileInfos in (mosh.modInfos,mosh.saveInfos):
            goodNames = set(fileInfos.data.keys())
            backupDir = os.path.join(fileInfos.dir,settings['mosh.fileInfo.backupDir'])
            if not os.path.isdir(backupDir): continue
            for name in sorted(os.listdir(backupDir)):
                path = os.path.join(backupDir,name)
                if name[-1] == 'f': name = name[:-1]
                if name not in goodNames and os.path.isfile(path):
                    os.remove(path)

#------------------------------------------------------------------------------
class DocBrowser(wx.Frame):
    """Doc Browser frame."""
    def __init__(self,modName=None):
        """Intialize.
        modName -- current modname (or None)."""
        #--Data
        self.modName = modName
        self.data = mosh.modInfos.table.getColumn('doc')
        self.docEdit = mosh.modInfos.table.getColumn('docEdit')
        self.docType = None
        self.docIsWtxt = False
        #--Singleton
        global docBrowser
        docBrowser = self
        #--Window
        pos = settings['mash.modDocs.pos']
        size = settings['mash.modDocs.size']
        wx.Frame.__init__(self, mashFrame, -1, _('Doc Browser'), pos, size,
            style=wx.DEFAULT_FRAME_STYLE)
        self.SetBackgroundColour(wx.NullColour)
        self.SetSizeHints(250,250)
        #--Mod Name
        self.modNameBox = wx.TextCtrl(self,-1,style=wx.TE_READONLY)
        self.modNameList = wx.ListBox(self,-1,choices=sorted(self.data.keys()),style=wx.LB_SINGLE|wx.LB_SORT)
        self.modNameList.Bind(wx.EVT_LISTBOX,self.DoSelectMod)
        #--Application Icons
        self.SetIcons(images['mash.icons2'].GetIconBundle())
        #--Set Doc
        self.setButton = wx.Button(self,ID_SET,_("Set Doc..."))
        wx.EVT_BUTTON(self.setButton,ID_SET,self.DoSet)
        #--Forget Doc
        self.forgetButton = wx.Button(self,wx.ID_DELETE,_("Forget Doc..."))
        wx.EVT_BUTTON(self.forgetButton,wx.ID_DELETE,self.DoForget)
        #--Rename Doc
        self.renameButton = wx.Button(self,ID_RENAME,_("Rename Doc..."))
        wx.EVT_BUTTON(self.renameButton,ID_RENAME,self.DoRename)
        #--Edit Doc
        self.editButton = wx.ToggleButton(self,ID_EDIT,_("Edit Doc..."))
        wx.EVT_TOGGLEBUTTON(self.editButton,ID_EDIT,self.DoEdit)
        #--Html Back
        self.prevButton = wx.Button(self,ID_BACK,"<<")
        wx.EVT_BUTTON(self.prevButton,ID_BACK,self.DoPrevPage)
        #--Html Forward
        self.nextButton = wx.Button(self,ID_NEXT,">>")
        wx.EVT_BUTTON(self.nextButton,ID_NEXT,self.DoNextPage)
        #--Doc Name
        self.docNameBox = wx.TextCtrl(self,-1,style=wx.TE_READONLY)
        #--Doc display
        self.plainText = wx.TextCtrl(self,-1,style=wx.TE_READONLY|wx.TE_MULTILINE|wx.TE_RICH2)
        self.htmlText = wx.lib.iewin.IEHtmlWindow(self, -1, style = wx.NO_FULL_REPAINT_ON_RESIZE)
        #--Events
        wx.EVT_CLOSE(self, self.OnCloseWindow)
        #--Layout
        self.mainSizer = vSizer(
            (hSizer( #--Buttons
                (self.setButton,0,wx.GROW),
                (self.forgetButton,0,wx.GROW),
                (self.renameButton,0,wx.GROW),
                (self.editButton,0,wx.GROW),
                (self.prevButton,0,wx.GROW),
                (self.nextButton,0,wx.GROW),
                ),0,wx.GROW|wx.ALL^wx.BOTTOM,4),
            (hSizer( #--Mod name, doc name
                #(self.modNameBox,2,wx.GROW|wx.RIGHT,4),
                (self.docNameBox,2,wx.GROW),
                ),0,wx.GROW|wx.TOP|wx.BOTTOM,4),
            (self.plainText,3,wx.GROW),
            (self.htmlText,3,wx.GROW),
            )
        sizer = hSizer(
            (vSizer(
                (self.modNameBox,0,wx.GROW),
                (self.modNameList,1,wx.GROW|wx.TOP,4),
                ),0,wx.GROW|wx.TOP|wx.RIGHT,4),
            (self.mainSizer,1,wx.GROW),
            )
        #--Set
        self.SetSizer(sizer)
        self.SetMod(modName)
        self.SetDocType('txt')

    def GetIsWtxt(self,docPath=None):
        """Determines whether specified path is a wtxt file."""
        docPath = docPath or self.data.get(self.modName,'')
        if not os.path.exists(docPath): return False
        textFile = file(docPath)
        maText = re.match(r'^=.+=#\s*$',textFile.readline())
        textFile.close()
        return (maText != None)

    def DoHome(self, event):
        """Handle "Home" button click."""
        self.htmlText.GoHome()

    def DoPrevPage(self, event):
        """Handle "Back" button click."""
        self.htmlText.GoBack()

    def DoNextPage(self, event):
        """Handle "Next" button click."""
        self.htmlText.GoForward()

    def DoEdit(self,event):
        """Handle "Edit Doc" button click."""
        self.DoSave()
        editing = self.editButton.GetValue()
        self.docEdit[self.modName] = editing
        self.docIsWtxt = self.GetIsWtxt()
        if self.docIsWtxt:
            self.SetMod(self.modName)
        else:
            self.plainText.SetEditable(editing)

    def DoForget(self,event):
        """Handle "Forget Doc" button click.
        Sets help document for current mod name to None."""
        #--Already have mod data?
        modName = self.modName
        if modName not in self.data:
            return
        index = self.modNameList.FindString(modName)
        if index != wx.NOT_FOUND:
            self.modNameList.Delete(index)
        del self.data[modName]
        self.SetMod(modName)

    def DoSelectMod(self,event):
        """Handle mod name combobox selection."""
        self.SetMod(event.GetString())

    def DoSet(self,event):
        """Handle "Set Doc" button click."""
        #--Already have mod data?
        modName = self.modName
        if modName in self.data:
            (docsDir,fileName) = os.path.split(self.data[modName])
        else:
            docsDir = (settings['mash.modDocs.dir'] or
                os.path.join(settings['mwDir'],'Data Files'))
            fileName = ''
        #--Dialog
        dialog = wx.FileDialog(self,_("Select doc for %s:") % (modName,),
            docsDir,fileName, '*.*', wx.OPEN)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return None
        path = dialog.GetPath()
        dialog.Destroy()
        settings['mash.modDocs.dir'] = os.path.split(path)[0]
        if modName not in self.data:
            self.modNameList.Append(modName)
        self.data[modName] = path
        self.SetMod(modName)

    def DoRename(self,event):
        """Handle "Rename Doc" button click."""
        modName = self.modName
        oldPath = self.data[modName]
        (workDir,fileName) = os.path.split(oldPath)
        #--Dialog
        dialog = wx.FileDialog(self,_("Rename file to:"),
            workDir,fileName, '*.*', wx.SAVE|wx.OVERWRITE_PROMPT)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return None
        path = dialog.GetPath()
        dialog.Destroy()
        #--OS renaming
        if path.lower() == oldPath.lower(): return
        if os.path.exists(path): os.remove(path)
        os.rename(oldPath,path)
        if self.docIsWtxt:
            oldHtml, newHtml = (os.path.splitext(xxx)[0]+'.html' for xxx in (oldPath,path))
            if os.path.exists(newHtml): os.remove(newHtml)
            if os.path.exists(oldHtml): os.rename(oldHtml,newHtml)
        #--Remember change
        settings['mosh.workDir'] = os.path.split(path)[0]
        self.data[modName] = path
        self.SetMod(modName)

    def DoSave(self):
        """Saves doc, if necessary."""
        if not self.plainText.IsModified(): return
        try:
            docPath = self.data.get(self.modName,'')
            if not docPath: 
                raise mosh.Error(_('Filename not defined.'))
            self.plainText.SaveFile(docPath)
            self.plainText.DiscardEdits()
            if self.docIsWtxt:
                import wtxt
                docsDir = os.path.join(mosh.modInfos.dir,'Docs')
                wtxt.genHtml(docPath,cssDir=docsDir)
        except:
            ErrorMessage(self,_("Failed to save changes to %s doc file!" % (self.modName,)))

    def SetMod(self,modName):
        """Sets the mod to show docs for."""
        import mush
        #--Save Current Edits
        self.DoSave()
        #--New modName
        self.modName = modName
        #--ModName
        if modName:
            self.modNameBox.SetValue(modName)
            index = self.modNameList.FindString(modName)
            self.modNameList.SetSelection(index)
            self.setButton.Enable(True)
        else:
            self.modNameBox.SetValue('')
            self.modNameList.SetSelection(wx.NOT_FOUND)
            self.setButton.Enable(False)
        #--Doc Data
        docPath = self.data.get(modName,'')
        docExt = os.path.splitext(docPath)[1].lower()
        self.docNameBox.SetValue(os.path.basename(docPath))
        self.forgetButton.Enable(docPath != '')
        self.renameButton.Enable(docPath != '')
        #--Edit defaults to false.
        self.editButton.SetValue(False)
        self.editButton.Enable(False)
        self.plainText.SetEditable(False)
        self.docIsWtxt = False
        #--View/edit doc.
        if not docPath:
            self.plainText.SetValue('')
            self.SetDocType('txt')
        elif not os.path.exists(docPath):
            myTemplate = os.path.join(mosh.modInfos.dir,'Docs',_('My Readme Template.txt'))
            mashTemplate = os.path.join(mosh.modInfos.dir,'Docs',_('Mash Readme Template.txt'))
            if os.path.exists(myTemplate):
                template = ''.join(open(myTemplate).readlines())
            elif os.path.exists(mashTemplate):
                template = ''.join(open(mashTemplate).readlines())
            else:
                template = '= $modName '+('='*(74-len(modName)))+'#\n'+docPath
            defaultText = string.Template(template).substitute(modName=modName)
            self.plainText.SetValue(defaultText)
            self.SetDocType('txt')
            if docExt in set(('.txt','.etxt')):
                self.editButton.Enable(True)
                editing = self.docEdit.get(modName,True)
                self.editButton.SetValue(editing)
                self.plainText.SetEditable(editing)
            self.docIsWtxt = (docExt == '.txt')
        elif docExt in set(('.htm','.html','.mht')):
            self.htmlText.Navigate(docPath)
            self.SetDocType('html')
        else:
            self.editButton.Enable(True)
            editing = self.docEdit.get(modName,False)
            self.editButton.SetValue(editing)
            self.plainText.SetEditable(editing)
            self.docIsWtxt = self.GetIsWtxt(docPath)
            htmlPath = self.docIsWtxt and (os.path.splitext(docPath)[0]+'.html')
            if htmlPath and (not os.path.exists(htmlPath) or 
                (os.path.getmtime(docPath) > os.path.getmtime(htmlPath))
                ):
                import wtxt
                docsDir = os.path.join(mosh.modInfos.dir,'Docs')
                wtxt.genHtml(docPath,cssDir=docsDir)
            if not editing and htmlPath and os.path.exists(htmlPath):
                self.htmlText.Navigate(htmlPath)
                self.SetDocType('html')
            else:
                self.plainText.LoadFile(docPath)
                self.SetDocType('txt')
        
    #--Set Doc Type
    def SetDocType(self,docType):
        """Shows the plainText or htmlText view depending on document type (i.e. file name extension)."""
        if docType == self.docType: 
            return
        sizer = self.mainSizer
        if docType == 'html':
            sizer.Show(self.plainText,False)
            sizer.Show(self.htmlText,True)
            self.prevButton.Enable(True)
            self.nextButton.Enable(True)
        else:
            sizer.Show(self.plainText,True)
            sizer.Show(self.htmlText,False)
            self.prevButton.Enable(False)
            self.nextButton.Enable(False)
        self.Layout()

    #--Window Closing
    def OnCloseWindow(self, event):
        """Handle window close event.
        Remember window size, position, etc."""
        self.DoSave()
        settings['mash.modDocs.show'] = False
        if not self.IsIconized() and not self.IsMaximized():
            settings['mash.modDocs.pos'] = self.GetPosition()
            settings['mash.modDocs.size'] = self.GetSizeTuple()
        self.Destroy()

#------------------------------------------------------------------------------
class JournalBrowser(wx.Frame):
    """Journal Browser frame."""
    def __init__(self,saveName=None):
        """Intialize.
        saveName -- current saveName (or None)."""
        #--Data
        self.saveName = saveName
        self.data = None
        self.counter = 0
        #--Singleton
        global journalBrowser
        journalBrowser = self
        #--Window
        pos = settings['mash.journal.pos']
        size = settings['mash.journal.size']
        wx.Frame.__init__(self, mashFrame, -1, _('Journal'), pos, size,
            style=wx.DEFAULT_FRAME_STYLE)
        self.SetBackgroundColour(wx.NullColour)
        self.SetSizeHints(250,250)
        #--Application Icons
        self.SetIcons(images['mash.icons2'].GetIconBundle())
        #--Sizers
        #--Doc fields
        mainSizer = wx.BoxSizer(wx.VERTICAL)
        #--Doc display
        self.htmlText = wx.lib.iewin.IEHtmlWindow(self, -1, style = wx.NO_FULL_REPAINT_ON_RESIZE)
        mainSizer.Add(self.htmlText,1,wx.GROW)
        #--Layout
        self.SetSizer(mainSizer)
        #--Events
        wx.EVT_CLOSE(self, self.OnCloseWindow)
        #--Set
        self.SetSave(saveName)

    def SetSave(self,saveName):
        """Sets the mod to show docs for."""
        self.saveName = saveName
        if not saveName:
            text = ''
        elif saveName in mosh.saveInfos:
            text = mosh.saveInfos[saveName].getJournal()
        else:
            text = _('[Savefile %s not found.]') % (saveName,)
        self.htmlText.LoadString(text)

    #--Window Closing
    def OnCloseWindow(self, event):
        """Handle window close event.
        Remember window size, position, etc."""
        settings['mash.journal.show'] = False
        if not self.IsIconized() and not self.IsMaximized():
            settings['mash.journal.pos'] = self.GetPosition()
            settings['mash.journal.size'] = self.GetSizeTuple()
        self.Destroy()

#------------------------------------------------------------------------------
class HelpBrowser(wx.Frame):
    """Help Browser frame."""
    def __init__(self):
        """Intialize."""
        #--Data
        self.data = None
        self.counter = 0
        #--Singleton
        global helpBrowser
        helpBrowser = self
        #--Window
        pos = settings.get('mash.help.pos',(-1,-1))
        size = settings.get('mash.help.size',(400,600))
        wx.Frame.__init__(self, mashFrame, -1, _('Help'), pos, size,
            style=wx.DEFAULT_FRAME_STYLE)
        self.SetBackgroundColour(wx.NullColour)
        self.SetSizeHints(250,250)
        #--Application Icons
        self.SetIcons(images['mash.icons2'].GetIconBundle())
        #--Sizers
        #--Doc fields
        mainSizer = wx.BoxSizer(wx.VERTICAL)
        #--Doc display
        self.htmlText = wx.lib.iewin.IEHtmlWindow(self, -1, style = wx.NO_FULL_REPAINT_ON_RESIZE)
        mainSizer.Add(self.htmlText,1,wx.GROW)
        #--Layout
        self.SetSizer(mainSizer)
        #--Events
        wx.EVT_CLOSE(self, self.OnCloseWindow)
        #--Document
        path = os.path.join(os.getcwd(),_('Wrye Mash.html'))
        self.htmlText.Navigate(path)

    #--Window Closing
    def OnCloseWindow(self, event):
        """Handle window close event.
        Remember window size, position, etc."""
        settings['mash.help.show'] = False
        if not self.IsIconized() and not self.IsMaximized():
            settings['mash.help.pos'] = self.GetPosition()
            settings['mash.help.size'] = self.GetSizeTuple()
        self.Destroy()

#------------------------------------------------------------------------------
class MashApp(wx.App):
    """Mash Application class."""
    def OnInit(self):
        """wxWindows: Initialization handler."""
        #--Check/Set mwDir
        if not self.SetMWDir(): return False
        #--Init Data
        self.InitData()
        self.InitVersion()
        #--Locale (Only in later versions of wxPython??)
        if sys.version[:3] != '2.4':
            wx.Locale(wx.LOCALE_LOAD_DEFAULT)
        #--MWFrame
        frame = MashFrame(
             pos=settings['mash.framePos'], 
             size=settings['mash.frameSize'])
        self.SetTopWindow(frame)
        frame.Show()
        #--DocBrowser, JournalBrowser
        if settings['mash.modDocs.show']:
            DocBrowser().Show()
        if settings['mash.journal.show']:
            JournalBrowser().Show()
        if settings.get('mash.help.show'):
            HelpBrowser().Show()
        return True
    
    def SetMWDir(self):
        """Dialog to select Morrowind installation directory. Called by OnInit()."""
        #--Try parent directory.
        parentDir = os.path.split(os.getcwd())[0]
        if os.path.exists(os.path.join(parentDir,'Morrowind.ini')):
            settings['mwDir'] = parentDir
            mosh.dirs['app'] = GPath(parentDir)
            return True
        #--Already set?
        if os.path.exists(os.path.join(settings['mwDir'],'Morrowind.ini')): 
            return True
        #--Ask user through dialog.
        while True:
            mwDirDialog = wx.DirDialog(None,_("Select your Morrowind installation directory."))
            result = mwDirDialog.ShowModal()
            mwDir = mwDirDialog.GetPath()
            mwDirDialog.Destroy()
            #--User canceled?
            if result != wx.ID_OK: 
                return False
            #--Valid Morrowind install directory?
            elif os.path.exists(os.path.join(mwDir,'Morrowind.ini')): 
                settings['mwDir'] = mwDir
                mosh.dirs['app'] = GPath(mwDir)
                return True
            #--Retry?
            retryDialog = wx.MessageDialog(None,
                _(r'Can\'t find %s\Morrowind.ini! Try again?') % (mwDir,),
                _('Morrowind Install Directory'),wx.YES_NO|wx.ICON_EXCLAMATION)
            result = retryDialog.ShowModal()
            retryDialog.Destroy()
            if result != wx.ID_YES:
                return False

    def InitData(self):
        """Initialize all data. Called by OnInit()."""
        mwDir = settings['mwDir']
        mosh.dirs['app'] = GPath(mwDir)
        mosh.mwIniFile = mosh.MWIniFile(mwDir)
        mosh.mwIniFile.refresh()
        mosh.modInfos = mosh.ModInfos(os.path.join(mwDir,'Data Files'))
        mosh.modInfos.refresh()
        mosh.saveInfos = mosh.SaveInfos(os.path.join(mwDir,'Saves'))
        mosh.saveInfos.refresh()
    
    def InitVersion(self):
        """Peform any version to version conversion. Called by OnInit()."""
        version = settings['mash.version']
        #--Version 0.42: MTimes from settings to ModInfos.table.
        if version < 42:
            mtimeKey = 'mosh.modInfos.mtimes'
            if mtimeKey in settings:
                modCol = mosh.modInfos.table.getColumn('mtime')
                for key,value in settings[mtimeKey].items():
                    modCol[key] = value[0]
                del settings[mtimeKey]
        #--Version 0.50 (0.60?): Genre to group
        if version < 60:
            colGenre = mosh.modInfos.table.getColumn('genre')
            colGroup = mosh.modInfos.table.getColumn('group')
            for fileName in colGenre.keys():
                colGroup[fileName] = colGenre[fileName]
                del colGenre[fileName]
                print fileName
            if settings['mash.mods.sort'] == 'Genre':
                settings['mash.mods.sort'] = 'Group'
            colWidths = settings['mash.mods.colWidths']
            if 'Genre' in colWidths:
                colWidths['Group'] = colWidths['Genre']
                del colWidths['Genre']
                settings.setChanged('mash.mods.colWidths')
        #--Version 0.71: Convert refRemoversdata to tuples
        if version < 71 and 'mash.refRemovers.data' in settings:
            import types
            data = settings['mash.refRemovers.data']
            for remover,path in data.items():
                if isinstance(path,types.StringTypes):
                    data[remover] = (path,)
            settings.setChanged('mash.refRemovers.data')
        #--Current version
        settings['mash.version'] = 71
        #--Version from readme
        readme = GPath(settings['mwDir']).join('Mopy','Wrye Mash.txt')
        if readme.exists() and readme.mtime != settings['mash.readme'][0]:
            reVersion = re.compile("^=== ([\.\d]+) \[")
            for line in readme.open():
                maVersion = reVersion.match(line)
                if maVersion:
                    settings['mash.readme'] = (readme.mtime,maVersion.group(1))
                    break

# Links -----------------------------------------------------------------------
#------------------------------------------------------------------------------
#class Link:
#    """Abstract class for a menuitem or button. These objects are added to a 
#    list, and the menuitems are then built on demand through the AppendToMenu 
#    method. Execution of the command is carried out by the Do method.
#
#    Design allows control items to be created by 1) defining link classes, and 
#    2) creating link objects all at once in an initLinks method. This design 
#    keeps link coding from being buried in the interface that it's attached to.
#    """
#    def __init__(self):
#        self.id = None
#
#    def AppendToMenu(self,menu,window,data):
#        self.window = window
#        self.data = data
#        if not self.id: self.id = wx.NewId()
#        wx.EVT_MENU(window,self.id,self.Execute)
#
#    def Execute(self, event):
#        """Event: link execution."""
#        raise mosh.AbstractError
#
##------------------------------------------------------------------------------
#class SeparatorLink(Link):
#    """Menu item separator line."""
#    def AppendToMenu(self,menu,window,data):
#        menu.AppendSeparator()
#
##------------------------------------------------------------------------------
#class MenuLink(Link):
#    """Submenu. Create this and then add other menu items to its links member."""
#    def __init__(self,name):
#        Link.__init__(self)
#        self.name = name
#        self.links = []
#
#    def AppendToMenu(self,menu,window,data):
#        subMenu = wx.Menu()
#        for link in self.links:
#            link.AppendToMenu(subMenu,window,data)
#        menu.AppendMenu(-1,self.name,subMenu)

# Files Links -----------------------------------------------------------------
#------------------------------------------------------------------------------
class Files_Open(Link):
    """Opens data directory in explorer."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Open...'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle selection."""
        dir = GPath(self.window.data.dir)
        if not dir.exists(): dir.makedirs()
        dir.start()

#------------------------------------------------------------------------------
class Files_SortBy(Link):
    """Sort files by specified key (sortCol)."""
    def __init__(self,sortCol,prefix=''):
        Link.__init__(self)
        self.sortCol = sortCol
        self.sortName = settings['mash.colNames'][sortCol]
        self.prefix = prefix

    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,self.prefix+self.sortName,kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        if window.sort == self.sortCol: menuItem.Check()

    def Execute(self,event):
        """Handle menu selection."""
        self.window.PopulateItems(self.sortCol,-1)

#------------------------------------------------------------------------------
class Files_Unhide(Link):
    """Unhide file(s). (Move files back to Data Files or Save directory.)"""
    def __init__(self,type='mod'):
        Link.__init__(self)
        self.type = type

    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_("Unhide..."))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle menu selection."""
        destDir = self.window.data.dir
        srcDir = os.path.join(destDir,settings['mosh.fileInfo.hiddenDir'])
        if self.type == 'mod':
            wildcard = 'Morrowind Mod Files (*.esp;*.esm)|*.esp;*.esm'
        elif self.type == 'save':
            wildcard = 'Morrowind Save files (*.ess)|*.ess'
        else:
            wildcard = '*.*'
        #--File dialog
        if not os.path.exists(srcDir): os.makedirs(srcDir)
        dialog = wx.FileDialog(self.window,'Unhide files:',srcDir, '', wildcard, 
            wx.OPEN|wx.MULTIPLE)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        srcPaths = dialog.GetPaths()
        dialog.Destroy()
        #--Iterate over Paths
        for srcPath in srcPaths:
            #--Copy from dest directory?
            (newSrcDir,srcFileName) = os.path.split(srcPath)
            if newSrcDir == destDir:
                ErrorMessage(self.window,_("You can't unhide files from this directory."))
                return
            #--File already unhidden?
            destPath = os.path.join(destDir,srcFileName)
            if os.path.exists(destPath):
                WarningMessage(self.window,_("File skipped: %s. File is already present.") 
                    % (srcFileName,))
            #--Move it?
            else:
                shutil.move(srcPath,destPath)
        #--Repopulate
        mashFrame.RefreshData()

# File Links ------------------------------------------------------------------
#------------------------------------------------------------------------------
class File_Delete(Link):
    """Delete the file and all backups."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menu.AppendItem(wx.MenuItem(menu,self.id,_('Delete')))

    def Execute(self,event):
        """Handle menu selection."""
        message = _(r'Delete these files? This operation cannot be undone.')
        message += '\n* ' + '\n* '.join(sorted(self.data))
        dialog = wx.MessageDialog(self.window,message,_('Delete Files'),
            style=wx.YES_NO|wx.ICON_EXCLAMATION)
        if dialog.ShowModal() != wx.ID_YES: 
            dialog.Destroy()
            return
        dialog.Destroy()
        #--Are mods?
        isMod = self.window.data[self.data[0]].isMod()
        #--Do it
        for fileName in self.data:
            self.window.data.delete(fileName)
        #--Refresh stuff
        self.window.Refresh()

#------------------------------------------------------------------------------
class File_Duplicate(Link):
    """Create a duplicate of the file."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Duplicate...'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        data = self.data
        fileName = data[0]
        fileInfo = self.window.data[fileName]
        (root,ext) = os.path.splitext(fileName)
        (destDir,destName,wildcard) = (fileInfo.dir, root+' Copy'+ext,'*'+ext)
        if not os.path.exists(destDir): os.makedirs(destDir)
        dialog = wx.FileDialog(self.window,_('Duplicate as:'),destDir,
            destName,wildcard,wx.SAVE|wx.OVERWRITE_PROMPT)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        (destDir,destName) = os.path.split(dialog.GetPath())
        dialog.Destroy()
        if (destDir == fileInfo.dir) and (destName == fileName):
            ErrorMessage(self.window,_("Files cannot be duplicated to themselves!"))
            return
        self.window.data.copy(fileName,destDir,destName,setMTime=True)
        if destName != fileName:
            self.window.data.table.copyRow(fileName,destName)
        if destDir == fileInfo.dir:
            self.window.Refresh(detail=fileName)
        else:
            self.window.Refresh()

#------------------------------------------------------------------------------
class File_Hide(Link):
    """Hide the file. (Move it to Mash/Hidden directory.)"""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menu.AppendItem(wx.MenuItem(menu,self.id,_('Hide')))

    def Execute(self,event):
        """Handle menu selection."""
        message = _(r'Hide these files? Note that hidden files are simply moved to the Mash\Hidden subdirectory.')
        if ContinueQuery(self.window,message,'mash.hideFiles.continue',_('Hide Files')) != wx.ID_OK:
            return
        #--Do it
        destRoot = os.path.join(self.window.data.dir,settings['mosh.fileInfo.hiddenDir'])
        fileInfos = self.window.data
        fileGroups = fileInfos.table.getColumn('group')
        for fileName in self.data:
            destDir = destRoot
            #--Use author subdirectory instead?
            author = fileInfos[fileName].tes3.hedr.author
            authorDir = os.path.join(destRoot,author)
            if author and os.path.isdir(authorDir):
                destDir = authorDir
            #--Use group subdirectory instead?
            elif fileName in fileGroups:
                groupDir = os.path.join(destRoot,fileGroups[fileName])
                if os.path.isdir(groupDir):
                    destDir = groupDir
            if not self.window.data.moveIsSafe(fileName,destDir):
                message = (_('A file named %s already exists in the hidden files directory. Overwrite it?') 
                    % (fileName,))
                if WarningQuery(self.window,message,_('Hide Files')) != wx.ID_YES: 
                    continue
            self.window.data.move(fileName,destDir)
        #--Refresh stuff
        self.window.Refresh()

#------------------------------------------------------------------------------
class File_MoveTo(Link):
    """Hide the file(s). I.e., move it/them to user selected directory."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menu.AppendItem(wx.MenuItem(menu,self.id,_('Move To...')))

    def Execute(self,event):
        """Handle menu selection."""
        destDir = os.path.join(self.window.data.dir,settings['mosh.fileInfo.hiddenDir'])
        destDir = DirDialog(self.window,_('Move To...'),destDir)
        if not destDir: return
        #--Do it
        fileInfos = self.window.data
        for fileName in self.data:
            if not self.window.data.moveIsSafe(fileName,destDir):
                message = (_('A file named %s already exists in the destination directory. Overwrite it?') 
                    % (fileName,))
                if WarningQuery(self.window,message,_('Hide Files')) != wx.ID_YES: 
                    continue
            self.window.data.move(fileName,destDir)
        #--Refresh stuff
        self.window.Refresh()

#------------------------------------------------------------------------------
class File_Redate(Link):
    """Move the selected files to start at a specified date."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Redate...'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle menu selection."""
        #--Get current start time.
        fileInfos = self.window.data
        #--Ask user for revised time.
        dialog = wx.TextEntryDialog(self.window,_('Redate selected mods starting at...'),
            _('Redate Mods'),formatDate(int(time.time())))
        result = dialog.ShowModal()
        newTimeStr = dialog.GetValue()
        dialog.Destroy()
        if result != wx.ID_OK: return
        try:
            newTimeTup = time.strptime(newTimeStr,'%c')
            newTime = int(time.mktime(newTimeTup))
        except ValueError:
            ErrorMessage(self.window,_('Unrecognized date: ')+newTimeStr)
            return
        except OverflowError:
            ErrorMessage(self,_('Mash cannot handle dates greater than January 19, 2038.)'))
            return
        #--Do it
        selInfos = [fileInfos[fileName] for fileName in self.data]
        selInfos.sort(key=lambda a: a.mtime)
        for fileInfo in selInfos:
            fileInfo.setMTime(newTime)
            newTime += 60
        #--Refresh
        fileInfos.refreshDoubleTime()
        self.window.Refresh()

#------------------------------------------------------------------------------
class File_Sort(Link):
    """Sort the selected files."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Sort'))
        menu.AppendItem(menuItem)
        if len(data) < 2: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        message = _("This command will sort the selected files alphabetically, assigning them dates one minute after each other. It cannot be undone.\n\nNote that some mods need to be in a specific order to work correctly, and this sort operation may break that order.")
        if ContinueQuery(self.window,message,'mash.sortMods.continue',_('Sort Mods')) != wx.ID_OK:
            return
        #--Scan for earliest date
        fileInfos = self.window.data
        newTime = min(fileInfos[fileName].mtime for fileName in self.data)
        #--Do it
        for fileName in sorted(self.data,key=lambda a: a[:-4].lower()):
            fileInfos[fileName].setMTime(newTime)
            newTime += 60
        #--Refresh
        fileInfos.refreshDoubleTime()
        self.window.Refresh()

#------------------------------------------------------------------------------
class File_Snapshot(Link):
    """Take a snapshot of the file."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Snapshot...'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        data = self.data
        fileName = data[0]
        fileInfo = self.window.data[fileName]
        (destDir,destName,wildcard) = fileInfo.getNextSnapshot()
        if not os.path.exists(destDir): os.makedirs(destDir)
        dialog = wx.FileDialog(self.window,_('Save snapshot as:'),destDir,
            destName,wildcard,wx.SAVE|wx.OVERWRITE_PROMPT)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        (destDir,destName) = os.path.split(dialog.GetPath())
        dialog.Destroy()
        #--Extract version number
        fileRoot = os.path.splitext(fileName)[0]
        destRoot = os.path.splitext(destName)[0]
        fileVersion = mosh.getMatch(re.search(r'[ _]+v?([\.0-9]+)$',fileRoot),1)
        snapVersion = mosh.getMatch(re.search(r'-[0-9\.]+$',destRoot))
        fileHedr = fileInfo.tes3.hedr
        if (fileVersion or snapVersion) and mosh.reVersion.search(fileHedr.description):
            newDescription = mosh.reVersion.sub(r'\1 '+fileVersion+snapVersion,
                fileHedr.description,1)
            fileInfo.writeDescription(newDescription)
            self.window.details.SetFile(fileName)
        #--Copy file
        self.window.data.copy(fileName,destDir,destName)

#------------------------------------------------------------------------------
class File_RevertToSnapshot(Link):
    """Revert to Snapshot."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Revert to Snapshot...'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(self.data) == 1)

    def Execute(self,event):
        """Handle menu selection."""
        fileInfo = self.window.data[self.data[0]]
        fileName = fileInfo.name
        #--Snapshot finder
        destDir = self.window.data.dir
        srcDir = os.path.join(destDir,settings['mosh.fileInfo.snapshotDir'])
        wildcard = fileInfo.getNextSnapshot()[2]
        #--File dialog
        if not os.path.exists(srcDir): os.makedirs(srcDir)
        dialog = wx.FileDialog(self.window,_('Revert %s to snapshot:') % (fileName,),
            srcDir, '', wildcard, wx.OPEN)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        snapPath = dialog.GetPath()
        snapName = os.path.basename(snapPath)
        dialog.Destroy()
        #--Warning box
        message = (_("Revert %s to snapshot %s dated %s?") 
            % (fileInfo.name,snapName,formatDate(mosh.getmtime(snapPath))))
        dialog = wx.MessageDialog(self.window,message,_('Revert to Snapshot'),
            style=wx.YES_NO|wx.ICON_EXCLAMATION)
        if dialog.ShowModal() != wx.ID_YES: 
            dialog.Destroy()
            return
        wx.BeginBusyCursor()
        destPath = os.path.join(fileInfo.dir,fileInfo.name)
        shutil.copy(snapPath,destPath)
        fileInfo.setMTime()
        try:
            self.window.data.refreshFile(fileName)
        except mosh.Tes3Error:
            ErrorMessage(self,_('Snapshot file is corrupt!'))
            self.window.details.SetFile(None)
        wx.EndBusyCursor()
        self.window.Refresh(fileName)

#------------------------------------------------------------------------------
class File_Backup(Link):
    """Backup file."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Backup'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(self.data)==1)

    def Execute(self,event):
        """Handle menu selection."""
        fileInfo = self.window.data[self.data[0]]
        fileInfo.makeBackup(True)

#------------------------------------------------------------------------------
class File_Open(Link):
    """Open specified file(s)."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Open...'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(self.data)>0)

    def Execute(self,event):
        """Handle selection."""
        dir = self.window.data.dir
        for file in self.data:
            dir.join(file).start()

#------------------------------------------------------------------------------
class File_RevertToBackup:
    """Revert to last or first backup."""
    def AppendToMenu(self,menu,window,data):
        self.window = window
        self.data = data
        #--Backup Files
        singleSelect = len(data) == 1
        self.fileInfo = window.data[data[0]]
        #--Backup Item
        wx.EVT_MENU(window,ID_REVERT_BACKUP,self.Execute)
        menuItem = wx.MenuItem(menu,ID_REVERT_BACKUP,_('Revert to Backup'))
        menu.AppendItem(menuItem)
        self.backup = os.path.join(self.fileInfo.dir,
            settings['mosh.fileInfo.backupDir'],self.fileInfo.name)
        menuItem.Enable(singleSelect and os.path.exists(self.backup))
        #--First Backup item
        wx.EVT_MENU(window,ID_REVERT_FIRST,self.Execute)
        menuItem = wx.MenuItem(menu,ID_REVERT_FIRST,_('Revert to First Backup'))
        menu.AppendItem(menuItem)
        self.firstBackup = self.backup +'f'
        menuItem.Enable(singleSelect and os.path.exists(self.firstBackup))

    def Execute(self,event):
        """Handle menu selection."""
        fileInfo = self.fileInfo
        fileName = fileInfo.name
        #--Backup/FirstBackup?
        if event.GetId() ==  ID_REVERT_BACKUP:
            backup = self.backup
        else:
            backup = self.firstBackup
        #--Warning box
        message = _("Revert %s to backup dated %s?") % (fileName,
            formatDate(mosh.getmtime(self.backup)))
        dialog = wx.MessageDialog(self.window,message,_('Revert to Backup'),
            style=wx.YES_NO|wx.ICON_EXCLAMATION)
        if dialog.ShowModal() == wx.ID_YES: 
            wx.BeginBusyCursor()
            dest = os.path.join(fileInfo.dir,fileName)
            shutil.copy(backup,dest)
            fileInfo.setMTime()
            try:
                self.window.data.refreshFile(fileName)
            except mosh.Tes3Error:
                ErrorMessage(self,_('Old file is corrupt!'))
            wx.EndBusyCursor()
        dialog.Destroy()
        self.window.Refresh(fileName)
    
#------------------------------------------------------------------------------
class File_Remove_RefsSafeCells(ListEditorData):
    """Data capsule for load list editing dialog."""
    def __init__(self,parent):
        """Initialize."""
        self.data = settings['mash.refRemovers.safeCells']
        self.data.sort(key=lambda a: a.lower())
        #--GUI
        ListEditorData.__init__(self,parent)
        self.showAdd = True
        self.showRemove = True

    def getItemList(self):
        """Returns safe cells in alpha order."""
        return self.data[:]

    def add(self):
        """Adds a safe cell."""
        #--Dialog
        dialog = wx.TextEntryDialog(self.parent,_('Cell Name:'),_('Add Safe Cell'))
        result = dialog.ShowModal()
        #--Canceled or empty?
        if result != wx.ID_OK or not dialog.GetValue():
            dialog.Destroy()
            return None
        newCell = dialog.GetValue()
        dialog.Destroy()
        #--Already have it?
        if newCell in self.data:
            return None
        settings.setChanged('mash.refRemovers.safeCells')
        self.data.append(newCell)
        self.data.sort(key=lambda a: a.lower())
        return newCell
       
    def remove(self,item):
        """Remove a safe cell."""
        settings.setChanged('mash.refRemovers.safeCells')
        self.data.remove(item)
        return True

#------------------------------------------------------------------------------
class File_Remove_RefsData(ListEditorData):
    """Data capsule for ref remover editing dialog."""
    def __init__(self,parent):
        """Initialize."""
        self.data = settings['mash.refRemovers.data']
        #--GUI
        ListEditorData.__init__(self,parent)
        self.showAdd = True
        self.showRename = True
        self.showRemove = True

    def getItemList(self):
        """Returns load list keys in alpha order."""
        return sorted(self.data.keys(),key=lambda a: a.lower())

    def add(self):
        """Adds a new ref remover."""
        #--File dialog
        workDir = settings.get('mosh.workDir',settings['mwDir'])
        dialog = wx.FileDialog(self.parent,_('Select Ref Remover file or files:'),
            workDir,'', '*.*', wx.OPEN|wx.MULTIPLE)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return None
        paths = dialog.GetPaths()
        dialog.Destroy()
        if len(paths) == 0: 
            return None
        elif len(paths) == 1:
            settings.setChanged('mash.refRemovers.data')
            name = os.path.splitext(os.path.basename(paths[0]))[0]
        else:
            root,number = _('Combo %d'),1
            while (root % (number,)) in self.data:
                number += 1
            name = root % (number,)
        settings['mosh.workDir'] = os.path.split(paths[0])[0]
        self.data[name] = paths
        return name

    def rename(self,oldName,newName):
        """Renames oldName to newName."""
        #--Right length?
        if len(newName) == 0 or len(newName) > 64:
            ErrorMessage(self.parent,
                _('Name must be between 1 and 64 characters long.'))
            return False
        #--Rename
        settings.setChanged('mash.refRemovers.data')
        self.data[newName] = self.data[oldName]
        del self.data[oldName]
        return newName

    def remove(self,item):
        """Removes load list."""
        settings.setChanged('mash.refRemovers.data')
        del self.data[item]
        return True

#------------------------------------------------------------------------------
class File_Remove_Refs:
    """Add ref remover links."""
    def __init__(self):
        """Initialize."""
        self.safeCells = settings['mash.refRemovers.safeCells'] 
        self.removers = settings['mash.refRemovers.data']

    def GetItems(self):
        items = self.removers.keys()
        items.sort(lambda a,b: cmp(a.lower(),b.lower()))
        return items

    def AppendToMenu(self,menu,window,data):
        """Append ref remover items to menu."""
        self.window = window
        self.data = data
        menu.Append(ID_REMOVERS.EDIT,_('Edit Removers...'))
        menu.Append(ID_REMOVERS.EDIT_CELLS,_('Edit Safe Cells...'))
        menu.AppendSeparator()
        enable = len(data) == 1
        ids = iter(ID_REMOVERS)
        for item in self.GetItems():
            try:
                menuItem = wx.MenuItem(menu,ids.next(),item)
                menu.AppendItem(menuItem)
                menuItem.Enable(enable)
            except StopIteration:
                pass
        #--Events
        wx.EVT_MENU(window,ID_REMOVERS.EDIT,self.DoData)
        wx.EVT_MENU(window,ID_REMOVERS.EDIT_CELLS,self.DoCells)
        wx.EVT_MENU_RANGE(window,ID_REMOVERS.BASE,ID_REMOVERS.MAX,self.DoList)

    def DoList(self,event):
        """Handle selection of one ref removers."""
        #--Continue Query
        message = _("This command will remove ALL instances of the refs listed in the associated file, EXCEPT for instances in safe cells. Be SURE that the cells you use for storage (strongholds, etc.) are included in the safe cells list, or you risk losing the items you have stored in them!")
        if ContinueQuery(self.window,message,'mash.refRemovers.continue',_('Remove Refs by Id...')) != wx.ID_OK:
            return
        #--Do it
        removerName = self.GetItems()[event.GetId()-ID_REMOVERS.BASE]
        removerPaths = self.removers[removerName]
        #--Get objIds
        objIds = set()
        for removerPath in removerPaths:
            removerFile = open(removerPath)
            reObjId = re.compile('"?(.*?)"?\t')
            for line in removerFile:
                maObjId = reObjId.match(line)
                if not maObjId or not maObjId.group(1): continue
                objIds.add(maObjId.group(1))
            removerFile.close()
        #--File Refs
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        caption = _('Refs Removed: ')+fileName
        progress = ProgressDialog(caption)
        log = mosh.LogFile(cStringIO.StringIO())
        try:
            fileRefs = mosh.FileRefs(fileInfo,log=log,progress=progress)
            fileRefs.refresh()
            fileRefs.removeRefsById(objIds,self.safeCells)
            fileRefs.log = mosh.Log() #--Null log. (Don't want orphan deletion in log.)
            fileRefs.removeOrphanContents()
            fileRefs.log = log
            fileRefs.safeSave()
            LogMessage(self.window,'',log.out.getvalue(),caption)
        finally:
            progress.Destroy()
            self.window.Refresh(fileName)

    def DoCells(self,event):
        """Show safe cells editing dialog."""
        data = File_Remove_RefsSafeCells(self.window)
        dialog = ListEditorDialog(self.window,-1,_('Safe Cells'),data)
        dialog.list.SetSizeHints(250,150)
        dialog.Fit()
        dialog.ShowModal()
        dialog.Destroy()

    def DoData(self,event):
        """Show ref removers editing dialog."""
        data = File_Remove_RefsData(self.window)
        dialog = ListEditorDialog(self.window,-1,_('Ref Removers'),data)
        dialog.ShowModal()
        dialog.Destroy()

#------------------------------------------------------------------------------
class File_Replace_RefsData(ListEditorData):
    """Data capsule for ref replacer editing dialog."""
    def __init__(self,parent):
        """Initialize."""
        self.data = settings['mash.refReplacers.data']
        #--GUI
        ListEditorData.__init__(self,parent)
        self.showAdd = True
        self.showRename = True
        self.showRemove = True

    def getItemList(self):
        """Returns load list keys in alpha order."""
        return sorted(self.data.keys(),key=lambda a: a.lower())

    def add(self):
        """Adds a new ref replacer."""
        #--File dialog
        workDir = settings.get('mosh.workDir',settings['mwDir'])
        dialog = wx.FileDialog(self.parent,_('Select Ref Replacer list file:'),
            workDir,'', '*.*', wx.OPEN)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return None
        path = dialog.GetPath()
        dialog.Destroy()
        settings['mosh.workDir'] = os.path.split(path)[0]
        settings.setChanged('mash.refReplacers.data')
        name = os.path.splitext(os.path.basename(path))[0]
        self.data[name] = path
        return name

    def rename(self,oldName,newName):
        """Renames oldName to newName."""
        #--Right length?
        if len(newName) == 0 or len(newName) > 64:
            ErrorMessage(self.parent,
                _('Name must be between 1 and 64 characters long.'))
            return False
        #--Rename
        settings.setChanged('mash.refReplacers.data')
        self.data[newName] = self.data[oldName]
        del self.data[oldName]
        return newName

    def remove(self,item):
        """Removes replacer."""
        settings.setChanged('mash.refReplacers.data')
        del self.data[item]
        return True

#------------------------------------------------------------------------------
class File_Replace_Refs:
    """Add ref replacer links."""
    def __init__(self):
        """Initialize."""
        self.replacers = settings['mash.refReplacers.data']

    def GetItems(self):
        items = self.replacers.keys()
        items.sort(lambda a,b: cmp(a.lower(),b.lower()))
        return items

    def AppendToMenu(self,menu,window,data):
        """Append ref replacer items to menu."""
        self.window = window
        self.data = data
        menu.Append(ID_REPLACERS.EDIT,_('Edit Replacers...')) 
        menu.AppendSeparator()
        ids = iter(ID_REPLACERS)
        enable = (len(data) == 1)
        for item in self.GetItems():
            try:
                menuItem = wx.MenuItem(menu,ids.next(),item)
                menu.AppendItem(menuItem)
                menuItem.Enable(enable)
            except StopIteration:
                pass
        #--Events
        wx.EVT_MENU(window,ID_REPLACERS.EDIT,self.DoData)
        wx.EVT_MENU_RANGE(window,ID_REPLACERS.BASE,ID_REPLACERS.MAX,self.DoList)

    def DoList(self,event):
        """Handle selection of one ref replacers."""
        #--Continue Query
        message = _("This command will replace all instances of objects listed in the replacer file with other objects.")
        if ContinueQuery(self.window,message,'mash.refReplacers.continue',_('Replace Refs by Id...')) != wx.ID_OK:
            return
        #--File Refs
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        caption = _('Refs Replaced: ')+fileName
        progress = ProgressDialog(caption)
        log = mosh.LogFile(cStringIO.StringIO())
        try:
            #--Replacer
            replacerName = self.GetItems()[event.GetId()-ID_REPLACERS.BASE]
            replacerPath = self.replacers[replacerName]
            refReplacer = mosh.RefReplacer(replacerPath)
            #--Source Mod?
            srcModName = refReplacer.srcModName
            if srcModName and srcModName not in mosh.modInfos:
                renames = settings['mash.mods.renames']
                if srcModName in renames:
                    srcModName = renames[srcModName]
                    refReplacer.srcModName = srcModName
                else:
                    ErrorMessage(self.window,_("Source mod %s is not in Data Files folder.") % (srcModName,))
                    return
            log.setHeader(_("Source Mod"))
            log(srcModName or _("None"))            
            #--File Refs
            fileRefs = mosh.FileRefs(fileInfo,log=log,progress=progress)
            fileRefs.refresh()
            if not fileRefs.replaceRefsById(refReplacer):
                InfoMessage(self.window,_("No replacements necessary."))
            else:
                fileRefs.sortRecords()
                fileRefs.safeSave()
                fileInfo.refresh()
                fileInfo.writeAuthorWM()
                self.window.details.SetFile(fileName)
                LogMessage(self.window,'',log.out.getvalue(),caption)
        finally:
            if progress != None: progress.Destroy()
            self.window.Refresh(fileName)

    def DoData(self,event):
        """Show ref replacers editing dialog."""
        data = File_Replace_RefsData(self.window)
        dialog = ListEditorDialog(self.window,-1,_('Ref Replacers'),data)
        dialog.ShowModal()
        dialog.Destroy()

#------------------------------------------------------------------------------
class File_RepairRefs(Link):
    """Repairs the save game's refs by comparing their type and id against the
    types and ids of the save game's masters."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Repair Refs'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        if fileInfo.getStatus() > 10:
            WarningMessage(self.window,
                _("File master list is out of date. Please edit masters before attempting repair."))
            return
        progress = None
        dialog = None
        try:
            #--Log and Progress
            caption = _('Repairing ')+fileName
            progress = ProgressDialog(caption)
            log = mosh.LogFile(cStringIO.StringIO())
            #--World Refs
            worldRefs = mosh.WorldRefs(log=log,progress=progress)
            try:
                worldRefs.addMasters(fileInfo.masterNames)
            except mosh.Tes3RefError, error:
                progress = progress.Destroy()
                message = ((
                    _("%s has bad refs and must be repaired first.\n") +
                    _("\nExample Bad Ref from %s:") +
                    _("\n  Cell: %s\n  Object Id: %s\n  Object Index: %d")+
                    _("\n  Mod Index: %d (%s)")) % 
                    (error.inName,error.inName,error.cellId,error.objId,error.iObj,error.iMod,error.masterName))
                ErrorMessage(self.window,message)
                return
            #--File Refs for Save File
            progress.setBaseScale()
            fileRefs = mosh.FileRefs(fileInfo,log=log,progress=progress)
            fileRefs.refresh()
            (cntRepaired,cntDeleted,cntUnnamed) = worldRefs.repair(fileRefs)
            #--No problems?
            if not (cntRepaired or cntDeleted or cntUnnamed):
                progress = progress.Destroy()
                InfoMessage(self.window,_("No problems found!"))
                return
            #--Save
            fileRefs.safeSave()
            progress = progress.Destroy()
            #--Problem Dialog
            message = (_("Objects repaired: %d.\nObjects deleted: %d.") % 
                (cntRepaired,cntDeleted))
            #InfoMessage(self.window,message)
            LogMessage(self.window,message,log.out.getvalue(),caption)
        #--Done
        finally:
            if progress != None: progress.Destroy()
            if dialog: dialog.Destroy()
            self.window.Refresh(fileName)

#------------------------------------------------------------------------------
class File_SortRecords(Link):
    """Sorts the records in the file. 
    This is just to make records easier to find in TESCS Details view."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Sort Records'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--Continue Query
        message = _("This function will sort the records of the selected esp for easier viewing in the Details view of TESCS. Generally, this is only useful for active modders.")
        if ContinueQuery(self.window,message,'mash.sortRecords.continue',_('Sort Records...')) != wx.ID_OK:
            return
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        fileRep = mosh.FileRep(fileInfo)
        fileRep.load()
        fileRep.sortRecords()
        fileRep.safeSave()
        InfoMessage(self.window,_("Record sort completed."))

#------------------------------------------------------------------------------
class File_StatsList(List):
    def __init__(self,parent,data):
        #--Columns
        self.cols = settings['mash.fileStats.cols']
        self.colNames = settings['mash.colNames']
        self.colWidths = settings['mash.fileStats.colWidths']
        self.colAligns = settings['mash.fileStats.colAligns']
        self.colReverse = settings['mash.fileStats.colReverse'].copy()
        #--Data/Items
        self.data = data
        self.sort = settings['mash.fileStats.sort']
        #--Links
        self.mainMenu = []
        self.itemMenu = []
        #--Parent init
        List.__init__(self,parent,-1)

    #--Populate Item
    def PopulateItem(self,itemDex,mode=0,selected=set()):
        type = self.items[itemDex]
        itemData = self.data[type]
        cols = self.cols
        for colDex in range(self.numCols):
            #--Value
            col = cols[colDex]
            if col == 'Type':
                value = type
            elif col == 'Count':
                value = formatInteger(itemData[0])
            elif col == 'Size':
                value = formatInteger(itemData[1])+' B'
            #--Insert/Set Value
            if mode and (colDex == 0):
                self.list.InsertStringItem(itemDex, value)
            else:
                self.list.SetStringItem(itemDex, colDex, value)
        #--State
        if type in selected:
            self.list.SetItemState(itemDex,wx.LIST_STATE_SELECTED,wx.LIST_STATE_SELECTED)
        else:
            self.list.SetItemState(itemDex,0,wx.LIST_STATE_SELECTED)

    #--Sort Items
    def SortItems(self,col=None,reverse=-2):
        #--Setup
        if not col:
            col = self.sort
            reverse = self.colReverse.get(col,0)
        elif reverse < 0:
            reverse = self.colReverse.get(col,0)
            if self.sort == col:
                reverse = not reverse
        self.sort = col
        self.colReverse[col] = reverse
        #--Sort
        data = self.data
        #--Start with sort by type
        self.items.sort(lambda a,b: cmp(a.lower(),b.lower()))
        if col == 'Type':
            pass #--Done by default
        elif col == 'Count':
            self.items.sort(lambda a,b: cmp(data[a][0],data[b][0]))
        elif col == 'Size':
            self.items.sort(lambda a,b: cmp(data[a][1],data[b][1]))
        #--Reverse?
        if reverse: self.items.reverse()

#------------------------------------------------------------------------------
class File_Stats(Link):
    """Show file statistics."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Statistics'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--Assume a single selection for now...
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        fileInfo.getStats()
        frame = wx.Frame(self.window,-1,fileName,size=(200,300),
            style= (wx.RESIZE_BORDER | wx.CAPTION | wx.SYSTEM_MENU | wx.CLOSE_BOX | wx.CLIP_CHILDREN))
        frame.SetIcons(images['mash.icons2'].GetIconBundle())
        sizer = wx.BoxSizer(wx.VERTICAL)
        sizer.Add(File_StatsList(frame,fileInfo.stats),1,wx.EXPAND)
        frame.SetSizer(sizer)
        frame.Show()

# Installers Links -----------------------------------------------------------------
#------------------------------------------------------------------------------
class Installers_AnnealAll(Link):
    """Anneal all packages."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Anneal All'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle selection."""
        progress = balt.Progress(_("Annealing..."),'\n'+' '*60)
        try:
            self.data.anneal(progress=progress)
        finally:
            progress.Destroy()
            self.data.refresh(what='NS')
            gInstallers.RefreshUIMods()
        
#------------------------------------------------------------------------------
class Installers_AutoAnneal(Link):
    """Toggle autoAnneal setting."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Auto-Anneal'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.autoAnneal'])

    def Execute(self,event):
        """Handle selection."""
        settings['bash.installers.autoAnneal'] ^= True
        
#------------------------------------------------------------------------------
class Installers_Enabled(Link):
    """Flips installer state."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Enabled'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.enabled'])

    def Execute(self,event):
        """Handle selection."""
        enabled = settings['bash.installers.enabled']
        message = _("Do you want to enable Installers? If you do, Bash will first need to initialize some data. If there are many new mods to process, then this may take on the order of five minutes.")
        if not enabled and not balt.askYes(self.gTank,fill(message,80),self.title): 
            return
        enabled = settings['bash.installers.enabled'] = not enabled
        if enabled:
            gInstallers.refreshed = False
            gInstallers.OnShow()
            gInstallers.gList.RefreshUI()
        else:
            gInstallers.gList.gList.DeleteAllItems()
            gInstallers.RefreshDetails(None)
        
#------------------------------------------------------------------------------
class Installers_ConflictsReportShowsInactive(Link):
    """Toggles option to show lower on conflicts report."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Show Inactive Conflicts'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.conflictsReport.showInactive'])

    def Execute(self,event):
        """Handle selection."""
        settings['bash.installers.conflictsReport.showInactive'] ^= True
        self.gTank.RefreshUI()
        
#------------------------------------------------------------------------------
class Installers_ConflictsReportShowsLower(Link):
    """Toggles option to show lower on conflicts report."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Show Lower Conflicts'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.conflictsReport.showLower'])

    def Execute(self,event):
        """Handle selection."""
        settings['bash.installers.conflictsReport.showLower'] ^= True
        self.gTank.RefreshUI()
        
#------------------------------------------------------------------------------
class Installers_AvoidOnStart(Link):
    """Ensures faster bash startup by preventing Installers from being startup tab."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Avoid at Startup'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.fastStart'])

    def Execute(self,event):
        """Handle selection."""
        settings['bash.installers.fastStart'] ^= True
        
#------------------------------------------------------------------------------
class Installers_Refresh(Link):
    """Refreshes all Installers data."""
    def __init__(self,fullRefresh=False):
        Link.__init__(self)
        self.fullRefresh = fullRefresh

    def AppendToMenu(self,menu,window,data):
        if not settings['bash.installers.enabled']: return
        Link.AppendToMenu(self,menu,window,data)
        self.title = (_('Refresh Data'),_('Full Refresh'))[self.fullRefresh]
        menuItem = wx.MenuItem(menu,self.id,self.title)
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle selection."""
        if self.fullRefresh:
            message = balt.fill(_("Refresh ALL data from scratch? This may take five to ten minutes (or more) depending on the number of mods you have installed."))
            if not balt.askWarning(self.gTank,fill(message,80),self.title): return
        gInstallers.refreshed = False
        gInstallers.fullRefresh = self.fullRefresh
        gInstallers.OnShow()

#------------------------------------------------------------------------------
class Installers_RemoveEmptyDirs(Link):
    """Toggles option to remove empty directories on file scan."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Clean Data Directory'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.removeEmptyDirs'])

    def Execute(self,event):
        """Handle selection."""
        settings['bash.installers.removeEmptyDirs'] ^= True
        
#------------------------------------------------------------------------------
class Installers_SortActive(Link):
    """Sort by type."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_("Sort by Active"),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.sortActive'])

    def Execute(self,event):
        settings['bash.installers.sortActive'] ^= True
        self.gTank.SortItems()

#------------------------------------------------------------------------------
class Installers_SortProjects(Link):
    """Sort dirs to the top."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_("Projects First"),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.sortProjects'])

    def Execute(self,event):
        settings['bash.installers.sortProjects'] ^= True
        self.gTank.SortItems()

#------------------------------------------------------------------------------
class Installers_SortStructure(Link):
    """Sort by type."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_("Sort by Structure"),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['bash.installers.sortStructure'])

    def Execute(self,event):
        settings['bash.installers.sortStructure'] ^= True
        self.gTank.SortItems()

# Installer Links -------------------------------------------------------------
#------------------------------------------------------------------------------
class InstallerLink(Link):
    """Common functions for installer links..."""

    def isSingle(self):
        """Indicates whether or not is single installer."""
        return len(self.selected) == 1

    def isSingleProject(self):
        """Indicates whether or not is single project."""
        if len(self.selected) != 1: return False
        else: return isinstance(self.data[self.selected[0]],mosh.InstallerProject)

    def isSingleArchive(self):
        """Indicates whether or not is single archive."""
        if len(self.selected) != 1: return False
        else: return isinstance(self.data[self.selected[0]],mosh.InstallerArchive)

    def getProjectPath(self):
        """Returns whether build directory exists."""
        archive = self.selected[0]
        return mosh.dirs['builds'].join(archive.sroot)

    def projectExists(self):
        if not len(self.selected) == 1: return False
        return self.getProjectPath().exists()

#------------------------------------------------------------------------------
class Installer_Anneal(InstallerLink):
    """Anneal all packages."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Anneal'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle selection."""
        progress = balt.Progress(_("Annealing..."),'\n'+' '*60)
        try:
            self.data.anneal(self.selected,progress)
        finally:
            progress.Destroy()
            self.data.refresh(what='NS')
            gInstallers.RefreshUIMods()
        
#------------------------------------------------------------------------------
class Installer_Delete(balt.Tank_Delete):
    """Deletes selected file from tank."""
    def Execute(self,event):
        balt.Tank_Delete.Execute(self,event)
        self.data.refreshOrder()
        self.data.refresh(what='N')
        self.gTank.RefreshUI()

#------------------------------------------------------------------------------
class Installer_Duplicate(InstallerLink):
    """Uninstall selected Installers."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        self.title = _('Duplicate...')
        menuItem = wx.MenuItem(menu,self.id,self.title)
        menu.AppendItem(menuItem)
        menuItem.Enable(self.isSingle())

    def Execute(self,event):
        """Handle selection."""
        curName = self.selected[0]
        isdir = self.data.dir.join(curName).isdir()
        if isdir: root,ext = curName,''
        else: root,ext = curName.rootExt
        newName = root+' Copy'+ext
        index = 0
        while newName in self.data:
            newName = root + (_(' Copy (%d)') % index) + ext
            index += 1
        result = balt.askText(self.gTank,_("Duplicate %s to:") % curName.s,
            self.title,newName.s)
        result = (result or '').strip()
        if not result: return
        #--Error checking
        newName = GPath(result).tail
        if not newName.s: 
            balt.ShowWarning(self.gTank,_("%s is not a valid name.") % result)
            return
        if newName in self.data:
            balt.ShowWarning(self.gTank,_("%s already exists.") % newName.s)
            return
        if self.data.dir.join(curName).isfile() and curName.cext != newName.cext:
            balt.ShowWarning(self.gTank,
                _("%s does not have correct extension (%s).") % (newName.s,curName.ext))
            return
        #--Duplicate
        try:
            wx.BeginBusyCursor()
            self.data.copy(curName,newName)
        finally:
            wx.EndBusyCursor()
        self.data.refresh(what='N')
        self.gTank.RefreshUI()

#------------------------------------------------------------------------------
class Installer_HasExtraData(InstallerLink):
    """Toggle hasExtraData flag on installer."""

    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Has Extra Directories'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Enable(self.isSingle())
        if self.isSingle():
            installer = self.data[self.selected[0]]
            menuItem.Check(installer.hasExtraData)

    def Execute(self,event):
        """Handle selection."""
        installer = self.data[self.selected[0]]
        installer.hasExtraData ^= True
        installer.refreshDataSizeCrc()
        installer.refreshStatus(self.data)
        self.data.refresh(what='N')
        self.gTank.RefreshUI()

#------------------------------------------------------------------------------
class Installer_Install(InstallerLink):
    """Install selected packages."""
    mode_title = {'DEFAULT':_('Install'),'LAST':_('Install Last'),'MISSING':_('Install Missing')}

    def __init__(self,mode='DEFAULT'):
        Link.__init__(self)
        self.mode = mode

    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        self.title = self.mode_title[self.mode]
        menuItem = wx.MenuItem(menu,self.id,self.title)
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle selection."""
        dir = self.data.dir
        progress = balt.Progress(_("Installing..."),'\n'+' '*60)
        try:
            last = (self.mode == 'LAST')
            override = (self.mode != 'MISSING')
            self.data.install(self.selected,progress,last,override)
        finally:
            progress.Destroy()
            self.data.refresh(what='N')
            gInstallers.RefreshUIMods()

#------------------------------------------------------------------------------
class Installer_Move(InstallerLink):
    """Moves selected installers to desired spot."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Move To...'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle selection."""
        curPos = min(self.data[x].order for x in self.selected)
        message = _("Move selected archives to what position?\nEnter position number.\nLast: -1; First of Last: -2; Semi-Last: -3.")
        newPos = balt.askText(self.gTank,message,self.title,`curPos`)
        if not newPos: return
        newPos = newPos.strip()
        if not re.match('-?\d+',newPos):
            balt.showError(self.gTank,_("Position must be an integer."))
            return
        newPos = int(newPos)
        if newPos == -3: newPos = self.data[self.data.lastKey].order
        elif newPos == -2: newPos = self.data[self.data.lastKey].order+1
        elif newPos < 0: newPos = len(self.data.data)
        self.data.moveArchives(self.selected,newPos)
        self.data.refresh(what='N')
        self.gTank.RefreshUI()

#------------------------------------------------------------------------------
class Installer_Open(balt.Tank_Open):
    """Open selected file(s)."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Open...'))
        menu.AppendItem(menuItem)
        self.selected = [x for x in self.selected if x != self.data.lastKey]
        menuItem.Enable(bool(self.selected))

#------------------------------------------------------------------------------
class Installer_Refresh(InstallerLink):
    """Rescans selected Installers."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Refresh'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle selection."""
        dir = self.data.dir
        progress = balt.Progress(_("Refreshing Packages..."),'\n'+' '*60)
        progress.setFull(len(self.selected))
        try:
            for index,archive in enumerate(self.selected):
                progress(index,_("Refreshing Packages...\n")+archive.s)
                installer = self.data[archive]
                apath = mosh.dirs['installers'].join(archive)
                installer.refreshBasic(apath,SubProgress(progress,index,index+1),True)
                self.data.hasChanged = True
        finally:
            if progress != None: progress.Destroy()
        self.data.refresh(what='NS')
        self.gTank.RefreshUI()

#------------------------------------------------------------------------------
class Installer_Uninstall(InstallerLink):
    """Uninstall selected Installers."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Uninstall'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle selection."""
        dir = self.data.dir
        progress = balt.Progress(_("Uninstalling..."),'\n'+' '*60)
        try:
            self.data.uninstall(self.selected,progress)
        finally:
            progress.Destroy()
            self.data.refresh(what='NS')
            gInstallers.RefreshUIMods()

# InstallerArchive Links ------------------------------------------------------
#------------------------------------------------------------------------------
class InstallerArchive_Unpack(InstallerLink):
    """Install selected packages."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        self.title = _('Unpack to Project...')
        menuItem = wx.MenuItem(menu,self.id,self.title)
        menu.AppendItem(menuItem)
        menuItem.Enable(self.isSingleArchive())

    def Execute(self,event):
        archive = self.selected[0]
        installer = self.data[archive]
        project = archive.root
        result = balt.askText(self.gTank,_("Unpack %s to Project:") % archive.s,
            self.title,project.s)
        result = (result or '').strip()
        if not result: return
        #--Error checking
        project = GPath(result).tail
        if not project.s or project.cext in ('.rar','.zip','.7z'): 
            balt.ShowWarning(self.gTank,_("%s is not a valid project name.") % result)
            return
        if self.data.dir.join(project).isfile():
            balt.ShowWarning(self.gTank,_("%s is a file.") % project.s)
            return
        if project in self.data:
            if not balt.askYes(self.gTank,_("%s already exists. Overwrite it?") % project.s,self.title,False):
                return
        #--Copy to Build
        progress = balt.Progress(_("Unpacking to Project..."),'\n'+' '*60)
        try:
            installer.unpackToProject(archive,project,SubProgress(progress,0,0.8))
            if project not in self.data:
                self.data[project] = mosh.InstallerProject(project)
            iProject = self.data[project]
            pProject = mosh.dirs['installers'].join(project)
            iProject.refreshed = False
            iProject.refreshBasic(pProject,SubProgress(progress,0.8,0.99),True)
            if iProject.order == -1:
                self.data.refreshOrder()
                self.data.moveArchives([project],installer.order+1)
            self.data.refresh(what='NS')
            self.gTank.RefreshUI()
            #pProject.start()
        finally:
            progress.Destroy()

# InstallerProject Links ------------------------------------------------------
#------------------------------------------------------------------------------
class InstallerProject_Sync(InstallerLink):
    """Install selected packages."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        self.title = _('Sync from Data')
        menuItem = wx.MenuItem(menu,self.id,self.title)
        menu.AppendItem(menuItem)
        enabled = False
        if self.isSingleProject():
            project = self.selected[0]
            installer = self.data[project]
            enabled = bool(installer.missingFiles or installer.mismatchedFiles)
        menuItem.Enable(enabled)

    def Execute(self,event):
        project = self.selected[0]
        installer = self.data[project]
        missing = installer.missingFiles
        mismatched = installer.mismatchedFiles
        message = _("Update %s according to data directory?\nFiles to delete: %d\nFiles to update: %d") % (
            project.s,len(missing),len(mismatched))
        if not balt.askWarning(self.gTank,message,self.title): return
        #--Sync it, baby!
        progress = balt.Progress(self.title,'\n'+' '*60)
        try:
            progress(0.1,_("Updating files."))
            installer.syncToData(project,missing|mismatched)
            pProject = mosh.dirs['installers'].join(project)
            installer.refreshed = False
            installer.refreshBasic(pProject,SubProgress(progress,0.1,0.99),True)
            self.data.refresh(what='NS')
            self.gTank.RefreshUI()
        finally:
            progress.Destroy()

#------------------------------------------------------------------------------
class InstallerProject_SyncPack(InstallerLink):
    """Install selected packages."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Sync and Pack'))
        menu.AppendItem(menuItem)
        menuItem.Enable(self.projectExists())

    def Execute(self,event):
        raise UncodedError

#------------------------------------------------------------------------------
class InstallerProject_Pack(InstallerLink):
    """Install selected packages."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Pack Archive'))
        menu.AppendItem(menuItem)
        menuItem.Enable(self.projectExists())

    def Execute(self,event):
        raise UncodedError

# Mods Links ------------------------------------------------------------------
#------------------------------------------------------------------------------
class Mods_LoadListData(ListEditorData):
    """Data capsule for load list editing dialog."""
    def __init__(self,parent):
        """Initialize."""
        self.data = settings['mash.loadLists.data']
        #--GUI
        ListEditorData.__init__(self,parent)
        self.showRename = True
        self.showRemove = True

    def getItemList(self):
        """Returns load list keys in alpha order."""
        return sorted(self.data.keys(),key=lambda a: a.lower())

    def rename(self,oldName,newName):
        """Renames oldName to newName."""
        #--Right length?
        if len(newName) == 0 or len(newName) > 64:
            ErrorMessage(self.parent,
                _('Name must be between 1 and 64 characters long.'))
            return False
        #--Rename
        settings.setChanged('mash.loadLists.data')
        self.data[newName] = self.data[oldName]
        del self.data[oldName]
        return newName

    def remove(self,item):
        """Removes load list."""
        settings.setChanged('mash.loadLists.data')
        del self.data[item]
        return True

#------------------------------------------------------------------------------
class Mods_LoadList:
    """Add load list links."""
    def __init__(self):
        self.data = settings['mash.loadLists.data']

    def GetItems(self):
        items = self.data.keys()
        items.sort(lambda a,b: cmp(a.lower(),b.lower()))
        return items

    def SortWindow(self):
        self.window.PopulateItems()

    def AppendToMenu(self,menu,window,data):
        self.window = window
        menu.Append(ID_LOADERS.NONE,_('None'))
        menu.Append(ID_LOADERS.SAVE,_('Save List...')) 
        menu.Append(ID_LOADERS.EDIT,_('Edit Lists...')) 
        menu.AppendSeparator()
        ids = iter(ID_LOADERS)
        for item in self.GetItems():
            try:
                menu.Append(ids.next(),item)
            except StopIteration:
                pass
        #--Disable Save?
        if not mosh.mwIniFile.loadFiles:
            menu.FindItemById(ID_LOADERS.SAVE).Enable(False)
        #--Events
        wx.EVT_MENU(window,ID_LOADERS.NONE,self.DoNone)
        wx.EVT_MENU(window,ID_LOADERS.SAVE,self.DoSave)
        wx.EVT_MENU(window,ID_LOADERS.EDIT,self.DoEdit)
        wx.EVT_MENU_RANGE(window,ID_LOADERS.BASE,ID_LOADERS.MAX,self.DoList)

    def DoNone(self,event):
        loadFiles = mosh.mwIniFile.loadFiles[:]
        for loadFile in loadFiles:
            mosh.modInfos.unload(loadFile,0)
        mosh.mwIniFile.safeSave()
        #--Refresh entries
        self.window.PopulateItems()

    def DoList(self,event):
        oldLoadFiles = mosh.mwIniFile.loadFiles[:]
        item = self.GetItems()[event.GetId()-ID_LOADERS.BASE]
        for loadFile in self.data[item]:
            try:
                mosh.modInfos.load(loadFile,False)
            except mosh.MaxLoadedError:
                ErrorMessage(self.window,_("Unable to add mod %s because load list is full.") 
                    % (loadFile,))
                break
        mosh.mwIniFile.safeSave()
        #--Repopulate changed entries
        self.window.PopulateItems()

    def DoSave(self,event):
        #--No slots left?
        if len(self.data) >= ID_LOADERS.size:
            ErrorMessage(self,_('All load list slots are full. Please delete an existing load list before adding another.'))
            return
        #--Dialog
        dialog = wx.TextEntryDialog(self.window,_('Save current load list as:'),
                'Wrye Mash')
        result = dialog.ShowModal()
        if result == wx.ID_OK:
            newItem = dialog.GetValue()
            dialog.Destroy()
            if len(newItem) == 0 or len(newItem) > 64:
                ErrorMessage(self.window,
                    _('Load list name must be between 1 and 64 characters long.'))
            else:
                self.data[newItem] = mosh.mwIniFile.loadFiles[:]
                settings.setChanged('mash.loadLists.data')
        #--Not Okay
        else:
            dialog.Destroy()

    def DoEdit(self,event):
        data = Mods_LoadListData(self.window)
        dialog = ListEditorDialog(self.window,-1,_('Load Lists'),data)
        dialog.ShowModal()
        dialog.Destroy()

#------------------------------------------------------------------------------
class Mods_EsmsFirst(Link):
    """Sort esms to the top."""
    def __init__(self,prefix=''):
        Link.__init__(self)
        self.prefix = prefix

    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,self.prefix+_('Type'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        if window.esmsFirst: menuItem.Check()

    def Execute(self,event):
        """Handle menu selection."""
        self.window.esmsFirst = not self.window.esmsFirst
        self.window.PopulateItems()

#------------------------------------------------------------------------------
class Mods_MorrowindIni(Link):
    """Open Oblivion.ini."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Morrowind.ini...'))
        menu.AppendItem(menuItem)
        self.path = os.path.join(settings['mwDir'],'Morrowind.ini')
        menuItem.Enable(os.path.exists(self.path))

    def Execute(self,event):
        """Handle selection."""
        os.startfile(self.path)

#------------------------------------------------------------------------------
class Mods_SelectedFirst(Link):
    """Sort loaded mods to the top."""
    def __init__(self,prefix=''):
        Link.__init__(self)
        self.prefix = prefix

    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,self.prefix+_('Selection'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        if window.selectedFirst: menuItem.Check()

    def Execute(self,event):
        """Handle menu selection."""
        self.window.selectedFirst = not self.window.selectedFirst
        self.window.PopulateItems()

#------------------------------------------------------------------------------
class Mods_LockTimes(Link):
    """Turn on resetMTimes feature."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Lock Times'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        if mosh.modInfos.resetMTimes: menuItem.Check()

    def Execute(self,event):
        """Handle menu selection."""
        mosh.modInfos.resetMTimes = not mosh.modInfos.resetMTimes
        settings['mosh.modInfos.resetMTimes'] = mosh.modInfos.resetMTimes
        if mosh.modInfos.resetMTimes:
            mosh.modInfos.refreshMTimes()
        else:
            mosh.modInfos.mtimes.clear()

#------------------------------------------------------------------------------
class Mods_ReplacersData(ListEditorData):
    """Data capsule for resource replacers dialog."""
    def __init__(self,parent):
        """Initialize."""
        self.data = mosh.modInfos.getResourceReplacers()
        #--GUI
        ListEditorData.__init__(self,parent)

    def getItemList(self):
        """Returns load list keys in alpha order."""
        return sorted(self.data.keys(),key=lambda a: a.lower())

    def getChecks(self):
        """Returns checked state of items as array of True/False values matching Item list."""
        checked = []
        for item in self.getItemList():
            checked.append(self.data[item].isApplied())
        return checked

    def check(self,item):
        """Checks items. Return true on success."""
        progress = None
        try:
            progress = ProgressDialog(item)
            self.data[item].apply(progress)
            return True
        finally:
            if progress != None: progress.Destroy()

    def uncheck(self,item):
        """Unchecks item. Return true on success."""
        try:
            wx.BeginBusyCursor()
            self.data[item].remove()
            return True
        finally:
            wx.EndBusyCursor()

#------------------------------------------------------------------------------
class Mods_Replacers(Link):
    """Mod Replacers dialog."""
    def AppendToMenu(self,menu,window,data):
        """Append ref replacer items to menu."""
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Replacers...'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle menu selection."""
        data = Mods_ReplacersData(self.window)
        dialog = ListEditorDialog(self.window,-1,_('Replacers'),data,'checklist')
        dialog.ShowModal()
        dialog.Destroy()

#------------------------------------------------------------------------------
class Mods_IniTweaks(Link):
    """Import LCV Schedules to mod file."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('INI Tweaks...'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle menu selection."""
        #--Continue Query
        message = _("Modifies games settings in Morrowind.ini by applying changes defined in a .mit (Morrowind INI Tweak) file.")
        if ContinueQuery(self.window,message,'mash.iniTweaks.continue',_('INI Tweaks')) != wx.ID_OK:
            return
        #--File dialog
        mitDir = os.path.join(mosh.modInfos.dir,'Mits')
        if not os.path.exists(mitDir):
            mitDir = settings['mwDir']
        dialog = wx.FileDialog(self.window,_('INI Tweaks'),mitDir,'', '*.mit', wx.OPEN )
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        mitPath = dialog.GetPath() 
        dialog.Destroy()
        mosh.mwIniFile.applyMit(mitPath)
        InfoMessage(self.window,_('%s applied.') 
            % (os.path.split(mitPath)[1],),_('INI Tweaks'))

# Mod Links -------------------------------------------------------------------
#------------------------------------------------------------------------------
class Mod_GroupsData:
    """Stub class for backward compatibility with old settings files."""
    pass

#------------------------------------------------------------------------------
class Mod_LabelsData(ListEditorData):
    """Data capsule for label editing dialog."""
    def __init__(self,parent,strings):
        """Initialize."""
        #--Strings
        self.column = strings.column
        self.setKey = strings.setKey
        self.addPrompt = strings.addPrompt
        #--Key/type
        self.data = settings[self.setKey]
        #--GUI
        ListEditorData.__init__(self,parent)
        self.showAdd = True
        self.showRename = True
        self.showRemove = True

    def getItemList(self):
        """Returns load list keys in alpha order."""
        return sorted(self.data,key=lambda a: a.lower())

    def add(self):
        """Adds a new group."""
        #--Name Dialog
        #--Dialog
        dialog = wx.TextEntryDialog(self.parent,self.addPrompt)
        result = dialog.ShowModal()
        #--Okay?
        if result != wx.ID_OK:
            dialog.Destroy()
            return
        newName = dialog.GetValue()
        dialog.Destroy()
        if newName in self.data:
            ErrorMessage(self.parent,_('Name must be unique.'))
            return False
        elif len(newName) == 0 or len(newName) > 64:
            ErrorMessage(self.parent,
                _('Name must be between 1 and 64 characters long.'))
            return False
        settings.setChanged(self.setKey)
        self.data.append(newName)
        self.data.sort()
        return newName

    def rename(self,oldName,newName):
        """Renames oldName to newName."""
        #--Right length?
        if len(newName) == 0 or len(newName) > 64:
            ErrorMessage(self.parent,
                _('Name must be between 1 and 64 characters long.'))
            return False
        #--Rename
        settings.setChanged(self.setKey)
        self.data.remove(oldName)
        self.data.append(newName)
        self.data.sort()
        #--Edit table entries.
        colGroup = self.parent.data.table.getColumn(self.column)
        for fileName in colGroup.keys():
            if colGroup[fileName] == oldName:
                colGroup[fileName] = newName
        self.parent.PopulateItems()
        #--Done
        return newName

    def remove(self,item):
        """Removes group."""
        settings.setChanged(self.setKey)
        self.data.remove(item)
        #--Edit table entries.
        colGroup = self.parent.data.table.getColumn(self.column)
        for fileName in colGroup.keys():
            if colGroup[fileName] == item:
                del colGroup[fileName]
        self.parent.PopulateItems()
        #--Done
        return True

#------------------------------------------------------------------------------
class Mod_Labels:
    """Add mod label links."""
    def __init__(self):
        """Initialize."""
        self.labels = settings[self.setKey]

    def GetItems(self):
        items = self.labels[:]
        items.sort(key=lambda a: a.lower())
        return items

    def AppendToMenu(self,menu,window,data):
        """Append label list to menu."""
        self.window = window
        self.data = data
        menu.Append(self.idList.EDIT,self.editMenu) 
        menu.AppendSeparator()
        menu.Append(self.idList.NONE,_('None'))
        ids = iter(self.idList)
        for item in self.GetItems():
            try:
                menu.Append(ids.next(),item)
            except StopIteration:
                pass
        #--Events
        wx.EVT_MENU(window,self.idList.EDIT,self.DoEdit)
        wx.EVT_MENU(window,self.idList.NONE,self.DoNone)
        wx.EVT_MENU_RANGE(window,self.idList.BASE,self.idList.MAX,self.DoList)

    def DoNone(self,event):
        """Handle selection of None."""
        fileLabels = self.window.data.table.getColumn(self.column)
        for fileName in self.data:
            del fileLabels[fileName]
        self.window.PopulateItems()

    def DoList(self,event):
        """Handle selection of label."""
        label = self.GetItems()[event.GetId()-self.idList.BASE]
        fileLabels = self.window.data.table.getColumn(self.column)
        for fileName in self.data:
            fileLabels[fileName] = label
        self.window.Refresh(self.data)

    def DoEdit(self,event):
        """Show label editing dialog."""
        data = Mod_LabelsData(self.window,self)
        dialog = ListEditorDialog(self.window,-1,self.editWindow,data)
        dialog.ShowModal()
        dialog.Destroy()

#------------------------------------------------------------------------------
class Mod_Groups(Mod_Labels):
    """Add mod group links."""
    def __init__(self):
        """Initialize."""
        self.column     = 'group'
        self.setKey     = 'mash.mods.groups'
        self.editMenu   = _('Edit Groups...')
        self.editWindow = _('Groups')
        self.addPrompt  = _('Add group:')
        self.idList     = ID_GROUPS
        Mod_Labels.__init__(self)

#------------------------------------------------------------------------------
class Mod_Ratings(Mod_Labels):
    """Add mod rating links."""
    def __init__(self):
        """Initialize."""
        self.column     = 'rating'
        self.setKey     = 'mash.mods.ratings'
        self.editMenu   = _('Edit Ratings...')
        self.editWindow = _('Ratings')
        self.addPrompt  = _('Add rating:')
        self.idList     = ID_RATINGS
        Mod_Labels.__init__(self)

#------------------------------------------------------------------------------
class Mod_CopyToEsmp(Link):
    """Create an esp(esm) copy of selected esm(esp)."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Copy to Esm'))
        menu.AppendItem(menuItem)
        if len(data) != 1:
            menuItem.Enable(False)
        #--Filetype
        fileInfo = self.fileInfo = window.data[data[0]]
        if fileInfo.isEsm():
            menuItem.SetText(_('Copy to Esp'))

    def Execute(self,event):
        """Handle menu selection."""
        fileInfo = self.fileInfo
        newType = (fileInfo.isEsm() and 'esp') or 'esm'
        modsDir = fileInfo.dir
        curName = fileInfo.name
        newName = curName[:-3]+newType
        #--Replace existing file?
        if os.path.exists(os.path.join(modsDir,newName)):
            result = WarningMessage(self.window,_('Replace existing %s?') % (newName,),
                style=(wx.YES_NO|wx.ICON_EXCLAMATION))
            if result != wx.ID_YES: return
            mosh.modInfos[newName].makeBackup()
        #--Copy, set type, update mtime.
        self.window.data.copy(curName,modsDir,newName,True)
        self.window.data.table.copyRow(curName,newName)
        newInfo = self.window.data[newName]
        newInfo.setType(newType)
        #--Repopulate
        self.window.Refresh(detail=newName)

#------------------------------------------------------------------------------
class Mod_Export_Dialogue(Link):
    """Export dialog from mod to text file."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Dialogue'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(self.data)==1)

    def Execute(self,event):
        """Handle menu selection."""
        fileName = self.data[0]
        textName = os.path.splitext(fileName)[0]+'_Dialogue.txt'
        textDir = settings.get('mosh.workDir',settings['mwDir'])
        #--File dialog
        dialog = wx.FileDialog(self.window,_('Export dialogs to:'),textDir,
            textName, '*.*', wx.SAVE|wx.OVERWRITE_PROMPT )
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        textPath = dialog.GetPath()
        dialog.Destroy()
        (textDir,textName) = os.path.split(textPath)
        settings['mosh.workDir'] = textDir
        settings['mash.dialEdit.path'] = textPath
        #--Export
        fileDials = mosh.FileDials(mosh.modInfos[fileName])
        fileDials.load()
        fileDials.dumpText(textPath,'topic')

#------------------------------------------------------------------------------
class Mod_Export_Scripts(Link):
    """Export scripts from mod to text file."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Scripts'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(self.data)==1)

    def Execute(self,event):
        """Handle menu selection."""
        fileName = self.data[0]
        textName = os.path.splitext(fileName)[0]+'_Scripts.mws'
        textDir = settings.get('mosh.workDir',settings['mwDir'])
        #--File dialog
        dialog = wx.FileDialog(self.window,_('Export scripts to:'),textDir,
            textName, '*.*', wx.SAVE|wx.OVERWRITE_PROMPT )
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        textPath = dialog.GetPath()
        dialog.Destroy()
        (textDir,textName) = os.path.split(textPath)
        settings['mosh.workDir'] = textDir
        settings['mash.scriptEdit.path'] = textPath
        #--Export
        fileScripts = mosh.FileScripts(mosh.modInfos[fileName])
        fileScripts.load()
        fileScripts.dumpText(textPath)

#------------------------------------------------------------------------------
class Mod_Import_Dialogue(Link):
    """Import dialog from text file to mod."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Dialogue'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(self.data)==1)

    def Execute(self,event):
        """Handle menu selection."""
        fileName = self.data[0]
        textPath = settings.get('mash.dialEdit.path')
        if textPath:
            (textDir,textName) = os.path.split(textPath)
        else:
            textName = os.path.splitext(fileName)[0]+'_Dialogue.txt'
            textDir = settings.get('mosh.workDir',settings['mwDir'])
        #--File dialog
        dialog = wx.FileDialog(self.window,_('Import dialogs from:'),textDir,
            textName, '*.*', wx.OPEN )
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        textPath = dialog.GetPath()
        dialog.Destroy()
        (textDir,textName) = os.path.split(textPath)
        settings['mosh.workDir'] = textDir
        settings['mash.dialEdit.path'] = textPath
        #--Import
        fileInfo = mosh.modInfos[fileName]
        fileInfo.makeBackup()
        fileDials = mosh.FileDials(fileInfo)
        fileDials.load()
        report = fileDials.loadText(textPath)
        fileDials.save()
        fileInfo.setMTime()
        fileInfo.refresh()
        self.window.Refresh(fileName)
        LogMessage(self.window,'',report,fileName)

#------------------------------------------------------------------------------
class Mod_Import_LCVSchedules(Link):
    """Import LCV Schedules to mod file."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('LCV Schedules'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(self.data)==1)

    def Execute(self,event):
        """Handle menu selection."""
        fileName = self.data[0]
        #--Continue Query
        message = _("Generates LCV schedule scripts from an LCV schedule text file, and inserts (but does not compile) the scripts into the current mod file. You should not use this feature unless you know exactly what you're doing.")
        if ContinueQuery(self.window,message,'mash.schedules.import.continue',_('Import LCV Schedules...')) != wx.ID_OK:
            return
        #--File dialog
        def pickScheduleFile(caption,textPath):
            """Shows file dialog to pick schedule file."""
            if textPath:
                (textDir,textName) = os.path.split(textPath)
            else:
                textDir = settings.get('mosh.workDir',settings['mwDir'])
                textName = 'LCV Schedules.etxt'
            dialog = wx.FileDialog(self.window,caption,textDir,
                textName, '*.*', wx.OPEN )
            if dialog.ShowModal() != wx.ID_OK: 
                textPath = None
            else:
                textPath = dialog.GetPath() 
            dialog.Destroy()
            return textPath
        #--Get text path
        table = self.window.data.table
        textPath = table.getItem(fileName,'schedules.path')
        textPath = pickScheduleFile(_('Import LCV schedules from:'),textPath)
        if not textPath: return
        (textDir,textName) = os.path.split(textPath)
        table.setItem(fileName,'schedules.path',textPath)
        #--Import
        caption = textName
        log = mosh.LogFile(cStringIO.StringIO())
        try:
            generator = mosh.ScheduleGenerator()
            generator.log = log
            generator.loadText(textPath,pickScheduleFile)
            fileInfo = mosh.modInfos.data.get(fileName)
            generator.save(fileInfo)
            fileInfo.refresh()
            self.window.Refresh(fileName)
        finally:
            LogMessage(self.window,'',log.out.getvalue(),caption)

#------------------------------------------------------------------------------
class Mod_Import_MergedLists(Link):
    """Import merged lists from load file."""
    def AppendToMenu(self,menu,window,data):
        """Append link to a menu."""
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Merged Lists'))
        menu.AppendItem(menuItem)
        enable = (len(self.data) == 1 and 
            mosh.modInfos[self.data[0]].tes3.hedr.author == 'Wrye Mash')
        menuItem.Enable(enable)

    def Execute(self,event):
        """Handle activation event."""
        fileName = self.data[0]
        fileInfo = mosh.modInfos[fileName]
        caption = fileName
        log = mosh.LogFile(cStringIO.StringIO())
        progress = ProgressDialog(caption)
        try:
            progress.setMax(10+len(mosh.mwIniFile.loadOrder))
            proCounter = 0
            progress(proCounter,_('Loading ')+fileName)
            fileLists = mosh.FileLists(fileInfo)
            fileLists.log = log
            fileLists.load()
            fileLists.beginMerge()
            proCounter += 5
            #--Go through load list
            bethMasters = set(['Morrowind.esm','Tribunal.esm','Bloodmoon.esm'])
            for loadName in mosh.mwIniFile.loadOrder:
                progress(proCounter,'Reading: '+loadName)
                proCounter += 1
                loadInfo = mosh.modInfos[loadName]
                #--Skip bethesda masters and mods with 'Wrye Mash' as author
                if (loadName in bethMasters or 
                    loadInfo.tes3.hedr.author == 'Wrye Mash'):
                    continue
                #--TesTool file?
                if loadName == 'Merged_Leveled_Lists.esp':
                    message = _("TesTool Merged_Leveled_Lists.esp skipped. Please remove it from your load list.")
                    WarningMessage(self.window,message)
                    continue
                newFL = mosh.FileLists(loadInfo,False)
                newFL.load()
                fileLists.mergeWith(newFL)
            progress(proCounter,_('Saving: ')+fileName)
            fileLists.completeMerge()
            fileLists.sortRecords()
            fileLists.safeSave()
            proCounter += 5
            #--Date
            fileInfo.refresh()
            fileHedr = fileInfo.tes3.hedr
            description = fileHedr.description
            reLists = re.compile('^Lists: .*$',re.M)
            description = reLists.sub(
                r'Lists: %s' % (formatDate(time.time()),),description)
            fileInfo.writeDescription(description)
            self.window.Refresh(fileName)
        finally:
            progress.Destroy()
            logValue = log.out.getvalue()
            if logValue:
                LogMessage(self.window,'',logValue,caption)
            else:
                InfoMessage(self.window,_("No lists required merging."))

#------------------------------------------------------------------------------
class Mod_Import_Scripts(Link):
    """Import scripts from text file to mod."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Scripts'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(self.data)==1)

    def Execute(self,event):
        """Handle menu selection."""
        fileName = self.data[0]
        textPath = settings.get('mash.scriptEdit.path')
        if textPath:
            (textDir,textName) = os.path.split(textPath)
        else:
            textName = os.path.splitext(fileName)[0]+'_Scripts.mws'
            textDir = settings.get('mosh.workDir',settings['mwDir'])
        #--File dialog
        dialog = wx.FileDialog(self.window,_('Import scripts from:'),textDir,
            textName, '*.*', wx.OPEN )
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        textPath = dialog.GetPath()
        dialog.Destroy()
        (textDir,textName) = os.path.split(textPath)
        settings['mosh.workDir'] = textDir
        settings['mash.dialEdit.path'] = textPath
        #--Import
        fileInfo = mosh.modInfos[fileName]
        fileInfo.makeBackup()
        fileScripts = mosh.FileScripts(fileInfo)
        fileScripts.load()
        changed = fileScripts.loadText(textPath)
        if changed:
            fileScripts.save()
            fileInfo.setMTime()
            fileInfo.refresh()
            self.window.Refresh(fileName)
            report = _("Scripts changed:\n* ") + '\n* '.join(changed)
            LogMessage(self.window,'',report,fileName)
        else:
            InfoMessage(self.window,_("No scripts changed."))

#------------------------------------------------------------------------------
class Mod_RenumberRefs(Link):
    """Renumbers the references of an esp in an attempt to avoid local ref 
    conflicts between mods."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Renumber Refs'))
        menu.AppendItem(menuItem)
        menuItem.Enable(
            len(data) == 1 and 
            self.window.data[self.data[0]].isEsp())

    def Execute(self,event):
        """Handle menu selection."""
        import random
        #--Continue Query
        message = _("Renumbers new objects placed by esp, thus reducing likelihood of local ref conflicts between mods. Note that using TESCS on this mod will undo this renumbering. Also if an existing save game depends on this mod, doubling will likely result.")
        if ContinueQuery(self.window,message,'mash.renumberRefs.continue',_('Renumber References...')) != wx.ID_OK:
            return
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        progress = None
        dialog = None
        try:
            #--Log and Progress
            caption = _('Renumbering ')+fileName
            progress = ProgressDialog(caption)
            log = mosh.LogFile(cStringIO.StringIO())
            #--File Refs
            fileRefs = mosh.FileRefs(fileInfo,log=log,progress=progress)
            fileRefs.refresh()
            #--Pick new object index number
            curFirst = fileRefs.getFirstObjectIndex()
            if curFirst == 0:
                InfoMessage(self.window,_("No local references to renumber."))
                return
            table = self.window.data.table
            first = table.getItem(fileName,'firstObjectIndex',random.randint(1001,10001))
            dialog = wx.TextEntryDialog(self.window,
                _("Enter first objectIndex. [Current value: %d]") % (curFirst,),
                _('First Object Index'), `first`)
            if dialog.ShowModal() != wx.ID_OK:
                return
            first = int(dialog.GetValue())
            if not (0 < first <= 100000):
                ErrorMessage(self.window,_("Object index must be an integer in range 1:100,000."))
                return
            if first == curFirst: 
                ErrorMessage(self.window,_("New object index is same as old object index!"))
                return
            #--Renumber objects
            changed = fileRefs.renumberObjects(first)
            fileRefs.safeSave()
            progress = progress.Destroy()
            InfoMessage(self.window,_("References changed: %d.") % (changed,))
            if first == 1:
                table.delItem(fileName,'firstObjectIndex')
            else:
                table.setItem(fileName,'firstObjectIndex',first)
        #--Done
        finally:
            if progress != None: progress.Destroy()
            if dialog: dialog.Destroy()
            self.window.Refresh(fileName)

    def getNewFirst(self,curFirst,newFirst):
        """Puts up a dialog asking user to select a new first number."""
        return 2000

#------------------------------------------------------------------------------
class Mod_ShowReadme(Link):
    """Open the readme."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Readme...'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(data) == 1)

    def Execute(self,event):
        """Handle menu selection."""
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        if not docBrowser: 
            DocBrowser().Show()
            settings['mash.modDocs.show'] = True
        docBrowser.SetMod(fileInfo.name)
        docBrowser.Raise()

#------------------------------------------------------------------------------
class Mod_UpdatersData(ListEditorData):
    """Data capsule for Mod Updaters dialog."""
    def __init__(self,parent,toMod):
        #--Data
        self.toMod = toMod
        #--GUI
        ListEditorData.__init__(self,parent)
        self.showAdd = True
        self.showRemove = True

    def getItemList(self):
        """Returns fromMod list in correct order."""
        objectMaps = mosh.modInfos.getObjectMaps(self.toMod)
        return sorted(objectMaps.keys(), key=lambda a: a.lower())

    def remove(self,fromMod):
        """Removes object mapping from fromMod to self.toMod."""
        mosh.modInfos.removeObjectMap(fromMod,self.toMod)
        mosh.modInfos.saveObjectMaps()
        return True

    def add(self):
        """Peforms add operation."""
        #--Select mod file
        modDir = mosh.modInfos.dir
        wildcard = _('Morrowind Mod Files')+' (*.esp;*.esm)|*.esp;*.esm'
        #--File dialog
        dialog = wx.FileDialog(self.parent,_('Select previous version:'),modDir,
            '', wildcard, wx.OPEN)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return None
        fromPath = dialog.GetPath()
        dialog.Destroy()
        #--In right directory?
        (fromDir,fromMod) = os.path.split(fromPath)
        if fromDir.lower() != modDir.lower():
            ErrorMessage(self.parent,_('Previous mod file must be located in Data Files directory.'))
            return None
        #--Old Refs
        oldInfo = mosh.modInfos[fromMod]
        oldRefs = oldInfo.extras.get('FileRefs')
        if not oldRefs:
            oldRefs = oldInfo.extras['FileRefs'] = mosh.FileRefs(oldInfo,True,True)
            oldRefs.refresh()
        #--New Refs
        newInfo = mosh.modInfos[self.toMod]
        newRefs = newInfo.extras.get('FileRefs')
        if not newRefs:
            newRefs = newInfo.extras['FileRefs'] = mosh.FileRefs(newInfo,True,True)
            newRefs.refresh()
        #--Remap
        objectMap = newRefs.getObjectMap(oldRefs)
        #--Save objectmap?
        if objectMap:
            mosh.modInfos.addObjectMap(fromMod,self.toMod,objectMap)
            mosh.modInfos.saveObjectMaps()
            return fromMod
        #--No object map to save?
        else:
            InfoMessage(self.parent,_("No updater required for conversion from %s to %s.") % 
                (fromMod,self.toMod))
            return None

#------------------------------------------------------------------------------
class Mod_Updaters(Link):
    """Show dialog for editing updaters."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Updaters...'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        data = Mod_UpdatersData(self.window,self.data[0])
        dialog = ListEditorDialog(self.window,-1,_('Updaters'),data)
        dialog.ShowModal()
        dialog.Destroy()

# Saves Links -----------------------------------------------------------------
#------------------------------------------------------------------------------
class Saves_ProfilesData(ListEditorData):
    """Data capsule for save profiles editing dialog."""
    def __init__(self,parent,hidden,defaultName):
        """Initialize."""
        self.hidden,self.defaultName = hidden,defaultName
        #--GUI
        ListEditorData.__init__(self,parent)
        self.showAdd    = True
        self.showRename = True
        self.showRemove = True

    def getItemList(self):
        """Returns load list keys in alpha order."""
        #--Get list of directories in Hidden, but do not include default.
        isGood = lambda a: os.path.isdir(os.path.join(self.hidden,a))
        profiles = [dir for dir in os.listdir(self.hidden) if isGood(dir)]
        profiles.sort(key=string.lower)
        profiles.sort(key=lambda a: a != self.defaultName)
        return profiles

    def add(self):
        """Adds a new profile."""
        newName = TextEntry(self.parent,_("Enter profile name:"))
        if not newName: 
            return False
        if newName in self.getItemList():
            ErrorMessage(self.parent,_('Name must be unique.'))
            return False
        if len(newName) == 0 or len(newName) > 64:
            ErrorMessage(self.parent,
                _('Name must be between 1 and 64 characters long.'))
            return False
        os.mkdir(os.path.join(self.hidden,newName))
        return newName

    def rename(self,oldName,newName):
        """Renames profile oldName to newName."""
        newName = newName.strip()
        lowerNames = (name.lower() for name in self.getItemList())
        #--Error checks
        if oldName == self.defaultName:
            ErrorMessage(self.parent,self.defaultName + _(' cannot be renamed.'))
            return False
        if newName.lower() in lowerNames:
            ErrorMessage(self,_('Name must be unique.'))
            return False
        if len(newName) == 0 or len(newName) > 64:
            ErrorMessage(self.parent,
                _('Name must be between 1 and 64 characters long.'))
            return False
        #--Rename
        oldDir,newDir = (os.path.join(self.hidden,dir) for dir in (oldName,newName))
        os.rename(oldDir,newDir)
        if oldName == settings['mash.profile']:
            settings['mash.profile'] = newName
        return newName

    def remove(self,profile):
        """Removes load list."""
        #--Can't remove active or Default directory.
        if profile ==  settings['mash.profile']:
            ErrorMessage(self.parent,_('Active profile cannot be removed.'))
            return False
        if profile == self.defaultName:
            ErrorMessage(self.parent,_('Default profile cannot be removed.'))
            return False
        #--Get file count. If > zero, verify with user.
        profileDir = os.path.join(self.hidden,profile)
        files = [file for file in os.listdir(profileDir) if mosh.reSaveFile.search(file)]
        if files:
            message = _('Delete profile %s and the %d save files it contains?') % (profile,len(files))
            if WarningQuery(self.parent,message,_('Delete Profile')) != wx.ID_YES:
                return False
        #--Remove directory
        shutil.rmtree(profileDir) #--DO NOT SCREW THIS UP!!!
        return True

#------------------------------------------------------------------------------
class Saves_Profiles:
    """Select a save set profile -- i.e. swap save files in/out."""
    def __init__(self):
        """Initialize."""
        self.idList = ID_PROFILES

    def GetItems(self):
        self.hidden = os.path.join(mosh.saveInfos.dir,settings['mosh.fileInfo.hiddenDir'])
        self.defaultName = _('Default')
        self.defaultDir = os.path.join(self.hidden,self.defaultName)
        if not os.path.exists(self.defaultDir): os.makedirs(self.defaultDir)
        isGood = lambda a: os.path.isdir(os.path.join(self.hidden,a))
        items = [dir for dir in os.listdir(self.hidden) if isGood(dir)]
        items.sort(key=string.lower)
        items.sort(key=lambda a: a!= self.defaultName)
        return items

    def AppendToMenu(self,menu,window,data):
        """Append label list to menu."""
        self.window = window
        #--Edit
        menu.Append(self.idList.EDIT,_("Edit Profiles...")) 
        menu.AppendSeparator()
        #--Profiles
        items = self.GetItems()
        curProfile = settings.get('mash.profile',self.defaultName)
        if curProfile not in items: curProfile = self.defaultName
        for id,item in zip(self.idList,items):
            menuItem = wx.MenuItem(menu,id,item,kind=wx.ITEM_CHECK)
            menu.AppendItem(menuItem)
            menuItem.Check(item.lower() == curProfile.lower())
        #--Events
        wx.EVT_MENU(window,self.idList.EDIT,self.DoEdit)
        wx.EVT_MENU_RANGE(window,self.idList.BASE,self.idList.MAX,self.DoList)

    def DoEdit(self,event):
        """Show profiles editing dialog."""
        data = Saves_ProfilesData(self.window,self.hidden,self.defaultName)
        dialog = ListEditorDialog(self.window,-1,_('Save Profiles'),data)
        dialog.ShowModal()
        dialog.Destroy()

    def DoList(self,event):
        """Handle selection of label."""
        #--Profile Names
        arcProfile = settings.get('mash.profile',self.defaultName)
        srcProfile = self.GetItems()[event.GetId()-self.idList.BASE]
        if srcProfile == arcProfile: return
        #--Dirs
        arcDir,srcDir = [os.path.join(self.hidden,dir) for dir in (arcProfile,srcProfile)]
        savesDir = mosh.saveInfos.dir
        #--Progress
        progress = None
        arcFiles = sorted(mosh.saveInfos.data)
        srcFiles = sorted(name for name in os.listdir(srcDir) if (len(name) > 5 and name[-4:].lower() == '.ess'))
        arcCount,srcCount = len(arcFiles),len(srcFiles)
        if (arcCount + srcCount) == 0: return
        try:
            progress = ProgressDialog(_('Moving Files'))
            #--Move arc saves to arc profile directory
            for num, saveName in enumerate(arcFiles):
                progress(1.0*num/(arcCount + srcCount),saveName)
                savesPath,profPath = [os.path.join(dir,saveName) for dir in (savesDir,arcDir)]
                if not os.path.exists(profPath): os.rename(savesPath,profPath)
            arcIniPath = os.path.join(arcDir,'Morrowind.ini')
            shutil.copyfile(mosh.mwIniFile.path, arcIniPath)
            settings['mash.profile'] = srcProfile
            #--Move src profile directory saves to saves directory.
            for num,saveName in enumerate(srcFiles):
                progress(1.0*(arcCount + num)/(arcCount + srcCount),saveName)
                savesPath,profPath = [os.path.join(dir,saveName) for dir in (savesDir,srcDir)]
                if not os.path.exists(savesPath): os.rename(profPath,savesPath)
            srcIniPath = os.path.join(srcDir,'Morrowind.ini')
            if os.path.exists(srcIniPath):
                shutil.copyfile(srcIniPath,mosh.mwIniFile.path)
            mashFrame.SetTitle('Wrye Mash: '+srcProfile)
        finally:
            progress.Destroy()
        self.window.details.SetFile(None)

#------------------------------------------------------------------------------
class Saves_MapGridLines(Link):
    """Turns Map Gridlines on/off."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('World Map Gridlines'),kind=wx.ITEM_CHECK)
        menu.AppendItem(menuItem)
        menuItem.Check(settings['mash.worldMap.gridLines'])

    def Execute(self,event):
        """Handle menu selection."""
        settings['mash.worldMap.gridLines'] = not settings['mash.worldMap.gridLines']

# Save Links ------------------------------------------------------------------
#------------------------------------------------------------------------------
class Save_Duplicate(File_Duplicate):
    """Create a duplicate of the (savegame) file."""

    def Execute(self,event):
        """Handle menu selection."""
        data = self.data
        fileName = data[0]
        fileInfo = self.window.data[fileName]
        saveName = fileInfo.tes3.hedr.description + _(" Copy")
        if len(saveName) > 31: saveName = saveName[:31]
        #--Save name
        dialog = wx.TextEntryDialog(self.window,_('Duplicate as:'),_('Duplicate'),saveName)
        result = dialog.ShowModal()
        saveName = dialog.GetValue()
        dialog.Destroy()
        if result != wx.ID_OK or not saveName:
            return
        if len(saveName) > 31: saveName = saveName[:31]
        #--File Name
        base = re.sub(r'\W','',saveName)
        if not base: base = 'SaveGame'
        if len(base) > 8: base = base[:8]
        count = 0
        destName = "%s%04d.ess" % (base,count)
        destDir = fileInfo.dir
        while os.path.exists(os.path.join(destDir,destName)):
            count += 1
            destName = "%s%04d.ess" % (base,count)
        #--Copy file and table info.
        self.window.data.copy(fileName,destDir,destName,setMTime=True)
        self.window.data.table.copyRow(fileName,destName)
        #--Set save name in new file
        saveInfo = self.window.data[destName]
        saveInfo.tes3.hedr.description = saveName
        saveInfo.tes3.hedr.changed = True
        saveInfo.writeHedr()
        #--Repopulate
        self.window.Refresh(detail=destName)
        
#------------------------------------------------------------------------------
class Save_LoadMasters(Link):
    """Sets the load list to the save game's masters."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Load Masters'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        #--Clear current mods
        loadFiles = mosh.mwIniFile.loadFiles[:]
        for loadFile in loadFiles:
            mosh.modInfos.unload(loadFile,0)
        #--Select my mods
        missing = []
        for masterName in fileInfo.masterNames:
            try:
                mosh.modInfos.load(masterName,0)
            except KeyError, error:
                missing.append(error.args[0])
        mosh.mwIniFile.safeSave()
        #--Repopulate mods
        modList.PopulateItems()
        saveList.PopulateItems()
        self.window.details.SetFile(fileName)
        #--Missing masters?
        if missing:
            message = (_('Please update masters to correct for missing masters (%s).')
                % (','.join(missing),))
            WarningMessage(self.window,message)

#------------------------------------------------------------------------------
class Save_MapNotes(Link):
    """Extracts map notes from save game."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Map Notes'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        reNewLine = re.compile(r'\n')
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        caption = _('Map Notes: ')+fileName
        progress = ProgressDialog(caption)
        log = mosh.LogFile(cStringIO.StringIO())
        try:
            fileRefs = mosh.FileRefs(fileInfo,True,True,log=log,progress=progress)
            fileRefs.refresh()
            for cell in sorted(fileRefs.cells, cmp=lambda a,b: a.cmpId(b)):
                log.setHeader(cell.getId())
                for endRecord in cell.endRecords:
                    if endRecord.name == 'MPNT':
                        log('  '+reNewLine.sub(r'\n  ',mosh.cstrip(endRecord.data)))
            LogMessage(self.window,'',log.out.getvalue(),caption)
        finally:
            if progress != None: progress.Destroy()

#------------------------------------------------------------------------------
class Save_Remove_SpawnedCreatures(Link):
    """Removes all lvcrs (leveled creature spawn points)."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Spawned Creatures'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        caption = fileName
        progress = ProgressDialog(caption)
        count = 0
        try:
            fileRefs = mosh.FileRefs(fileInfo,progress=progress)
            fileRefs.refresh()
            count = fileRefs.removeLvcrs()
            if count:
                fileRefs.removeOrphanContents()
                fileRefs.safeSave()
                InfoMessage(self.window,_("%d spawn points removed/reset.") % (count,))
            else:
                InfoMessage(self.window,_("No spawn points to remove/reset!"))
        finally:
            if progress != None: progress.Destroy()
            self.window.Refresh(fileName)

#------------------------------------------------------------------------------
class Save_Remove_DebrisCells(Link):
    """Removes all debris cells -- cells that are not supported by any of the master files."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Debris Cells'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--Continue Query
        message = _("This command will remove all references in cells that have been visited, but are not supported by the current set of masters. Typically this is used to cleanup exterior cells that were added by mods that have since been removed. Note that if you have intentionally placed objects into such cells (e.g., a travelling ship), then those objects WILL BE LOST!")
        if ContinueQuery(self.window,message,'mash.removeDebrisCells.continue',_('Remove Debris Cells')) != wx.ID_OK:
            return
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        progress = ProgressDialog(fileName)
        log = mosh.LogFile(cStringIO.StringIO())
        count = 0
        try:
            #--Log and Progress
            caption = _('Removing Debris Cells')
            #--World Refs
            worldRefs = mosh.WorldRefs(log=log,progress=progress)
            try:
                worldRefs.addMasters(fileInfo.masterNames)
            except mosh.Tes3RefError, error:
                progress = progress.Destroy()
                message = ((
                    _("%s has bad refs and must be repaired first.\n") +
                    _("\nExample Bad Ref from %s:") +
                    _("\n  Cell: %s\n  Object Id: %s\n  Object Index: %d")+
                    _("\n  Mod Index: %d (%s)")) % 
                    (error.inName,error.inName,error.cellId,error.objId,error.iObj,error.iMod,error.masterName))
                ErrorMessage(self.window,message)
                return
            #--File refs
            fileRefs = mosh.FileRefs(fileInfo,log=log,progress=progress)
            fileRefs.refresh()
            count = worldRefs.removeDebrisCells(fileRefs)
            if count:
                fileRefs.safeSave()
                LogMessage(self.window,'',log.out.getvalue(),fileName)
            else:
                InfoMessage(self.window,_("No orphaned content present."))
        finally:
            progress.Destroy()
            self.window.Refresh(fileName)

#------------------------------------------------------------------------------
class Save_RepairAll(Link):
    """Repairs the save game's refs by comparing their type and id against the
    types and ids of the save game's masters."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Repair All'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        if fileInfo.getStatus() > 10:
            WarningMessage(self.window,
               _( "File master list is out of date. Please edit masters before attempting repair."))
            return
        progress = None
        dialog = None
        try:
            #--Log and Progress
            caption = _('Repairing ')+fileName
            progress = ProgressDialog(caption)
            log = mosh.LogFile(cStringIO.StringIO())
            #--World Refs
            worldRefs = mosh.WorldRefs(log=log,progress=progress)
            try:
                worldRefs.addMasters(fileInfo.masterNames)
            except mosh.Tes3RefError, error:
                progress = progress.Destroy()
                message = ((
                    _("%s has bad refs and must be repaired first.\n") +
                    _("\nExample Bad Ref from %s:") +
                    _("\n  Cell: %s\n  Object Id: %s\n  Object Index: %d")+
                    _("\n  Mod Index: %d (%s)")) % 
                    (error.inName,error.inName,error.cellId,error.objId,error.iObj,error.iMod,error.masterName))
                ErrorMessage(self.window,message)
                return
            #--File Refs for Save File
            progress.setBaseScale()
            fileRefs = mosh.FileRefs(fileInfo,log=log,progress=progress)
            fileRefs.refresh()
            (cntRepaired,cntDeleted,cntUnnamed) = worldRefs.repair(fileRefs)
            #--Save games only...
            #--Remove debris records.
            cntDebris = worldRefs.removeDebrisRecords(fileRefs)
            #--Remove orphan contents
            log.setHeader(_("Orphaned content records:"))
            cntOrphans = fileRefs.removeOrphanContents()
            #--Remove bad leveled lists
            log.setHeader(_("Overriding lists:"))
            cntLists = worldRefs.removeOverLists(fileRefs)
            #--No problems?
            if not (cntRepaired or cntDeleted or cntUnnamed or cntDebris or cntOrphans or cntLists):
                progress = progress.Destroy()
                InfoMessage(self.window,_("No problems found!"))
                return
            fileRefs.safeSave()
            progress = progress.Destroy()
            #--Problem Dialog
            message = (_("Objects repaired: %d.\nObjects deleted: %d.") % 
                (cntRepaired,cntDeleted))
            message += (_("\nDebris records deleted: %d.\nOrphan contents deleted: %d.") % 
                (cntDebris,cntOrphans))
            message += (_("\nOverriding lists deleted: %d.") % (cntLists,))
            LogMessage(self.window,message,log.out.getvalue(),caption)
        #--Done
        finally:
            if progress != None: progress.Destroy()
            if dialog: dialog.Destroy()
            self.window.Refresh(fileName)

#------------------------------------------------------------------------------
class Save_Review(Link):
    """Presents a list of warnings of possible problems."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Review'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        progress = None
        try:
            #--Log and Progress
            caption = _('Review of ')+fileName
            progress = ProgressDialog(caption)
            log = mosh.LogFile(cStringIO.StringIO())
            #--File Refs for Save File
            fileRefs = mosh.FileRefs(fileInfo,log=log,progress=progress)
            fileRefs.refresh()
            progress.Destroy()
            #--List Bad refs
            fileRefs.listBadRefScripts()
            #--No problems?
            if not log.out.getvalue():
                InfoMessage(self.window,_("Nothing noteworthy found."))
                return
            #--Problem Dialog
            LogMessage(self.window,'',log.out.getvalue(),caption)
        #--Done
        finally:
            if progress != None: progress.Destroy()

#------------------------------------------------------------------------------
class Save_ShowJournal(Link):
    """Open the journal."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Journal...'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(data) == 1)

    def Execute(self,event):
        """Handle menu selection."""
        fileName = self.data[0]
        if not journalBrowser: 
            JournalBrowser().Show()
            settings['mash.journal.show'] = True
        journalBrowser.SetSave(fileName)
        journalBrowser.Raise()

#------------------------------------------------------------------------------
class Save_UpdateWorldMap(Link):
    """Updates the savegame's world map."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Update Map'))
        menu.AppendItem(menuItem)
        if len(data) != 1: menuItem.Enable(False)

    def Execute(self,event):
        """Handle menu selection."""
        #--File Info
        fileName = self.data[0]
        fileInfo = self.window.data[fileName]
        if fileInfo.getStatus() > 10:
            WarningMessage(self.window,
               _( "File master list is out of date. Please edit masters before attempting repair."))
            return
        progress = None
        dialog = None
        try:
            #--Log and Progress
            caption = _('Re-mapping ')+fileName
            progress = ProgressDialog(caption)
            #--World Refs
            worldRefs = mosh.WorldRefs(progress=progress)
            try:
                worldRefs.addMasters(fileInfo.masterNames)
            except mosh.Tes3RefError, error:
                progress = progress.Destroy()
                message = ((
                    _("%s has bad refs and must be repaired first.\n") +
                    _("\nExample Bad Ref from %s:") +
                    _("\n  Cell: %s\n  Object Id: %s\n  Object Index: %d")+
                    _("\n  Mod Index: %d (%s)")) % 
                    (error.inName,error.inName,error.cellId,error.objId,error.iObj,error.iMod,error.masterName))
                ErrorMessage(self.window,message)
                return
            #--File Refs for Save File
            progress.setBaseScale()
            fileRefs = mosh.FileRefs(fileInfo,progress=progress)
            fileRefs.refresh()
            worldRefs.repairWorldMap(fileRefs,settings['mash.worldMap.gridLines'])
            fileRefs.safeSave()
            progress = progress.Destroy()
            InfoMessage(self.window,_("World map updated."))
        #--Done
        finally:
            if progress != None: progress.Destroy()
            if dialog: dialog.Destroy()
            self.window.Refresh(fileName)

# Masters Links ---------------------------------------------------------------
#------------------------------------------------------------------------------
class Masters_CopyList(Link):
    """Copies list of masters to clipboard."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_("Copy List"))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle menu selection."""
        fileInfo = self.window.fileInfo
        fileName = fileInfo.name
        #--Get masters list
        caption = _('Masters for %s:') % (fileName,)
        log = mosh.LogFile(cStringIO.StringIO())
        log.setHeader(caption)
        for num, name in enumerate(fileInfo.masterNames):
            version = mosh.modInfos.getVersion(name)
            if version: 
                log('%03d  %s  (Version %s)' % (num+1,name,version))
            else:
                log('%03d  %s' % (num+1,name))
        #--Copy to clipboard
        if (wx.TheClipboard.Open()):
            text = mosh.winNewLines(log.out.getvalue())
            wx.TheClipboard.SetData(wx.TextDataObject(text))
            wx.TheClipboard.Close()

#------------------------------------------------------------------------------
class Masters_Update(Link):
    """Updates masters list and prepares it for further manual editing.
    Automatically fixes: names, sizes and load order."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_("Update"))
        menu.AppendItem(menuItem)
        menuItem.Enable(not self.window.edited)

    def Execute(self,event):
        """Handle menu selection."""
        self.window.InitEdit()

#------------------------------------------------------------------------------
class Masters_SyncToLoad(Link):
    """Syncs master list to current load list (for save games) or to masters files in current load list (for mods)."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        if self.window.fileInfo.isMod():
            menuItem = wx.MenuItem(menu,self.id,_("Sync to Load ESMs"))
        else:
            menuItem = wx.MenuItem(menu,self.id,_("Sync to Load List"))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        """Handle menu selection."""
        if not self.window.edited:
            self.window.InitEdit()
        #--Data
        fileInfo = self.window.fileInfo
        fileName = fileInfo.name
        #--Precheck Circularity
        if fileInfo.isMod():
            newMasters = []
            for loadFile in mosh.mwIniFile.loadFiles:
                if loadFile[-1].lower() == 'm':
                    newMasters.append(loadFile)
            if mosh.modInfos.circularMasters([fileName],newMasters):
                ErrorMessage(self.window,
                    _("Cannot Sync to Load ESMs, since resulting master list would be circular."))
                return
        #--Unselect all
        for masterName in self.window.newMasters[:]:
            self.window.unload(masterName)
        #--Select esms?
        if fileInfo.isMod():
            fileName = fileInfo.name
            for loadFile in mosh.mwIniFile.loadFiles:
                if loadFile[-1].lower() == 'm':
                    self.window.load(loadFile)
        #--Select all?
        else:
            for loadFile in mosh.mwIniFile.loadFiles:
                self.window.load(loadFile)
        #--Repop
        self.window.PopulateItems()

# Master Links ----------------------------------------------------------------
#------------------------------------------------------------------------------
class Master_ChangeTo(Link):
    """Rename/replace master through file dialog."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_("Change to..."))
        menu.AppendItem(menuItem)
        menuItem.Enable(self.window.edited)

    def Execute(self,event):
        """Handle menu selection."""
        itemId = self.data[0]
        masterInfo = self.window.data[itemId]
        masterName = masterInfo.name
        #--File Dialog
        wildcard = _('Morrowind Mod Files')+' (*.esp;*.esm)|*.esp;*.esm'
        dialog = wx.FileDialog(self.window,_('Change master name to:'),
            mosh.modInfos.dir, masterName, wildcard, wx.OPEN)
        if dialog.ShowModal() != wx.ID_OK: 
            dialog.Destroy()
            return
        (newDir,newName) = os.path.split(dialog.GetPath())
        dialog.Destroy()
        #--Valid directory?
        if newDir != mosh.modInfos.dir:
            ErrorMessage(self.window,
                _("File must be selected from Morrowind Data Files directory."))
            return
        elif newName == masterName:
            return
        #--Unselect item?
        if masterInfo.isLoaded:
            self.window.unload(masterName)
        #--Save Name
        masterInfo.setName(newName)
        self.window.load(newName)
        self.window.PopulateItems()

# Screen Links ------------------------------------------------------------------
#------------------------------------------------------------------------------
class Screens_NextScreenShot(Link):
    """Sets screenshot base name and number."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Next Shot...'))
        menu.AppendItem(menuItem)

    def Execute(self,event):
        ini = mosh.mwIniFile
        base = ini.getSetting('General','Screen Shot Base Name','ScreenShot')
        next = ini.getSetting('General','Screen Shot Index','0')
        rePattern = re.compile(r'^(.+?)(\d*)$',re.I)
        pattern = balt.askText(self.window,_("Screenshot base name, optionally with next screenshot number.\nE.g. ScreenShot or ScreenShot_101 or Subdir\\ScreenShot_201."),_("Next Shot..."),base+next)
        if not pattern: return
        maPattern = rePattern.match(pattern)
        newBase,newNext = maPattern.groups()
        settings = {LString('General'):{
            LString('Screen Shot Base Name'): newBase,
            LString('Screen Shot Index'): (newNext or next),
            LString('Screen Shot Enable'): '1',
            }}
        screensDir = GPath(newBase).head
        if screensDir: 
            if not screensDir.isabs(): screensDir = mosh.dirs['app'].join(screensDir)
            screensDir.makedirs()
        ini.saveSettings(settings)
        mosh.screensData.refresh()
        self.window.RefreshUI()

#------------------------------------------------------------------------------
class Screen_ConvertToJpg(Link):
    """Converts selected images to jpg files."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Convert to jpg'))
        menu.AppendItem(menuItem)
        convertable = [name for name in self.data if GPath(name).cext != '.jpg']
        menuItem.Enable(len(convertable) > 0)

    def Execute(self,event):
        #--File Info
        srcDir = self.window.data.dir
        progress = balt.Progress(_("Converting to Jpg"))
        try:
            progress.setFull(len(self.data))
            srcDir = mosh.screensData.dir
            for index,fileName in enumerate(self.data):
                progress(index,fileName.s)
                srcPath = srcDir.join(fileName)
                destPath = srcPath.root+'.jpg'
                if srcPath == destPath or destPath.exists(): continue
                bitmap = wx.Bitmap(srcPath.s)
                result = bitmap.SaveFile(destPath.s,wx.BITMAP_TYPE_JPEG)
                if not result: continue
                srcPath.remove()
        finally:
            if progress: progress.Destroy()
            mosh.screensData.refresh()
            self.window.RefreshUI()

#------------------------------------------------------------------------------
class Screen_Rename(Link):
    """Renames files by pattern."""
    def AppendToMenu(self,menu,window,data):
        Link.AppendToMenu(self,menu,window,data)
        menuItem = wx.MenuItem(menu,self.id,_('Rename...'))
        menu.AppendItem(menuItem)
        menuItem.Enable(len(data) > 0)

    def Execute(self,event):
        #--File Info
        rePattern = re.compile(r'^([^\\/]+?)(\d*)(\.(jpg|bmp))$',re.I)
        fileName0 = self.data[0]
        pattern = balt.askText(self.window,_("Enter new name. E.g. Screenshot 123.bmp"),
            _("Rename Files"),fileName0.s)
        if not pattern: return
        maPattern = rePattern.match(pattern)
        if not maPattern:
            balt.showError(self.window,_("Bad extension or file root: ")+pattern)
            return
        root,numStr,ext = maPattern.groups()[:3]
        numLen = len(numStr)
        num = int(numStr or 0)
        screensDir = mosh.screensData.dir
        for oldName in map(GPath,self.data):
            newName = GPath(root)+numStr+oldName.ext
            if newName != oldName: 
                oldPath = screensDir.join(oldName)
                newPath = screensDir.join(newName)
                if not newPath.exists():
                    oldPath.moveTo(newPath)
            num += 1
            numStr = `num`
            numStr = '0'*(numLen-len(numStr))+numStr
        mosh.screensData.refresh()
        self.window.RefreshUI()

# App Links -------------------------------------------------------------------
#------------------------------------------------------------------------------
class App_Morrowind(Link):
    """Launch Morrowind."""
    def GetBitmapButton(self,window,style=0):
        if not self.id: self.id = wx.NewId()
        button = wx.BitmapButton(window,self.id,images['morrowind'].GetBitmap(),style=style)
        button.SetToolTip(wx.ToolTip(_("Launch Morrowind")))
        wx.EVT_BUTTON(button,self.id,self.Execute)
        return button

    def Execute(self,event):
        """Handle menu selection."""
        cwd = os.getcwd()
        os.chdir(settings['mwDir'])
        os.spawnl(os.P_NOWAIT,os.path.join(settings['mwDir'],'Morrowind.exe'))
        os.chdir(cwd)
        if settings.get('mash.autoQuit.on',False):
            mashFrame.Close()

#------------------------------------------------------------------------------
class App_Button(Link):
    """Launch an application."""

    def __init__(self,exePathArgs,image,tip,obseTip=None,obseArg=None):
        """Initialize
        exePathArgs (string): exePath
        exePathArgs (tuple): (exePath,*exeArgs)"""
        Link.__init__(self)
        self.gButton = None
        if isinstance(exePathArgs,tuple):
            self.exePath = exePathArgs[0]
            self.exeArgs = exePathArgs[1:]
        else:
            self.exePath = exePathArgs
            self.exeArgs = tuple()
        self.image = image
        self.tip = tip
        #--Java stuff
        if self.exePath and str((self.exePath).ext) == '.jar': #Sometimes exePath is "None"
            self.isJava = True
            self.java = GPath(os.environ['SYSTEMROOT']).join('system32','javaw.exe')
            self.jar = self.exePath
            self.appArgs = ''.join(self.exeArgs)
        else:
            self.isJava = False

    def IsPresent(self):
        if self.isJava:
            return self.java.exists() and self.jar.exists()
        else:
            return self.exePath.exists()

    def GetBitmapButton(self,window,style=0):
        if self.IsPresent():
            self.gButton = bitmapButton(window,self.image.GetBitmap(),style=style,
                onClick=self.Execute,tip=self.tip)
            return self.gButton
        else:
            return None

    def Execute(self,event,extraArgs=None):
        if self.isJava:
            cwd = bolt.Path.getcwd()
            self.jar.head.setcwd()
            os.spawnv(os.P_NOWAIT,self.java.s,(self.java.stail,'-jar',self.jar.stail,self.appArgs))
            cwd.setcwd()
        else:
            exeArgs = self.exeArgs
            exePath = self.exePath
            exeArgs = (exePath.stail,)+exeArgs
            if extraArgs: exeArgs += extraArgs
            statusBar.SetStatusText(' '.join(exeArgs),1)
            cwd = bolt.Path.getcwd()
            exePath.head.setcwd()
            os.spawnv(os.P_NOWAIT,exePath.s,exeArgs)
            cwd.setcwd()
#------------------------------------------------------------------------------
class AutoQuit_Button(Link):
    """Button toggling application closure when launching Oblivion."""
    def __init__(self):
        Link.__init__(self)
        self.gButton = None
    
    def SetState(self,state=None):
        """Sets state related info. If newState != none, sets to new state first. 
        For convenience, returns state when done."""
        if state == None: #--Default
            state = settings.get('mash.autoQuit.on',False)
        elif state == -1: #--Invert
            state = not settings.get('mash.autoQuit.on',False)
        settings['mash.autoQuit.on'] = state
        image = images[('checkbox.red.off','checkbox.red.x')[state]]
        tip = (_("Auto-Quit Disabled"),_("Auto-Quit Enabled"))[state]
        self.gButton.SetBitmapLabel(image.GetBitmap())
        self.gButton.SetToolTip(tooltip(tip))

    def GetBitmapButton(self,window,style=0):
        bitmap = images['checkbox.red.off'].GetBitmap()
        gButton = self.gButton = wx.BitmapButton(window,-1,bitmap,style=style)
        gButton.Bind(wx.EVT_BUTTON,self.Execute)
        gButton.SetSize((16,16))
        self.SetState()
        return gButton

    def Execute(self,event):
        """Invert state."""
        self.SetState(-1)

#------------------------------------------------------------------------------
class App_Help(Link):
    """Show help browser."""
    def GetBitmapButton(self,window,style=0):
        if not self.id: self.id = wx.NewId()
        button = wx.BitmapButton(window,self.id,images['help'].GetBitmap(),style=style)
        button.SetToolTip(wx.ToolTip(_("Help File")))
        wx.EVT_BUTTON(button,self.id,self.Execute)
        return button

    def Execute(self,event):
        """Handle menu selection."""
        if not helpBrowser: 
            HelpBrowser().Show()
            settings['mash.help.show'] = True
        helpBrowser.Raise()

# Initialization --------------------------------------------------------------
#def InitSettings():
#    """Initialize settings (configuration store). First, read from file, then
#    load defaults (defaults will not overwrite items extracted from file)."""
#    mosh.initSettings()
#    global settings
#    settings = mosh.settings
#    settings.loadDefaults(settingDefaults)
    #mosh.initDirs()

def InitImages():
    """Initialize images (icons, checkboxes, etc.)."""
    #--Standard
    images['save.on'] = Image(r'images\save_on.png',wx.BITMAP_TYPE_PNG)
    images['save.off'] = Image(r'images\save_off.png',wx.BITMAP_TYPE_PNG)
    #--Misc
    images['morrowind'] = Image(r'images\morrowind.png',wx.BITMAP_TYPE_PNG)
    images['help'] = Image(r'images\help.png',wx.BITMAP_TYPE_PNG)
    #--Tools
    images['doc.on'] = Image(r'images\doc_on.png',wx.BITMAP_TYPE_PNG)
    #--Checkboxes
    images['mash.checkboxes'] = Checkboxes()
    images['checkbox.green.on.32'] = (
        Image(r'images\checkbox_green_on_32.png',wx.BITMAP_TYPE_PNG))
    images['checkbox.blue.on.32'] = (
        Image(r'images\checkbox_blue_on_32.png',wx.BITMAP_TYPE_PNG))
    images['checkbox.red.x'] = Image(r'images\checkbox_red_x.png',wx.BITMAP_TYPE_PNG)
    #--Applications Icons
    wryeMashIcons = balt.ImageBundle()
    wryeMashIcons.Add(images['checkbox.green.on'])
    wryeMashIcons.Add(images['checkbox.green.on.32'])
    images['mash.icons'] = wryeMashIcons
    #--Application Subwindow Icons
    wryeMashIcons2 = balt.ImageBundle()
    wryeMashIcons2.Add(images['checkbox.blue.on'])
    wryeMashIcons2.Add(images['checkbox.blue.on.32'])
    images['mash.icons2'] = wryeMashIcons2
    #--Colors
    colors['mash.esm'] = (220,220,255)
    colors['mash.doubleTime.not'] = 'WHITE'
    colors['mash.doubleTime.exists'] = (255,220,220)
    colors['mash.doubleTime.load'] = (255,100,100)
    colors['mash.exOverLoaded'] = (0xFF,0x99,0)
    colors['mash.masters.remapped'] = (100,255,100)
    colors['mash.masters.changed'] = (220,255,220)
    colors['bash.installers.skipped'] = (0xe0,0xe0,0xe0)
    colors['bash.installers.outOfOrder'] = (0xDF,0xDF,0xC5)
    colors['bash.installers.dirty'] = (0xFF,0xBB,0x33)

def InitStatusBar():
    """Initialize status bar buttons."""
    MashStatusBar.buttons.append(App_Morrowind())
    MashStatusBar.buttons.append(AutoQuit_Button())
    #MashStatusBar.buttons.append(
    #    App_BOSS(
    #        mosh.dirs['app'].join('Data\\BOSS.bat'),
    #        Image(r'images/Boss'+mosh.inisettings['iconSize']+'.png'),
    #        _("Launch BOSS")))
    if mosh.inisettings['showmodelingtoollaunchers']:
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['BlenderPath'],
                Image(r'images/blender'+mosh.inisettings['iconSize']+'.png'),
                _("Launch Blender")))
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['GmaxPath'],
                Image(r'images/gmax'+mosh.inisettings['iconSize']+'.png'),
                _("Launch Gmax")))
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['MayaPath'],
                Image(r'images/maya'+mosh.inisettings['iconSize']+'.png'),
                _("Launch Maya")))
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['MaxPath'],
                Image(r'images/max'+mosh.inisettings['iconSize']+'.png'),
                _("Launch 3dsMax")))
    if mosh.inisettings['showmodelingtoollaunchers'] or mosh.inisettings['showtexturetoollaunchers']:
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['NifskopePath'],
                Image(r'images/nifskope'+mosh.inisettings['iconSize']+'.png'),
                _("Launch Nifskope")))
    if mosh.inisettings['showtexturetoollaunchers']:
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['GIMP'],
                Image(r'images/gimp'+mosh.inisettings['iconSize']+'.png'),
                _("Launch GIMP")))
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['Photoshop'],
                Image(r'images/photoshop'+mosh.inisettings['iconSize']+'.png'),
                _("Launch Photoshop")))
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['Artweaver'],
                Image(r'images/artweaver'+mosh.inisettings['iconSize']+'.png'),
                _("Launch Artweaver")))
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['PaintNET'],
                Image(r'images/paint.net'+mosh.inisettings['iconSize']+'.png'),
                _("Launch Paint.NET")))
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['DDSConverter'],
                Image(r'images/ddsconverter'+mosh.inisettings['iconSize']+'.png'),
                _("Launch DDSConverter")))
    if mosh.inisettings['showaudiotoollaunchers']:
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['Audacity'],
                Image(r'images/audacity'+mosh.inisettings['iconSize']+'.png'),
                _("Launch Audacity")))
    MashStatusBar.buttons.append(
        App_Button(
            mosh.dirs['Fraps'],
            Image(r'images/fraps'+mosh.inisettings['iconSize']+'.png'),
            _("Launch Fraps")))
    MashStatusBar.buttons.append(
        App_Button(
            mosh.dirs['NPP'],
            Image(r'images/notepad++'+mosh.inisettings['iconSize']+'.png'),
            _("Launch Notepad++")))
    if mosh.inisettings['custom1opt']:
        MashStatusBar.buttons.append(
            App_Button(
                (mosh.dirs['Custom1'], mosh.inisettings['custom1opt']),
                Image(r'images/custom1'+mosh.inisettings['iconSize']+'.png'),
                (mosh.inisettings['custom1txt'])))
    else:
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['Custom1'],
                Image(r'images/custom1'+mosh.inisettings['iconSize']+'.png'),
                (mosh.inisettings['custom1txt'])))
    if mosh.inisettings['custom2opt']:
        MashStatusBar.buttons.append(
            App_Button(
                (mosh.dirs['Custom2'], mosh.inisettings['custom2opt']),
                Image(r'images/custom2'+mosh.inisettings['iconSize']+'.png'),
                (mosh.inisettings['custom2txt'])))
    else:
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['Custom2'],
                Image(r'images/custom2'+mosh.inisettings['iconSize']+'.png'),
                (mosh.inisettings['custom2txt'])))
    if mosh.inisettings['custom3opt']:
        MashStatusBar.buttons.append(
            App_Button(
                (mosh.dirs['Custom3'], mosh.inisettings['custom3opt']),
                Image(r'images/custom3'+mosh.inisettings['iconSize']+'.png'),
                (mosh.inisettings['custom3txt'])))
    else:
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['Custom3'],
                Image(r'images/custom3'+mosh.inisettings['iconSize']+'.png'),
                (mosh.inisettings['custom3txt'])))
    if mosh.inisettings['custom4opt']:
        MashStatusBar.buttons.append(
            App_Button(
                (mosh.dirs['Custom4'], mosh.inisettings['custom4opt']),
                Image(r'images/custom4'+mosh.inisettings['iconSize']+'.png'),
                (mosh.inisettings['custom4txt'])))
    else:
        MashStatusBar.buttons.append(
            App_Button(
                mosh.dirs['Custom4'],
                Image(r'images/custom4'+mosh.inisettings['iconSize']+'.png'),
                (mosh.inisettings['custom4txt'])))
    MashStatusBar.buttons.append(App_Help())

def InitMasterLinks():
    """Initialize master list menus."""
    if True: #--Sort by
        sortMenu = MenuLink(_("Sort by"))
        sortMenu.links.append(Mods_EsmsFirst())
        sortMenu.links.append(SeparatorLink())
        sortMenu.links.append(Files_SortBy('File'))
        sortMenu.links.append(Files_SortBy('Author'))
        sortMenu.links.append(Files_SortBy('Group'))
        sortMenu.links.append(Files_SortBy('Load Order'))
        sortMenu.links.append(Files_SortBy('Modified'))
        sortMenu.links.append(Files_SortBy('Status'))
        MasterList.mainMenu.append(sortMenu)
    #--------------------------------------------
    MasterList.mainMenu.append(SeparatorLink())
    MasterList.mainMenu.append(Masters_CopyList())
    MasterList.mainMenu.append(Masters_SyncToLoad())
    MasterList.mainMenu.append(Masters_Update())
    
    #--MasterList: Item Links
    MasterList.itemMenu.append(Master_ChangeTo())
    
def InitInstallerLinks():
    """Initialize people tab menus."""
    #--Column links
    #--Sorting
    InstallersPanel.mainMenu.append(Installers_SortActive())
    InstallersPanel.mainMenu.append(Installers_SortProjects())
    #InstallersPanel.mainMenu.append(Installers_SortStructure())
    #--Actions
    InstallersPanel.mainMenu.append(SeparatorLink())
    InstallersPanel.mainMenu.append(balt.Tanks_Open())
    InstallersPanel.mainMenu.append(Installers_Refresh(fullRefresh=False))
    InstallersPanel.mainMenu.append(Installers_Refresh(fullRefresh=True))
    #InstallersPanel.mainMenu.append(Mods_IniTweaks())
    InstallersPanel.mainMenu.append(SeparatorLink())
    InstallersPanel.mainMenu.append(Installers_AnnealAll())
    #--Behavior
    InstallersPanel.mainMenu.append(SeparatorLink())
    InstallersPanel.mainMenu.append(Installers_AvoidOnStart())
    InstallersPanel.mainMenu.append(Installers_Enabled())
    InstallersPanel.mainMenu.append(SeparatorLink())
    InstallersPanel.mainMenu.append(Installers_AutoAnneal())
    InstallersPanel.mainMenu.append(Installers_RemoveEmptyDirs())
    InstallersPanel.mainMenu.append(Installers_ConflictsReportShowsInactive())
    InstallersPanel.mainMenu.append(Installers_ConflictsReportShowsLower())

    #--Item links
    #--File
    InstallersPanel.itemMenu.append(Installer_Open())
    InstallersPanel.itemMenu.append(Installer_Duplicate())
    InstallersPanel.itemMenu.append(Installer_Delete())
    #--Install, uninstall, etc.
    InstallersPanel.itemMenu.append(SeparatorLink())
    InstallersPanel.itemMenu.append(Installer_Refresh())
    InstallersPanel.itemMenu.append(Installer_Move())
    InstallersPanel.itemMenu.append(SeparatorLink())
    InstallersPanel.itemMenu.append(Installer_HasExtraData())
    InstallersPanel.itemMenu.append(SeparatorLink())
    InstallersPanel.itemMenu.append(Installer_Anneal())
    InstallersPanel.itemMenu.append(Installer_Install())
    InstallersPanel.itemMenu.append(Installer_Install('LAST'))
    InstallersPanel.itemMenu.append(Installer_Install('MISSING'))
    InstallersPanel.itemMenu.append(Installer_Uninstall())
    InstallersPanel.itemMenu.append(SeparatorLink())
    #--Build
    InstallersPanel.itemMenu.append(InstallerArchive_Unpack())
    InstallersPanel.itemMenu.append(InstallerProject_Sync())

def InitModLinks():
    """Initialize Mods tab menus."""
    #--ModList: Column Links
    if True: #--Load
        loadMenu = MenuLink(_("Load"))
        loadMenu.links.append(Mods_LoadList())
        ModList.mainMenu.append(loadMenu)
    if True: #--Sort by
        sortMenu = MenuLink(_("Sort by"))
        sortMenu.links.append(Mods_EsmsFirst())
        sortMenu.links.append(Mods_SelectedFirst())
        sortMenu.links.append(SeparatorLink())
        sortMenu.links.append(Files_SortBy('File'))
        sortMenu.links.append(Files_SortBy('Author'))
        sortMenu.links.append(Files_SortBy('Group'))
        sortMenu.links.append(Files_SortBy('Load Order'))
        sortMenu.links.append(Files_SortBy('Modified'))
        sortMenu.links.append(Files_SortBy('Rating'))
        sortMenu.links.append(Files_SortBy('Size'))
        sortMenu.links.append(Files_SortBy('Status'))
        sortMenu.links.append(Files_SortBy('Version'))
        ModList.mainMenu.append(sortMenu)
    #--------------------------------------------
    ModList.mainMenu.append(SeparatorLink())
    ModList.mainMenu.append(Files_Open())
    ModList.mainMenu.append(Files_Unhide('mod'))
    ModList.mainMenu.append(SeparatorLink())
    ModList.mainMenu.append(Mods_MorrowindIni())
    ModList.mainMenu.append(Mods_IniTweaks())
    ModList.mainMenu.append(Mods_LockTimes())
    ModList.mainMenu.append(Mods_Replacers())
    
    #--ModList: Item Links
    if True: #--File
        fileMenu = MenuLink(_("File"))
        fileMenu.links.append(File_Backup())
        fileMenu.links.append(File_Duplicate())
        fileMenu.links.append(File_Snapshot())
        fileMenu.links.append(SeparatorLink())
        fileMenu.links.append(File_Delete())
        fileMenu.links.append(File_Hide())
        fileMenu.links.append(File_Redate())
        fileMenu.links.append(File_Sort())
        fileMenu.links.append(SeparatorLink())
        fileMenu.links.append(File_RevertToBackup())
        fileMenu.links.append(File_RevertToSnapshot())
        ModList.itemMenu.append(fileMenu)
    if True: #--Groups
        groupMenu = MenuLink(_("Group"))
        groupMenu.links.append(Mod_Groups())
        ModList.itemMenu.append(groupMenu)
    if True: #--Ratings
        ratingMenu = MenuLink(_("Rating"))
        ratingMenu.links.append(Mod_Ratings())
        ModList.itemMenu.append(ratingMenu)
    #--------------------------------------------
    ModList.itemMenu.append(SeparatorLink())
    if True: #--Export
        exportMenu = MenuLink(_("Export"))
        exportMenu.links.append(Mod_Export_Dialogue())
        exportMenu.links.append(Mod_Export_Scripts())
        ModList.itemMenu.append(exportMenu)
    if True: #--Import
        importMenu = MenuLink(_("Import"))
        importMenu.links.append(Mod_Import_Dialogue())
        importMenu.links.append(Mod_Import_LCVSchedules())
        importMenu.links.append(Mod_Import_MergedLists())
        importMenu.links.append(Mod_Import_Scripts())
        importMenu.links.append(SeparatorLink())
        importMenu.links.append(File_Replace_Refs())
        ModList.itemMenu.append(importMenu)
    #--------------------------------------------
    ModList.itemMenu.append(SeparatorLink())
    ModList.itemMenu.append(Mod_ShowReadme())
    ModList.itemMenu.append(Mod_CopyToEsmp())
    ModList.itemMenu.append(Mod_RenumberRefs())
    ModList.itemMenu.append(File_RepairRefs())
    ModList.itemMenu.append(File_SortRecords())
    ModList.itemMenu.append(File_Stats())
    ModList.itemMenu.append(Mod_Updaters())
    
def InitSaveLinks():
    """Initialize save tab menus."""
    #--SaveList: Column Links
    if True: #--Sort
        sortMenu = MenuLink(_("Sort by"))
        sortMenu.links.append(Files_SortBy('File'))
        sortMenu.links.append(Files_SortBy('Cell'))
        sortMenu.links.append(Files_SortBy('Modified'))
        sortMenu.links.append(Files_SortBy('Player'))
        sortMenu.links.append(Files_SortBy('Save Name'))
        sortMenu.links.append(Files_SortBy('Status'))
        SaveList.mainMenu.append(sortMenu)
    if True: #--Save Subdirs
        subDirMenu = MenuLink(_("Profile"))
        subDirMenu.links.append(Saves_Profiles())
        SaveList.mainMenu.append(subDirMenu)
    SaveList.mainMenu.append(SeparatorLink())
    SaveList.mainMenu.append(Files_Open())
    SaveList.mainMenu.append(Files_Unhide('save'))
    SaveList.mainMenu.append(Saves_MapGridLines())
    
    #--SaveList: Item Links
    if True: #--File
        fileMenu = MenuLink(_("File")) #>>
        fileMenu.links.append(File_Backup())
        fileMenu.links.append(Save_Duplicate())
        fileMenu.links.append(File_Snapshot())
        fileMenu.links.append(SeparatorLink())
        fileMenu.links.append(File_Delete())
        fileMenu.links.append(File_MoveTo())
        fileMenu.links.append(SeparatorLink())
        fileMenu.links.append(File_RevertToBackup())
        fileMenu.links.append(File_RevertToSnapshot())
        SaveList.itemMenu.append(fileMenu)
    #--------------------------------------------
    SaveList.itemMenu.append(SeparatorLink())
    if True: #--Remove
        removeMenu = MenuLink(_("Remove"))
        removeMenu.links.append(Save_Remove_DebrisCells())
        removeMenu.links.append(Save_Remove_SpawnedCreatures())
        #--------------------------------------------
        removeMenu.links.append(SeparatorLink())
        removeMenu.links.append(File_Remove_Refs())
        SaveList.itemMenu.append(removeMenu)
    #--------------------------------------------
    SaveList.itemMenu.append(SeparatorLink())
    SaveList.itemMenu.append(Save_ShowJournal())
    SaveList.itemMenu.append(Save_LoadMasters())
    SaveList.itemMenu.append(Save_MapNotes())
    SaveList.itemMenu.append(Save_RepairAll())
    #SaveList.itemMenu.append(Save_Review()) #--Not that useful.
    SaveList.itemMenu.append(File_Stats())
    SaveList.itemMenu.append(Save_UpdateWorldMap())

def InitScreenLinks():
    """Initialize screens tab menus."""
    #--SaveList: Column Links
    ScreensList.mainMenu.append(Files_Open())
    ScreensList.mainMenu.append(SeparatorLink())
    ScreensList.mainMenu.append(Screens_NextScreenShot())

    #--ScreensList: Item Links
    ScreensList.itemMenu.append(File_Open())
    ScreensList.itemMenu.append(Screen_Rename())
    #ScreensList.itemMenu.append(File_Delete())
    ScreensList.itemMenu.append(SeparatorLink())
    ScreensList.itemMenu.append(Screen_ConvertToJpg())

def InitLinks():
    """Call other link initializers."""
    InitStatusBar()
    InitMasterLinks()
    InitInstallerLinks()
    InitModLinks()
    InitSaveLinks()
    InitScreenLinks()

# Main ------------------------------------------------------------------------
if __name__ == '__main__':
    print _('Compiled')
