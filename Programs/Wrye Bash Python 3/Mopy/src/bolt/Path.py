# -*- coding: utf-8 -*-
# src/bolt/Path.py
#
# GPL License and Copyright Notice =============================================
#  This file is part of Wrye Bolt.
#
#  Wrye Bolt is free software; you can redistribute it and/or
#  modify it under the terms of the GNU General Public License
#  as published by the Free Software Foundation; either version 2
#  of the License, or (at your option) any later version.
#
#  Wrye Bolt is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Wrye Bolt; if not, write to the Free Software Foundation,
#  Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#
#  Wrye Bolt copyright (C) 2005, 2006, 2007, 2008, 2009 Wrye
#
# ==============================================================================

"""This module contains a class wrapper around path operations, as well as a
   few useful helper functions."""

# Imports ----------------------------------------------------------------------
#--Standard
import os
import stat
import shutil
import time
import random
import sys
import subprocess
import codecs
import tempfile
from binascii import crc32

#--Libraries
try:
    import win32api
    from win32com.shell import shell, shellcon
    _win32apiGetFileVersionInfo = win32api.GetFileVersionInfo
    _win32apiHIWORD = win32api.HIWORD
    _win32apiLOWORD = win32api.LOWORD
except ImportError:
    win32api = None

# Bring functions into local namespace for quicker execution -------------------
#--os
_osGetcwd = os.getcwd
_osWalk = os.walk
_osUtime = os.utime
_osStat = os.stat
_osChdir = os.chdir
_osChmod = os.chmod
_osAccess = os.access
_osMakedirs = os.makedirs
_osRemove = os.remove
_osRemovedirs = os.removedirs
_osStartfile = os.startfile
_osListdir = os.listdir
_osW_OK = os.W_OK
#--stat
_statFlags = stat.S_IWUSR|stat.S_IWOTH
#--os.path
_osPathExists = os.path.exists
_osPathIsdir = os.path.isdir
_osPathIsfile = os.path.isfile
_osPathIsabs = os.path.isabs
_osPathNormpath = os.path.normpath
_osPathNormcase = os.path.normcase
_osPathSplitext = os.path.splitext
_osPathSplitdrive = os.path.splitdrive
_osPathSplit = os.path.split
_osPathBasename = os.path.basename
_osPathJoin = os.path.join
_osPathGetsize = os.path.getsize
_osPathGetatime = os.path.getatime
_osPathGetctime = os.path.getctime
_osPathGetmtime = os.path.getmtime
_osPathSep = os.path.sep
_osPathAltsep = os.path.altsep
_osPathRelpath = os.path.relpath
_osPathRealpath = os.path.realpath
#--sys
_sysGetrefcount = sys.getrefcount
#--time
_timeTime = time.time
_timeMktime = time.mktime
#--random
_randomRandint = random.randint
#--tempfile
_tempfileMktemp = tempfile.mktemp
_tempfileMkdtemp = tempfile.mkdtemp
_tempfileGettempdir = tempfile.gettempdir
#--codecs
_codecsOpen = codecs.open
#--shutil
_shutilCopyfile = shutil.copyfile
_shutilCopytree = shutil.copytree
_shutilMove = shutil.move
_shutilRmtree = shutil.rmtree
#--subprocess
_subprocessCall = subprocess.call
_subprocessPopen = subprocess.Popen
_subprocessPIPE = subprocess.PIPE

# Startupinfo - so subprocess.Popen can launch things with no cmd.exe window ---
startupinfo = None
if os.name == 'nt':
    startupinfo = subprocess.STARTUPINFO()
    startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW

#--Paths -----------------------------------------------------------------------
#-------------------------------------------------------------------------------
Path = None  # Place holder, so GPath doesn't have undefined 'Path'

#--GPaths: global dictionary of saved Path class objects, to avoid duplication
#  when dealing with lots of path items.
_gpaths = {}
_gpathsGet = _gpaths.get
_gpathsSetdefault = _gpaths.setdefault
_gpathsKeys = _gpaths.keys

