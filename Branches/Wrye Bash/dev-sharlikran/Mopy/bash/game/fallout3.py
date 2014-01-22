# -*- coding: utf-8 -*-
#
# GPL License and Copyright Notice ============================================
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
#  Wrye Bash copyright (C) 2005-2009 Wrye, 2010-2011 Wrye Bash Team
#
# =============================================================================

"""This modules defines static data for use by bush, when
   Fallout 3 is set at the active game."""

# Imports ----------------------------------------------------------------------
import struct
from .. import brec
from .. import bolt
from .. import bush
from ..brec import *
from ..bolt import Flags, DataDict, StateError

# Util Constants ---------------------------------------------------------------
#--Null strings (for default empty byte arrays)
null1 = '\x00'
null2 = null1*2
null3 = null1*3
null4 = null1*4

# Mod Record Elements ----------------------------------------------------------
#-------------------------------------------------------------------------------
# Constants
FID = 'FID' #--Used by MelStruct classes to indicate fid elements.

#--Name of the game
name = u'Fallout 3'

#--Name of the game as used in related filenames and paths.
safeName = u'Fallout3'

#--Alternat display name of Wrye Bash when managing this game
altName = u'Wrye Flash'

#--Exe to look for to see if this is the right game
exe = u'Fallout3.exe'

#--Registry keys to read to find the install location
## These are relative to:
##  HKLM\Software
##  HKLM\Software\Wow6432Node
##  HKCU\Software
##  HKCU\Software\Wow6432Node
## Example: (u'Bethesda Softworks\\Oblivion',u'Installed Path')
regInstallKeys = [
    (u'Bethesda Softworks\\Fallout3',u'Installed Path')
    ]

#--Patch information
## URL to download patches for the main game.
patchURL = u''
## Tooltip to display over the URL when displayed
patchTip = u''

#--URL to the Nexus site for this game
nexusUrl = u'http://www.nexusmods.com/fallout3/'
nexusName = u'Fallout 3 Nexus'
nexusKey = u'bash.installers.openFallout3Nexus'
   
#--Construction Set information
class cs:
    shortName = u'GECK'                  # Abbreviated name
    longName = u'Garden of Eden Creation Kit'                   # Full name
    exe = u'GECK.exe'                   # Executable to run
    seArgs = u'-editor'                     # Argument to pass to the SE to load the CS
    imageName = u'geck%s.png'                  # Image name template for the status bar
    
#--Script Extender information
class se:
    shortName = u'FOSE'                      # Abbreviated name
    longName = u'Fallout 3 Script Extender'   # Full name
    exe = u'fose_loader.exe'                 # Exe to run
    steamExe = u'fose_loader.dll'           # Exe to run if a steam install
    url = u'http://fose.silverlock.org/'     # URL to download from
    urlTip = u'http://fose.silverlock.org/'  # Tooltip for mouse over the URL
    
#--Script Dragon
class sd:
    shortName = u''
    longName = u''
    installDir = u''
	
#--SkyProc Patchers
class sp:
    shortName = u''
    longName = u''
    installDir = u''

#--Quick shortcut for combining the SE and SD names
se_sd = u''

#--Graphics Extender information
class ge:
    shortName = u''
    longName = u''
    ## exe is treated specially here.  If it is a string, then it should
    ## be the path relative to the root directory of the game
    ## if it is list, each list element should be an iterable to pass to Path.join
    ## relative to the root directory of the game.  In this case, each filename
    ## will be tested in reverse order.  This was required for Oblivion, as the newer
    ## OBGE has a different filename than the older OBGE
    exe = u''
    url = u''
    urlTip = u''

#--4gb Launcher
class laa:
    name = u''           # Display name of the launcher
    exe = u'*DNE*'       # Executable to run
    launchesSE = False   # Whether the launcher will automatically launch the SE

# Files BAIN shouldn't skip
dontSkip = (
# Nothing so far
)

# Directories where specific file extensions should not be skipped by BAIN
dontSkipDirs = {
# Nothing so far
}

#--Folders BAIN should never CRC check in the Data directory
SkipBAINRefresh = set((
    # Use lowercase names
))

#--Some stuff dealing with INI files
class ini:
    #--True means new lines are allowed to be added via INI tweaks
    #  (by default)
    allowNewLines = False

    #--INI Entry to enable BSA Redirection
    bsaRedirection = (u'Archive',u'sArchiveList')

#--Save Game format stuff
# Requires header.pcNick and header.playTime to be added to SaveHeader in bosh.py.
class ess:
    # Save file capabilities
    canReadBasic = True        # Can read the info needed for the Save Tab display
    canEditMasters = True      # Can adjust save file masters
    canEditMore = False         # Advanced editing
    
    # Save file extension.
    ext = u'.fos';

    @staticmethod
    def load(ins,header):
        """Extract basic info from save file.close
           At a minimum, this should set the following
           attrubutes in 'header':
            pcName
            pcLevel
            pcLocation
            gameDate
            gameDays
            gameTicks (seconds*1000)
            image (ssWidth,ssHeight,ssData)
            masters
        """
        if ins.read(11) != 'FO3SAVEGAME':
            raise Exception(u'Save file is not a Fallout New Vegas save game.')
        headerSize, = struct.unpack('I',ins.read(4))
        unknown,delim = struct.unpack('Ic',ins.read(5))
        ssWidth,delim1,ssHeight,delim2,ssDepth,delim3 = struct.unpack('=IcIcIc',ins.read(15))
        #--Name, nickname, level, location, playtime
        size,delim = struct.unpack('Hc',ins.read(3))
        header.pcName = ins.read(size)
        delim, = struct.unpack('c',ins.read(1))
        size,delim = struct.unpack('Hc',ins.read(3))
        header.pcNick = ins.read(size)
        delim, = struct.unpack('c',ins.read(1))
        header.pcLevel,delim = struct.unpack('Ic',ins.read(5))
        size,delim = struct.unpack('Hc',ins.read(3))
        header.pcLocation = ins.read(size)
        delim, = struct.unpack('c',ins.read(1))
        size,delim = struct.unpack('Hc',ins.read(3))
        header.playTime = ins.read(size)
        delim, = struct.unpack('c',ins.read(1))
        #--Image Data
        ssData = ins.read(3*ssWidth*ssHeight)
        header.image = (ssWidth,ssHeight,ssData)
        #--Masters
        unknown,masterListSize = struct.unpack('=BI',ins.read(5))
        if unknown != 0x15:
            raise Exception(u'%s: Unknown byte is not 0x15.' % path)
        del header.masters[:]
        numMasters,delim = struct.unpack('Bc',ins.read(2))
        for count in range(numMasters):
            size,delim = struct.unpack('Hc',ins.read(3))
            header.masters.append(ins.read(size))
            delim, = struct.unpack('c',ins.read(1))
        

    @staticmethod
    def writeMasters(ins,out,header):
        """Rewrites masters of existing save file."""
        def unpack(format,size):
            return struct.unpack(format,ins.read(size))
        def pack(format,*args):
            out.write(struct.pack(format,*args))
        #--Header
        out.write(ins.read(11))
        #--SaveGameHeader
        size, = unpack('I',4)
        pack('I',size)
        out.write(ins.read(5))
        ssWidth,delim1,ssHeight,delim2 = unpack('=IcIc',10)
        pack('=IcIc',ssWidth,delim1,ssHeight,delim2)
        out.write(ins.read(size-15))
        #--Image Data
        out.write(ins.read(3*ssWidth*ssHeight))
        #--Skip old masters
        unknown,oldMasterListSize = unpack('=BI',5)
        if unknown != 0x15:
            raise Exception(u'%s: Unknown byte is not 0x15.' % path)
        numMasters,delim = unpack('Bc',2)
        oldMasters = []
        for count in range(numMasters):
            size,delim = unpack('Hc',3)
            oldMasters.append(ins.read(size))
            delim, = unpack('c',1)
        #--Write new masters
        newMasterListSize = 2 + (4 * len(header.masters))
        for master in header.masters:
            newMasterListSize += len(master)
        pack('=BI',unknown,newMasterListSize)
        pack('Bc',len(header.masters),'|')
        for master in header.masters:
            pack('Hc',len(master),'|')
            out.write(master.s)
            pack('c','|')
        #--Fids Address
        offset = out.tell() - ins.tell()
        fidsAddress, = unpack('I',4)
        pack('I',fidsAddress+offset)
        #--???? Address
        unknownAddress, = unpack('I',4)
        pack('I',unknownAddress+offset)
        #--???? Address
        unknownAddress, = unpack('I',4)
        pack('I',unknownAddress+offset)
        #--???? Address
        unknownAddress, = unpack('I',4)
        pack('I',unknownAddress+offset)
        #--???? Address
        unknownAddress, = unpack('I',4)
        pack('I',unknownAddress+offset)
        #--Copy remainder
        while True:
            buffer= ins.read(0x5000000)
            if not buffer: break
            out.write(buffer)
        return oldMasters

#--The main plugin Wrye Bash should look for
masterFiles = [
    u'Fallout3.esm',
    ]

#--INI files that should show up in the INI Edits tab
iniFiles = [
    u'FalloutPrefs.ini',
    ]

#--Name of the default ini file.
defaultIniFile = u'Fallout_default.ini'
    
#-- INI setting used to setup Save Profiles
## (section,key)
saveProfilesKey = (u'General',u'SLocalSavePath')

#--Game ESM/ESP/BSA files
bethDataFiles = set((
    #--Vanilla
    ur'fallout3.esm',
    ur'fallout - menuvoices.bsa',
    ur'fallout - meshes.bsa',
    ur'fallout - misc.bsa',
    ur'fallout - sound.bsa',
    ur'fallout - textures.bsa',
    ur'fallout - voices.bsa',
    #-- DLC
    ur'anchorage.esm',
    ur'anchorage - main.bsa',
    ur'anchorage - sounds.bsa',
    ur'thepitt.esm',
    ur'thepitt - main.bsa',
    ur'thepitt - sounds.bsa',
    ur'brokensteel.esm',
    ur'brokensteel - main.bsa',
    ur'brokensteel - sounds.bsa',
    ur'pointlookout.esm',
    ur'pointlookout - main.bsa',
    ur'pointlookout - sounds.bsa',
    ur'zeta.esm',
    ur'zeta - main.bsa',
    ur'zeta - sounds.bsa',
    ))

#--Every file in the Data directory from Bethsoft
allBethFiles = set((
    #vanilla
    ur'Credits.txt',
    ur'CreditsWacky.txt',
    ur'Fallout3.esm',
    ur'Fallout - MenuVoices.bsa',
    ur'Fallout - Meshes.bsa',
    ur'Fallout - Misc.bsa',
    ur'Fallout - Sound.bsa',
    ur'Fallout - Textures.bsa',
    ur'Fallout - Voices.bsa',
    ur'LODSettings\aaaForgotten1.DLODSettings',
    ur'LODSettings\aaaForgotten4.DLODSettings',
    ur'LODSettings\aaaForgotten5.DLODSettings',
    ur'Music\Base\Base_01.mp3',
    ur'Music\Base\Base_02.mp3',
    ur'Music\Base\Base_03.mp3',
    ur'Music\Base\Base_04.mp3',
    ur'Music\Battle\Battle_01.mp3',
    ur'Music\Battle\Battle_02.mp3',
    ur'Music\Battle\Battle_03.mp3',
    ur'Music\Battle\Battle_04.mp3',
    ur'Music\Battle\Battle_05.mp3',
    ur'Music\Battle\Battle_06.mp3',
    ur'Music\Battle\Battle_07.mp3',
    ur'Music\Battle\Finale\Battle_01.mp3',
    ur'Music\Battle\Finale\Battle_02.mp3',
    ur'Music\Battle\Finale\Battle_03.mp3',
    ur'Music\Battle\Finale\Battle_04.mp3',
    ur'Music\Battle\Finale\Battle_05.mp3',
    ur'Music\Battle\Finale\Battle_06.mp3',
    ur'Music\Battle\Finale\Battle_07.mp3',
    ur'Music\Dungeon\Dungeon_01.mp3',
    ur'Music\Dungeon\Dungeon_02.mp3',
    ur'Music\Dungeon\Dungeon_03.mp3',
    ur'Music\Dungeon\Dungeon_04.mp3',
    ur'Music\Dungeon\Dungeon_05.mp3',
    ur'Music\Endgame\Endgame_01.mp3',
    ur'Music\Endgame\Endgame_02.mp3',
    ur'Music\Endgame\Endgame_03.mp3',
    ur'Music\Endgame\Endgame_04.mp3',
    ur'Music\Endgame\Endgame_05.mp3',
    ur'Music\Endgame\Endgame_06.mp3',
    ur'Music\Endgame\Endgame_07.mp3',
    ur'Music\Endgame\Endgame_08.mp3',
    ur'Music\Endgame\Endgame_09.mp3',
    ur'Music\Endgame\Endgame_11.mp3',
    ur'Music\Endgame\Endgame_12.mp3',
    ur'Music\Endgame\Endgame_14.mp3',
    ur'Music\Endgame\Endgame_15.mp3',
    ur'Music\Endgame\Endgame_17.mp3',
    ur'Music\Endgame\Endgame_18.mp3',
    ur'Music\Endgame\Endgame_19.mp3',
    ur'Music\Explore\Explore_01.mp3',
    ur'Music\Explore\Explore_02.mp3',
    ur'Music\Explore\Explore_03.mp3',
    ur'Music\Explore\Explore_04.mp3',
    ur'Music\Explore\Explore_05.mp3',
    ur'Music\Explore\Explore_06.mp3',
    ur'Music\Explore\Explore_07.mp3',
    ur'Music\Public\Public_01.mp3',
    ur'Music\Public\Public_02.mp3',
    ur'Music\Public\Public_03.mp3',
    ur'Music\Public\Public_04.mp3',
    ur'Music\Public\Public_05.mp3',
    ur'Music\Special\Death.mp3',
    ur'Music\Special\ExitTheVault.mp3',
    ur'Music\Special\MainTitle.mp3',
    ur'Music\Special\Success.mp3',
    ur'Music\Tension\Tension_01.mp3',
    ur'Music\TranquilityLane\MUS_TranquilityLane_01_LP.mp3',
    ur'Music\TranquilityLane\MUS_TranquilityLane_02_LP.mp3',
    ur'Shaders\shaderpackage002.sdp',
    ur'Shaders\shaderpackage003.sdp',
    ur'Shaders\shaderpackage004.sdp',
    ur'Shaders\shaderpackage006.sdp',
    ur'Shaders\shaderpackage007.sdp',
    ur'Shaders\shaderpackage009.sdp',
    ur'Shaders\shaderpackage010.sdp',
    ur'Shaders\shaderpackage011.sdp',
    ur'Shaders\shaderpackage012.sdp',
    ur'Shaders\shaderpackage013.sdp',
    ur'Shaders\shaderpackage014.sdp',
    ur'Shaders\shaderpackage015.sdp',
    ur'Shaders\shaderpackage016.sdp',
    ur'Shaders\shaderpackage017.sdp',
    ur'Shaders\shaderpackage018.sdp',
    ur'Shaders\shaderpackage019.sdp',
    ur'Video\1 year later.bik',
    ur'Video\2 weeks later.bik',
    ur'Video\3 years later.bik',
    ur'Video\6 years later.bik',
    ur'Video\9 years later.bik',
    ur'Video\B01.bik',
    ur'Video\B02.bik',
    ur'Video\B03.bik',
    ur'Video\B04.bik',
    ur'Video\B05.bik',
    ur'Video\B06.bik',
    ur'Video\B07.bik',
    ur'Video\B08.bik',
    ur'Video\B09.bik',
    ur'Video\B10.bik',
    ur'Video\B11.bik',
    ur'Video\B12.bik',
    ur'Video\B13.bik',
    ur'Video\B14.bik',
    ur'Video\B15.bik',
    ur'Video\B16.bik',
    ur'Video\B17.bik',
    ur'Video\B18.bik',
    ur'Video\B19.bik',
    ur'Video\B20.bik',
    ur'Video\B21.bik',
    ur'Video\B22.bik',
    ur'Video\B23.bik',
    ur'Video\B24.bik',
    ur'Video\B25.bik',
    ur'Video\B26.bik',
    ur'Video\B27.bik',
    ur'Video\B28.bik',
    ur'Video\B29.bik',
    ur'Video\Fallout INTRO Vsk.bik',
    #DLCs
    ur'anchorage.esm',
    ur'anchorage - main.bsa',
    ur'anchorage - sounds.bsa',
    ur'thepitt.esm',
    ur'thepitt - main.bsa',
    ur'thepitt - sounds.bsa',
    ur'brokensteel.esm',
    ur'brokensteel - main.bsa',
    ur'brokensteel - sounds.bsa',
    ur'pointlookout.esm',
    ur'pointlookout - main.bsa',
    ur'pointlookout - sounds.bsa',
    ur'zeta.esm',
    ur'zeta - main.bsa',
    ur'zeta - sounds.bsa',
    ur'DLCList.txt',
    ))

#--Plugin files that can't be deactivated
nonDeactivatableFiles = []

#--BAIN:
## These are the allowed default data directories that BAIN can install to
dataDirs = set((
    'bash patches',
    'distantlod',
    'docs',
    'facegen',
    'fonts',
    'menus',
    'meshes',
    'music',
    'shaders',
    'sound', 
    'textures', 
    'trees',
    'video'
    ))
## These are additional special directories that BAIN can install to
dataDirsPlus = set((
    u'streamline',
    u'_tejon',
    u'ini tweaks',
    u'scripts',
    u'pluggy',
    u'ini',
    u'fose'))  
    
# Installer -------------------------------------------------------------------
# ensure all path strings are prefixed with 'r' to avoid interpretation of
#   accidental escape sequences
wryeBashDataFiles = set((
    ur'Bashed Patch.esp',
    ur'Bashed Patch, 0.esp',
    ur'Bashed Patch, 1.esp',
    ur'Bashed Patch, 2.esp',
    ur'Bashed Patch, 3.esp',
    ur'Bashed Patch, 4.esp',
    ur'Bashed Patch, 5.esp',
    ur'Bashed Patch, 6.esp',
    ur'Bashed Patch, 7.esp',
    ur'Bashed Patch, 8.esp',
    ur'Bashed Patch, 9.esp',
    ur'Bashed Patch, CBash.esp',
    ur'Bashed Patch, Python.esp',
    ur'Bashed Patch, FCOM.esp',
    ur'Bashed Patch, Warrior.esp',
    ur'Bashed Patch, Thief.esp',
    ur'Bashed Patch, Mage.esp',
    ur'Bashed Patch, Test.esp',
    ur'ArchiveInvalidationInvalidated!.bsa'
    ur'Fallout - AI!.bsa'
    ))
wryeBashDataDirs = set((
    ur'Bash Patches',
    ur'INI Tweaks'
    ))
ignoreDataFiles = set((
#    ur'FOSE\Plugins\Construction Set Extender.dll',
#    ur'FOSE\Plugins\Construction Set Extender.ini'
    ))
ignoreDataFilePrefixes = set(())
ignoreDataDirs = set((
#    r'FOSE\Plugins\ComponentDLLs\CSE',
    ur'LSData'
    ))    
  
#--Plugin format stuff
class esp:
    #--Wrye Bash capabilities
    canBash = False         # Can create Bashed Patches
    canCBash = False         # CBash can handle this game's records
    canEditHeader = True   # Can edit basic info in the TES4 record
    
    #--Valid ESM/ESP header versions
    ## These are the valid 'version' numbers for the game file headers
    validHeaderVersions = (0.85,0.94)

    stringsFiles = []

    #--Class to use to read the TES4 record
    ## This is the class name in bosh.py to use for the TES4 record when reading
    ## Example: 'MreTes4'
    tes4ClassName = ''

    #--Information about the basic record header
    class header:
        format = ''         # Format passed to struct.unpack to unpack the header
        size = 0            # Size of the record header
        attrs = tuple()     # List of attributes to set = the return of struct.unpack
        defaults = tuple()  # Default values for each of the above attributes
    
    #--Top types in Fallout3 order.
    topTypes = ['GMST', 'TXST', 'MICN', 'GLOB', 'CLAS', 'FACT', 'HDPT', 'HAIR', 'EYES',
        'RACE', 'SOUN', 'ASPC', 'MGEF', 'SCPT', 'LTEX', 'ENCH', 'SPEL', 'ACTI', 'TACT',
        'TERM', 'ARMO', 'BOOK', 'CONT', 'DOOR', 'INGR', 'LIGH', 'MISC', 'STAT', 'SCOL',
        'MSTT', 'PWAT', 'GRAS', 'TREE', 'FURN', 'WEAP', 'AMMO', 'NPC_', 'CREA', 'LVLC',
        'LVLN', 'KEYM', 'ALCH', 'IDLM', 'NOTE', 'COBJ', 'PROJ', 'LVLI', 'WTHR', 'CLMT',
        'REGN', 'NAVI', 'CELL', 'WRLD', 'DIAL', 'QUST', 'IDLE', 'PACK', 'CSTY', 'LSCR',
        'ANIO', 'WATR', 'EFSH', 'EXPL', 'DEBR', 'IMGS', 'IMAD', 'FLST', 'PERK', 'BPTD',
        'ADDN', 'AVIF', 'RADS', 'CAMS', 'CPTH', 'VTYP', 'IPCT', 'IPDS', 'ARMA', 'ECZN',
        'MESG', 'RGDL', 'DOBJ', 'LGTM', 'MUSC',
        # Unused types in fallout3. (dummy)
        'SLGM', 'BSGN', 'FLOR', 'SGST', 'CLOT', 'SBSP', 'SKIL', 'LVSP', 'APPA',
        ]
        
    #--Dict mapping 'ignored' top types to un-ignored top types
    topIgTopTYpes = dict()
        
    recordTypes = set(topTypes + 'GRUP,TES4,ROAD,REFR,ACHR,ACRE,PGRD,LAND,INFO,PGRE,NAVM'.split(','))
     
class RecordHeader(brec.BaseRecordHeader):
    size = 24 # Size in bytes of a record header

    def __init__(self,recType='TES4',size=0,arg1=0,arg2=0,arg3=0,*extra):
        self.recType = recType
        self.size = size
        if recType == 'GRUP':
            self.label = arg1
            self.groupType = arg2
            self.stamp = arg3
        else:
            self.flags1 = arg1
            self.fid = arg2
            self.flags2 = arg2
        self.extra = extra

    @staticmethod
    def unpack(ins):
        """Returns a RecordHeader object by reading the input stream."""
        type,size,uint0,uint1,uint2,uint3 = ins.unpack('4s5I',24,'REC_HEAD')
        #--Bad?
        if type not in esp.recordTypes:
            raise brec.ModError(ins.inName,u'Bad header type: '+repr(type))
        #--Record
        if type != 'GRUP':
            pass
        #--Top Group
        elif uint1 == 0: # groupType == 0 (Top Group)
            str0 = struct.pack('I',uint0)
            if str0 in esp.topTypes:
                uint0 = str0
            elif str0 in esp.topIgTypes:
                uint0 = esp.topIgTypes[str0]
            else:
                raise brec.ModError(ins.inName,u'Bad Top GRUP type: '+repr(str0))
        return RecordHeader(type,size,uint0,uint1,uint2,uint3)

    def pack(self):
        """Returns the record header packed into a string for writing to file"""
        if self.recType == 'GRUP':
            if isinstance(self.label,str):
                return struct.pack('=4sI4sIII',self.recType,self.size,self.label,self.groupType,self.stamp,self.stamp2)
            elif isinstance(self.label,tuple):
                return struct.pack('=4sIhhIII',self.recType,self.size,self.label[0],self.label[1],self.groupType,self.stamp,self.stamp2)
            else:
                return struct.pack('=4s5I',self.recType,self.size,self.label,self.groupType,self.stamp,self.stamp2)
        else:
            return struct.pack('=4s5I',self.recType,self.size,self.flags1,self.fid,self.flags2,self.flags3)
	
#--The pickle file for this game.  Holds encoded GMST IDs from the big list below
pklfile = ur'bash\db\Fallout3_ids.pkl'

#--List of GMST's in the main plugin (Oblivion.esm) that have 0x00000000
#  as the form id.  Any GMST as such needs it Editor Id listed here.
gmstEids = ['fPlayerDeathReloadTime','iMapMarkerVisibleDistance','fVanityModeWheelMax','fChase3rdPersonZUnitsPerSecond',
    'fAutoAimMaxDegreesMiss','iHoursToRespawnCell','fEssentialDeathTime','fJumpHeightMin','fPlayerPipBoyLightTimer',
    'iAINumberActorsComplexScene','iHackingMaxWords','fGunShellLifetime','fGunShellCameraDistance','fGunDecalCameraDistance',
    'iDebrisMaxCount','iHackingDumpRate','iHackingInputRate','iHackingOutputRate','iHackingFlashOffDuration',
    'iHackingFlashOnDuration','iComputersDisplayRateMenus','iComputersDisplayRateNotes','iInventoryAskQuantityAt',
    'iNumberActorsInCombatPlayer','iNumberActorsAllowedToFollowPlayer','iRemoveExcessDeadCount','iRemoveExcessDeadTotalActorCount',
    'iRemoveExcessDeadComplexTotalActorCount','iRemoveExcessDeadComplexCount', 'fRemoveExcessDeadTime','fRemoveExcessComplexDeadTime',
    'iLevItemLevelDifferenceMax','fMoveWeightMax',
    ]

