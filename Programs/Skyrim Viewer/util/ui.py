import time
import wx
from wx.lib.mixins.listctrl import ListCtrlAutoWidthMixin, TextEditMixin
import wx.gizmos

# Menus ------------------------------------------------------------------------
class Links(list):
    def AppendToMenu(self,menu,parent,*args):
        for link in self:
            link.AppendToMenu(menu,parent,*args)

    def PopupMenu(self,parent,*args):
        menu = wx.Menu()
        self.AppendToMenu(menu,parent,*args)
        menu.Destroy()

    def GetAccelerators(self):
        return [link.Accelerator for x in links if link.Accelerator]

class Link(object):
    def __init__(self):
        self.id = None
        self.accelerator = None

    def Set(self,menu,window,data):
        self.window = window
        self.data = data
        if not self.id: self.id = wx.NewId()

    def AppendToMenu(self,menu,window,data):
        self.Set(menu,window,data)
        wx.EVT_MENU(parent,self.id,self.Execute)

    def AppendToToolbar(self,toolbar,window,data):
        """wx.EVT_MENU and wx.EVT_TOOL are the same"""
        Link.AppendToMenu(self,toolbar,window,data)

    @property
    def Accelerator(self): return self.accelerator
    def Execute(self,event): pass

class BasicLink(Link):
    def __init__(self,
                 text,
                 helpStr='',
                 accelerator=None,
                 kind=wx.ITEM_NORMAL,
                 bitmapId=None,
                 bitmapSize=16,
                 onClick=None):
        super(BasicLink,self).__init__()
        self.onClick = onClick
        self.text = text
        self.helpStr = helpStr
        self.accelerator = accelerator
        self.kind = kind
        self.bitmapId = bitmapId
        self.bitmapSize = bitmapSize

    def AppendToMenu(self,menu,window,data):
        super(BasicLink,self).AppendToMenu(menu,window,data)
        if self.accelerator:
            menuItem = wx.MenuItem(menu,self.id,
                                   '%s\t%s'%(self.text,self.accelerator[2]),
                                   self.helpStr,self.kind)
        else:
            menuItem = wx.MenuItem(menu,self.id,
                                   self.text,self.helpStr,self.kind)
        menuItem.SetBitmap(self.GetBimap(wx.ART_MENU))
        menu.AppendItem(menuItem)

    def AppendToToolbar(self,toolbar,window,data):
        super(BasicLink,self).AppendToToolbar(toolbar,window,data)
        toolbar.AddSimpleTool(self.id,self.GetBitmap(wx.ART_TOOLBAR),self.text)

    def GetBitmap(self,kind=wx.ART_MENU):
        return wx.ArtProvider.GetBitmap(self.bitmapId,kind,
                                        (self.bitmapSize,self.bitmapSize))

    def Execute(self,event):
        if self.onClick: self.onClick(self.window,event)

class SeparatorLink(Link):
    def AppendToMenu(self,menu,window,data):
        menu.AppendSeparator()

    def AppendToToolbar(self,toolbar,window,data):
        toolbar.AddSeparator()

class BreakLink(Link):
    def AppendToMenu(self,menu,window,data):
        menu.Break()

    def AppendToToolbar(self,toolbar,window,data):
        pass

class MenuLink(Link):
    def __init__(self,name,oneDatumOnly=False,*links):
        super(MenuLink,self).__init__()
        self.name = name
        self.links = Links(*links)
        self.oneDatumOnly = oneDatumOnly

    def AppendToMenu(self,menu,window,data):
        subMenu = wx.Menu()
        for link in self.links:
            link.AppendToMenu(subMenu,window,data)
        menu.AppendMenu(wx.ID_ANY,self.name,subMenu)
        if self.oneDatumOnly and len(data) != 1:
            id = menu.FindItem(self.name)
            menu.Enable(id,False)

    def AppendToToolbar(self,toolbar,window,data):
        pass

# Sizers -----------------------------------------------------------------------
spacer = ((0,0),1)