def GPath(name):
    """Returns common Path instance for specified name/path."""
    # None
    if name is None: return None
    # Empty string
    elif not name: norm = ''
    # Path object
    elif isinstance(name,Path): norm = name._s
    # Unicode object (str)
    elif isinstance(name,str): norm = _osPathNormpath(name)
    # Try coercing to unicode
    else: norm = _osPathNormpath(str(name))
    # Lookup
    path = _gpathsGet(norm)
    if path is not None: return path
    else: return _gpathsSetdefault(norm,Path(norm))

def GPathPurge():
    """Cleans out the _gpaths dictionary of unused Path object."""
    for key in _gpathsKeys():
        if sysGetrefcount(_gpaths[key]) == 2:
            # 1 for the reference actually in _gpaths
            # 1 for the temp reference passed to sys.getrefcount
            # So 2 = no other references
            del _gpaths[key]

#-------------------------------------------------------------------------------
def getcwd():
    """Get the current working directory."""
    return GPath(_osGetcwd())

def getNorm(name):
    """Return normalized path for specified string/path object."""
    if not name: return name
    elif isinstance(name,Path): return name._s
    return _osPathNormpath(name)

def getCase(name):
    """Return normalized path + normalizes case for string/path object."""
    if not name: return name
    if isinstance(name,Path): return name._s
    return _osPathNormcase(_osPathNormpath(name))

def tempdir():
    """Returns directory where temp files are made."""
    return GPath(_tempfileGettempdir())

def makeTempdir(suffix='',prefix='tmp'):
    """Creates a new temporary directory."""
    return GPath(_tempfileMkdtemp(suffix,prefix))