#--Bash Tags supported by this game
allTags = sorted(('Body-F', 'Body-M', 'Body-Size-M', 'Body-Size-F', 'C.Climate', 'C.Light', 'C.Music', 'C.Name', 'C.RecordFlags',
                  'C.Owner', 'C.Water','Deactivate', 'Delev', 'Eyes', 'Factions', 'Relations', 'Filter', 'Graphics', 'Hair',
                  'IIM', 'Invent', 'Names', 'NoMerge', 'NpcFaces', 'R.Relations', 'Relev', 'Scripts', 'ScriptContents', 'Sound',
                  'Stats', 'Voice-F', 'Voice-M', 'R.Teeth', 'R.Mouth', 'R.Ears', 'R.Head', 'R.Attributes-F',
                  'R.Attributes-M', 'R.Skills', 'R.Description', 'Roads', 'Actors.Anims',
                  'Actors.AIData', 'Actors.DeathItem', 'Actors.AIPackages', 'Actors.AIPackagesForceAdd', 'Actors.Stats',
                  'Actors.ACBS', 'NPC.Class', 'Actors.CombatStyle', 'Creatures.Blood',
                  'NPC.Race','Actors.Skeleton', 'NpcFacesForceFullImport', 'MustBeActiveIfImported',
                  'Deflst', 'Destructible'))

#--GLOB record tweaks used by bosh's GmstTweaker
#  Each entry is a tuple in the following format:
#    (DisplayText, MouseoverText, GLOB EditorID, Option1, Option2, Option3, ..., OptionN)
#    -EditorID can be a plain string, or a tuple of multiple Editor IDs.  If it's a tuple,
#     then Value (below) must be a tuple of equal length, providing values for each GLOB
#  Each Option is a tuple:
#    (DisplayText, Value)
#    - If you enclose DisplayText in brackets like this: _(u'[Default]'), then the patcher
#      will treat this option as the default value.
#    - If you use _(u'Custom') as the entry, the patcher will bring up a number input dialog
#  To make a tweak Enabled by Default, enclose the tuple entry for the tweak in a list, and make
#  a dictionary as the second list item with {'defaultEnabled':True}.  See the UOP Vampire face
#  fix for an example of this (in the GMST Tweaks)
## NOTE: only required if the GmstTweaker has been enabled for this game
GlobalsTweaks = [
]

#--GMST record tweaks used by bosh's GmstTweaker
#  Each entry is a tuple in the following format:
#    (DisplayText, MouseoverText, GMST EditorID, Option1, Option2, Option3, ..., OptionN)
#    -EditorID can be a plain string, or a tuple of multiple Editor IDs.  If it's a tuple,
#     then Value (below) must be a tuple of equal length, providing values for each GMST
#  Each Option is a tuple:
#    (DisplayText, Value)
#    - If you enclose DisplayText in brackets like this: _(u'[Default]'), then the patcher
#      will treat this option as the default value.
#    - If you use _(u'Custom') as the entry, the patcher will bring up a number input dialog
#  To make a tweak Enabled by Default, enclose the tuple entry for the tweak in a list, and make
#  a dictionary as the second list item with {'defaultEnabled':True}.  See the UOP Vampire face
#  fix for an example of this (in the GMST Tweaks)
## NOTE: only required if the GmstTweaker has been enabled for this game
GmstTweaks = [
]

#--Patcher available when building a Bashed Patch (refrerenced by class name)
patchers = (
)

#--For ListMerger patcher (leveled list patcher)
listTypes = ()

#--CBash patchers available when building a Bashed Patch
CBash_patchers = (
)
# Function Info ---------------------------------------------------------------
conditionFunctionData = ( #--0: no param; 1: int param; 2: formid param
    (153, 'CanHaveFlames', 0, 0, 0, 0),
    (127, 'HasBeenEatan', 0, 0, 0, 0),
    ( 14, 'GetActorValue', 1, 0, 0, 0),
    ( 61, 'GetAlarmed', 0, 0, 0, 0),
    (190, 'GetAmountSoldStolen', 0, 0, 0, 0),
    (  8, 'GetAngle', 1, 0, 0, 0),
    ( 81, 'GetArmorRating', 0, 0, 0, 0),
    (274, 'GetArmorRatingUpperBody', 0, 0, 0, 0),
    ( 63, 'GetAttacked', 0, 0, 0, 0),
    (264, 'GetBarterGold', 0, 0, 0, 0),
    (277, 'GetBaseActorValue', 1, 0, 0, 0),
    (229, 'GetClassDefaultMatch', 0, 0, 0, 0),
    ( 41, 'GetClothingValue', 0, 0, 0, 0),
    (122, 'GetCrime', 2, 1, 0, 0),
    (116, 'GetMinorCrimeCount', 0, 0, 0, 0),
    (110, 'GetCurrentAIPackage', 0, 0, 0, 0),
    (143, 'GetCurrentAIProcedure', 0, 0, 0, 0),
    ( 18, 'GetCurrentTime', 0, 0, 0, 0),
    (148, 'GetCurrentWeatherPercent', 0, 0, 0, 0),
    (170, 'GetDayOfWeek', 0, 0, 0, 0),
    ( 46, 'GetDead', 0, 0, 0, 0),
    ( 84, 'GetDeadCount', 2, 0, 0, 0),
    (203, 'GetDestroyed', 0, 0, 0, 0),
    ( 45, 'GetDetected', 2, 0, 0, 0),
    (180, 'GetDetectionLevel', 2, 0, 0, 0),
    ( 35, 'GetDisabled', 0, 0, 0, 0),
    ( 39, 'GetDisease', 0, 0, 0, 0),
    ( 76, 'GetDisposition', 2, 0, 0, 0),
    (  1, 'GetDistance', 2, 0, 0, 0),
    (215, 'GetDefaultOpen', 0, 0, 0, 0),
    (182, 'GetEquipped', 2, 0, 0, 0),
    ( 73, 'GetFactionRank', 2, 0, 0, 0),
    ( 60, 'GetFactionRankDifference', 2, 2, 0, 0),
    (128, 'GetFatiguePercentage', 0, 0, 0, 0),
    (288, 'GetFriendHit', 2, 0, 0, 0),
    (160, 'GetFurnitureMarkerID', 0, 0, 0, 0),
    ( 74, 'GetGlobalValue', 2, 0, 0, 0),
    ( 48, 'GetGold', 0, 0, 0, 0),
    ( 99, 'GetHeadingAngle', 2, 0, 0, 0),
    (318, 'GetIdleDoneOnce', 0, 0, 0, 0),
    (338, 'GetIgnoreFriendlyHits', 0, 0, 0, 0),
    ( 67, 'GetInCell', 2, 0, 0, 0),
    (230, 'GetInCellParam', 2, 2, 0, 0),
    ( 71, 'GetInFaction', 2, 0, 0, 0),
    ( 32, 'GetInSameCell', 2, 0, 0, 0),
    (310, 'GetInWorldspace', 2, 0, 0, 0),
    ( 91, 'GetIsAlerted', 0, 0, 0, 0),
    ( 68, 'GetIsClass', 2, 0, 0, 0),
    (228, 'GetIsClassDefault', 2, 0, 0, 0),
    ( 64, 'GetIsCreature', 0, 0, 0, 0),
    (161, 'GetIsCurrentPackage', 2, 0, 0, 0),
    (149, 'GetIsCurrentWeather', 2, 0, 0, 0),
    (237, 'GetIsGhost', 0, 0, 0, 0),
    ( 72, 'GetIsID', 2, 0, 0, 0),
    (254, 'GetIsPlayableRace', 0, 0, 0, 0),
    (224, 'GetVATSMode', 0, 0, 0, 0),
    ( 69, 'GetIsRace', 2, 0, 0, 0),
    (136, 'GetIsReference', 2, 0, 0, 0),
    ( 70, 'GetIsSex', 1, 0, 0, 0),
    (246, 'GetIsUsedItem', 2, 0, 0, 0),
    (247, 'GetIsUsedItemType', 1, 0, 0, 0),
    ( 47, 'GetItemCount', 2, 0, 0, 0),
    (107, 'GetKnockedState', 0, 0, 0, 0),
    ( 80, 'GetLevel', 0, 0, 0, 0),
    ( 27, 'GetLineOfSight', 2, 0, 0, 0),
    (  5, 'GetLocked', 0, 0, 0, 0),
    ( 65, 'GetLockLevel', 0, 0, 0, 0),
    (320, 'GetNoRumors', 0, 0, 0, 0),
    (255, 'GetOffersServicesNow', 0, 0, 0, 0),
    (157, 'GetOpenState', 0, 0, 0, 0),
    (193, 'GetPCExpelled', 2, 0, 0, 0),
    (199, 'GetPCFactionAttack', 2, 0, 0, 0),
    (195, 'GetPCFactionMurder', 2, 0, 0, 0),
    (197, 'GetPCEnemyofFaction', 2, 0, 0, 0),
    (132, 'GetPCInFaction', 2, 0, 0, 0),
    (129, 'GetPCIsClass', 2, 0, 0, 0),
    (130, 'GetPCIsRace', 2, 0, 0, 0),
    (131, 'GetPCIsSex', 1, 0, 0, 0),
    (312, 'GetPCMiscStat', 1, 0, 0, 0),
    (225, 'GetPersuasionNumber', 0, 0, 0, 0),
    ( 98, 'GetPlayerControlsDisabled', 0, 0, 0, 0),
    (365, 'IsChild', 0, 0, 0, 0),
    (362, 'GetPlayerHasLastRiddenHorse', 0, 0, 0, 0),
    (  6, 'GetPos', 1, 0, 0, 0),
    ( 56, 'GetQuestRunning', 2, 0, 0, 0),
    ( 79, 'GetQuestVariable', 2, 1, 0, 0),
    ( 77, 'GetRandomPercent', 0, 0, 0, 0),
    (244, 'GetRestrained', 0, 0, 0, 0),
    ( 24, 'GetScale', 0, 0, 0, 0),
    ( 53, 'GetScriptVariable', 2, 1, 0, 0),
    ( 12, 'GetSecondsPassed', 0, 0, 0, 0),
    ( 66, 'GetShouldAttack', 2, 0, 0, 0),
    (159, 'GetSitting', 0, 0, 0, 0),
    ( 49, 'GetSleeping', 0, 0, 0, 0),
    ( 58, 'GetStage', 2, 0, 0, 0),
    ( 59, 'GetStageDone', 2, 1, 0, 0),
    ( 11, 'GetStartingAngle', 1, 0, 0, 0),
    ( 10, 'GetStartingPos', 1, 0, 0, 0),
    ( 50, 'GetTalkedToPC', 0, 0, 0, 0),
    (172, 'GetTalkedToPCParam', 2, 0, 0, 0),
    (361, 'GetTimeDead', 0, 0, 0, 0),
    (315, 'GetTotalPersuasionNumber', 0, 0, 0, 0),
    (144, 'GetTrespassWarningLevel', 0, 0, 0, 0),
    (242, 'GetUnconscious', 0, 0, 0, 0),
    (259, 'GetUsedItemActivate', 0, 0, 0, 0),
    (258, 'GetUsedItemLevel', 0, 0, 0, 0),
    ( 40, 'GetVampire', 0, 0, 0, 0),
    (142, 'GetWalkSpeed', 0, 0, 0, 0),
    (108, 'GetWeaponAnimType', 0, 0, 0, 0),
    (109, 'IsWeaponSkillType', 1, 0, 0, 0),
    (147, 'GetWindSpeed', 0, 0, 0, 0),
    (154, 'HasFlames', 0, 0, 0, 0),
    (214, 'HasMagicEffect', 2, 0, 0, 0),
    (227, 'HasCannibal', 0, 0, 0, 0),
    (353, 'IsActor', 0, 0, 0, 0),
    (314, 'IsActorAVictim', 0, 0, 0, 0),
    (313, 'IsActorEvil', 0, 0, 0, 0),
    (306, 'IsActorUsingATorch', 0, 0, 0, 0),
    (280, 'IsCellOwner', 2, 2, 0, 0),
    (267, 'IsCloudy', 0, 0, 0, 0),
    (150, 'IsContinuingPackagePCNear', 0, 0, 0, 0),
    (163, 'IsCurrentFurnitureObj', 2, 0, 0, 0),
    (162, 'IsCurrentFurnitureRef', 2, 0, 0, 0),
    (354, 'IsEssential', 0, 0, 0, 0),
    (106, 'IsFacingUp', 0, 0, 0, 0),
    (125, 'IsGuard', 0, 0, 0, 0),
    (282, 'IsHorseStolen', 0, 0, 0, 0),
    (112, 'IsIdlePlaying', 0, 0, 0, 0),
    (289, 'IsInCombat', 0, 0, 0, 0),
    (332, 'IsInDangerousWater', 0, 0, 0, 0),
    (300, 'IsInInterior', 0, 0, 0, 0),
    (146, 'IsInMyOwnedCell', 0, 0, 0, 0),
    (285, 'IsLeftUp', 0, 0, 0, 0),
    (278, 'IsOwner', 2, 0, 0, 0),
    (176, 'IsPCAMurderer', 0, 0, 0, 0),
    (175, 'IsPCSleeping', 0, 0, 0, 0),
    (358, 'IsPlayerMovingIntoNewSpace', 0, 0, 0, 0),
    (339, 'IsPlayersLastRiddenHorse', 0, 0, 0, 0),
    (266, 'IsPleasant', 0, 0, 0, 0),
    ( 62, 'IsRaining', 0, 0, 0, 0),
    (327, 'IsRidingHorse', 0, 0, 0, 0),
    (287, 'IsRunning', 0, 0, 0, 0),
    (103, 'IsShieldOut', 0, 0, 0, 0),
    (286, 'IsSneaking', 0, 0, 0, 0),
    ( 75, 'IsSnowing', 0, 0, 0, 0),
    (223, 'IsSpellTarget', 2, 0, 0, 0),
    (185, 'IsSwimming', 0, 0, 0, 0),
    (141, 'IsTalking', 0, 0, 0, 0),
    (265, 'IsTimePassing', 0, 0, 0, 0),
    (102, 'IsTorchOut', 0, 0, 0, 0),
    (145, 'IsTrespassing', 0, 0, 0, 0),
    (111, 'IsWaiting', 0, 0, 0, 0),
    (101, 'IsWeaponOut', 0, 0, 0, 0),
    (309, 'IsXBox', 0, 0, 0, 0),
    ( 36, 'MenuMode', 1, 0, 0, 0),
    ( 42, 'SameFaction', 2, 0, 0, 0),
    (133, 'SameFactionAsPC', 0, 0, 0, 0),
    ( 43, 'SameRace', 2, 0, 0, 0),
    (134, 'SameRaceAsPC', 0, 0, 0, 0),
    ( 44, 'SameSex', 2, 0, 0, 0),
    (135, 'SameSexAsPC', 0, 0, 0, 0),
    (323, 'WhichServiceMenu', 0, 0, 0, 0),
    (449, 'HasPerk', 2, 1, 1, 2),
    (546, 'GetQuestCompleted', 2, 0, 0, 0),
    (427, 'GetIsVoiceType', 2, 0, 0, 0),
    (523, 'IsPS3', 0, 0, 0, 0),
    (524, 'IsWin32', 0, 0, 0, 0),
    (372, 'IsInList', 2, 0, 0, 0),
    (382, 'GetHasNote', 2, 1, 1, 2),
    (492, 'GetMapMakerVisible', 1, 1, 1, 2),
    (446, 'GetInZone', 2, 1, 1, 2),
    ( 25, 'IsMoving', 0, 0, 0, 0),
    ( 26, 'IsTurning', 0, 0, 0, 0),
    (451, 'IsLastIdlePlayed', 2, 0, 0, 0),
    (399, 'IsWeaponInList', 2, 0, 0, 0),
    (408, 'GetVATSValue', 1, 2, 0, 0),
    (435, 'GetDialogueEmotion', 0, 0, 0, 0),
    (235, 'GetVatsTargetHeight', 0, 0, 0, 0),
    (391, 'GetHitLocation', 0, 0, 0, 0),
    (392, 'IsPC1stPerson', 0, 0, 0, 0),
    (226, 'GetSandman', 0, 0, 0, 0),
    (428, 'GetPlantedExplosive', 0, 0, 0, 0),
    (304, 'IsWaterObject', 0, 0, 0, 0),
    (123, 'IsGreetingPlayer', 0, 0, 0, 0),
    (438, 'GetIsCreatureType', 1, 0, 0, 0),
    (503, 'GetRadiationLevel', 0, 0, 0, 0),
    (431, 'GetHealthPercentage', 0, 0, 0, 0),
    (411, 'GetFactionCombatReaction', 2, 2, 0, 0),
    (515, 'IsCombatTarget', 2, 0, 0, 0),
    (495, 'GetPermanentActorValue', 1, 0, 0, 0),
    (474, 'GetIsAlignment', 1, 0, 0, 0),
    (454, 'GetPlayerTeammate', 0, 0, 0, 0),
    (522, 'GetIsLockBroken', 0, 0, 0, 0),
    (433, 'GetIsObjectType', 1, 0, 0, 0),
    (500, 'GetWeaponHealthPerc', 0, 0, 0, 0),
    (368, 'IsPlayerActionActive', 1, 0, 0, 0),
    (416, 'GetGroupMemberCount', 0, 0, 0, 0),
    (417, 'GetGroupTargetCount', 0, 0, 0, 0),
    (510, 'GetLastHitCritical', 0, 0, 0, 0),
    (450, 'GetFactionRelation', 1, 0, 0, 0),
    (455, 'GetPlayerTeammateCount', 0, 0, 0, 0),
    (219, 'GetAnimAction', 0, 0, 0, 0),
    (430, 'IsActorTalkingThroughActivator', 0, 0, 0, 0),
    (480, 'GetIsUsedItemEquipType', 1, 0, 0, 0),
    (398, 'IsLimbGone', 1, 0, 0, 0),
    (550, 'IsGoreDisabled', 0, 0, 0, 0),
    (420, 'GetObjectiveCompleted', 2, 1, 0, 0),
    (421, 'GetObjectiveDisplayed', 2, 1, 0, 0),
    (397, 'GetCauseofDeath', 0, 0, 0, 0),
    (415, 'Exists', 2, 0, 0, 0),
    (117, 'GetMajorCrimeCount', 0, 0, 0, 0),
    (471, 'GetDestructionStage', 0, 0, 0, 0),
    (460, 'GetActorFactionPlayerEnemy', 0, 0, 0, 0),

    # extended by FOSE
    (1024, 'GetFOSEVersion', 0, 0, 0, 0),
    (1025, 'GetFOSERevision', 0, 0, 0, 0),
    (1213, 'GetFOSEBeta', 0, 0, 0, 0),
    (1082, 'IsKeyPressed', 1, 0, 0, 0),
    (1166, 'IsControlPressed', 1, 0, 0, 0),
    (1028, 'GetWeight', 2, 0, 0, 0),
    (1165, 'GetWeaponHasScope', 0, 0, 0, 0),
    )
allConditions = set(entry[0] for entry in conditionFunctionData)
fid1Conditions = set(entry[0] for entry in conditionFunctionData if entry[2] == 2)
fid2Conditions = set(entry[0] for entry in conditionFunctionData if entry[3] == 2)
fid3Conditions = set(entry[0] for entry in conditionFunctionData if entry[4] == 2)
fid4Conditions = set(entry[0] for entry in conditionFunctionData if entry[5] == 2)
    
# Magic Info ------------------------------------------------------------------
weaponTypes = (
    _('Big gun'),
    _('Energy'),
    _('Small gun'),
    _('Melee'),
    _('Unarmed'),
    _('Thrown'),
    _('Mine'),
    )
 
namesTypes = set((
        'ALCH', 'AMMO', 'APPA', 'ARMO', 'BOOK', 'CLAS', 'CLOT', 'CONT', 'CREA', 'DOOR',
        'EYES', 'FACT', 'FLOR', 'HAIR', 'INGR', 'KEYM', 'LIGH', 'MISC', 'NOTE', 'NPC_',
        'RACE', 'SPEL', 'TERM', 'WEAP', 'ACTI', 'TACT'))
pricesTypes = {}      
statsTypes = {
        'ALCH':('eid', 'weight', 'value'),
        'AMMO':('eid', 'speed',  'value', 'clipRounds'),
        'ARMO':('eid', 'weight', 'value', 'health', 'ar'),
        'ARMA':('eid', 'weight', 'value', 'health', 'ar'),
        'BOOK':('eid', 'weight', 'value'),
        'INGR':('eid', 'weight', 'value'),
        'KEYM':('eid', 'weight', 'value'),
        'LIGH':('eid', 'weight', 'value', 'duration'),
        'MISC':('eid', 'weight', 'value'),
        'WEAP':('eid', 'weight', 'value', 'health', 'damage','clipsize',
                'animationMultiplier','reach','ammoUse','minSpread','spread','sightFov','baseVatsToHitChance','projectileCount',
                'minRange','maxRange','animationAttackMultiplier','fireRate','overrideActionPoint','rumbleLeftMotorStrength',
                'rumbleRightMotorStrength','rumbleDuration','overrideDamageToWeaponMult','attackShotsPerSec',
                'reloadTime','jamTime','aimArc','rambleWavelangth','limbDmgMult','sightUsage',
                'semiAutomaticFireDelayMin','semiAutomaticFireDelayMax','criticalDamage','criticalMultiplier'),
        }