def aSizer(sizer,*elements):
    for element in elements:
        if isinstance(element,tuple):
            if element[0] != None:
                sizer.Add(*element)
        elif element != None:
            sizer.Add(element)
    return sizer

def hSizer(*elements):
    return aSizer(wx.BoxSizer(wx.HORIZONTAL),*elements)

def vSizer(*elements):
    return aSizer(wx.BoxSizer(wx.VERTICAL),*elements)

def hsbSizer(boxArgs,*elements):
    return aSizer(wx.StaticBoxSizer(wx.StaticBox(*boxArgs),wx.HORIZONTAL),
                  *elements)

def vsbSizer(boxArgs,*elements):
    return aSizer(wx.StaticBoxSizer(wx.StaticBox(*boxArgs),wx.VERTICAL),
                  *elements)

# Custom controls -------------------------------------------------------------
class ProportionalSplitterWindow(wx.SplitterWindow):
    def __init__(self,proportion,*args,**kwdargs):
        super(ProportionalSplitterWindow,self).__init__(*args,**kwdargs)
        self.SetMinimumPaneSize(50)
        if not 0 < proportion < 1:
            proportion = 0.66
        self.proportion = proportion
        self.ResetSash()
        self.Bind(wx.EVT_SIZE,self.__OnSize)
        self.Bind(wx.EVT_SPLITTER_SASH_POS_CHANGED, self.__OnSash)
        self.Bind(wx.EVT_PAINT, self.__OnPaint)

    def SplitHorizontally(self, left, right):
        if self.GetParent() is None: return False
        return super(ProportionalSplitterWindow,self).SplitHorizontally(
            lef,right,int(round(self.GetParent().GetSize().GetHeight()
                                * self.proportion)))

    def SplitVertically(self,top,bottom):
        if self.GetParent() is None: return False
        return super(ProportionalSplitterWindow,self).SplitVertically(
            top,bottom,int(round(self.GetParent().GetSize().GetWidth()
                                 * self.proportion)))

    def GetExpectedSashPosition(self):
        return int(round(self._gettot() * self.proportion))

    def ResetSash(self):
        self.SetSashPosition(self.GetExpectedSashPosition())

    def _gettot(self):
        if self.GetSplitMode() == wx.SPLIT_HORIZONTAL:
            attr = 'height'
        else:
            attr = 'width'
        return max(self.GetMinimumPaneSize(),
                   getattr(self.GetParent().GetClientSize(),attr))

    def __OnSize(self,event):
        self.ResetSash()
        event.Skip()

    def __OnSash(self,event):
        pos = float(self.GetSashPosition())
        self.proportion = pos / self._gettot()
        event.Skip()

    def __OnPaint(self,event):
        self.Unbind(wx.EVT_PAINT)
        if self.GetSashPosition() != self.GetExpectedSashPosition():
            self.ResetSash()
        event.Skip()

class StatusBar(wx.StatusBar):
    """wx.StatusBar with auto-sizing controls"""
    def __init__(self,*args,**kwdargs):
        wx.StatusBar.__init__(self,*args,**kwdargs)
        self.controls = {}
        self.Bind(wx.EVT_SIZE,self._onResize)

    def SetControl(self,idex,control,flags=0,border=0):
        top = border if flags & wx.TOP else 0
        bottom = border if flags & wx.BOTTOM else 0
        left = border if flags & wx.LEFT else 0
        right = border if flags & wx.RIGHT else 0
        self.controls[idex] = (control,top,bottom,left,right)
        self._onResize()

    def GetControl(self,idex):
        return self.controls[idex]

    def _onResize(self,event=None):
        num = self.GetFieldsCount()
        for idex in self.controls:
            if 0 <= idex < num:
                # Valid control
                control,top,bottom,left,right = self.controls[idex]
                rect = self.GetFieldRect(idex)
                x = rect.x + left
                y = rect.y + top
                w = rect.width - (left + right)
                h = rect.height - (top + bottom)
                control.SetDimensions(x,y,w,h)
        if event:
            event.Skip()

