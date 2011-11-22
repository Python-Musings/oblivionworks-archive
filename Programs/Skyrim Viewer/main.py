from __future__ import division

import os
import _winreg
import cStringIO
import zlib
import ConfigParser
import copy
from collections import OrderedDict

import wx
import wx.gizmos
import win32api

import util.thread
from util.path import *
import util.flags
from util.binaryfile import *
import util.ui as ui
from util.ui import vSizer, hSizer, vsbSizer
import records

from game import *

PluginExts = ('.esp','.esm')
StringsExts = ('.STRINGS','.DLSTRINGS','.ILSTRINGS')

settings = {
    'language': None,
    'load.masters': True,
    'load.strings': True,
    'ui.sortRecTypes': True,
    'skyrim': None,
    }

resources = {
    }

# For 'Type' popup menu
TypePopup = [
    #Type,RequiredSize,MenuId
    ('Goto Record',wx.NewId()),
    (None,None),
    ('CString',wx.NewId()),
    ('PString',wx.NewId()),
    ('BString',wx.NewId()),
    ('LString',wx.NewId()),
    ('LStringA',wx.NewId()),
    ('StringId',wx.NewId()),
    ('FormId',wx.NewId()),
    ('FormIdA',wx.NewId()),
    ('Float',wx.NewId()),
    ('UInt32',wx.NewId()),
    ('Int32',wx.NewId()),
    ('UInt16',wx.NewId()),
    ('Int16',wx.NewId()),
    ('UByte',wx.NewId()),
    ('Byte',wx.NewId()),
    ('Raw',wx.NewId()),
    (None,None),
    ('Custom',wx.NewId()),
    (None,None),
    ('Use Global Definition',wx.NewId()),
    ('Save as Global Definition',wx.NewId()),
    ]

def AskSkyrimPath():
    skyrimPath = GetSkyrimPath()
    if not skyrimPath:
        skyrimPath = settings['skyrim']
        if not skyrimPath:
            skyrimPath = GPath('.')
    return GPath(ui.FolderDialog(defaultDir=skyrimPath.s))

class StringTable(dict):
    def __init__(self,path=None):
        super(StringTable,self).__init__()
        self.path = None
        self.source = {}
        if path is not None:
            self.load()

    def clear(self):
        super(StringTable,self).clear()
        self.source.clear()

    def load(self,path=None,progress=ui.BaseProgress()):
        if path is None: path = self.path
        else: path = GPath(path)

        source = path.cext
        if source == '.strings':
            type = 0
        elif source.upper() in StringsExts:
            type = 1
        else:
            raise Exception("Unrecognized string table file extension.")

        source = source[1:-6].upper() + source[-6:-1].lower()
        with BinaryFile(path.s) as ins:
            ins.seek(0,os.SEEK_END)
            eof = ins.tell()
            ins.seek(0)

            num = ins.read(UInt32)
            progress.setFull(num)
            size = ins.read(UInt32)
            strings_start = eof - size

            for x in xrange(num):
                progress(x)
                id = ins.read(UInt32)
                offset = ins.read(UInt32)
                pos = ins.tell()
                ins.seek(strings_start + offset)
                if type == 0:
                    value = ins.read(CString)
                else:
                    strLen = ins.read(UInt32)
                    value = ins.read(strLen).strip('\x00')
                value = unicode(value,'cp1252')
                ins.seek(pos)
                self[id] = value
                self.source[id] = source

grup_types = {
    0: ('Tops','%s',4,False),
    1: ('World Children','World Children: Parent 0x%08X', UInt32,True),
    2: ('Interior Cell Block','Interior Cell Block: Block # %s', UInt32,False),
    3: ('Interior Cell Sub-Block','Interior Cell Sub-Block: Sub-Block # %s', UInt32,False),
    4: ('Exterior Cell Block', 'Exterior Cell Block: Block # %s', UInt32,False),
    5: ('Exterior Cell Sub-Block','Exterior Cell Sub-Block: Sub-Block # %s', UInt32,False),
    6: ('Cell Children', 'Cell Children: Parent 0x%08X', UInt32,True),
    7: ('Topic Children', 'Topic Children: Parent 0x%08X', UInt32,True),
    8: ('Cell Persistent Children','Cell Persistent Children: Parent 0x%08X', UInt32,True),
    9: ('Cell Temporary Children', 'Cell Temporary Children: Parent 0x%08X', UInt32,True),
    10:('Cell Visible Distant Children', 'Cell Visible Distant Children: Parent 0x%08X', UInt32,True),
    }

class LoadDialog(wx.Dialog):
    UNKNOWN = 'Unknown Record Types'
    def __init__(self,plugins,knownRecTypes,parent=None,title='Load'):
        wx.Dialog.__init__(self,parent,wx.ID_ANY,title,
                           style=wx.CAPTION|wx.CLOSE_BOX)
        self.SetIcons(resources['icons'])
        knownRecTypes.sort()
        knownRecTypes.append(self.UNKNOWN)
        self.popupControl = None
        # Controls
        buttonSizer = self.CreateStdDialogButtonSizer(wx.OK|wx.CANCEL)
        self.list = wx.CheckListBox(self,wx.ID_ANY,choices=[x.s for x in plugins.all])
        self.list.SetFocus()
        self.grups = wx.CheckListBox(self,wx.ID_ANY,choices=[grup_types[x][0] for x in grup_types])
        self.recs = wx.CheckListBox(self,wx.ID_ANY,choices=knownRecTypes)
        for i,plugin in enumerate(plugins.all):
            if plugin in plugins.active:
                self.list.Check(i)
            elif plugin == GPath('Update.esm'):
                self.list.Check(i)
        self.grups.Check(0)
        for i in xrange(self.recs.GetCount()):
            self.recs.Check(i)
        self.checkStrings = wx.CheckBox(self,wx.ID_ANY,'Load Strings')
        self.checkStrings.SetValue(settings['load.strings'])
        self.checkMasters = wx.CheckBox(self,wx.ID_ANY,'Load Masters')
        self.checkMasters.SetValue(settings['load.masters'])
        # Events
        self.Bind(wx.EVT_LISTBOX_DCLICK,self.OnDClick)
        self.list.Bind(wx.EVT_CONTEXT_MENU,self.OnPopup)
        self.grups.Bind(wx.EVT_CONTEXT_MENU,self.OnPopup)
        self.recs.Bind(wx.EVT_CONTEXT_MENU,self.OnPopup)
        self.Bind(wx.EVT_MENU,self.OnMenu)
        # Layout
        sizer = vSizer(
            (hSizer(
                (vSizer(
                    (wx.StaticText(self,wx.ID_ANY,'Plugins')),
                    (self.list,1,wx.GROW),
                    ),1,wx.GROW|wx.RIGHT,5),
                (vSizer(
                    (wx.StaticText(self,wx.ID_ANY,'GRUP Types')),
                    (self.grups,1,wx.GROW),
                    ),1,wx.GROW|wx.RIGHT,5),
                (vSizer(
                    (wx.StaticText(self,wx.ID_ANY,'Record Types')),
                    (self.recs,1,wx.GROW),
                    ),1,wx.GROW)
                ),1,wx.GROW|wx.ALL,5),
            (hSizer(
                (self.checkMasters),
                (self.checkStrings),
                ),0,wx.GROW|wx.ALL^wx.TOP,5),
            (buttonSizer,0,wx.GROW|wx.BOTTOM,5),
            )
        self.SetSizer(sizer)
        sizer.SetSizeHints(self)

    def OnPopup(self,event):
        self.popupControl = event.GetId()
        menu = wx.Menu()
        menu.Append(100,'Select All')
        menu.Append(101,'Deselect All')
        self.PopupMenu(menu)

    def OnMenu(self,event):
        event.Skip()
        check = event.GetId() == 100
        control = self.FindWindowById(self.popupControl)
        for i in xrange(control.GetCount()):
            control.Check(i,check)
        self.popupControl = None

    def UpdateSettings(self):
        settings['load.strings'] = self.checkStrings.GetValue()
        settings['load.masters'] = self.checkMasters.GetValue()

    def GetRecordTypes(self):
        recs = [self.recs.GetString(i)
                for i in xrange(self.recs.GetCount())
                if self.recs.IsChecked(i)]
        if recs and recs[-1] == self.UNKNOWN:
            recs[-1] = None
        return recs

    def GetPlugins(self):
        return [GPath(self.list.GetString(i))
                for i in xrange(self.list.GetCount())
                if self.list.IsChecked(i)]

    def GetGRUPTypes(self):
        return [i for i in xrange(self.grups.GetCount())
                if self.grups.IsChecked(i)]

    def OnDClick(self,event):
        event.Skip()
        control = self.FindWindowById(event.GetId())
        idex = control.GetSelection()
        if idex < 0: return
        checked = control.IsChecked(idex)
        control.Check(idex,not checked)

