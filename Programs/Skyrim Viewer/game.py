# Utilities for getting information for the game:
# - Skyrim path
# - Skyrim AppData path
# - plugins.txt parsing
# - .STRINGS,.DLSTRINGS,.ILSTRINGS file reading
# - minimal .esp and .esm reading (just the TES4 record)
import re
import _winreg
from util.path import *
from util.binaryfile import *

reComment = re.compile('#.*')
reModExt = re.compile(r'\.es[mp](.ghost)?$',re.I)
reEsmExt = re.compile(r'\.esm(.ghost)?$',re.I)
reEspExt = re.compile(r'\.esp(.ghost)?$',re.I)

def GetAppDataPath(): return Path.getLocalAppData().join('Skyrim')
def GetSavesPath(): return Path.getPersonal().join('My Games','Skyrim')

def GetSkyrimPath():
    for hkey in (_winreg.HKEY_CURRENT_USER,_winreg.HKEY_LOCAL_MACHINE):
        for wow6432 in (None,'Wow6432Node\\'):
            for item in ('SteamPath','InstallPath'):
                try:
                    key = _winreg.OpenKey(
                        hkey,
                        'Software\\%sValve\\Steam' % wow6432
                        )
                except Exception as e:
                    continue
                try:
                    value = _winreg.QueryValueEx(
                        key,
                        item
                        )
                    if value[1] == _winreg.REG_SZ:
                        steamPath = GPath(value[0])
                        skyrimPath = steamPath.join('SteamApps','common','skyrim')
                        if skyrimPath.join('TESV.exe').exists:
                            return skyrimPath
                except Exception as e:
                    continue
                finally:
                    _winreg.CloseKey(key)
    return None

class Plugins(object):
    """Manages plugins.txt"""
    def __init__(self):
        # Plugins.txt
        self.file = GetAppDataPath().join('plugins.txt')
        self.mtime = 0
        self.size = 0
        # the plugins
        self.all = [] # all plugins, sorted by load order
        self.active = [] # active plugins, sorted by load order

    def refresh(self,dataPath):
        # Scan the Data directory
        self.all = (dataPath.join(x) for x in dataPath.list())
        self.all = (x for x in self.all if x.exists and not x.isdir)
        self.all = (x for x in self.all if reModExt.match(x.cext))
        self.all = sorted(self.all,key=lambda x: x.mtime)
        self.all = sorted(self.all,key=lambda x: reEspExt.match(x.cext))
        self.all = [x.tail for x in self.all]
        if GPath('Update.esm') in self.all and GPath('Skyrim.esm') in self.all:
            self.all.remove(GPath('Update.esm'))
            idex = self.all.index(GPath('Skyrim.esm'))
            self.all.insert(idex+1,GPath('Update.esm'))
        # Read the plugins.txt
        active = set()
        try:
            if (self.mtime != self.file.mtime or
                self.size != self.file.size):
                self.active = set()
                with self.file.open('r') as ins:
                    for line in ins:
                        modName = reComment.sub('',line).strip()
                        if not modName: continue
                        modName = GPath(modName)
                        if modName in self.active: continue
                        active.add(modName)
            self.active = [x for x in self.all if x in active]
        except:
            pass