class TreeListCtrl(wx.gizmos.TreeListCtrl, ListCtrlAutoWidthMixin):
    def __init__(self,*args,**kwdargs):
        wx.gizmos.TreeListCtrl.__init__(self,*args,**kwdargs)
        ListCtrlAutoWidthMixin.__init__(self)

class ListCtrl(wx.ListCtrl,ListCtrlAutoWidthMixin, TextEditMixin):
    editorBgColour = wx.Colour(255,255,255)

    def __init__(self,*args,**kwdargs):
        wx.ListCtrl.__init__(self,*args,**kwdargs)
        ListCtrlAutoWidthMixin.__init__(self)
        TextEditMixin.__init__(self)
        self._no_edit_cols = set()
        self.Bind(wx.EVT_LIST_BEGIN_LABEL_EDIT, self.__OnBeginEdit)

    def __OnBeginEdit(self,event):
        if event.GetColumn() in self._no_edit_cols:
            event.Veto()

    def SetColumnEditable(self,col,editable=True):
        if editable:
            self._no_edit_cols.discard(col)
        else:
            self._no_edit_cols.add(col)

    # Have to override some of TextEditMixin's functions to get it to work
    # how I want
    def OnChar(self,event):
        if event.GetKeyCode() == wx.WXK_ESCAPE:
            self.CloseEditor(canceled=True)
        else:
            TextEditMixin.OnChar(self,event)

    def CloseEditor(self,event=None,canceled=False):
        if canceled:
            self.editor.Hide()
            self.SetFocus()
        else:
            TextEditMixin.CloseEditor(self,event)

# Busy Cursor -----------------------------------------------------------------
class BusyCursor(object):
    def __enter__(self):
        wx.BeginBusyCursor()
    def __exit__(self,*args):
        wx.EndBusyCursor()

# StatusBar Guage and Text manipulations --------------------------------------
class BaseProgress(object):
    def __init__(self,full=1.0):
        self.setFull(1.0)
        self.message = ''
        self.state = 0

    def setFull(self,full):
        if 1.0*full == 0: raise ValueError('full must be non-zero')
        self.full = full
        return self

    def __call__(self,state=None,msg=None):
        if state is None: state = self.state + 10
        if msg: self.message = msg
        self.doProgress(float(state)/self.full,self.message)
        self.state = state

    def doProgress(self,state,message):
        pass

class Progress(BaseProgress):
    def __init__(self,guage,statusbar,textField,message='',full=1.0):
        self.guage = guage
        guage.SetRange(1000)
        self.statusbar = statusbar
        self.textField = textField
        self.message = message
        self.prevMessage = ''
        self.prevState = -1
        self.prevTime = 0

    def doProgress(self,state,msg):
        if (state == 0 or state == 1 or
            msg != self.prevMessage or
            state - self.prevState > 0.05 or
            time.time() - self.prevTime > 0.25):
            if msg != self.prevMessage:
                self.statusbar.SetStatusText(msg,self.textField)
            self.guage.SetValue(int(state*1000))
            self.prevMessage = msg
            self.prevState = state
            self.prevTime = time.time()

class SubProgress(BaseProgress):
    def __init__(self,parent,baseFrom='+0',baseTo='+1',full=1.0):
        super(SubProgress,self).__init__(full)
        if baseFrom == '+0': baseFrom = parent.state
        if baseTo == '+1': baseTo = baseFrom + 1
        if 0 > baseFrom >= baseTo:
            raise ValueError('baseFrom must be >= 0 and baseTo must be > baseFrom')
        self.parent = parent
        self.baseFrom = baseFrom
        self.scale = 1.0*(baseTo-baseFrom)

    def __call__(self,state,message=''):
        self.parent(self.baseFrom+self.scale*state/self.full,message)
        self.state = state