class CustomFormatDialog(wx.Dialog):
    def __init__(self,parent,config,title='????:????'):
        wx.Dialog.__init__(self,parent,wx.ID_ANY,title,
                           style=wx.CAPTION|wx.CLOSE_BOX)
        self.SetIcons(resources['icons'])
        self.config = config
        # Controls
        outerButtonSizer = self.CreateSeparatedButtonSizer(0)
        ok = wx.Button(self,wx.ID_OK)
        ok.SetDefault()
        self.addButton = wx.Button(self,wx.ID_ANY,'Insert Type')
        self.removeButton = wx.Button(self,wx.ID_ANY,'Remove Type')
        self.makeFlagsButton = wx.Button(self,wx.ID_ANY,'Add Flags')
        self.makeFlagsButton.Enable(False)
        self.editFlagsButton = wx.Button(self,wx.ID_ANY,'Edit Flags')
        self.editFlagsButton.Enable(False)
        self.resetButton = wx.Button(self,wx.ID_ANY,'Reset')
        self.previewButton = wx.Button(self,wx.ID_ANY,'Preview')
        sizer = hSizer(
            (self.previewButton,wx.RIGHT,5),
            (self.resetButton),
            (ui.spacer),
            (ok),
            (wx.Button(self,wx.ID_CANCEL)),
            )
        outerButtonSizer.Add(sizer,1,wx.GROW)
        self.list = wx.ListBox(self,wx.ID_ANY,style=wx.LB_SINGLE|wx.LB_HSCROLL)
        self.Reset()
        # Events
        self.addButton.Bind(wx.EVT_BUTTON,self.OnAdd)
        self.removeButton.Bind(wx.EVT_BUTTON,self.OnRemove)
        self.resetButton.Bind(wx.EVT_BUTTON,self.Reset)
        self.makeFlagsButton.Bind(wx.EVT_BUTTON,self.OnMakeFlags)
        self.editFlagsButton.Bind(wx.EVT_BUTTON,self.OnEditFlags)
        self.previewButton.Bind(wx.EVT_BUTTON,self.OnPreview)
        self.list.Bind(wx.EVT_LISTBOX,self.OnSelect)
        # Layout
        sizer = vSizer(
            (self.list,1,wx.GROW|wx.ALL,5),
            (hSizer(
                (self.addButton),
                (self.removeButton),
                (ui.spacer),
                (self.editFlagsButton),
                (self.makeFlagsButton),
                ),0,wx.GROW|wx.LEFT|wx.BOTTOM,5),
            (outerButtonSizer,0,wx.GROW|wx.ALL^wx.TOP,5),
            )
        self.SetSizer(sizer)

    def Reset(self,event=None):
        if event:
            event.Skip()
        choices = []
        for x in self.config:
            if x[0] is None: continue
            Type,Name,Format,flags = x
            if flags:
                choices.append('%s - flags: %s' % (Type, ', '.join(flags.allNames())))
            else:
                choices.append(Type)
        if choices and choices[-1] == 'raw':
            choices.pop(-1)
        choices.append('')
        self.list.Set(choices)

    def Save(self):
        del self.config[:]
        name = 0
        for Type in self.list.GetItems():
            if not Type: continue
            flags = None
            idex = Type.find('flags')
            if idex != -1:
                names = Type[idex+7:]
                Type = Type[:idex-3]
                names = [x.strip() for x in names.split(', ')]
                flags = util.flags.Flags(0,*names)
            Unpacker,ReqSize,Eval,Format = records.Types[Type]
            self.config.append((str(Type),'%s'%name,Format,flags))
            name += 1
        if not self.config:
            self.config.append(('raw','',None,None))

    def OnSelect(self,event=None):
        if event: event.Skip()
        idex = self.list.GetSelection()
        enableMake = False
        enableEdit = False
        label = 'Add Flags'
        if idex >= 0:
            text = self.list.GetString(idex)
            for prefix in ('ubyte','uint16','uint32','uint64'):
                if text.startswith(prefix):
                    enableMake = True
                    break
            if 'flags' in text:
                label = 'Remove Flags'
                enableEdit = True
        self.makeFlagsButton.SetLabel(label)
        self.makeFlagsButton.Enable(enableMake)
        self.editFlagsButton.Enable(enableEdit)

    def OnEditFlags(self,event=None):
        if event: event.Skip()
        idex = self.list.GetSelection()
        text = self.list.GetString(idex)
        for i,prefix in enumerate(('ubyte','uint16','uint32','uint64')):
            if text.startswith(prefix):
                num = (i+1)*8
                prefixLen = len(prefix)
                if len(text) > prefixLen:
                    names = [x.strip() for x in text[prefixLen+10:].split(',')]
                else:
                    names = ['unk%i'%x for x in xrange(num)]
                dialog = wx.Dialog(self,wx.ID_ANY,'Flags',style=(wx.CLOSE_BOX|
                            wx.CAPTION))
                dialog.SetIcons(resources['icons'])
                list = ui.ListCtrl(dialog,wx.ID_ANY,style=(wx.LC_REPORT|
                            wx.LC_NO_HEADER|wx.LC_EDIT_LABELS|
                            wx.LC_SINGLE_SEL|wx.LC_HRULES|wx.LC_VRULES))

                def _EndEdit(event):
                    final = event.GetText()
                    if ',' in final:
                        event.Veto()
                    else:
                        event.Skip()
                list.Bind(wx.EVT_LIST_END_LABEL_EDIT, _EndEdit)
                list.InsertColumn(0,'')
                list.InsertColumn(1,'')
                format = '0x%%0%iX' % (2*(i+1))
                for x,name in enumerate(names):
                    list.InsertStringItem(x,format % (1<<x))
                    list.SetStringItem(x,1,name)
                list.SetColumnEditable(0,False)
                list.SetColumnWidth(0,wx.LIST_AUTOSIZE)
                list.resizeLastColumn(20)
                buttonSizer = dialog.CreateSeparatedButtonSizer(wx.OK|wx.CANCEL)
                sizer = vSizer(
                    (list,1,wx.GROW|wx.ALL^wx.BOTTOM,5),
                    (buttonSizer,0,wx.GROW|wx.ALL,5),
                    )
                dialog.SetSizer(sizer)
                ret = dialog.ShowModal()
                if ret == wx.ID_CANCEL:
                    dialog.Destroy()
                    return
                names = [list.GetItemText(x,1) for x in xrange(num)]
                dialog.Destroy()
                text = '%s - flags: %s' % (prefix, ', '.join(names))
                self.list.SetString(idex,text)

    def OnMakeFlags(self,event):
        label = self.makeFlagsButton.GetLabel()
        if label[0] == 'A': # Add Flags
            self.OnEditFlags(event)
            self.OnSelect(event)
        else:
            # Remove flags
            idex = self.list.GetSelection()
            text = self.list.GetString(idex)
            self.list.SetString(idex,text.split(' ')[0])
            self.makeFlagsButton.SetLabel('Add Flags')
        event.Skip()

    def OnAdd(self,event=None):
        if event: event.Skip()
        menu = wx.Menu()
        for Type,Id in TypePopup:
            if not Type or Type in ('Goto Record','Raw','Custom','Save as Global Definition','Use Global Definition'):
                continue
            menu.AppendCheckItem(Id,Type)
        self.Bind(wx.EVT_MENU,self.OnMenu)
        self.PopupMenu(menu)

    def OnMenu(self,event=None):
        id = event.GetId()
        for Type,Id in TypePopup:
            if Id == id:
                idex = self.list.GetSelection()
                if idex < 0: idex = self.list.GetCount() - 1
                self.list.InsertItems([Type.lower()],idex)
                self.OnSelect()
                return

    def OnRemove(self,event):
        event.Skip()
        idex = self.list.GetSelection()
        if idex >= 0:
            if idex < self.list.GetCount() - 1:
                self.list.Delete(idex)
                if idex < self.list.GetCount() - 1:
                    self.list.SetSelection(idex)
                else:
                    self.list.SetSelection(idex-1)
                self.OnSelect()

    def OnPreview(self,event):
        parent = self.GetParent()
        backupConfig =  self.config[:]
        self.Save()
        parent.OnTree()
        del self.config[:]
        self.config.extend(backupConfig)