class Path(object):
    """A file path.  May be a directory or filename, a full path or relative
       path.  Can include the drive or not.  Supports Pickling."""

    __slots__ = ('_s','_cs','_sroot','_csroot','_shead','_stail','_ext','_cext',
                 '_sbody','_csbody')

    def __init__(self, name):
        """Initialize."""
        if isinstance(name,Path):
            self.__setstate__(name._s)
        else:
            self.__setstate__(name)

    def __getstate__(self):
        """Used by pickler to picked object."""
        return self._s

    def __setstate__(self,norm):
        """Used by unpickler to create object.  Doubles as an initializer."""
        self._s = norm
        self._cs = _osPathNormcase(self._s)
        self._sroot,self._ext = _osPathSplitext(self._s)
        self._csroot,self._cext = _osPathSplitext(self._cs)
        self._shead,self._stail = _osPathSplit(self._s)
        self._sbody = _osPathBasename(self._sroot)
        self._csbody = _osPathBasename(self._csroot)

    def __len__(self):
        """Length of path string."""
        return len(self._s)

    def __repr__(self):
        """Representation of Path object."""
        return 'Path('+repr(self._s)+')'

    def __str__(self):
        """Same as self.s, return Path as unicode string"""
        return self._s

    def __add__(self,other):
        """Add two path strings together.  Does not insert path separators."""
        return GPath(self._s + getNorm(other))

    def __div__(self,other):
        """Join to paths together with path separator."""
        return GPath(_osPathJoin(self._s,getNorm(other)))

    def __hash__(self):
        """Has function for use as a key for containers."""
        return hash(self._cs)

    #--Comparison functions (__cmp__ doesn't exist in Python 3)
    def __lt__(self,other):
        """Comparison less than."""
        return self._cs < getCase(other)

    def __le__(self,other):
        """Comparison less than or equal to."""
        return self._cs <= getCase(other)

    def __gt__(self,other):
        """Comparison greater than."""
        return self._cs > getCase(other)

    def __ge__(self,other):
        """Comparison greater than or equal to."""
        return self._cs >= getCase(other)

    def __eq__(self,other):
        """Comparison equals function."""
        return self._cs == getCase(other)

    def __ne__(self,other):
        """Comparison not equals function."""
        return self._cs != getCase(other)

    #--Properties ----------------------------------------------------------
    @property
    def s(self):
        """Return path as string."""
        return self._s

    @property
    def cs(self):
        """Return path as case normalized string."""
        return self._cs

    @property
    def sroot(self):
        """Return root as string."""
        return self._sroot

    @property
    def csroot(self):
        """Return root as case normalized string."""
        return self._csroot

    @property
    def shead(self):
        """Return head as string."""
        return self._shead

    @property
    def stail(self):
        """Return tail as string."""
        return self._stail

    @property
    def sbody(self):
        """Return body as string."""
        return self._sbody

    @property
    def csbody(self):
        """Return body as case normalized string."""
        return self._csbody

    @property
    def headTail(self):
        """For alpha\beta.gamma returns (alpha,beta.gamma)"""
        return map(GPath(self._shead,self._stail))

    @property
    def head(self):
        """For alpha\beta.gamma returns alpha."""
        return GPath(self._shead)

    @property
    def tail(self):
        """For alpha\beta.gamma returns beta.gamma"""
        return GPath(self._stail)

    @property
    def body(self):
        """For alpha\beta.gamma returns beta."""
        return GPath(self._sbody)

    @property
    def rootExt(self):
        """For alpha\beta.gamma returns (alpha\beta, gamma)."""
        return (GPath(self._sroot),self._ext)

    @property
    def root(self):
        """For alpha\beta.gamma returns alpha\beta"""
        return GPath(self._sroot)

    @property
    def ext(self):
        """Returns extension, including period"""
        return self._ext

    @property
    def cext(self):
        """Returns extension, including period, as case normalized stirng"""
        return self._cext

    @property
    def temp(self,encodingSafe='ascii'):
        """"Returns a temp file path for this file.  If encodingSafe is
            specified, renames the temp path if necessary so it can be
            encoded in the specified encoding.  This would be useful for
            example when used with subprocess.Popen, which encodes to ASCII
            automatically before passing to the command line."""
        path = _tempfileMktemp(prefix='WryeBash_',suffix='.tmp'+self._ext)
        path = Path(path)
        if encodingSafe:
            try:
                path._s.encode(encodingSafe)
            except UnicodeEncodeError:
                path = str(path._s.encode(encodingSafe,'xmlcharrefreplace'),
                           encodingSafe)
                path = Path(path)
        return path

    @property
    def size(self):
        """Size of file or directory."""
        if _osPathIsdir(self._s):
            # Directory, return cumulative size
            # of all contained files
            join = _osPathJoin
            getSize = _osPathGetsize
            try:
                return sum([sum(map(getSize,map(lambda z: join(x,z),files)))
                            for x,y,files in _osWalk(self._s)])
            except ValueError:
                return 0
        else:
            return _osPathGetsize(self._s)

    @property
    def atime(self):
        """Time file was last accessed."""
        return _osPathGetatime(self._s)

    @property
    def ctime(self):
        """Time file was created."""
        return _osPathGetctime(self._s)

    #--mtime
    def getmtime(self,maxMTime=False):
        if _osPathIsdir(self._s) and maxMTime:
            c = []
            cExtend = c.extend
            join = _osPathJoin
            getM = _osPathGetmtime
            [cExtend([getM(join(root,dir)) for dir in dirs] +
                     [getM(join(root,file)) for file in files])
             for root,dirs,files in _osWalk(self._s)]
            try:
                return max(c)
            except ValueError:
                return 0
        else:
            mtime = int(_osPathGetmtime(self._s))
            if mtime <= 0:
                #--Y2038 bug - os.path.getmtime can't handle years past
                #  the Unix epoch, reset to a random time 10 days within
                #  1/1/2037
                mtime = _timeMktime((2037,1,1,0,0,0,3,1,0))
                mtime += _randomRandint(864000) # 10 days in seconds
                _osUtime(self._s,(_osPathGetatime(self._s),mtime))
            return mtime
    def setmtime(self,mtime):
        _osUtime(self._s,(_osPathGetatime(self._s),int(mtime)))
    mtime = property(getmtime,setmtime,doc='Time file was last modified.')

    @property
    def stat(self):
        """File stats."""
        return _osStat(self._s)

    @property
    def version(self):
        """File version (exe/dll)."""
        if win32api:
            try:
                info = _win32apiGetFileVersionInfo(self._s,'\\')
                ms = info['FileVersionMS']
                ls = info['FileVersionLS']
                return (_win32apiHIWORD(ms),_win32apiLOWORD(ms),
                        _win32apiHIWORD(ls),_win32apiLOWORD(ls))
            except:
                return (0,0,0,0)
        else:
            return (0,0,0,0)

    @property
    def version_stripped(self):
        """File version (exe/dll), with leading and trailing zeros removed."""
        version = list(self.version)
        versionPop = version.pop
        while len(version) > 1 and version[0] == 0:
            versionPop(0)
        while len(version) > 1 and version[-1] == 0:
            versionPop()
        return tuple(version)

    @property
    def crc(self):
        """Calculates CRC for self."""
        size = _osPathGetsize(self._s)
        crc = 0
        with open(self._s,'rb') as ins:
            insRead = ins.read
            insTell = ins.tell
            while insTell() < size:
                crc = crc32(insRead(2097152),crc) # 2MB at a time
        return crc & 0xFFFFFFFF

    @property
    def exists(self):
        """File exists."""
        return _osPathExists(self._s)

    @property
    def isdir(self):
        """Path is a directory."""
        return _osPathIsdir(self._s)

    @property
    def isfile(self):
        """Path is a file."""
        return _osPathIsfile(self._s)

    @property
    def isabs(self):
        """Path is an absolute path."""
        return _osPathIsabs(self._s)

    @property
    def realpath(self):
        """Returns real path (follows symlinks, makes absolute path)"""
        return GPath(_osPathRealpath(self._s))

    #--Accessor functions --------------------------------------------------
    def crc_callback(self, callback):
        """Calculates CRC, but allows for a callback for UI feedback.
           callback should be a callable that will be called with how many
           bytes have been read in."""
        size = _osPathGetsize(self._s)
        crc = 0
        with open(self._s,'rb') as ins:
            insRead = ins.read
            insTell = ins.tell
            pos = 0
            while pos < size:
                crc = crc32(insRead(2097152),crc) # 2MB at a time
                pos = insTell()
                callback(pos)
        return crc & 0xFFFFFFFF

    def join(*args):
        """Joins self with path elements, using path seperators."""
        norms = map(getNorm,args)
        return GPath(_osPathJoin(*norms))

    def list(self):
        """Returns files/directories in this directory."""
        if not _osPathExists(self._s): return []
        return map(GPath,_osListdir(self._s))

    def walk(self,topdown=True,onerror=None,relative=False):
        """Like os.walk"""
        if relative:
            start = len(self._s)
            for root,dirs,files in _osWalk(self._s,topdown,onerror):
                yield (GPath(root[start:]),map(GPath,dirs),map(GPath,files))
        else:
            for root,dirs,files in _osWalk(self._s,topdown,onerror):
                yield (GPath(root),map(GPath,dirs),map(GPath,files))

    def split(self):
        """Splits the path along path seperators.
           IE: C:\Program Files\Bethesda Softworks
           returns ['C:','Program Files','Bethesda Softworks']"""
        dirs = []
        dirsAppend = dirs.append
        osPathSplit = _osPathSplit
        drive,path = _osPathSplitdrive(self._s)
        path = path.rstrip(_osPathSep+_osPathAltsep)
        l,r = osPathSplit(path)
        while l != '':
            dirsAppend(r)
            l,r = osPathSplit(l)
        dirsAppend(r)
        if drive != '':
            dirsAppend(drive)
        dirs.reverse()
        return dirs

    def relpath(self,path):
        """Returns self's relative path to path."""
        return GPath(_osPathRelpath(self._s,getNorm(path)))

    #--Helper functions
    @staticmethod
    def _onerror(func,path,exc_info):
        """shutil error handler: remove RO flag"""
        if not _osAccess(path,_osW_OK):
            _osChmod(path,_statFlags)
            func(path)
        else:
            raise

    def clearRO(self):
        """Clears RO flag on path (recursively for directories)."""
        if not _osPathIsdir(self._s):
            _osChmod(self._s,_statFlags)
        else:
            try:
                # First try using Windows' attrib command
                cmd = 'attrib -R "%s\\*" /S /D' % self._s
                _subprocessCall(cmd,stdout=_subprocessPIPE,
                                startupinfo=startupinfo)
            except UnicodeError:
                # Unicode filenames, could not be encoded to
                # pass to subprocess
                flags = _statFlags
                osChmod = _osChmod
                join = _osPathJoin
                for root,dirs,files in _osWalk(self._s):
                    for dir in dirs:
                        try: osChmod(join(root,dir),flags)
                        except: pass
                    for file in files:
                        try: osChmod(join(root,file),flags)
                        except: pass

    def open(self,*args,**kwdargs):
        """Open file for read/write, etc.  Accepts encoding argument."""
        if self._shead and not _osPathExists(self._shead):
            _osMakedirs(self._shead)
        if 'encoding' in kwdargs:
            return _codecsOpen(self._s,*args,**kwdargs)
        else:
            return open(self._s,*args,**kwdargs)

    def makedirs(self):
        """Create directories"""
        if not _osPathExists(self._s):
            _osMakedirs(self._s)

    def remove(self):
        """Remove file if exists."""
        try:
            if _osPathExists(self._s):
                _osRemove(self._s)
        except OSError:
            # Try clearing RO flag
            _osChmod(self._s,_statFlags)
            _osRemove(self._s)

    def removedirs(self):
        """Remove directories.  Must be empty."""
        try:
            if _osPathExists(self._s):
                _osRemovedirs(self._s)
        except OSError:
            self.clearRO()
            _osRemovedirs(self._s)

    def rmtree(self):
        """Removes directory and subdirectoris and files recursively."""
        if _osPathIsdir(self._s):
            _shutilRmtree(self._s,onerror=Path._onerror)

    def start(self, exeArgs=None):
        """Starts a file as if doubleclicked in explorer."""
        if self._cext == '.exe':
            if not exeArgs:
                _subprocessPopen([self._s],close_fds=True)
            else:
                _subprocessPopen(exeArgs,executable=self._s,close_fds=True)
        else:
            _osStartfile(self._s)

    def copyTo(self,dest):
        """Copies self to destination."""
        if _osPathIsdir(self._s):
            _shutilCopytree(self._s,getNorm(dest))
        else:
            dest = GPath(dest)
            if dest._shead and not _osPathExists(dest._shead):
                _osMakedirs(dest._shead)
            _shutilCopyfile(self._s,dest._s)
            _osUtime(dest._s,(_osPathGetatime(dest._s),_osPathGetmtime(self._s)))

    def moveTo(self,dest):
        """Moves file to destination."""
        dest = GPath(dest)
        if dest._cs == self._cs: return
        if dest._shead and not _osPathExists(dest._shead):
            _osMakedirs(dest._shead)
        elif _osPathExists(dest._s):
            _osRemove(dest._s)
        try:
            _shutilMove(self._s,dest._s)
        except OSError:
            self.clearRO()
            _shutilMove(self._s,dest._s)

    def tempMoveTo(self,dest):
        """Temporarily moves file to destination.  Use with the 'with' statement
           After leaving the 'with' statement, file will be moved back."""
        class temp(object):
            __slots__=('_oldpath','_temppath')
            def __init__(self,oldPath,tempPath):
                self._oldpath = oldPath
                self._temppath = tempPath
            def __enter__(self): return self._temppath
            def __exit__(self,*args,**kwdargs):
                self._temppath.moveTo(self._oldpath)
        self.moveTo(dest)
        return temp(self,dest)

    def touch(self):
        """Link unix 'touch'.  Creates file with current date/time."""
        if _osPathExists(self._s):
            _osUtime(self._s,(_osPathGetatime(self._s),_timeTime()))
        else:
            temp = self.temp
            with open(temp._s,'w'):
                pass
            temp.moveTo(self._s)

    def setcwd(self):
        """Set current working directory to self."""
        _osChdir(self._s)