statsHeaders = (
        #--Alch
        ('ALCH',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'))) + '"\n')),
        #Ammo
        ('AMMO',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Speed'),_('Value'),_('Clip Rounds'))) + '"\n')),
        #--Armor
        ('ARMO',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'),_('Health'),_('AR'))) + '"\n')),
        #--Armor Addon
        ('ARMA',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'),_('Health'),_('AR'))) + '"\n')),
        #Books
        ('BOOK',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'))) + '"\n')),
        #Ingredients
        ('INGR',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'))) + '"\n')),
        #--Keys
        ('KEYM',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'))) + '"\n')),
        #Lights
        ('LIGH',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'),_('Duration'))) + '"\n')),
        #--Misc
        ('MISC',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'))) + '"\n')),
        #--Weapons
        ('WEAP',
            ('"' + '","'.join((_('Type'),_('Mod Name'),_('ObjectIndex'),
            _('Editor Id'),_('Weight'),_('Value'),_('Health'),_('Damage'),_('Clip Size'),
            _('Animation Multiplier'), _('Reach'), _('Ammo Use'), _('Min Spread'), _('Spread'), _('Sight Fov'), _('Base VATS To-Hit Chance'), _('Projectile Count'),
            _('Min Range'), _('Max Range'), _('Animation Attack Multiplier'), _('Fire Rate'), _('Override - Action Point'), _('Rumble - Left Motor Strength'),
            _('rRmble - Right Motor Strength'), _('Rumble - Duration'), _('Override - Damage To Weapon Mult'), _('Attack Shots/Sec'),
            _('Reload Time'), _('Jam Time'), _('Aim Arc'), _('Ramble - Wavelangth'), _('Limb Dmg Mult'), _('Sight Usage'),
            _('Semi-Automatic Fire Delay Min'), _('Semi-Automatic Fire Delay Max'),
            _('Critical Damage'), _('Crit % Mult'))) + '"\n')),
        )

# Race Info -------------------------------------------------------------------
raceNames = {
    0x000019 : _('Caucasian'),
    0x0038e5 : _('Hispanic'),
    0x0038e6 : _('Asian'),
    0x003b3e : _('Ghoul'),
    0x00424a : _('AfricanAmerican'),
    0x0042be : _('AfricanAmerican Child'),
    0x0042bf : _('AfricanAmerican Old'),
    0x0042c0 : _('Asian Child'),
    0x0042c1 : _('Asian Old'),
    0x0042c2 : _('Caucasian Child'),
    0x0042c3 : _('Caucasian Old'),
    0x0042c4 : _('Hispanic Child'),
    0x0042c5 : _('Hispanic Old'),
    0x04bb8d : _('Caucasian Raider'),
    0x04bf70 : _('Hispanic Raider'),
    0x04bf71 : _('Asian Raider'),
    0x04bf72 : _('AfricanAmerican Raider'),
    0x0987dc : _('Hispanic Old Aged'),
    0x0987dd : _('Asian Old Aged'),
    0x0987de : _('AfricanAmerican Old Aged'),
    0x0987df : _('Caucasian Old Aged'),
    }

raceShortNames = {
    0x000019 : 'Cau',
    0x0038e5 : 'His',
    0x0038e6 : 'Asi',
    0x003b3e : 'Gho',
    0x00424a : 'Afr',
    0x0042be : 'AfC',
    0x0042bf : 'AfO',
    0x0042c0 : 'AsC',
    0x0042c1 : 'AsO',
    0x0042c2 : 'CaC',
    0x0042c3 : 'CaO',
    0x0042c4 : 'HiC',
    0x0042c5 : 'HiO',
    0x04bb8d : 'CaR',
    0x04bf70 : 'HiR',
    0x04bf71 : 'AsR',
    0x04bf72 : 'AfR',
    0x0987dc : 'HOA',
    0x0987dd : 'AOA',
    0x0987de : 'FOA',
    0x0987df : 'COA',
    }
           
raceHairMale = {
    0x000019 : 0x014b90, #--Cau
    0x0038e5 : 0x0a9d6f, #--His
    0x0038e6 : 0x014b90, #--Asi
    0x003b3e : None, #--Gho
    0x00424a : 0x0306be, #--Afr
    0x0042be : 0x060232, #--AfC
    0x0042bf : 0x0306be, #--AfO
    0x0042c0 : 0x060232, #--AsC
    0x0042c1 : 0x014b90, #--AsO
    0x0042c2 : 0x060232, #--CaC
    0x0042c3 : 0x02bfdb, #--CaO
    0x0042c4 : 0x060232, #--HiC
    0x0042c5 : 0x02ddee, #--HiO
    0x04bb8d : 0x02bfdb, #--CaR
    0x04bf70 : 0x02bfdb, #--HiR
    0x04bf71 : 0x02bfdb, #--AsR
    0x04bf72 : 0x0306be, #--AfR
    0x0987dc : 0x0987da, #--HOA
    0x0987dd : 0x0987da, #--AOA
    0x0987de : 0x0987d9, #--FOA
    0x0987df : 0x0987da, #--COA
    }
 
raceHairFemale = {
    0x000019 : 0x05dc6b, #--Cau
    0x0038e5 : 0x05dc76, #--His
    0x0038e6 : 0x022e50, #--Asi
    0x003b3e : None, #--Gho
    0x00424a : 0x05dc78, #--Afr
    0x0042be : 0x05a59e, #--AfC
    0x0042bf : 0x072e39, #--AfO
    0x0042c0 : 0x05a5a3, #--AsC
    0x0042c1 : 0x072e39, #--AsO
    0x0042c2 : 0x05a59e, #--CaC
    0x0042c3 : 0x072e39, #--CaO
    0x0042c4 : 0x05a59e, #--HiC
    0x0042c5 : 0x072e39, #--HiO
    0x04bb8d : 0x072e39, #--CaR
    0x04bf70 : 0x072e39, #--HiR
    0x04bf71 : 0x072e39, #--AsR
    0x04bf72 : 0x072e39, #--AfR
    0x0987dc : 0x044529, #--HOA
    0x0987dd : 0x044529, #--AOA
    0x0987de : 0x044529, #--FOA
    0x0987df : 0x044529, #--COA
    }
    
    
# These eye variables have been refactored from the Wrye Flash version of bosh.py. 
# Their Oblivion equivalents remain in Bash's bosh.py.
def getIdFunc(modName):
    return lambda x: (GPath(modName),x)
ob = getIdFunc(masterFiles[0])
standardEyes = [ob(x) for x in (0x4252,0x4253,0x4254,0x4255,0x4256)]
 
defaultEyes = {
    #--fallout3.esm
    ob(0x000019): #--Caucasian
        standardEyes,
    ob(0x0038e5): #--Hispanic
        standardEyes,
    ob(0x0038e6): #--Asian
        standardEyes,
    ob(0x003b3e): #--Ghoul
        [ob(0x35e4f)],
    ob(0x00424a): #--AfricanAmerican
        standardEyes,
    ob(0x0042be): #--AfricanAmerican Child
        standardEyes,
    ob(0x0042bf): #--AfricanAmerican Old
        standardEyes,
    ob(0x0042c0): #--Asian Child
        standardEyes,
    ob(0x0042c1): #--Asian Old
        standardEyes,
    ob(0x0042c2): #--Caucasian Child
        standardEyes,
    ob(0x0042c3): #--Caucasian Old
        standardEyes,
    ob(0x0042c4): #--Hispanic Child
        standardEyes,
    ob(0x0042c5): #--Hispanic Old
        standardEyes,
    ob(0x04bb8d): #--Caucasian Raider
        [ob(0x4cb10)],
    ob(0x04bf70): #--Hispanic Raider
        [ob(0x4cb10)],
    ob(0x04bf71): #--Asian Raider
        [ob(0x4cb10)],
    ob(0x04bf72): #--AfricanAmerican Raider
        [ob(0x4cb10)],
    ob(0x0987dc): #--Hispanic Old Aged
        standardEyes,
    ob(0x0987dd): #--Asian Old Aged
        standardEyes,
    ob(0x0987de): #--AfricanAmerican Old Aged
        standardEyes,
    ob(0x0987df): #--Caucasian Old Aged
        standardEyes,
    }
 
# Flags
#------------------------------------------------------------------------------
class MelBipedFlags(bolt.Flags):
    """Biped flags element. Includes biped flag set by default."""
    mask = 0xFFFF
    def __init__(self,default=0L,newNames=None):
        names = bolt.Flags.getNames(
            'head', 'hair', 'upperBody', 'leftHand', 'rightHand', 'weapon',
            'pipboy', 'backpack', 'necklace', 'headband', 'hat', 'eyeGlasses',
            'noseRing', 'earrings', 'mask', 'choker', 'mouthObject',
            'bodyAddOn1', 'bodyAddOn2', 'bodyAddOn3')
        if newNames: names.update(newNames)
        bolt.Flags.__init__(self,default,names)

#------------------------------------------------------------------------------
class MelDestructible(MelGroup):
    """Represents a set of destruct record."""
    def __init__(self,attr='destructible'):
        """Initialize elements."""
        MelGroup.__init__(self,attr,
            MelBase('DEST','header'),
            MelStruct('DEST','IhH','health','count','flags'),
            MelGroups('stages',
                      MelStruct('DSTD','=4B4I','health','index','damageStage','flags',
                                'selfDamagePerSecond',(FID,'explosion',None),(FID,'debris',None),'debrisCount'),
                      MelString('DMDL','model'),
                      MelBase('DMDT','dmdt'), #--Should be a struct. Maybe later.
                      MelBase('DSTF','footer'),
                      ),
            )

#------------------------------------------------------------------------------
class MelEffects(MelGroups):
    """Represents ingredient/potion/enchantment/spell effects."""

    #--Class Data
    seFlags = Flags(0x0L,Flags.getNames('hostile'))
    # class MelEffectsScit(MelStruct):
    #     """Subclass to support alternate format."""
    #     def __init__(self):
    #         MelStruct.__init__(self,'SCIT','II4sB3s',(FID,'script',None),('school',0),
    #             ('visual','REHE'),(MelEffects.seFlags,'flags',0x0L),('unused1',null3))
    #     def loadData(self,record,ins,type,size,readId):
    #         #--Alternate formats
    #         if size == 16:
    #             attrs,actions = self.attrs,self.actions
    #             unpacked = ins.unpack(self.format,size,readId)
    #         elif size == 12:
    #             attrs,actions = ('script','school','visual'),(0,0,0)
    #             unpacked = ins.unpack('II4s',size,readId)
    #             record.unused1 = null3
    #         else: #--size == 4
    #             #--The script fid for MS40TestSpell doesn't point to a valid script.
    #             #--But it's not used, so... Not a problem! It's also t
    #             record.unused1 = null3
    #             attrs,actions = ('script',),(0,)
    #             unpacked = ins.unpack('I',size,readId)
    #             if unpacked[0] & 0xFF000000L:
    #                 unpacked = (0L,) #--Discard bogus MS40TestSpell fid
    #         #--Unpack
    #         record.__slots__ = self.attrs
    #         setter = record.__setattr__
    #         for attr,value,action in zip(attrs,unpacked,actions):
    #             if callable(action): value = action(value)
    #             setter(attr,value)
    #         if self._debug: print ' ',unpacked

    #--Instance methods
    def __init__(self,attr='effects'):
        """Initialize elements."""
        MelGroups.__init__(self,attr,
            MelFid('EFID','baseEffect'),
            MelStruct('EFIT','5i','magnitude','area','duration','recipient','actorValue'),
            # MelGroup('scriptEffect',
            #     MelEffects.MelEffectsScit(),
            #     MelString('FULL','full'),
            #     ),
            MelConditions(),
            )

#------------------------------------------------------------------------------
class MelScrxen(MelFids):
    """Handles mixed sets of SCRO and SCRV for scripts, quests, etc."""

    def getLoaders(self,loaders):
        loaders['SCRV'] = self
        loaders['SCRO'] = self

    def loadData(self,record,ins,type,size,readId):
        isFid = (type == 'SCRO')
        if isFid: value = ins.unpackRef(readId)
        else: value, = ins.unpack('I',4,readId)
        record.__getattribute__(self.attr).append((isFid,value))

    def dumpData(self,record,out):
        for isFid,value in record.__getattribute__(self.attr):
            if isFid: out.packRef('SCRO',value)
            else: out.packSub('SCRV','I',value)

    def mapFids(self,record,function,save=False):
        scrxen = record.__getattribute__(self.attr)
        for index,(isFid,value) in enumerate(scrxen):
            if isFid:
                result = function(value)
                if save: scrxen[index] = (isFid,result)

#------------------------------------------------------------------------------
class MelOwnership(MelGroup):
    """Handles XOWN, XRNK, and XGLB for cells and cell children."""

    def __init__(self):
        """Initialize."""
        MelGroup.__init__(self, 'ownership',
            MelFid('XOWN','owner'),
            MelOptStruct('XRNK','i',('rank',None)),
            MelFid('XGLB','global'),
        )

    def dumpData(self,record,out):
        """Dumps data from record to outstream."""
        if record.ownership and record.ownership.owner:
            MelGroup.dumpData(self,record,out)

#------------------------------------------------------------------------------
class MelConditions(MelStructs):
    """Represents a set of quest/dialog conditions. Difficulty is that FID state
    of parameters depends on function index."""
    def __init__(self):
        """Initialize."""
        MelStructs.__init__(self,'CTDA','B3sfIiiii','conditions',
            'operFlag',('unused1',null3),'compValue','ifunc','param1','param2','param3','param4')

    def getLoaders(self,loaders):
        """Adds self as loader for type."""
        loaders[self.subType] = self
        #loaders['CTDT'] = self #--Older CTDT type for ai package records.

    def getDefault(self):
        """Returns a default copy of object."""
        target = MelStructs.getDefault(self)
        target.form1234 = 'iiii'
        return target

    def hasFids(self,formElements):
        """Include self if has fids."""
        formElements.add(self)

    def loadData(self,record,ins,type,size,readId):
        """Reads data from ins into record attribute."""
        if type == 'CTDA':
            if size != 28 and size != 24 and size != 20:
                raise ModSizeError(ins.inName,readId,28,size,False)
        else:
            raise ModError(ins.inName,_('Unexpected subrecord: ')+readId)
        #if type == 'CTDT' and size != 20:
        #    raise ModSizeError(ins.inName,readId,20,size,True)
        target = MelObject()
        record.conditions.append(target)
        target.__slots__ = self.attrs
        unpacked1 = ins.unpack('B3sfI',12,readId)
        (target.operFlag,target.unused1,target.compValue,ifunc) = unpacked1
        #--Get parameters
        if ifunc not in bush.allConditions:
            raise BoltError(_('Unknown condition function: %d') % ifunc)
        form1 = 'iI'[ifunc in bush.fid1Conditions]
        form2 = 'iI'[ifunc in bush.fid2Conditions]
        form3 = 'iI'[ifunc in bush.fid3Conditions]
        form4 = 'iI'[ifunc in bush.fid4Conditions]
        if size == 28:
            form1234 = form1+form2+form3+form4
            unpacked2 = ins.unpack(form1234,16,readId)
            (target.param1,target.param2,target.param3,target.param4) = unpacked2
        elif size == 24:
            form1234 = form1+form2+form3
            unpacked2 = ins.unpack(form1234,12,readId)
            (target.param1,target.param2,target.param3) = unpacked2
            target.param4 = null4
        elif size == 20:
            form1234 = form1+form2
            unpacked2 = ins.unpack(form1234,8,readId)
            (target.param1,target.param2) = unpacked2
            target.param3 = null4
            target.param4 = null4
        else:
            raise ModSizeError(ins.inName,readId,28,size,False)
        (target.ifunc,target.form1234) = (ifunc,form1234)
        if self._debug:
            unpacked = unpacked1+unpacked2
            print ' ',zip(self.attrs,unpacked)
            if len(unpacked) != len(self.attrs):
                print ' ',unpacked

    def dumpData(self,record,out):
        """Dumps data from record to outstream."""
        for target in record.conditions:
            out.packSub('CTDA','B3sfI'+target.form1234,
                target.operFlag, target.unused1, target.compValue,
                target.ifunc, target.param1, target.param2, target.param3, target.param4)

    def mapFids(self,record,function,save=False):
        """Applies function to fids. If save is true, then fid is set
        to result of function."""
        for target in record.conditions:
            form1234 = target.form1234
            if form1234[0] == 'I':
                result = function(target.param1)
                if save: target.param1 = result
            if form1234[1] == 'I':
                result = function(target.param2)
                if save: target.param2 = result
            if len(form1234) > 2 and form1234[2] == 'I':
                result = function(target.param3)
                if save: target.param3 = result
            if len(form1234) > 3 and form1234[3] == 'I':
                result = function(target.param4)
                if save: target.param4 = result
    
class MreHeader(MreHeaderBase):
    """TES4 Record.  File header."""
    classType = 'TES4'

    #--Data elements
    melSet = MelSet(
        MelStruct('HEDR','f2I',('version',0.85),'numRecords',('nextObject',0xCE6)),
        MelBase('OFST','ofst_p',),  #--Obsolete?
        MelBase('DELE','dele_p',),  #--Obsolete?
        MelUnicode('CNAM','author',u'',512),
        MelUnicode('SNAM','description',u'',512),
        MreHeaderBase.MelMasterName('MAST','masters'),
        MelNull('DATA'), # 8 Bytes in Length
        MelFidList('ONAM','overrides'),
		# INTV and INCC are not used in Fallout 3
        #MelBase('INTV','ingv_p'),
        #MelBase('INCC', 'ingv_p'),
		# wbByteArray for Edit = MelBase for Wrye Bash
		MelBase('SCRN', 'ingv_p'),
        )
    __slots__ = MreHeaderBase.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreActor(MelRecord):
    """Creatures and NPCs."""

    def mergeFilter(self,modSet):
        """Filter out items that don't come from specified modSet.
        Filters spells, factions and items."""
        if not self.longFids: raise StateError(_("Fids not in long format"))
        self.spells = [x for x in self.spells if x[0] in modSet]
        self.factions = [x for x in self.factions if x.faction[0] in modSet]
        self.items = [x for x in self.items if x.item[0] in modSet]

#------------------------------------------------------------------------------
class MreAnio(MelRecord):
    """Animation object record."""
    classType = 'ANIO'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelFid('DATA','animationId'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreAppa(MelRecord):
    """Alchemical apparatus record."""
    classType = 'APPA'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelModel(),
        MelString('ICON','iconPath'),
        MelFid('SCRI','script'),
        MelStruct('DATA','=BIff',('apparatus',0),('value',25),('weight',1),('quality',10)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreBook(MelRecord):
    """BOOK record."""
    classType = 'BOOK'
    _flags = Flags(0,Flags.getNames('isScroll','isFixed'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelFid('SCRI','script'),
        MelString('DESC','text'),
        MelDestructible(),
        MelStruct('DATA', '=BbIf',(_flags,'flags',0L),('teaches',-1),'value','weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed() + ['modb']

#------------------------------------------------------------------------------
class MreBsgn(MelRecord):
    """Birthsign record."""
    classType = 'BSGN'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('ICON','iconPath'),
        MelString('DESC','text'),
        MelFids('SPLO','spells'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreCell(MelRecord):
    """Cell record."""
    classType = 'CELL'
    cellFlags = Flags(0L,Flags.getNames((0, 'isInterior'),(1,'hasWater'),(2,'invertFastTravel'),
        (3,'forceHideLand'),(5,'publicPlace'),(6,'handChanged'),(7,'behaveLikeExterior')))
    inheritFlags = Flags(0L,Flags.getNames('ambientColor','directionalColor','fogColor','fogNear','fogFar',
        'directionalRotation','directionalFade','clipDistance','fogPower'))
    class MelCoordinates(MelOptStruct):
        """Handle older trucated XCLC for CELL subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 12:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 8:
                unpacked = ins.unpack('ii',size,readId)
            else:
                raise "Unexpected size encountered for XCLC subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked, record.flags.getTrueAttrs()
        def dumpData(self,record,out):
            if not record.flags.isInterior:
                MelOptStruct.dumpData(self,record,out)
    class MelCellXcll(MelOptStruct):
        """Handle older trucated XCLL for CELL subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 40:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 36:
                unpacked = ins.unpack('=3Bs3Bs3Bs2f2i2f',size,readId)
            else:
                raise "Unexpected size encountered for XCLL subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked, record.flags.getTrueAttrs()
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelStruct('DATA','B',(cellFlags,'flags',0L)),
        MelCoordinates('XCLC','iiI',('posX',None),('posY',None),('forceHideLand',0L)),
        MelCellXcll('XCLL','=3Bs3Bs3Bs2f2i3f','ambientRed','ambientGreen','ambientBlue',
            ('unused1',null1),'directionalRed','directionalGreen','directionalBlue',
            ('unused2',null1),'fogRed','fogGreen','fogBlue',
            ('unused3',null1),'fogNear','fogFar','directionalXY','directionalZ',
            'directionalFade','fogClip','fogPower'),
        MelBase('IMPF','footstepMaterials'), #--todo rewrite specific class.
        MelFid('LTMP','lightTemplate'),
        MelOptStruct('LNAM','I',(inheritFlags,'lightInheritFlags',0L)),
        #--CS default for water is -2147483648, but by setting default here to -2147483649,
        #  we force the bashed patch to retain the value of the last mod.
        MelOptStruct('XCLW','f',('waterHeight',-2147483649)),
        MelString('XNAM','waterNoiseTexture'),
        MelFidList('XCLR','regions'),
        MelOptStruct('XCMT','B','xcmt_p'),
        MelFid('XCIM','imageSpace'),
        MelOptStruct('XCET','B','xcet_p'),
        MelFid('XEZN','encounterZone'),
        MelFid('XCCM','climate'),
        MelFid('XCWT','water'),
        MelOwnership(),
        MelFid('XCAS','acousticSpace'),
        MelFid('XCMO','music'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreClas(MelRecord):
    """Class record."""
    classType = 'CLAS'
    _flags = Flags(0L,Flags.getNames(
        ( 0,'Playable'),
        ( 1,'Guard'),
        ))
    aiService = Flags(0L,Flags.getNames(
        (0,'weapons'),
        (1,'armor'),
        (2,'clothing'),
        (3,'books'),
        (4,'foods'),
        (5,'chems'),
        (6,'stimpacks'),
        (7,'lights'),
        (10,'miscItems'),
        (13,'potions'),
        (14,'training'),
        (16,'recharge'),
        (17,'repair'),))
    aiTeaches = Flags(0L,Flags.getNames(
        (0,'barter'),
        (1,'bigGuns'),
        (2,'energyWeapons'),
        (3,'explosives'),
        (4,'lockpick'),
        (5,'medicine'),
        (6,'meleeWeapons'),
        (7,'none'),
        (8,'repair'),
        (9,'science'),
        (10,'smallGuns'),
        (11,'sneak'),
        (12,'throwing'),
        (13,'unarmed'),))
    class MelClasData(MelStruct):
        """Handle older trucated DATA for CLAS subrecords."""
    #     def loadData(self,record,ins,type,size,readId):
    #         if size == 52:
    #             MelStruct.loadData(self,record,ins,type,size,readId)
    #             return
    #         #--Else 42 byte record (skips trainSkill, trainLevel,unused1...
    #         unpacked = ins.unpack('2iI7i2I',size,readId)
    #         unpacked += self.defaults[len(unpacked):]
    #         setter = record.__setattr__
    #         for attr,value,action in zip(self.attrs,unpacked,self.actions):
    #             if callable(action): value = action(value)
    #             setter(attr,value)
    #         if self._debug: print unpacked, record.flags.getTrueAttrs()
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('DESC','description'),
        MelString('ICON','iconPath'),
        MelClasData('DATA','4I2IbB2s','tagSkill1','tagSkill2','tagSkill3','tagSkill4',(_flags,'flags',0L),(aiService,'services',0L),('trainSkill',0),('trainLevel',0),('unused1',null2)),
        MelTuple('ATTR','7B','attributes',[0]*7),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreClmt(MelRecord):
    """Climate record."""
    classType = 'CLMT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStructA('WLST','IiI', 'Weather', (FID,'weather'), 'chance', (FID,'global')),
        MelString('FNAM','sunPath'),
        MelString('GNAM','glarePath'),
        MelModel(),
        MelStruct('TNAM','6B','riseBegin','riseEnd','setBegin','setEnd','volatility','phaseLength'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreClot(MelRecord):
    """Clothing record."""
    classType = 'CLOT'
    _flags = MelBipedFlags(0L,Flags.getNames((16,'hideRings'),(17,'hideAmulet'),(22,'notPlayable')))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelFid('SCRI','script'),
        MelFid('ENAM','enchantment'),
        MelOptStruct('ANAM','H','enchantPoints'),
        MelStruct('BMDT','I',(_flags,'flags',0L)),
        MelModel('maleBody',0),
        MelModel('maleWorld',2),
        MelString('ICON','maleIconPath'),
        MelModel('femaleBody',3),
        MelModel('femaleWorld',4),
        MelString('ICO2','femaleIconPath'),
        MelStruct('DATA','If','value','weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreCrea(MreActor):
    """Creature Record."""
    classType = 'CREA'
    #--Main flags
    _flags = Flags(0L,Flags.getNames(
        ( 0,'biped'),
        ( 1,'essential'),
        ( 2,'weaponAndShield'),
        ( 3,'respawn'),
        ( 4,'swims'),
        ( 5,'flies'),
        ( 6,'walks'),
        ( 7,'pcLevelOffset'),
        ( 9,'noLowLevel'),
        (11,'noBloodSpray'),
        (12,'noBloodDecal'),
        (15,'noHead'),
        (16,'noRightArm'),
        (17,'noLeftArm'),
        (18,'noCombatInWater'),
        (19,'noShadow'),
        (20,'noVATSMelee'),
        (21,'allowPCDialogue'),
        (22,'cantOpenDoors'),
        (23,'immobile'),
        (24,'tiltFrontBack'),
        (25,'tiltLeftRight'),
        (26,'noKnockDown'),
        (27,'notPushable'),
        (28,'allowPickpocket'),
        (29,'isGhost'),
        (30,'noRotatingHeadTrack'),
        (31,'invulnerable'),))
    #--AI Service flags
    aiService = Flags(0L,Flags.getNames(
        (0,'weapons'),
        (1,'armor'),
        (2,'clothing'),
        (3,'books'),
        (4,'foods'),
        (5,'chems'),
        (6,'stimpacks'),
        (7,'lights'),
        (10,'miscItems'),
        (13,'potions'),
        (14,'training'),
        (16,'recharge'),
        (17,'repair'),))
    aiTrainSkill = Flags(0L,Flags.getNames(
        (0,'barter'),
        (1,'bigGuns'),
        (2,'energyWeapons'),
        (3,'explosives'),
        (4,'lockpick'),
        (5,'medicine'),
        (6,'meleeWeapons'),
        (7,'none'),
        (8,'repair'),
        (9,'science'),
        (10,'smallGuns'),
        (11,'sneak'),
        (12,'throwing'),
        (13,'unarmed'),))
    #--Mel Set
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelFids('SPLO','spells'),
        MelFid('EITM','effect'),
        MelStruct('EAMT','H', 'eamt'),
        MelStrings('NIFZ','bodyParts'),
        MelBase('NIFT','nift_p'), ###Texture File hashes, Byte Array
        MelStruct('ACBS','=I2Hh3Hf2H',
            (_flags,'flags',0L),'fatigue','barterGold',
            ('level',1),'calcMin','calcMax','speedMultiplier','karma','dispotionBase','templateFlags'),
        MelStructs('SNAM','=IB3s','factions',
            (FID,'faction',None),'rank',('unused1','IFZ')),
        MelFid('INAM','deathItem'),
        MelFid('VTCK','voice'),
        MelFid('TPLT','template'),
        MelDestructible(),
        MelFid('SCRI','script'),
        MelGroups('items',
            MelStruct('CNTO','Ii',(FID,'item',None),('count',1)),
            MelOptStruct('COED','IIf',(FID,'owner',None),(FID,'glob',None),('condition',1.0)),
        ),
        MelStruct('AIDT','=5B2I3Bi',
            ('aggression',5),('confidence',50),('energyLevel',50),('responsibility',50),('mood',0L),
            (aiService,'services',0L),(aiTrainSkill,'trainSkill',0L),'trainLevel','assistance',
            'aggroRadiusBehavior','aggroRadius'),
        MelFids('PKID','aiPackages'),
        MelStrings('KFFZ','animations'),
        MelStruct('DATA','=4BIH7B','type','combatSkill','magicSkill','StealthSkill',
            'health','damage','strength','perception','endurance','charisma','intelligence','agility','luck'),
        MelStruct('RNAM','B','attackReach'),
        MelFid('ZNAM','combatStyle'),
        MelFid('PNAM','bodyPartData'),
        MelStruct('TNAM','f','turningSpeed'),
        MelStruct('BNAM','f','baseScale'),
        MelStruct('WNAM','f','footWeight'),
        MelFid('CSCR','inheritsSoundsFrom'),
        MelStruct('NAM4','I',('impactMaterialType',0L)),
        MelStruct('NAM5','I',('soundLevel',0L)),
        MelGroups('sounds',
            MelStruct('CSDT','I','type'),
            MelFid('CSDI','sound'),
            MelStruct('CSDC','B','chance'),
        ),
        MelFid('CNAM','impactDataset'),
        MelFid('LNAM','meleeWeaponList'),
        )
    __slots__ = MreActor.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreDoor(MelRecord):
    """Container record."""
    classType = 'DOOR'
    _flags = Flags(0,Flags.getNames('oblivionGate','automatic','hidden','minimalUse'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelFid('SCRI','script'),
        MelDestructible(),
        MelFid('SNAM','soundOpen'),
        MelFid('ANAM','soundClose'),
        MelFid('BNAM','soundLoop'),
        MelStruct('FNAM','B',(_flags,'flags',0L)),
        #MelFids('TNAM','destinations'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreEfsh(MelRecord):
    """Effect shader record."""
    classType = 'EFSH'
    _flags = Flags(0L,Flags.getNames(
        ( 0,'noMemShader'),
        ( 3,'noPartShader'),
        ( 4,'edgeInverse'),
        ( 5,'memSkinOnly'),
        ))

    class MelEfshData(MelStruct):
        """Handle older trucated DATA for EFSH subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 308:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 300:
                unpacked = ins.unpack('B3s3I3Bs9f3Bs8f5I19f3Bs3Bs3Bs6f5fI5f3BsfII4f',size,readId)
            elif size == 284:
                unpacked = ins.unpack('B3s3I3Bs9f3Bs8f5I19f3Bs3Bs3Bs6f5fI5f3BsfII',size,readId)
            elif size == 248:
                unpacked = ins.unpack('B3s3I3Bs9f3Bs8f5I19f3Bs3Bs3Bs6f5fI',size,readId)
            elif size == 244:
                unpacked = ins.unpack('B3s3I3Bs9f3Bs8f5I19f3Bs3Bs3Bs6f5f',size,readId)
            elif size == 224:
                unpacked = ins.unpack('B3s3I3Bs9f3Bs8f5I19f3Bs3Bs3Bs6f',size,readId)
            else:
                raise "Unexpected size encountered for EFSH subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked, record.flags.getTrueAttrs()
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('ICON','fillTexture'),
        MelString('ICO2','particleTexture'),
        MelString('NAM7','holesTexture'),
        MelEfshData('DATA','B3s3I3Bs9f3Bs8f5I19f3Bs3Bs3Bs6f5fI5f3BsfII6f',(_flags,'flags'),('unused1',null3),'memSBlend',
            'memBlendOp','memZFunc','fillRed','fillGreen','fillBlue',('unused2',null1),
            'fillAIn','fillAFull','fillAOut','fillAPRatio','fillAAmp',
            'fillAFreq','fillAnimSpdU','fillAnimSpdV','edgeOff','edgeRed',
            'edgeGreen','edgeBlue',('unused3',null1),'edgeAIn','edgeAFull',
            'edgeAOut','edgeAPRatio','edgeAAmp','edgeAFreq','fillAFRatio',
            'edgeAFRatio','memDBlend',('partSBlend',5),('partBlendOp',1),
            ('partZFunc',4),('partDBlend',6),('partBUp',0.0),('partBFull',0.0),('partBDown',0.0),
            ('partBFRatio',1.0),('partBPRatio',1.0),('partLTime',1.0),('partLDelta',0.0),('partNSpd',0.0),
            ('partNAcc',0.0),('partVel1',0.0),('partVel2',0.0),('partVel3',0.0),('partAcc1',0.0),
            ('partAcc2',0.0),('partAcc3',0.0),('partKey1',1.0),('partKey2',1.0),('partKey1Time',0.0),
            ('partKey2Time',1.0),('key1Red',255),('key1Green',255),('key1Blue',255),('unused4',null1),
            ('key2Red',255),('key2Green',255),('key2Blue',255),('unused5',null1),('key3Red',255),('key3Green',255),
            ('key3Blue',255),('unused6',null1),('key1A',1.0),('key2A',1.0),('key3A',1.0),('key1Time',0.0),
            ('key2Time',0.5),('key3Time',1.0),
            ('partNSpdDelta',0.00000),('partRot',0.00000),('partRotDelta',0.00000),('partRotSpeed',0.00000),('partRotSpeedDelta',0.00000),
            (FID,'addonModels',None),('holesStartTime',0.00000),('holesEndTime',0.00000),('holesStartVal',0.00000),('holesEndVal',0.00000),
            ('edgeWidth',0.00000),('edgeRed',255),('edgeGreen',255),('edgeBlue',255),('unused7',null1),
            ('explosionWindSpeed',0.00000),('textureCountU',1),('textureCountV',1),
            ('addonModelsFadeInTime',1.00000),('addonModelsFadeOutTime',1.00000),
            ('addonModelsScaleStart',1.00000),('addonModelsScaleEnd',1.00000),
            ('addonModelsScaleInTime',1.00000),('addonModelsScaleOutTime',1.00000),
            ),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreHasEffects:
    """Mixin class for magic items."""
    def getEffects(self):
        """Returns a summary of effects. Useful for alchemical catalog."""
        effects = []
        avEffects = bush.genericAVEffects
        effectsAppend = effects.append
        for effect in self.effects:
            mgef, actorValue = effect.name, effect.actorValue
            if mgef not in avEffects:
                actorValue = 0
            effectsAppend((mgef,actorValue))
        return effects

    def getSpellSchool(self,mgef_school=bush.mgef_school):
        """Returns the school based on the highest cost spell effect."""
        spellSchool = [0,0]
        for effect in self.effects:
            school = mgef_school[effect.name]
            effectValue = bush.mgef_basevalue[effect.name]
            if effect.magnitude:
                effectValue *=  effect.magnitude
            if effect.area:
                effectValue *=  (effect.area/10)
            if effect.duration:
                effectValue *=  effect.duration
            if spellSchool[0] < effectValue:
                spellSchool = [effectValue,school]
        return spellSchool[1]

    def getEffectsSummary(self,mgef_school=None,mgef_name=None):
        """Return a text description of magic effects."""
        mgef_school = mgef_school or bush.mgef_school
        mgef_name = mgef_name or bush.mgef_name
        buff = stringBuffer()
        avEffects = bush.genericAVEffects
        aValues = bush.actorValues
        buffWrite = buff.write
        if self.effects:
            school = self.getSpellSchool(mgef_school)
            buffWrite(bush.actorValues[20+school] + '\n')
        for index,effect in enumerate(self.effects):
            if effect.scriptEffect:
                effectName = effect.scriptEffect.full or 'Script Effect'
            else:
                effectName = mgef_name[effect.name]
                if effect.name in avEffects:
                    effectName = re.sub(_('(Attribute|Skill)'),aValues[effect.actorValue],effectName)
            buffWrite('o+*'[effect.recipient]+' '+Unicode(effectName,'mbcs'))
            if effect.magnitude: buffWrite(' '+`effect.magnitude`+'m')
            if effect.area: buffWrite(' '+`effect.area`+'a')
            if effect.duration > 1: buffWrite(' '+`effect.duration`+'d')
            buffWrite('\n')
        return buff.getvalue()

#------------------------------------------------------------------------------
class MreEnch(MelRecord,MreHasEffects):
    """Enchantment (Object Effect) record."""
    classType = 'ENCH'
    _flags = Flags(0L,Flags.getNames('noAutoCalc'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFull0(), #--At least one mod has this. Odd.
        MelStruct('ENIT','3IB3s','itemType','chargeAmount','enchantCost',(_flags,'flags',0L),('unused1',null3)),
        #--itemType = 0: Scroll, 1: Staff, 2: Weapon, 3: Apparel
        MelEffects(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreEyes(MelRecord):
    """Eyes record."""
    classType = 'EYES'
    _flags = Flags(0L,Flags.getNames('playable',))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('ICON','iconPath'),
        MelStruct('DATA','B',(_flags,'flags')),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreFlor(MelRecord):
    """Flora (plant) record."""
    classType = 'FLOR'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelModel(),
        MelFid('SCRI','script'),
        MelFid('PFIG','ingredient'),
        MelStruct('PFPC','4B','spring','summer','fall','winter'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreFurn(MelRecord):
    """Furniture record."""
    classType = 'FURN'
    _flags = Flags() #--Governs type of furniture and which anims are available
    #--E.g., whether it's a bed, and which of the bed entry/exit animations are available
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelFid('SCRI','script'),
        MelDestructible(),
        MelStruct('MNAM','I',(_flags,'activeMarkers',0L)), ####ByteArray
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreGmst(MelRecord):
    """Gmst record"""
    falloutIds = None
    classType = 'GMST'
    class MelGmstValue(MelBase):
        def loadData(self,record,ins,type,size,readId):
            format = record.eid[0] #-- s|i|f
            if format == 's':
                record.value = ins.readString(size,readId)
            else:
                record.value, = ins.unpack(format,size,readId)
        def dumpData(self,record,out):
            format = record.eid[0] #-- s|i|f
            if format == 's':
                out.packSub0(self.subType,record.value)
            else:
                out.packSub(self.subType,format,record.value)
    melSet = MelSet(
        MelString('EDID','eid'),
        MelGmstValue('DATA','value'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

    def getGMSTFid(self):
        """Returns Fallout3.esm fid in long format for specified eid."""
        myClass = self.__class__
        if not myClass.falloutIds:
            try:
                myClass.falloutIds = cPickle.load(dirs['db'].join('Fallout3_ids.pkl').open())['GMST']
            except:
                old = bolt.deprintOn
                bolt.deprintOn = True
                print
                print 'Error loading Fallout3_ids.pkl:'
                deprint(' ',traceback=True)
                bolt.deprintOn = old
                print
                print 'Manually testing if file exists:', dirs['db'].join('Fallout3_ids.pkl').exists()
                print 'Current working directory:', os.getcwd()
                print "dirs['db']:", dirs['db']
                print
                raise
        return (modInfos.masterName, myClass.falloutIds[self.eid])

#------------------------------------------------------------------------------
class MreGras(MelRecord):
    """Grass record."""
    classType = 'GRAS'
    _flags = Flags(0,Flags.getNames('vLighting','uScaling','fitSlope'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelModel(),
        MelStruct('DATA','3BsH2sI4fB3s','density','minSlope',
                  'maxSlope',('unused1',null1),'waterDistance',('unused2',null2),
                  'waterOp','posRange','heightRange','colorRange',
                  'wavePeriod',(_flags,'flags'),('unused3',null3)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreHair(MelRecord):
    """Hair record."""
    classType = 'HAIR'
    _flags = Flags(0L,Flags.getNames('playable','notMale','notFemale','fixed'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelModel(),
        MelString('ICON','iconPath'),
        MelStruct('DATA','B',(_flags,'flags')),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreIngr(MelRecord,MreHasEffects):
    """INGR (ingredient) record."""
    classType = 'INGR'
    _flags = Flags(0L,Flags.getNames('noAutoCalc','isFood'))
    _etype = Flags(0L,Flags.getNames(
        'alcohol','bigGuns','bodyWear','chems','energyWeapons','food','handWear','headWear',
        'meleeWeapons','mine','none','smallGuns','stimpack','thrownWeapons','unarmedWeapon'
    ))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFull0(),
        MelModel(),
        MelString('ICON','iconPath'),
        MelFid('SCRI','script'),
        MelStruct('ETYP','I',(_etype,'etype',0L)),
        MelStruct('DATA','f','weight'),
        MelStruct('ENIT','iB3s','value',(_flags,'flags',0L),('unused1',null3)),
        MelEffects(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreKeym(MelRecord):
    """MISC (miscellaneous item) record."""
    classType = 'KEYM'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelFid('SCRI','script'),
        MelDestructible(),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        MelStruct('DATA','if','value','weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreLeveledList(MelRecord):
    """Leveled item/creature/spell list.."""
    _flags = Flags(0,Flags.getNames('calcFromAllLevels','calcForEachItem','useAllSpells'))
    #--Special load classes
    class MelLevListLvld(MelStruct):
        """Subclass to support alternate format."""
        def loadData(self,record,ins,type,size,readId):
            MelStruct.loadData(self,record,ins,type,size,readId)
            if record.chanceNone > 127:
                record.flags.calcFromAllLevels = True
                record.chanceNone &= 127

    class MelLevListLvlo(MelStruct):
        """Subclass to support alternate format."""
        def loadData(self,record,ins,type,size,readId):
            if size == 12:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 8:
                format,attrs = ('iI',('level','listId'))####might be h2sI
            else:
                raise "Unexpected size encountered for LVLO subrecord: %s" % size
            unpacked = ins.unpack(format,size,readId)
            setter = record.__setattr__
            for attr,value,action in zip(attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked
    #--Element Set
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelLevListLvld('LVLD','B','chanceNone'),
        MelStruct('LVLF','B',(_flags,'flags',0L)),
        MelFid('SCRI','script'),
        MelFid('TNAM','template'),
        MelFid('LVLG','glob'),
        MelGroups('entries',
                  MelLevListLvlo('LVLO','h2sIh2s','level',('unused1',null2),(FID,'listId',None),('count',1),('unused2',null2)),
                  MelOptStruct('COED','IIf',(FID,'owner',None),(FID,'glob',None),('condition',1.0)),
                  ),
        MelNull('DATA'),
        )
    __slots__ = (MelRecord.__slots__ + melSet.getSlotsUsed() +
        ['mergeOverLast','mergeSources','items','delevs','relevs'])

    def __init__(self,header,ins=None,unpack=False):
        """Initialize."""
        MelRecord.__init__(self,header,ins,unpack)
        self.mergeOverLast = False #--Merge overrides last mod merged
        self.mergeSources = None #--Set to list by other functions
        self.items  = None #--Set of items included in list
        self.delevs = None #--Set of items deleted by list (Delev and Relev mods)
        self.relevs = None #--Set of items relevelled by list (Relev mods)

    def mergeFilter(self,modSet):
        """Filter out items that don't come from specified modSet."""
        if not self.longFids: raise StateError(_("Fids not in long format"))
        self.entries = [entry for entry in self.entries if entry.listId[0] in modSet]

    def mergeWith(self,other,otherMod):
        """Merges newLevl settings and entries with self.
        Requires that: self.items, other.delevs and other.relevs be defined."""
        if not self.longFids: raise StateError(_("Fids not in long format"))
        if not other.longFids: raise StateError(_("Fids not in long format"))
        #--Relevel or not?
        if other.relevs:
            self.chanceNone = other.chanceNone
            self.script = other.script
            self.template = other.template
            self.flags = other.flags()
            self.glob = other.glob
        else:
            self.chanceNone = other.chanceNone or self.chanceNone
            self.script   = other.script or self.script
            self.template = other.template or self.template
            self.flags |= other.flags
            self.glob = other.glob or self.glob
        #--Remove items based on other.removes
        if other.delevs or other.relevs:
            removeItems = self.items & (other.delevs | other.relevs)
            self.entries = [entry for entry in self.entries if entry.listId not in removeItems]
            self.items = (self.items | other.delevs) - other.relevs
        hasOldItems = bool(self.items)
        #--Add new items from other
        newItems = set()
        entriesAppend = self.entries.append
        newItemsAdd = newItems.add
        for entry in other.entries:
            if entry.listId not in self.items:
                entriesAppend(entry)
                newItemsAdd(entry.listId)
        if newItems:
            self.items |= newItems
            self.entries.sort(key=attrgetter('listId','level','count','owner','condition'))
        #--Is merged list different from other? (And thus written to patch.)
        if (self.chanceNone != other.chanceNone or
            self.script != other.script or
            self.template != other.template or
            #self.flags != other.flags or
            self.glob != other.glob or
            len(self.entries) != len(other.entries)
            ):
            self.mergeOverLast = True
        else:
            otherlist = other.entries
            otherlist.sort(key=attrgetter('listId','level','count','owner','condition'))
            for selfEntry,otherEntry in zip(self.entries,otherlist):
                if (selfEntry.listId != otherEntry.listId or
                    selfEntry.level != otherEntry.level or
                    selfEntry.count != otherEntry.count or
                    selfEntry.owner != otherEntry.owner or
                    selfEntry.condition != otherEntry.condition):
                    self.mergeOverLast = True
                    break
            else:
                self.mergeOverLast = False
        if self.mergeOverLast:
            self.mergeSources.append(otherMod)
        else:
            self.mergeSources = [otherMod]
        #--Done
        self.setChanged(self.mergeOverLast)

#------------------------------------------------------------------------------
class MreLtex(MelRecord):
    """Landscape Texture."""
    _flags = Flags(0L,Flags.getNames(
        ( 0,'stone'),
        ( 1,'cloth'),
        ( 2,'dirt'),
        ( 3,'glass'),
        ( 4,'grass'),
        ( 5,'metal'),
        ( 6,'organic'),
        ( 7,'skin'),
        ( 8,'water'),
        ( 9,'wood'),
        (10,'heavyStone'),
        (11,'heavyMetal'),
        (12,'heavyWood'),
        (13,'chain'),
        (14,'snow'),))
    classType = 'LTEX'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('ICON','iconPath'),
        MelFid('TNAM', 'texture'),
        MelOptStruct('HNAM','3B',(_flags,'flags'),'friction','restitution'), ####flags are actually an enum....
        MelOptStruct('SNAM','B','specular'),
        MelFids('GNAM', 'grass'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreLvlc(MreLeveledList):
    """LVLC record. Leveled list for creatures."""
    classType = 'LVLC'
    __slots__ = MreLeveledList.__slots__

#------------------------------------------------------------------------------
class MreLvli(MreLeveledList):
    """LVLI record. Leveled list for items."""
    classType = 'LVLI'
    __slots__ = MreLeveledList.__slots__

#------------------------------------------------------------------------------
class MreLvsp(MreLeveledList):
    """LVSP record. Leveled list for items."""
    classType = 'LVSP'
    __slots__ = MreLeveledList.__slots__

#------------------------------------------------------------------------------
class MreLvln(MreLeveledList):
    """LVLN record. Leveled list for NPC."""
    classType = 'LVLN'
    __slots__ = MreLeveledList.__slots__

#------------------------------------------------------------------------------
class MreMgef(MelRecord):
    """MGEF (magic effect) record."""
    classType = 'MGEF'
    #--Main flags
    _flags = Flags(0L,Flags.getNames(
        ( 0,'hostile'),
        ( 1,'recover'),
        ( 2,'detrimental'),
        ( 3,'magnitude'),
        ( 4,'self'),
        ( 5,'touch'),
        ( 6,'target'),
        ( 7,'noDuration'),
        ( 8,'noMagnitude'),
        ( 9,'noArea'),
        (10,'fxPersist'),
        (11,'spellmaking'),
        (12,'enchanting'),
        (13,'noIngredient'),
        (16,'useWeapon'),
        (17,'useArmor'),
        (18,'useCreature'),
        (19,'useSkill'),
        (20,'useAttr'),
        (24,'useAV'),
        (25,'sprayType'),
        (26,'boltType'),
        (27,'noHitEffect'),))

    #--Mel NPC DATA
    class MelMgefData(MelStruct):
        """Handle older trucated DATA for DARK subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 72:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 36:
                #--Else is data for DARK record, read it all.
                unpacked = ins.unpack('IfIiiH2sIfI',size,readId)
            else:
                raise ModError(ins.inName,_('Unexpected size encountered for MGEF:DATA subrecord: ')+str(size))
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('DESC','text'),
        MelString('ICON','iconPath'),
        MelModel(),
        MelMgefData('DATA','IfIiiIIf6I2fII',
            (_flags,'flags'),'baseCost',(FID,'associated'),'school','resistValue','unk1',
            (FID,'light',0),'projectileSpeed',(FID,'effectShader',0),(FID,'objectDisplayShader',0),
            (FID,'castingSound',0),(FID,'boltSound',0),(FID,'hitSound',0),(FID,'areaSound',0),
            ('cefEnchantment',0.0),('cefBarter',0.0),'archType','actorValue'),
        MelStructA('ESCE','4s','counterEffects','effect'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreMisc(MelRecord):
    """MISC (miscellaneous item) record."""
    classType = 'MISC'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelFid('SCRI','script'),
        MelDestructible(),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        MelStruct('DATA','if','value','weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreNpc(MreActor):
    """NPC Record. Non-Player Character."""
    classType = 'NPC_'
    #--Main flags
    _flags = Flags(0L,Flags.getNames(
        ( 0,'female'),
        ( 1,'essential'),
        ( 2,'isChargenFacePreset'),
        ( 3,'respawn'),
        ( 4,'autoCalc'),
        ( 7,'pcLevelOffset'),
        ( 8,'useTemplate'),
        ( 9,'noLowLevel'),
        (11,'noBloodSpray'),
        (12,'noBloodDecal'),
        (20,'noVATSMelee'),
        (22,'canBeAllRaces'),
        (26,'noKnockDown'),
        (27,'notPushable'),
        (30,'noRotatingHeadTrack'),))
    #--AI Service flags
    aiService = Flags(0L,Flags.getNames(
        (0,'weapons'),
        (1,'armor'),
        (2,'clothing'),
        (3,'books'),
        (4,'foods'),
        (5,'chems'),
        (6,'stimpacks'),
        (7,'lights'),
        (10,'miscItems'),
        (13,'potions'),
        (14,'training'),
        (16,'recharge'),
        (17,'repair'),))
    aiTrainSkill = Flags(0L,Flags.getNames(
        (0,'barter'),
        (1,'bigGuns'),
        (2,'energyWeapons'),
        (3,'explosives'),
        (4,'lockpick'),
        (5,'medicine'),
        (6,'meleeWeapons'),
        (7,'none'),
        (8,'repair'),
        (9,'science'),
        (10,'smallGuns'),
        (11,'sneak'),
        (12,'throwing'),
        (13,'unarmed'),))
    #--Mel NPC DATA
    class MelNpcData(MelStruct):
        """Convert npc stats into skills, health, attributes."""
        def loadData(self,record,ins,type,size,readId):
            if size == 11:
                unpacked = list(ins.unpack('=I7B',size,readId))
            else:
                unpacked = list(ins.unpack('=I21B',size,readId))
            recordSetAttr = record.__setattr__
            recordSetAttr('health',unpacked[0])
            recordSetAttr('attributes',unpacked[1:])
            if self._debug: print unpacked[0],unpacked[1:]
        def dumpData(self,record,out):
            """Dumps data from record to outstream."""
            recordGetAttr = record.__getattribute__
            values = [recordGetAttr('health')]+recordGetAttr('attributes')
            if len(recordGetAttr('attributes')) == 7:
                out.packSub(self.subType,'=I7B',*values)
            else:
                out.packSub(self.subType,'=I21B',*values)

    #--Mel NPC DNAM
    class MelNpcDnam(MelStruct):
        """Convert npc stats into skills."""
        def loadData(self,record,ins,type,size,readId):
            unpacked = list(ins.unpack('=28B',size,readId))
            recordSetAttr = record.__setattr__
            recordSetAttr('skillValues',unpacked[:14])
            recordSetAttr('skillOffsets',unpacked[14:])
            if self._debug: print unpacked[:14]+unpacked[14:]
        def dumpData(self,record,out):
            """Dumps data from record to outstream."""
            recordGetAttr = record.__getattribute__
            values = recordGetAttr('skillValues')+recordGetAttr('skillOffsets')
            out.packSub(self.subType,'=28B',*values)

    #--Mel Set
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelModel(),
        MelStruct('ACBS','=I2Hh3Hf2H',
            (_flags,'flags',0L),'fatigue','barterGold',
            ('level',1),'calcMin','calcMax','speedMultiplier','karma','dispotionBase','templateFlags'),
        MelStructs('SNAM','=IB3s','factions',
            (FID,'faction',None),'rank',('unused1','ODB')),
        MelFid('INAM','deathItem'),
        MelFid('VTCK','voice'),
        MelFid('TPLT','template'),
        MelFid('RNAM','race'),
        #MelFid('????','actorEffects'),
        MelFid('EITM','unarmedAttackEffect'),
        MelStruct('EAMT','H', 'unarmedAttackAnimation'),
        MelDestructible(),
        MelFids('SPLO','spells'),
        MelFid('SCRI','script'),
        MelGroups('items',
            MelStruct('CNTO','Ii',(FID,'item',None),('count',1)),
            MelOptStruct('COED','IIf',(FID,'owner',None),(FID,'glob',None),('condition',1.0)),
        ),
        MelStruct('AIDT','=5B2I3Bi',
            ('aggression',5),('confidence',50),('energyLevel',50),('responsibility',50),('mood',0L),
            (aiService,'services',0L),(aiTrainSkill,'trainSkill',0L),'trainLevel','assistance',
            'aggroRadiusBehavior','aggroRadius'),
        MelFids('PKID','aiPackages'),
        MelStrings('KFFZ','animations'),
        MelFid('CNAM','iclass'),
        MelNpcData('DATA','','health',('attributes',[0]*21)),
        MelFids('PNAM','headParts'),
        MelNpcDnam('DNAM','',('skillValues',[0]*14),('skillOffsets',[0]*14)),
        MelFid('HNAM','hair'),
        MelOptStruct('LNAM','f',('hairLength',1)),
        MelFid('ENAM','eye'), ####fid Array
        MelStruct('HCLR','3Bs','hairRed','hairBlue','hairGreen',('unused3',null1)),
        MelFid('ZNAM','combatStyle'),
        MelStruct('NAM4','I',('impactMaterialType',0L)),
        MelBase('FGGS','fggs_p'), ####FaceGen Geometry-Symmetric
        MelBase('FGGA','fgga_p'), ####FaceGen Geometry-Asymmetric
        MelBase('FGTS','fgts_p'), ####FaceGen Texture-Symmetric
        MelStruct('NAM5','H',('unknown',0L)),
        MelStruct('NAM6','f',('height',0L)),
        MelStruct('NAM7','f',('weight',0L)),
        )
    __slots__ = MreActor.__slots__ + melSet.getSlotsUsed()

    def setRace(self,race):
        """Set additional race info."""
        self.race = race
        #--Model
        if not self.model:
            self.model = self.getDefault('model')
        if race in (0x23fe9,0x223c7):
            self.model.modPath = r"Characters\_Male\SkeletonBeast.NIF"
        else:
            self.model.modPath = r"Characters\_Male\skeleton.nif"
        #--FNAM
        fnams = {
            0x23fe9 : 0x3cdc ,#--Argonian
            0x224fc : 0x1d48 ,#--Breton
            0x191c1 : 0x5472 ,#--Dark Elf
            0x19204 : 0x21e6 ,#--High Elf
            0x00907 : 0x358e ,#--Imperial
            0x22c37 : 0x5b54 ,#--Khajiit
            0x224fd : 0x03b6 ,#--Nord
            0x191c0 : 0x0974 ,#--Orc
            0x00d43 : 0x61a9 ,#--Redguard
            0x00019 : 0x4477 ,#--Vampire
            0x223c8 : 0x4a2e ,#--Wood Elf
            }
        self.fnam = fnams.get(race,0x358e)

#------------------------------------------------------------------------------
class MrePack(MelRecord):
    """AI package record."""
    classType = 'PACK'
    _flags = Flags(0,Flags.getNames(
        'offersServices','mustReachLocation','mustComplete','lockAtStart',
        'lockAtEnd','lockAtLocation','unlockAtStart','unlockAtEnd',
        'unlockAtLocation','continueIfPcNear','oncePerDay',None,
        'skipFallout','alwaysRun',None,None,
        None,'alwaysSneak','allowSwimming','allowFalls',
        'unequipArmor','unequipWeapons','defensiveCombat','useHorse',
        'noIdleAnims',))
    _variableFlags = Flags(0L,Flags.getNames('isLongOrShort'))
    class MelPackPkdt(MelStruct):
        """Support older 8 byte version."""
        def loadData(self,record,ins,type,size,readId):
            if size == 12:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 8:
                unpacked = ins.unpack('IHH',size,readId)
            else:
                raise "Unexpected size encountered for PACK:PKDT subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked
    class MelPackPkdd(MelOptStruct):
        """Handle older trucated PKDD for PACK subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 24:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 20:
                unpacked = ins.unpack('fII4sI',size,readId)
            elif size == 16:
                unpacked = ins.unpack('fII4s',size,readId)
            elif size == 12:
                unpacked = ins.unpack('fII',size,readId)
            else:
                raise "Unexpected size encountered for PACK:PKDD subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked
    class MelPackLT(MelOptStruct):
        """For PLDT and PTDT. Second element of both may be either an FID or a long,
        depending on value of first element."""
        def loadData(self,record,ins,type,size,readId):
            if ((self.subType == 'PLDT' and size == 12) or
                (self.subType == 'PLD2' and size == 12) or
                (self.subType == 'PTDT' and size == 16) or
                (self.subType == 'PTD2' and size == 16)):
                MelOptStruct.loadData(self,record,ins,type,size,readId)
                return
            elif ((self.subType == 'PTDT' and size == 12) or
                  (self.subType == 'PTD2' and size == 12)):
                unpacked = ins.unpack('iIi',size,readId)
            else:
                raise "Unexpected size encountered for PACK:%s subrecord: %s" % (self.subType, size)
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked
        def hasFids(self,formElements):
            formElements.add(self)
        def dumpData(self,record,out):
            if ((self.subType == 'PLDT' and (record.locType or record.locId)) or
                (self.subType == 'PLD2' and (record.locType2 or record.locId2)) or
                (self.subType == 'PTDT' and (record.targetType or record.targetId)) or
                (self.subType == 'PTD2' and (record.targetType2 or record.targetId2))):
                MelStruct.dumpData(self,record,out)
        def mapFids(self,record,function,save=False):
            """Applies function to fids. If save is true, then fid is set
            to result of function."""
            if self.subType == 'PLDT' and record.locType != 5:
                result = function(record.locId)
                if save: record.locId = result
            elif self.subType == 'PLD2' and record.locType2 != 5:
                result = function(record.locId2)
                if save: record.locId2 = result
            elif self.subType == 'PTDT' and record.targetType != 2:
                result = function(record.targetId)
                if save: record.targetId = result
            elif self.subType == 'PTD2' and record.targetType2 != 2:
                result = function(record.targetId2)
                if save: record.targetId2 = result
    class MelPackDistributor(MelNull):
        """Handles embedded script records. Distributes load
        duties to other elements as needed."""
        def __init__(self):
            self._debug = False
        def getLoaders(self,loaders):
            """Self as loader for structure types."""
            for type in ('POBA','POEA','POCA'):
                loaders[type] = self
        def setMelSet(self,melSet):
            """Set parent melset. Need this so that can reassign loaders later."""
            self.melSet = melSet
            self.loaders = {}
            for element in melSet.elements:
                attr = element.__dict__.get('attr',None)
                if attr: self.loaders[attr] = element
        def loadData(self,record,ins,type,size,readId):
            if type == 'POBA':
                element = self.loaders['onBegin']
            elif type == 'POEA':
                element = self.loaders['onEnd']
            elif type == 'POCA':
                element = self.loaders['onChange']
            for subtype in ('INAM','SCHR','SCDA','SCTX','SLSD','SCVR','SCRV','SCRO','TNAM'):
                self.melSet.loaders[subtype] = element
            element.loadData(record,ins,type,size,readId)
    #--MelSet
    melSet = MelSet(
        MelString('EDID','eid'),
        MelPackPkdt('PKDT','IHHI',(_flags,'flags'),'aiType','falloutBehaviorFlags','typeSpecificFlags'),
        MelPackLT('PLDT','iIi','locType','locId','locRadius'),
        MelPackLT('PLD2','iIi','locType2','locId2','locRadius2'),
        MelStruct('PSDT','2bBbi','month','day','date','time','duration'),
        MelPackLT('PTDT','iIif','targetType','targetId','targetCount','targetUnknown1'),
        MelConditions(),
        MelGroup('idleAnimations',
            MelStruct('IDLF','B','animationFlags'),
            MelBase('IDLC','animationCount'), # byte or short
            MelStruct('IDLT','f','idleTimerSetting'),
            MelFidList('IDLA','animations'),
            MelBase('IDLB','idlb_p'),
            ),
        MelBase('PKED','eatMarker'),
        MelOptStruct('PKE2','I','escordDistance'),
        MelFid('CNAM','combatStyle'),
        MelOptStruct('PKFD','f','followStartLocationTrigerRadius'),
        MelBase('PKPT','patrolFlags'), # byte or short
        MelOptStruct('PKW3','IBB3Hff4s','weaponFlags','fireRate','fireCount','numBursts',
                     'shootPerVolleysMin','shootPerVolleysMax','pauseBetweenVolleysMin','pauseBetweenVolleysMax','weaponUnknown'),
        MelPackLT('PTD2','iIif','targetType2','targetId2','targetCount2','targetUnknown2'),
        MelBase('PUID','useItemMarker'),
        MelBase('PKAM','ambushMarker'),
        MelPackPkdd('PKDD','fII4sI4s','dialFov','dialTopic','dialFlags','dialUnknown1','dialType','dialUnknown2'),
        MelGroup('onBegin',
            MelBase('POBA', 'marker', ''), #### onBegin Marker, wbEmpty
            MelFid('INAM', 'idle'),
            MelStruct('SCHR','4s4I',('unused1',null4),'numRefs','compiledSize','lastIndex','scriptType'),
            MelBase('SCDA','compiled_p'),
            MelString('SCTX','scriptText'),
            MelGroups('vars',
                MelStruct('SLSD','I12sB7s','index',('unused1',null4+null4+null4),(_variableFlags,'flags',0L),('unused2',null4+null3)),
                MelString('SCVR','name')),
            MelScrxen('SCRV/SCRO','references'),
            MelFid('TNAM', 'topic'),
            ),
        MelGroup('onEnd',
            MelBase('POEA', 'marker', ''), #### onEnd Marker, wbEmpty
            MelFid('INAM', 'idle'),
            MelStruct('SCHR','4s4I',('unused1',null4),'numRefs','compiledSize','lastIndex','scriptType'),
            MelBase('SCDA','compiled_p'),
            MelString('SCTX','scriptText'),
            MelGroups('vars',
                MelStruct('SLSD','I12sB7s','index',('unused1',null4+null4+null4),(_variableFlags,'flags',0L),('unused2',null4+null3)),
                MelString('SCVR','name')),
            MelScrxen('SCRV/SCRO','references'),
            MelFid('TNAM', 'topic'),
            ),
        MelGroup('onChange',
            MelBase('POCA', 'marker', ''), #### onChange Marker, wbEmpty
            MelFid('INAM', 'idle'),
            MelStruct('SCHR','4s4I',('unused1',null4),'numRefs','compiledSize','lastIndex','scriptType'),
            MelBase('SCDA','compiled_p'),
            MelString('SCTX','scriptText'),
            MelGroups('vars',
                MelStruct('SLSD','I12sB7s','index',('unused1',null4+null4+null4),(_variableFlags,'flags',0L),('unused2',null4+null3)),
                MelString('SCVR','name')),
            MelScrxen('SCRV/SCRO','references'),
            MelFid('TNAM', 'topic'),
            ),
        #--Distributor for embedded script entries.
        MelPackDistributor(),
        )
    melSet.elements[-1].setMelSet(melSet)
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreQust(MelRecord):
    """Quest record."""
    classType = 'QUST'
    _questFlags = Flags(0,Flags.getNames('startGameEnabled',None,'repeatedTopics','repeatedStages'))
    _variableFlags = Flags(0L,Flags.getNames('isLongOrShort'))
    stageFlags = Flags(0,Flags.getNames('complete'))
    targetFlags = Flags(0,Flags.getNames('ignoresLocks'))

    #--CDTA loader
    class MelQustLoaders(DataDict):
        """Since CDTA subrecords occur in three different places, we need
        to replace ordinary 'loaders' dictionary with a 'dictionary' that will
        return the correct element to handle the CDTA subrecord. 'Correct'
        element is determined by which other subrecords have been encountered."""
        def __init__(self,loaders,quest,stages,targets):
            self.data = loaders
            self.type_ctda = {'EDID':quest, 'INDX':stages, 'QSTA':targets}
            self.ctda = quest #--Which ctda element loader to use next.
        def __getitem__(self,key):
            if key == 'CTDA': return self.ctda
            self.ctda = self.type_ctda.get(key, self.ctda)
            return self.data[key]

    class MelQustData(MelStruct):
        """Handle older trucated DATA for QUST subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 8:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 2:
                #--Else 2 byte record
                unpacked = ins.unpack('BB',size,readId)
            else:
                raise "Unexpected size encountered for QUST:DATA subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked

    #--MelSet
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('SCRI','script'),
        MelString('FULL','full'),
        MelString('ICON','iconPath'),
        MelQustData('DATA','=BB2sf',(_questFlags,'questFlags',0),('priority',0),('unused2',null2),('questDelay',0.0)),
        MelConditions(),
        MelGroups('stages',
            MelStruct('INDX','h','stage'),
            MelGroups('entries',
                MelStruct('QSDT','B',(stageFlags,'flags')),
                MelConditions(),
                MelString('CNAM','text'),
                MelStruct('SCHR','4s4I',('unused1',null4),'numRefs','compiledSize','lastIndex','scriptType'),
                MelBase('SCDA','compiled_p'),
                MelString('SCTX','scriptText'),
                MelGroups('vars',
                    MelStruct('SLSD','I12sB7s','index',('unused1',null4+null4+null4),(_variableFlags,'flags',0L),('unused2',null4+null3)),
                    MelString('SCVR','name')),
                MelScrxen('SCRV/SCRO','references'),
                MelFid('NAM0', 'nextQuest'),
                ),
            ),
        MelGroups('objectives',
             MelStruct('QOBJ','i','index'),
             MelString('NNAM','description'),
             MelGroups('targets',
                 MelStruct('QSTA','IB3s',(FID,'targetId'),(targetFlags,'flags'),('unused1',null3)),
                 MelConditions(),
                 ),
             ),
        )
    melSet.loaders = MelQustLoaders(melSet.loaders,*melSet.elements[5:8])
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreRace(MelRecord):
    """Race record.

    This record is complex to read and write. Relatively simple problems are the VNAM
    which can be empty or zeroed depending on relationship between voices and
    the fid for the race.

    The face and body data is much more complicated, with the same subrecord types
    mapping to different attributes depending on preceding flag subrecords (NAM0, NAM1,
    NMAN, FNAM and INDX.) These are handled by using the MelRaceDistributor class
    to dynamically reassign melSet.loaders[type] as the flag records are encountered.

    It's a mess, but this is the shortest, clearest implementation that I could
    think of."""

    classType = 'RACE'
    _flags = Flags(0L,Flags.getNames('playable','child'))

    class MelRaceVoices(MelStruct):
        """Set voices to zero, if equal race fid. If both are zero, then don't skip dump."""
        def dumpData(self,record,out):
            if record.maleVoice == record.fid: record.maleVoice = 0L
            if record.femaleVoice == record.fid: record.femaleVoice = 0L
            if (record.maleVoice,record.femaleVoice) != (0,0):
                MelStruct.dumpData(self,record,out)

    class MelRaceHeadModel(MelGroup):
        """Most face data, like a MelModel + ICON + MICO. Load is controlled by MelRaceDistributor."""
        def __init__(self,attr,index):
            MelGroup.__init__(self,attr,
                MelString('MODL','modPath'),
                MelBase('MODB','modb_p'),
                MelBase('MODT','modt_p'),
                MelBase('MODS','mods_p'),
                MelOptStruct('MODD','B','modd_p'),
                MelString('ICON','iconPath'),
                MelBase('MICO','mico'))
            self.index = index
        def dumpData(self,record,out):
            out.packSub('INDX','I',self.index)
            MelGroup.dumpData(self,record,out)

    class MelRaceBodyModel(MelGroup):
        """Most body data, like a MelModel - MODB + ICON + MICO. Load is controlled by MelRaceDistributor."""
        def __init__(self,attr,index):
            MelGroup.__init__(self,attr,
                MelString('ICON','iconPath'),
                MelBase('MICO','mico'),
                MelString('MODL','modPath'),
                MelBase('MODT','modt_p'),
                MelBase('MODS','mods_p'),
                MelOptStruct('MODD','B','modd_p'))
            self.index = index
        def dumpData(self,record,out):
            out.packSub('INDX','I',self.index)
            MelGroup.dumpData(self,record,out)

    class MelRaceIcon(MelString):
        """Most body data plus eyes for face. Load is controlled by MelRaceDistributor."""
        def __init__(self,attr,index):
            MelString.__init__(self,'ICON',attr)
            self.index = index
        def dumpData(self,record,out):
            out.packSub('INDX','I',self.index)
            MelString.dumpData(self,record,out)

    class MelRaceFaceGen(MelGroup):
        """Most fecegen data. Load is controlled by MelRaceDistributor."""
        def __init__(self,attr):
            MelGroup.__init__(self,attr,
                MelBase('FGGS','fggs_p'), ####FaceGen Geometry-Symmetric
                MelBase('FGGA','fgga_p'), ####FaceGen Geometry-Asymmetric
                MelBase('FGTS','fgts_p'), ####FaceGen Texture-Symmetric
                MelStruct('SNAM','2s',('snam_p',null2)))

    class MelRaceDistributor(MelNull):
        """Handles NAM0, NAM1, MNAM, FMAN and INDX records. Distributes load
        duties to other elements as needed."""
        def __init__(self):
            headAttrs = ('Head', 'Ears', 'Mouth', 'TeethLower', 'TeethUpper', 'Tongue', 'LeftEye', 'RightEye')
            bodyAttrs = ('UpperBody','LeftHand','RightHand','UpperBodyTexture')
            self.headModelAttrs = {
                'MNAM':tuple('male'+text for text in headAttrs),
                'FNAM':tuple('female'+text for text in headAttrs),
                }
            self.bodyModelAttrs = {
                'MNAM':tuple('male'+text for text in bodyAttrs),
                'FNAM':tuple('female'+text for text in bodyAttrs),
                }
            self.attrs = {
                'NAM0':self.headModelAttrs,
                'NAM1':self.bodyModelAttrs
                }
            self.facegenAttrs = {'MNAM':'maleFaceGen','FNAM':'femaleFaceGen'}
            self._debug = False

        def getSlotsUsed(self):
            return ('_loadAttrs','_modelAttrs')

        def getLoaders(self,loaders):
            """Self as loader for structure types."""
            for type in ('NAM0','NAM1','MNAM','FNAM','INDX'):
                loaders[type] = self

        def setMelSet(self,melSet):
            """Set parent melset. Need this so that can reassign loaders later."""
            self.melSet = melSet
            self.loaders = {}
            for element in melSet.elements:
                attr = element.__dict__.get('attr',None)
                if attr: self.loaders[attr] = element

        def loadData(self,record,ins,type,size,readId):
            if type in ('NAM0','NAM1'):
                record._modelAttrs = self.attrs[type]
                return
            elif type in ('MNAM','FNAM'):
                record._loadAttrs = record._modelAttrs[type]
                attr = self.facegenAttrs.get(type)
                element = self.loaders[attr]
                for type in ('FGGS','FGGA','FGTS','SNAM'):
                    self.melSet.loaders[type] = element
            else: #--INDX
                index, = ins.unpack('I',4,readId)
                attr = record._loadAttrs[index]
                element = self.loaders[attr]
                for type in ('MODL','MODB','MODT','MODS','MODD','ICON','MICO'):
                    self.melSet.loaders[type] = element

    #--Mel Set
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('DESC','text'),
        MelStruct('DATA','14b2s4fI','skill1','skill1Boost','skill2','skill2Boost',
                  'skill3','skill3Boost','skill4','skill4Boost','skill5','skill5Boost',
                  'skill6','skill6Boost','skill7','skill7Boost',('unused1',null2),
                  'maleHeight','femaleHeight','maleWeight','femaleWeight',(_flags,'flags',0L)),
        MelFid('ONAM','Older'),
        MelFid('YNAM','Younger'),
        MelBase('NAM2','_nam2',''),
        MelRaceVoices('VTCK','2I',(FID,'maleVoice'),(FID,'femaleVoice')), #--0 same as race fid.
        MelOptStruct('DNAM','2I',(FID,'defaultHairMale',0L),(FID,'defaultHairFemale',0L)), #--0=None
        MelStruct('CNAM','2B','defaultHairColorMale','defaultHairColorFemale'), #--Int corresponding to GMST sHairColorNN
        MelOptStruct('PNAM','f','mainClamp'),
        MelOptStruct('UNAM','f','faceClamp'),
        MelStruct('ATTR','2B','maleBaseAttribute','femaleBaseAttribute'),
        #--Begin Indexed entries
        MelBase('NAM0','_nam0',''), ####Face Data Marker, wbEmpty
        MelBase('MNAM','_mnam',''),
        MelRaceHeadModel('maleHead',0),
        MelRaceIcon('maleEars',1),
        MelRaceHeadModel('maleMouth',2),
        MelRaceHeadModel('maleTeethLower',3),
        MelRaceHeadModel('maleTeethUpper',4),
        MelRaceHeadModel('maleTongue',5),
        MelRaceHeadModel('maleLeftEye',6),
        MelRaceHeadModel('maleRightEye',7),
        MelBase('FNAM','_fnam',''),
        MelRaceHeadModel('femaleHead',0),
        MelRaceIcon('femaleEars',1),
        MelRaceHeadModel('femaleMouth',2),
        MelRaceHeadModel('femaleTeethLower',3),
        MelRaceHeadModel('femaleTeethUpper',4),
        MelRaceHeadModel('femaleTongue',5),
        MelRaceHeadModel('femaleLeftEye',6),
        MelRaceHeadModel('femaleRightEye',7),
        MelBase('NAM1','_nam1',''), ####Body Data Marker, wbEmpty
        MelBase('MNAM','_mnam',''), ####Male Body Data Marker, wbEmpty
        MelRaceBodyModel('maleUpperBody',0),
        MelRaceBodyModel('maleLeftHand',1),
        MelRaceBodyModel('maleRightHand',2),
        MelRaceBodyModel('maleUpperBodyTexture',3),
        MelBase('FNAM','_fnam',''), ####Female Body Data Marker, wbEmpty
        MelRaceBodyModel('femaleUpperBody',0),
        MelRaceBodyModel('femaleLeftHand',1),
        MelRaceBodyModel('femaleRightHand',2),
        MelRaceBodyModel('femaleUpperBodyTexture',3),
        #--Normal Entries
        MelFidList('HNAM','hairs'),
        MelFidList('ENAM','eyes'),
        #--FaceGen Entries
        MelBase('MNAM','_mnam',''),
        MelRaceFaceGen('maleFaceGen'),
        MelBase('FNAM','_fnam',''),
        MelRaceFaceGen('femaleFaceGen'),
        #--Distributor for face and body entries.
        MelRaceDistributor(),
        )
    melSet.elements[-1].setMelSet(melSet)
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreRefr(MelRecord):
    classType = 'REFR'
    _flags = Flags(0L,Flags.getNames('visible', 'canTravelTo'))
    _parentFlags = Flags(0L,Flags.getNames('oppositeParent'))
    _actFlags = Flags(0L,Flags.getNames('useDefault', 'activate','open','openByDefault'))
    _lockFlags = Flags(0L,Flags.getNames(None, None, 'leveledLock'))
    _destinationFlags = Flags(0L,Flags.getNames('noAlarm'))
    _variableFlags = Flags(0L,Flags.getNames('isLongOrShort'))
    class MelRefrXloc(MelOptStruct):
        """Handle older trucated XLOC for REFR subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 20:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            #elif size == 16:
            #    unpacked = ins.unpack('B3sIB3s',size,readId)
            elif size == 12:
                unpacked = ins.unpack('B3sI4s',size,readId)
            else:
                print ins.unpack(('%dB' % size),size)
                raise ModError(ins.inName,_('Unexpected size encountered for REFR:XLOC subrecord: ')+str(size))
            unpacked = unpacked[:-2] + self.defaults[len(unpacked)-2:-2] + unpacked[-2:]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked

    class MelRefrXmrk(MelStruct):
        """Handler for xmrk record. Conditionally loads next items."""
        def loadData(self,record,ins,type,size,readId):
            """Reads data from ins into record attribute."""
            junk = ins.read(size,readId)
            record.hasXmrk = True
            insTell = ins.tell
            insUnpack = ins.unpack
            pos = insTell()
            (type,size) = insUnpack('4sH',6,readId+'.FULL')
            while type in ['FNAM','FULL','TNAM','WMI1']:
                if type == 'FNAM':
                    value = insUnpack('B',size,readId)
                    record.flags = MreRefr._flags(*value)
                elif type == 'FULL':
                    record.full = ins.readString(size,readId)
                elif type == 'TNAM':
                    record.markerType, record.unused5 = insUnpack('Bs',size,readId)
                elif type == 'WMI1':
                    record.reputation = insUnpack('I',size,readId)
                pos = insTell()
                (type,size) = insUnpack('4sH',6,readId+'.FULL')
            ins.seek(pos)
            if self._debug: print ' ',record.flags,record.full,record.markerType
        def dumpData(self,record,out):
            if (record.flags,record.full,record.markerType,record.unused5,record.reputation) != self.defaults[1:]:
                record.hasXmrk = True
            if record.hasXmrk:
                try:
                    out.write(struct.pack('=4sH','XMRK',0))
                    out.packSub('FNAM','B',record.flags.dump())
                    value = record.full
                    if value != None:
                        out.packSub0('FULL',value)
                    out.packSub('TNAM','Bs',record.markerType, record.unused5)
                    out.packRef('WMI1',record.reputation)
                except struct.error:
                    print self.subType,self.format,record.flags,record.full,record.markerType
                    raise

    melSet = MelSet(
        MelString('EDID','eid'),
        MelOptStruct('RCLR','8B','referenceStartColorRed','referenceStartColorGreen','referenceStartColorBlue',('referenceColorUnused1',null1),
                     'referenceEndColorRed','referenceEndColorGreen','referenceEndColorBlue',('referenceColorUnused2',null1)),
        MelFid('NAME','base'),
        MelFid('XEZN','encounterZone'),
        MelBase('XRGD','ragdollData'),
        MelBase('XRGB','ragdollBipedData'),
        MelOptStruct('XPRM','3f3IfI','primitiveBoundX','primitiveBoundY','primitiveBoundX',
                     'primitiveColorRed','primitiveColorGreen','primitiveColorBlue','primitiveUnknown','primitiveType'),
        MelOptStruct('XTRI','I','collisionLayer'),
        MelBase('XMBP','multiboundPrimitiveMarker'),
        MelOptStruct('XMBO','3f','boundHalfExtentsX','boundHalfExtentsY','boundHalfExtentsZ'),
        MelOptStruct('XTEL','I6fI',(FID,'destinationFid'),'destinationPosX','destinationPosY',
            'destinationPosZ','destinationRotX','destinationRotY','destinationRotZ',(_destinationFlags,'destinationFlags')),
        MelRefrXmrk('XMRK','',('hasXmrk',False),(_flags,'flags',0L),'full','markerType',('unused5',null1),(FID,'reputation')), ####Map Marker Start Marker, wbEmpty
        MelGroup('audioData',
            MelBase('MMRK','audioMarker'),
            MelBase('FULL','full_p'),
            MelFid('CNAM','audioLocation'),
            MelBase('BNAM','bnam_p'),
            MelBase('MNAM','mnam_p'),
            MelBase('NNAM','nnam_p'),
            ),
        MelBase('XSRF','xsrf_p'),
        MelBase('XSRD','xsrd_p'),
        MelFid('XTRG','targetId'),
        MelOptStruct('XLCM','i',('levelMod',None)),
        MelGroup('patrolData',
            MelStruct('XPRD','f','idleTime'),
            MelBase('XPPA','patrolScriptMarker'),
            MelFid('INAM', 'idle'),
            MelStruct('SCHR','4s4I',('unused1',null4),'numRefs','compiledSize','lastIndex','scriptType'),
            MelBase('SCDA','compiled_p'),
            MelString('SCTX','scriptText'),
            MelGroups('vars',
                MelStruct('SLSD','I12sB7s','index',('unused1',null4+null4+null4),(_variableFlags,'flags',0L),('unused2',null4+null3)),
                MelString('SCVR','name')),
            MelScrxen('SCRV/SCRO','references'),
            MelFid('TNAM','topic'),
            ),
        MelOptStruct('XRDO','fIfI','rangeRadius','broadcastRangeType','staticPercentage',(FID,'positionReference')),
        MelOwnership(),
        MelRefrXloc('XLOC','B3sI4sB3s4s','lockLevel',('unused1',null3),(FID,'lockKey'),('unused2',null4),(_lockFlags,'lockFlags'),('unused3',null3),('unused4',null4)),
        MelOptStruct('XCNT','i','count'),
        MelOptStruct('XRDS','f','radius'),
        MelOptStruct('XHLP','f','health'),
        MelOptStruct('XRAD','f','radiation'),
        MelOptStruct('XCHG','f',('charge',None)),
        MelGroup('ammo',
            MelFid('XAMT','type'),
            MelStruct('XAMC','I','count'),
            ),
        MelStructs('XPWR','II','reflectedByWaters',(FID,'reference'),'type'),
        MelFids('XLTW','litWaters'),
        MelStructs('XDCR','II','linkedDecals',(FID,'reference'),'unknown'), # ??
        MelFid('XLKR','linkedReference'),
        MelOptStruct('XCLP','8B','linkStartColorRed','linkStartColorGreen','linkStartColorBlue',('linkColorUnused1',null1),
                     'linkEndColorRed','linkEndColorGreen','linkEndColorBlue',('linkColorUnused2',null1)),
        MelGroup('activateParents',
            MelStruct('XAPD','B','flags'),
            MelStructs('XAPR','If','activateParentRefs',(FID,'reference'),'delay')
            ),
        MelString('XATO','activationPrompt'),
        MelOptStruct('XESP','IB3s',(FID,'parent'),(_parentFlags,'parentFlags'),('unused6',null3)),
        MelOptStruct('XEMI','I',(FID,'emitance')),
        MelFid('XMBR','multiboundReference'),
        MelOptStruct('XACT','I',(_actFlags,'actFlags',0L)), ####Action Flag
        MelBase('ONAM','onam_p'), ####Open by Default, wbEmpty
        MelBase('XIBS','ignoredBySandbox'),
        MelOptStruct('XNDP','2I',(FID,'navMesh'),'unknown'),
        MelOptStruct('XPOD','II',(FID,'portalDataRoom0'),(FID,'portalDataRoom1')),
        MelOptStruct('XPTL','9f','portalWidth','portalHeight','portalPosX','portalPosY','portalPosZ',
                     'portalRot1','portalRot2','portalRot3','portalRot4'),
        MelBase('XSED','speedTreeSeed'),
        ####SpeedTree Seed, if it's a single byte then it's an offset into the list of seed values in the TREE record
        ####if it's 4 byte it's the seed value directly.
        MelGroup('roomData',
            MelStruct('XRMR','H2s','linkedRoomsCount','unknown'),
            MelFids('XLRM','linkedRoom'),
            ),
        MelOptStruct('XOCP','9f','occlusionPlaneWidth','occlusionPlaneHeight','occlusionPlanePosX','occlusionPlanePosY','occlusionPlanePosZ',
                     'occlusionPlaneRot1','occlusionPlaneRot2','occlusionPlaneRot3','occlusionPlaneRot4'),
        MelOptStruct('XORD','4I',(FID,'linkedOcclusionPlane0'),(FID,'linkedOcclusionPlane1'),(FID,'linkedOcclusionPlane2'),(FID,'linkedOcclusionPlane3')),
        MelOptStruct('XLOD','3f',('lod1',None),('lod2',None),('lod3',None)), ####Distant LOD Data, unknown
        MelOptStruct('XSCL','f',('scale',1.0)),
        MelOptStruct('DATA','=6f',('posX',None),('posY',None),('posZ',None),('rotX',None),('rotY',None),('rotZ',None)),

        ##Oblivion subrecords
        #MelOptStruct('XHLT','i',('health',None)),
        #MelXpci('XPCI'), ####fid, unknown
        #MelFid('XRTM','xrtm'), ####unknown
        #MelOptStruct('XSOL','B',('soul',None)), ####Was entirely missing. Confirmed by creating a test mod...it isn't present in any of the official esps
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreRegn(MelRecord):
    """Region record."""
    classType = 'REGN'
    _flags = Flags(0L,Flags.getNames(
        ( 2,'objects'),
        ( 3,'weather'),
        ( 4,'map'),
        ( 6,'grass'),
        ( 7,'sound'),))
    obflags = Flags(0L,Flags.getNames(
        ( 0,'conform'),
        ( 1,'paintVertices'),
        ( 2,'sizeVariance'),
        ( 3,'deltaX'),
        ( 4,'deltaY'),
        ( 5,'deltaZ'),
        ( 6,'Tree'),
        ( 7,'hugeRock'),))
    sdflags = Flags(0L,Flags.getNames(
        ( 0,'pleasant'),
        ( 1,'cloudy'),
        ( 2,'rainy'),
        ( 3,'snowy'),))

    ####Lazy hacks to correctly read/write regn data
    class MelRegnStructA(MelStructA):
        """Handler for regn record. Conditionally dumps next items."""
        def loadData(self,record,ins,type,size,readId):
            if record.entryType == 2 and self.subType == 'RDOT':
                MelStructA.loadData(self,record,ins,type,size,readId)
            elif record.entryType == 3 and self.subType == 'RDWT':
                MelStructA.loadData(self,record,ins,type,size,readId)
            elif record.entryType == 6 and self.subType == 'RDGS':
                MelStructA.loadData(self,record,ins,type,size,readId)
            elif record.entryType == 7 and self.subType == 'RDSD':
                MelStructA.loadData(self,record,ins,type,size,readId)

        def dumpData(self,record,out):
            """Conditionally dumps data."""
            if record.entryType == 2 and self.subType == 'RDOT':
                MelStructA.dumpData(self,record,out)
            elif record.entryType == 3 and self.subType == 'RDWT':
                MelStructA.dumpData(self,record,out)
            elif record.entryType == 6 and self.subType == 'RDGS':
                MelStructA.dumpData(self,record,out)
            elif record.entryType == 7 and self.subType == 'RDSD':
                MelStructA.dumpData(self,record,out)

    class MelRegnString(MelString):
        """Handler for regn record. Conditionally dumps next items."""
        def loadData(self,record,ins,type,size,readId):
            if record.entryType == 4 and self.subType == 'RDMP':
                MelString.loadData(self,record,ins,type,size,readId)
            elif record.entryType == 5 and self.subType == 'ICON':
                MelString.loadData(self,record,ins,type,size,readId)

        def dumpData(self,record,out):
            """Conditionally dumps data."""
            if record.entryType == 4 and self.subType == 'RDMP':
                MelString.dumpData(self,record,out)
            elif record.entryType == 5 and self.subType == 'ICON':
                MelString.dumpData(self,record,out)

    class MelRegnOptStruct(MelOptStruct):
        """Handler for regn record. Conditionally dumps next items."""
        def loadData(self,record,ins,type,size,readId):
            if record.entryType == 7 and self.subType == 'RDMD':
                MelOptStruct.loadData(self,record,ins,type,size,readId)

        def dumpData(self,record,out):
            """Conditionally dumps data."""
            if record.entryType == 7 and self.subType == 'RDMD':
                MelOptStruct.dumpData(self,record,out)

    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelStruct('RCLR','3Bs','mapRed','mapBlue','mapGreen',('unused1',null1)),
        MelFid('WNAM','worldspace'),
        MelGroups('areas',
            MelStruct('RPLI','I','edgeFalloff'),
            MelStructA('RPLD','2f','points','posX','posY')),
        MelGroups('entries',
            MelStruct('RDAT', 'I2B2s','entryType', (_flags,'flags'), 'priority', ('unused1',null2)), ####flags actually an enum...
            MelRegnStructA('RDOT', 'IH2sf4B2H4s4f3H2s4s', 'objects', (FID,'objectId'), 'parentIndex',
            ('unused1',null2), 'density', 'clustering', 'minSlope', 'maxSlope',
            (obflags, 'flags'), 'radiusWRTParent', 'radius', ('unk1',null4),
            'maxHeight', 'sink', 'sinkVar', 'sizeVar', 'angleVarX',
            'angleVarY',  'angleVarZ', ('unused2',null2), ('unk2',null4)),
            MelRegnString('RDMP', 'mapName'),
            #MelRegnString('ICON', 'iconPath'),  ####Obsolete? Only one record in Fallout3.esm
            MelRegnStructA('RDGS', 'I4s', 'grasses', (FID,'grass'), ('unk1',null4)),
            MelRegnOptStruct('RDMD', 'I', ('musicType',None)),
            MelRegnStructA('RDSD', '3I', 'sounds', (FID, 'sound'), (sdflags, 'flags'), 'chance'),
            MelRegnStructA('RDWT', '3I', 'weather', (FID, 'weather', None), 'chance', (FID, 'global', None))),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreRoad(MelRecord):
    """Road structure. Part of large worldspaces."""
    ####Could probably be loaded via MelStructA,
    ####but little point since it is too complex to manipulate
    classType = 'ROAD'
    melSet = MelSet(
        MelBase('PGRP','points_p'),
        MelBase('PGRR','connections_p'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreSbsp(MelRecord):
    """Subspace record."""
    classType = 'SBSP'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DNAM','3f','sizeX','sizeY','sizeZ'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreScpt(MelRecord):
    """Script record."""
    classType = 'SCPT'
    _flags = Flags(0L,Flags.getNames('isLongOrShort'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('SCHR','4s4I',('unused1',null4),'numRefs','compiledSize','lastIndex','scriptType'),
        #--Type: 0: Object, 1: Quest, 0x100: Magic Effect
        MelBase('SCDA','compiled_p'),
        MelString('SCTX','scriptText'),
        MelGroups('vars',
            MelStruct('SLSD','I12sB7s','index',('unused1',null4+null4+null4),(_flags,'flags',0L),('unused2',null4+null3)),
            MelString('SCVR','name')),
        MelScrxen('SCRV/SCRO','references'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreSgst(MelRecord,MreHasEffects):
    """Sigil stone record."""
    classType = 'SGST'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFull0(),
        MelModel(),
        MelString('ICON','iconPath'),
        MelFid('SCRI','script'),
        MelEffects(),
        MelStruct('DATA','=BIf','uses','value','weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreSkil(MelRecord):
    """Skill record."""
    classType = 'SKIL'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('INDX','i','skill'),
        MelString('DESC','description'),
        MelString('ICON','iconPath'),
        MelStruct('DATA','2iI2f','action','attribute','specialization',('use0',1.0),'use1'),
        MelString('ANAM','apprentice'),
        MelString('JNAM','journeyman'),
        MelString('ENAM','expert'),
        MelString('MNAM','master'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreSlgm(MelRecord):
    """Soul gem record."""
    classType = 'SLGM'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelModel(),
        MelString('ICON','iconPath'),
        MelFid('SCRI','script'),
        MelStruct('DATA','If','value','weight'),
        MelStruct('SOUL','B',('soul',0)),
        MelStruct('SLCP','B',('capacity',1)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreSoun(MelRecord):
    """Sound record."""
    classType = 'SOUN'
    _flags = Flags(0L,Flags.getNames('randomFrequencyShift', 'playAtRandom',
        'environmentIgnored', 'randomLocation', 'loop','menuSound', '2d', '360LFE'))
    class MelSounSndx(MelStruct):
        """SNDX is a reduced version of SNDD. Allow it to read in, but not set defaults or write."""
        def loadData(self,record,ins,type,size,readId):
            MelStruct.loadData(self,record,ins,type,size,readId)
            record.point0 = 0
            record.point1 = 0
            record.point2 = 0
            record.point3 = 0
            record.point4 = 0
            record.reverb = 0
            record.priority = 0
            record.unknown = "\0"*8
        def getSlotsUsed(self):
            return ()
        def setDefault(self,record): return
        def dumpData(self,record,out): return
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FNAM','soundFile'),
        MelOptStruct('SNDD','=2BbsIh2B6HI8s',('minDistance',None), ('maxDistance',None), ('freqAdjustment',None), ('unused1',null1),
            (_flags,'flags',None),('staticAtten',None),('stopTime',None),('startTime',None),
            ('point0',0),('point1',0),('point2',0),('point3',0),('point4',0),('reverb',0),('priority',0),'unknown'),
        MelSounSndx('SNDX','=2BbsIh2B',('minDistance',None), ('maxDistance',None), ('freqAdjustment',None), ('unused1',null1),
            (_flags,'flags',None),('staticAtten',None),('stopTime',None),('startTime',None),),
        MelBase('ANAM','_anam'), #--Should be a struct. Maybe later.
        MelBase('GNAM','_gnam'), #--Should be a struct. Maybe later.
        MelBase('HNAM','_hnam'), #--Should be a struct. Maybe later.
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreSpel(MelRecord,MreHasEffects):
    """Spell record."""
    classType = 'SPEL'
    class SpellFlags(Flags):
        """For SpellFlags, immuneSilence activates bits 1 AND 3."""
        def __setitem__(self,index,value):
            setter = Flags.__setitem__
            setter(self,index,value)
            if index == 1:
                setter(self,3,value)
    flags = SpellFlags(0L,Flags.getNames('noAutoCalc', 'immuneToSilence',
        'startSpell', None,'ignoreLOS','scriptEffectAlwaysApplies','disallowAbsorbReflect','touchExplodesWOTarget'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFull0(),
        MelStruct('SPIT','3IB3s','spellType','cost','level',(flags,'flags',0L),('unused1',null3)),
        # spellType = 0: Spell, 1: Disease, 3: Lesser Power, 4: Ability, 5: Poison
        MelEffects(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreStat(MelRecord):
    """Static model record."""
    classType = 'STAT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelModel(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreTes4(MelRecord):
    """TES4 Record. File header."""
    classType = 'TES4' #--Used by LoadFactory
    #--Masters array element
    class MelTes4Name(MelBase):
        def setDefault(self,record):
            record.masters = []
        def loadData(self,record,ins,type,size,readId):
            name = GPath(ins.readString(size,readId))
            record.masters.append(name)
        def dumpData(self,record,out):
            pack1 = out.packSub0
            pack2 = out.packSub
            for name in record.masters:
                pack1('MAST',name.s)
                pack2('DATA','Q',0)
    #--Data elements
    melSet = MelSet(
        MelStruct('HEDR','f2I',('version',0.8),'numRecords',('nextObject',0xCE6)),
        MelBase('OFST','ofst_p',), #--Obsolete?
        MelBase('DELE','dele_p'), #--Obsolete?
        MelString('CNAM','author','',512),
        MelString('SNAM','description','',512),
        MelTes4Name('MAST','masters'),
        MelBase('ONAM','onam'),
        MelNull('DATA'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

    def getNextObject(self):
        """Gets next object index and increments it for next time."""
        self.changed = True
        self.nextObject += 1
        return (self.nextObject -1)

#------------------------------------------------------------------------------
class MreTree(MelRecord):
    """Tree record."""
    classType = 'TREE'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelModel(),
        MelString('ICON','iconPath'),
        MelStructA('SNAM','I','speedTree','seed'),
        MelStruct('CNAM','5fi2f', 'curvature','minAngle','maxAngle',
                  'branchDim','leafDim','shadowRadius','rockSpeed',
                  'rustleSpeed'),
        MelStruct('BNAM','2f','widthBill','heightBill'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreWatr(MelRecord):
    """Water record."""
    classType = 'WATR'
    _flags = Flags(0L,Flags.getNames('causesDmg','reflective'))
    class MelWatrData(MelStruct):
        """Handle older trucated DATA for WATR subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 186:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 2:
                (record.damage,) = ins.unpack('H',size,readId)
                return
            else:
                raise "Unexpected size encountered for WATR subrecord: %s" % size
        def dumpData(self,record,out):
            out.packSub(self.subType,'H',record.damage)

    class MelWatrDnam(MelStruct):
        """Handle older trucated DNAM for WATR subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 196:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 184:
                unpacked = ins.unpack('10f3Bs3Bs3BsI32f',size,readId)
            else:
                raise ModError(ins.inName,_('Unexpected size encountered for WATR subrecord: ')+str(size))
            unpacked = unpacked[:-1]
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked

    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('NNAM','texture'),
        MelStruct('ANAM','B','opacity'),
        MelStruct('FNAM','B',(_flags,'flags',0)),
        MelString('MNAM','material'),
        MelFid('SNAM','sound'),
        MelFid('XNAM','effect'),
        MelWatrData('DATA','10f3Bs3Bs3BsI32fH',('windVelocity',0.100),('windDirection',90.0),
            ('waveAmp',0.5),('waveFreq',1.0),('sunPower',50.0),('reflectAmt',0.5),
            ('fresnelAmt',0.0250),('unknown1',0.0),('fogNear',27852.8),('fogFar',163840.0),
            ('shallowRed',0),('shallowGreen',128),('shallowBlue',128),('unused1',null1),
            ('deepRed',0),('deepGreen',0),('deepBlue',25),('unused2',null1),
            ('reflRed',255),('reflGreen',255),('reflBlue',255),('unused3',null1),
            ('unknown2',0),
            ('rainForce',0.1000),('rainVelocity',0.6000),('rainFalloff',0.9850),('rainDampner',2.0000),('rainSize',0.0100),
            ('dispForce',0.4000),('dispVelocity', 0.6000),('dispFalloff',0.9850),('dispDampner',10.0000),('dispSize',0.0500),
            ('noiseNormalsScale',1.8000),('noiseLayer1WindDirection',0.0000),('noiseLayer2WindDirection',-431602080.0500),
            ('noiseLayer3WindDirection',-431602080.0500),('noiseLayer1WindVelocity',0.0000),
            ('noiseLayer2WindVelocity',-431602080.0500),('noiseLayer3WindVelocity',-431602080.0500),
            ('noiseNormalsDepthFalloffStart',0.00000),('noiseNormalsDepthFalloffEnd',0.10000),
            ('fogAboveWaterAmount',1.00000),('noiseNormalsUvScale',500.00000),
            ('fogUnderWaterAmount',1.00000),('fogUnderWaterNear',0.00000),('fogUnderWaterFar',1000.00000),
            ('distortionAmount',250.00000),('shininess',100.00000),('reflectHdrMult',1.00000),
            ('lightRadius',10000.00000),('lightBrightness',1.00000),
            ('noiseLayer1UvScale',100.00000),('noiseLayer2UvScale',100.00000),('noiseLayer3UvScale',100.00000),
            ('damage',0)),
        MelWatrDnam('DNAM','10f3Bs3Bs3BsI35f',('windVelocity',0.100),('windDirection',90.0),
            ('waveAmp',0.5),('waveFreq',1.0),('sunPower',50.0),('reflectAmt',0.5),
            ('fresnelAmt',0.0250),('unknown1',0.0),('fogNear',27852.8),('fogFar',163840.0),
            ('shallowRed',0),('shallowGreen',128),('shallowBlue',128),('unused1',null1),
            ('deepRed',0),('deepGreen',0),('deepBlue',25),('unused2',null1),
            ('reflRed',255),('reflGreen',255),('reflBlue',255),('unused3',null1),
            ('unknown2',0),
            ('rainForce',0.1000),('rainVelocity',0.6000),('rainFalloff',0.9850),('rainDampner',2.0000),('rainSize',0.0100),
            ('dispForce',0.4000),('dispVelocity', 0.6000),('dispFalloff',0.9850),('dispDampner',10.0000),('dispSize',0.0500),
            ('noiseNormalsScale',1.8000),('noiseLayer1WindDirection',0.0000),('noiseLayer2WindDirection',-431602080.0500),
            ('noiseLayer3WindDirection',-431602080.0500),('noiseLayer1WindVelocity',0.0000),
            ('noiseLayer2WindVelocity',-431602080.0500),('noiseLayer3WindVelocity',-431602080.0500),
            ('noiseNormalsDepthFalloffStart',0.00000),('noiseNormalsDepthFalloffEnd',0.10000),
            ('fogAboveWaterAmount',1.00000),('noiseNormalsUvScale',500.00000),
            ('fogUnderWaterAmount',1.00000),('fogUnderWaterNear',0.00000),('fogUnderWaterFar',1000.00000),
            ('distortionAmount',250.00000),('shininess',100.00000),('reflectHdrMult',1.00000),
            ('lightRadius',10000.00000),('lightBrightness',1.00000),
            ('noiseLayer1UvScale',100.00000),('noiseLayer2UvScale',100.00000),('noiseLayer3UvScale',100.00000),
            ('noiseLayer1Amp',0.00000),('noiseLayer2Amp',0.00000),('noiseLayer3Amp',0.00000),
            ),
        MelFidList('GNAM','relatedWaters'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreWeap(MelRecord):
    """Weapon record."""
    classType = 'WEAP'
    _flags = Flags(0L,Flags.getNames('notNormalWeapon'))
    _dflags1 = Flags(0L,Flags.getNames(
            'ignoresNormalWeaponResistance',
            'isAutomatic',
            'hasScope',
            'cantDrop',
            'hideBackpack',
            'embeddedWeapon',
            'dontUse1stPersonISAnimations',
            'nonPlayable',
        ))
    _dflags2 = Flags(0L,Flags.getNames(
            'playerOnly',
            'npcsUseAmmo',
            'noJamAfterReload',
            'overrideActionPoint',
            'minorCrime',
            'rangeFixed',
            'notUseInNormalCombat',
            'overrideDamageToWeaponMult',
            'dontUse3rdPersonISAnimations',
            'shortBurst',
            'RumbleAlternate',
            'longBurst',
            'unknown12','unknown13','unknown14','unknown15',
            'unknown16','unknown17','unknown18','unknown19',
            'unknown20','unknown21','unknown22','unknown23',
            'unknown24','unknown25','unknown26','unknown27',
            'unknown28','unknown29','unknown30','unknown31',
        ))
    _cflags = Flags(0L,Flags.getNames(
            'onDeath',
            'unknown1','unknown2','unknown3','unknown4',
            'unknown5','unknown6','unknown7',
        ))

    class MelWeapDnam(MelStruct):
        """Handle older trucated DNAM for WEAP subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 136:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 124:
                #--Else 124 byte record (skips sightUsage, semiAutomaticFireDelayMin and semiAutomaticFireDelayMax...
                unpacked = ins.unpack('Iff4B5fI4BffII11fIIffI',size,readId)
            elif size == 120:
                #--Else 120 byte record (skips resistType, sightUsage, semiAutomaticFireDelayMin and semiAutomaticFireDelayMax...
                unpacked = ins.unpack('Iff4B5fI4BffII11fIIff',size,readId)
            else:
                raise "Unexpected size encountered for WEAP:DNAM subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked

    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel('model'),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelFid('SCRI','script'),
        MelFid('EITM','effect'),
        MelOptStruct('EAMT','H', 'enchantment'),
        MelFid('NAM0','ammo'),
        MelDestructible(),
        MelFid('REPL','repairList'),
        #0:bigGuns,1:energyWeapons,2:smallGuns,3:meleeWeapons,4:unarmedWeapon,5:thrownWeapons,6:mine,
        MelStruct('ETYP','I','etype'),
        MelFid('BIPL','bipedModelList'),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        MelModel('shellCasingModel',2),
        MelModel('scopeModel',3),
        MelFid('EFSD','scopeEffect'),
        MelModel('worldModel',4),
        MelString('NNAM','embeddedWeaponNode'),
        MelFid('INAM','impactDataset'),
        MelFid('WNAM','firstPersonModel'),
        MelFid('SNAM','soundGunShot3D'),
        MelFid('XNAM','soundGunShot2D'),
        MelFid('NAM7','soundGunShot3DLooping'),
        MelFid('TNAM','soundMeleeSwingGunNoAmmo'),
        MelFid('NAM6','soundBlock'),
        MelFid('UNAM','idle'),
        MelFid('NAM9','equip'),
        MelFid('NAM8','unequip'),
        MelStruct('DATA','2IfHB','value','health','weight','damage','clipsize'),
        MelWeapDnam('DNAM','Iff4B5fI4BffII11fIIffIfff',
                    'animationType','animationMultiplier','reach',(_dflags1,'dnamFlags1',0L),
                    'gripAnimation','ammoUse','reloadAnimation','minSpread','spread',
                    'unknown','sightFov','unknown2',(FID,'projectile',0L),
                    'baseVatsToHitChance','attackAnimation','projectileCount','embeddedWeaponActorValue','minRange','maxRange',
                    'onHit',(_dflags2,'dnamFlags2',0L),'animationAttackMultiplier','fireRate','overrideActionPoint',
                    'rumbleLeftMotorStrength','rumbleRightMotorStrength','rumbleDuration','overrideDamageToWeaponMult',
                    'attackShotsPerSec','reloadTime','jamTime','aimArc','skill','rumblePattern','rambleWavelangth','limbDmgMult',
                    ('resistType',0xFFFFFFFF),'sightUsage','semiAutomaticFireDelayMin','semiAutomaticFireDelayMax'),
        MelStruct('CRDT','IfHI','criticalDamage','criticalMultiplier',(_cflags,'criticalFlags',0L),(FID,'criticalEffect',0L)),
        MelBase('VNAM','soundLevel'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreWrld(MelRecord):
    """Worldspace record."""
    classType = 'WRLD'
    _flags = Flags(0L,Flags.getNames('smallWorld','noFastTravel','oblivionWorldspace',None,'noLODWater','noLODNoise','noAllowNPCFallDamage'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelFid('XEZN','encounterZone'),
        MelFid('WNAM','parent'),
        MelOptStruct('PNAM','BB','parentFlags',('unknownff',0xff)),
        MelFid('CNAM','climate'),
        MelFid('NAM2','water'),
        MelFid('NAM3','waterType'),
        MelStruct('NAM4','f','waterHeight'),
        MelStruct('DNAM','ff','defaultLandHeight','defaultWaterHeight'),
        MelString('ICON','mapPath'),
        MelOptStruct('MNAM','2i4h',('dimX',None),('dimY',None),('NWCellX',None),('NWCellY',None),('SECellX',None),('SECellY',None)),
        MelStruct('ONAM','fff','worldMapScale','cellXOffset','cellYOffset'),
        MelFid('INAM','imageSpace'),
        MelStruct('DATA','B',(_flags,'flags',0L)),
        MelTuple('NAM0','ff','unknown0',(None,None)),
        MelTuple('NAM9','ff','unknown9',(None,None)),
        MelFid('ZNAM','music'),
        MelString('NNAM','canopyShadow'),
        MelString('XNAM','waterNoiseTexture'),
        MelStructs('IMPS','III','swappedImpacts', 'materialType',(FID,'old'),(FID,'new')),
        MelBase('IMPF','footstepMaterials'), #--todo rewrite specific class.
        MelBase('OFST','ofst_p'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreWthr(MelRecord):
    """Weather record."""
    classType = 'WTHR'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid("\x00IAD", 'sunriseImageSpaceModifier'),
        MelFid("\x01IAD", 'dayImageSpaceModifier'),
        MelFid("\x02IAD", 'sunsetImageSpaceModifier'),
        MelFid("\x03IAD", 'nightImageSpaceModifier'),
        MelString('DNAM','upperLayer'),
        MelString('CNAM','lowerLayer'),
        MelString('ANAM','layer2'),
        MelString('BNAM','layer3'),
        MelModel(),
        MelBase('LNAM','unknown1'),
        MelStruct('ONAM','4B','cloudSpeed0','cloudSpeed1','cloudSpeed3','cloudSpeed4'),
        MelBase('PNAM','_pnam'), #--RGB(3Bs) * 16?
        MelStructA('NAM0','3Bs3Bs3Bs3Bs','colors',
                   'riseRed','riseGreen','riseBlue',('unused1',null1),
                   'dayRed','dayGreen','dayBlue',('unused2',null1),
                   'setRed','setGreen','setBlue',('unused3',null1),
                   'nightRed','nightGreen','nightBlue',('unused4',null1),
                   ),
        MelStruct('FNAM','6f','fogDayNear','fogDayFar','fogNightNear','fogNightFar','fogDayPower','fogNightPower'),
        MelBase('INAM','_inam'), #--Should be a struct. Maybe later.
        MelStruct('DATA','15B',
            'windSpeed','lowerCloudSpeed','upperCloudSpeed','transDelta',
            'sunGlare','sunDamage','rainFadeIn','rainFadeOut','boltFadeIn',
            'boltFadeOut','boltFrequency','weatherType','boltRed','boltBlue','boltGreen'),
        MelStructs('SNAM','2I','sounds',(FID,'sound'),'type'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreTxst(MelRecord):
    """Texture set record."""
    classType = 'TXST'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('TX00','baseImage'),
        MelString('TX01','normalMap'),
        MelString('TX02','environmentMapMask'),
        MelString('TX03','growMap'),
        MelString('TX04','parallaxMap'),
        MelString('TX05','environmentMap'),
        MelOptStruct('DODT','7fBB2s3Bs','minWidth','maxWidth','minHeight','maxHeight','depth','shininess',
                     'parallaxScale','parallaxPasses','decalFlags',('unused1',null2),'red','green','blue',('unused2',null1)),
        MelStruct('DNAM','H','flags'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreMicn(MelRecord):
    """Menu icon record."""
    classType = 'MICN'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreFlst(MelRecord):
    """FormID list record."""
    classType = 'FLST'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFids('LNAM','fids'),
        )
    __slots__ = (MelRecord.__slots__ + melSet.getSlotsUsed() +
        ['mergeOverLast','mergeSources','items','deflsts'])

    def __init__(self,header,ins=None,unpack=False):
        """Initialize."""
        MelRecord.__init__(self,header,ins,unpack)
        self.mergeOverLast = False #--Merge overrides last mod merged
        self.mergeSources = None #--Set to list by other functions
        self.items  = None #--Set of items included in list
        self.deflsts = None #--Set of items deleted by list (Deflst mods)

    def mergeFilter(self,modSet):
        """Filter out items that don't come from specified modSet."""
        if not self.longFids: raise StateError(_("Fids not in long format"))
        self.fids = [fid for fid in self.fids if fid[0] in modSet]

    def mergeWith(self,other,otherMod):
        """Merges newLevl settings and entries with self.
        Requires that: self.items, other.deflsts be defined."""
        if not self.longFids: raise StateError(_("Fids not in long format"))
        if not other.longFids: raise StateError(_("Fids not in long format"))
        #--Remove items based on other.removes
        if other.deflsts:
            removeItems = self.items & other.deflsts
            #self.entries = [entry for entry in self.entries if entry.listId not in removeItems]
            self.fids = [fid for fid in self.fids if fid not in removeItems]
            self.items = (self.items | other.deflsts)
        hasOldItems = bool(self.items)
        #--Add new items from other
        newItems = set()
        fidsAppend = self.fids.append
        newItemsAdd = newItems.add
        for fid in other.fids:
            if fid not in self.items:
                fidsAppend(fid)
                newItemsAdd(fid)
        if newItems:
            self.items |= newItems
            #self.fids.sort(key=attrgetter('level'))
            self.fids.sort
        #--Is merged list different from other? (And thus written to patch.)
        if len(self.fids) != len(other.fids):
            self.mergeOverLast = True
        else:
            for selfEntry,otherEntry in zip(self.fids,other.fids):
                if selfEntry != otherEntry:
                    self.mergeOverLast = True
                    break
            else:
                self.mergeOverLast = False
        if self.mergeOverLast:
            self.mergeSources.append(otherMod)
        else:
            self.mergeSources = [otherMod]
        #--Done
        self.setChanged()

#------------------------------------------------------------------------------
class MrePerk(MelRecord):
    """Perk record."""
    classType = 'PERK'
    class MelPerkData(MelStruct):
        """Handle older trucated DATA for PERK subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 5:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 4:
                unpacked = ins.unpack('BBBB',size,readId)
            else:
                raise "Unexpected size encountered for DATA subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked, record.flagsA.getTrueAttrs()
    class MelPerkEffectData(MelBase):
        def hasFids(self,formElements):
            formElements.add(self)
        def loadData(self,record,ins,type,size,readId):
            target = MelObject()
            record.__setattr__(self.attr,target)
            if record.type == 0:
                format,attrs = ('II',('quest','queststage'))
            elif record.type == 1:
                format,attrs = ('I',('ability',))
            elif record.type == 2:
                format,attrs = ('HB',('entrypoint','function'))
            else:
                raise ModError(ins.inName,_('Unexpected type: %d') % record.type)
            unpacked = ins.unpack(format,size,readId)
            setter = target.__setattr__
            for attr,value in zip(attrs,unpacked):
                setter(attr,value)
            if self._debug: print unpacked
        def dumpData(self,record,out):
            target = record.__getattribute__(self.attr)
            if not target: return
            if record.type == 0:
                format,attrs = ('II',('quest','queststage'))
            elif record.type == 1:
                format,attrs = ('I',('ability',))
            elif record.type == 2:
                format,attrs = ('HB',('entrypoint','function'))
            else:
                raise ModError(ins.inName,_('Unexpected type: %d') % record.type)
            values = []
            valuesAppend = values.append
            getter = target.__getattribute__
            for attr in attrs:
                value = getter(attr)
                valuesAppend(value)
            try:
                out.packSub(self.subType,format,*values)
            except struct.error:
                print self.subType,format,values
                raise
        def mapFids(self,record,function,save=False):
            target = record.__getattribute__(self.attr)
            if not target: return
            if record.type == 0:
                result = function(target.quest)
                if save: target.quest = result
            elif record.type == 1:
                result = function(target.ability)
                if save: target.ability = result
    class MelPerkEffects(MelGroups):
        def __init__(self,attr,*elements):
            MelGroups.__init__(self,attr,*elements)
        def setMelSet(self,melSet):
            self.melSet = melSet
            self.attrLoaders = {}
            for element in melSet.elements:
                attr = element.__dict__.get('attr',None)
                if attr: self.attrLoaders[attr] = element
        def loadData(self,record,ins,type,size,readId):
            if type == 'DATA' or type == 'CTDA':
                effects = record.__getattribute__(self.attr)
                if not effects:
                    if type == 'DATA':
                        element = self.attrLoaders['_data']
                    elif type == 'CTDA':
                        element = self.attrLoaders['conditions']
                    element.loadData(record,ins,type,size,readId)
                    return
            MelGroups.loadData(self,record,ins,type,size,readId)
    class MelPerkEffectParams(MelGroups):
        def loadData(self,record,ins,type,size,readId):
            if type in ('EPFD','EPFT','EPF2','EPF3','SCHR'):
                target = self.getDefault()
                record.__getattribute__(self.attr).append(target)
            else:
                target = record.__getattribute__(self.attr)[-1]
            element = self.loaders[type]
            slots = ['recordType']
            slots.extend(element.getSlotsUsed())
            target.__slots__ = slots
            target.recordType = type
            element.loadData(target,ins,type,size,readId)
        def dumpData(self,record,out):
            for target in record.__getattribute__(self.attr):
                element = self.loaders[target.recordType]
                if not element:
                    raise ModError(ins.inName,_('Unexpected type: %d') % target.recordType)
                element.dumpData(target,out)

    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('DESC','description'),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelConditions(),
        MelGroup('_data',
            MelPerkData('DATA', 'BBBBB', ('trait',0), ('minLevel',0), ('ranks',0), ('playable',0), ('hidden',0)),
            ),
        MelPerkEffects('effects',
            MelStruct('PRKE', 'BBB', 'type', 'rank', 'priority'),
            MelPerkEffectData('DATA','effectData'),
            MelGroups('effectConditions',
                MelStruct('PRKC', 'B', 'runOn'),
                MelConditions(),
            ),
            MelPerkEffectParams('effectParams',
                MelBase('EPFD', 'floats'), # [Float] or [Float,Float], todo rewrite specific class
                MelStruct('EPFT','B','_epft'),
                MelString('EPF2','buttonLabel'),
                MelStruct('EPF3','H','scriptFlag'),
                MelGroup('embeddedScript',
                    MelStruct('SCHR','4s4I',('unused1',null4),'numRefs','compiledSize','lastIndex','scriptType'),
                    MelBase('SCDA','compiled_p'),
                    MelString('SCTX','scriptText'),
                    MelScrxen('SCRV/SCRO','references'),
                ),
            ),
            MelBase('PRKF','footer'),
            ),
        )
    melSet.elements[-1].setMelSet(melSet)
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreExpl(MelRecord):
    """Explosion record."""
    classType = 'EXPL'
    _flags = Flags(0,Flags.getNames('unknown1',
                                    'alwaysUsesWorldOrientation',
                                    'knockDownAlways',
                                    'knockDownByFormular',
                                    'IgnoreLosCheck',
                                    'pushExplosionSourceRefOnly',
                                    'ignoreImageSpaceSwap'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelFid('EITM','objectEffect'),
        MelFid('MNAM','imageSpaceModifier'),
        MelStruct('DATA','fffIIHfIIfffI','force','damage','radius',(FID,'light',None),
                  (FID,'sound1',None),(_flags,'flags'),'isRadius',(FID,'impactDataset',None),
                  (FID,'sound2',None),'radiationLevel','radiationTime','radiationRadius','soundLevel'),
        MelFid('INAM','placedImpactObject'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreIpct(MelRecord):
    """Impact record."""
    classType = 'IPCT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelStruct('DATA','fIffII','effectDuration','effectOrientation','angleThreshold','placementRadius',
                  'soundLevel','flags'),
        MelOptStruct('DODT','7fBB2s3Bs','minWidth','maxWidth','minHeight','maxHeight','depth','shininess',
                     'parallaxScale','parallaxPasses','decalFlags',('unused1',null2),'red','green','blue',('unused2',null1)),
        MelFid('DNAM','textureSet'),
        MelFid('SNAM','sound1'),
        MelFid('NAM1','sound2'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreIpds(MelRecord):
    """Impact Dataset record."""
    classType = 'IPDS'
    class MelIpdsData(MelStruct):
        """Handle older trucated DATA for IPDS subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 48:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 40:
                unpacked = ins.unpack('10I',size,readId)
            elif size == 36:
                unpacked = ins.unpack('9I',size,readId)
            else:
                raise "Unexpected size encountered for IPDS:DATA subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked
    melSet = MelSet(
        MelString('EDID','eid'),
        MelIpdsData('DATA','12I',(FID,'stone',0),(FID,'dirt',0),(FID,'grass',0),(FID,'metal',0),
                    (FID,'wood',0),(FID,'organic',0),(FID,'cloth',0),(FID,'water',0),
                    (FID,'hollowMetal',0),(FID,'organicBug',0),(FID,'organicGlow',0)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreProj(MelRecord):
    """Projectile record."""
    classType = 'PROJ'
    _flags = Flags(0,Flags.getNames('hitscan',
                                    'explosive',
                                    'altTriger',
                                    'muzzleFlash',
                                    'unknown4',
                                    'canbeDisable',
                                    'canbePickedUp',
                                    'superSonic',
                                    'pinsLimbs',
                                    'passThroughSmallTransparent'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelStruct('DATA','HHfffIIfffIIfffIII',(_flags,'flags'),'type',
                  ('gravity',0.00000),('speed',10000.00000),('range',10000.00000),
                  (FID,'light',0),(FID,'muzzleFlash',0),('tracerChance',0.00000),
                  ('explosionAltTrigerProximity',0.00000),('explosionAltTrigerTimer',0.00000),
                  (FID,'explosion',0),(FID,'sound',0),('muzzleFlashDuration',0.00000),
                  ('fadeDuration',0.00000),('impactForce',0.00000),
                  (FID,'soundCountDown',0),(FID,'soundDisable',0),(FID,'defaultWeaponSource',0)),
        MelString('NAM1','muzzleFlashPath'),
        MelBase('NAM2','_nam2'), #--Should be a struct. Maybe later.
        MelStruct('VNAM','I','soundLevel'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreDebr(MelRecord):
    """Debris record."""
    classType = 'DEBR'
    class MelDebrData(MelStruct):
        def __init__(self):
            """Initialize."""
            MelStruct.__init__(self,'DATA','IsI',('percentage',0),('modPath',null1),('flags',0))
        def loadData(self,record,ins,type,size,readId):
            """Reads data from ins into record attribute."""
            data = ins.read(size,readId)
            (record.percentage,) = struct.unpack('B',data[0:1])
            record.modPath = data[1:-2]
            if data[-2] != null1:
                raise ModError(ins.inName,_('Unexpected subrecord: ')+readId)
            (record.flags,) = struct.unpack('B',data[-1])
        def dumpData(self,record,out):
            """Dumps data from record to outstream."""
            data = ''
            data += struct.pack('B',record.percentage)
            data += record.modPath
            data += null1
            data += struct.pack('B',record.flags)
            out.packSub('DATA',data)
    melSet = MelSet(
        MelString('EDID','eid'),
        MelGroups('models',
            MelDebrData(),
            MelBase('MODT','modt_p'),
        ),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreImad(MelRecord):
    """Image space modifier record."""
    classType = 'IMAD'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelBase('DNAM','dnam_p'),
        MelBase('BNAM','bnam_p'),
        MelBase('VNAM','vnam_p'),
        MelBase('TNAM','tnam_p'),
        MelBase('NAM3','nam3_p'),
        MelBase('RNAM','rnam_p'),
        MelBase('SNAM','snam_p'),
        MelBase('UNAM','unam_p'),
        MelBase('NAM1','nam1_p'),
        MelBase('NAM2','nam2_p'),
        MelBase('WNAM','wnam_p'),
        MelBase('XNAM','xnam_p'),
        MelBase('YNAM','ynam_p'),
        MelBase('NAM4','nam4_p'),
        MelBase('aIAD','_aiad_p'),
        MelBase('\x00IAD','_00iad_p'),
        MelBase('@IAD','_atiad_p'),
        MelBase('bIAD','_biad_p'),
        MelBase('\x01IAD','_01iad_p'),
        MelBase('AIAD','aiad_p'),
        MelBase('cIAD','_ciad_p'),
        MelBase('\x02IAD','_02iad_p'),
        MelBase('BIAD','biad_p'),
        MelBase('\x03IAD','_03iad_p'),
        MelBase('dIAD','_diad_p'),
        MelBase('CIAD','ciad_p'),
        MelBase('\x04IAD','_04iad_p'),
        MelBase('eIAD','_eiad_p'),
        MelBase('DIAD','diad_p'),
        MelBase('\x05IAD','_05iad_p'),
        MelBase('fIAD','_fiad_p'),
        MelBase('EIAD','eiad_p'),
        MelBase('\x06IAD','_06iad_p'),
        MelBase('gIAD','_giad_p'),
        MelBase('FIAD','fiad_p'),
        MelBase('\x07IAD','_07iad_p'),
        MelBase('hIAD','_hiad_p'),
        MelBase('GIAD','giad_p'),
        MelBase('\x08IAD','_08iad_p'),
        MelBase('iIAD','_iiad_p'),
        MelBase('HIAD','hiad_p'),
        MelBase('\x09IAD','_09iad_p'),
        MelBase('jIAD','_jiad_p'),
        MelBase('IIAD','iiad_p'),
        MelBase('\x0aIAD','_0aiad_p'),
        MelBase('kIAD','_kiad_p'),
        MelBase('JIAD','jiad_p'),
        MelBase('\x0bIAD','_0biad_p'),
        MelBase('lIAD','_liad_p'),
        MelBase('KIAD','kiad_p'),
        MelBase('\x0cIAD','_0ciad_p'),
        MelBase('mIAD','_miad_p'),
        MelBase('LIAD','liad_p'),
        MelBase('\x0dIAD','_0diad_p'),
        MelBase('nIAD','_niad_p'),
        MelBase('MIAD','miad_p'),
        MelBase('\x0eIAD','_0eiad_p'),
        MelBase('oIAD','_oiad_p'),
        MelBase('NIAD','niad_p'),
        MelBase('\x0fIAD','_0fiad_p'),
        MelBase('pIAD','_piad_p'),
        MelBase('OIAD','oiad_p'),
        MelBase('\x10IAD','_10iad_p'),
        MelBase('qIAD','_qiad_p'),
        MelBase('PIAD','piad_p'),
        MelBase('\x11IAD','_11iad_p'),
        MelBase('rIAD','_riad_p'),
        MelBase('QIAD','qiad_p'),
        MelBase('\x12IAD','_12iad_p'),
        MelBase('sIAD','_siad_p'),
        MelBase('RIAD','riad_p'),
        MelBase('\x13IAD','_13iad_p'),
        MelBase('tIAD','_tiad_p'),
        MelBase('SIAD','siad_p'),
        MelBase('\x14IAD','_14iad_p'),
        MelBase('uIAD','_uiad_p'),
        MelBase('TIAD','tiad_p'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreMstt(MelRecord):
    """Moveable static record."""
    classType = 'MSTT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelBase('DATA','data_p'),
        MelFid('SNAM','sound'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreNote(MelRecord):
    """Note record."""
    classType = 'NOTE'
    _type = Flags(0,Flags.getNames(
            ( 0,'sound' ),
            ( 1,'text' ),
            ( 2,'image' ),
            ( 3,'voice' ),
            ))
    class MelNoteTnam(MelBase):
        """text or topic"""
        def hasFids(self,formElements):
            formElements.add(self)
        def loadData(self,record,ins,type,size,readId):
            if record.dataType == 1: # text (string)
                value = ins.readString(size,readId)
                record.__setattr__(self.attr, (False, value))
            elif record.dataType == 3: # voice (fid:DIAL)
                (value,) = ins.unpack('I',size,readId)
                record.__setattr__(self.attr, (True, value))
            else:
                raise ModError(ins.inName,_('Unexpected type: %d') % record.type)
            if self._debug: print unpacked
        def dumpData(self,record,out):
            value = record.__getattribute__(self.attr)
            if value is None: return
            (isFid, value) = value
            if value is not None:
                if record.dataType == 1: # text (string)
                    out.packSub0(self.subType,value)
                elif record.dataType == 3: # voice (fid:DIAL)
                    out.packRef(self.subType,value)
                else:
                    raise ModError(ins.inName,_('Unexpected type: %d') % record.type)
        def mapFids(self,record,function,save=False):
            value = record.__getattribute__(self.attr)
            if value is None: return
            (isFid, value) = value
            if isFid:
                result = function(value)
                if save: record.__setattr__(self.attr,(isFid,result))
    class MelNoteSnam(MelBase):
        """sound or npc"""
        def hasFids(self,formElements):
            formElements.add(self)
        def loadData(self,record,ins,type,size,readId):
            if record.dataType == 0: # sound (fid:SOUN)
                (value,) = ins.unpack('I',size,readId)
                record.__setattr__(self.attr, (True, value))
            elif record.dataType == 3: # voice (fid:NPC_)
                (value,) = ins.unpack('I',size,readId)
                record.__setattr__(self.attr, (True, value))
            else:
                raise ModError(ins.inName,_('Unexpected type: %d') % record.type)
            if self._debug: print unpacked
        def dumpData(self,record,out):
            value = record.__getattribute__(self.attr)
            if value is None: return
            (isFid, value) = value
            if value is not None: out.packRef(self.subType,value)
        def mapFids(self,record,function,save=False):
            value = record.__getattribute__(self.attr)
            if value is None: return
            (isFid, value) = value
            if isFid:
                result = function(value)
                if save: record.__setattr__(self.attr,(isFid,result))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        MelStruct('DATA','B','dataType'),
        MelFidList('ONAM','quests'),
        MelString('XNAM','texture'),
        MelNoteTnam('TNAM', 'textTopic'),
        MelNoteSnam('SNAM', 'soundNpc'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreTerm(MelRecord):
    """Terminal record."""
    classType = 'TERM'
    _flags = Flags(0L,Flags.getNames('leveled','unlocked','alternateColors','hideWellcomeTextWhenDisplayingImage'))
    _menuFlags = Flags(0L,Flags.getNames('addNote','forceRedraw'))
    _variableFlags = Flags(0L,Flags.getNames('isLongOrShort'))
    class MelTermDnam(MelStruct):
        """Handle older trucated DNAM for TERM subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 4:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 3:
                unpacked = ins.unpack('BBB',size,readId)
            else:
                raise "Unexpected size encountered for TERM:DNAM subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelFid('SCRI','script'),
        MelDestructible(),
        MelString('DESC','description'),
        MelFid('SNAM','soundLooping'),
        MelFid('PNAM','passwordNote'),
        MelTermDnam('DNAM','BBH','baseHackingDifficulty',(_flags,'flags'),'serverType'),
        MelGroups('menuItems',
            MelString('ITXT','itemText'),
            MelString('RNAM','resultText'),
            MelStruct('ANAM','B',(_menuFlags,'menuFlags')),
            MelFid('INAM','displayNote'),
            MelFid('TNAM','subMenu'),
            MelStruct('SCHR','4s4I',('unused1',null4),'numRefs','compiledSize','lastIndex','scriptType'),
            MelBase('SCDA','compiled_p'),
            MelString('SCTX','scriptText'),
            MelGroups('vars',
                MelStruct('SLSD','I12sB7s','index',('unused1',null4+null4+null4),(_variableFlags,'flags',0L),('unused2',null4+null3)),
                MelString('SCVR','name')),
            MelScrxen('SCRV/SCRO','references'),
            MelConditions(),
        ),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreAvif(MelRecord):
    """ActorValue Information record."""
    classType = 'AVIF'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('DESC','description'),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelString('ANAM','shortName'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreEczn(MelRecord):
    """Encounter Zone record."""
    classType = 'ECZN'
    _flags = Flags(0L,Flags.getNames('neverResets','matchPCBelowMinimumLevel'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DATA','=I2b2B',(FID,'owner',None),'rank','minimumLevel',(_flags,'flags',0L),('unused1',null1)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreBptd(MelRecord):
    """Body part data record."""
    classType = 'BPTD'
    _flags = Flags(0L,Flags.getNames('severable','ikData','ikBipedData','explodable','ikIsHead','ikHeadtracking','toHitChanceAbsolute'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelGroups('bodyParts',
            MelString('BPTN','partName'),
            MelString('BPNN','nodeName'),
            MelString('BPNT','vatsTarget'),
            MelString('BPNI','ikDataStartNode'),
            MelStruct('BPND','f6BH2I2f3I7f2I2B2sf','damageMult',(_flags,'flags'),'partType','healthPercent','actorValue',
                      'toHitChance','explodableChancePercent','explodableDebrisCount',(FID,'explodableDebris',0L),(FID,'explodableExplosion',0L),
                      'trackingMaxAngle','explodableDebrisScale','severableDebrisCount',(FID,'severableDebris',0L),(FID,'severableExplosion',0L),
                      'severableDebrisScale','goreEffectPosTransX','goreEffectPosTransY','goreEffectPosTransZ',
                      'goreEffectPosRotX','goreEffectPosRotY','goreEffectPosRotZ',(FID,'severableImpactDataSet',0L),(FID,'explodableImpactDataSet',0L),
                      'severableDecalCount','explodableDecalCount',('unused',null2),'limbReplacementScale'),
            MelString('NAM1','limbReplacementModel'),
            MelString('NAM4','goreEffectsTargetBone'),
            MelBase('NAM5','endMarker'),
            ),
        MelFid('RAGA','ragdoll'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreVtyp(MelRecord):
    """Voice type record."""
    classType = 'VTYP'
    _flags = Flags(0L,Flags.getNames('allowDefaultDialog','female'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DNAM','B',(_flags,'flags')),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreMusc(MelRecord):
    """Music type record."""
    classType = 'MUSC'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FNAM','filename'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MrePwat(MelRecord):
    """Placeable water record."""
    classType = 'PWAT'
    _flags = Flags(0L,Flags.getNames(
        ( 0,'reflects'),
        ( 1,'reflectsActers'),
        ( 2,'reflectsLand'),
        ( 3,'reflectsLODLand'),
        ( 4,'reflectsLODBuildings'),
        ( 5,'reflectsTrees'),
        ( 6,'reflectsSky'),
        ( 7,'reflectsDynamicObjects'),
        ( 8,'reflectsDeadBodies'),
        ( 9,'reflects2'),
        (10,'reflects2Actors'),
        (11,'reflects2Lands'),
        (16,'reflects2DynamicObjects'),
        (17,'reflects2DeadBodies'),
        (18,'silhouetteReflections'),
        (28,'depth'),
        (29,'objectTextureCoordinates'),
        (31,'noUnderwaterFog'),
        ))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelModel(),
        MelStruct('DNAM','2I',(_flags,'flags'),(FID,'water'))
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreAspc(MelRecord):
    """Acoustic space record."""
    classType = 'ASPC'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelFid('SNAM','soundLooping'),
        MelFid('RDAT','useSoundFromRegion'),
        MelStruct('ANAM','I','environmentType'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreHdpt(MelRecord):
    """Head part record."""
    classType = 'HDPT'
    _flags = Flags(0L,Flags.getNames('playable',))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelModel(),
        MelStruct('DATA','B',(_flags,'flags')),
        MelFids('HNAM','extraParts'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreDobj(MelRecord):
    """Default object manager record."""
    classType = 'DOBJ'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DATA','21I',(FID,'stimpack'),(FID,'superStimpack'),(FID,'radX'),(FID,'radAway'),
            (FID,'morphine'),(FID,'perkParalysis'),(FID,'playerFaction'),(FID,'mysteriousStrangerNpc'),
            (FID,'mysteriousStrangerFaction'),(FID,'defaultMusic'),(FID,'battleMusic'),(FID,'deathMusic'),
            (FID,'successMusic'),(FID,'levelUpMusic'),(FID,'playerVoiceMale'),(FID,'playerVoiceMaleChild'),
            (FID,'playerVoiceFemale'),(FID,'playerVoiceFemaleChild'),(FID,'eatPackageDefaultFood'),
            (FID,'everyActorAbility'),(FID,'drugWearsOffImageSpace'),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreIdlm(MelRecord):
    """Idle marker record."""
    classType = 'IDLM'
    class MelIdlmIdlc(MelStruct):
        """Handle older trucated IDLC for IDLM subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 4:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 1:
                unpacked = ins.unpack('B',size,readId)
            else:
                raise "Unexpected size encountered for TERM:DNAM subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelStruct('IDLF','B','flags'),
        MelIdlmIdlc('IDLC','B3s','animationCount',('unused',null3)),
        MelStruct('IDLT','f','idleTimerSetting'),
        MelFidList('IDLA','animations'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreArma(MelRecord):
    """Armor addon record."""
    classType = 'ARMA'
    _flags = MelBipedFlags(0L,Flags.getNames())
    _generalFlags = Flags(0L,Flags.getNames(
        (5,'powerArmor'),
        (6,'notPlayable'),
        (7,'heavyArmor')
    ))
    _etype = Flags(0L,Flags.getNames(
        'alcohol','bigGuns','bodyWear','chems','energyWeapons','food','handWear','headWear',
        'meleeWeapons','mine','none','smallGuns','stimpack','thrownWeapons','unarmedWeapon'
    ))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelStruct('BMDT','=2I',(_flags,'bipedFlags',0L),(_generalFlags,'generalFlags',0L)),
        MelModel('maleBody'),
        MelModel('maleWorld',2),
        MelString('ICON','maleLargeIconPath'),
        MelString('MICO','maleSmallIconPath'),
        MelModel('femaleBody',3),
        MelModel('femaleWorld',4),
        MelString('ICO2','femaleLargeIconPath'),
        MelString('MIC2','femaleSmallIconPath'),
        MelStruct('ETYP','I',(_etype,'etype',0L)),
        MelStruct('DATA','IIf','value','health','weight'),
        MelStruct('DNAM','HH','ar','flags'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreTact(MelRecord):
    """Talking activator record."""
    classType = 'TACT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel('model'),
        MelFid('SCRI','script'),
        MelDestructible(),
        MelFid('SNAM','sound'),
        MelFid('VNAM','voiceType'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreNavm(MelRecord):
    """Navigation Mesh."""
    classType = 'NAVM'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('NVER','I',('version',11)),
        MelStruct('DATA','I5I',(FID,'cell'),'vertexCount','triangleCount','enternalConnectionsCount','nvcaCount','doorsCount'),
        MelStructA('NVVX','3f','vertices','vertexX','vertexY','vertexZ'),
        MelStructA('NVTR','6hI','triangles','vertex0','vertex1','vertex2','triangle0','triangle1','triangle2','flags'),
        MelOptStruct('NVCA','h','nvca_p'),
        MelStructA('NVDP','II','doors',(FID,'doorReference'),'doorUnknown'),
        MelBase('NVGD','nvgd_p'),
        MelStructA('NVEX','=IIH','externalConnections','nvexUnknown',(FID,'navigationMesh'),'triangle'),
       )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreNavi(MelRecord):
    """Navigation Mesh Info Map."""
    classType = 'NAVI'
    class MelNaviNvmi(MelStructs):
        """Handle older trucated NVMI for NAVI subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size <= 16:
                raise "Unexpected size encountered for NAVI subrecord: %s" % size
            format = '4s2I2H %ds'%(size-16)
            target = self.getDefault()
            record.__getattribute__(self.attr).append(target)
            target.__slots__ = self.attrs
            unpacked = ins.unpack(format,size,readId)
            setter = target.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked, target.flags.getTrueAttrs()
        def dumpData(self,record,out):
            """Dumps data from record to outstream."""
            for target in record.__getattribute__(self.attr):
                format = '4s2I2H %ds'%len(target.unknown2)
                values = []
                valuesAppend = values.append
                getter = target.__getattribute__
                for attr,action in zip(self.attrs,self.actions):
                    value = getter(attr)
                    if action: value = value.dump()
                    valuesAppend(value)
                try:
                    out.packSub(self.subType,format,*values)
                except struct.error:
                    print self.subType,format,values
                    raise
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('NVER','I',('version',11)),
        MelNaviNvmi('NVMI','','unknowns',
                   'unknown1',(FID,'navigationMesh'),(FID,'location'),'gridX','gridY','unknown2'),
       )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreAlch(MelRecord,MreHasEffects):
    """ALCH (potion) record."""
    classType = 'ALCH'
    _flags = Flags(0L,Flags.getNames('autoCalc','isFood'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelFull0(),
        MelModel(),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelFid('SCRI','script'),
        MelDestructible(),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        #--10:chems,11:stimpack,12:food,13:alcohol
        MelStruct('ETYP','I','etype'),
        MelStruct('DATA','f','weight'),
        MelStruct('ENIT','iB3sIfI','value',(_flags,'flags',0L),('unused1',null3),
                  (FID,'withdrawalEffect',None),'addictionChance',(FID,'soundConsume',None)),
        MelEffects(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreAchr(MelRecord): # Placed NPC
    classType = 'ACHR'
    _flags = Flags(0L,Flags.getNames('oppositeParent'))
    melSet=MelSet(
        MelString('EDID','eid'),
        MelFid('NAME','base'),
        MelXpci('XPCI'),
        MelOptStruct('XLOD','3f',('lod1',None),('lod2',None),('lod3',None)), ####Distant LOD Data, unknown
        MelOptStruct('XESP','IB3s',(FID,'parent'),(_flags,'parentFlags'),('unused1',null3)),
        MelFid('XMRC','merchantContainer'),
        MelFid('XHRS','horse'),
        MelBase('XRGD','xrgd_p'), ###Ragdoll Data, ByteArray
        MelOptStruct('XSCL','f',('scale',1.0)),
        MelOptStruct('DATA','=6f',('posX',None),('posY',None),('posZ',None),('rotX',None),('rotY',None),('rotZ',None)),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreAcre(MelRecord): # Placed Creature
    classType = 'ACRE'
    _flags = Flags(0L,Flags.getNames('oppositeParent'))
    melSet=MelSet(
        MelString('EDID','eid'),
        MelFid('NAME','base'),
        MelOwnership(),
        MelOptStruct('XLOD','3f',('lod1',None),('lod2',None),('lod3',None)), ####Distant LOD Data, unknown
        MelOptStruct('XESP','IB3s',(FID,'parent'),(_flags,'parentFlags'),('unused1',null3)),
        MelBase('XRGD','xrgd_p'), ###Ragdoll Data, ByteArray
        MelOptStruct('XSCL','f',('scale',1.0)),
        MelOptStruct('DATA','=6f',('posX',None),('posY',None),('posZ',None),('rotX',None),('rotY',None),('rotZ',None)),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreActi(MelRecord):
    """Activator record."""
    classType = 'ACTI'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelFid('SCRI','script'),
        MelDestructible(),
        MelFid('SNAM','soundLooping'),
        MelFid('VNAM','soundActivation'),
        MelFid('RNAM','radioStation'),
        MelFid('WNAM','waterType'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreAmmo(MelRecord):
    """Ammo (arrow) record."""
    classType = 'AMMO'
    _flags = bolt.Flags(0L,bolt.Flags.getNames('notNormalWeapon'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelString('ICON','largeIconPath'),
        MelString('MICO','smallIconPath'),
        MelDestructible(),
        MelFid('YNAM','soundPickup'),
        MelFid('ZNAM','soundDrop'),
        MelStruct('DATA','fB3sIB','speed',(_flags,'flags',0L),('unused1',null3),'value','clipRounds'),
        MelString('ONAM','shortName'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed() 
    
#------------------------------------------------------------------------------
class MreArmo(MelRecord):
    """Armor record."""
    classType = 'ARMO'
    _flags = MelBipedFlags(0L,bolt.Flags.getNames())
    _generalFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (5,'powerArmor'),
        (6,'notPlayable'),
        (7,'heavyArmor')
    ))
    _etype = bolt.Flags(0L,bolt.Flags.getNames(
        'alcohol','bigGuns','bodyWear','chems','energyWeapons','food','handWear','headWear',
        'meleeWeapons','mine','none','smallGuns','stimpack','thrownWeapons','unarmedWeapon'
    ))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelFid('SCRI','script'),
        MelFid('EITM','objectEffect'),
        MelStruct('BMDT','=2I',(_flags,'bipedFlags',0L),(_generalFlags,'generalFlags',0L)),
        MelModel('maleBody'),
        MelModel('maleWorld',2),
        MelString('ICON','maleLargeIconPath'),
        MelString('MICO','maleSmallIconPath'),
        MelModel('femaleBody',3),
        MelModel('femaleWorld',4),
        MelString('ICO2','femaleLargeIconPath'),
        MelString('MIC2','femaleSmallIconPath'),
        MelString('BMCT','ragdollConstraintTemplate'),
        MelDestructible(),
        MelFid('REPL','repairList'),
        MelFid('BIPL','bipedModelList'),
        MelStruct('ETYP','I',(_etype,'etype',0L)),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        MelStruct('DATA','=IIf','value','health','weight'),
        MelStruct('DNAM','=HH','ar','flags'), # AR is multiplied by 100.
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreCont(MelRecord):
    """Container record."""
    classType = 'CONT'
    _flags = bolt.Flags(0,bolt.Flags.getNames(None,'respawns'))
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelString('FULL','full'),
        MelModel(),
        MelFid('SCRI','script'),
        MelGroups('items',
            MelStruct('CNTO','Ii',(FID,'item',None),('count',1)),
            MelOptStruct('COED','IIf',(FID,'owner',None),(FID,'glob',None),('condition',1.0)),
        ),
        MelDestructible(),
        MelStruct('DATA','=Bf',(_flags,'flags',0L),'weight'),
        MelFid('SNAM','soundOpen'),
        MelFid('QNAM','soundClose'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreCsty(MelRecord):
    """CSTY Record. Combat Styles."""
    classType = 'CSTY'
    _flagsA = Flags(0L,Flags.getNames(
        ( 0,'advanced'),
        ( 1,'useChanceForAttack'),
        ( 2,'ignoreAllies'),
        ( 3,'willYield'),
        ( 4,'rejectsYields'),
        ( 5,'fleeingDisabled'),
        ( 6,'prefersRanged'),
        ( 7,'meleeAlertOK'),
        ))
    _flagsB = Flags(0L,Flags.getNames(
        ( 0,'doNotAcquire'),
        ))

    class MelCstdData(MelStruct):
        """Handle older trucated DATA for CSTD subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 124:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 120:
                #--Else 120 byte record (skips flagsB
                unpacked = ins.unpack('2B2s8f2B2s3fB3s2f5B3s2f2B2s7fB3sf',size,readId)
            elif size == 112:
                #--112 byte record (skips flagsB, rushChance, unused6, rushMult
                unpacked = ins.unpack('2B2s8f2B2s3fB3s2f5B3s2f2B2s7f',size,readId)
            elif size == 104:
                #--104 byte record (skips flagsB, rushChance, unused6, rushMult, rStand, groupStand
                #-- only one occurence (AndragilTraining
                unpacked = ins.unpack('2B2s8f2B2s3fB3s2f5B3s2f2B2s5f',size,readId)
            elif size == 92:
                #--92 byte record (skips flagsB, rushChance, unused6, rushMult, rStand, groupStand
                #--                mDistance, rDistance, buffStand
                #-- These records keep getting shorter and shorter...
                #-- This one is used by quite a few npcs
                unpacked = ins.unpack('2B2s8f2B2s3fB3s2f5B3s2f2B2s2f',size,readId)
            elif size == 84:
                #--84 byte record (skips flagsB, rushChance, unused6, rushMult, rStand, groupStand
                #--                mDistance, rDistance, buffStand, rMultOpt, rMultMax
                #-- This one is present once: VidCaptureNoAttacks and it isn't actually used.
                unpacked = ins.unpack('2B2s8f2B2s3fB3s2f5B3s2f2B2s',size,readId)
            else:
                raise ModError(ins.inName,_('Unexpected size encountered for CSTD subrecord: ')+str(size))
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked, record.flagsA.getTrueAttrs()
    #--Mel Set
    melSet = MelSet(
        MelString('EDID','eid'),
        MelCstdData('CSTD', '2B2s8f2B2s3fB3s2f5B3s2f2B2s7fB3sfI', 'dodgeChance', 'lrChance',
                    ('unused1',null2), 'lrTimerMin', 'lrTimerMax', 'forTimerMin', 'forTimerMax',
                    'backTimerMin', 'backTimerMax', 'idleTimerMin', 'idleTimerMax',
                    'blkChance', 'atkChance', ('unused2',null2), 'atkBRecoil','atkBunc',
                    'atkBh2h', 'pAtkChance', ('unused3',null3), 'pAtkBRecoil', 'pAtkBUnc',
                    'pAtkNormal', 'pAtkFor', 'pAtkBack', 'pAtkL', 'pAtkR', ('unused4',null3),
                    'holdTimerMin', 'holdTimerMax', (_flagsA,'flagsA'), 'acroDodge',
                    ('unused5',null2), ('rMultOpt',1.0), ('rMultMax',1.0), ('mDistance',250.0), ('rDistance',1000.0),
                    ('buffStand',325.0), ('rStand',500.0), ('groupStand',325.0), ('rushChance',25),
                    ('unused6',null3), ('rushMult',1.0), (_flagsB,'flagsB')),
        MelOptStruct('CSAD', '21f', 'dodgeFMult', 'dodgeFBase', 'encSBase', 'encSMult',
                     'dodgeAtkMult', 'dodgeNAtkMult', 'dodgeBAtkMult', 'dodgeBNAtkMult',
                     'dodgeFAtkMult', 'dodgeFNAtkMult', 'blockMult', 'blockBase',
                     'blockAtkMult', 'blockNAtkMult', 'atkMult','atkBase', 'atkAtkMult',
                     'atkNAtkMult', 'atkBlockMult', 'pAtkFBase', 'pAtkFMult'),
        MelOptStruct('CSSD', '9fII5f', 'coverSearchRadius', 'takeCoverChance', 'waitTimerMin', 'waitTimerMax',
                     'waitToFireTimerMin', 'waitToFireTimerMax', 'fireTimerMin', 'fireTimerMax'
                     'rangedWeaponRangeMultMin','weaponRestrictions','unknown1','rangedWeaponRangeMultMax',
                     'maxTargetingFov','combatRadius','semiAutomaticFireDelayMultMin','semiAutomaticFireDelayMultMax'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreDial(MelRecord):
    """Dialog record."""
    classType = 'DIAL'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFids('QSTI','quests'), ### QSTRs?
        MelString('FULL','full'),
        MelStruct('DATA','B','dialType'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed() + ['infoStamp','infoStamp2','infos']

    def __init__(self,header,ins=None,unpack=False):
        """Initialize."""
        MelRecord.__init__(self,header,ins,unpack)
        self.infoStamp = 0 #--Stamp for info GRUP
        self.infoStamp2 = 0 #--Stamp for info GRUP
        self.infos = []

    def loadInfos(self,ins,endPos,infoClass):
        """Load infos from ins. Called from MobDials."""
        infos = self.infos
        recHead = ins.unpackRecHeader
        infosAppend = infos.append
        while not ins.atEnd(endPos,'INFO Block'):
            #--Get record info and handle it
            header = recHead()
            recType = header[0]
            if recType == 'INFO':
                info = infoClass(header,ins,True)
                infosAppend(info)
            else:
                raise ModError(ins.inName, _('Unexpected %s record in %s group.')
                    % (recType,"INFO"))

    def dump(self,out):
        """Dumps self., then group header and then records."""
        MreRecord.dump(self,out)
        if not self.infos: return
        size = 20 + sum([20 + info.getSize() for info in self.infos])
        out.pack('4sIIIII','GRUP',size,self.fid,7,self.infoStamp,self.infoStamp2)
        for info in self.infos: info.dump(out)

    def updateMasters(self,masters):
        """Updates set of master names according to masters actually used."""
        MelRecord.updateMasters(self,masters)
        for info in self.infos:
            info.updateMasters(masters)

    def convertFids(self,mapper,toLong):
        """Converts fids between formats according to mapper.
        toLong should be True if converting to long format or False if converting to short format."""
        MelRecord.convertFids(self,mapper,toLong)
        for info in self.infos:
            info.convertFids(mapper,toLong)

#------------------------------------------------------------------------------
class MreFact(MelRecord):
    """Faction record."""
    classType = 'FACT'
    _flags = bolt.Flags(0L,bolt.Flags.getNames('hiddenFromPC','evil','specialCombat'))

    class MelFactData(MelStruct):
        """Handle older trucated DATA for FACT subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 4:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 2:
                #--Else 2 byte record
                unpacked = ins.unpack('2B',size,readId)
            elif size == 1:
                #--Else 1 byte record
                unpacked = ins.unpack('B',size,readId)
            else:
                raise "Unexpected size encountered for FACT:DATA subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked

    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelStructs('XNAM','I2i','relations',(FID,'faction'),'mod','groupCombatReaction'),
        MelFactData('DATA','2BH',(_flags,'flags',0L),'flags2','unknown'),
        MelOptStruct('CNAM','f',('crimeGoldMultiplier',None)),
        MelGroups('ranks',
            MelStruct('RNAM','i','rank'),
            MelString('MNAM','male'),
            MelString('FNAM','female'),
            MelString('INAM','insigniaPath'),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreIdle(MelRecord):
    """Idle record."""
    classType = 'IDLE'
    #--Mel IDLE DATA
    class MelIdleData(MelStruct):
        """Handle older trucated DATA for IDLE subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 8:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 6:
                #--Else 6 byte record (skips flags and unknown2...
                unpacked = ins.unpack('4BH',size,readId)
            else:
                raise "Unexpected size encountered for IDLE:DATA subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked, record.flags.getTrueAttrs()
    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelConditions(),
        MelStruct('ANAM','II',(FID,'parent'),(FID,'prevId')),
        MelIdleData('DATA','4BH2B','group','loopMin','loopMax','unknown1','delay','flags','unknown2'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreInfo(MelRecord):
    """Info (dialog entry) record."""
    classType = 'INFO'
    _flags = bolt.Flags(0,bolt.Flags.getNames(
        'goodbye','random','sayOnce','runImmediately','infoRefusal','randomEnd','runForRumors','sayOnceADay','alwaysDarken'))
    class MelInfoData(MelStruct):
        """Support older 2 byte version."""
        def loadData(self,record,ins,type,size,readId):
            if size != 2:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            unpacked = ins.unpack('2B',size,readId)
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print (record.dialType,record.flags.getTrueAttrs())

    class MelInfoSchr(MelStruct):
        """Print only if schd record is null."""
        def dumpData(self,record,out):
            if not record.schd_p:
                MelStruct.dumpData(self,record,out)
    #--MelSet
    melSet = MelSet(
        MelInfoData('DATA','HH','dialType',(_flags,'flags')),
        MelFid('QSTI','quests'),
        MelFid('TPIC','topic'),
        MelFid('PNAM','prevInfo'),
        MelFids('NAME','addTopics'),
        MelGroups('responses',
            MelStruct('TRDT','Ii4sB3s','emotionType','emotionValue',('unused1',null4),'responseNum',('unused2',null3)),
            MelString('NAM1','responseText'),
            MelString('NAM2','actorNotes'),
            ),
        MelConditions(),
        MelFids('TCLT','choices'),
        MelFids('TCLF','linksFrom'),
        MelBase('SCHD','schd_p'), #--Old format script header?
        MelInfoSchr('SCHR','4s4I',('unused2',null4),'numRefs','compiledSize','lastIndex','scriptType'),
        MelBase('SCDA','compiled_p'),
        MelString('SCTX','scriptText'),
        MelScrxen('SCRV/SCRO','references')
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreLigh(MelRecord):
    """Light source record."""
    classType = 'LIGH'
    _flags = bolt.Flags(0L,bolt.Flags.getNames('dynamic','canTake','negative','flickers',
        'unk1','offByDefault','flickerSlow','pulse','pulseSlow','spotLight','spotShadow'))
    #--Mel NPC DATA
    class MelLighData(MelStruct):
        """Handle older trucated DATA for LIGH subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 32:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 24:
                #--Else 24 byte record (skips value and weight...
                unpacked = ins.unpack('iI3BsIff',size,readId)
            else:
                raise ModError(ins.inName,_('Unexpected size encountered for LIGH:DATA subrecord: ')+str(size))
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked, record.flags.getTrueAttrs()
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('OBND','=6h',
                  'corner0X','corner0Y','corner0Z',
                  'corner1X','corner1Y','corner1Z'),
        MelModel(),
        MelFid('SCRI','script'),
        MelString('FULL','full'),
        MelString('ICON','iconPath'),
        MelLighData('DATA','iI3BsIffIf','duration','radius','red','green','blue',('unused1',null1),
            (_flags,'flags',0L),'falloff','fov','value','weight'),
        MelOptStruct('FNAM','f',('fade',None)),
        MelFid('SNAM','sound'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
class MreLscr(MelRecord):
    """Load screen."""
    classType = 'LSCR'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('ICON','iconPath'),
        MelString('DESC','text'),
        MelStructs('LNAM','2I2h','Locations',(FID,'direct'),(FID,'indirect'),'gridy','gridx'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()


#--Mergeable record types
mergeClasses = (
    MreActi, MreAlch, MreAmmo, MreAnio, MreAppa, MreArmo, MreBook, MreBsgn, MreClas,
    MreClot, MreCont, MreCrea, MreDoor, MreEfsh, MreEnch, MreEyes, MreFact, MreFlor, MreFurn,
    MreGlob, MreGras, MreHair, MreIngr, MreKeym, MreLigh, MreLscr, MreLvlc, MreLvli,
    MreLvsp, MreMgef, MreMisc, MreNpc,  MrePack, MreQust, MreRace, MreScpt, MreSgst,
    MreSlgm, MreSoun, MreSpel, MreStat, MreTree, MreWatr, MreWeap, MreWthr,
    MreClmt, MreCsty, MreIdle, MreLtex, MreRegn, MreSbsp, MreSkil,
    MreTxst, MreMicn, MreFlst, MreLvln, MrePerk, MreExpl, MreIpct, MreIpds, MreProj,
    MreDebr, MreImad, MreMstt, MreNote, MreTerm, MreAvif, MreEczn, MreBptd, MreVtyp,
    MreMusc, MrePwat, MreAspc, MreHdpt, MreDobj, MreIdlm, MreArma, MreTact
    )

#--Extra read classes: these record types will always be loaded, even if patchers
#  don't need them directly (for example, for MGEF info)
readClasses = ()
writeClasses = ()

def init():
    # Due to a bug with py2exe, 'reload' doesn't function properly.  Instead of
    # re-executing all lines within the module, it acts like another 'import'
    # statement - in otherwords, nothing happens.  This means any lines that
    # affect outside modules must do so within this function, which will be
    # called instead of 'reload'
    brec.ModReader.recHeader = RecordHeader
    
    MreRecord.type_class = dict((x.classType,x) for x in (
        MreAchr, MreAcre, MreActi, MreAlch, MreAmmo, MreAnio, MreAppa, MreArmo, MreBook, MreBsgn,
        MreCell, MreClas, MreClot, MreCont, MreCrea, MreDoor, MreEfsh, MreEnch, MreEyes, MreFact,
        MreFlor, MreFurn, MreGlob, MreGmst, MreGras, MreHair, MreIngr, MreKeym, MreLigh, MreLscr,
        MreLvlc, MreLvli, MreLvsp, MreMgef, MreMisc, MreNpc,  MrePack, MreQust, MreRace, MreRefr,
        MreRoad, MreScpt, MreSgst, MreSkil, MreSlgm, MreSoun, MreSpel, MreStat, MreTree, MreTes4,
        MreWatr, MreWeap, MreWrld, MreWthr, MreClmt, MreCsty, MreIdle, MreLtex, MreRegn, MreSbsp,
        MreDial, MreInfo, MreTxst, MreMicn, MreFlst, MrePerk, MreExpl, MreIpct, MreIpds, MreProj,
        MreLvln, MreDebr, MreImad, MreMstt, MreNote, MreTerm, MreAvif, MreEczn, MreBptd, MreVtyp,
        MreMusc, MrePwat, MreAspc, MreHdpt, MreDobj, MreIdlm, MreArma, MreTact, MreNavm
    ))
    
    MreRecord.simpleTypes = (set(MreRecord.type_class) -
        set(('TES4','ACHR','ACRE','REFR','CELL','PGRD','ROAD','LAND','WRLD','INFO','DIAL','PGRE','NAVM')))