# List modes
NONE = 0
RECORD = 1
STRING = 2

class MyFrame(wx.Frame):
    settingsMenu = [
        ('Skyrim Path...', None, wx.NewId(), 'OnAskSkyrimPath'),
        (),
        ('Load Masters','load.masters',wx.NewId(),'SwapSetting'),
        ('Load Strings','load.strings',wx.NewId(),'SwapSetting'),
        (),
        ('Sort Record Types','ui.sortRecTypes',wx.NewId(),'SwapSorting'),
        (),
        ('Remove Global Definition Overrides',None,wx.NewId(),'OnEnforceGlobal'),
        ]

    toolbar = [
        ('loadButton',wx.ART_FILE_OPEN,'&Open','OnOpen',True,(wx.ACCEL_CTRL,ord('O'),'Ctrl+O')),
        (),
        ('loadDefsButton',wx.ART_UNDO,'Reload Record Definitions','LoadRecordDefs',True,None),
        ('saveButton',wx.ART_FILE_SAVE,'&Save Record Definitions','SaveRecordDefs',False,(wx.ACCEL_CTRL, ord('S'),'Ctrl+S')),
        (),
        ('loadStringsButton',wx.ART_FOLDER_OPEN,'Reload Strings','OnLoadStrings',False,None),
        ]

    def SwapSetting(self,event):
        id = event.GetId()
        event.Skip()
        for item in self.settingsMenu:
            if item and id == item[2]:
                key = item[1]
                settings[key] ^= True
                break

    def SwapSorting(self,event):
        event.Skip()
        settings['ui.sortRecTypes'] ^= True
        sort = settings['ui.sortRecTypes']
        for file in self.data:
            data = self.data[file]
            root = data['ui']['root']
            hedr = data['ui']['header']
            strg = data['ui']['strings']
            if hedr:
                self.tree.SetItemText(hedr,'000000000')
            if strg:
                self.tree.SetItemText(strg,'000000001')
            if sort:
                # Sort alphabetically
                self.tree.SortChildren(root)
            else:
                # Sort by order in mod
                recOrder = data['ui'].get('recOrder')
                child,cookie = self.tree.GetFirstChild(root)
                while child.IsOk():
                    if child != hedr and child != strg:
                        recType = self.tree.GetItemText(child)
                        i = recOrder.index(recType) + 2
                        self.tree.SetItemText(child,'%09X' % i)
                    child = self.tree.GetNextSibling(child)
                self.tree.SortChildren(root)
                child,cookie = self.tree.GetFirstChild(root)
                while child.IsOk():
                    i = self.tree.GetItemText(child)
                    i = int(i,16)
                    if i != 1 or i != 2:
                        i -= 2
                        recType = recOrder[i]
                        self.tree.SetItemText(child,recType)
                    child = self.tree.GetNextSibling(child)
            if hedr:
                self.tree.SetItemText(hedr,'Header')
            if strg:
                self.tree.SetItemText(strg,'Strings')

    def __init__(self):
        wx.Frame.__init__(self, None, wx.ID_ANY, 'Skyrim Viewer',size=(800,600))
        if resources['icons']: self.SetIcons(resources['icons'])
        self.listMode = NONE
        self.workers = {}
        self.loadedPlugins = []

        self.plugins = Plugins()
        self.data = {}

        # Setup the Toolbar ----------------------------------------------------
        self.tb = tb = self.CreateToolBar()

        for tool in self.toolbar:
            if len(tool) == 0:
                tb.AddSeparator()
            else:
                attr,artId,tip,fn,enable,accel = tool
                id = wx.NewId()
                setattr(self,attr,id)
                tool = tb.AddSimpleTool(
                    id,
                    wx.ArtProvider.GetBitmap(artId,wx.ART_TOOLBAR,(16,16)),
                    tip)
                if fn:
                    self.Bind(wx.EVT_TOOL,getattr(self,fn),tool)
                tb.EnableTool(id,enable)
        tb.Realize()

        # Setup the MenuBar ----------------------------------------------------
        menuBar = wx.MenuBar()
        aTable = []
        #--File
        menu = wx.Menu()
        for tool in self.toolbar:
            if len(tool) == 0:
                menu.AppendSeparator()
            else:
                attr,artId,tip,fn,enable,accel = tool
                id = getattr(self,attr)
                if accel:
                    tip = '%s\t%s' % (tip,accel[2])
                    accel = (accel[0],accel[1],id)
                    aTable.append(accel)
                menuItem = wx.MenuItem(menu,id,tip)
                menuItem.SetBitmap(wx.ArtProvider.GetBitmap(artId,wx.ART_MENU,(16,16)))
                menu.AppendItem(menuItem)
                menuItem.Enable(enable)
        menu.AppendSeparator()
        menu.Append(wx.ID_EXIT,'&Quit')
        self.Bind(wx.EVT_MENU,self.OnQuit,id=wx.ID_EXIT)
        menuBar.Append(menu,'&File')
        #--Settings
        menu = wx.Menu()
        for tool in self.settingsMenu:
            if len(tool) == 0:
                menu.AppendSeparator()
            else:
                name,key,id,fn = tool
                if not key:
                    item = wx.MenuItem(menu,id,name)
                    menu.AppendItem(item)
                else:
                    item = menu.AppendCheckItem(id,name)
                    item.Check(settings[key])
                if  fn:
                    self.Bind(wx.EVT_MENU,getattr(self,fn),id=id)
        menuBar.Append(menu,'&Settings')
        aTable = wx.AcceleratorTable(aTable)
        self.SetAcceleratorTable(aTable)
        self.SetMenuBar(menuBar)

        # Load record definitions ----------------------------------------------
        self.LoadRecordDefs()

        # Setup the Statusbar --------------------------------------------------
        self.sb = ui.StatusBar(self)
        self.SetStatusBar(self.sb)
        self.sb.SetFieldsCount(4)
        self.sb.SetStatusWidths([-1,15,-2,-2])
        self.sb.SetStatusStyles([wx.SB_FLAT,wx.SB_NORMAL,wx.SB_FLAT,wx.SB_NORMAL])
        self.SetStatusBarPane(3)
        self.guage = wx.Gauge(self.sb,wx.ID_ANY,100)
        self.cancelButton = wx.BitmapButton(self.sb,wx.ID_ANY,
            wx.BitmapFromIcon(wx.Icon(resources['stop_ico'],wx.BITMAP_TYPE_ICO,16,16)))
        self.cancelButton.Enable(False)
        self.sb.SetControl(0,self.guage,wx.ALL,2)
        self.sb.SetControl(1,self.cancelButton,wx.ALL,1)
        self.progress = ui.Progress(self.guage,self.sb,2)
        

        # Setup controls -------------------------------------------------------
        panel = wx.Panel(self)
        splitter = wx.SplitterWindow(panel,style=wx.SP_LIVE_UPDATE)
        left = wx.Panel(splitter)
        right = wx.Panel(splitter)
        splitter.SplitVertically(left,right)
        splitter.SetSashPosition(250)

        #-Search Boxes
        self.searchMods = wx.SearchCtrl(left,wx.ID_ANY,
                            style=wx.TE_PROCESS_ENTER)
        self.searchMods.SetToolTip(wx.ToolTip('Search for records'))
        self.searchMods.Enable(False)

        self.searchRecord = wx.SearchCtrl(right,wx.ID_ANY,
                            style=wx.TE_PROCESS_ENTER)
        self.searchRecord.SetToolTip(wx.ToolTip('Search this record'))
        self.searchRecord.Enable(False)

        #-Tree
        self.tree = ui.TreeListCtrl(left,
            style=(wx.TR_EDIT_LABELS|wx.TR_HAS_BUTTONS|wx.TR_ROW_LINES|
                   wx.TR_SINGLE|wx.TR_HIDE_ROOT|wx.TR_FULL_ROW_HIGHLIGHT))
        self.tree.AddColumn('Plugin')
        self.tree.AddColumn('Editor Id')
        self.tree.SetColumnWidth(0,160)
        self.tree.resizeLastColumn(20)
        self.tree.AddRoot('ROOT')

        #-List
        self.list = ui.ListCtrl(right,wx.ID_ANY,
            style=(wx.LC_REPORT|wx.LC_SINGLE_SEL|wx.LC_EDIT_LABELS
                   |wx.LC_HRULES|wx.LC_VRULES))

        # Setup events --------------------------------------------------------
        self.tree.Bind(wx.EVT_TREE_SEL_CHANGED,self.OnTree)
        self.tree.Bind(wx.EVT_LIST_COL_BEGIN_DRAG,self.OnDragTreeColumn)
        self.tree.Bind(wx.EVT_TREE_ITEM_RIGHT_CLICK,self.OnTreeContext)
        self.cancelButton.Bind(wx.EVT_BUTTON, self.OnCancel)
        self.list.Bind(wx.EVT_LIST_ITEM_RIGHT_CLICK,self.OnListContext)
        self.list.Bind(wx.EVT_LIST_BEGIN_LABEL_EDIT,self.OnListEditBegin)
        self.list.Bind(wx.EVT_LIST_END_LABEL_EDIT,self.OnListEditEnd)
        self.searchMods.Bind(wx.EVT_SEARCHCTRL_SEARCH_BTN,self.OnSearchMods)
        self.searchMods.Bind(wx.EVT_TEXT_ENTER,self.OnSearchMods)
        self.searchRecord.Bind(wx.EVT_SEARCHCTRL_SEARCH_BTN,self.OnSearchRecord)
        self.searchRecord.Bind(wx.EVT_TEXT_ENTER,self.OnSearchRecord)
        self.Bind(wx.EVT_CLOSE,self.OnClose)

        # Setup the layout ----------------------------------------------------
        sizer = vSizer(
            (self.searchMods,0,wx.GROW|wx.BOTTOM,5),
            (self.tree,1,wx.GROW),
            )
        left.SetSizer(sizer)

        sizer = vSizer(
            (self.searchRecord,0,wx.GROW|wx.BOTTOM,5),
            (self.list,1,wx.GROW),
            )
        right.SetSizer(sizer)

        sizer = vSizer(
            (splitter,1,wx.GROW|wx.ALL,5),
            )
        panel.SetSizer(sizer)
        # ----------------------------------------------------------------------

    def OnAskSkyrimPath(self,event):
        event.Skip()
        skyrimPath = AskSkyrimPath()
        if not skyrimPath:
            return
        settings['skyrim'] = skyrimPath

    def OnEnforceGlobal(self,event):
        globalTypes = records.RecordDef.globalConfig.keys()
        changed = False
        for recType in self.record_configs:
            config = self.record_configs[recType]
            for subType in globalTypes:
                if subType in config:
                    del config[subType]
                    changed = True
        if changed:
            self.SetRecordDefsChanged()

    def OnDragTreeColumn(self,event):
        if 'load' in self.workers:
            event.Veto()
        else:
            event.Skip()

    def OnTreeContext(self,event):
        item = event.GetItem()
        for file in self.data:
            data = self.data[file]
            if item == data['ui']['root'] or item == data['ui']['strings']:
                menu = wx.Menu()
                menuItem = wx.MenuItem(menu,wx.ID_ANY,'Add a String')
                menu.AppendItem(menuItem)
                self.Bind(wx.EVT_MENU,self.OnAddString,menuItem)
                self.tree.SelectItem(item)
                self.tree.PopupMenu(menu)
        event.Skip()

    def OnAddString(self,event):
        class hexValidator(wx.PyValidator):
            def __init__(self):
                super(hexValidator,self).__init__()
                self.Bind(wx.EVT_TEXT,self.OnText)

            def Clone(self): return hexValidator()
            def TransferToWindow(self): return True
            def TransferFromWindow(self): return True

            def OnText(self,event):
                value = event.GetString()
                if value:
                    try:
                        int(value,16)
                        self.GetWindow().SetBackgroundColour('lightgreen')
                        self.GetWindow().Refresh()
                    except:
                        self.GetWindow().SetBackgroundColour('pink')
                        self.GetWindow().Refresh()
                event.Skip()

            def Validate(self,parent):
                value = self.GetWindow().GetValue()
                try:
                    value = int(value,16)
                    return True
                except:
                    return False

        dialog = wx.Dialog(self,wx.ID_ANY,'lala')
        button = wx.Button(dialog,wx.ID_OK)
        textCtrl = wx.TextCtrl(dialog,validator=hexValidator())
        sizer = hSizer(
            (textCtrl,1,wx.GROW|wx.ALL,5),
            (button,0,wx.ALL^wx.TOP,5),
            )
        dialog.SetSizer(sizer)
        dialog.ShowModal()
        dialog.Destroy()

    def OnClose(self,event):
        if self.tb.GetToolEnabled(self.saveButton):
            dialog = wx.MessageDialog(self,
                'You have changed some of the record definitions.  Would you like to save them now?',
                'Record Definitions',
                style=wx.YES_NO|wx.YES_DEFAULT|wx.CANCEL)
            ret = dialog.ShowModal()
            if ret == wx.ID_CANCEL:
                event.Veto()
                return
            elif ret == wx.ID_YES:
                self.SaveRecordDefs()
        event.Skip()

    def FindItem(self,search):
        item = self.tree.GetSelection()
        if not item.IsOk():
            item = self.tree.GetRootItem()
        try:
            search = int(search,16)
            search = '%08X' % search
            item = self.tree.FindItem(item,search)
            if not item.IsOk():
                item = self.tree.FindItem(self.tree.GetRootItem(),search)
            return item
        except:
            search = search.lower()
            itemStart = item
            badItem = None
            while True:
                item = self.tree.GetNext(item)
                if not item.IsOk():
                    badItem = item
                    item = self.tree.GetRootItem()
                if item == itemStart:
                    return badItem
                edid = self.tree.GetItemText(item,1)
                if search in edid.lower():
                    return item

    def OnSearchMods(self,event):
        event.Skip()
        search = self.searchMods.GetValue().strip()
        if search:
            item = self.FindItem(search)
            if item.IsOk():
                self.tree.EnsureVisible(item)
                self.tree.SelectItem(item)

    def OnSearchRecord(self,event):
        # Only search Strings right now
        event.Skip()
        search = self.searchRecord.GetValue().strip()
        if search:
            start = -1
            for i in xrange(self.list.GetItemCount()):
                if self.list.GetItemState(i,wx.LIST_STATE_SELECTED):
                    start = i
                    break
            found = None
            for i in xrange(start+1,self.list.GetItemCount()):
                item = self.list.GetItem(i,2)
                if search in item.GetText().lower():
                    found = i
                    break
            if found is None:
                for i in xrange(0,start):
                    item = self.list.GetItem(i,2)
                    if search in item.GetText().lower():
                        found = i
                        break
            if found is not None:
                self.list.SetItemState(start,0,wx.LIST_STATE_SELECTED)
                self.list.SetItemState(i,wx.LIST_STATE_SELECTED,wx.LIST_STATE_SELECTED)
                self.list.EnsureVisible(i)

    def SetRecordDefsChanged(self,changed=True):
        self.GetMenuBar().Enable(self.saveButton,changed)
        self.tb.EnableTool(self.saveButton,changed)

    def SaveRecordDefs(self,event=None):
        if event:
            event.Skip()
        for recType in self.record_configs:
            config = self.record_configs[recType]
            file = GPath('records').join(recType+'.ini')
            with file.open('w') as out:
                config.save(out)
        file = GPath('records').join('ALL.ini')
        with file.open('w') as out:
            records.RecordDef.globalConfig.save(out)
        self.SetRecordDefsChanged(False)

    def LoadRecordDefs(self,event=None):
        if event:
            event.Skip()
        self.record_configs = records.LoadRecordDefs('records\\')
        if hasattr(self,'tree'):
            self.OnTree()

    def OnQuit(self,event):
        self.Close(True)

    def OnLoadStrings(self,event):
        event.Skip()
        files = [x for x in self.data if self.data[x]['ui']['header'] is not None]
        self.LoadStrings(files,self.progress)
        self.OnTree()
        self.progress(0,' ')

    def LoadStrings(self,plugins,prgs=ui.BaseProgress):
        prgs.setFull(len(plugins))
        for i,file in enumerate(plugins):
            prgs(i,'%s: Loading strings...' % file.s)
            dir = settings['skyrim'].join('Data','Strings')
            strings_files = [dir.join('%s_%s%s' % (file.sbody,settings['language'],x)) for x in StringsExts]
            strings_files = [x for x in strings_files if x.exists and not x.isdir]
            if not strings_files: continue
            subprogress2 = ui.SubProgress(prgs,i)
            subprogress2.setFull(len(strings_files))
            table = self.data[file]['strings']
            table.clear()
            for j,string_file in enumerate(strings_files):
                subprogress2(j)
                subprogress3 = ui.SubProgress(subprogress2,j)
                table.load(string_file,subprogress3)
            item = self.data[file]['ui']['strings']
            if item is None:
                item = self.tree.InsertItem(self.data[file]['ui']['root'],
                                            self.data[file]['ui']['header'],
                                            'Strings')
                self.data[file]['ui']['strings'] = item
            # Update 'Strings' item
            self.tree.DeleteChildren(item)
            allIds = sorted(table.keys())
            while len(allIds) > 200:
                startId = allIds[0]
                stopId = allIds[199]
                del allIds[0:199]
                self.tree.AppendItem(item,'%08X - %08X' % (startId,stopId))
            if allIds:
                self.tree.AppendItem(item,'%08X - %08X' % (allIds[0],allIds[-1]))

    def ClearUI(self):
        self.listMode = NONE
        self.tree.DeleteChildren(self.tree.GetRootItem())
        self.list.DeleteAllItems()
        self.list.DeleteAllColumns()
        self.tb.EnableTool(self.loadStringsButton,False)
        self.GetMenuBar().Enable(self.loadStringsButton,False)
        self.searchMods.Enable(False)
        self.searchRecord.Enable(False)

    def SetListStyle(self,style):
        if style == self.listMode: return
        self.listMode = style
        self.list.ClearAll()
        if style == RECORD:
            self.searchRecord.Enable(False)
            self.list.InsertColumn(0,'Field')
            self.list.InsertColumn(1,'Name')
            self.list.InsertColumn(2,'Value')
            self.list.SetColumnEditable(0,False)
            self.list.SetColumnEditable(1,True)
            self.list.SetColumnEditable(2,False)
        elif style == STRING:
            self.searchRecord.Enable(True)
            self.list.InsertColumn(0,'Id')
            self.list.InsertColumn(1,'Source')
            self.list.InsertColumn(2,'Value')
            self.list.SetColumnEditable(0,False)
            self.list.SetColumnEditable(1,False)
            self.list.SetColumnEditable(2,True)
        self.list.resizeLastColumn(20)

    def SetListCount(self,count):
        cur = self.list.GetItemCount()
        if cur > count:
            for i in xrange(cur-count):
                self.list.DeleteItem(0)
        elif cur < count:
            for i in xrange(count-cur):
                self.list.InsertStringItem(0,'')

    def GetTreeSelection(self):
        item = self.tree.GetSelection()
        selection = []
        while True:
            if not item.IsOk(): break
            selection.insert(0,item)
            item = self.tree.GetItemParent(item)
        if len(selection) < 3: return None
        selection.pop(0)
        return selection

    def GetInfoFromListIdex(self,idex):
        selection = self.GetTreeSelection();
        if not selection:
            return None
        plugin = GPath(self.tree.GetItemText(selection.pop(0))[5:])
        if selection[0] == self.data[plugin]['ui']['strings']:
            return False
        elif selection[-1] == self.data[plugin]['ui']['header']:
            formId = 'TES4'
        else:
            formId = self.tree.GetItemText(selection[-1])
            try:
                formId = int(formId,16)
            except:
                return None
        formData = self.data[plugin][formId]
        recType = formData[0]
        subType = ''
        i = idex
        while not subType:
            subType = self.list.GetItemText(i,0)
            i -= 1
        defIdex = idex - i - 1
        config = self.record_configs.setdefault(recType,records.RecordDef()).get(subType)
        if not config:
            config = records.RecordDef.globalConfig.get(subType)
            if not config:
                config = self.record_configs[recType].setdefault(subType,[(None,'',None,None)])
        defDelta = defIdex - (len(config) - 1)
        if defDelta >= 0:
            defIdex -= defDelta
        return plugin,config,recType,formId,subType,defIdex,defDelta

    def OnListEditBegin(self,event):
        idex = event.GetIndex()
        info = self.GetInfoFromListIdex(idex)
        if info is None:
            event.Veto()
            return
        elif not info:
            event.Skip()
            return
        plugin,config,recType,formId,subType,defIdex,defDelta = info
        Type,Name,Format,flags = config[defIdex]
        Unpacker,ReqSize,Eval,DefFormat = records.Types.get(Type,(None,None,None,None))
        if isinstance(Unpacker,records.TypeArray):
            if defDelta != 0:
                event.Veto()
                return
        event.Skip()

    def OnListEditEnd(self,event):
        idex = event.GetIndex()
        col = event.GetColumn()
        newValue = event.GetText().strip()
        oldValue = self.list.GetItemText(idex,col)
        if newValue == oldValue:
            event.Skip()
            return
        # Update config
        info = self.GetInfoFromListIdex(idex)
        if not info:
            event.Veto()
            return
        plugin,config,recType,formId,subType,defIdex,defDelta = info

        Type,Name,Format,flags = config[defIdex]
        config[defIdex] = (Type,newValue,Format,flags)

        self.SetRecordDefsChanged()

    def OnListContext(self,event):
        event.Skip()
        idex = event.GetIndex()

        info = self.GetInfoFromListIdex(idex)
        if not info:
            return

        plugin,config,recType,formId,subType,defIdex,defDelta = info

        formData = self.data[plugin][formId]
        for data in formData:
            if data[0] == subType:
                subData = data[1]
                break
        else:
            return
        subData.seek(0,os.SEEK_END)
        subSize = subData.tell()
        subData.seek(0)

        try:
            unpacker_name,name,format,flags = config[defIdex]
        except:
            print config
            print defIdex
            print config[idex]
            raise
        if unpacker_name in records.Types:
            unpacker = records.Types[unpacker_name][0]
        else:
            unpacker = records.Types['raw'][0]

        def OnMenu(event):
            id = event.GetId()
            for Type,Id in TypePopup:
                if Id == id:
                    if Type == 'Custom':
                        self.CustomFormatDialog(config,'%s:%s (%s bytes)'%(recType,subType,subSize))
                    elif Type == 'Goto Record':
                        formId = bfRead(subData,UInt32)
                        subData.seek(0)
                        shortId,longId,recordId = records.FormIdTable.ShortToLongFid(formId)
                        _recType,edid,masterIdex,master = records.FormIdTable.FindWinningRecord(shortId,longId,recordId)
                        if _recType:
                            item = self.data[master]['ui']['root']
                            item = self.tree.FindItem(item,'%08X'%formId)
                            if item.IsOk:
                                self.tree.EnsureVisible(item)
                                self.tree.SelectItem(item)
                    elif Type == 'Use Global Definition':
                        if subType in self.record_configs[recType]:
                            del self.record_configs[recType][subType]
                        else:
                            self.record_configs[recType][subType] = copy.copy(records.RecordDef.globalConfig[subType])
                        self.SetRecordDefsChanged()
                        self.OnTree()
                    elif Type == 'Save as Global Definition':
                        records.RecordDef.globalConfig[subType] = self.record_configs[recType][subType]
                        del self.record_configs[recType][subType]
                        self.SetRecordDefsChanged()
                        self.OnTree()
                    else:
                        Unpacker,ReqSize,Eval,Format = records.Types[Type.lower()]
                        self.record_configs.setdefault(recType,records.RecordDef())
                        self.record_configs[recType].setdefault(subType,[()])
                        self.record_configs[recType][subType][defIdex] = (Type.lower(),name,format,None)
                        self.SetRecordDefsChanged()
                        pos = self.list.GetScrollPos(wx.VERTICAL)
                        self.OnTree()
                        self.list.EnsureVisible(idex)
                        pos2 = self.list.GetScrollPos(wx.VERTICAL)
                        self.list.ScrollLines(pos-pos2)
                    return

        # Build menu
        menu = wx.Menu()
        isCustom = len(config) > 1
        records.StringTable.SetStrings(self.data[plugin]['strings'])
        for Type,Id in TypePopup:
            if Id is None:
                menu.AppendSeparator()
            else:
                if Type == 'Custom':
                    menu.AppendCheckItem(Id,Type)
                    menu.Check(Id,isCustom)
                elif Type == 'Goto Record':
                    added = False
                    if subSize == 4:
                        formId = bfRead(subData,UInt32)
                        subData.seek(0)
                        shortId,longId,recordId = records.FormIdTable.ShortToLongFid(formId)
                        if longId is not None:
                            _recType,edid,masterIdex,master = records.FormIdTable.FindWinningRecord(shortId,longId,recordId)
                            if _recType:
                                added = True
                                menu.AppendCheckItem(Id,"Goto [%02X%06X] in '[%02X] %s'" % (longId,recordId,masterIdex,master.s))
                    if not added:
                        menu.AppendCheckItem(Id,Type)
                        menu.Enable(Id,False)
                elif Type == 'Use Global Definition':
                    notUsingGlobal = subType in self.record_configs.get(recType,{})
                    globalAvail = subType in records.RecordDef.globalConfig
                    menu.AppendCheckItem(Id,Type)
                    menu.Check(Id,not notUsingGlobal)
                    menu.Enable(Id,globalAvail)
                elif Type == 'Save as Global Definition':
                    item = wx.MenuItem(menu,Id,Type)
                    notUsingGlobal = subType in self.record_configs.get(recType,{})
                    menu.AppendItem(item)
                    item.Enable(notUsingGlobal)
                else:
                    Unpacker,ReqSize,Eval,Format = records.Types[Type.lower()]
                    usable = (ReqSize == subSize or ReqSize is None)
                    if usable:
                        try:
                            value = bfRead(subData,Unpacker)
                            if Eval: value = Eval(value)
                            if Format: value = Format % value
                            label = '%s - %s' % (Type,value)
                            extent = self.GetTextExtent(label)
                            a = 0
                            while extent[0] > 600:
                                a -= 10
                                extent = self.GetTextExtent(label[:a])
                            if a < 0: label = label[:a] + '...'
                        except:
                            label = Type
                        finally:
                            subData.seek(0)
                    else:
                        label = Type
                    try:
                        menu.AppendCheckItem(Id,label)
                    except UnicodeDecodeError:
                        menu.AppendCheckItem(Id,Type)
                    menu.Check(Id,Type.lower()==unpacker_name or (Unpacker is None and Type=='Raw'))
                    menu.Enable(Id,usable)
                self.Bind(wx.EVT_MENU,OnMenu,id=Id)

        self.PopupMenu(menu)
        records.StringTable.SetStrings()

    def CustomFormatDialog(self,config,title):
        dialog = CustomFormatDialog(self,config,title)
        if dialog.ShowModal() == wx.ID_OK:
            dialog.Save()
            self.SetRecordDefsChanged()
        self.OnTree()
        dialog.Destroy()

    def OnTree(self,event=None):
        if event:
            event.Skip()
        item = self.tree.GetSelection()
        selection = self.GetTreeSelection()
        if not selection: return
        plugin = GPath(self.tree.GetItemText(selection.pop(0))[5:])
        data = self.data[plugin]
        tops = selection.pop(0)
        if tops == data['ui']['strings']:
            if not selection:
                self.SetListStyle(NONE)
                return
            stringIdRange = self.tree.GetItemText(selection[-1])
            stringIdFrom = stringIdRange[:8]
            stringIdTo = stringIdRange[-8:]
            try:
                stringIdFrom = int(stringIdFrom,16)
                stringIdTo = int(stringIdTo,16)
            except:
                self.SetListStyle(NONE)
                return
            self.SetListStyle(STRING)
            data = data['strings']
            allIds = sorted(data.keys())
            startIdex = allIds.index(stringIdFrom)
            stopIdex = allIds.index(stringIdTo)
            self.SetListCount(stopIdex-startIdex+1)
            for idex in xrange(startIdex,stopIdex+1):
                id = allIds[idex]
                source = data.source[id]
                value = data[id]
                self.list.SetStringItem(idex-startIdex,0,'%08X' % id)
                self.list.SetStringItem(idex-startIdex,1,source)
                self.list.SetStringItem(idex-startIdex,2,value)
        else:
            if item == data['ui']['header']:
                formId = 'TES4'
            elif not selection:
                self.SetListStyle(NONE)
                return
            else:
                formId = self.tree.GetItemText(selection[-1])
                try:
                    formId = int(formId,16)
                except:
                    self.SetListStyle(NONE)
                    return
            records.FormIdTable.SetPlugin(plugin)
            self.SetListStyle(RECORD)
            self.list.DeleteAllItems()
            data = data.get(formId,{})
            if isinstance(data,str):
                self.list.InsertStringItem(0,data)
                return
            idex = 0
            recType = data[0]
            config = self.record_configs.get(recType,records.RecordDef())
            config.setStrings(self.data[plugin]['strings'])
            for subType,subData in data[1:]:
                unpacked = config.unpackSubrecord(subType,subData)
                lastSubType = ''
                for value,Name,Format in unpacked:
                    if not isinstance(value,list):
                        value = [value]
                    firstValue = True
                    for v in value:
                        if subType == lastSubType:
                            subType = ''
                        else:
                            lastSubType = subType

                        self.list.InsertStringItem(idex,subType)
                        if firstValue:
                            self.list.SetStringItem(idex,1,Name)
                            firstValue = False
                        if Format is not None:
                            self.list.SetStringItem(idex,2,Format % v)
                        elif not isinstance(v,(str,unicode)):
                            self.list.SetStringItem(idex,2,'%s' % v)
                        else:
                            self.list.SetStringItem(idex,2,v)
                        idex += 1
            config.setStrings()
        return

    def OnOpen(self,event):
        """Pick plugins to load"""
        event.Skip()
        self.plugins.refresh(settings['skyrim'].join('Data'))
        dialog = LoadDialog(self.plugins,self.record_configs.keys())
        ret = dialog.ShowModal()
        if ret == wx.ID_CANCEL:
            dialog.Destroy()
            return
        dialog.UpdateSettings()
        load = dialog.GetPlugins()
        grups = dialog.GetGRUPTypes()
        recs = dialog.GetRecordTypes()
        dialog.Destroy()
        self.ClearUI()
        if load:
            self.Load(load,grups,recs)
            self.tree.SetFocus()

    def OnCancel(self,event):
        if 'load' in self.workers:
            self.sb.SetStatusText('Cancelling',2)
            self.workers['load'].kill()

    def Load(self,files,grups,recs):
        """Start a thread to do the work async"""
        if 'load' not in self.workers:
            th = util.thread.KThread(
                target=self._Load,
                args=(files,grups,recs),)
            self.workers['load'] = th
            th.daemon = True
            th.start()
            self.tb.EnableTool(self.loadButton,False)
            self.cancelButton.Enable(True)

    def _Load(self,files,grups,recs):
        """Runs in a seperate thread.  A ThreadExit exception will be raised
           if the thread is requested termination."""
        try:
            dataPath = settings['skyrim'].join('Data')
            self.data.clear()
            self.ClearUI()

            self.progress.setFull(100)

            # Step 1: load files, determine if any masters are missing/required
            # ~5% of progress?
            subprogress = ui.SubProgress(self.progress,0,5)
            subprogress.setFull(len(files))

            def parseRecord(ins,root,data,prgs=ui.BaseProgress):
                (recType,recSize,flags,
                 formId,versionControl,unk) = bfUnpack(ins,'4s 5I')
                # Save data for display
                ins.seek(-24,os.SEEK_CUR)
                _header = cStringIO.StringIO(ins.read(24))

                eor = ins.tell() + recSize
                if recType not in recs:
                    if (recType in self.record_configs.keys() or
                        None not in recs) and recType != 'TES4':
                        ins.seek(eor)
                        return
                if root:
                    shortId,longId,recordId = records.FormIdTable.ShortToLongFid(formId)
                    if longId is None:
                        # missing master
                        item = self.tree.AppendItem(root,'%02X%06X' % (shortId,recordId))
                        self.tree.SetItemBackgroundColour(item,'pink')
                    else:
                        item = self.tree.AppendItem(root,'%02X%06X' % (longId,recordId))
                compressed = bool(flags & 0x40000)
                if compressed:
                    size = bfRead(ins,UInt32)
                    try:
                        recData = zlib.decompress(ins.read(recSize-4))
                        if len(recData) != size:
                            data[formId] = [recType,'ERROR - decompressed record size does not match expected']
                            ins.seek(eor)
                            return
                    except Exception as e:
                        data[formId] = [recType,'ERROR - an exception occured: %s' % e]
                        ins.seek(eor)
                        return
                    recSize = size
                else:
                    recData = ins.read(recSize)
                recData = cStringIO.StringIO(recData)

                data[formId] = [recType,('Record Header',_header)]
                formData = data[formId]
                edid = ''
                while recData.tell() < recSize:
                    subType = recData.read(4)
                    subSize = bfRead(recData,UInt16)
                    subData = cStringIO.StringIO(recData.read(subSize))
                    if subType == 'EDID':
                        edid = bfRead(subData,CString)
                        subData.seek(0)
                    formData.append((subType,subData))
                if root:
                    self.tree.SetItemText(item,edid,1)
                ins.seek(eor)

            def loadHeader(ins):
                recType = ins.read(4)
                if recType != 'TES4': return None
                ins.seek(-4,os.SEEK_CUR)
                data = {}
                parseRecord(ins,None,data)
                # TES4 record is always formId 0x00000000
                if 0 not in data: return None
                if isinstance(data[0],str): return None
                return data[0]

            masters = []
            def loadMasters(file):
                thisMasters = []
                with BinaryFile(dataPath.join(file).s) as ins:
                    header = loadHeader(ins)
                    if not header: return
                    recHeader = header[1][1]
                    recHeader.seek(8) # Skip type,datasize
                    flags = bfRead(recHeader,UInt32)
                    if flags & 0x80: self.data[file]['strings'] = StringTable()
                    for subType,subData in header[2:]:
                        if subType == 'MAST':
                            subData.seek(0)
                            master = GPath(bfRead(subData,CString))
                            thisMasters.append(master)
                self.data[file]['masters'] = thisMasters
                for master in thisMasters:
                    if master in masters: continue
                    masters.append(master)
                    self.data[master] = OrderedDict()
                    if settings['load.masters']:
                        loadMasters(master)

            subprogress(0,'Pre-scanning')
            for i,file in enumerate(files):
                records.FormIdTable.SetPlugin(file)
                # Read TES4 record
                subprogress(i)
                masters.append(file)
                self.data[file] = OrderedDict()
                loadMasters(file)
            masters.sort(key=lambda x: self.plugins.all.index(x))
            if settings['load.masters']:
                files = masters
            else:
                files.sort(key=lambda x: self.plugins.all.index(x))
            records.FormIdTable.SetPlugins(masters)
            records.FormIdTable.SetData(self.data)

            # Step 2: update the UI with the new items
            root = self.tree.GetRootItem()
            for i,file in enumerate(masters):
                item = self.tree.AppendItem(root,'[%02X] %s' % (i,file.s))
                if file in files:
                    hedr = self.tree.AppendItem(item,'Header')
                    self.data[file]['ui'] = {
                        'root': item,
                        'header': hedr,
                        'strings': None,
                        }
            self.searchMods.Enable(True)

            # Step 3: Load the STRINGS for each plugin
            # ~10% of progress?
            if settings['load.strings']:
                strings_to_load = [x for x in files if 'strings' in self.data[x]]
                subprogress = ui.SubProgress(self.progress,5,15)
                self.LoadStrings(strings_to_load,subprogress)
            else:
                self.progress(15)

            # Step 4: Load the rest of the records
            # ~85% of progress?
            def parseGRUP(ins,root,data,prgs):
                prgs(ins.tell())
                (recType,groupSize,label,
                 groupType,stamp,unk) = bfUnpack(ins,('4s I 4s I I I'))
                label = cStringIO.StringIO(label)

                grup_info = grup_types.get(groupType,('Unknown','%s',4,False))
                eog = ins.tell() + groupSize - 24

                if groupType not in grups:
                    ins.seek(eog)
                    return

                label = bfRead(label,grup_info[2])
                if grup_info[3]:
                    shortId,longId,recordId = records.FormIdTable.ShortToLongFid(label)
                    if longId is None:
                        # missing master, item was added to tree control as
                        # shortId,recordId, and colored pink
                        formId = '%02X%06X' % (shortId,recordId)
                    else:
                        # use the resolved formId to search
                        formId = '%02X%06X' % (longId,recordId)
                    parent = self.tree.FindItem(data['ui']['root'],formId)
                    if parent.IsOk():
                        root = self.tree.AppendItem(parent,grup_info[0])
                    else:
                        root = self.tree.AppendItem(root,grup_info[1] % label)
                else:
                    root = self.tree.AppendItem(root,grup_info[1] % label)
                if self.tree.GetItemParent(root) == data['ui']['root']:
                    data['ui'].setdefault('recOrder',[]).append(grup_info[1] % label)

                while ins.tell() < eog:
                    prgs(ins.tell())
                    subType = ins.read(4)
                    ins.seek(-4,os.SEEK_CUR)
                    if subType == 'GRUP':
                        parseGRUP(ins,root,data,prgs)
                    else:
                        parseRecord(ins,root,data,prgs)

            files = [dataPath.join(x) for x in files]
            total = max(sum(x.size for x in files),1)
            subprogress = ui.SubProgress(self.progress,15,100)
            subprogress.setFull(total)
            subprogress(0)
            done = 0
            for file in files:
                records.FormIdTable.SetPlugin(file.tail)
                size = max(file.size,1)
                subprogress2 = ui.SubProgress(subprogress,done,done+size)
                done += file.size
                subprogress2(0,'%s: Loading records...' % file.stail)
                data = self.data[file.tail]
                root = data['ui']['root']
                with BinaryFile(file.s) as ins:
                    ins.seek(0,os.SEEK_END)
                    eof = ins.tell()
                    ins.seek(0)
                    subprogress2.setFull(eof)

                    while ins.tell() < eof:
                        recType = ins.read(4)
                        size = ins.read(UInt32)
                        if recType == 'TES4':
                            ins.seek(-8,os.SEEK_CUR)
                            parseRecord(ins,None,data)
                            if 0 in data:
                                data['TES4'] = data[0]
                                del data[0]
                        elif recType != 'GRUP':
                            self.tree.AppendItem(root,'ERROR - %s' % recType)
                            break
                        else:
                            # GRUP
                            ins.seek(-8,os.SEEK_CUR)
                            parseGRUP(ins,root,data,subprogress2)

                # Remove any items without children (except Header and Strings)
                child,cookie = self.tree.GetFirstChild(root)
                while child.IsOk():
                    next = self.tree.GetNextSibling(child)
                    if (child != data['ui']['header'] and
                        child != data['ui']['strings'] and
                        self.tree.GetChildrenCount(child,False) == 0):
                        self.tree.Delete(child)
                    child = next
                # Sort the entries if needed
                if settings['ui.sortRecTypes']:
                    hedr = data['ui']['header']
                    strg = data['ui']['strings']
                    self.tree.SetItemText(hedr,'0000') # So it sorts first
                    if strg:
                        self.tree.SetItemText(strg,'0001') # So it sorts second
                    self.tree.SortChildren(root)
                    self.tree.SetItemText(hedr,'Header')
                    if strg:
                        self.tree.SetItemText(strg,'Strings')

            self.progress(100,'Complete')
            self.tb.EnableTool(self.loadButton,True)
            self.tb.EnableTool(self.loadStringsButton,True)
            self.GetMenuBar().Enable(self.loadStringsButton,True)
            self.cancelButton.Enable(False)
            time.sleep(5)
            self.progress(0)
        except SystemExit:
            self.ClearUI()
            self.progress(0,'Canceled')
        except:
            self.ClearUI()
            self.progress(0,'Error')
            raise
        finally:
            self.cancelButton.Enable(False)
            self.tb.EnableTool(self.loadButton,True)
            self.Refresh()
            del self.workers['load']
        return