# Dialogs ---------------------------------------------------------------------
def FolderDialog(parent=None,message='Choose a folder',**kwdargs):
    kwdargs['defaultFile'] = 'Folder Selection'
    kwdargs['wildcard'] = 'Folder Selection|*.?!>'
    dialog = wx.FileDialog(parent,message,**kwdargs)
    if dialog.ShowModal() == wx.ID_OK:
        folder = dialog.GetDirectory()
    else:
        folder = None
    dialog.Destroy()
    return folder

'''

# Progress Dialogs ------------------------------------------------------------
class BaseProgress(object):
    """Progress Callable: Shows progress when called."""
    def __init__(self,full=1.0):
        self.setFull(full)
        self.message = ''
        self.state = 0

    def setFull(self,full):
        """Set's full and for convenience, returns self."""
        if (1.0*full) == 0: raise Excepton('Full must be non-zero!')
        self.full = full
        return self

    def __call__(self,state=None,message=''):
        """Update progress with current state. Progress is state/full."""
        if state is None: state = self.state + 1
        if message: self.message = message
        self.doProgress(float(state)/self.full, self.message)
        self.state = state

    def doProgress(self,progress,message):
        """Default doProgress does nothing."""
        pass

class Progress(BaseProgress):
    """Progress as progress dialog."""
    def __init__(self,title='Progress',message=' '*60,parent=None,
                 style=wx.PD_APP_MODAL|wx.PD_ELAPSED_TIME|wx.PD_AUTO_HIDE,
                 abort=False,onAbort=None,full=1.0):
        if abort:
            style |= wx.PD_CAN_ABORT
            self.fnAbort = onAbort
        self.dialog = wx.ProgressDialog(title,message,100,parent,style)
        self.dialog.SetFocus()
        super(Progress,self).__init__(full)
        self.message = message
        self.isDestroyed = False
        self.prevMessage = ''
        self.prevState = -1
        self.prevTime = 0

    # __enter__ and __exit__ for use with the 'with' statement
    def __enter__(self):
        return self
    def __exit__(self,type,value,traceback):
        self.Destroy()

    def setCancel(self, enabled=True):
        cancel = self.dialog.FindWindowById(wx.ID_CANCEL)
        cancel.Enable(enabled)

    def onAbort(self):
        if self.fnAbort:
            return self.fnAbort()
        return True

    def doProgress(self,state,message):
        if not self.dialog:
            raise Exception('Dialog already destroyed.')
        elif (state == 0 or state == 1 or (message != self.prevMessage) or
              (state - self.prevState) > 0.05 or
              (time.time() - self.prevTime) > 0.25):
            self.dialog.SetFocus()
            if message != self.prevMessage:
                ret = self.dialog.Update(int(state*100),message)
            else:
                ret = self.dialog.Update(int(state*100))
            if not ret[0]:
                if self.onAbort():
                    raise Exception('Canceled')
            self.prevMessage = message
            self.prevState = state
            self.prevTime = time.time()

    def Destroy(self):
        if self.dialog:
            self.dialog.Destroy()
            self.dialog = None

class SubProgress(BaseProgress):
    """Sub progress goes from base to ceiling."""
    def __init__(self,parent,baseFrom='+0',baseTo='+1',full=1.0,silent=False):
        """For creating a subprogress of another progress meter.
        progress: parent (base) progress meter
        baseFrom: Base progress when this progress == 0.
        baseTo: Base progress when this progress == full
          Usually a number. But string '+1' sets it to baseFrom + 1
        full: Full meter by this progress' scale."""
        BaseProgress.__init__(self,full)
        if baseFrom == '+0': baseFrom = parent.state
        if baseTo == '+1': baseTo = baseFrom + 1
        if (baseFrom < 0 or baseFrom >= baseTo):
            raise Exception('BaseFrom must be >= 0 and BaseTo must be > BaseFrom')
        self.parent = parent
        self.baseFrom = baseFrom
        self.scale = 1.0*(baseTo-baseFrom)
        self.silent = silent

    def __call__(self,state,message=''):
        """Update progress with current state. Progress is state/full."""
        if self.silent: message = ''
        self.parent(self.baseFrom+self.scale*state/self.full,message)
        self.state = state
'''