class PathUnion(object):
    """A Path-like object for directories.  Minimal functions, just useful
       for specifying 2 or more search paths for files.

       MODE_ORDER - when searching for matches for join, the first occurance
           of an existing file is used.  When no match is found, the last
           directory of the union is used.
       MODE_TIMESTAMP - when searching for matches for join, the file with the
           newest timestamp is used.  When no match is found, the last
           directory of the union is uses.
       MODE_REVERSE - Modifies the above, so MODE_ORDER uses the last occurance,
           and no file uses the first directory of the union, MODE_TIMESTAMP
           uses the oldest file."""
    __slots__ = ('_dirs','_mode')

    MODE_ORDER = 1
    MODE_REVERSE = 2
    MODE_TIMESTAMP = 4

    def __init__(self,*names,mode=MODE_ORDER):
        self._dirs = [GPath(x) for x in names]
        self._mode = mode
        if mode & PathUnion.MODE_REVERSE:
            self._dirs.reverse()

    def __repr__(self):
        """Representaion of a PathUnion"""
        return 'PathUnion(%s)' % self._dirs

    def list(self):
        """Returns list of filenames/dirnames in this union."""
        items = set()
        for dirname in self._dirs:
            items |= set(dirname.list())
        return list(items)

    def join(self,*args):
        """Retrun Path object from joining directory with names.  How
           the true path is decided by creation mode."""
        norms = [getNorm(x) for x in args]
        if self._mode & PathUnion.MODE_TIMESTAMP:
            # Newest/oldest file returned
            if self._mode & PathUnion.MODE_REVERSE:
                def getmatch(old,new):
                    if not old: return new
                    if _osPathGetmtime(old) < _osPathGetmtime(new):
                        return old
                    return new
            else:
                def getmatch(old,new):
                    if not old: return new
                    if _osPathGetmtime(old) < _osPathGetmtime(new):
                        return new
                    return old
            match = None
            for dirname in self._dirs:
                full = _osPathJoin(dirname._s,*norms)
                if _osPathExists(full):
                    match = getmatch(match,full)
            if match:
                return GPath(match)
        else: # MODE_ORDER
            # First/last match returned
            for dirname in self._dirs:
                full = _osPathJoin(dirname._s,*norms)
                if _osPathExists(full):
                    return GPath(full)
        # None exist, use first directory to create
        return self._dirs[0].join(*norms)

# Initialize Shell Paths -------------------------------------------------------
def _shell_path(name):
    if not win32api: return None
    try:
        folder_id = getattr(shellcon,'CSIDL_'+name)
        return GPath(shell.SHGetFolderPath(0,folder_id,None,0))
    except AttributeError:
        return None

Desktop = _shell_path('DESKTOP')
AppData = _shell_path('APPDATA')
LocalAppData = _shell_path('LOCAL_APPDATA')
Favorites = _shell_path('FAVORITES')
StartMenu = _shell_path('STARTMENU')
Programs = _shell_path('PROGRAMS')
Startup = _shell_path('STARTUP')
Personal = _shell_path('PERSONAL')
Recent = _shell_path('RECENT')
SendTo = _shell_path('SENDTO')