def loadSettings(markDirty=False):
    # Load config
    config = ConfigParser.ConfigParser(allow_no_value=True)
    config.read('Skyrim Viewer.ini')
    try:
        try:
            lang = config.get('General','Language')
            if lang: lang = lang.strip()
            settings['language'] = lang
        except ConfigParser.NoOptionError:
            if markDirty:
                settings['language'] = None
            pass
        try:
            path = config.get('General','Language')
            if path: path = path.strip()
            if path:
                path = GPath(path)
                if path.exists and path.isdir:
                    settings['skyrim'] = path
        except ConfigParser.NoOptionError:
            pass
        try:
            settings['load.masters'] = config.getboolean('General','LoadMasters')
        except (ConfigParser.NoOptionError,ValueError):
            if markDirty:
                settings['load.masters'] = None
            pass
        try:
            settings['load.strings'] = config.getboolean('General','LoadStrings')
        except (ConfigParser.NoOptionError, ValueError):
            if markDirty:
                settings['load.strings'] = None
            pass
        try:
            settings['ui.sortRecTypes'] = config.getboolean('General','SortRecords')
        except (ConfigParser.NoOptionError, ValueError):
            if markDirty:
                settings['ui.sortRecTypes'] = None
            pass
    except ConfigParser.NoSectionError:
        pass

