# From Wrye Bash

import os, sys
import shutil
import time, random
import subprocess
import binascii

import winshell

# Paths -----------------------------------------------------------------------
#------------------------------------------------------------------------------
_gpaths = {}
Path = None
def GPath(name):
    """Returns common path object for specified name/path."""
    if name is None: return None
    elif not name: norm = name
    elif isinstance(name,Path): norm = name._s
    else: norm = os.path.normpath(name)
    path = _gpaths.get(norm)
    if path != None: return path
    else: return _gpaths.setdefault(norm,Path(norm))

def APath(name):
    """Returns an absolute path."""
    if name is None: return None
    elif not name: norm = name
    elif isinstance(name,Path): norm = name._s
    else: norm = os.path.normpath(name)
    path = Path(norm)
    if not path.isabs:
        path = Path.getcwd().join(path)
    return path

#------------------------------------------------------------------------------
class Path(object):
    """A file path. May be just a directory, filename or full path."""
    """Paths are immutable objects that represent file directory paths."""

    #--Class Vars/Methods -------------------------------------------
    norm_path = {} #--Dictionary of paths
    mtimeResets = [] #--Used by getmtime

    @staticmethod
    def get(name):
        """Returns path object for specified name/path."""
        if isinstance(name,Path): norm = name._s
        else: norm = os.path.normpath(name)
        return Path.norm_path.setdefault(norm,Path(norm))

    @staticmethod
    def getNorm(name):
        """Return the normpath for specified name/path object."""
        if not name: return name
        elif isinstance(name,Path): return name._s
        else: return os.path.normpath(name)

    @staticmethod
    def getCase(name):
        """Return the normpath+normcase for specified name/path object."""
        if not name: return name
        if isinstance(name,Path): return name._cs
        else: return os.path.normcase(os.path.normpath(name))

    @staticmethod
    def getcwd():
        return Path(unicode(os.getcwd()))

    @staticmethod
    def getDesktop(common=False):
        return Path(winshell.desktop(common))
    @staticmethod
    def getAppData(common=False):
        return Path(winshell.application_data(common))
    @staticmethod
    def getLocalAppData():
        return Path(winshell.local_application_data())
    @staticmethod
    def getFavorites(common=False):
        return Path(winshell.favourites(common))
    @staticmethod
    def getStartMenu(common=False):
        return Path(winshell.start_menu(common))
    @staticmethod
    def getPrograms(common=False):
        return Path(winshell.programs(common))
    @staticmethod
    def getStartup(common=False):
        return Path(winshell.startup(common))
    @staticmethod
    def getPersonal():
        return Path(winshell.personal_folder())
    @staticmethod
    def getRecent():
        return Path(winshell.recent())
    @staticmethod
    def getSendTo():
        return Path(winshell.sendto())

    def setcwd(self):
        """Set cwd. Works as either instance or class method."""
        if isinstance(self,Path): dir = self._s
        else: dir = self
        os.chdir(dir)

    #--Instance stuff --------------------------------------------------
    #--Slots: _s is normalized path. All other slots are just pre-calced
    #  variations of it.
    __slots__ = ('_s','_cs','_csroot','_sroot','_shead','_stail','_ext','_cext','_sbody','_csbody')

    def __init__(self, name):
        """Initialize."""
        if isinstance(name,Path):
            norm = name._s
        elif isinstance(name,unicode):
            norm = name
        elif isinstance(name,str):
            norm = unicode(name,'utf8')
        else:
            raise TypeError, u'Expected Path, unicode, or str.  Got %s.' % type(name)
        self._s = norm
        self._sroot,self._ext = os.path.splitext(self._s)
        self._shead,self._stail = os.path.split(self._s)
        self._sbody = os.path.basename(self._sroot)
        self._cs = os.path.normcase(self._s)
        self._cext = os.path.normcase(self._ext)
        self._csroot = os.path.normcase(self._sroot)
        self._csbody = os.path.normcase(self._sbody)

    def __len__(self):
        """Length of path string."""
        return len(self._s)

    def __repr__(self):
        return "Path("+repr(self._s)+")"

    def __str__(self):
        return self._s

    #--Properties--------------------------------------------------------
    @property
    def s(self):
        "Path as string."
        return self._s
    @property
    def cs(self):
        "Path as string in normalized case."
        return self._cs

    @property
    def sroot(self):
        "Root as string."
        return self._sroot
    @property
    def csroot(self):
        "Root as string."
        return self._csroot

    @property
    def shead(self):
        "Head as string."
        return self._shead

    @property
    def stail(self):
        "Tail as string."
        return self._stail

    @property
    def sbody(self):
        "For alpha\beta.gamma returns beta as string."
        return self._sbody
    @property
    def csbody(self):
        "For alpha\beta.gamma returns beta as string in normalized case."
        return self._csbody

    #--Head, tail
    @property
    def headTail(self):
        "For alpha\beta.gamma returns (alpha,beta.gamma)"
        return map(GPath,(self._shead,self._stail))
    @property
    def head(self):
        "For alpha\beta.gamma, returns alpha."
        return GPath(self._shead)
    @property
    def tail(self):
        "For alpha\beta.gamma, returns beta.gamma."
        return GPath(self._stail)
    @property
    def body(self):
        "For alpha\beta.gamma, returns beta."
        return GPath(self._sbody)

    #--Root, ext
    @property
    def rootExt(self):
        return (GPath(self._sroot),self._ext)
    @property
    def root(self):
        "For alpha\beta.gamma returns alpha\beta"
        return GPath(self._sroot)
    @property
    def ext(self):
        "Extension (including leading period, e.g. '.txt')."
        return self._ext
    @property
    def cext(self):
        "Extension in normalized case."
        return self._cext
    @property
    def temp(self):
        "Temp file path.."
        return self+'.tmp'
    @property
    def backup(self):
        "Backup file path."
        return self+'.bak'

    #--size, atim, ctime
    @property
    def size(self):
        "Size of file or directory."
        if self.isdir:
            join = os.path.join
            getSize = os.path.getsize
            try:
                return sum([sum(map(getSize,map(lambda z: join(x,z),files))) for x,y,files in os.walk(self._s)])
            except ValueError:
                return 0
        else:
            return os.path.getsize(self._s)
    @property
    def atime(self):
        "Access time."
        return os.path.getatime(self._s)
    @property
    def ctime(self):
        "Creation time."
        return os.path.getctime(self._s)

    #--Mtime
    def getmtime(self,maxMTime=False):
        """Returns mtime for path. But if mtime is outside of epoch, then resets
        mtime to an in-epoch date and uses that."""
        if self.isdir and maxMTime:
            #fastest implementation I could make
            c = []
            cExtend = c.extend
            join = os.path.join
            getM = os.path.getmtime
            [cExtend([getM(join(root,dir)) for dir in dirs] + [getM(join(root,file)) for file in files]) for root,dirs,files in os.walk(self._s)]
            try:
                return max(c)
            except ValueError:
                return 0
        mtime = int(os.path.getmtime(self._s))
        #--Y2038 bug? (os.path.getmtime() can't handle years over unix epoch)
        if mtime <= 0:
            import random
            #--Kludge mtime to a random time within 10 days of 1/1/2037
            mtime = time.mktime((2037,1,1,0,0,0,3,1,0))
            mtime += random.randint(0,10*24*60*60) #--10 days in seconds
            self.mtime = mtime
            Path.mtimeResets.append(self)
        return mtime
    def setmtime(self,mtime):
        os.utime(self._s,(self.atime,int(mtime)))
    mtime = property(getmtime,setmtime,doc="Time file was last modified.")

    #--crc
    @property
    def crc(self,readSize=0x200000):
        """Calculates and returns crc value for self."""
        size = self.size
        crc = 0L
        ins = self.open('rb')
        insRead = ins.read
        while ins.tell() < size:
            crc = crc32(insRead(readSize),crc)
        ins.close()
        return crc & 0xffffffff

    #--crc with a callback
    def crcProgress(self,callback,readSize=0x200000):
        """Calculates and returns crc value for self, updating progress as it goes."""
        size = self.size
        if size == 0:
            return 0L
        else:
            crc = 0L
            with self.open('rb') as ins:
                insRead = ins.read
                while ins.tell() < size:
                    crc = crc32(insRead(readSize),crc)
                    callback((ins.tell(),size))
            return crc & 0xFFFFFFFF

    #--File system info
    @property
    def exists(self):
        return os.path.exists(self._s)
    @property
    def isdir(self):
        return os.path.isdir(self._s)
    @property
    def isfile(self):
        return os.path.isfile(self._s)
    @property
    def isabs(self):
        return os.path.isabs(self._s)

    #--Path stuff -------------------------------------------------------
    #--New Paths, subpaths
    def __add__(self,other):
        """self + other"""
        return GPath(self._s + Path.getNorm(other))
    def join(*args):
        """self.join([...])"""
        norms = [Path.getNorm(x) for x in args]
        return GPath(os.path.join(*norms))
    def list(self):
        """For directory: Returns list of files."""
        if not self.exists: return []
        return [GPath(x) for x in os.listdir(self._s)]
    def walk(self,topdown=True,onerror=None,relative=False):
        """Like os.walk."""
        if relative:
            start = len(self)
            return ((GPath(x[start:]),[GPath(u) for u in y],[GPath(u) for u in z])
                for x,y,z in os.walk(self._s,topdown,onerror))
        else:
            return ((GPath(x),[GPath(u) for u in y],[GPath(u) for u in z])
                for x,y,z in os.walk(self._s,topdown,onerror))
    def split(self):
        """Splits the path into each of it's sub parts.  IE: C:\Program Files\Bethesda Softworks
           would return ['C:','Program Files','Bethesda Softworks']"""
        dirs = []
        drive, path = os.path.splitdrive(self.s)
        path = path.strip(os.path.sep)
        l,r = os.path.split(path)
        while l != '':
            dirs.append(r)
            l,r = os.path.split(l)
        dirs.append(r)
        if drive != '':
            dirs.append(drive)
        dirs.reverse()
        return dirs
    def relpath(self,path):
        return GPath(os.path.relpath(self._s,Path.getNorm(path)))

    #--File system manipulation
    def open(self,*args):
        if self._shead and not os.path.exists(self._shead):
            os.makedirs(self._shead)
        return open(self._s,*args)
    def makedirs(self):
        if not self.exists: os.makedirs(self._s)
    def remove(self,allowUndo=True,noConfirm=True,renameOnCollision=True,silent=False):
        if self.exists:
            winshell.delete_file(self._s,allowUndo,noConfirm,renameOnCollision,silent)

    #--start, move, copy, touch, untemp
    def start(self, exeArgs=None):
        """Starts file as if it had been doubleclicked in file explorer."""
        if self._cext == '.exe':
            if not exeArgs:
                subprocess.Popen([self.s], close_fds=True)
            else:
                subprocess.Popen(exeArgs, executable=self.s, close_fds=True)
        else:
            os.startfile(self._s)
    def copyTo(self,destName,allowUndo=True,noConfirm=False,renameOnCollision=True,silent=False):
        destName = GPath(destName)
        if self.isdir:
            winshell.copy_file(self._s,destName._s,allowUndo,noConfirm,renameOnCollision,silent)
        else:
            if destName._shead and not os.path.exists(destName._shead):
                os.makedirs(destName._shead)
            winshell.copy_file(self._s,destName._s,allowUndo,noConfirm,renameOnCollision,silent)
            destName.mtime = self.mtime
    def moveTo(self,destName,allowUndo=True,noConfirm=False,renameOnCollision=True,silent=False):
        if not self.exists:
            raise IOError(self._s + u' cannot be moved because it does not exist.')
        destPath = GPath(destName)
        if destPath._cs == self._cs: return
        if destPath._shead and not os.path.exists(destPath._shead):
            os.makedirs(destPath._shead)
        winshell.move_file(self._s,destPath._s,allowUndo,noConfirm,renameOnCollision,silent)
    def rename(self,destName,allowUndo=True,noConfirm=False,renameOnCollision=True,silent=False):
        if not self.exists:
            raise IOError(self._s + u' cannot be renamed because it does not exist.')
        destPath = GPath(destName)
        if destPath._s == self._s: return
        winshell.rename_file(self._s,destPath._s,allowUndo,noConform,renameOnCollision,silent)
    def touch(self):
        """Like unix 'touch' command. Creates a file with current date/time."""
        if self.exists():
            self.mtime = time.time()
        else:
            self.temp.open('w').close()
            self.untemp()
    def untemp(self,doBackup=False):
        """Replaces file with temp version, optionally making backup of file first."""
        if self.temp.exists:
            if self.exists:
                if doBackup:
                    winshell.delete_file(self.backup._s,True,True,True,True)
                    shutil.move(self._s, self.backup._s)
                else:
                    winshell.delete_file(self._s,True,True,True,True)
            shutil.move(self.temp._s, self._s)

    #--Hash/Compare
    def __hash__(self):
        return hash(self._cs)

    def __cmp__(self, other):
        if isinstance(other,Path):
            return cmp(self._cs, other._cs)
        elif isinstance(other,unicode):
            return cmp(self._cs, Path.getCase(other))
        elif isinstance(other,str):
            return cmp(self._cs, Path.getCase(unicode(other,'utf8')))
        elif other is None:
            return cmp(self._cs,other)
        else:
            raise TypeError, "Expected Path, unicode, or str.  Got %s." % type(other)