def saveSettings():
    # Save config
    newSettings = settings.copy()
    loadSettings(True)

    config = ConfigParser.ConfigParser(allow_no_value=True)
    config.add_section('General')
    if settings['language'] != newSettings['language']:
        value = newSettings['language']
    else:
        value = ''
    config.set('General','Language',value)
    config.set('General','LoadMasters',newSettings['load.masters'])
    config.set('General','LoadStrings',newSettings['load.strings'])
    config.set('General','SortRecords',newSettings['ui.sortRecTypes'])
    if settings['skyrim'] != newSettings['skyrim']:
        value = newSettings['skyrim']
    else:
        value = ''
    config.set('General','SkyrimPath',value)
    try:
        with open('Skyrim Viewer.ini','w') as out:
            config.write(out)
    except:
        pass

def main():
    if hasattr(sys,'frozen'):
        exe = GPath(sys.executable).stail
        errLog = GPath(exe+'.log')
        if errLog.exists:
            try:
                errLog.remove()
            except:
                # Error deleting, try just emptying it out
                try:
                    with errLog.open('w') as out:
                        pass
                except:
                    pass
        resources['main_ico'] = exe + ';0'
        resources['stop_ico'] = exe + ';1'
    else:
        resources['main_ico'] = 'images\\viewer.ico'
        resources['stop_ico'] = 'images\\stop.ico'

    loadSettings()

    app = wx.App(False)

    skyrimPath = settings['skyrim']
    if not skyrimPath:
        skyrimPath = GetSkyrimPath()
    if not skyrimPath:
        skyrimPath = AskSkyrimPath()
    if not skyrimPath:
        return
    settings['skyrim'] = skyrimPath

    if not settings['language']:
        iniPath = GetSavesPath().join('Skyrim.ini')
        if not iniPath.exists:
            iniPath = skyrimPath.join('Skyrim.ini')
        if not iniPath.exists:
            iniPath = skyrimPath.join('Skyrim_default.ini')
        if not iniPath.exists:
            settings['language'] = 'ENGLISH'
        else:
            iniConfig = ConfigParser.ConfigParser()
            iniConfig.read(iniPath.s)
            if iniConfig.has_section('General'):
                if iniConfig.has_option('General','sLanguage'):
                    settings['language'] = iniConfig.get('General','sLanguage').strip()

    icons = wx.IconBundle()
    for size in (16,32,48,64,128):
        icon = wx.Icon(resources['main_ico'],wx.BITMAP_TYPE_ICO,size,size)
        if icon:
            icons.AddIcon(icon)
    resources['icons'] = icons

    frame = MyFrame()
    app.SetTopWindow(frame)
    frame.Show()
    frame.Center()
    app.MainLoop()
    saveSettings()

if __name__=='__main__':
    main()