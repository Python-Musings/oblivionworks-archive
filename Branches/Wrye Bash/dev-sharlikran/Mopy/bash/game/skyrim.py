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

"""This modules defines static data for use by bush, when TES V:
   Skyrim is set at the active game."""

import struct
from .. import brec
from .. import bolt
from ..bolt import _encode
from ..brec import *
from skyrim_const import bethDataFiles, allBethFiles

import itertools
from_iterable = itertools.chain.from_iterable

# Util Constants ---------------------------------------------------------------
#--Null strings (for default empty byte arrays)
null1 = '\x00'
null2 = null1*2
null3 = null1*3
null4 = null1*4

#--Name of the game
name = u'Skyrim'

#--Name of the game as used in related filenames and paths.
safeName = name

#--Alternate display name to use instead of "Wrye Bash for ***"
altName = u'Wrye Smash'

#--exe to look for to see if this is the right game
exe = u'TESV.exe'

#--Registry keys to read to find the install location
regInstallKeys = [
    (u'Bethesda Softworks\\Skyrim',u'Installed Path'),
    ]

#--patch information
patchURL = u'' # Update via steam
patchTip = u'Update via Steam'

#--URL to the Nexus site for this game
nexusUrl = u'http://www.nexusmods.com/skyrim/'
nexusName = u'Skyrim Nexus'
nexusKey = 'bash.installers.openSkyrimNexus'

#--Creation Kit Set information
class cs:
    shortName = u'CK'                # Abbreviated name
    longName = u'Creation Kit'       # Full name
    exe = u'CreationKit.exe'         # Executable to run
    seArgs = None # u'-editor'       # Argument to pass to the SE to load the CS # Not yet needed
    imageName = u'creationkit%s.png' # Image name template for the status bar

#--Script Extender information
class se:
    shortName = u'SKSE'                      # Abbreviated name
    longName = u'Skyrim Script Extender'     # Full name
    exe = u'skse_loader.exe'                 # Exe to run
    steamExe = u'skse_loader.exe'            # Exe to run if a steam install
    url = u'http://skse.silverlock.org/'     # URL to download from
    urlTip = u'http://skse.silverlock.org/'  # Tooltip for mouse over the URL

#--Script Dragon
class sd:
    shortName = u'SD'
    longName = u'Script Dragon'
    installDir = u'asi'

#--SkyProc Patchers
class sp:
    shortName = u'SP'
    longName = u'SkyProc'
    installDir = u'SkyProc Patchers'

#--Quick shortcut for combining both the SE and SD names
se_sd = se.shortName+u'/'+sd.longName

#--Graphics Extender information
class ge:
    shortName = u''
    longName = u''
    exe = u'**DNE**'
    url = u''
    urlTip = u''

#--4gb Launcher
class laa:
    # Skyrim has a 4gb Launcher, but as of patch 1.3.10, it is
    # no longer required (Bethsoft updated TESV.exe to already
    # be LAA)
    name = u''
    exe = u'**DNE**'
    launchesSE = False

# Files BAIN shouldn't skip
dontSkip = (
       #These are all in the Interface folder. Apart from the skyui_ files, they are all present in vanilla.
       u'skyui_cfg.txt',
       u'skyui_translate.txt',
       u'credits.txt',
       u'credits_french.txt',
       u'fontconfig.txt',
       u'controlmap.txt',
       u'gamepad.txt',
       u'mouse.txt',
       u'keyboard_english.txt',
       u'keyboard_french.txt',
       u'keyboard_german.txt',
       u'keyboard_spanish.txt',
       u'keyboard_italian.txt',
)

# Directories where specific file extensions should not be skipped by BAIN
dontSkipDirs = {
                # This rule is to allow mods with string translation enabled.
                'interface\\translations':['.txt']
}

#Folders BAIN should never check
SkipBAINRefresh = set ((
    #Use lowercase names
    u'tes5edit backups',
))

#--Some stuff dealing with INI files
class ini:
    #--True means new lines are allowed to be added via INI Tweaks
    #  (by default)
    allowNewLines = True

    #--INI Entry to enable BSA Redirection
    bsaRedirection = (u'',u'')

#--Save Game format stuff
class ess:
    # Save file capabilities
    canReadBasic = True         # All the basic stuff needed for the Saves Tab
    canEditMasters = True       # Adjusting save file masters
    canEditMore = False         # No advanced editing

    # Save file extension.
    ext = u'.ess';

    @staticmethod
    def load(ins,header):
        """Extract info from save file."""
        #--Header
        if ins.read(13) != 'TESV_SAVEGAME':
            raise Exception(u'Save file is not a Skyrim save game.')
        headerSize, = struct.unpack('I',ins.read(4))
        #--Name, location
        version,saveNumber,size = struct.unpack('2IH',ins.read(10))
        header.pcName = ins.read(size)
        header.pcLevel, = struct.unpack('I',ins.read(4))
        size, = struct.unpack('H',ins.read(2))
        header.pcLocation = ins.read(size)
        size, = struct.unpack('H',ins.read(2))
        header.gameDate = ins.read(size)
        hours,minutes,seconds = [int(x) for x in header.gameDate.split('.')]
        playSeconds = hours*60*60 + minutes*60 + seconds
        header.gameDays = float(playSeconds)/(24*60*60)
        header.gameTicks = playSeconds * 1000
        size, = struct.unpack('H',ins.read(2))
        ins.seek(ins.tell()+size+2+4+4+8) # raceEdid, unk0, unk1, unk2, ftime
        ssWidth, = struct.unpack('I',ins.read(4))
        ssHeight, = struct.unpack('I',ins.read(4))
        if ins.tell() != headerSize + 17:
            raise Exception(u'Save game header size (%s) not as expected (%s).' % (ins.tell()-17,headerSize))
        #--Image Data
        ssData = ins.read(3*ssWidth*ssHeight)
        header.image = (ssWidth,ssHeight,ssData)
        #--unknown
        unk3 = ins.read(1)
        #--Masters
        mastersSize, = struct.unpack('I',ins.read(4))
        mastersStart = ins.tell()
        del header.masters[:]
        numMasters, = struct.unpack('B',ins.read(1))
        for count in xrange(numMasters):
            size, = struct.unpack('H',ins.read(2))
            header.masters.append(ins.read(size))
        if ins.tell() != mastersStart + mastersSize:
            raise Exception(u'Save game masters size (%i) not as expected (%i).' % (ins.tell()-mastersStart,mastersSize))

    @staticmethod
    def writeMasters(ins,out,header):
        """Rewrites masters of existing save file."""
        def unpack(format,size): return struct.unpack(format,ins.read(size))
        def pack(format,*args): out.write(struct.pack(format,*args))
        #--Magic (TESV_SAVEGAME)
        out.write(ins.read(13))
        #--Header
        size, = unpack('I',4)
        pack('I',size)
        out.write(ins.read(size-8))
        ssWidth,ssHeight = unpack('2I',8)
        pack('2I',ssWidth,ssHeight)
        #--Screenshot
        out.write(ins.read(3*ssWidth*ssHeight))
        #--formVersion
        out.write(ins.read(1))
        #--plugin info
        oldSize, = unpack('I',4)
        newSize = 1 + sum(len(x)+2 for x in header.masters)
        pack('I',newSize)
        #  Skip old masters
        oldMasters = []
        numMasters, = unpack('B',1)
        pack('B',len(header.masters))
        for x in xrange(numMasters):
            size, = unpack('H',2)
            oldMasters.append(ins.read(size))
        #  Write new masters
        for master in header.masters:
            pack('H',len(master))
            out.write(master.s)
        #--Offsets
        offset = out.tell() - ins.tell()
        #--File Location Table
        for i in xrange(6):
            # formIdArrayCount offset, unkownTable3Offset,
            # globalDataTable1Offset, globalDataTable2Offset,
            # changeFormsOffset, globalDataTable3Offset
            oldOffset, = unpack('I',4)
            pack('I',oldOffset+offset)
        #--Copy the rest
        while True:
            buffer = ins.read(0x5000000)
            if not buffer: break
            out.write(buffer)
        return oldMasters

#--INI files that should show up in the INI Edits tab
iniFiles = [
    u'Skyrim.ini',
    u'SkyrimPrefs.ini',
    ]

#--Name of the default ini file.
defaultIniFile = u'Skyrim_default.ini'

#--INI setting to setup Save Profiles
saveProfilesKey = (u'General',u'SLocalSavePath')

#--The main plugin file Wrye Bash should look for
masterFiles = [
    u'Skyrim.esm',
    u'Update.esm',
    ]

#--Plugin files that can't be deactivated
nonDeactivatableFiles = [
    u'Skyrim.esm',
    u'Update.esm',
    ]

#--Game ESM/ESP/BSA files
#  These filenames need to be in lowercase,
# bethDataFiles = set()
# Moved to skyrim_const

#--Every file in the Data directory from Bethsoft
# allBethFiles = set()
# Moved to skyrim_const

#--BAIN: Directories that are OK to install to
dataDirs = set((
    u'bash patches',
    u'dialogueviews',
    u'docs',
    u'interface',
    u'meshes',
    u'strings',
    u'textures',
    u'video',
    u'lodsettings',
    u'grass',
    u'scripts',
    u'shadersfx',
    u'music',
    u'sound',
    u'seq',
    ))
dataDirsPlus = set((
    u'ini tweaks',
    u'skse',
    u'ini',
    u'asi',
    u'skyproc patchers',
    ))

# Installer -------------------------------------------------------------------
# ensure all path strings are prefixed with 'r' to avoid interpretation of
#   accidental escape sequences
wryeBashDataFiles = set((
    u'Bashed Patch.esp',
    u'Bashed Patch, 0.esp',
    u'Bashed Patch, 1.esp',
    u'Bashed Patch, 2.esp',
    u'Bashed Patch, 3.esp',
    u'Bashed Patch, 4.esp',
    u'Bashed Patch, 5.esp',
    u'Bashed Patch, 6.esp',
    u'Bashed Patch, 7.esp',
    u'Bashed Patch, 8.esp',
    u'Bashed Patch, 9.esp',
    u'Bashed Patch, CBash.esp',
    u'Bashed Patch, Python.esp',
    u'Bashed Patch, Warrior.esp',
    u'Bashed Patch, Thief.esp',
    u'Bashed Patch, Mage.esp',
    u'Bashed Patch, Test.esp',
    u'Docs\\Bash Readme Template.html',
    u'Docs\\wtxt_sand_small.css',
    u'Docs\\wtxt_teal.css',
    u'Docs\\Bash Readme Template.txt',
    u'Docs\\Bashed Patch, 0.html',
    u'Docs\\Bashed Patch, 0.txt',
    ))
wryeBashDataDirs = set((
    u'Bash Patches',
    u'INI Tweaks'
    ))
ignoreDataFiles = set((
    ))
ignoreDataFilePrefixes = set((
    ))
ignoreDataDirs = set((
    u'LSData'
    ))

# Function Info ----------------------------------------------------------------
conditionFunctionData = ( #--0: no param; 1: int param; 2 formid param
    (  0, 'GetWantBlocking', 0, 0),
    (  1, 'GetDistance', 2, 0),
    (  5, 'GetLocked', 0, 0),
    (  6, 'GetPos', 1, 0),
    (  8, 'GetAngle', 1, 0),
    ( 10, 'GetStartingPos', 1, 0),
    ( 11, 'GetStartingAngle', 1, 0),
    ( 12, 'GetSecondsPassed', 0, 0),
    ( 14, 'GetActorValue', 1, 0),
    ( 18, 'GetCurrentTime', 0, 0),
    ( 24, 'GetScale', 0, 0),
    ( 25, 'IsMoving', 0, 0),
    ( 26, 'IsTurning', 0, 0),
    ( 27, 'GetLineOfSight', 2, 0),
    ( 31, 'GetButtonPressed', 0, 0),
    ( 32, 'GetInSameCell', 2, 0),
    ( 35, 'GetDisabled', 0, 0),
    ( 36, 'MenuMode', 1, 0),
    ( 39, 'GetDisease', 0, 0),
    ( 41, 'GetClothingValue', 0, 0),
    ( 42, 'SameFction', 2, 0),
    ( 43, 'SameRace', 2, 0),
    ( 44, 'SameSex', 2, 0),
    ( 45, 'GetDetected', 2, 0),
    ( 46, 'GetDead', 0, 0),
    ( 47, 'GetItemCount', 2, 0),
    ( 48, 'GetGold', 0, 0),
    ( 49, 'GetSleeping', 0, 0),
    ( 50, 'GetTalkedToPC', 0, 0),
    ( 53, 'GetScriptVariable', 2, 1),
    ( 56, 'GetQuestRunning', 2, 0),
    ( 58, 'GetStage', 2, 0),
    ( 59, 'GetStageDone', 2, 1),
    ( 60, 'GetFactionRankDifference', 2, 2),
    ( 61, 'GetAlarmed', 0, 0),
    ( 62, 'IsRaining', 0, 0),
    ( 63, 'GetAttacked', 0, 0),
    ( 64, 'GetIsCreature', 0, 0),
    ( 65, 'GetLockLevel', 0, 0),
    ( 66, 'GetShouldAttack', 2, 0),
    ( 67, 'GetInCell', 2, 0),
    ( 68, 'GetIsClass', 2, 0),
    ( 69, 'GetIsRace', 2, 0),
    ( 70, 'GetIsSex', 2, 0),
    ( 71, 'GetInFaction', 2, 0),
    ( 72, 'GetIsID', 2, 0),
    ( 73, 'GetFactionRank', 2, 0),
    ( 74, 'GetGlobalValue', 2, 0),
    ( 75, 'IsSnowing', 0, 0),
    ( 77, 'GetRandomPercent', 0, 0),
    ( 79, 'GetQuestVvariable', 2, 1),
    ( 80, 'GetLevel', 0, 0),
    ( 81, 'IsRotating', 0, 0),
    ( 83, 'GetLeveledEncounterValue', 1, 0),
    ( 84, 'GetDeadCount', 2, 0),
    ( 91, 'GetIsAlerted', 0, 0),
    ( 98, 'GetPlayerControlsDisabled', 0, 0),
    ( 99, 'GetHeadingAngle', 2, 0),
    (101, 'IsWeaponMagicOut', 0, 0),
    (102, 'IsTorchOut', 0, 0),
    (103, 'IsShieldOut', 0, 0),
    (105, 'IsActionRef', 2, 0),
    (106, 'IsFacingUp', 0, 0),
    (107, 'GetKnockedState', 0, 0),
    (108, 'GetWeaponAnimType', 0, 0),
    (109, 'IsWeaponSkillType', 1, 0),
    (110, 'GetCurrentAIPackage', 0, 0),
    (111, 'IsWaiting', 0, 0),
    (112, 'IsIdlePlaying', 0, 0),
    (116, 'IsIntimidatedbyPlayer', 0, 0),
    (117, 'IsPlayerInRegion', 2, 0),
    (118, 'GetActorAggroRadiusViolated', 0, 0),
    (119, 'GetCrimeKnown', 1, 2),
    (122, 'GetCrime', 2, 1),
    (123, 'IsGreetingPlayer', 0, 0),
    (125, 'IsGuard', 0, 0),
    (127, 'HasBeenEaten', 0, 0),
    (128, 'GetStaminaPercentage', 0, 0),
    (129, 'GetPCIsClass', 2, 0),
    (130, 'GetPCIsRace', 2, 0),
    (131, 'GetPCIsSex', 2, 0),
    (132, 'GetPCInFaction', 2, 0),
    (133, 'SameFactionAsPC', 0, 0),
    (134, 'SameRaceAsPC', 0, 0),
    (135, 'SameSexAsPC', 0, 0),
    (136, 'GetIsReference', 2, 0),
    (141, 'IsTalking', 0, 0),
    (142, 'GetWalkSpeed', 0, 0),
    (143, 'GetCurrentAIProcedure', 0, 0),
    (144, 'GetTrespassWarningLevel', 0, 0),
    (145, 'IsTresPassing', 0, 0),
    (146, 'IsInMyOwnedCell', 0, 0),
    (147, 'GetWindSpeed', 0, 0),
    (148, 'GetCurrentWeatherPercent', 0, 0),
    (149, 'GetIsCurrentWeather', 2, 0),
    (150, 'IsContinuingPackagePCNear', 0, 0),
    (152, 'GetIsCrimeFaction', 2, 0),
    (153, 'CanHaveFlames', 0, 0),
    (154, 'HasFlames', 0, 0),
    (157, 'GetOpenState', 0, 0),
    (159, 'GetSitting', 0, 0),
    (160, 'GetFurnitureMarkerID', 0, 0),
    (161, 'GetIsCurrentPackage', 2, 0),
    (162, 'IsCurrentFurnitureRef', 2, 0),
    (163, 'IsCurrentFurnitureObj', 2, 0),
    (167, 'GetFactionReaction', 2, 2),
    (170, 'GetDayOfWeek', 0, 0),
    (172, 'GetTalkedToPCParam', 2, 0),
    (175, 'IsPCSleeping', 0, 0),
    (176, 'IsPCAMurderer', 0, 0),
    (180, 'HasSameEditorLocAsRef', 2, 2),
    (181, 'HasSameEditorLocAsRefAlias', 2, 2),
    (182, 'GetEquiped', 2, 0),
    (185, 'IsSwimming', 0, 0),
    (188, 'GetPCSleepHours', 0, 0),
    (190, 'GetAmountSoldStolen', 0, 0),
    (192, 'GetIgnoreCrime', 0, 0),
    (193, 'GetPCExpelled', 2, 0),
    (195, 'GetPCFactionMurder', 2, 0),
    (197, 'GetPCEnemyofFaction', 2, 0),
    (199, 'GetPCFactionAttack', 2, 0),
    (203, 'GetDestroyed', 0, 0),
    (205, 'GetActionRef', 0, 0),
    (206, 'GetSelf', 0, 0),
    (207, 'GetContainer', 0, 0),
    (208, 'GetForceRun', 0, 0),
    (210, 'GetForceSneak', 0, 0),
    (214, 'HasMagicEffect', 2, 0),
    (215, 'GetDefaultOpen', 0, 0),
    (219, 'GetAnimAction', 0, 0),
    (223, 'IsSpellTarget', 2, 0),
    (224, 'GetVATSMode', 0, 0),
    (225, 'GetPersuationNumber', 0, 0),
    (226, 'GetVampireFeed', 0, 0),
    (227, 'GetCannibal', 0, 0),
    (228, 'GetIsClassDefault', 2, 0),
    (229, 'GetClassDefaultMatch', 0, 0),
    (230, 'GetInCellParam', 2, 2),
    (232, 'GetCombatTarget', 0, 0),
    (233, 'GetPackageTarget', 0, 0),
    (235, 'GetVatsTargetHeight', 0, 0),
    (237, 'GetIsGhost', 0, 0),
    (242, 'GetUnconscious', 0, 0),
    (244, 'GetRestrained', 0, 0),
    (246, 'GetIsUsedItem', 2, 0),
    (247, 'GetIsUsedItemType', 2, 0),
    (248, 'IsScenePlaying', 2, 0),
    (249, 'IsInDialogWithPlayer', 0, 0),
    (250, 'GetLocationCleared', 2, 0),
    (254, 'GetIsPlayableRace', 0, 0),
    (255, 'GetOffersServicesNow', 0, 0),
    (256, 'GetGameSetting', 1, 0),
    (258, 'HasAssociationType', 2, 2),
    (259, 'HasFamilyRelationship', 2, 0),
    (261, 'HasParentRelationship', 2, 0),
    (262, 'IsWarningAbout', 2, 0),
    (263, 'IsWeaponOut', 0, 0),
    (264, 'HasSpell', 2, 0),
    (265, 'IsTimePassing', 0, 0),
    (266, 'IsPleasant', 0, 0),
    (267, 'IsCloudy', 0, 0),
    (274, 'IsSmallBump', 0, 0),
    (275, 'GetParentRef', 0, 0),
    (277, 'GetBaseActorValue', 1, 0),
    (278, 'IsOwner', 2, 0),
    (280, 'IsCellOwner', 2, 2),
    (282, 'IsHorseStolen', 0, 0),
    (285, 'IsLeftUp', 0, 0),
    (286, 'IsSneaking', 0, 0),
    (287, 'IsRunning', 0, 0),
    (288, 'GetFriendHit', 0, 0),
    (289, 'IsInCombat', 1, 0),
    (296, 'IsAnimPlaying', 2, 0),
    (300, 'IsInInterior', 0, 0),
    (303, 'IsActorsAIOff', 0, 0),
    (304, 'IsWaterObject', 0, 0),
    (305, 'GetPlayerAction', 0, 0),
    (306, 'IsActorUsingATorch', 0, 0),
    (309, 'IsXBox', 0, 0),
    (310, 'GetInWorldspace', 2, 0),
    (312, 'GetPCMiscStat', 1, 0),
    (313, 'GetPairedAnimation', 0, 0),
    (314, 'IsActorAVictim', 0, 0),
    (315, 'GetTotalPersuationNumber', 0, 0),
    (318, 'GetIdleDoneOnce', 0, 0),
    (320, 'GetNoRumors', 0, 0),
    (323, 'GetCombatState', 0, 0),
    (325, 'GetWithinPackageLocation', 2, 0),
    (327, 'IsRidingHorse', 0, 0),
    (329, 'IsFleeing', 0, 0),
    (332, 'IsInDangerousWater', 0, 0),
    (338, 'GetIgnoreFriendlyHits', 0, 0),
    (339, 'IsPlayersLastRiddenHorse', 0, 0),
    (353, 'IsActor', 0, 0),
    (354, 'IsEssential', 0, 0),
    (358, 'IsPlayerMovingIntoNewSpace', 0, 0),
    (359, 'GetInCurrentLoc', 2, 0),
    (360, 'GetInCurrentLocAlias', 2, 0),
    (361, 'GetTimeDead', 0, 0),
    (362, 'HasLinkedRef', 2, 0),
    (363, 'GetLinkedRef', 2, 0),
    (365, 'IsChild', 0, 0),
    (366, 'GetStolenItemValueNoCrime', 2, 0),
    (367, 'GetLastPlayerAction', 0, 0),
    (368, 'IsPlayerActionActive', 1, 0),
    (370, 'IsTalkingActivatorActor', 2, 0),
    (372, 'IsInList', 2, 0),
    (373, 'GetStolenItemValue', 2, 0),
    (375, 'GetCrimeGoldViolent', 2, 0),
    (376, 'GetCrimeGoldNonviolent', 2, 0),
    (378, 'HasShout', 2, 0),
    (381, 'GetHasNote', 2, 0),
    (387, 'GetObjectiveFailed', 2, 1),
    (390, 'GetHitLocation', 0, 0),
    (391, 'IsPC1stPerson', 0, 0),
    (396, 'GetCauseofDeath', 0, 0),
    (397, 'IsLimbGone', 1, 0),
    (398, 'IsWeaponInList', 2, 0),
    (402, 'IsBribedbyPlayer', 0, 0),
    (403, 'GetRelationshipRank', 2, 0),
    (407, 'GetVATSValue', 1, 1),
    (408, 'IsKiller', 2, 0),
    (409, 'IsKillerObject', 2, 0),
    (410, 'GetFactionCombatReaction', 2, 2),
    (414, 'Exists', 2, 0),
    (415, 'GetGroupMemberCount', 0, 0),
    (416, 'GetGroupTargetCount', 0, 0),
    (419, 'GetObjectiveCompleted', 2, 1),
    (420, 'GetObjectiveDisplayed', 2, 1),
    (425, 'GetIsFormType', 2, 0),
    (426, 'GetIsVoiceType', 2, 0),
    (427, 'GetPlantedExplosive', 0, 0),
    (429, 'IsScenePackageRunning', 0, 0),
    (430, 'GetHealthPercentage', 0, 0),
    (432, 'GetIsObjectType', 2, 0),
    (434, 'GetDialogEmotion', 0, 0),
    (435, 'GetDialogEmotionValue', 0, 0),
    (437, 'GetIsCreatureType', 1, 0),
    (444, 'GetInCurrentLocFormList', 2, 0),
    (445, 'GetInZone', 2, 0),
    (446, 'GetVelocity', 1, 0),
    (447, 'GetGraphVariableFloat', 1, 0),
    (448, 'HasPerk', 2, 0),
    (449, 'GetFactionRelation', 2, 0),
    (450, 'IsLastIdlePlayed', 2, 0),
    (453, 'GetPlayerTeammate', 0, 0),
    (458, 'GetActorCrimePlayerEnemy', 0, 0),
    (459, 'GetCrimeGold', 2, 0),
    (462, 'GetPlayerGrabbedRef', 0, 0),
    (463, 'IsPlayerGrabbedRef', 2, 0),
    (465, 'GetKeywordItemCount', 2, 0),
    (467, 'GetBroadcastState', 0, 0),
    (470, 'GetDestructionStage', 0, 0),
    (473, 'GetIsAlignment', 2, 0),
    (476, 'IsProtected', 0, 0),
    (477, 'GetThreatRatio', 2, 0),
    (479, 'GetIsUsedItemEquipType', 2, 0),
    (480, 'GetPlayerName', 0, 0),
    (487, 'IsCarryable', 0, 0),
    (488, 'GetConcussed', 0, 0),
    (491, 'GetMapMarkerVisible', 0, 0),
    (494, 'GetPermanentActorValue', 1, 0),
    (495, 'GetKillingBlowLimb', 0, 0),
    (497, 'CanPayCrimeGold', 2, 0),
    (499, 'GetDaysInJail', 0, 0),
    (500, 'EPAlchemyGetMakingPoison', 0, 0),
    (501, 'EPAlchemyEffectHasKeyword', 2, 0),
    (503, 'GetAllowWorldInteractions', 0, 0),
    (508, 'GetLastHitCritical', 0, 0),
    (513, 'IsCombatTarget', 2, 0),
    (515, 'GetVATSRightAreaFree', 2, 0),
    (516, 'GetVATSLeftAreaFree', 2, 0),
    (517, 'GetVATSBackAreaFree', 2, 0),
    (518, 'GetVATSFrontAreaFree', 2, 0),
    (519, 'GetIsLockBroken', 0, 0),
    (520, 'IsPS3', 0, 0),
    (521, 'IsWin32', 0, 0),
    (522, 'GetVATSRightTargetVisible', 2, 0),
    (523, 'GetVATSLeftTargetVisible', 2, 0),
    (524, 'GetVATSBackTargetVisible', 2, 0),
    (525, 'GetVATSFrontTargetVisible', 2, 0),
    (528, 'IsInCriticalStage', 2, 0),
    (530, 'GetXPForNextLevel', 0, 0),
    (533, 'GetInfamy', 2, 0),
    (534, 'GetInfamyViolent', 2, 0),
    (535, 'GetInfamyNonViolent', 2, 0),
    (543, 'GetQuestCompleted', 2, 0),
    (547, 'IsGoreDisabled', 0, 0),
    (550, 'IsSceneActionComplete', 2, 1),
    (552, 'GetSpellUsageNum', 2, 0),
    (554, 'GetActorsInHigh', 0, 0),
    (555, 'HasLoaded3D', 0, 0),
    (559, 'IsImageSpaceActive', 2, 0),
    (560, 'HasKeyword', 2, 0),
    (561, 'HasRefType', 2, 0),
    (562, 'LocationHasKeyword', 2, 0),
    (563, 'LocationHasRefType', 2, 0),
    (565, 'GetIsEditorLocation', 2, 0),
    (566, 'GetIsAliasRef', 2, 0),
    (567, 'GetIsEditorLocAlias', 2, 0),
    (568, 'IsSprinting', 0, 0),
    (569, 'IsBlocking', 0, 0),
    (570, 'HasEquippedSpell', 2, 0),
    (571, 'GetCurrentCastingType', 2, 0),
    (572, 'GetCurrentDeliveryType', 2, 0),
    (574, 'GetAttackState', 0, 0),
    (575, 'GetAliasedRef', 2, 0),
    (576, 'GetEventData', 2, 2),
    (577, 'IsCloserToAThanB', 2, 2),
    (579, 'GetEquippedShout', 2, 0),
    (580, 'IsBleedingOut', 0, 0),
    (584, 'GetRelativeAngle', 2, 1),
    (589, 'GetMovementDirection', 0, 0),
    (590, 'IsInScene', 0, 0),
    (591, 'GetRefTypeDeadCount', 2, 2),
    (592, 'GetRefTypeAliveCount', 2, 2),
    (594, 'GetIsFlying', 0, 0),
    (595, 'IsCurrentSpell', 2, 2),
    (596, 'SpellHasKeyword', 2, 2),
    (597, 'GetEquippedItemType', 2, 0),
    (598, 'GetLocationAliasCleared', 2, 0),
    (600, 'GetLocAliasRefTypeDeadCount', 2, 2),
    (601, 'GetLocAliasRefTypeAliveCount', 2, 2),
    (602, 'IsWardState', 2, 0),
    (603, 'IsInSameCurrentLocAsRef', 2, 2),
    (604, 'IsInSameCurrentLocAsRefAlias', 2, 2),
    (605, 'LocAliasIsLocation', 2, 2),
    (606, 'GetKeywordDataForLocation', 2, 2),
    (608, 'GetKeywordDataForAlias', 2, 2),
    (610, 'LocAliasHasKeyword', 2, 2),
    (611, 'IsNullPackageData', 1, 0),
    (612, 'GetNumericPackageData', 1, 0),
    (613, 'IsFurnitureAnimType', 2, 0),
    (614, 'IsFurnitureEntryType', 2, 0),
    (615, 'GetHighestRelationshipRank', 0, 0),
    (616, 'GetLowestRelationshipRank', 0, 0),
    (617, 'HasAssociationTypeAny', 2, 0),
    (618, 'HasFamilyRelationshipAny', 0, 0),
    (619, 'GetPathingTargetOffset', 1, 0),
    (620, 'GetPathingTargetAngleOffset', 1, 0),
    (621, 'GetPathingTargetSpeed', 0, 0),
    (622, 'GetPathingTargetSpeedAngle', 1, 0),
    (623, 'GetMovementSpeed', 0, 0),
    (624, 'GetInContainer', 2, 0),
    (625, 'IsLocationLoaded', 2, 0),
    (626, 'IsLocAliasLoaded', 2, 0),
    (627, 'IsDualCasting', 0, 0),
    (629, 'GetVMQuestVariable', 2, 1),
    (630, 'GetVMScriptVariable', 2, 1),
    (631, 'IsEnteringInteractionQuick', 0, 0),
    (632, 'IsCasting', 0, 0),
    (633, 'GetFlyingState', 0, 0),
    (635, 'IsInFavorState', 0, 0),
    (636, 'HasTwoHandedWeaponEquipped', 0, 0),
    (637, 'IsExitingInstant', 0, 0),
    (638, 'IsInFriendStatewithPlayer', 0, 0),
    (639, 'GetWithinDistance', 2, 1),
    (640, 'GetActorValuePercent', 1, 0),
    (641, 'IsUnique', 0, 0),
    (642, 'GetLastBumpDirection', 0, 0),
    (644, 'IsInFurnitureState', 2, 0),
    (645, 'GetIsInjured', 0, 0),
    (646, 'GetIsCrashLandRequest', 0, 0),
    (647, 'GetIsHastyLandRequest', 0, 0),
    (650, 'IsLinkedTo', 2, 2),
    (651, 'GetKeywordDataForCurrentLocation', 2, 0),
    (652, 'GetInSharedCrimeFaction', 2, 0),
    (653, 'GetBribeAmount', 0, 0),
    (654, 'GetBribeSuccess', 0, 0),
    (655, 'GetIntimidateSuccess', 0, 0),
    (656, 'GetArrestedState', 0, 0),
    (657, 'GetArrestingActor', 0, 0),
    (659, 'EPTemperingItemIsEnchanted', 0, 0),
    (660, 'EPTemperingItemHasKeyword', 2, 0),
    (661, 'GetReceivedGiftValue', 0, 0),
    (662, 'GetGiftGivenValue', 0, 0),
    (664, 'GetReplacedItemType', 2, 0),
    (672, 'IsAttacking', 0, 0),
    (673, 'IsPowerAttacking', 0, 0),
    (674, 'IsLastHostileActor', 0, 0),
    (675, 'GetGraphVariableInt', 1, 0),
    (676, 'GetCurrentShoutVariation', 0, 0),
    (678, 'ShouldAttackKill', 2, 0),
    (680, 'GetActivationHeight', 0, 0),
    (681, 'EPModSkillUsage_IsAdvancedSkill', 1, 0),
    (682, 'WornHasKeyword', 2, 0),
    (683, 'GetPathingCurrentSpeed', 0, 0),
    (684, 'GetPathingCurrentSpeedAngle', 1, 0),
    (691, 'EPModSkillUsage_AdvancedObjectHasKeyword', 2, 0),
    (692, 'EPModSkillUsage_IsAdvancedAction', 2, 0),
    (693, 'EPMagic_SpellHasKeyword', 2, 0),
    (694, 'GetNoBleedoutRecovery', 0, 0),
    (696, 'EPMagic_SpellHasSkill', 1, 0),
    (697, 'IsAttackType', 2, 0),
    (698, 'IsAllowedToFly', 0, 0),
    (699, 'HasMagicEffectKeyword', 2, 0),
    (700, 'IsCommandedActor', 0, 0),
    (701, 'IsStaggered', 0, 0),
    (702, 'IsRecoiling', 0, 0),
    (703, 'IsExitingInteractionQuick', 0, 0),
    (704, 'IsPathing', 0, 0),
    (705, 'GetShouldHelp', 2, 0),
    (706, 'HasBoundWeaponEquipped', 2, 0),
    (707, 'GetCombatTargetHasKeyword', 2, 0),
    (709, 'GetCombatGroupMemberCount', 0, 0),
    (710, 'IsIgnoringCombat', 0, 0),
    (711, 'GetLightLevel', 0, 0),
    (713, 'SpellHasCastingPerk', 2, 0),
    (714, 'IsBeingRidden', 0, 0),
    (715, 'IsUndead', 0, 0),
    (716, 'GetRealHoursPassed', 0, 0),
    (718, 'IsUnlockedDoor', 0, 0),
    (719, 'IsHostileToActor', 2, 0),
    (720, 'GetTargetHeight', 0, 0),
    (721, 'IsPoison', 0, 0),
    (722, 'WornApparelHasKeywordCount', 2, 0),
    (723, 'GetItemHealthPercent', 0, 0),
    (724, 'EffectWasDualCast', 0, 0),
    (725, 'GetKnockStateEnum', 0, 0),
    )

allConditions = set(entry[0] for entry in conditionFunctionData)
fid1Conditions = set(entry[0] for entry in conditionFunctionData if entry[2] == 2)
fid2Conditions = set(entry[0] for entry in conditionFunctionData if entry[3] == 2)

# Magic Info ------------------------------------------------------------------
weaponTypes = (
    _(u'Blade (1 Handed)'),
    _(u'Blade (2 Handed)'),
    _(u'Blunt (1 Handed)'),
    _(u'Blunt (2 Handed)'),
    _(u'Staff'),
    _(u'Bow'),
    )

#The pickle file for this game. Holds encoded GMST IDs from the big list below.
pklfile = r'bash\db\Skyrim_ids.pkl'

#--List of GMST's in the main plugin (Skyrim.esm) that have 0x00000000
#  as the form id.  Any GMST as such needs it Editor Id listed here.
gmstEids = ['bAutoAimBasedOnDistance','fActionPointsAttackMagic','fActionPointsAttackRanged',
    'fActionPointsFOVBase','fActiveEffectConditionUpdateInterval','fActorAlertSoundTimer',
    'fActorAlphaFadeSeconds','fActorAnimZAdjust','fActorArmorDesirabilityDamageMult',
    'fActorArmorDesirabilitySkillMult','fActorLeaveWaterBreathTimer','fActorLuckSkillMult',
    'fActorStrengthEncumbranceMult','fActorSwimBreathBase','fActorTeleportFadeSeconds',
    'fActorWeaponDesirabilityDamageMult','fActorWeaponDesirabilitySkillMult','fAiAcquireKillBase',
    'fAiAcquireKillMult','fAIAcquireObjectDistance','fAiAcquirePickBase',
    'fAiAcquirePickMult','fAiAcquireStealBase','fAiAcquireStealMult',
    'fAIActivateHeight','fAIActivateReach','fAIActorPackTargetHeadTrackMod',
    'fAIAimBlockedHalfCircleRadius','fAIAimBlockedToleranceDegrees','fAIAwareofPlayerTimer',
    'fAIBestHeadTrackDistance','fAICombatFleeScoreThreshold','fAICombatNoAreaEffectAllyDistance',
    'fAICombatNoTargetLOSPriorityMult','fAICombatSlopeDifference','fAICombatTargetUnreachablePriorityMult',
    'fAICombatUnreachableTargetPriorityMult','fAICommentTimeWindow','fAIConversationExploreTime',
    'fAIDialogueDistance','fAIDistanceRadiusMinLocation','fAIDistanceTeammateDrawWeapon',
    'fAIDodgeDecisionBase','fAIDodgeFavorLeftRightMult','fAIDodgeVerticalRangedAttackMult',
    'fAIDodgeWalkChance','fAIEnergyLevelBase','fAIEngergyLevelMult',
    'fAIEscortFastTravelMaxDistFromPath','fAIEscortHysteresisWidth','fAIEscortStartStopDelayTime',
    'fAIEscortWaitDistanceExterior','fAIEscortWaitDistanceInterior','fAIExplosiveWeaponDamageMult',
    'fAIExplosiveWeaponRangeMult','fAIExteriorSpectatorDetection','fAIExteriorSpectatorDistance',
    'fAIFaceTargetAnimationAngle','fAIFindBedChairsDistance','fAIFleeConfBase',
    'fAIFleeConfMult','fAIFleeHealthMult','fAIFleeSuccessTimeout',
    'fAIHoldDefaultHeadTrackTimer','fAIHorseSearchDistance','fAIIdleAnimationDistance',
    'fAIIdleAnimationLargeCreatureDistanceMult','fAIIdleWaitTimeComplexScene','fAIInteriorHeadTrackMult',
    'fAIInteriorSpectatorDetection','fAIInteriorSpectatorDistance','fAILockDoorsSeenRecentlySeconds',
    'fAIMagicSpellMult','fAIMagicTimer','fAIMaxAngleRangeMovingToStartSceneDialogue','fAIMaxHeadTrackDistance',
    'fAIMaxHeadTrackDistanceFromPC','fAIMaxLargeCreatureHeadTrackDistance','fAIMaxSmileDistance',
    'fAIMaxWanderTime','fAIMeleeArmorMult','fAIMeleeHandMult',
    'fAIMeleeWeaponMult','fAIMinAngleRangeToStartSceneDialogue','fAIMinGreetingDistance',
    'fAIMinLocationHeight','fAIMoveDistanceToRecalcFollowPath','fAIMoveDistanceToRecalcTravelPath',
    'fAIMoveDistanceToRecalcTravelToActor','fAIPatrolHysteresisWidth','fAIPatrolMinSecondsAtNormalFurniture',
    'fAIPowerAttackCreatureChance','fAIPowerAttackKnockdownBonus','fAIPowerAttackNPCChance',
    'fAIPowerAttackRecoilBonus','fAIPursueDistanceLineOfSight','fAIRandomizeInitialLocationMinRadius',
    'fAIRangedWeaponMult','fAIRangMagicSpellMult','fAIRevertToScriptTracking',
    'fAIShoutRetryDelaySeconds','fAISocialTimerToWaitForEvent','fAISpectatorCommentTimer',
    'fAISpectatorShutdownDistance','fAISpectatorThreatDistExplosion','fAISpectatorThreatDistMelee',
    'fAISpectatorThreatDistMine','fAISpectatorThreatDistRanged','fAIStayonScriptHeadtrack',
    'fAItalktoNPCtimer','fAItalktosameNPCtimer','fAIUpdateMovementRestrictionsDistance',
    'fAIUseMagicToleranceDegrees','fAIUseWeaponAnimationTimeoutSeconds','fAIUseWeaponToleranceDegrees',
    'fAIWaitingforScriptCallback','fAIWalkAwayTimerForConversation','fAIWanderDefaultMinDist',
    'fAlchemyGoldMult','fAlchemyIngredientInitMult','fAlignEvilMaxKarma',
    'fAlignGoodMinKarma','fAlignMaxKarma','fAlignMinKarma',
    'fAlignVeryEvilMaxKarma','fAlignVeryGoodMinKarma','fAmbushOverRideRadiusforPlayerDetection',
    'fArmorRatingPCBase','fArmorWeightLightMaxMod','fArrowBounceBlockPercentage',
    'fArrowBounceLinearSpeed','fArrowBounceRotateSpeed','fArrowBowFastMult',
    'fArrowBowMinTime','fArrowBowSlowMult','fArrowFakeMass',
    'fArrowGravityBase','fArrowGravityMin','fArrowGravityMult',
    'fArrowMaxDistance','fArrowMinBowVelocity','fArrowMinDistanceForTrails',
    'fArrowMinPower','fArrowMinSpeedForTrails','fArrowMinVelocity',
    'fArrowOptimalDistance','fArrowSpeedMult','fArrowWeakGravity',
    'fArrowWobbleAmplitude','fArrowWobbleCurve','fArrowWobbleDuration',
    'fArrowWobblePeriod','fAttributeClassPrimaryBonus','fAttributeClassSecondaryBonus',
    'fAuroraFadeOutStart','fAutoAimMaxDegreesMelee','fAutoAimMaxDegreesVATS',
    'fAutomaticWeaponBurstCooldownTime','fAutomaticWeaponBurstFireTime','fAvoidPlayerDistance',
    'fBarterBuyMin','fBarterSellMax','fBeamWidthDefault',
    'fBigBumpSpeed','fBleedoutCheck','fBlockAmountHandToHandMult',
    'fBlockScoreNoShieldMult','fBlockSkillBase','fBloodSplatterCountBase',
    'fBloodSplatterCountDamageBase','fBloodSplatterCountDamageMult','fBloodSplatterCountRandomMargin',
    'fBodyMorphWeaponAdjustMult','fBowDrawTime','fBowHoldTimer',
    'fBowZoomStaminaRegenDelay','fBribeBase','fBribeMoralityMult',
    'fBribeMult','fBribeNPCLevelMult','fBSUnitsPerFoot',
    'fBuoyancyMultBody','fBuoyancyMultExtremity','fCameraShakeDistFadeDelta',
    'fCameraShakeDistFadeStart','fCameraShakeDistMin','fCameraShakeExplosionDistMult',
    'fCameraShakeFadeTime','fCameraShakeMultMin','fCameraShakeTime',
    'fCharacterControllerMultipleStepSpeed','fChaseDetectionTimerSetting','fCheckDeadBodyTimer',
    'fCheckPositionFallDistance','fClosetoPlayerDistance','fClothingArmorBase',
    'fClothingBase','fClothingClassScale','fClothingJewelryBase',
    'fClothingJewelryScale','fCombatAcquirePickupAnimationDelay','fCombatAcquireWeaponAmmoMinimumScoreMult',
    'fCombatAcquireWeaponAvoidTargetRadius','fCombatAcquireWeaponCloseDistanceMax','fCombatAcquireWeaponCloseDistanceMin',
    'fCombatAcquireWeaponDisarmedAcquireTime','fCombatAcquireWeaponDisarmedDistanceMax','fCombatAcquireWeaponDisarmedDistanceMin',
    'fCombatAcquireWeaponDisarmedTime','fCombatAcquireWeaponEnchantmentChargeMult','fCombatAcquireWeaponFindAmmoDistance',
    'fCombatAcquireWeaponMeleeScoreMult','fCombatAcquireWeaponMinimumScoreMult','fCombatAcquireWeaponMinimumTargetDistance',
    'fCombatAcquireWeaponRangedDistanceMax','fCombatAcquireWeaponRangedDistanceMin','fCombatAcquireWeaponReachDistance',
    'fCombatAcquireWeaponScoreCostMult','fCombatAcquireWeaponScoreRatioMax','fCombatAcquireWeaponSearchFailedDelay',
    'fCombatAcquireWeaponSearchRadiusBuffer','fCombatAcquireWeaponSearchSuccessDelay','fCombatAcquireWeaponTargetDistanceCheck',
    'fCombatAcquireWeaponUnarmedDistanceMax','fCombatAcquireWeaponUnarmedDistanceMin','fCombatActiveCombatantAttackRangeDistance',
    'fCombatActiveCombatantLastSeenTime','fCombatAdvanceInnerRadiusMid','fCombatAdvanceInnerRadiusMin',
    'fCombatAdvanceLastDamagedThreshold','fCombatAdvanceNormalAttackChance','fCombatAdvancePathRetryTime',
    'fCombatAdvanceRadiusStaggerMult','fCombatAimDeltaThreshold','fCombatAimLastSeenLocationTimeLimit',
    'fCombatAimMeleeHighPriorityUpdateTime','fCombatAimMeleeUpdateTime','fCombatAimProjectileBlockedTime',
    'fCombatAimProjectileGroundMinRadius','fCombatAimProjectileRandomOffset','fCombatAimProjectileUpdateTime',
    'fCombatAimTrackTargetUpdateTime','fCombatAngleTolerance','fCombatAnticipatedLocationCheckDistance',
    'fCombatAnticipateTime','fCombatApproachTargetSlowdownDecelerationMult','fCombatApproachTargetSlowdownDistance',
    'fCombatApproachTargetSlowdownUpdateTime','fCombatApproachTargetSlowdownVelocityAngle','fCombatApproachTargetSprintStopMovingRange',
    'fCombatApproachTargetSprintStopRange','fCombatApproachTargetUpdateTime','fCombatAreaHoldPositionMinimumRadius',
    'fCombatAreaStandardAttackedRadius','fCombatAreaStandardAttackedTime','fCombatAreaStandardCheckViewConeDistanceMax',
    'fCombatAreaStandardCheckViewConeDistanceMin','fCombatAreaStandardFlyingRadiusMult','fCombatAreaStandardRadius',
    'fCombatAttackAllowedOverrunDistance','fCombatAttackAnimationDrivenDelayTime','fCombatAttackAnticipatedDistanceMin',
    'fCombatAttackChanceBlockingMultMax','fCombatAttackChanceBlockingMultMin','fCombatAttackChanceBlockingSwingMult',
    'fCombatAttackChanceLastAttackBonus','fCombatAttackChanceLastAttackBonusTime','fCombatAttackChanceMax',
    'fCombatAttackChanceMin','fCombatAttackCheckTargetRangeDistance','fCombatAttackMovingAttackDistance',
    'fCombatAttackMovingAttackReachMult','fCombatAttackMovingStrikeAngleMult','fCombatAttackPlayerAnticipateMult',
    'fCombatAttackStationaryAttackDistance','fCombatAttackStrikeAngleMult','fCombatAvoidThreatsChance',
    'fCombatBackoffChance','fCombatBackoffMinDistanceMult','fCombatBashChanceMax',
    'fCombatBashChanceMin','fCombatBashTargetBlockingMult','fCombatBetweenAdvanceTimer',
    'fCombatBlockAttackChanceMax','fCombatBlockAttackChanceMin','fCombatBlockAttackReachMult',
    'fCombatBlockAttackStrikeAngleMult','fCombatBlockChanceMax','fCombatBlockChanceMin',
    'fCombatBlockMaxTargetRetreatVelocity','fCombatBlockStartDistanceMax','fCombatBlockStartDistanceMin',
    'fCombatBlockStopDistanceMax','fCombatBlockStopDistanceMin','fCombatBlockTimeMax',
    'fCombatBlockTimeMid','fCombatBlockTimeMin','fCombatBoltStickDepth',
    'fCombatBoundWeaponDPSBonus','fCombatBuffMaxTimer','fCombatBuffStandoffTimer',
    'fCombatCastConcentrationOffensiveMagicCastTimeMax','fCombatCastConcentrationOffensiveMagicCastTimeMin','fCombatCastConcentrationOffensiveMagicChanceMax',
    'fCombatCastConcentrationOffensiveMagicChanceMin','fCombatCastConcentrationOffensiveMagicWaitTimeMax','fCombatCastConcentrationOffensiveMagicWaitTimeMin',
    'fCombatCastImmediateOffensiveMagicChanceMax','fCombatCastImmediateOffensiveMagicChanceMin','fCombatCastImmediateOffensiveMagicHoldTimeAbsoluteMin',
    'fCombatCastImmediateOffensiveMagicHoldTimeMax','fCombatCastImmediateOffensiveMagicHoldTimeMin','fCombatCastImmediateOffensiveMagicHoldTimeMinDistance',
    'fCombatChangeProcessFaceTargetDistance','fCombatCircleAngleMax','fCombatCircleAngleMin',
    'fCombatCircleAnglePlayerMult','fCombatCircleChanceMax','fCombatCircleChanceMin',
    'fCombatCircleDistanceMax','fCombatCircleDistantChanceMax','fCombatCircleDistantChanceMin',
    'fCombatCircleMinDistanceMult','fCombatCircleMinDistanceRadiusMult','fCombatCircleMinMovementDistance',
    'fCombatCircleViewConeAngle','fCombatCloseRangeTrackTargetDistance','fCombatClusterUpdateTime',
    'fCombatCollectAlliesTimer','fCombatCoverAttackMaxWaitTime','fCombatCoverAttackOffsetDistance',
    'fCombatCoverAttackTimeMax','fCombatCoverAttackTimeMid','fCombatCoverAttackTimeMin',
    'fCombatCoverAvoidTargetRadius','fCombatCoverCheckCoverHeightMin','fCombatCoverCheckCoverHeightOffset',
    'fCombatCoverEdgeOffsetDistance','fCombatCoverLedgeOffsetDistance','fCombatCoverMaxRangeMult',
    'fCombatCoverMidPointMaxRangeBuffer','fCombatCoverMinimumActiveRange','fCombatCoverMinimumRange',
    'fCombatCoverObstacleMovedTime','fCombatCoverRangeMaxActiveMult','fCombatCoverRangeMaxBufferDistance',
    'fCombatCoverRangeMinActiveMult','fCombatCoverRangeMinBufferDistance','fCombatCoverReservationWidthMult',
    'fCombatCoverSearchDistanceMax','fCombatCoverSearchDistanceMin','fCombatCoverSearchFailedDelay',
    'fCombatCoverSecondaryThreatLastSeenTime','fCombatCoverSecondaryThreatMinDistance','fCombatCoverWaitLookOffsetDistance',
    'fCombatCoverWaitTimeMax','fCombatCoverWaitTimeMid','fCombatCoverWaitTimeMin',
    'fCombatDamageBonusMeleeSneakingMult','fCombatDamageScale','fCombatDeadActorHitConeMult',
    'fCombatDetectionDialogueMaxElapsedTime','fCombatDetectionDialogueMinElapsedTime','fCombatDetectionFleeingLostRemoveTime',
    'fCombatDetectionLostCheckNoticedDistance','fCombatDetectionLostRemoveDistance','fCombatDetectionLostRemoveDistanceTime',
    'fCombatDetectionLostRemoveTime','fCombatDetectionLostTimeLimit','fCombatDetectionLowDetectionDistance',
    'fCombatDetectionLowPriorityDistance','fCombatDetectionNoticedDistanceLimit','fCombatDetectionNoticedTimeLimit',
    'fCombatDetectionVeryLowPriorityDistance','fCombatDialogueAllyKilledDistanceMult','fCombatDialogueAllyKilledMaxElapsedTime',
    'fCombatDialogueAllyKilledMinElapsedTime','fCombatDialogueAttackDistanceMult','fCombatDialogueAvoidThreatDistanceMult',
    'fCombatDialogueAvoidThreatMaxElapsedTime','fCombatDialogueAvoidThreatMinElapsedTime','fCombatDialogueBashDistanceMult',
    'fCombatDialogueBleedoutDistanceMult','fCombatDialogueBleedOutMaxElapsedTime','fCombatDialogueBleedOutMinElapsedTime',
    'fCombatDialogueBlockDistanceMult','fCombatDialogueDeathDistanceMult','fCombatDialogueFleeDistanceMult',
    'fCombatDialogueFleeMaxElapsedTime','fCombatDialogueFleeMinElapsedTime','fCombatDialogueGroupStrategyDistanceMult',
    'fCombatDialogueHitDistanceMult','fCombatDialoguePowerAttackDistanceMult','fCombatDialogueTauntDistanceMult',
    'fCombatDisarmedFindBetterWeaponInitialTime','fCombatDisarmedFindBetterWeaponTime','fCombatDismemberedLimbVelocity',
    'fCombatDistanceMin','fCombatDiveBombChanceMax','fCombatDiveBombChanceMin',
    'fCombatDiveBombOffsetPercent','fCombatDiveBombSlowDownDistance','fCombatDodgeAccelerationMult',
    'fCombatDodgeAcceptableThreatScoreMult','fCombatDodgeAnticipateThreatTime','fCombatDodgeBufferDistance',
    'fCombatDodgeChanceMax','fCombatDodgeChanceMin','fCombatDodgeDecelerationMult',
    'fCombatDodgeMovingReactionTime','fCombatDodgeReactionTime','fCombatDPSBowSpeedMult',
    'fCombatDPSMeleeSpeedMult','fCombatEffectiveDistanceAnticipateTime','fCombatEnvironmentBloodChance',
    'fCombatFallbackChanceMax','fCombatFallbackChanceMin','fCombatFallbackDistanceMax',
    'fCombatFallbackDistanceMin','fCombatFallbackMaxAngle','fCombatFallbackMinMovementDistance',
    'fCombatFallbackWaitTimeMax','fCombatFallbackWaitTimeMin','fCombatFindAllyAttackLocationAllyRadius',
    'fCombatFindAllyAttackLocationDistanceMax','fCombatFindAllyAttackLocationDistanceMin','fCombatFindAttackLocationAvoidTargetRadius',
    'fCombatFindAttackLocationDistance','fCombatFindAttackLocationKeyAngle','fCombatFindAttackLocationKeyHeight',
    'fCombatFindBetterWeaponTime','fCombatFindLateralAttackLocationDistance','fCombatFindLateralAttackLocationIntervalMax',
    'fCombatFindLateralAttackLocationIntervalMin','fCombatFiringArcStationaryTurnMult','fCombatFlankingAngleOffset',
    'fCombatFlankingAngleOffsetCostMult','fCombatFlankingAngleOffsetMax','fCombatFlankingDirectionDistanceMult',
    'fCombatFlankingDirectionGoalAngleOffset','fCombatFlankingDirectionOffsetCostMult','fCombatFlankingDirectionRotateAngleOffset',
    'fCombatFlankingDistanceMax','fCombatFlankingDistanceMin','fCombatFlankingGoalAngleFarMax',
    'fCombatFlankingGoalAngleFarMaxDistance','fCombatFlankingGoalAngleFarMin','fCombatFlankingGoalAngleFarMinDistance',
    'fCombatFlankingGoalAngleNear','fCombatFlankingGoalCheckDistanceMax','fCombatFlankingGoalCheckDistanceMin',
    'fCombatFlankingGoalCheckDistanceMult','fCombatFlankingLocationGridSize','fCombatFlankingMaxTurnAngle',
    'fCombatFlankingMaxTurnAngleGoal','fCombatFlankingNearDistance','fCombatFlankingRotateAngle',
    'fCombatFlankingStalkRange','fCombatFlankingStalkTimeMax','fCombatFlankingStalkTimeMin',
    'fCombatFlankingStepDistanceMax','fCombatFlankingStepDistanceMin','fCombatFlankingStepDistanceMult',
    'fCombatFleeAllyDistanceMax','fCombatFleeAllyDistanceMin','fCombatFleeAllyRadius',
    'fCombatFleeCoverMinDistance','fCombatFleeCoverSearchRadius','fCombatFleeDistanceExterior',
    'fCombatFleeDistanceInterior','fCombatFleeDoorDistanceMax','fCombatFleeDoorTargetCheckDistance',
    'fCombatFleeInitialDoorRestrictChance','fCombatFleeLastDoorRestrictTime','fCombatFleeTargetAvoidRadius',
    'fCombatFleeTargetGatherRadius','fCombatFleeUseDoorChance','fCombatFleeUseDoorRestrictTime',
    'fCombatFlightEffectiveDistance','fCombatFlightMinimumRange','fCombatFlyingAttackChanceMax',
    'fCombatFlyingAttackChanceMin','fCombatFlyingAttackTargetDistanceThreshold','fCombatFollowRadiusBase',
    'fCombatFollowRadiusMin','fCombatFollowSneakFollowRadius','fCombatForwardAttackChance',
    'fCombatGiantCreatureReachMult','fCombatGrenadeBounceTimeMax','fCombatGrenadeBounceTimeMin',
    'fCombatGroundAttackChanceMax','fCombatGroundAttackChanceMin','fCombatGroupCombatStrengthUpdateTime',
    'fCombatGroupOffensiveMultMin','fCombatGuardFollowBufferDistance','fCombatGuardRadiusMin',
    'fCombatGuardRadiusMult','fCombatHideCheckViewConeDistanceMax','fCombatHideCheckViewConeDistanceMin',
    'fCombatHideFailedTargetDistance','fCombatHideFailedTargetLOSDistance','fCombatHitConeAngle',
    'fCombatHoverAngleLimit','fCombatHoverAngleMax','fCombatHoverAngleMin',
    'fCombatHoverChanceMax','fCombatHoverChanceMin','fCombatHoverTimeMax',
    'fCombatInTheWayTimer','fCombatInventoryDesiredRangeScoreMultMax','fCombatInventoryDesiredRangeScoreMultMid',
    'fCombatInventoryDesiredRangeScoreMultMin','fCombatInventoryDualWieldScorePenalty','fCombatInventoryEquipmentMinScoreMult',
    'fCombatInventoryEquippedScoreBonus','fCombatInventoryMaxRangeEquippedBonus','fCombatInventoryMaxRangeScoreMult',
    'fCombatInventoryMeleeEquipRange','fCombatInventoryMinEquipTimeBlock','fCombatInventoryMinEquipTimeDefault',
    'fCombatInventoryMinEquipTimeMagic','fCombatInventoryMinEquipTimeShout','fCombatInventoryMinEquipTimeStaff',
    'fCombatInventoryMinEquipTimeTorch','fCombatInventoryMinEquipTimeWeapon','fCombatInventoryMinRangeScoreMult',
    'fCombatInventoryMinRangeUnequippedBonus','fCombatInventoryOptimalRangePercent','fCombatInventoryRangedScoreMult',
    'fCombatInventoryResourceCurrentRequiredMult','fCombatInventoryResourceDesiredRequiredMult','fCombatInventoryResourceRegenTime',
    'fCombatInventoryShieldEquipRange','fCombatInventoryShoutMaxRecoveryTime','fCombatInventoryTorchEquipRange',
    'fCombatInventoryUpdateTimer','fCombatIronSightsDistance','fCombatIronSightsRangeMult',
    'fCombatItemBuffTimer','fCombatKillMoveDamageMult','fCombatLandingAvoidActorRadius',
    'fCombatLandingSearchDistance','fCombatLandingZoneDistance','fCombatLineOfSightTimer',
    'fCombatLocationTargetRadiusMin','fCombatLowFleeingTargetHitPercent','fCombatLowMaxAttackDistance',
    'fCombatLowTargetHitPercent','fCombatMagicArmorDistanceMax','fCombatMagicArmorDistanceMin',
    'fCombatMagicArmorMinCastTime','fCombatMagicBoundItemDistance','fCombatMagicBuffDuration',
    'fCombatMagicCloakDistanceMax','fCombatMagicCloakDistanceMin','fCombatMagicCloakMinCastTime',
    'fCombatMagicConcentrationAimVariance','fCombatMagicConcentrationFiringArcMult','fCombatMagicConcentrationMinCastTime',
    'fCombatMagicConcentrationScoreDuration','fCombatMagicDefaultLongDuration','fCombatMagicDefaultMinCastTime',
    'fCombatMagicDefaultShortDuration','fCombatMagicDisarmDistance','fCombatMagicDisarmRestrictTime',
    'fCombatMagicDrinkPotionWaitTime','fCombatMagicDualCastChance','fCombatMagicDualCastInterruptTime',
    'fCombatMagicImmediateAimVariance','fCombatMagicInvisibilityDistance','fCombatMagicInvisibilityMinCastTime',
    'fCombatMagicLightMinCastTime','fCombatMagicOffensiveMinCastTime','fCombatMagicParalyzeDistance',
    'fCombatMagicParalyzeMinCastTime','fCombatMagicParalyzeRestrictTime','fCombatMagicProjectileFiringArc',
    'fCombatMagicReanimateDistance','fCombatMagicReanimateMinCastTime','fCombatMagicReanimateRestrictTime',
    'fCombatMagicStaggerDistance','fCombatMagicSummonMinCastTime','fCombatMagicSummonRestrictTime',
    'fCombatMagicTacticalDuration','fCombatMagicTargetEffectMinCastTime','fCombatMagicWardAttackRangeDistance',
    'fCombatMagicWardAttackReachMult','fCombatMagicWardCooldownTime','fCombatMagicWardMagickaCastLimit',
    'fCombatMagicWardMagickaEquipLimit','fCombatMagicWardMinCastTime','fCombatMaintainOptimalDistanceMaxAngle',
    'fCombatMaintainRangeDistanceMin','fCombatMaxHoldScore','fCombatMaximumOptimalRangeMax',
    'fCombatMaximumOptimalRangeMid','fCombatMaximumOptimalRangeMin','fCombatMaximumProjectileRange',
    'fCombatMaximumRange','fCombatMeleeTrackTargetDistanceMax','fCombatMeleeTrackTargetDistanceMin',
    'fCombatMinEngageDistance','fCombatMissileImpaleDepth','fCombatMonitorBuffsTimer',
    'fCombatMoveToActorBufferDistance','fCombatMusicGroupThreatRatioMax','fCombatMusicGroupThreatRatioMin',
    'fCombatMusicGroupThreatRatioTimer','fCombatMusicNearCombatInnerRadius','fCombatMusicNearCombatOuterRadius',
    'fCombatMusicPlayerCombatStrengthCap','fCombatMusicPlayerNearStrengthMult','fCombatMusicStopTime',
    'fCombatMusicUpdateTime','fCombatOffensiveBashChanceMax','fCombatOffensiveBashChanceMin',
    'fCombatOptimalRangeMaxBufferDistance','fCombatOptimalRangeMinBufferDistance','fCombatOrbitTimeMax',
    'fCombatOrbitTimeMin','fCombatParalyzeTacticalDuration','fCombatPathingAccelerationMult',
    'fCombatPathingCurvedPathSmoothingMult','fCombatPathingDecelerationMult','fCombatPathingGoalRayCastPathDistance',
    'fCombatPathingIncompletePathMinDistance','fCombatPathingLocationCenterOffsetMult','fCombatPathingLookAheadDelta',
    'fCombatPathingNormalizedRotationSpeed','fCombatPathingRefLocationUpdateDistance','fCombatPathingRefLocationUpdateTimeDistanceMax',
    'fCombatPathingRefLocationUpdateTimeDistanceMin','fCombatPathingRefLocationUpdateTimeMax','fCombatPathingRefLocationUpdateTimeMin',
    'fCombatPathingRetryWaitTime','fCombatPathingRotationAccelerationMult','fCombatPathingStartRayCastPathDistance',
    'fCombatPathingStraightPathCheckDistance','fCombatPathingStraightRayCastPathDistance','fCombatPathingUpdatePathCostMult',
    'fCombatPerchAttackChanceMax','fCombatPerchAttackChanceMin','fCombatPerchAttackTimeMax',
    'fCombatPerchMaxTargetAngle','fCombatProjectileMaxRangeMult','fCombatProjectileMaxRangeOptimalMult',
    'fCombatRadiusMinMult','fCombatRangedAimVariance','fCombatRangedAttackChanceLastAttackBonus',
    'fCombatRangedAttackChanceLastAttackBonusTime','fCombatRangedAttackChanceMax','fCombatRangedAttackChanceMin',
    'fCombatRangedAttackHoldTimeAbsoluteMin','fCombatRangedAttackHoldTimeMax','fCombatRangedAttackHoldTimeMin',
    'fCombatRangedAttackHoldTimeMinDistance','fCombatRangedAttackMaximumHoldTime','fCombatRangedDistance',
    'fCombatRangedMinimumRange','fCombatRangedProjectileFiringArc','fCombatRangedStandoffTimer',
    'fCombatRelativeDamageMod','fCombatRestoreHealthPercentMax','fCombatRestoreHealthPercentMin',
    'fCombatRestoreHealthRestrictTime','fCombatRestoreMagickaPercentMax','fCombatRestoreMagickaPercentMin',
    'fCombatRestoreMagickaRestrictTime','fCombatRestoreStopCastThreshold','fCombatRoundAmount',
    'fCombatSearchAreaUpdateTime','fCombatSearchCenterRadius','fCombatSearchCheckDestinationDistanceMax',
    'fCombatSearchCheckDestinationDistanceMid','fCombatSearchCheckDestinationDistanceMin','fCombatSearchCheckDestinationTime',
    'fCombatSearchDoorDistance','fCombatSearchDoorDistanceLow','fCombatSearchDoorSearchRadius',
    'fCombatSearchExteriorRadiusMax','fCombatSearchExteriorRadiusMin','fCombatSearchIgnoreLocationRadius',
    'fCombatSearchInteriorRadiusMax','fCombatSearchInteriorRadiusMin','fCombatSearchInvestigateTime',
    'fCombatSearchLocationCheckDistance','fCombatSearchLocationCheckTime','fCombatSearchLocationInitialCheckTime',
    'fCombatSearchLocationInvestigateDistance','fCombatSearchLocationRadius','fCombatSearchLookTime',
    'fCombatSearchRadiusBufferDistance','fCombatSearchRadiusMemberDistance','fCombatSearchSightRadius',
    'fCombatSearchStartWaitTime','fCombatSearchUpdateTime','fCombatSearchWanderDistance',
    'fCombatSelectTargetSwitchUpdateTime','fCombatSelectTargetUpdateTime','fCombatShoutHeadTrackingAngleMovingMult',
    'fCombatShoutHeadTrackingAngleMult','fCombatShoutLongRecoveryTime','fCombatShoutMaxHeadTrackingAngle',
    'fCombatShoutReleaseTime','fCombatShoutShortRecoveryTime','fCombatSneakBowMult',
    'fCombatSneakCrossbowMult','fCombatSneakStaffMult','fCombatSpeakPowerAttackChance',
    'fCombatSpeakTauntChance','fCombatSpecialAttackChanceMax','fCombatSpecialAttackChanceMin',
    'fCombatSplashDamageMaxSpeed','fCombatSplashDamageMinDamage','fCombatSplashDamageMinRadius',
    'fCombatStaffTimer','fCombatStealthPointAttackedMaxValue','fCombatStealthPointDetectedEventMaxValue',
    'fCombatStealthPointMax','fCombatStealthPointRegenAlertWaitTime','fCombatStealthPointRegenLostWaitTime',
    'fCombatStepAdvanceDistance','fCombatStrafeChanceMax','fCombatStrafeChanceMin',
    'fCombatStrafeDistanceMax','fCombatStrafeDistanceMin','fCombatStrafeMinDistanceRadiusMult',
    'fCombatStrengthUpdateTime','fCombatSurroundDistanceMax','fCombatSurroundDistanceMin',
    'fCombatTargetEngagedLastSeenTime','fCombatTargetLocationAvoidNodeRadiusOffset','fCombatTargetLocationCurrentReservationDistanceMult',
    'fCombatTargetLocationMaxDistance','fCombatTargetLocationMinDistanceMult','fCombatTargetLocationPathingRadius',
    'fCombatTargetLocationRadiusSizeMult','fCombatTargetLocationRepositionAngleMult','fCombatTargetLocationSwimmingOffset',
    'fCombatTargetLocationWidthMax','fCombatTargetLocationWidthMin','fCombatTargetLocationWidthSizeMult',
    'fCombatTeammateFollowRadiusBase','fCombatTeammateFollowRadiusMin','fCombatThreatAnticipateTime',
    'fCombatThreatAvoidCost','fCombatThreatBufferRadius','fCombatThreatCacheVelocityTime',
    'fCombatThreatDangerousObjectHealth','fCombatThreatExplosiveObjectThreatTime','fCombatThreatExtrudeTime',
    'fCombatThreatExtrudeVelocityThreshold','fCombatThreatNegativeExtrudeTime','fCombatThreatSignificantScore',
    'fCombatThreatTimedExplosionLength','fCombatThreatUpdateTimeMax','fCombatThreatUpdateTimeMin',
    'fCombatThreatViewCone','fCombatUnreachableTargetCheckTime','fCombatVulnerabilityMod',
    'fCombatYieldRetryTime','fCombatYieldTime','fConeProjectileForceBase',
    'fConeProjectileForceMult','fConeProjectileForceMultAngular','fConeProjectileForceMultLinear',
    'fConeProjectileWaterScaleMult','fConfidenceCowardly','fConfidenceFoolhardy',
    'fConstructibleSkillUseConst','fConstructibleSkilluseExp','fConstructibleSkillUseMult',
    'fCoveredAdvanceMinAdvanceDistanceMax','fCoveredAdvanceMinAdvanceDistanceMin','fCoverEvaluationLastSeenExpireTime',
    'fCoverFiredProjectileExpireTime','fCoverFiringReloadClipPercent','fCoverWaitReloadClipPercent',
    'fCreatureDefaultTurningSpeed','fCrimeAlarmRespMult','fCrimeDispAttack',
    'fCrimeDispMurder','fCrimeDispPersonal','fCrimeDispPickpocket',
    'fCrimeDispSteal','fCrimeDispTresspass','fCrimeFavorMult',
    'fCrimeGoldSkillPenaltyMult','fCrimeGoldSteal','fCrimePersonalRegardMult',
    'fCrimeRegardMult','fCrimeSoundBase','fCrimeSoundMult',
    'fCrimeWitnessRegardMult','fDamageArmConditionBase','fDamageArmConditionMult',
    'fDamagePCSkillMin','fDamageSkillMax','fDamageSkillMin',
    'fDamageStrengthBase','fDamageStrengthMult','fDamageUnarmedPenalty',
    'fDamageWeaponMult','fDangerousObjectExplosionDamage','fDangerousObjectExplosionRadius',
    'fDangerousProjectileExplosionDamage','fDangerousProjectileExplosionRadius','fDaytimeColorExtension',
    'fDeadReactionDistance','fDeathForceMassBase','fDeathForceMassMult',
    'fDeathSoundMaxDistance','fDebrisFadeTime','fDebrisMaxVelocity',
    'fDebrisMinExtent','fDecapitateBloodTime','fDefaultAngleTolerance',
    'fDefaultHealth','fDefaultMagicka','fDefaultMass',
    'fDefaultRelaunchInterval','fDefaultStamina','fDemandBase',
    'fDemandMult','fDetectEventDistanceNPC','fDetectEventDistancePlayer',
    'fDetectEventDistanceVeryLoudMult','fDetectEventSneakDistanceVeryLoud','fDetectionActionTimer',
    'fDetectionCombatNonTargetDistanceMult','fDetectionCommentTimer','fDetectionEventExpireTime',
    'fDetectionLargeActorSizeMult','fDetectionLOSDistanceAngle','fDetectionLOSDistanceMultExterior',
    'fDetectionLOSDistanceMultInterior','fDetectionNightEyeBonus','fDetectionStateExpireTime',
    'fDetectionUpdateTimeMax','fDetectionUpdateTimeMaxComplex','fDetectionUpdateTimeMin',
    'fDetectionUpdateTimeMinComplex','fDetectionViewCone','fDetectProjectileDistanceNPC',
    'fDetectProjectileDistancePlayer','fDialogFocalDepthRange','fDialogFocalDepthStrength',
    'fDialogZoomInSeconds','fDialogZoomOutSeconds','fDifficultyDamageMultiplier',
    'fDifficultyDefaultValue','fDifficultyMaxValue','fDifficultyMinValue',
    'fDiffMultHPByPCE','fDiffMultHPByPCH','fDiffMultHPByPCN',
    'fDiffMultHPByPCVE','fDiffMultHPByPCVH','fDiffMultHPToPCE',
    'fDiffMultHPToPCH','fDiffMultHPToPCN','fDiffMultHPToPCVE',
    'fDiffMultHPToPCVH','fDiffMultXPE','fDiffMultXPN',
    'fDiffMultXPVE','fDisarmedPickupWeaponDistanceMult','fDistanceAutomaticallyActivateDoor',
    'fDistanceExteriorReactCombat','fDistanceFadeActorAutoLoadDoor','fDistanceInteriorReactCombat',
    'fDistanceProjectileExplosionDetection','fDistancetoPlayerforConversations','fDOFDistanceMult',
    'fDragonLandingZoneClearRadius','fDrinkRepeatRate','fDyingTimer',
    'fEffectShaderFillEdgeMinFadeRate','fEffectShaderParticleMinFadeRate','fEmbeddedWeaponSwitchChance',
    'fEmbeddedWeaponSwitchTime','fEnchantingCostExponent','fEnchantingSkillCostBase',
    'fEnchantingSkillCostMult','fEnchantingSkillCostScale','fEnchantmentGoldMult',
    'fEnemyHealthBarTimer','fEssentialDownCombatHealthRegenMult','fEssentialHealthPercentReGain',
    'fEssentialNonCombatHealRateBonus','fEssentialNPCMinimumHealth','fEvaluatePackageTimer',
    'fEvaluateProcedureTimer','fExplosionForceClutterUpBias','fExplosionForceKnockdownMinimum',
    'fExplosionKnockStateExplodeDownTime','fExplosionLOSBuffer','fExplosionLOSBufferDistance',
    'fExplosionMaxImpulse','fExplosiveProjectileBlockedResetTime','fExplosiveProjectileBlockedWaitTime',
    'fExpressionChangePerSec','fExpressionStrengthAdd','fEyePitchMaxOffsetEmotionSad',
    'fEyePitchMinOffsetEmotionAngry','fEyePitchMinOffsetEmotionHappy','fFallLegDamageMult',
    'fFastTravelSpeedMult','fFavorCostActivator','fFavorCostAttack',
    'fFavorCostAttackCrimeMult','fFavorCostLoadDoor','fFavorCostNonLoadDoor',
    'fFavorCostOwnedDoorMult','fFavorCostStealContainerCrime','fFavorCostStealContainerMult',
    'fFavorCostStealObjectMult','fFavorCostTakeObject','fFavorCostUnlockContainer',
    'fFavorCostUnlockDoor','fFavorEventStopDistance','fFavorEventTriggerDistance',
    'fFleeDoneDistanceExterior','fFleeDoneDistanceInterior','fFleeIsSafeTimer',
    'fFloatQuestMarkerFloatHeight','fFloatQuestMarkerMaxDistance','fFloatQuestMarkerMinDistance',
    'fFollowerSpacingAtDoors','fFollowExtraCatchUpSpeedMult','fFollowMatchSpeedZoneWidth',
    'fFollowRunMaxSpeedupMultiplier','fFollowSlowdownZoneWidth','fFollowStartSprintDistance',
    'fFollowStopZoneMinMult','fFollowWalkMaxSpeedupMultiplier','fFollowWalkMinSlowdownMultiplier',
    'fFollowWalkZoneMult','fFriendHitTimer','fFriendMinimumLastHitTime',
    'fFurnitureScaleAnimDurationNPC','fFurnitureScaleAnimDurationPlayer','fGameplayImpulseMinMass',
    'fGameplayImpulseMultTrap','fGameplayImpulseScale','fGameplaySpeakingEmotionMaxChangeValue',
    'fGetHitPainMult','fGrabMaxWeightRunning','fGrabMaxWeightWalking',
    'fGrenadeAgeMax','fGrenadeHighArcSpeedPercentage','fGrenadeThrowHitFractionThreshold',
    'fGunDecalCameraDistance','fGunReferenceSkill','fGunShellCameraDistance',
    'fGunShellLifetime','fGunShellRotateRandomize','fGunShellRotateSpeed',
    'fGunSpreadCrouchBase','fGunSpreadDriftBase','fGunSpreadHeadBase',
    'fGunSpreadHeadMult','fGunSpreadIronSightsBase','fGunSpreadIronSightsMult',
    'fGunSpreadNPCArmBase','fGunSpreadNPCArmMult','fGunSpreadRunBase',
    'fGunSpreadWalkBase','fGunSpreadWalkMult','fHandDamageSkillBase',
    'fHandDamageSkillMult','fHandDamageStrengthBase','fHandDamageStrengthMult',
    'fHandHealthMax','fHandHealthMin','fHandReachDefault',
    'fHazardDefaultTargetInterval','fHazardDropMaxDistance','fHazardMaxWaitTime',
    'fHazardMinimumSpawnInterval','fHazardSpacingMult','fHeadingMarkerAngleTolerance',
    'fHealthDataValue2','fHealthDataValue3','fHealthDataValue4',
    'fHealthDataValue5','fHealthDataValue6','fHealthRegenDelayMax',
    'fHorseMountOffsetX','fHorseMountOffsetY','fHostileActorExteriorDistance',
    'fHostileActorInteriorDistance','fHostileFlyingActorExteriorDistance','fIdleMarkerAngleTolerance',
    'fImpactShaderMaxMagnitude','fImpactShaderMinMagnitude','fIntimidateConfidenceMultAverage',
    'fIntimidateConfidenceMultBrave','fIntimidateConfidenceMultCautious','fIntimidateConfidenceMultCowardly',
    'fIntimidateConfidenceMultFoolhardy','fIntimidateSpeechcraftCurve','fInvisibilityMinRefraction',
    'fIronSightsDOFSwitchSeconds','fIronSightsFOVTimeChange','fItemPointsMult',
    'fItemRepairCostMult','fJumpDoubleMult','fJumpFallRiderMult',
    'fJumpFallSkillBase','fJumpFallSkillMult','fJumpMoveBase',
    'fJumpMoveMult','fKarmaModMurderingNonEvilCreature','fKarmaModMurderingNonEvilNPC',
    'fKillCamLevelBias','fKillCamLevelFactor','fKillCamLevelMaxBias',
    'fKillMoveMaxDuration','fKillWitnessesTimerSetting','fKnockbackAgilBase',
    'fKnockbackAgilMult','fKnockbackDamageBase','fKnockdownAgilBase',
    'fKnockdownAgilMult','fKnockdownBaseHealthThreshold','fKnockdownChance',
    'fKnockdownDamageBase','fKnockdownDamageMult','fLargeProjectilePickBufferSize',
    'fLargeProjectileSize','fLargeRefMinSize','fLeveledLockMult',
    'fLightRecalcTimer','fLightRecalcTimerPlayer','fLoadingWheelScale',
    'fLockLevelBase','fLockLevelMult','fLockpickBreakAdept',
    'fLockpickBreakApprentice','fLockPickBreakBase','fLockpickBreakExpert',
    'fLockpickBreakMaster','fLockPickBreakMult','fLockpickBreakNovice',
    'fLockpickBreakSkillBase','fLockpickBreakSkillMult','fLockPickQualityBase',
    'fLockPickQualityMult','fLockpickSkillSweetSpotBase','fLockSkillBase',
    'fLockSkillMult','fLockTrapGoOffBase','fLockTrapGoOffMult',
    'fLookDownDisableBlinkingAmt','fLowHealthTutorialPercentage','fLowLevelNPCBaseHealthMult',
    'fLowMagickaTutorialPercentage','fLowStaminaTutorialPercentage','fMagicAbsorbDistanceReachMult',
    'fMagicAccumulatingModifierEffectHoldDuration','fMagicAreaScale','fMagicBallMaximumDistance',
    'fMagicBallOptimalDistance','fMagicBarrierDepth','fMagicBarrierHeight',
    'fMagicBarrierSpacing','fMagicBoltDuration','fMagicBoltMaximumDistance',
    'fMagicBoltOptimalDistance','fMagicBoltSegmentLength','fMagicCasterPCSkillCostMult',
    'fMagicCasterSkillCostBase','fMagicCasterTargetUpdateInterval','fMagicCEEnchantMagOffset',
    'fMagicChainExplosionEffectivenessDelta','fMagicCloudAreaMin','fMagicCloudDurationMin',
    'fMagicCloudFindTargetTime','fMagicCloudLifeScale','fMagicCloudSizeScale',
    'fMagicCloudSlowdownRate','fMagicCloudSpeedBase','fMagicCloudSpeedScale',
    'fMagicCostScale','fMagicDefaultAccumulatingModifierEffectRate','fMagicDefaultCEBarterFactor',
    'fMagicDefaultTouchDistance','fMagicDiseaseTransferBase','fMagicDiseaseTransferMult',
    'fMagicDispelMagnitudeMult','fMagicDualCastingCostBase','fMagicDualCastingEffectivenessMult',
    'fMagicDualCastingTimeBase','fMagicDurMagBaseCostMult','fMagicEnchantmentChargeBase',
    'fMagicEnchantmentChargeMult','fMagicEnchantmentDrainBase','fMagicEnchantmentDrainMult',
    'fMagicExplosionAgilityMult','fMagicExplosionClutterMult','fMagicExplosionIncorporealMult',
    'fMagicExplosionIncorporealTime','fMagicExplosionPowerBase','fMagicExplosionPowerMax',
    'fMagicExplosionPowerMin','fMagicExplosionPowerMult','fMagicFogMaximumDistance',
    'fMagicFogOptimalDistance','fMagicGrabActorDrawSpeed','fMagicGrabActorMinDistance',
    'fMagicGrabActorRange','fMagicGrabActorThrowForce','fMagickaRegenDelayMax',
    'fMagickaReturnBase','fMagickaReturnMult','fMagicLesserPowerCooldownTimer',
    'fMagicLightRadiusBase','fMagicNightEyeAmbient','fMagicPlayerMinimumInvisibility',
    'fMagicPostDrawCastDelay','fMagicProjectileMaxDistance','fMagicResistActorSkillBase',
    'fMagicResistActorSkillMult','fMagicResistTargetWillpowerBase','fMagicResistTargetWillpowerMult',
    'fMagicSprayMaximumDistance','fMagicSprayOptimalDistance','fMagicSummonMaxAppearTime',
    'fMagicTelekinesisComplexMaxForce','fMagicTelekinesisComplexObjectDamping','fMagicTelekinesisComplexSpringDamping',
    'fMagicTelekinesisComplexSpringElasticity','fMagicTelekinesisDamageBase','fMagicTelekinesisDamageMult',
    'fMagicTelekinesisDualCastDamageMult','fMagicTelekinesisDualCastThrowMult','fMagicTelekinesisLiftPowerMult',
    'fMagicTelekinesisMaxForce','fMagicTelekinesisMoveAccelerate','fMagicTelekinesisMoveBase',
    'fMagicTelekinesisMoveMax','fMagicTelekinesisObjectDamping','fMagicTelekinesisSpringDamping',
    'fMagicTelekinesisSpringElasticity','fMagicTelekinesisThrow','fMagicTelekinesisThrowAccelerate',
    'fMagicTelekinesisThrowMax','fMagicTrackingLimit','fMagicTrackingLimitComplex',
    'fMagicTrackingMultBall','fMagicTrackingMultBolt','fMagicTrackingMultFog',
    'fMagicUnitsPerFoot','fMagicVACNoPartTargetedMult','fMagicVACPartTargetedMult',
    'fMapMarkerMaxPercentSize','fMapMarkerMinFadeAlpha','fMapMarkerMinPercentSize',
    'fMapQuestMarkerMaxPercentSize','fMapQuestMarkerMinFadeAlpha','fMapQuestMarkerMinPercentSize',
    'fMasserAngleShadowEarlyFade','fMasserSpeed','fMasserZOffset',
    'fMaximumWind','fMaxSandboxRescanSeconds','fMaxSellMult',
    'fMeleeMovementRestrictionsUpdateTime','fMeleeSweepViewAngleMult','fMinDistanceUseHorse',
    'fMineAgeMax','fMinesBlinkFast','fMinesBlinkMax',
    'fMinesBlinkSlow','fMinSandboxRescanSeconds','fModelReferenceEffectMaxWaitTime',
    'fmodifiedTargetAttackRange','fMotionBlur','fMountedMaxLookingDown',
    'fMoveCharRunBase','fMovementNearTargetAvoidCost','fMovementNearTargetAvoidRadius',
    'fMovementTargetAvoidCost','fMovementTargetAvoidRadius','fMovementTargetAvoidRadiusMult',
    'fMoveSprintMult','fMoveSwimMult','fMoveWeightMin',
    'fNPCAttributeHealthMult','fNPCBaseMagickaMult','fNPCGeneticVariation',
    'fObjectHitH2HReach','fObjectHitTwoHandReach','fObjectHitWeaponReach',
    'fObjectMotionBlur','fObjectWeightPickupDetectionMult','fOutOfBreathStaminaRegenDelay',
    'fPainDelay','fPCBaseHealthMult','fPCBaseMagickaMult',
    'fPerceptionMult','fPerkHeavyArmorExpertSpeedMult','fPerkHeavyArmorJourneymanDamageMult',
    'fPerkHeavyArmorMasterSpeedMult','fPerkHeavyArmorNoviceDamageMult','fPerkHeavyArmorSinkGravityMult',
    'fPerkLightArmorExpertSpeedMult','fPerkLightArmorJourneymanDamageMult','fPerkLightArmorMasterRatingMult',
    'fPerkLightArmorNoviceDamageMult','fPersAdmireAggr','fPersAdmireConf',
    'fPersAdmireEner','fPersAdmireIntel','fPersAdmirePers',
    'fPersAdmireResp','fPersAdmireStre','fPersAdmireWillp',
    'fPersBoastAggr','fPersBoastConf','fPersBoastEner',
    'fPersBoastIntel','fPersBoastPers','fPersBoastResp',
    'fPersBoastStre','fPersBoastWillp','fPersBullyAggr',
    'fPersBullyConf','fPersBullyEner','fPersBullyIntel',
    'fPersBullyPers','fPersBullyResp','fPersBullyStre',
    'fPersBullyWillp','fPersJokeAggr','fPersJokeConf',
    'fPersJokeEner','fPersJokeIntel','fPersJokePers',
    'fPersJokeResp','fPersJokeStre','fPersJokeWillp',
    'fPersuasionAccuracyMaxDisposition','fPersuasionAccuracyMaxSelect','fPersuasionAccuracyMinDispostion',
    'fPersuasionAccuracyMinSelect','fPersuasionBaseValueMaxDisposition','fPersuasionBaseValueMaxSelect',
    'fPersuasionBaseValueMinDispostion','fPersuasionBaseValueMinSelect','fPersuasionBaseValueShape',
    'fPersuasionMaxDisposition','fPersuasionMaxInput','fPersuasionMaxSelect',
    'fPersuasionMinDispostion','fPersuasionMinInput','fPersuasionMinPercentCircle',
    'fPersuasionMinSelect','fPersuasionShape','fPhysicsDamage1Damage',
    'fPhysicsDamage2Damage','fPhysicsDamage2Mass','fPhysicsDamage3Damage',
    'fPhysicsDamage3Mass','fPhysicsDamageSpeedBase','fPhysicsDamageSpeedMin',
    'fPhysicsDamageSpeedMult','fPickLevelBase','fPickLevelMult',
    'fPickNumBase','fPickNumMult','fPickPocketAmountBase',
    'fPickPocketDetected','fPickPocketWeightBase','fPickSpring1',
    'fPickSpring2','fPickSpring3','fPickSpring4',
    'fPickSpring5','fPickupItemDistanceFudge','fPickUpWeaponDelay',
    'fPickupWeaponDistanceMinMaxDPSMult','fPickupWeaponMeleeDistanceMax','fPickupWeaponMeleeDistanceMin',
    'fPickupWeaponMeleeWeaponDPSMult','fPickupWeaponMinDPSImprovementPercent','fPickupWeaponRangedDistanceMax',
    'fPickupWeaponRangedDistanceMin','fPickupWeaponRangedMeleeDPSRatioThreshold','fPickupWeaponTargetUnreachableDistanceMult',
    'fPickupWeaponUnarmedDistanceMax','fPickupWeaponUnarmedDistanceMin','fPlayerDropDistance',
    'fPlayerHealthHeartbeatFast','fPlayerHealthHeartbeatSlow','fPlayerMaxResistance',
    'fPlayerTargetCombatDistance','fPlayerTeleportFadeSeconds','fPotionGoldValueMult',
    'fPotionMortPestleMult','fPotionMortPestleMult','fPotionT1AleDurMult',
    'fPotionT1AleMagMult','fPotionT1CalDurMult','fPotionT1CalMagMult',
    'fPotionT1MagMult','fPotionT1RetDurMult','fPotionT1RetMagMult',
    'fPotionT2AleDurMult','fPotionT2CalDurMult','fPotionT2RetDurMult',
    'fPotionT3AleMagMult','fPotionT3CalMagMult','fPotionT3RetMagMult',
    'fPrecipWindMult','fProjectileCollisionImpulseScale','fProjectileDefaultTracerRange',
    'fProjectileDeflectionTime','fProjectileKnockMinMass','fProjectileKnockMultClutter',
    'fProjectileKnockMultProp','fProjectileKnockMultTrap','fProjectileMaxDistance',
    'fProjectileReorientTracerMin','fQuestCinematicCharacterFadeIn','fQuestCinematicCharacterFadeInDelay',
    'fQuestCinematicCharacterFadeOut','fQuestCinematicCharacterRemain','fQuestCinematicObjectiveFadeIn',
    'fQuestCinematicObjectiveFadeInDelay','fQuestCinematicObjectiveFadeOut','fQuestCinematicObjectivePauseTime',
    'fQuestCinematicObjectiveScrollTime','fRandomDoorDistance','fRechargeGoldMult',
    'fReEquipArmorTime','fReflectedAbsorbChanceReduction','fRefTranslationAlmostDonePercent',
    'fRegionGenNoiseFactor','fRegionGenTexGenMatch','fRegionGenTexGenNotMatch',
    'fRegionGenTexPlacedMatch','fRegionGenTexPlacedNotMatch','fRegionGenTreeSinkPower',
    'fRegionObjectDensityPower','fRelationshipBase','fRelationshipMult',
    'fRemoteCombatMissedAttack','fRemoveExcessComplexDeadTime','fRepairMax',
    'fRepairMin','fRepairScavengeMult','fRepairSkillBase',
    'fReservationExpirationSeconds','fResistArrestTimer','fRockitDamageBonusWeightMin',
    'fRockitDamageBonusWeightMult','fRoomLightingTransitionDuration','fRumbleBlockStrength',
    'fRumbleBlockTime','fRumbleHitBlockedStrength','fRumbleHitBlockedTime',
    'fRumbleHitStrength','fRumbleHitTime','fRumblePainStrength',
    'fRumblePainTime','fRumbleShakeRadiusMult','fRumbleShakeTimeMult',
    'fRumbleStruckStrength','fRumbleStruckTime','fSandboxBreakfastMax',
    'fSandboxBreakfastMin','fSandboxCylinderTop','fSandBoxDelayEvalSeconds',
    'fSandboxDurationMultSitting','fSandboxDurationMultSleeping','fSandboxDurationMultWandering',
    'fSandboxDurationRangeMult','fSandboxEnergyMult','fSandboxEnergyMultEatSitting',
    'fSandboxEnergyMultEatStanding','fSandboxEnergyMultFurniture','fSandboxEnergyMultSitting',
    'fSandboxEnergyMultSleeping','fSandboxEnergyMultWandering','fSandBoxExtraDialogueRange',
    'fSandBoxInterMarkerMinDist','fSandBoxSearchRadius','fSandboxSleepDurationMax',
    'fSandboxSleepDurationMin','fSandboxSleepStartMax','fSandboxSleepStartMin',
    'fSayOncePerDayInfoTimer','fScrollCostMult','fSecondsBetweenWindowUpdate',
    'fSecundaAngleShadowEarlyFade','fSecundaSpeed','fSecundaZOffset',
    'fSeenDataUpdateRadius','fShieldBashPCMin','fShieldBashSkillUseBase',
    'fShieldBashSkillUseMult','fShockBoltGrowWidth','fShockBoltsLength',
    'fShockBoltSmallWidth','fShockBoltsRadius','fShockBoltsRadiusStrength',
    'fShockBranchBoltsRadius','fShockBranchBoltsRadiusStrength','fShockBranchLifetime',
    'fShockBranchSegmentLength','fShockBranchSegmentVariance','fShockCastVOffset',
    'fShockCoreColorB','fShockCoreColorG','fShockCoreColorR',
    'fShockGlowColorB','fShockGlowColorG','fShockGlowColorR',
    'fShockSegmentLength','fShockSegmentVariance','fShockSubSegmentVariance',
    'fShoutTimeout','fSittingMaxLookingDown','fSkillUsageSneakHidden',
    'fSkyCellRefProcessDistanceMult','fSmallBumpSpeed','fSmithingArmorMax',
    'fSmithingConditionFactor','fSmithingWeaponMax','fSneakAmbushTargetMod',
    'fSneakAttackSkillUsageRanged','fSneakCombatMod','fSneakDetectionSizeLarge',
    'fSneakDetectionSizeNormal','fSneakDetectionSizeSmall','fSneakDetectionSizeVeryLarge',
    'fSneakDistanceAttenuationExponent','fSneakEquippedWeightBase','fSneakEquippedWeightMult',
    'fSneakLightMoveMult','fSneakLightRunMult','fSneakNoticeMin',
    'fSneakSizeBase','fSneakStealthBoyMult','fSortActorDistanceListTimer',
    'fSpecialLootMaxPCLevelBase','fSpecialLootMaxPCLevelMult','fSpecialLootMaxZoneLevelBase',
    'fSpecialLootMinPCLevelBase','fSpecialLootMinZoneLevelBase','fSpeechCraftBase',
    'fSpeechcraftFavorMax','fSpeechcraftFavorMin','fSpeechCraftMult',
    'fSpellCastingDetectionHitActorMod','fSpellCastingDetectionMod','fSpellmakingGoldMult',
    'fSplashScale1','fSplashScale2','fSplashScale3',
    'fSplashSoundLight','fSplashSoundOutMult','fSplashSoundTimer',
    'fSplashSoundVelocityMult','fSprayDecalsDistance','fSprayDecalsGravity',
    'fSprintEncumbranceMult','fStagger1WeapAR','fStagger1WeapMult',
    'fStagger2WeapAR','fStagger2WeapMult','fStaggerAttackBase',
    'fStaggerAttackMult','fStaggerBlockAttackBase','fStaggerBowAR',
    'fStaggerDaggerAR','fStaggerMassBase','fStaggerMassMult',
    'fStaggerMassOffsetMult','fStaggerMaxDuration','fStaggerRecoilingMult',
    'fStaggerRunningMult','fStaggerShieldMult','fStaminaBlockStaggerMult',
    'fStaminaRegenDelayMax','fStatsCameraNearDistance','fStatsHealthLevelMult',
    'fStatsHealthStartMult','fStatsLineScale','fStatsRotationRampTime',
    'fStatsRotationSpeedMax','fStatsSkillsLookAtX','fStatsSkillsLookAtY',
    'fStatsSkillsLookAtZ','fStatsStarCameraOffsetX','fStatsStarCameraOffsetY',
    'fStatsStarCameraOffsetZ','fStatsStarLookAtX','fStatsStarLookAtY',
    'fStatsStarLookAtZ','fStatsStarScale','fStatsStarZInitialOffset',
    'fSubmergedAngularDamping','fSubmergedLinearDampingH','fSubmergedLinearDampingV',
    'fSubmergedLODDistance','fSubmergedMaxSpeed','fSubmergedMaxWaterDistance',
    'fSubSegmentVariance','fSummonDistanceCheckThreshold','fSummonedCreatureSearchRadius',
    'fSunReduceGlareSpeed','fTakeBackTimerSetting','fTargetMovedCoveredMoveRepathLength',
    'fTargetMovedRepathLength','fTargetMovedRepathLengthLow','fTargetSearchRadius',
    'fTeammateAggroOnDistancefromPlayer','fTemperingSkillUseItemValConst','fTemperingSkillUseItemValExp',
    'fTemperingSkillUseItemValMult','fTimerForPlayerFurnitureEnter','fTimeSpanAfternoonEnd',
    'fTimeSpanAfternoonStart','fTimeSpanEveningEnd','fTimeSpanEveningStart',
    'fTimeSpanMidnightEnd','fTimeSpanMidnightStart','fTimeSpanMorningEnd',
    'fTimeSpanMorningStart','fTimeSpanNightEnd','fTimeSpanNightStart',
    'fTimeSpanSunriseEnd','fTimeSpanSunriseStart','fTimeSpanSunsetEnd',
    'fTimeSpanSunsetStart','fTorchEvaluationTimer','fTorchLightLevelInterior',
    'fTorchLightLevelMorning','fTorchLightLevelNight','fTrackEyeXY',
    'fTrackEyeZ','fTrackFudgeXY','fTrackFudgeZ',
    'fTrackJustAcquiredDuration','fTrackMaxZ','fTrackMinZ',
    'fTrackXY','fTrainingBaseCost','fTreeTrunkToFoliageMultiplier',
    'fTriggerAvoidPlayerDistance','fUnarmedCreatureDPSMult','fUnarmedDamageMult',
    'fUnarmedNPCDPSMult','fUnderwaterFullDepth','fValueofItemForNoOwnership',
    'fVATSAutomaticMeleeDamageMult','fVATSCameraMinTime','fVATSCamTransRBDownStart',
    'fVATSCamTransRBRampDown','fVATSCamTransRBRampup','fVATSCamZoomInTime',
    'fVATSCriticalChanceBonus','fVATSDestructibleMult','fVATSDOFSwitchSeconds',
    'fVATSGrenadeChanceMult','fVATSGrenadeDistAimZMult','fVATSGrenadeRangeMin',
    'fVATSGrenadeRangeMult','fVATSGrenadeSkillFactor','fVATSGrenadeSuccessExplodeTimer',
    'fVATSGrenadeSuccessMaxDistance','fVATSGrenadeTargetMelee','fVATSHitChanceMult',
    'fVATSImageSpaceTransitionTime','fVATSLimbSelectCamPanTime','fVATSMaxChance',
    'fVATSMaxEngageDistance','fVATSMeleeArmConditionBase','fVATSMeleeArmConditionMult',
    'fVATSMeleeChanceMult','fVATSMeleeMaxDistance','fVATSMeleeReachMult',
    'fVATSMoveCameraMaxSpeed','fVATSMoveCameraYPercent','fVATSSafetyMaxTime',
    'fVATSSafetyMaxTimeRanged','fVATSShotBurstTime','fVatsShotgunSpreadRatio',
    'fVATSSkillFactor','fVATSSmartCameraCheckStepCount','fVATSStealthMult',
    'fVATSTargetActorHeightPanMult','fVATSTargetFOVMinDist','fVATSTargetFOVMinFOV',
    'fVATSTargetScanRotateMult','fVATSTargetSelectCamPanTime','fVATSTargetTimeUpdateMult',
    'fVATSThrownWeaponRangeMult','fVoiceRateBase','fWardAngleForExplosions',
    'fWeaponBashMin','fWeaponBashPCMax','fWeaponBashPCMin',
    'fWeaponBashSkillUseBase','fWeaponBashSkillUseMult','fWeaponBlockSkillUseBase',
    'fWeaponBlockSkillUseMult','fWeaponBloodAlphaToRGBScale','fWeaponClutterKnockBipedScale',
    'fWeaponClutterKnockMaxWeaponMass','fWeaponClutterKnockMinClutterMass','fWeaponClutterKnockMult',
    'fWeaponConditionCriticalChanceMult','fWeaponConditionJam10','fWeaponConditionJam5',
    'fWeaponConditionJam6','fWeaponConditionJam7','fWeaponConditionJam8',
    'fWeaponConditionJam9','fWeaponConditionRateOfFire10','fWeaponConditionReloadJam1',
    'fWeaponConditionReloadJam10','fWeaponConditionReloadJam2','fWeaponConditionReloadJam3',
    'fWeaponConditionReloadJam4','fWeaponConditionReloadJam5','fWeaponConditionReloadJam6',
    'fWeaponConditionReloadJam7','fWeaponConditionReloadJam8','fWeaponConditionReloadJam9',
    'fWeaponConditionSpread10','fWeaponTwoHandedAnimationSpeedMult','fWeatherCloudSpeedMax',
    'fWeatherFlashAmbient','fWeatherFlashDirectional','fWeatherFlashDuration',
    'fWeatherTransMax','fWeatherTransMin','fWortalchmult',
    'fWortcraftChanceIntDenom','fWortcraftChanceLuckDenom','fWortcraftStrChanceDenom',
    'fWortcraftStrCostDenom','fWortStrMult','fXPPerSkillRank',
    'fZKeyComplexHelperMinDistance','fZKeyComplexHelperScale','fZKeyComplexHelperWeightMax',
    'fZKeyComplexHelperWeightMin','fZKeyHeavyWeight','fZKeyMaxContactDistance',
    'fZKeyMaxContactMassRatio','fZKeyMaxForceScaleHigh','fZKeyMaxForceScaleLow',
    'fZKeyMaxForceWeightLow','fZKeyObjectDamping','fZKeySpringDamping',
    'fZKeySpringElasticity','iActivatePickLength','iActorKeepTurnDegree',
    'iActorLuckSkillBase','iActorTorsoMaxRotation','iAICombatMaxAllySummonCount',
    'iAICombatMinDetection','iAICombatRestoreMagickaPercentage','iAIFleeMaxHitCount',
    'iAIMaxSocialDistanceToTriggerEvent','iAimingNumIterations','iAINPCRacePowerChance',
    'iAINumberActorsComplexScene','iAINumberDaysToStayBribed','iAINumberDaysToStayIntimidated',
    'iAlertAgressionMin','iAllowAlchemyDuringCombat','iAllowRechargeDuringCombat',
    'iAllowRepairDuringCombat','iAllyHitCombatAllowed','iAllyHitNonCombatAllowed',
    'iArmorBaseSkill','iArmorDamageBootsChance','iArmorDamageCuirassChance',
    'iArmorDamageGauntletsChance','iArmorDamageGreavesChance','iArmorDamageHelmChance',
    'iArmorDamageShieldChance','iArmorWeightBoots','iArmorWeightCuirass',
    'iArmorWeightGauntlets','iArmorWeightGreaves','iArmorWeightHelmet',
    'iArmorWeightShield','iArrestOnSightNonViolent','iArrestOnSightViolent',
    'iArrowMaxCount','iAttackOnSightNonViolent','iAttackOnSightViolent',
    'iAttractModeIdleTime','iAVDAutoCalcSkillMax','iAVDSkillStart',
    'iAvoidHurtingNonTargetsResponsibility','iBallisticProjectilePathPickSegments',
    'iBaseDisposition','iBoneLODDistMult','iClassAcrobat',
    'iClassAgent','iClassArcher',
    'iClassAssassin','iClassBarbarian','iClassBard',
    'iClassBattlemage','iClassCharactergenClass','iClassCrusader',
    'iClassHealer','iClassKnight','iClassMage',
    'iClassMonk','iClassNightblade','iClassPilgrim',
    'iClassPriest','iClassRogue','iClassScout',
    'iClassSorcerer','iClassSpellsword','iClassThief',
    'iClassWarrior','iClassWitchhunter','iCombatAimMaxIterations',
    'iCombatCastDrainMinimumValue','iCombatCrippledTorsoHitStaggerChance','iCombatFlankingAngleOffsetCount',
    'iCombatFlankingAngleOffsetGoalCount','iCombatFlankingDirectionOffsetCount','iCombatHighPriorityModifier',
    'iCombatHoverLocationCount','iCombatSearchDoorFailureMax','iCombatStealthPointSneakDetectionThreshold',
    'iCombatTargetLocationCount','iCombatTargetPlayerSoftCap','iCombatUnloadedActorLastSeenTimeLimit',
    'iCrimeAlarmLowRecDistance','iCrimeAlarmRecDistance','iCrimeCommentNumber',
    'iCrimeDaysInPrisonMod','iCrimeEnemyCoolDownTimer','iCrimeFavorBaseValue',
    'iCrimeGoldAttack','iCrimeGoldEscape','iCrimeGoldMinValue',
    'iCrimeGoldMurder','iCrimeGoldPickpocket','iCrimeGoldTrespass',
    'iCrimeMaxNumberofDaysinJail','iCrimeRegardBaseValue','iCrimeValueAttackValue',
    'iCurrentTargetBonus','iDebrisMaxCount','iDetectEventLightLevelExterior',
    'iDetectEventLightLevelInterior','iDialogueDispositionFriendValue','iDismemberBloodDecalCount',
    'iDispKaramMax','iDistancetoAttackedTarget','iFallLegDamageChance',
    'iFloraEmptyAlpha','iFloraFullAlpha','iFriendHitNonCombatAllowed',
    'iGameplayiSpeakingEmotionDeltaChange','iGameplayiSpeakingEmotionListenValue','iHairColor00',
    'iHairColor01','iHairColor02','iHairColor03',
    'iHairColor04','iHairColor05','iHairColor06',
    'iHairColor07','iHairColor08','iHairColor09',
    'iHairColor10','iHairColor11','iHairColor12',
    'iHairColor13','iHairColor14','iHairColor15',
    'iHorseTurnDegreesPerSecond','iHorseTurnDegreesRampUpPerSecond','iHoursToClearCorpses',
    'iInventoryAskQuantityAt','iKarmaMax','iKarmaMin',
    'iKillCamLevelOffset','iLargeProjectilePickCount','iLevCharLevelDifferenceMax',
    'iLevelUpReminder','iLevItemLevelDifferenceMax','iLightLevelExteriorMod',
    'iLockLevelMaxAverage','iLockLevelMaxEasy','iLockLevelMaxHard',
    'iLockLevelMaxImpossible','iLockLevelMaxVeryHard','iLowLevelNPCMaxLevel',
    'iMagicLightMaxCount','iMaxArrowsInQuiver','iMaxAttachedArrows',
    'iMaxCharacterLevel','iMaxSummonedCreatures','iMessageBoxMaxItems',
    'iMinClipSizeToAddReloadDelay','iMoodFaceValue','iNPCBasePerLevelHealthMult',
    'iNumberActorsAllowedToFollowPlayer','iNumberActorsGoThroughLoadDoorInCombat','iNumberActorsInCombatPlayer',
    'iNumberGuardsCrimeResponse','iNumExplosionDecalCDPoint','iPCStartSpellSkillLevel',
    'iPerkBlockStaggerChance','iPerkHandToHandBlockRecoilChance','iPerkHeavyArmorJumpSum',
    'iPerkHeavyArmorSinkSum','iPerkLightArmorMasterMinSum','iPerkMarksmanKnockdownChance',
    'iPerkMarksmanParalyzeChance','iPersuasionAngleMax','iPersuasionAngleMin',
    'iPersuasionBribeCrime','iPersuasionBribeGold','iPersuasionBribeRefuse',
    'iPersuasionBribeScale','iPersuasionDemandDisposition','iPersuasionDemandGold',
    'iPersuasionDemandRefuse','iPersuasionDemandScale','iPersuasionInner',
    'iPersuasionMiddle','iPersuasionOuter','iPersuasionPower1',
    'iPersuasionPower2','iPersuasionPower3','iPickPocketWarnings',
    'iPlayerCustomClass','iPlayerHealthHeartbeatFadeMS','iProjectileMaxRefCount',
    'iQuestReminderPipboyDisabledTime','iRegionGenClusterAttempts','iRegionGenClusterPasses',
    'iRegionGenRandomnessType','iRelationshipAcquaintanceValue','iRelationshipAllyValue',
    'iRelationshipArchnemesisValue','iRelationshipConfidantValue','iRelationshipEnemyValue',
    'iRelationshipFoeValue','iRelationshipFriendValue','iRelationshipLoverValue',
    'iRelationshipRivalValue','iRemoveExcessDeadComplexCount','iRemoveExcessDeadComplexTotalActorCount',
    'iRemoveExcessDeadTotalActorCount','iSecondsToSleepPerUpdate','iShockBranchNumBolts',
    'iShockBranchSegmentsPerBolt','iShockDebug','iShockNumBolts',
    'iShockSegmentsPerBolt','iShockSubSegments','iSkillPointsTagSkillMult',
    'iSkillUsageSneakFullDetection','iSkillUsageSneakMinDetection','iSneakSkillUseDistance',
    'iSoulLevelValueCommon','iSoulLevelValueGrand','iSoulLevelValueGreater',
    'iSoulLevelValueLesser','iSoulLevelValuePetty','iSoundLevelLoud',
    'iSoundLevelNormal','iSoundLevelVeryLoud','iSprayDecalsDebug',
    'iStandardEmotionValue','iStealWarnings','iTrainingExpertSkill',
    'iTrainingJourneymanCost','iTrainingJourneymanSkill','iTrainingMasterSkill',
    'iTrainingNumAllowedPerLevel','iTrespassWarnings','iUpdateESMVersion',
    'iVATSCameraHitDist','iVATSConcentratedFireBonus','iVoicePointsDefault',
    'iWeaponCriticalHitDropChance','iWortcraftMaxEffectsApprentice','iWortcraftMaxEffectsExpert',
    'iWortcraftMaxEffectsJourneyman','iWortcraftMaxEffectsMaster','iWortcraftMaxEffectsNovice',
    'iXPBase','iXPLevelHackComputerAverage','iXPLevelHackComputerEasy',
    'iXPLevelHackComputerHard','iXPLevelHackComputerVeryEasy','iXPLevelHackComputerVeryHard',
    'iXPLevelPickLockAverage','iXPLevelPickLockEasy','iXPLevelPickLockHard',
    'iXPLevelPickLockVeryEasy','iXPLevelPickLockVeryHard','iXPLevelSpeechChallengeAverage',
    'iXPLevelSpeechChallengeEasy','iXPLevelSpeechChallengeHard','iXPLevelSpeechChallengeVeryEasy',
    'iXPLevelSpeechChallengeVeryHard','iXPRewardDiscoverSecretArea','iXPRewardKillNPCAverage',
    'iXPRewardKillNPCEasy','iXPRewardKillNPCHard','iXPRewardKillNPCVeryEasy',
    'iXPRewardKillNPCVeryHard','iXPRewardKillOpponent','iXPRewardSpeechChallengeAverage',
    'iXPRewardSpeechChallengeEasy','iXPRewardSpeechChallengeHard','iXPRewardSpeechChallengeVeryHard',
    'sGenericCraftKeywordName01','sGenericCraftKeywordName02','sGenericCraftKeywordName03',
    'sGenericCraftKeywordName04','sGenericCraftKeywordName05','sGenericCraftKeywordName06',
    'sGenericCraftKeywordName07','sGenericCraftKeywordName08','sGenericCraftKeywordName09',
    'sGenericCraftKeywordName10','sInvalidTagString','sKinectAllyTooFarToTrade',
    'sKinectCantInit','sKinectNotCalibrated','sNoBolts',
    'sRSMFinishedWarning','sVerletCape','uiMuteMusicPauseTime',
    ]

#--Tags supported by this game
allTags = sorted((u'Relev',u'Delev',u'Filter',u'NoMerge',u'Deactivate',u'Names',u'Stats'))

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
GlobalsTweaks = [
    (_(u'Timescale'),_(u'Timescale will be set to:'),
        u'timescale',
        (u'1',         1),
        (u'8',         8),
        (u'10',       10),
        (u'12',       12),
        (u'[20]',     20),
        (u'24',       24),
        (u'30',       30),
        (u'40',       40),
        (_(u'Custom'), 20),
        ),
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
GmstTweaks = [
    (_(u'Msg: Soul Captured!'),_(u'Message upon capturing a sould in a Soul Gem.'),
     u'sSoulCaptured',
     (_(u'[None]'),          u' '),
     (u'.',                  u'.'),
     (_(u'Custom'),       _(u' ')),
     ),
    (_(u'Actor Strength Encumbrance Multiplier'),_(u"Actor's Strength X this = Actor's Encumbrance capacity."),
        (u'fActorStrengthEncumbranceMult',),
        (u'1',                 1.0),
        (u'3',                 3.0),
        (u'[5]',               5.0),
        (u'8',                 8.0),
        (u'10',               10.0),
        (u'20',               20.0),
        (_(u'Unlimited'), 999999.0),
        (_(u'Custom'),         5.0),
        ),
    (_(u'AI: Max Active Actors'),_(u'Maximum actors whose AI can be active. Must be higher than Combat: Max Actors'),
        (u'iAINumberActorsComplexScene',),
        (u'[20]',               20),
        (u'25',                 25),
        (u'30',                 30),
        (u'35',                 35),
        (_(u'MMM Default: 40'), 40),
        (u'50',                 50),
        (u'60',                 60),
        (u'100',               100),
        (_(u'Custom'),          20),
        ),
    (_(u'Arrow: Recovery from Actor'),_(u'Chance that an arrow shot into an actor can be recovered.'),
        (u'iArrowInventoryChance',),
        (u'[33%]',     33),
        (u'50%',       50),
        (u'60%',       60),
        (u'70%',       70),
        (u'80%',       80),
        (u'90%',       90),
        (u'100%',     100),
        (_(u'Custom'), 33),
        ),
    (_(u'Arrow: Speed'),_(u'Speed of a full power arrow.'),
        (u'fArrowSpeedMult',),
        (u'[x 1.0]',                  1500.0),
        (u'x 1.2',                1500.0*1.2),
        (u'x 1.4',                1500.0*1.4),
        (u'x 1.6',                1500.0*1.6),
        (u'x 1.8',                1500.0*1.8),
        (u'x 2.0',                1500.0*2.0),
        (u'x 2.2',                1500.0*2.2),
        (u'x 2.4',                1500.0*2.4),
        (u'x 2.6',                1500.0*2.6),
        (u'x 2.8',                1500.0*2.8),
        (u'x 3.0',                1500.0*3.0),
        (_(u'Custom (base is 1500)'), 1500.0),
        ),
    (_(u'Cell: Respawn Time'),_(u'Time before unvisited cells respawn. Longer times increase save game size.'),
        (u'iHoursToRespawnCell',),
        (_(u'1 Day'),           24*1),
        (_(u'3 Days'),          24*3),
        (_(u'5 Days'),          24*5),
        (_(u'[10 Days]'),      24*10),
        (_(u'20 Days'),        24*20),
        (_(u'1 Month'),        24*30),
        (_(u'6 Months'),      24*182),
        (_(u'1 Year'),        24*365),
        (_(u'Custom (in hours)'), 240),
        ),
    (_(u'Cell: Respawn Time (Cleared)'),_(u'Time before a cleared cell will respawn. Longer times increase save game size.'),
        (u'iHoursToRespawnCellCleared',),
        (_(u'10 Days'),         24*10),
        (_(u'15 Days'),         24*15),
        (_(u'20 Days'),         24*20),
        (_(u'25 Days'),         24*25),
        (_(u'[30 Days]'),       24*30),
        (_(u'2 Months'),        24*60),
        (_(u'6 Months'),      24*180),
        (_(u'1 Year'),        24*365),
        (_(u'Custom (in hours)'), 720),
        ),
    (_(u'Combat: Alchemy'),_(u'Allow alchemy during combat.'),
        (u'iAllowAlchemyDuringCombat',),
        (_(u'Allow'),      1),
        (_(u'[Disallow]'), 0),
        ),
    (_(u'Combat: Max Actors'),_(u'Maximum number of actors that can actively be in combat with the player.'),
        (u'iNumberActorsInCombatPlayer',),
        (u'10',        10),
        (u'15',        15),
        (u'[20]',      20),
        (u'30',        30),
        (u'40',        40),
        (u'50',        50),
        (u'80',        80),
        (_(u'Custom'), 20),
        ),
    (_(u'Combat: Recharge Weapons'),_(u'Allow recharging weapons during combat.'),
        (u'iAllowRechargeDuringCombat',),
        (_(u'[Allow]'),  1),
        (_(u'Disallow'), 0),
        ),
    (_(u'Companions: Max Number'),_(u'Maximum number of actors following the player.'),
        (u'iNumberActorsAllowedToFollowPlayer',),
        (u'2',         2),
        (u'4',         4),
        (u'[6]',       6),
        (u'8',         8),
        (u'10',       10),
        (_(u'Custom'), 6),
        ),
    (_(u'Crime: Alarm Distance'),_(u'Distance from player that NPCs(guards) will be alerted of a crime.'),
        (u'iCrimeAlarmRecDistance',),
        (u'8000',      8000),
        (u'6000',      6000),
        (u'[4000]',    4000),
        (u'3000',      3000),
        (u'2000',      2000),
        (u'1000',      1000),
        (u'500',        500),
        (_(u'Custom'), 4000),
        ),
    (_(u'Crime: Assault Fine'),_(u'Fine in septims for committing an assault.'),
        (u'iCrimeGoldAttack',),
        (u'[40]',     40),
        (u'50',       50),
        (u'60',       60),
        (u'70',       70),
        (u'80',       80),
        (u'90',       90),
        (u'100',     100),
        (_(u'Custom'), 40),
        ),
    (_(u'Crime: Days in Prison'),_(u'Number of days to advance the calendar when serving prison time.'),
        (u'iCrimeDaysInPrisonMod',),
        (u'50',       50),
        (u'60',       60),
        (u'70',       70),
        (u'80',       80),
        (u'90',       90),
        (u'[100]',   100),
        (_(u'Custom'), 100),
        ),
    (_(u'Crime: Escape Jail'),_(u'Amount added to your bounty for escaping from jail.'),
        (u'iCrimeGoldEscape',),
        (u'[100]',   100),
        (u'125',     125),
        (u'150',     150),
        (u'175',     175),
        (u'200',     200),
        (_(u'Custom'), 100),
        ),
    (_(u'Crime: Murder Bounty'),_(u'Bounty for committing a witnessed murder.'),
        (u'iCrimeGoldMurder',),
        (u'500',      500),
        (u'750',      750),
        (u'[1000]',  1000),
        (u'1250',    1250),
        (u'1500',    1500),
        (_(u'Custom'), 1000),
        ),
    (_(u'Crime: Pickpocketing Fine'),_(u'Fine in septims for picpocketing.'),
        (u'iCrimeGoldPickpocket',),
        (u'5',          5),
        (u'8',          8),
        (u'10',        10),
        (u'[25]',      25),
        (u'50',        50),
        (u'100',      100),
        (_(u'Custom'), 25),
        ),
    (_(u'Crime: Trespassing Fine'),_(u'Fine in septims for trespassing.'),
        (u'iCrimeGoldTrespass',),
        (u'1',         1),
        (u'[5]',       5),
        (u'8',         8),
        (u'10',       10),
        (u'20',       20),
        (_(u'Custom'), 5),
        ),
    (_(u'Max Resistence'),_(u'Maximum level of resistence a player can have to various forms of magic and disease.'),
        (u'fPlayerMaxResistance',),
        (u'50%',       50),
        (u'60%',       60),
        (u'70%',       70),
        (u'80%',       80),
        (u'[85%]',     85),
        (u'90%',       90),
        (u'100%',     100),
        (_(u'Custom'), 85),
        ),
    (_(u'Max Summons'),_(u'Maximum number of allowed summoned creatures the player can call forth.'),
        (u'iMaxSummonedCreatures',),
        (u'[1]',              1),
        (u'2',                2),
        (u'3',                3),
        (u'4',                4),
        (u'5',                5),
        (_(u'Custom'),        1),
        ),
    (_(u'Max Training'),_(u'Maximum number of training sessions the player can have per level.'),
        (u'iTrainingNumAllowedPerLevel',),
        (u'3',         3),
        (u'4',         4),
        (u'[5]',       5),
        (u'8',         8),
        (u'10',       10),
        (_(u'Custom'), 5),
        ),
    ]

#--Patchers available when building a Bashed Patch
patchers = (
    u'AliasesPatcher', u'PatchMerger', u'ListsMerger', u'GmstTweaker',
    u'NamesPatcher', u'StatsPatcher'
    )

# For ListsMerger
listTypes = ('LVLI','LVLN','LVSP',)

namesTypes = set(('ACTI', 'AMMO', 'ARMO', 'APPA', 'MISC',))
pricesTypes = {'AMMO':{},'ARMO':{},'APPA':{},'MISC':{}}
statsTypes = {
            'AMMO':('eid', 'value', 'damage'),
            'ARMO':('eid', 'weight', 'value', 'armorRating'),
            'APPA':('eid', 'weight', 'value'),
            'MISC':('eid', 'weight', 'value'),
            }
statsHeaders = (
                #--Ammo
                (u'AMMO',
                    (u'"' + u'","'.join((_(u'Type'),_(u'Mod Name'),_(u'ObjectIndex'),
                    _(u'Editor Id'),_(u'Weight'),_(u'Value'))) + u'"\n')),
                #--Armo
                (u'ARMO',
                    (u'"' + u'","'.join((_(u'Type'),_(u'Mod Name'),_(u'ObjectIndex'),
                    _(u'Editor Id'),_(u'Weight'),_(u'Value'),_('armorRating'))) + u'"\n')),
                (u'APPA',
                    (u'"' + u'","'.join((_(u'Type'),_(u'Mod Name'),_(u'ObjectIndex'),
                    _(u'Editor Id'),_(u'Weight'),_(u'Value'))) + u'"\n')),
                (u'MISC',
                    (u'"' + u'","'.join((_(u'Type'),_(u'Mod Name'),_(u'ObjectIndex'),
                    _(u'Editor Id'),_(u'Weight'),_(u'Value'))) + u'"\n')),
                )

#--CBash patchers available when building a Bashed Patch
CBash_patchers = tuple()

# Mod Record Elements ----------------------------------------------------------
#-------------------------------------------------------------------------------
# Constants
FID = 'FID' #--Used by MelStruct classes to indicate fid elements.

# Race Info -------------------------------------------------------------------
raceNames = {
    0x13740 : _(u'Argonian'),
    0x13741 : _(u'Breton'),
    0x13742 : _(u'Dark Elf'),
    0x13743 : _(u'High Elf'),
    0x13744 : _(u'Imperial'),
    0x13745 : _(u'Khajiit'),
    0x13746 : _(u'Nord'),
    0x13747 : _(u'Orc'),
    0x13748 : _(u'Redguard'),
    0x13749 : _(u'Wood Elf'),
    }

raceShortNames = {
    0x13740 : u'Arg',
    0x13741 : u'Bre',
    0x13742 : u'Dun',
    0x13743 : u'Alt',
    0x13744 : u'Imp',
    0x13745 : u'Kha',
    0x13746 : u'Nor',
    0x13747 : u'Orc',
    0x13748 : u'Red',
    0x13749 : u'Bos',
    }

raceHairMale = {
    0x13740 : 0x64f32, #--Arg
    0x13741 : 0x90475, #--Bre
    0x13742 : 0x64214, #--Dun
    0x13743 : 0x7b792, #--Alt
    0x13744 : 0x90475, #--Imp
    0x13745 : 0x653d4, #--Kha
    0x13746 : 0x1da82, #--Nor
    0x13747 : 0x66a27, #--Orc
    0x13748 : 0x64215, #--Red
    0x13749 : 0x690bc, #--Bos
    }

raceHairFemale = {
    0x13740 : 0x64f33, #--Arg
    0x13741 : 0x1da83, #--Bre
    0x13742 : 0x1da83, #--Dun
    0x13743 : 0x690c2, #--Alt
    0x13744 : 0x1da83, #--Imp
    0x13745 : 0x653d0, #--Kha
    0x13746 : 0x1da83, #--Nor
    0x13747 : 0x64218, #--Orc
    0x13748 : 0x64210, #--Red
    0x13749 : 0x69473, #--Bos
    }

#--Plugin format stuff
class esp:
    #--Wrye Bash capabilities
    canBash = True         # No Bashed Patch creation
    canCBash = False        # CBash cannot handle this game's records
    canEditHeader = True    # Can edit anything in the TES4 record

    #--Valid ESM/ESP header versions
    validHeaderVersions = (0.94, 1.70,)

    #--Strings Files
    stringsFiles = [
        ('mods',(u'Strings',),u'%(body)s_%(language)s.STRINGS'),
        ('mods',(u'Strings',),u'%(body)s_%(language)s.DLSTRINGS'),
        ('mods',(u'Strings',),u'%(body)s_%(language)s.ILSTRINGS'),
        ]

    #--Top types in Skyrim order.
    topTypes = ['GMST', 'KYWD', 'LCRT', 'AACT', 'TXST', 'GLOB', 'CLAS', 'FACT', 'HDPT',
                'HAIR', 'EYES', 'RACE', 'SOUN', 'ASPC', 'MGEF', 'SCPT', 'LTEX', 'ENCH',
                'SPEL', 'SCRL', 'ACTI', 'TACT', 'ARMO', 'BOOK', 'CONT', 'DOOR', 'INGR',
                'LIGH', 'MISC', 'APPA', 'STAT', 'SCOL', 'MSTT', 'PWAT', 'GRAS', 'TREE',
                'CLDC', 'FLOR', 'FURN', 'WEAP', 'AMMO', 'NPC_', 'LVLN', 'KEYM', 'ALCH',
                'IDLM', 'COBJ', 'PROJ', 'HAZD', 'SLGM', 'LVLI', 'WTHR', 'CLMT', 'SPGD',
                'RFCT', 'REGN', 'NAVI', 'CELL', 'WRLD', 'DIAL', 'QUST', 'IDLE', 'PACK',
                'CSTY', 'LSCR', 'LVSP', 'ANIO', 'WATR', 'EFSH', 'EXPL', 'DEBR', 'IMGS',
                'IMAD', 'FLST', 'PERK', 'BPTD', 'ADDN', 'AVIF', 'CAMS', 'CPTH', 'VTYP',
                'MATT', 'IPCT', 'IPDS', 'ARMA', 'ECZN', 'LCTN', 'MESG', 'RGDL', 'DOBJ',
                'LGTM', 'MUSC', 'FSTP', 'FSTS', 'SMBN', 'SMQN', 'SMEN', 'DLBR', 'MUST',
                'DLVW', 'WOOP', 'SHOU', 'EQUP', 'RELA', 'SCEN', 'ASTP', 'OTFT', 'ARTO',
                'MATO', 'MOVT', 'SNDR', 'DUAL', 'SNCT', 'SOPM', 'COLL', 'CLFM', 'REVB',]

    #--Dict mapping 'ignored' top types to un-ignored top types.
    topIgTypes = dict([(struct.pack('I',(struct.unpack('I',type)[0]) | 0x1000),type) for type in topTypes])

    #-> this needs updating for Skyrim
    recordTypes = set(topTypes + 'GRUP,TES4,REFR,ACHR,ACRE,LAND,INFO,NAVM,PHZD,PGRE'.split(','))

#--Mod I/O
class RecordHeader(brec.BaseRecordHeader):
    size = 24

    def __init__(self,recType='TES4',size=0,arg1=0,arg2=0,arg3=0,extra=0):
        self.recType = recType
        self.size = size
        if recType == 'GRUP':
            self.label = arg1
            self.groupType = arg2
            self.stamp = arg3
        else:
            self.flags1 = arg1
            self.fid = arg2
            self.flags2 = arg3
        self.extra = extra

    @staticmethod
    def unpack(ins):
        """Returns a RecordHeader object by reading the niput stream."""
        type,size,uint0,uint1,uint2,uint3 = ins.unpack('=4s5I',24,'REC_HEADER')
        #--Bad type?
        if type not in esp.recordTypes:
            raise brec.ModError(ins.inName,u'Bad header type: '+repr(type))
        #--Record
        if type != 'GRUP':
            pass
        #--Top Group
        elif uint1 == 0: #groupType == 0 (Top Type)
            str0 = struct.pack('I',uint0)
            if str0 in esp.topTypes:
                uint0 = str0
            elif str0 in esp.topIgTypes:
                uint0 = esp.topIgTypes[str0]
            else:
                raise brec.ModError(ins.inName,u'Bad Top GRUP type: '+repr(str0))
        #--Other groups
        return RecordHeader(type,size,uint0,uint1,uint2,uint3)

    def pack(self):
        """Return the record header packed into a bitstream to be written to file."""
        if self.recType == 'GRUP':
            if isinstance(self.label,str):
                return struct.pack('=4sI4sIII',self.recType,self.size,
                                   self.label,self.groupType,self.stamp,
                                   self.extra)
            elif isinstance(self.label,tuple):
                return struct.pack('=4sIhhIII',self.recType,self.size,
                                   self.label[0],self.label[1],self.groupType,
                                   self.stamp,self.extra)
            else:
                return struct.pack('=4s5I',self.recType,self.size,self.label,
                                   self.groupType,self.stamp,self.extra)
        else:
            return struct.pack('=4s5I',self.recType,self.size,self.flags1,
                               self.fid,self.flags2,self.extra)

#------------------------------------------------------------------------------
# Common/Special Elements
#------------------------------------------------------------------------------
# Flags
#------------------------------------------------------------------------------
# class MelBipedFlags(bolt.Flags):
#    """Biped flags element. Includes biped flag set by default."""
#    mask = 0xFFFF
#    def __init__(self,default=0L,newNames=None):
#        names = bolt.Flags.getNames(
#            'head', 'hair', 'upperBody', 'leftHand', 'rightHand', 'weapon',
#            'pipboy', 'backpack', 'necklace', 'headband', 'hat', 'eyeGlasses',
#            'noseRing', 'earrings', 'mask', 'choker', 'mouthObject',
#            'bodyAddOn1', 'bodyAddOn2', 'bodyAddOn3')
#        if newNames: names.update(newNames)
#        bolt.Flags.__init__(self,default,names)
#
# Skyrim does not use this but it will need it
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
class MelDestructible(MelGroup):
    """Represents a set of destruct record."""

    MelDestTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'capDamage'),
        (1, 'disable'),
        (2, 'clothing'),
        (3, 'ignoreExternalDmg'),
        ))

    def __init__(self,attr='destructible'):
        """Initialize elements."""
        MelGroup.__init__(
            self,attr,
            # 'vatsTargetable' is either True or False
            # wbInteger('Health', itS32),  The S means signed
            MelStruct('DEST','iBB2s','health','count','vatsTargetable','dest_unused'),
            # wbRArray('Stages',
                # wbRStruct('Stage', [
                    # wbStruct(DSTD, 'Destruction Stage Data', [
                    # wbRStructSK([0], 'Model', [
            MelGroups('stages',
                MelStruct('DSTD','=4B4I','health','index','damageStage',(MelDestructible.MelDestTypeFlags,'flags',0L),
                          'selfDamagePerSecond',(FID,'explosion',None),(FID,'debris',None),'debrisCount'),
                MelModel('model','DMDL'),
            ),
            MelBase('DSTF','dstf_p'), # Appears just to signal the end of the destruction data
        )

# Updated for skyrim check Syntax
#------------------------------------------------------------------------------
class MelEffects(MelGroups):
    """Represents ingredient/potion/enchantment/spell effects."""

    def __init__(self,attr='effects'):
        """Initialize elements."""
        MelGroups.__init__(self,attr,
            MelFid('EFID','baseEffect'),
            MelStruct('EFIT','f2I','magnitude','area','duration',),
            MelConditions(),
            )

# Updated for Skyrim check Syntax
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

# Probably obsolete.  Included for reference and testing.
#------------------------------------------------------------------------------
class MelOwnership(MelGroup):
    """Handles XOWN, XRNK for cells and cell children."""

#  wbOwnership := wbRStruct('Ownership', [
#    wbFormIDCkNoReach(XOWN, 'Owner', [FACT, ACHR, NPC_]),
#    wbInteger(XRNK, 'Faction rank', itS32)
#  ], []);

    def __init__(self,attr='ownership'):
        """Initialize."""
        MelGroup.__init__(self,attr,
            MelFid('XOWN','owner'),
            MelOptStruct('XRNK','i',('rank',None)),
        )

    def dumpData(self,record,out):
        """Dumps data from record to outstream."""
        if record.ownership and record.ownership.owner:
            MelGroup.dumpData(self,record,out)

# Needs syntax check but otherwise Correct for Skyrim
#------------------------------------------------------------------------------
class MelConditions(MelStructs):
    """Represents a set of quest/dialog/etc conditions. Difficulty is that FID
    state of parameters depends on function index."""
    def __init__(self):
        """Initialize."""
        MelStructs.__init__(self,'CTDA','=B3sfH2sii4sII','conditions',
            'operFlag',('unused1',null3),'compValue',
            'ifunc',('unused2',null2),'param1','param2',
            ('unused3',null4),'reference','unknown')

    def getDefault(self):
        """Returns a default copy of object."""
        target = MelStructs.getDefault(self)
        target.form12 = 'ii'
        return target

    def hasFids(self,formElements):
        """Include self if has fids."""
        formElements.add(self)

    def loadData(self,record,ins,type,size,readId):
        """Reads data from ins into record attribute."""
        target = MelObject()
        record.conditions.append(target)
        target.__slots__ = self.attrs
        unpacked1 = ins.unpack('=B3sfH2s',12,readId)
        (target.operFlag,target.unused1,target.compValue,ifunc,target.unused2) = unpacked1
        #--Get parameters
        if ifunc not in allConditions:
            raise bolt.BoltError(u'Unknown condition function: %d\nparam1: %08X\nparam2: %08X' % (ifunc,ins.unpackRef(), ins.unpackRef()))
        form1 = 'I' if ifunc in fid1Conditions else 'i'
        form2 = 'I' if ifunc in fid2Conditions else 'i'
        form12 = form1+form2
        unpacked2 = ins.unpack(form12,8,readId)
        (target.param1,target.param2) = unpacked2
        target.unused3,target.reference,target.unused4 = ins.unpack('=4s2I',12,readId)
        (target.ifunc,target.form12) = (ifunc,form12)
        if self._debug:
            unpacked = unpacked1+unpacked2
            print u' ',zip(self.attrs,unpacked)
            if len(unpacked) != len(self.attrs):
                print u' ',unpacked

    def dumpData(self,record,out):
        """Dumps data from record to outstream."""
        for target in record.conditions:
            ##format = 'B3sfI'+target.form12+'4s'
            out.packSub('CTDA','=B3sfH2s'+target.form12+'4s2I',
                target.operFlag, target.unused1, target.compValue,
                target.ifunc, target.unused2, target.param1,
                target.param2, target.unused3, target.reference, target.unused4)

    def mapFids(self,record,function,save=False):
        """Applies function to fids. If save is true, then fid is set
        to result of function."""
        for target in record.conditions:
            form12 = target.form12
            if form12[0] == 'I':
                result = function(target.param1)
                if save: target.param1 = result
            if form12[1] == 'I':
                result = function(target.param2)
                if save: target.param2 = result
            if target.reference:
                result = function(target.reference)
                if save: target.reference = result

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

# Probably obsolete.  Included for reference and testing.
#------------------------------------------------------------------------------
class MelBounds(MelStruct):
    def __init__(self):
        MelStruct.__init__(self,'OBND','=6h',
            'x1','y1','z1',
            'x2','y2','z2')

# Verified Correct for Skyrim
#------------------------------------------------------------------------------
class MelColorN(MelStruct):
        def __init__(self):
                MelStruct.__init__(self,'CNAM','=4B',
                        'red','green','blue','unused')

# Verified Correct for Skyrim
#------------------------------------------------------------------------------
class MelIcons(MelGroup):
    """Handles ICON and MICO."""

#  wbICON := wbRStruct('Icon', [
#    wbString(ICON, 'Large Icon filename'),
#    wbString(MICO, 'Small Icon filename')
#  ], [], cpNormal, False, nil, True);

    def __init__(self,attr='iconsIaM'):
        """Initialize."""
        # iconsIaM = icons ICON and MICO
        MelGroup.__init__(self,attr,
            MelString('ICON','icon'),
            MelString('MICO','mico_n'),
        )

#------------------------------------------------------------------------------
# Is this required?  How can it be modified to only output what is present in
# the record.  Example, if ICON is there but MICO is not Skip MICO but if MICO
# is there but ICON is not Skip ICON
#------------------------------------------------------------------------------
#    def dumpData(self,record,out):
#        """Dumps data from record to outstream."""
#        if record.iconsIaM and record.iconsIaM.icon:
#            MelGroup.dumpData(self,record,out)

# Needs syntax check but otherwise Correct for Skyrim
#------------------------------------------------------------------------------
#function wbCOEDOwnerDecider(aBasePtr: Pointer; aEndPtr: Pointer; const aElement: IwbElement): Integer;
#var
#  Container  : IwbContainer;
#  LinksTo    : IwbElement;
#  MainRecord : IwbMainRecord;
#begin
#  Result := 0;
#  if aElement.ElementType = etValue then
#    Container := aElement.Container
#  else
#    Container := aElement as IwbContainer;
#
#  LinksTo := Container.ElementByName['Owner'].LinksTo;
#
#
#  if Supports(LinksTo, IwbMainRecord, MainRecord) then
#    if MainRecord.Signature = 'NPC_' then
#      Result := 1
#    else if MainRecord.Signature = 'FACT' then
#      Result := 2;
#end;
#Basically the Idea is this;
#When it's an NPC_ then it's a FormID of a [GLOB]
#When it's an FACT (Faction) then it's a 4Byte integer Rank of the faction.
#When it's not an NPC_ or FACT then it's unknown and just a 4Byte integer

#class MelCoed(MelStruct):
# wbCOED := wbStructExSK(COED, [2], [0, 1], 'Extra Data', [
#    {00} wbFormIDCkNoReach('Owner', [NPC_, FACT, NULL]),
#    {04} wbUnion('Global Variable / Required Rank', wbCOEDOwnerDecider, [
#           wbByteArray('Unknown', 4, cpIgnore),
#           wbFormIDCk('Global Variable', [GLOB, NULL]),
#           wbInteger('Required Rank', itS32)
#         ]),
#    {08} wbFloat('Item Condition')
#  ]);

class MelCoed(MelOptStruct):
    def __init__(self):
        MelOptStruct.__init__(self,'COED','=IIf',(FID,'owner'),(FID,'rank_or_glob_or_unk'), ('rank'))

# When all of Skyrim's records are entered this needs to be updated
# To more closly resemple the wbCOEDOwnerDecider from TES5Edit
#-------------------------------------------------------------------------------
class MelKeywords(MelFidList):
    """Handle writing out the KSIZ subrecord for the KWDA subrecord"""
    def dumpData(self,record,out):
        keywords = record.__getattribute__(self.attr)
        if keywords:
            # Only write the KSIZ/KWDA subrecords if count > 0
            out.packSub('KSIZ','I',len(keywords))
            MelFidList.dumpData(self,record,out)

# Verified Correct for Skyrim
#-------------------------------------------------------------------------------
class MelComponents(MelStructs):
    """Handle writing COCT subrecord for the CNTO subrecord"""
    def dumpData(self,record,out):
        components = record.__getattribute__(self.attr)
        if components:
            # Only write the COCT/CNTO subrecords if count > 0
            out.packSub('COCT','I',len(components))
            MelStructs.dumpData(self,record,out)

class MelPerks(MelStructs):
    """Handle writing PRKZ subrecord for the PRKR subrecord"""
    def dumpData(self,record,out):
        perks = record.__getattribute__(self.attr)
        if perks:
            out.packSub('PRKZ','<I',len(perks))
            MelStructs.dumpData(self,record,out)

class MelSpells(MelStructs):
    """Handle writing out the SPCT subrecord for the SPLO subrecord"""
    def dumpData(self, record, out):
        spells = record.__getattribute__(self.attr)
        if spells:
            out.packSub('SPCT','<I',len(spells))
            MelStructs.dumpData(self,record,out)

# Verified Correct for Skyrim
#------------------------------------------------------------------------------
class MelString16(MelString):
    """Represents a mod record string element."""
    def loadData(self,record,ins,type,size,readId):
        """Reads data from ins into record attribute."""
        strLen = ins.unpack('H',2,readId)
        value = ins.readString(strLen,readId)
        record.__setattr__(self.attr,value)
        if self._debug: print u' ',record.__getattribute__(self.attr)

    def dumpData(self,record,out):
        """Dumps data from record to outstream."""
        value = record.__getattribute__(self.attr)
        if value != None:
            if self.maxSize:
                value = bolt.winNewLines(value.rstrip())
                size = min(self.maxSize,len(value))
                test,encoding = _encode(value,returnEncoding=True)
                extra_encoded = len(test) - self.maxSize
                if extra_encoded > 0:
                    total = 0
                    i = -1
                    while total < extra_encoded:
                        total += len(value[i].encode(encoding))
                        i -= 1
                    size += i + 1
                    value = value[:size]
                    value = _encode(value,firstEncoding=encoding)
                else:
                    value = test
            else:
                value = _encode(value)
            value = struct.pack('H',len(value))+value
            out.packSub0(self.subType,value)

#-------------------------------------------------------------------------------
class MelString32(MelString):
    """Represents a mod record string element."""
    def loadData(self,record,ins,type,size,readId):
        """Reads data from ins into record attribute."""
        strLen = ins.unpack('I',4,readId)
        value = ins.readString(strLen,readId)
        record.__setattr__(self.attr,value)
        if self._debug: print u' ',record.__getattribute__(self.attr)

    def dumpData(self,record,out):
        """Dumps data from record to outstream."""
        value = record.__getattribute__(self.attr)
        if value != None:
            if self.maxSize:
                value = bolt.winNewLines(value.rstrip())
                size = min(self.maxSize,len(value))
                test,encoding = _encode(value,returnEncoding=True)
                extra_encoded = len(test) - self.maxSize
                if extra_encoded > 0:
                    total = 0
                    i = -1
                    while total < extra_encoded:
                        total += len(value[i].encode(encoding))
                        i -= 1
                    size += i + 1
                    value = value[:size]
                    value = _encode(value,firstEncoding=encoding)
                else:
                    value = test
            else:
                value = _encode(value)
            value = struct.pack('I',len(value))+value
            out.packSub0(self.subType,value)

#-------------------------------------------------------------------------------
class MelMODS(MelBase):
    """MODS/MO2S/etc/DMDS subrecord"""
    def hasFids(self,formElements):
        """Include self if has fids."""
        formElements.add(self)

    def setDefault(self,record):
        """Sets default value for record instance."""
        record.__setattr__(self.attr,None)

    def loadData(self,record,ins,type,size,readId):
        """Reads data from ins into record attribute."""
        insUnpack = ins.unpack
        insRead32 = ins.readString32
        count, = insUnpack('I',4,readId)
        data = []
        dataAppend = data.append
        for x in xrange(count):
            string = ins.readString32(size,readId)
            fid = ins.unpackRef(readId)
            unk, = ins.unpack('I',4,readId)
            dataAppend((string,fid,unk))
        record.__setattr__(self.attr,data)

    def dumpData(self,record,out):
        """Dumps data from record to outstream."""
        data = record.__getattribute__(self.attr)
        if data is not None:
            structPack = struct.pack
            data = record.__getattribute__(self.attr)
            outData = structPack('I',len(data))
            for (string,fid,unk) in data:
                outData += structPack('I',len(string))
                outData += _encode(string)
                outData += structPack('=2I',fid,unk)
            out.packSub(self.subType,outData)

    def mapFids(self,record,function,save=False):
        """Applies function to fids.  If save is true, then fid is set
           to result of function."""
        attr = self.attr
        data = record.__getattribute__(attr)
        if data is not None:
            data = [(string,function(fid),unk) for (string,fid,unk) in record.__getattribute__(attr)]
            if save: record.__setattr__(attr,data)

# Verified Correct for Skyrim
#-------------------------------------------------------------------------------
class MelModel(MelGroup):
    """Represents a model record."""
    # MODB and MODD are no longer used by TES5Edit
    typeSets = {
        'MODL': ('MODL','MODT','MODS'),
        'MOD2': ('MOD2','MO2T','MO2S'),
        'MOD3': ('MOD3','MO3T','MO3S'),
        'MOD4': ('MOD4','MO4T','MO4S'),
        'MOD5': ('MOD5','MO5T','MO5S'),
        'DMDL': ('DMDL','DMDT','DMDS'),
        }
    def __init__(self,attr='model',type='MODL'):
        """Initialize."""
        types = self.__class__.typeSets[type]
        MelGroup.__init__(self,attr,
            MelString(types[0],'modPath'),
            MelBase(types[1],'modt_p'),
            MelMODS(types[2],'mod_s'),
            )

    def debug(self,on=True):
        """Sets debug flag on self."""
        for element in self.elements[:2]: element.debug(on)
        return self

#-------------------------------------------------------------------------------
class MelBipedObjectData(MelStruct):
    """Handler for BODT/BOD2 subrecords.  Reads both types, writes only BOD2"""
    BipedFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'head'),
            (1, 'hair'),
            (2, 'body'),
            (3, 'hands'),
            (4, 'forearms'),
            (5, 'amulet'),
            (6, 'ring'),
            (7, 'feet'),
            (8, 'calves'),
            (9, 'shield'),
            (10, 'bodyaddon1_tail'),
            (11, 'long_hair'),
            (12, 'circlet'),
            (13, 'bodyaddon2'),
            (14, 'dragon_head'),
            (15, 'dragon_lwing'),
            (16, 'dragon_rwing'),
            (17, 'dragon_body'),
            (18, 'bodyaddon7'),
            (19, 'bodyaddon8'),
            (20, 'decapate_head'),
            (21, 'decapate'),
            (22, 'bodyaddon9'),
            (23, 'bodyaddon10'),
            (24, 'bodyaddon11'),
            (25, 'bodyaddon12'),
            (26, 'bodyaddon13'),
            (27, 'bodyaddon14'),
            (28, 'bodyaddon15'),
            (29, 'bodyaddon16'),
            (30, 'bodyaddon17'),
            (31, 'fx01'),
        ))

    ## Legacy Flags, (For BODT subrecords) - #4 is the only one not discarded.
    LegacyFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'modulates_voice'), #{>>> From ARMA <<<}
            (1, 'unknown_2'),
            (2, 'unknown_3'),
            (3, 'unknown_4'),
            (4, 'non_playable'), #{>>> From ARMO <<<}
        ))

    ArmorTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'light_armor'),
        (1, 'heavy_armor'),
        (2, 'clothing'),
        ))

    def __init__(self):
        MelStruct.__init__(self,'BOD2','=2I',(MelBipedObjectData.BipedFlags,'bipedFlags',0L),(MelBipedObjectData.ArmorTypeFlags,'armorFlags',0L))

    def getLoaders(self,loaders):
        # Loads either old style BODT or new style BOD2 records
        loaders['BOD2'] = self
        loaders['BODT'] = self

    def loadData(self,record,ins,type,size,readId):
        if type == 'BODT':
            # Old record type, use alternate loading routine
            if size == 8:
                # Version 20 of this subrecord is only 8 bytes (armorType omitted)
                bipedFlags,legacyData = ins.unpack('=2I',size,readId)
                armorFlags = 0
            elif size != 12:
                raise ModSizeError(ins.inName,readId,12,size,True)
            else:
                bipedFlags,legacyData,armorFlags = ins.unpack('=3I',size,readId)
            # legacyData is discarded except for non-playable status
            setter = record.__setattr__
            setter('bipedFlags',MelBipedObjectData.BipedFlags(bipedFlags))
            legacyFlags = MelBipedObjectData.LegacyFlags(legacyData)
            record.flags1[2] = legacyFlags[4]
            setter('armorFlags',MelBipedObjectData.ArmorTypeFlags(armorFlags))
        else:
            # BOD2 - new style, MelStruct can handle it
            MelStruct.loadData(self,record,ins,type,size,readId)

# Verified Correct for Skyrim
#-------------------------------------------------------------------------------
# Record Elements
#-------------------------------------------------------------------------------
class MelVmad(MelBase):
    """Virtual Machine data (VMAD)"""
    # Maybe use this later for better access to Fid,Aid pairs?
    ##ObjectRef = collections.namedtuple('ObjectRef',['fid','aid'])
    class FragmentInfo(object):
        __slots__ = ('unk','fileName',)
        def __init__(self):
            self.unk = 0
            self.fileName = u''

        def loadData(self,ins,Type,readId):
            if Type == 'INFO':
                raise Exception(u"Fragment Scripts for 'INFO' records are not implemented.")
            elif Type == 'PACK':
                self.unk,count = ins.unpack('=bB',2,readId)
                self.fileName = ins.readString16(-1,readId)
                count = bin(count).count('1')
            elif Type == 'PERK':
                self.unk, = ins.unpack('=b',1,readId)
                self.fileName = ins.readString16(-1,readId)
                count, = ins.unpack('=H',2,readId)
            elif Type == 'QUST':
                self.unk,count = ins.unpack('=bH',3,readId)
                self.fileName = ins.readString16(-1,readId)
            elif Type == 'SCEN':
                raise Exception(u"Fragment Scripts for 'SCEN' records are not implemented.")
            else:
                raise Exception(u"Unexpected Fragment Scripts for record type '%s'." % Type)
            return count

        def dumpData(self,Type,count):
            structPack = struct.pack
            fileName = _encode(self.fileName)
            if Type == 'INFO':
                raise Exception(u"Fragment Scripts for 'INFO' records are not implemented.")
            elif Type == 'PACK':
                # TODO: check if this is right!
                count = int(count*'1',2)
                data = structPack('=bBH',self.unk,count,len(fileName)) + fileName
            elif Type == 'PERK':
                data = structPack('=bH',self.unk,len(fileName)) + fileName
                data += structPack('=H',count)
            elif Type == 'QUST':
                data = structPack('=bHH',self.unk,count,len(fileName)) + fileName
            elif Type == 'SCEN':
                raise Exception(u"Fragment Scripts for 'SCEN' records are not implemented.")
            else:
                raise Exception(u"Unexpected Fragment Scripts for record type '%s'." % Type)
            return data

    class INFOFragment(object):
        pass

    class PACKFragment(object):
        __slots__ = ('unk','scriptName','fragmentName',)
        def __init__(self):
            self.unk = 0
            self.scriptName = u''
            self.fragmentName = u''

        def loadData(self,ins,readId):
            self.unk = ins.unpack('=b',1,readId)
            self.scriptName = ins.readString16(-1,readId)
            self.fragmentName = ins.readString16(-1,readId)

        def dumpData(self):
            structPack = struct.pack
            scriptName = _encode(self.scriptName)
            fragmentName = _encode(self.fragmentName)
            data = structPack('=bH',self.unk,len(scriptName)) + scriptName
            data += structPack('=H',len(fragmentName)) + fragmentName
            return data

    class PERKFragment(object):
        __slots__ = ('index','unk1','unk2','scriptName','fragmentName',)
        def __init__(self):
            self.index = -1
            self.unk1 = 0
            self.unk2 = 0
            self.scriptName = u''
            self.fragmentName= u''

        def loadData(self,ins,readId):
            self.index,self.unk1,self.unk2 = ins.unpack('=Hhb',4,readId)
            self.scriptName = ins.readString16(-1,readId)
            self.fragmentName = ins.readString16(-1,readId)

        def dumpData(self):
            structPack = struct.pack
            scriptName = _encode(self.scriptName)
            fragmentName = _encode(self.fragmentName)
            data = structPack('=HhbH',self.index,self.unk1,self.unk2,len(scriptName)) + scriptName
            data += structPack('=H',len(fragmentName)) + fragmentName
            return data

    class QUSTFragment(object):
        __slots__ = ('index','unk1','unk2','unk3','scriptName','fragmentName',)
        def __init__(self):
            self.index = -1
            self.unk1 = 0
            self.unk2 = 0
            self.unk3 = 0
            self.scriptName = u''
            self.fragmentName = u''

        def loadData(self,ins,readId):
            self.index,self.unk1,self.unk2,self.unk3 = ins.unpack('=Hhib',9,readId)
            self.scriptName = ins.readString16(-1,readId)
            self.fragmentName = ins.readString16(-1,readId)

        def dumpData(self):
            structPack = struct.pack
            scriptName = _encode(self.scriptName)
            fragmentName = _encode(self.fragmentName)
            data = structPack('=HhibH',self.index,self.unk1,self.unk2,self.unk3,len(scriptName)) + scriptName
            data += structPack('=H',len(fragmentName)) + fragmentName
            return data

    class SCENFragment(object):
        pass

    FragmentMap = {'INFO': INFOFragment,
                   'PACK': PACKFragment,
                   'PERK': PERKFragment,
                   'QUST': QUSTFragment,
                   'SCEN': SCENFragment,
                   }

    class Property(object):
        __slots__ = ('name','unk','value',)
        def __init__(self):
            self.name = u''
            self.unk = 1
            self.value = None

        def loadData(self,ins,version,objFormat,readId):
            insUnpack = ins.unpack
            # Script Property
            self.name = ins.readString16(-1,readId)
            if version >= 4:
                Type,self.unk = insUnpack('=2B',2,readId)
            else:
                Type, = insUnpack('=B',1,readId)
                self.unk = 1
            # Data
            if Type == 1:
                # Object (8 Bytes)
                if objFormat == 1:
                    fid,aid,nul = insUnpack('=IHH',8,readId)
                else:
                    nul,aid,fid = insUnpack('=HHI',8,readId)
                self.value = (fid,aid)
            elif Type == 2:
                # String
                self.value = ins.readString16(-1,readId)
            elif Type == 3:
                # Int32
                self.value, = insUnpack('=i',4,readId)
            elif Type == 4:
                # Float
                self.value, = insUnpack('=f',4,readId)
            elif Type == 5:
                # Bool (Int8)
                self.value = bool(insUnpack('=b',1,readId)[0])
            elif Type == 11:
                # List of Objects
                count, = insUnpack('=I',4,readId)
                if objFormat == 1: # (fid,aid,nul)
                    value = insUnpack('='+count*'IHH',count*8,readId)
                    self.value = zip(value[::3],value[1::3]) # list of (fid,aid)'s
                else: # (nul,aid,fid)
                    value = insUnpack('='+count*'HHI',count*8,readId)
                    self.value = zip(value[2::3],value[1::3]) # list of (fid,aid)'s
            elif Type == 12:
                # List of Strings
                count, = insUnpack('=I',4,readId)
                self.value = [ins.readString16(-1,readId) for i in xrange(count)]
            elif Type == 13:
                # List of Int32s
                count, = insUnpack('=I',4,readId)
                self.value = list(insUnpack('='+`count`+'i',count*4,readId))
            elif Type == 14:
                # List of Floats
                count, = insUnpack('=I',4,readId)
                self.value = list(insUnpack('='+`count`+'f',count*4,readId))
            elif Type == 15:
                # List of Bools (int8)
                count, = insUnpack('=I',4,readId)
                self.value = map(bool,insUnpack('='+`count`+'b',count,readId))
            else:
                raise Exception(u'Unrecognized VM Data property type: %i' % Type)

        def dumpData(self):
            structPack = struct.pack
            ## Property Entry
            # Property Name
            name = _encode(self.name)
            data = structPack('=H',len(name))+name
            # Property Type
            value = self.value
            # Type 1 - Object Reference
            if isinstance(value,tuple):
                # Object Format 1 - (Fid, Aid, NULL)
                data += structPack('=BBIHH',1,self.unk,value[0],value[1],0)
            # Type 2 - String
            elif isinstance(value,basestring):
                value = _encode(value)
                data += structPack('=BBH',2,self.unk,len(value))+value
            # Type 3 - Int
            elif isinstance(value,(int,long)):
                data += structPack('=BBi',3,self.unk,value)
            # Type 4 - Float
            elif isinstance(value,float):
                data += structPack('=BBf',4,self.unk,value)
            # Type 5 - Bool
            elif isinstance(value,bool):
                data += structPack('=BBb',5,self.unk,value)
            # Type 11 -> 15 - lists
            elif isinstance(value,list):
                # Empty list, fail to object refereneces?
                count = len(value)
                if not count:
                    data += structPack('=BBI',11,self.unk,count)
                else:
                    Type = value[0]
                    # Type 11 - Object References
                    if isinstance(Type,tuple):
                        value = list(from_iterable([x+(0,) for x in value]))
                        # value = [fid,aid,NULL, fid,aid,NULL, ...]
                        data += structPack('=BBI'+count*'IHH',11,self.unk,count,*value)
                    # Type 12 - Strings
                    elif isinstance(Type,basestring):
                        data += structPack('=BBI',12,self.unk,count)
                        for string in value:
                            string = _encode(string)
                            data += structPack('=H',len(string))+string
                    # Type 13 - Ints
                    elif isinstance(Type,(int,long)):
                        data += structPack('=BBI'+`count`+'i',13,self.unk,count,*value)
                    # Type 14 - Floats
                    elif isinstance(Type,float):
                        data += structPack('=BBI'+`count`+'f',14,self.unk,count,*value)
                    # Type 15 - Bools
                    elif isinstance(Type,bool):
                        data += structPack('=BBI'+`count`+'b',15,self.unk,count,*value)
                    else:
                        raise Exception(u'Unrecognized VMAD property type: %s' % type(Type))
            else:
                raise Exception(u'Unrecognized VMAD property type: %s' % type(Type))
            return data

    class Script(object):
        __slots__ = ('name','unk','properties',)
        def __init__(self):
            self.name = u''
            self.unk = 0
            self.properties = []

        def loadData(self,ins,version,objFormat,readId):
            Property = MelVmad.Property
            self.properties = []
            propAppend = self.properties.append
            # Script Entry
            self.name = ins.readString16(-1,readId)
            if version >= 4:
                self.unk,propCount = ins.unpack('=BH',3,readId)
            else:
                self.unk = 0
                propCount, = ins.unpack('=H',2,readId)
            # Properties
            for x in xrange(propCount):
                prop = Property()
                prop.loadData(ins,version,objFormat,readId)
                propAppend(prop)

        def dumpData(self):
            structPack = struct.pack
            ## Script Entry
            # scriptName
            name = _encode(self.name)
            data = structPack('=H',len(name))+name
            # unkown, property count
            data += structPack('=BH',self.unk,len(self.properties))
            # properties
            for prop in self.properties:
                data += prop.dumpData()
            return data

        def mapFids(self,record,function,save=False):
            for prop in self.properties:
                value = prop.value
                # Type 1 - Object Reference
                if isinstance(value,tuple):
                    value = (function(value[0]),value[1])
                    if save:
                        prop.value = value
                # Type 11 - List of Object References
                elif isinstance(value,list) and value and isinstance(value[0],tuple):
                    value = [(function(x[0]),x[1]) for x in value]
                    if save:
                        prop.value = value

    class Alias(object):
        __slots__ = ('unk1','aid','unk2','unk3','scripts',)
        def __init__(self):
            self.unk1 = 0
            self.aid = 0
            self.unk2 = 0
            self.unk3 = 0
            self.scripts = []

        def loadData(self,ins,version,readId):
            self.unk1,self.aid,self.unk2,self.unk3,objFormat,count = ins.unpack('=hHihhH',14)
            Script = MelVmad.Script
            self.scripts = []
            scriptAppend = self.scripts.append
            for x in xrange(count):
                script = Script()
                script.loadData(ins,version,objFormat,readId)
                scriptAppend(script)

        def mapFids(self,record,function,save=False):
            for script in self.scripts:
                script.mapFids(record,function,save)

    class Vmad(object):
        __slots__ = ('scripts','fragmentInfo','fragments','aliases',)
        def __init__(self):
            self.scripts = []
            self.fragmentInfo = None
            self.fragments = None
            self.aliases = None

        def loadData(self,record,ins,size,readId):
            insTell = ins.tell
            endOfField = insTell() + size
            self.scripts = []
            scriptsAppend = self.scripts.append
            Script = MelVmad.Script
            # VMAD Header
            version,objFormat,scriptCount = ins.unpack('=3H',6,readId)
            # Primary Scripts
            for x in xrange(scriptCount):
                script = Script()
                script.loadData(ins,version,objFormat,readId)
                scriptsAppend(script)
            # Script Fragments
            if insTell() < endOfField:
                self.fragmentInfo = MelVmad.FragmentInfo()
                Type = record._Type
                fragCount = self.fragmentInfo.loadData(ins,Type,readId)
                self.fragments = []
                fragAppend = self.fragments.append
                Fragment = MelVmad.FragmentMap[Type]
                for x in xrange(fragCount):
                    frag = Fragment()
                    frag.loadData(ins,readId)
                    fragAppend(frag)
                # Alias Scripts
                if Type == 'QUST':
                    aliasCount = ins.unpack('=H',2,readId)
                    Alias = MelVmad.Alias
                    self.aliases = []
                    aliasAppend = self.aliases.append
                    for x in xrange(aliasCount):
                        alias = Alias()
                        alias.loadData(ins,version,readId)
                        aliasAppend(alias)
                else:
                    self.aliases = None
            else:
                self.fragmentInfo = None
                self.fragments = None
                self.aliases = None

        def dumpData(self,record):
            structPack = struct.pack
            # Header
            data = structPack('=3H',4,1,len(self.scripts)) # vmad version, object format, script count
            # Primary Scripts
            for script in self.scripts:
                data += script.dumpData()
            # Script Fragments
            if self.fragments:
                Type = record._Type
                data += self.fragmentInfo.dumpData(Type,len(self.fragments))
                for frag in self.fragments:
                    data += frag.dumpData()
                if Type == 'QUST':
                    # Alias Scripts
                    aliases = self.aliases
                    data += structPack('=H',2,len(aliases))
                    for alias in aliases:
                        data += alias.dumpData()
            return data

        def mapFids(self,record,function,save=False):
            for script in self.scripts:
                script.mapFids(record,function,save)
            if not self.aliases:
                return
            for alias in self.aliases:
                alias.mapFids(record,function,save)

    def __init__(self,type='VMAD',attr='vmdata'):
        MelBase.__init__(self,type,attr)

    def hasFids(self,formElements):
        """Include self if has fids."""
        formElements.add(self)

    def setDefault(self,record):
        record.__setattr__(self.attr,None)

    def getDefault(self):
        target = MelObject()
        return self.setDefault(target)

    def loadData(self,record,ins,type,size,readId):
        vmad = MelVmad.Vmad()
        vmad.loadData(record,ins,size,readId)
        record.__setattr__(self.attr,vmad)

    def dumpData(self,record,out):
        """Dumps data from record to outstream"""
        vmad = record.__getattribute__(self.attr)
        if vmad is None: return
        # Write
        out.packSub(self.subType,vmad.dumpData(record))

    def mapFids(self,record,function,save=False):
        """Applies function to fids.  If save is true, then fid is set
           to result of function."""
        vmad = record.__getattribute__(self.attr)
        if vmad is None: return
        vmad.mapFids(record,function,save)

#-------------------------------------------------------------------------------
#class MelActorValue(MelStructs):
#    """Represents Actor Values."""
#
# As more complex and large records are added this will be used more than once.
# Needs syntax Check, should me a enum type struct that can be used by any
# routine after a subrecord, example:
#
# wbInteger('Magic Skill', itS32, wbActorValueEnum),
# wbInteger('Resist Value', itS32, wbActorValueEnum),
#
# MelStruct('DATA','2I',MelActorValue,MelActorValue,),
#
#-------------------------------------------------------------------------------
# Commonly Used Flags ----------------------------------------------------------
#-------------------------------------------------------------------------------
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

#-------------------------------------------------------------------------------
# Skyrim Records ---------------------------------------------------------------
#-------------------------------------------------------------------------------
class MreHeader(MreHeaderBase):
    """TES4 Record.  File header."""
    classType = 'TES4'

    #--Data elements
    melSet = MelSet(
        MelStruct('HEDR','f2I',('version',0.94),'numRecords',('nextObject',0xCE6)),
        MelBase('OFST','ofst_p',),  #--Obsolete?
        MelBase('DELE','dele_p',),  #--Obsolete?
        MelUnicode('CNAM','author',u'',512),
        MelUnicode('SNAM','description',u'',512),
        # How do I know this is an array MAST DATA MAST DATA MAST DATA MAST DATA
        # For each Master File in the esm/esp
        # Why is MelGroups Not needed?
        # MreHeaderBase.MelMasterName should be updated to handle the DATA record
        MreHeaderBase.MelMasterName('MAST','masters'),
        MelNull('DATA'), # 8 Bytes in Length
        MelFidList('ONAM','overrides'),
        MelBase('SCRN', 'ingv_p'),
        MelBase('INTV','ingv_p'),
        MelBase('INCC', 'ingv_p'),
        )
    __slots__ = MreHeaderBase.__slots__ + melSet.getSlotsUsed()

# MAST and DATA need to be grouped together like MAST DATA MAST DATA, are they that way already?
#------------------------------------------------------------------------------
class MreAchr(MelRecord):
    """Placed NPC"""
    classType = 'ACHR'

    ACHRTopicDataFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'topicRef'),
        (1, 'topicSubtype'),
    ))

    # 'Set Enable State to Opposite of Parent',
    # 'Pop In'
    EnableParentFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'setEnableStatetoOppositeofParent'),
            (1, 'popIn'),
        ))
    
    # 'Parent Activate Only'
    ActivateParentsFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'parentActivateOnly'),
        ))
    
    # 'Easy',
    # 'Medium',
    # 'Hard',
    # 'Very Hard'
    LeveledActorFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'easy'),
            (1, 'medium'),
            (2, 'hard'),
            (3, 'veryHard'),
        ))
    
    # -------------------------
    # class MelACHRPDTOHandeler(MelGroup):
    # -------------------------
    # wbPDTO :=
    #   wbStruct(PDTO, 'Topic Data', [
    #   wbInteger('Type', itU32, wbEnum([
    #     'Topic Ref',
    #     'Topic Subtype'
    #   ])),
    #   wbUnion('Data', wbTypeDecider, [
    #     wbFormIDCk('Topic', [DIAL, NULL]),
    #     wbString('Subtype', 4)
    #   ])
    # ]);
    # wbPDTOs := wbRArray('Topic', wbPDTO, cpNormal, False, nil);
    # wbPDTOs,
    # -------------------------
    # wbUnion means that a condition has to be met. When the condition is 0
    # it uses the first line, 1 is the next line.
    # -------------------------
    # wbEnum is an Enumeration and I don't know how to handle them.  For now
    # make them flags so that if the patcher needs to mitigate the change
    # it will be there already.
    # -------------------------
    # wbPDTOs,
    # -------------------------
    # Data in PDTO needs decide if it's a FormID or String.
    # Because one is four bytes and the other is a string with a four
    # byte length.  [00000008] 'A string' not [08] 'A string'
    # -------------------------
    # class MelACHRPDTOHandeler(MelGroup):
    # -------------------------
    # Handle the flags first
    # (ACHRTopicDataFlags,'topicDataFlags',0L),
    # Then decide if it is a string or FormID.
    # If it's a valid string then it's a string,
    # if it's a valid FormID then it's a FormID
    # Output the record with the flags first then the Data
    # Also this need to be repeating for as many as are present.
    # like with MelGroups, or MelFids it's repeating
    # PDTO, PDTO, PDTO, PDTO...
    # -------------------------

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        #These last two don't have other records to reference for how to enter them
        # wbFormIDCk(NAME, 'Base', [NPC_], False, cpNormal, True),
        # wbFormIDCk(XEZN, 'Encounter Zone', [ECZN]),
        # TES5Edit cares about what the FormID type is such as NPC_ or ECZN, Wrye Bash does not.
        # Use the four letter name of the record and then the label within quotes from TES5Edit
        MelFid('NAME','base'),
        MelFid('XEZN','encounterZone'),

        # {--- Ragdoll ---}

        # wbXRGD := wbByteArray(XRGD, 'Ragdoll Data');
        # wbXRGB := wbByteArray(XRGB, 'Ragdoll Biped Data');
        # wbByteArray without a parameter or "0" means that the data is
        # read from the file just as it is found, and written without being altered.
        MelBase('XRGD','xrgd_p'),
        MelBase('XRGB','xrgb_p'),
        # A Struct in TES5Edit either contains fields or records.
        # Wrye Bash uses MelStruct for fields and MelGroup for Records.

        # {--- Patrol Data ---}

        MelGroup('patrolData',
            MelStruct('XPRD','f','idleTime',),
            # In TES5Edit wbEmpty is normally a Marker with no value like an
            # empty xml tag <record></record> or <record />
            MelNull('XPPA'),
            MelFid('INAM','idle'),
            MelGroup('patrolData',
                # wbUnknown means it has not or cannot be decoded
                MelBase('SCHR','schr_p'),
                MelBase('SCDA','scda_p'),
                MelBase('SCTX','sctx_p'),
                MelBase('QNAM','qnam_p'),
                MelBase('SCRO','scro_p'),
            ),
            # Should Be -> MelACHRPDTOHandeler(),
            MelBase('PDTO','pdto_p'),
            MelFid('TNAM','topic'),
        ),
        # End of MelGroup('patrolData',"

        # {--- Leveled Actor ----}
        MelStruct('XLCM','I',(LeveledActorFlags,'flags',0L),),

        # {--- Merchant Container ----}
        MelFid('XMRC','merchantContainer',),

        # {--- Extra ---}
        MelStruct('XCNT','i','count'),
        MelStruct('XRDS','f','radius',),
        MelStruct('XHLP','f','health',),
        MelGroup('linkedReferences',
            MelSortedFidList('XLCM', 'fids'),
        ),

        # {--- Activate Parents ---}
        MelGroup('activateParents',
            MelStruct('XAPD','I',(ActivateParentsFlags,'flags',0L),),
            MelGroups('activateParentRefs',
                MelStruct('XAPR','If',(FID,'reference'),'delay',),
            ),
        ),

        # {--- Linked Ref ---}
        MelStruct('XCLP','3Bs3Bs','startColorRed','startColorGreen','startColorBlue','startColorUnknown',
                  'endColorRed','endColorGreen','endColorBlue','endColorUnknown',),
        MelFid('XLCN','persistentLocation',),
        MelFid('XLRL','locationReference',),
        MelNull('XIS2'),
        MelFidList('XLRT','locationRefType',),
        MelFid('XHOR','horse',),
        MelStruct('XHTW','f','headTrackingWeight',),
        MelStruct('XFVC','f','favorCost',),
        
        # {--- Enable Parent ---}
        MelStruct('XESP','IB',(FID,'Reference'),(EnableParentFlags,'flags',0L),'unused',),
        
        # {--- Ownership ---}
        MelOwnership(),

        # {--- Emittance ---}
        MelFid('XEMI','emittance',),

        # {--- MultiBound ---}
        MelFid('XMBR','multiBoundReference',),

        # {--- Flags ---}
        MelNull('XIBS'),

        # {--- 3D Data ---}
        MelStruct('XSCL','f','scale',),
        MelStruct('DATA','6f','potX','potY','potY','rotX','rotY','rotZ',),

        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Needs more updating
#------------------------------------------------------------------------------
class MreActi(MelRecord):
    """Activator."""
    classType = 'ACTI'

    ActivatorFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'noDisplacement'),
        (0, 'ignoredBySandbox'),
    ))


    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelStruct('PNAM','=4B','red','green','blue','unused'),
        MelOptStruct('SNAM','I',(FID,'dropSound')),
        MelOptStruct('VNAM','I',(FID,'pickupSound')),
        MelOptStruct('WNAM','I',(FID,'water')),
        MelLString('RNAM','rnam_p'),
        MelStruct('FNAM','I',(ActivatorFlags,'flags',0L),),
        MelOptStruct('KNAM','I',(FID,'keyword')),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreTact(MelRecord):
    """Talking Activator"""
    classType = 'TACT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelBase('PNAM','pnam_p'),
        MelOptStruct('SNAM','I',(FID,'loopingSound')),
        MelBase('FNAM','fnam_p'),
        MelOptStruct('VNAM','I',(FID,'voiceType')),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAlch(MelRecord):
    """Ingestible"""
    classType = 'ALCH'

    IngestibleFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'noAutoCalcUnused'),
        (1, 'foodItem'),
        (2, 'unknown3'),
        (3, 'unknown4'),
        (4, 'unknown5'),
        (5, 'unknown6'),
        (6, 'unknown7'),
        (7, 'unknown8'),
        (8, 'unknown9'),
        (9, 'unknown10'),
        (10, 'unknown11'),
        (11, 'unknown12'),
        (12, 'unknown13'),
        (13, 'unknown14'),
        (14, 'unknown15'),
        (15, 'unknown16'),
        (16, 'medicine'),
        (17, 'poison'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelLString('FULL','full'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelLString('DESC','description'),
        MelModel(),
        MelDestructible(),
        MelIcons(),
        MelOptStruct('YNAM','I',(FID,'pickupSound')),
        MelOptStruct('ZNAM','I',(FID,'dropSound')),
        MelOptStruct('ETYP','I',(FID,'equipType')),
        MelStruct('DATA','f','weight_p'),
        MelStruct('ENIT','i2IfI',
                  'value_pd',(IngestibleFlags,'flags',0L),'addiction','addictionChance','soundConsume',),
        MelEffects(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAmmo(MelRecord):
    """Ammo record (arrows)"""
    classType = 'AMMO'

    AmmoTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'notNormalWeapon'),
        (1, 'nonPlayable'),
        (2, 'nonBolt'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelIcons(),
        MelDestructible(),
        MelFid('YNAM','pickupSound'),
        MelFid('ZNAM','dropSound'),
        MelLString('DESC','description'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelStruct('DATA','IIfI',(FID,'projectile'),(AmmoTypeFlags,'flags',0L),'damage','value'),
        MelString('ONAM','onam_n'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAnio(MelRecord):
    """Anio record (Animated Object)"""
    classType = 'ANIO'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelString('BNAM','unloadEvent'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreArmo(MelRecord):
    """Armor"""
    classType = 'ARMO'

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelOptStruct('EITM','I',(FID,'enchantment')),
        MelOptStruct('EAMT','H','enchantmentAmount',),
        MelModel('model1','MOD2'),
        MelIcons(),
        MelModel('model3','MOD4'),
        MelString('ICO2','ico2_n'),
        MelString('MIC2','mic2_n'),
        MelBipedObjectData(),
        MelDestructible(),
        MelOptStruct('YNAM','I',(FID,'pickupSound')),
        MelOptStruct('ZNAM','I',(FID,'dropSound')),
        MelString('BMCT','ragConTemp'), #Ragdoll Constraint Template
        MelOptStruct('ETYP','I',(FID,'equipType')),
        MelOptStruct('BIDS','I',(FID,'bashImpact')),
        MelOptStruct('BAMT','I',(FID,'material')),
        MelOptStruct('RNAM','I',(FID,'race')),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelLString('DESC','description'),
        MelFids('MODL','addons'),
        MelStruct('DATA','=if','value','weight'),
        MelStruct('DNAM','i','armorRating'),
        MelFid('TNAM','baseItem'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreArma(MelRecord):
    """Armor addon?"""
    classType = 'ARMA'

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBipedObjectData(),
        MelFid('RNAM','race'),
        MelBase('DNAM','dnam_p'),
        MelModel('male_model','MOD2'),
        MelModel('female_model','MOD3'),
        MelModel('male_model_1st','MOD4'),
        MelModel('female_model_1st','MOD5'),
        MelOptStruct('NAM0','I',(FID,'skin0')),
        MelOptStruct('NAM1','I',(FID,'skin1')),
        MelOptStruct('NAM2','I',(FID,'skin2')),
        MelOptStruct('NAM3','I',(FID,'skin3')),
        MelFids('MODL','races'),
        MelOptStruct('SNDD','I',(FID,'footstepSound')),
        MelOptStruct('ONAM','I',(FID,'art_object')),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreBook(MelRecord):
    """Book Item"""
    classType = 'BOOK'

    BookTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'teachesSkill'),
            (1, 'cantBeTaken'),
            (2, 'teachesSpell'),
        ))

    bookTypes = {
        0:'book_tome',
        255:'note_scroll',
        }

    skillTypes = {
       -1 :'None',
        7 :'One Handed',
        8 :'Two Handed',
        9 :'Archery',
        10:'Block',
        11:'Smithing',
        12:'Heavy Armor',
        13:'Light Armor',
        14:'Pickpocket',
        15:'Lockpicking',
        16:'Sneak',
        17:'Alchemy',
        18:'Speech',
        19:'Alteration',
        20:'Conjuration',
        21:'Destruction',
        22:'Illusion',
        23:'Restoration',
        24:'Enchanting',
        }

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelIcons(),
        MelLString('DESC','description'),
        MelDestructible(),
        MelOptStruct('YNAM','I',(FID,'pickupSound')),
        MelOptStruct('ZNAM','I',(FID,'dropSound')),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelStruct('DATA','4B2If',(BookTypeFlags,'flags',0L),'bookType','unused','unused','skillOrSpell','value','weight'),
        MelFids('INAM','races'),
        MelString('CNAM','cnam_n'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# DATA needs to have 'skillOrSpell' save an integer or FormID to be mergable.
# BookTypeFlags needs syntax check.
# bookTypes needs syntax check.
# skillTypes needs syntax check.
# After syntax checks and DATA is formated correctly, this record is correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreCell(MelRecord):
    """Cell"""
    classType = 'CELL'

    # {0x00000001}'Ambient Color',
    # {0x00000002}'Directional Color',
    # {0x00000004}'Fog Color',
    # {0x00000008}'Fog Near',
    # {0x00000010}'Fog Far',
    # {0x00000020}'Directional Rotation',
    # {0x00000040}'Directional Fade',
    # {0x00000080}'Clip Distance',
    # {0x00000100}'Fog Power',
    # {0x00000200}'Fog Max',
    # {0x00000400}'Light Fade Distances'
    CellInheritedFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'ambientColor'),
            (1, 'directionalColor'),
            (2, 'fogColor'),
            (3, 'fogNear'),
            (4, 'fogFar'),
            (5, 'directionalRotation'),
            (6, 'directionalFade'),
            (7, 'clipDistance'),
            (8, 'fogPower'),
            (9, 'fogMax'),
            (10, 'lightFadeDistances'),
        ))
    
    # 'Quad 1',
    # 'Quad 2',
    # 'Quad 3',
    # 'Quad 4'
    CellGridFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'quad1'),
            (1, 'quad2'),
            (2, 'quad3'),
            (3, 'quad4'),
        ))
    
    # {0x0001} 'Is Interior Cell',
    # {0x0002} 'Has Water',
    # {0x0004} 'Can''t Travel From Here',
    # {0x0008} 'No LOD Water',
    # {0x0010} 'Unknown 5',
    # {0x0020} 'Public Area',
    # {0x0040} 'Hand Changed',
    # {0x0080} 'Show Sky',
    # {0x0100} 'Use Sky Lighting'
    CellDataFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'isInteriorCell'),
            (1, 'hasWater'),
            (2, 'can'),
            (3, 'noLODWater'),
            (4, 'unknown5'),
            (5, 'publicArea'),
            (6, 'handChanged'),
            (7, 'showSky'),
            (8, 'useSkyLighting'),
        ))

# Flags can be itU8, but CELL\DATA has a critical role in various wbImplementation.pas routines
# and replacing it with wbUnion generates error when setting for example persistent flag in REFR.
# So let it be always itU16
    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelStruct('DATA','I',(CellDataFlags,'flags',0L),),
        MelStruct('XCLC','2iI','pos_x','pos_y',(CellGridFlags,'flags',0L),),
        MelStruct('XCLL','3Bs3Bs3Bs2f2i3f3Bs3fI',
                  'red_ac','green_ac','blue_ac','unknown_ac',
                  'red_dc','green_dc','blue_dc','unknown_dc',
                  'red_fcn','green_fcn','blue_fcn','unknown_fcn',
                  'fogNear','fogFar','directionalRotationXY','directionalRotationZ',
                  'directionalFade','fogClipDistance','fogPower',
                  'red_fcf','green_fcf','blue_fcf','unknown_fcf',
                  'fogMax','lightFadeBegin','lightFadeEnd',(CellInheritedFlags,'flags',0L),),
        MelBase('TVDT','unknown_TVDT'),
        MelBase('MHDT','unknown_MHDT'),
        MelFid('LTMP','lightingTemplate',),
        # leftover flags, they are now in XCLC
        MelBase('LNAM','unknown_LNAM'),
        # XCLW sometimes has $FF7FFFFF and causes invalid floation point
        MelStruct('XCLW','f','waterHeight',),
        MelString('XNAM','waterNoiseTexture'),
        MelFidList('XCLR','regions'),
        MelFid('XLCN','location',),
        MelBase('XWCN','unknown_XWCN'),
        MelBase('XWCS','unknown_XWCS'),
        # unknown_ang = wbByteArray('Unknown', 0) 'XWCU' neds custom unpacker
        # MelStruct('XWCU','6f','xOffset','yOffset','zOffset','unknown_off',
        #          'xAngle','yAngle','zAngle','unknown_ang',),
        MelBase('XWCU','waterVelocity',),
        MelFid('XCWT','water',),        

        # {--- Ownership ---}
        MelOwnership(),
        MelFid('XILL','lockList',),
        MelString('XWEM','waterEnvironmentMap'),
        MelFid('XCCM','skyWeatherFromRegion',),
        MelFid('XCAS','acousticSpace',),
        MelFid('XEZN','encounterZone',),
        MelFid('XCMO','musicType',),
        MelFid('XCIM','imageSpace',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreClas(MelRecord):
    """Clas record (Alchemical Apparatus)"""
    classType = 'CLAS'

    ClasTeachesFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'oneHanded'),
        (1, 'twoHanded'),
        (2, 'archery'),
        (3, 'block'),
        (4, 'smithing'),
        (5, 'heavyArmor'),
        (6, 'lightArmor'),
        (7, 'pickpocket'),
        (8, 'lockpicking'),
        (9, 'sneak'),
        (10, 'alchemy'),
        (11, 'speech'),
        (12, 'alteration'),
        (13, 'conjuration'),
        (14, 'destruction'),
        (15, 'illusion'),
        (16, 'restoration'),
        (17, 'enchanting'),
    ))

    ClasSkillWeightsFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'oneHanded'),
        (1, 'twoHanded'),
        (2, 'archery'),
        (3, 'block'),
        (4, 'smithing'),
        (5, 'heavyArmor'),
        (6, 'lightArmor'),
        (7, 'pickpocket'),
        (8, 'lockpicking'),
        (9, 'sneak'),
        (10, 'alchemy'),
        (11, 'speech'),
        (12, 'alteration'),
        (13, 'conjuration'),
        (14, 'destruction'),
        (15, 'illusion'),
        (16, 'restoration'),
        (17, 'enchanting'),
    ))

    ClasAttributeWeightsFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'health'),
        (1, 'magicka'),
        (2, 'stamina'),
        (3, 'unknown1'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelLString('DESC','description'),
        MelIcons(),
        MelStruct('DATA','Ib2BfIB','unknownValue',(ClasTeachesFlags,'teachesSkill',0L),'maxTrainingLvl',
                  (ClasSkillWeightsFlags,'skillWeights',0L),'bleedoutDefault','voicePoints',
                  (ClasAttributeWeightsFlags,'attributeWeights',0L),
            ),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreClmt(MelRecord):
    """Climate"""
    classType = 'CLMT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('WLST','IiI',(FID,'weather',None),'chance',(FID,'global',None),),
        MelLString('FNAM','sunTexture'),
        MelLString('GNAM','sunGlareTexture'),
        MelModel(),
        MelStruct('TNAM','6B','timingBegin','timingEnd','sunsetBegin','sunsetEnd',
        'volatility','moonsPhaseLength',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MelSpgdData(MelStruct):
    def __init__(self,type='DATA'):
        MelStruct.__init__(self,type,'=7f4If',
                           'gravityVelocity','rotationVelocity','particleSizeX','particleSizeY',
                           'centerOffsetMin','centerOffsetMax','initialRotationRange',
                           'numSubtexturesX','numSubtexturesY','type',
                           ('boxSize',0),
                           ('particleDensity',0),
                           )


    def loadData(self,record,ins,type,size,readId):
        """Reads data from ins into record attribute."""
        if size == 40:
            # 40 Bytes for legacy data post Skyrim 1.5 DATA is always 48 bytes
            # fffffffIIIIf
            # Type is an Enum 0 = Rain; 1 = Snow
            unpacked = ins.unpack('=7f3I',size,readId) + (0,0,)
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if action: value = action(value)
                setter(attr,value)
            if self._debug:
                print u' ',zip(self.attrs,unpacked)
                if len(unpacked) != len(self.attrs):
                    print u' ',unpacked
        elif size != 48:
            raise ModSizeError(ins.inName,readId,48,size,True)
        else:
            MelStruct.loadData(self,record,ins,type,size,readId)

#------------------------------------------------------------------------------
#    class MelIdlmIdlc(MelStruct):
#        """Handle older trucated IDLC for IDLM subrecord."""
#        def loadData(self,record,ins,type,size,readId):
#            if size == 4:
#                MelStruct.loadData(self,record,ins,type,size,readId)
#                return
#            elif size == 1:
#                unpacked = ins.unpack('B',size,readId)
#            else:
#                raise "Unexpected size encountered for TERM:DNAM subrecord: %s" % size
#            unpacked += self.defaults[len(unpacked):]
#            setter = record.__setattr__
#            for attr,value,action in zip(self.attrs,unpacked,self.actions):
#                if callable(action): value = action(value)
#                setter(attr,value)
#            if self._debug: print unpacked
#------------------------------------------------------------------------------
# Intigrate above syntax into the syntax below for consistancy unless the above
# syntax is better
#------------------------------------------------------------------------------

class MreSpgd(MelRecord):
    """Spgd Item"""
    classType = 'SPGD'

    melSet = MelSet(
        MelString('EDID','eid'),
        MelSpgdData(),
        MelString('ICON','icon'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreRfct(MelRecord):
    """Rfct Item"""
    classType = 'RFCT'

    RfctTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'rotateToFaceTarget'),
        (1, 'attachToCamera'),
        (2, 'inheritRotation'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DATA','3I',(FID,'impactSet'),(FID,'impactSet'),(RfctTypeFlags,'flags',0L),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreCont(MelRecord):
    """Container"""
    classType = 'CONT'

	# wbCNTO :=
	#   wbRStructExSK([0], [1], 'Item', [
	#     wbStructExSK(CNTO, [0], [1], 'Item', [
	#       wbFormIDCk('Item', [ARMO, AMMO, APPA, MISC, WEAP, BOOK, LVLI, KEYM, ALCH, INGR, LIGH, SLGM, SCRL]),
	#       wbInteger('Count', itS32)
	#     ]),
	# 	wbCOED
	# 	], []);
	# wbCOCT := wbInteger(COCT, 'Count', itU32)
	# wbCNTOs := wbRArrayS('Items', wbCNTO)

    class MelContCnto(MelGroups):
        def __init__(self):
            MelGroups.__init__(self,'components',
                MelStruct('CNTO','=2I',(FID,'item',None),'count'),
                MelCoed(),
                )

        def dumpData(self,record,out):
            # Only write the COCT/CNTO/COED subrecords if count > 0
            out.packSub('COCT','I',len(record.components))
            MelGroups.dumpData(self,record,out)


    ContTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'allowSoundsWhenAnimation'),
        (1, 'respawns'),
        (2, 'showOwner'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        # One Count: COCT
        # Handled by MreContCnto
        MelNull('COCT'),
        # Repeating CNTO records: CNTO, CNTO, CNTO, CNTO : Of the Count COCT
        MelContCnto(),
        MelDestructible(),
        MelStruct('DATA','Bf',(ContTypeFlags,'flags',0L),'weight'),
        MelFid('SNAM','openSound'),
        MelFid('QNAM','closeSound'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# MelContCnto, COCT, and CNTO needs to be finished.
#------------------------------------------------------------------------------
class MreCsty(MelRecord):
    """Csty Item"""
    classType = 'CSTY'

    CstyTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'dueling'),
        (1, 'flanking'),
        (2, 'allowDualWielding'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        # esm = Equipment Score Mult
        MelStruct('CSGD','10f','offensiveMult','defensiveMult','groupOffensiveMult',
        'esmMelee','esmMagic','esmRanged','esmShout','esmUnarmed','esmStaff',
        'avoidThreatChance',),
        MelBase('CSMD','unknownValue'),
        MelStruct('CSME','8f','atkStaggeredMult','powerAtkStaggeredMult','powerAtkBlockingMult',
        'bashMult','bashRecoilMult','bashAttackMult','bashPowerAtkMult','specialAtkMult',),
        MelStruct('CSCR','4f','circleMult','fallbackMult','flankDistance','stalkTime',),
        MelStruct('CSLR','f','strafeMult'),
        MelStruct('CSFL','8f','hoverChance','diveBombChance','groundAttackChance','hoverTime',
        'groundAttackTime','perchAttackChance','perchAttackTime','flyingAttackChance',),
        MelStruct('DATA','I',(CstyTypeFlags,'flags',0L),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreDial(MelRecord):
    """Dialogue Records"""
    classType = 'DIAL'

    DialTopicFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'doAllBeforeRepeating'),
    ))

    DialSubtypeTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'custom'),
        (1, 'forceGreet'),
        (2, 'rumors'),
        (3, 'custom'),
        (4, 'intimidate'),
        (5, 'flatter'),
        (6, 'bribe'),
        (7, 'askGift'),
        (8, 'gift'),
        (9, 'askFavor'),
        (10, 'favor'),
        (11, 'showRelationships'),
        (12, 'folow'),
        (13, 'reject'),
        (14, 'scene'),
        (15, 'show'),
        (16, 'agree'),
        (17, 'refuse'),
        (18, 'exitFavorState'),
        (19, 'moralRefusal'),
        (20, 'flyingMountLand'),
        (21, 'flyingMountCancelLand'),
        (22, 'flyingMountAcceptTarget'),
        (23, 'flyingMountRejectTarget'),
        (24, 'flyingMountNoTarget'),
        (25, 'flyingMountDestinationReached'),
        (26, 'attack'),
        (27, 'powerAttack'),
        (28, 'bash'),
        (29, 'hit'),
        (30, 'flee'),
        (31, 'bleedout'),
        (32, 'avoidThreat'),
        (33, 'death'),
        (34, 'groupStrategy'),
        (35, 'block'),
        (36, 'taunt'),
        (37, 'allyKilled'),
        (38, 'steal'),
        (39, 'yield'),
        (40, 'acceptYield'),
        (41, 'pickpocketCombat'),
        (42, 'assault'),
        (43, 'murder'),
        (44, 'assaultNC'),
        (45, 'murderNC'),
        (46, 'pickpocketNC'),
        (47, 'stealFromNC'),
        (48, 'trespassAgainstNC'),
        (49, 'trespass'),
        (50, 'wereTransformCrime'),
        (51, 'voicePowerStartShort'),
        (52, 'voicePowerStartLong'),
        (53, 'voicePowerEndShort'),
        (54, 'voicePowerEndLong'),
        (55, 'alertIdle'),
        (56, 'lostIdle'),
        (57, 'normalToAlert'),
        (58, 'alertToCombat'),
        (59, 'normalToCombat'),
        (60, 'alertToNormal'),
        (61, 'combatToNormal'),
        (62, 'combatToLost'),
        (63, 'lostToNormal'),
        (64, 'lostToCombat'),
        (65, 'detectFriendDie'),
        (66, 'serviceRefusal'),
        (67, 'repair'),
        (68, 'travel'),
        (69, 'training'),
        (70, 'barterExit'),
        (71, 'repairExit'),
        (72, 'recharge'),
        (73, 'rechargeExit'),
        (74, 'trainingExit'),
        (75, 'observeCombat'),
        (76, 'noticeCorpse'),
        (77, 'timeToGo'),
        (78, 'goodBye'),
        (79, 'hello'),
        (80, 'swingMeleeWeapon'),
        (81, 'shootBow'),
        (82, 'zKeyObject'),
        (83, 'jump'),
        (84, 'knockOverObject'),
        (85, 'destroyObject'),
        (86, 'StandonFurniture'),
        (87, 'lockedObject'),
        (88, 'pickpocketTopic'),
        (89, 'pursueIdleTopic'),
        (90, 'sharedInfo'),
        (91, 'playerCastProjectileSpell'),
        (92, 'playerCastSelfSpell'),
        (93, 'playerShout'),
        (94, 'idle'),
        (95, 'enterSprintBreath'),
        (96, 'enterBowZoomBreath'),
        (97, 'exitBowZoomBreath'),
        (98, 'actorCollidewithActor'),
        (99, 'playerinIronSights'),
        (100, 'outofBreath'),
        (101, 'combatGrunt'),
        (102, 'leaveWaterBreath'),
    ))

    # {0} 'Topic',
    # {1} 'Favor', // only in DA14 quest topics
    # {2} 'Scene',
    # {3} 'Combat',
    # {4} 'Favors',
    # {5} 'Detection',
    # {6} 'Service',
    # {7} 'Miscellaneous'
    DialCategoryFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'topic'),
            (1, 'favor'),
            (2, 'scene'),
            (3, 'combat'),
            (4, 'favors'),
            (5, 'detection'),
            (6, 'service'),
            (7, 'miscellaneous'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelStruct('PNAM','f','priority',),
        MelFid('BNAM','branch',),
        MelFid('QNAM','quest',),
        MelStruct('DATA','2BH',(DialTopicFlags,'flags_dt',0L),(DialCategoryFlags,'flags_cf',0L),
                  (DialSubtypeTypeFlags,'flags_st',0L),),
        MelString('SNAM','subtypeName',),
        MelStruct('TIFC','I','infoCount',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

#------------------------------------------------------------------------------
# Found error.  MobDials in bosh.py is looking for "def loadInfos"
#------------------------------------------------------------------------------
#     def __init__(self,header,ins=None,unpack=False):
#         """Initialize."""
#         MelRecord.__init__(self,header,ins,unpack)
#         self.infoStamp = 0 #--Stamp for info GRUP
#         self.infoStamp2 = 0 #--Stamp for info GRUP
#         self.infos = []
#
#     def loadInfos(self,ins,endPos,infoClass):
#         """Load infos from ins. Called from MobDials."""
#         infos = self.infos
#         recHead = ins.unpackRecHeader
#         infosAppend = infos.append
#         while not ins.atEnd(endPos,'INFO Block'):
#             #--Get record info and handle it
#             header = recHead()
#             recType = header[0]
#             if recType == 'INFO':
#                 info = infoClass(header,ins,True)
#                 infosAppend(info)
#             else:
#                 raise ModError(ins.inName, _('Unexpected %s record in %s group.')
#                     % (recType,"INFO"))
#
#     def dump(self,out):
#         """Dumps self., then group header and then records."""
#         MreRecord.dump(self,out)
#         if not self.infos: return
#         size = 20 + sum([20 + info.getSize() for info in self.infos])
#         out.pack('4sIIIII','GRUP',size,self.fid,7,self.infoStamp,self.infoStamp2)
#         for info in self.infos: info.dump(out)
#
#     def updateMasters(self,masters):
#         """Updates set of master names according to masters actually used."""
#         MelRecord.updateMasters(self,masters)
#         for info in self.infos:
#             info.updateMasters(masters)
#
#     def convertFids(self,mapper,toLong):
#         """Converts fids between formats according to mapper.
#         toLong should be True if converting to long format or False if converting to short format."""
#         MelRecord.convertFids(self,mapper,toLong)
#         for info in self.infos:
#             info.convertFids(mapper,toLong)
#
#------------------------------------------------------------------------------
# Above routines need update for Skyrim
#------------------------------------------------------------------------------
# Causes unknown errors that don't make sense
# Error in Dawnguard.esm
# bosh.py 1526 load:
# Traceback (most recent call last):
#   File "bash\bosh.py", line 1520, in load
#     selfTops[label].load(ins,unpack and (topClass != MobBase))
#   File "bash\bosh.py", line 495, in load
#     self.loadData(ins, ins.tell()+self.size-self.header.__class__.size)
#   File "bash\bosh.py", line 718, in loadData
#     recordLoadInfos = record.loadInfos
# AttributeError: 'MreRecord' object has no attribute 'loadInfos'
#
# Otherwise should be correct for Skyrim
#------------------------------------------------------------------------------
class MreDoor(MelRecord):
    """Door Record"""
    classType = 'DOOR'

    DoorTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'unknown1'),
        (1, 'automatic'),
        (2, 'hidden'),
        (3, 'minimalUse'),
        (4, 'sliding'),
        (5, 'doNotOpenInCombatSearch'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelFid('SNAM','openSound'),
        MelFid('ANAM','openSound'),
        MelFid('BNAM','openSound'),
        MelStruct('FNAM','Bf',(DoorTypeFlags,'flags',0L),'weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreEfsh(MelRecord):
    """Efsh Record"""
    classType = 'EFSH'

    EfshBlendModeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'unknownOne'),
        (1, 'zero'),
        (2, 'one'),
        (3, 'sourceColor'),
        (4, 'sourceInverseColor'),
        (5, 'sourceAlpha'),
        (6, 'sourceInvertedAlpha'),
        (7, 'destAlpha'),
        (8, 'destInvertedAlpha'),
        (9, 'destColor'),
        (10, 'destInverseColor'),
        (11, 'sourceAlphaSAT'),
    ))

    EfshBlendOpFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'unknown1'),
        (1, 'add'),
        (2, 'subtract'),
        (3, 'reverseSubrtact'),
        (4, 'minimum'),
        (5, 'maximum'),
    ))

    EfshZTestFuncFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'unknown1'),
        (1, 'unknown2'),
        (2, 'unknown3'),
        (3, 'equalTo'),
        (4, 'normal'),
        (5, 'greaterThan'),
        (6, 'unknown4'),
        (7, 'greaterThanOrEqualTo'),
        (8, 'alwaysShow'),
    ))

    EfshGeneralFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'noMembraneShader'),
        (1, 'membraneGrayscaleColor'),
        (2, 'membraneGrayscaleAlpha'),
        (3, 'noParticleShader'),
        (4, 'edgeEffectInverse'),
        (5, 'affectSkinOnly'),
        (6, 'ignoreAlpha'),
        (7, 'projectUVs'),
        (8, 'ignoreBaseGeometryAlpha'),
        (9, 'lighting'),
        (10, 'noWeapons'),
        (11, 'unknown11'),
        (12, 'unknown12'),
        (13, 'unknown13'),
        (14, 'unknown14'),
        (15, 'particleAnimated'),
        (16, 'particleGrayscaleColor'),
        (17, 'particleGrayscaleAlpha'),
        (18, 'unknown18'),
        (19, 'unknown19'),
        (20, 'unknown20'),
        (21, 'unknown21'),
        (22, 'unknown22'),
        (23, 'unknown23'),
        (24, 'useBloodGeometry'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('ICON','icon_s'),
        MelString('ICO2','ico2_s'),
        MelString('NAM7','nam7_s'),
        MelString('NAM8','nam8_s'),
        MelString('NAM9','nam9_s'),
        MelStruct('DATA','4I4B9f4B8f5I19f12B11fI5f4Bf2I6fI8B9f8I2fI','unknownValue','sourceBlendMode','blendOperation'
        'zTestFunction','fteColorKey1Red','fteColorKey1Green','fteColorKey1Blue','fteColorKey1Unk','fteAlphaFadeInTime',
        'fteFullAlphaTime','fteAlphaFadeOutTime','ftePresistentAlphaRatio','fteAlphaPulseAmplitude','fteAlphaPulseFrequency',
        'fteTextureAnimationSpeed_U','fteTextureAnimationSpeed_V','EdgeEffectFallOff','efColorRed','efColorGreen','efColorBlue',
        'efColorUnk','efAlphaFadeInTime','efFullAlphaTime','efAlphaFadeOutTime','efPersistentAlphaRatio','efAlphaPulseAmplitude',
        'efAlphaPusleFrequency','fteFullAlphaRatio','efFullAlphaRatio','msDestBlendMode','psSourceBlendMode','psBlendOperation',
        'psZTestFunction','psDestBlendMode','psParticleBirthRampUpTime','psFullParticleBirthTime','psParticleBirthRampDownTime',
        'psFullParticleBirthRatio','psPersistantParticleCount','psParticleLifetime','psParticleLifetimePlusMinus',
        'psInitialSpeedAlongNormal','psAccelerationAlongNormal','psInitialVelocity_1','psInitialVelocity_2','psInitialVelocity_3',
        'psAcceleration_1','psAcceleration_2','psAcceleration_3','psScaleKey1','psScaleKey2','psScaleKey1Time','psScaleKey2Time',
        'ck1ColorRed','ck1ColorGreen','ck1ColorBlue','ck1ColorUnk','ck2ColorRed','ck2ColorGreen','ck2ColorBlue','ck2ColorUnk',
        'ck3ColorRed','ck3ColorGreen','ck3ColorBlue','ck3ColorUnk','ck1ColorColorAlpha','ck2ColorColorAlpha','ck3ColorColorAlpha',
        'ck1ColorKeyTime','ck2ColorKeyTime','ck3ColorKeyTime','psInitialSpeedAlongNormalPlusMinus','psInitialRotationdeg',
        'psInitialRotationdegPlusMinus','psRotationSpeedDegSec','psRotationSpeedDegSecPlusMinus',(FID,'AddonModels'),
        'holesStartTime','holesEndTime','holesStartVal','holesEndVal','edgeWidthAlphaUnits','edgColorRed','edgColorGreen',
        'edgColorBlue','edgColorUnk','explosionWindSpeed','textureCount_U','textureCount_V','addonMdlFadeInTime','addonMdlFadeOutTime',
        'addonMdlScaleStart','addonMdlScaleEnd','addonMdlScaleInTime','addonMdlScaleOutTime',(FID,'AmbientSound'),'fteColorKey2Red',
        'fteColorKey2Green','fteColorKey2Blue','fteColorKey2Unk','fteColorKey3Red','fteColorKey3Green','fteColorKey3Blue',
        'fteColorKey3Unk','fteColorKey1Scale','fteColorKey2Scale','fteColorKey3Scale','fteColorKey1Time','fteColorKey2Time',
        'fteColorKey3Time''ColorScale','birthPositionOffset','birthPositionOffsetRangePlusMinus','psaStartFrame','psaStartFrameVariation',
        'psaEndFrame','psaLoopStartFrame','psaLoopStartVariation','psaFrameCount','psaFrameCountVariation',
        (EfshGeneralFlags,'teachesSkill',0L),'fteTextureScale_U','fteTextureScale_V','SceneGraphEmitDepthLimit_unused',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreEnch(MelRecord):
    """Enchants"""
    classType = 'ENCH'

    EnchGeneralFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'noAutoCalc'),
        (1, 'unknownTwo'),
        (2, 'extendDurationOnRecast'),
    ))

    # enchantment _06_
    # staffEnchantment _0C_
    EnchEnchantTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'enchantment'),
        (1, 'staffEnchantment'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelLString('FULL','full'),
        MelStruct('ENIT','6If2I','enchantmentCost',(EnchGeneralFlags,'generalFlags',0L),'castType',
                  'enchantmentAmount','targetType',(EnchEnchantTypeFlags,'enchantType',0L),
                  'chargeTime',(FID,'baseEnchantment'),(FID,'wornRestrictions'),
            ),
        MelEffects(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreEyes(MelRecord):
    """Eyes Item"""
    classType = 'EYES'

    # {0x01}'Playable',
    # {0x02}'Not Male',
    # {0x04}'Not Female',
    EyesTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'playable'),
            (1, 'notMale'),
            (2, 'notFemale'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelString('ICON','icon'),
        MelStruct('DATA','B',(EyesTypeFlags,'flags',0L)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreFact(MelRecord):
    """Fact Faction Records"""
    classType = 'FACT'

    FactCombatReactionFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'neutral'),
        (1, 'enemy'),
        (2, 'ally'),
        (3, 'friend'),
    ))

    FactGeneralTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'hiddenFromNPC'),
        (1, 'specialCombat'),
        (2, 'unknown3'),
        (3, 'unknown4'),
        (4, 'unknown5'),
        (5, 'unknown6'),
        (6, 'irackCrime'),
        (7, 'ignoreCrimes:Murder'),
        (8, 'ignoreCrimes:Assult'),
        (9, 'ignoreCrimes:Stealing'),
        (10, 'ignoreCrimes:Trespass'),
        (11, 'doNotReportCrimesAgainstMembers'),
        (12, 'crimeGold-UseDefaults'),
        (13, 'ignoreCrimes:Pickpocket'),
        (14, 'vendor'),
        (15, 'canBeOwner'),
        (16, 'ignoreCrimes:Werewolf'),
        (17, 'unknown18'),
        (18, 'unknown19'),
        (19, 'unknown20'),
        (20, 'unknown21'),
        (21, 'unknown22'),
        (22, 'unknown23'),
        (23, 'unknown24'),
        (24, 'unknown25'),
        (25, 'unknown26'),
        (26, 'unknown27'),
        (27, 'unknown28'),
        (28, 'unknown29'),
        (29, 'unknown30'),
        (30, 'unknown31'),
        (31, 'unknown32'),
    ))

# wbXNAM :=
#   wbStructSK(XNAM, [0], 'Relation', [
#    wbFormIDCkNoReach('Faction', [FACT, RACE]),
#    wbInteger('Modifier', itS32),
#    wbInteger('Group Combat Reaction', itU32, wbEnum([
#      {0x00000001}'Neutral',
#      {0x00000002}'Enemy',
#      {0x00000004}'Ally',
#      {0x00000008}'Friend'
#    ]))
# ]);

#   wbPLVD := wbStruct(PLVD, 'Location', [
#     wbInteger('Type', itS32, wbLocationEnum),
#     wbUnion('Location Value', wbTypeDecider, [
#       {0} wbFormIDCkNoReach('Reference', [NULL, DOOR, PLYR, ACHR, REFR, PGRE, PHZD, PARW, PBAR, PBEA, PCON, PFLA]),
#       {1} wbFormIDCkNoReach('Cell', [NULL, CELL]),
#       {2} wbByteArray('Near Package Start Location', 4, cpIgnore),
#       {3} wbByteArray('Near Editor Location', 4, cpIgnore),
#       {4} wbFormIDCkNoReach('Object ID', [NULL, ACTI, DOOR, STAT, FURN, SPEL, SCRL, NPC_, CONT, ARMO, AMMO, MISC, WEAP, BOOK, KEYM, ALCH, INGR, LIGH, FACT, FLST, IDLM, SHOU]),
#       {5} wbInteger('Object Type', itU32, wbObjectTypeEnum),
#       {6} wbFormIDCk('Keyword', [NULL, KYWD]),
#       {7} wbByteArray('Unknown', 4, cpIgnore),
#       {8} wbInteger('Alias ID', itU32),
#       {9} wbFormIDCkNoReach('Reference', [NULL, DOOR, PLYR, ACHR, REFR, PGRE, PHZD, PARW, PBAR, PBEA, PCON, PFLA]),
#      {10} wbByteArray('Unknown', 4, cpIgnore),
#      {11} wbByteArray('Unknown', 4, cpIgnore),
#      {12} wbByteArray('Unknown', 4, cpIgnore)
#     ]),
#     wbInteger('Radius', itS32)
#   ]);

#    class MelFactCrva(MelStruct):
#        """Handle older trucated DATA for PROJ subrecord."""
#        def loadData(self,record,ins,type,size,readId):
#            if size == 84:
#                MelStruct.loadData(self,record,ins,type,size,readId)
#                return
#            elif size == 68:
#                unpacked = ins.unpack('HHfffIIfffIIfffIII',size,readId)
#            else:
#                raise "Unexpected size encountered for PROJ:DATA subrecord: %s" % size
#            unpacked += self.defaults[len(unpacked):]
#            setter = record.__setattr__
#            for attr,value,action in zip(self.attrs,unpacked,self.actions):
#                if callable(action): value = action(value)
#                setter(attr,value)
#            if self._debug: print unpacked
#------------------------------------------------------------------------------
# Needs updated and to process old CRVA size
#------------------------------------------------------------------------------

    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelGroups('relationArray',
            MelStruct('XNAM','IiI',(FID,'faction'),'modifier',(FactCombatReactionFlags,'combatReactionFlags',0L),),
            ),
        MelStruct('DATA','I',(FactGeneralTypeFlags,'factionGeneralFlags',0L),),
        MelFid('JAIL','exteriorJailMarker'),
        MelFid('WAIT','followerWaitMarker'),
        MelFid('STOL','stolenGoodsContainer'),
        MelFid('PLCN','playerInventoryContainer'),
        MelFid('CRGR','sharedCrimeFactionList'),
        MelFid('JOUT','jailOutfit'),
        # MelFactCrva() when finished
        MelStruct('CRVA','2B5Hf2H','arrest','attackOnSight','murder','assult','trespass','pickpocket','unknown','stealMultiplier','escape','werewolf'),
        MelStruct('RNAM','I','rankNumber'),
        MelLString('MNAM','maleTitle'),
        MelLString('FNAM','femaleTitle'),
        MelString('INAM','insigniaUnused'),
        MelFid('VEND','vendorBuySellList'),
        MelFid('VENC','merchantContainer'),
        MelStruct('VENV','4H2BH','startHour','endHour','radius','unknownOne','onlyBuysStolenItems','notSellBuy','UnknownTwo'),
        MelStruct('PLVD','iIi','type','locationValue','radius',),
        MelConditions(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# XNAM and PLVD Need to be reviewed
#------------------------------------------------------------------------------
class MreFurn(MelRecord):
    """Furniture"""
    classType = 'FURN'

    FurnGeneralFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'unknownOne'),
        (1, 'ignoredBySandbox'),
    ))

    FurnActiveMarkerFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'sit0'),
        (1, 'sit1'),
        (2, 'sit2'),
        (3, 'sit3'),
        (4, 'sit4'),
        (5, 'sit5'),
        (6, 'sit6'),
        (7, 'sit7'),
        (8, 'sit8'),
        (9, 'sit9'),
        (10, 'sit10'),
        (11, 'sit11'),
        (12, 'sit12'),
        (13, 'sit13'),
        (14, 'sit14'),
        (15, 'sit15'),
        (16, 'sit16'),
        (17, 'sit17'),
        (18, 'sit18'),
        (19, 'sit19'),
        (20, 'sit20'),
        (21, 'Sit21'),
        (22, 'Sit22'),
        (23, 'sit23'),
        (24, 'unknown25'),
        (25, 'disablesActivation'),
        (26, 'isPerch'),
        (27, 'mustExittoTalk'),
        (28, 'unknown29'),
        (29, 'unknown30'),
        (30, 'unknown31'),
        (31, 'unknown32'),
    ))

    FurnBenchTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'none'),
        (1, 'createobject'),
        (2, 'smithingWeapon'),
        (3, 'enchanting'),
        (4, 'enchantingExperiment'),
        (5, 'alchemy'),
        (6, 'alchemyExperiment'),
        (7, 'smithingArmor'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelBase('PNAM','pnam_p'),
        MelStruct('FNAM','H',(FurnGeneralFlags,'general_f',0L)),
        MelFid('KNAM','interactionKeyword'),
        MelStruct('MNAM','I',(FurnActiveMarkerFlags,'activeMarker_f',0L)),
        # UsesSkill needs to be flags or an Enum at some point
        MelStruct('WBDT','Bb',(FurnActiveMarkerFlags,'activeMarker_f',0L),'usesSkill',),
        MelFid('NAM1','associatedSpell'),
        # markerArray needs to be a repeating Array
        # Disabled  needs to be flags or an Enum at some point
        MelGroups('markerArray',
            MelStruct('ENAM','I','markerIndex',),
            MelStruct('NAM0','I','unknown','disabledPoints',),
            MelFid('FNMK','markerKeyword',),
            ),
        # furnitureAnimType and furnitureEntryType need to be flags or an Enum at some point
        MelGroups('markerArray',
            MelStruct('FNPR','2H','furnitureAnimType','furnitureEntryType',),
            ),
        MelString('XMRK','mico_n'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Needs syntax check, but should be Correct for Skyrim 1.8
# XNAM and PLVD Need to be reviewed
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# GLOB ------------------------------------------------------------------------
# Defined in brec.py as class MreGlob(MelRecord) ------------------------------
#------------------------------------------------------------------------------
class MreGmst(MreGmstBase):
    """Skyrim GMST record"""
    Master = u'Skyrim'
    isKeyedByEid = True # NULL fids are acceptable.

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreKywd(MelRecord):
    """Keyword record."""
    classType = 'KYWD'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelColorN(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreLcrt(MelRecord):
    """Location Reference Type record."""
    classType = 'LCRT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelColorN(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAact(MelRecord):
    """Action record."""
    classType = 'AACT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelColorN(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreTxst(MelRecord):
    """Texture Set"""
    classType = 'TXST'
    TxstTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'noSpecularMap'),
        (1, 'facegenTextures'),
        (2, 'hasModelSpaceNormalMap'),
    ))

    TxstParaTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'parallax'),
        (1, 'alphaBlending'),
        (2, 'alphaTesting'),
        (3, 'noSubtextures'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelGroups('destructionData',
            MelString('TX00','difuse'),
            MelString('TX01','normalGloss'),
            MelString('TX02','enviroMaskSubSurfaceTint'),
            MelString('TX03','glowDetailMap'),
            MelString('TX04','height'),
            MelString('TX05','environment'),
            MelString('TX06','multilayer'),
            MelString('TX07','backlightMaskSpecular'),
            ),
        MelStruct('DODT','7f8B','minWidth','maxWidth','minHeight','maxHeight',
                  'depth','shininess','parallaxScale','parallaxPasses',(TxstParaTypeFlags,'para_flags',0L),
                  'unknown_01','unknown_02','red','green','blue','unused',),
        MelStruct('DNAM','H',(TxstTypeFlags,'flags',0L),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreHdpt(MelRecord):
    """Head Part"""
    classType = 'HDPT'
    HdptTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'playable'),
        (1, 'male'),
        (2, 'female'),
        (3, 'isExtraPart'),
        (4, 'useSolidTint'),
    ))

    HdptTypeFlags02 = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'misc'),
        (1, 'face'),
        (2, 'eyes'),
        (3, 'hair'),
        (4, 'facialHair'),
        (5, 'scar'),
        (6, 'eyebrows'),
    ))

    HdptTypeFlags03 = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'raceMorph'),
        (1, 'tri'),
        (2, 'chargenMorph'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelModel(),
        MelStruct('DATA','B',(HdptTypeFlags,'hdptDataFlags',0L),),
        MelStruct('PNAM','I',(HdptTypeFlags02,'hdptDataFlags02',0L),),
        MelFids('HNAM','extraParts'),
        MelGroups('partsData',
            MelStruct('NAM0','I',(HdptTypeFlags03,'hdptDataFlags03',0L),),
            MelLString('NAM1','filename'),
            ),
        MelFids('TNAM','textureSet'),
        MelFids('CNAM','color'),
        MelFids('RNAM','validRaces'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAspc(MelRecord):
    """Aspc record (Acoustic Space)"""
    classType = 'ASPC'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelOptStruct('SNAM','I',(FID,'ambientSound')),
        MelOptStruct('RDAT','I',(FID,'regionData')),
        MelOptStruct('BNAM','I',(FID,'reverb')),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMstt(MelRecord):
    """Moveable static record."""
    classType = 'MSTT'

    MsttTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'onLocalMap'),
        (1, 'unknown2'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelStruct('DATA','B',(MsttTypeFlags,'flags',0L),),
        MelFid('SNAM','sound'),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreIdlm(MelRecord):
    """Idle marker record."""
    classType = 'IDLM'

    IdlmTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'runInSequence'),
        (1, 'unknown1'),
        (2, 'doOnce'),
        (3, 'unknown3'),
        (4, 'ignoredBySandbox'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelStruct('IDLF','B',(IdlmTypeFlags,'flags',0L),),
        MelStruct('IDLC','B','animationCount',),
        MelStruct('IDLT','f','idleTimerSetting'),
        MelFidList('IDLA','animations'),
        MelModel(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreProj(MelRecord):
    """Projectile record."""
    classType = 'PROJ'

    ProjTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'hitscan'),
        (1, 'explosive'),
        (2, 'altTriger'),
        (3, 'muzzleFlash'),
        (4, 'unknown4'),
        (5, 'canbeDisable'),
        (6, 'canbePickedUp'),
        (7, 'superSonic'),
        (8, 'pinsLimbs'),
        (9, 'passThroughSmallTransparent'),
        (10, 'disableCombatAimCorrection'),
        (11, 'rotation'),
    ))

    ProjectileTypes = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'missile'),
        (1, 'lobber'),
        (2, 'beam'),
        (3, 'flame'),
        (4, 'cone'),
        (5, 'barrier'),
        (6, 'arrow'),
    ))

    ProjSoundLevels = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'loud'),
        (1, 'normal'),
        (2, 'silent'),
        (3, 'veryLoud'),
    ))

    class MelProjData(MelStruct):
        """Handle older trucated DATA for PROJ subrecord."""
        def loadData(self,record,ins,type,size,readId):
            if size == 88:
                MelStruct.loadData(self,record,ins,type,size,readId)
                return
            elif size == 80:
                unpacked = ins.unpack('2H3f2I3f2I3f3I4f',size,readId)
            else:
                raise "Unexpected size encountered for PROJ:DATA subrecord: %s" % size
            unpacked += self.defaults[len(unpacked):]
            setter = record.__setattr__
            for attr,value,action in zip(self.attrs,unpacked,self.actions):
                if callable(action): value = action(value)
                setter(attr,value)
            if self._debug: print unpacked

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelProjData('DATA','2H3f2I3f2I3f3I4f2I',(ProjTypeFlags,'flags',0L),(ProjectileTypes,'type',0L),
                  ('gravity',0.00000),('speed',10000.00000),('range',10000.00000),
                  (FID,'light',0),(FID,'muzzleFlash',0),('tracerChance',0.00000),
                  ('explosionAltTrigerProximity',0.00000),('explosionAltTrigerTimer',0.00000),
                  (FID,'explosion',0),(FID,'sound',0),('muzzleFlashDuration',0.00000),
                  ('fadeDuration',0.00000),('impactForce',0.00000),
                  (FID,'soundCountDown',0),(FID,'soundDisable',0),(FID,'defaultWeaponSource',0),
                  ('coneSpread',0.00000),('collisionRadius',0.00000),('lifetime',0.00000),
                  ('relaunchInterval',0.00000),(FID,'decalData',0),(FID,'collisionLayer',0),
                  ),
        MelGroups('models',
            MelString('NAM1','muzzleFlashPath'),
            MelBase('NAM2','_nam2'), #--Should be a struct. Maybe later.
        ),
        MelStruct('VNAM','I',(ProjSoundLevels,'soundLevel',0L),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Needs syntax check but otherwise correct for Skyrim
#------------------------------------------------------------------------------
class MreHazd(MelRecord):
    """Hazard"""
    classType = 'HAZD'

    HazdTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'affectsPlayerOnly'),
        (1, 'inheritDurationFromSpawnSpell'),
        (2, 'alignToImpactNormal'),
        (3, 'inheritRadiusFromSpawnSpell'),
        (4, 'dropToGround'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelStruct('DATA','I4f5I','limit','radius','lifetime',
                  'imageSpaceRadius','targetInterval',(HazdTypeFlags,'flags',0L),
                  (FID,'spell'),(FID,'light'),(FID,'impactDataSet'),(FID,'sound'),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreSlgm(MelRecord):
    """Soul gem record."""
    classType = 'SLGM'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelString('FULL','full'),
        MelModel(),
        MelIcons(),
        MelDestructible(),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelIcons(),
        MelStruct('DATA','If','value','weight'),
        MelStruct('SOUL','B',('soul',0)),
        MelStruct('SLCP','B',('capacity',1)),
        MelFid('NAM0','linkedTo'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreNavi(MelRecord):
    """Navigation Mesh Info Map"""
    classType = 'NAVI'

    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('NVER','I','version'),
        # NVMI and NVPP would need special routines to handle them
        # If no mitigation is needed, then leave it as MelBase
        MelBase('NVMI','navigationMapInfos',),
        MelBase('NVPP','preferredPathing',),
        MelFidList('NVSI','navigationMesh'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreNavm(MelRecord):
    """Navigation Mesh"""
    classType = 'NAVM'

    # 'Unknown 1',
    # 'Unknown 2',
    # 'Unknown 3',
    # 'Unknown 4',
    # 'Unknown 5',
    # 'Unknown 6',
    # 'Preferred pathing',
    # 'Unknown 8',
    # 'Unknown 9',
    # 'Water',
    # 'Unknown 11',
    # 'Unknown 12',
    # 'Unknown 13',
    # 'Unknown 14',
    # 'Unknown 15',
    # 'Unknown 16'
    NavmTrianglesFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'unknown1'),
            (1, 'unknown2'),
            (2, 'unknown3'),
            (3, 'unknown4'),
            (4, 'unknown5'),
            (5, 'unknown6'),
            (6, 'preferredpathing'),
            (7, 'unknown8'),
            (8, 'unknown9'),
            (9, 'water'),
            (10, 'unknown11'),
            (11, 'unknown12'),
            (12, 'unknown13'),
            (13, 'unknown14'),
            (14, 'unknown15'),
            (15, 'unknown16'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        # NVNM, ONAM, PNAM, NNAM would need special routines to handle them
        # If no mitigation is needed, then leave it as MelBase
        MelBase('NVNM','navMeshGeometry'),
        MelBase('ONAM','onam_p'),
        MelBase('PNAM','pnam_p'),
        MelBase('NNAM','nnam_p'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreExpl(MelRecord):
    """Explosion record."""
    classType = 'EXPL'

    ExplTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'unknown1'),
        (1, 'alwaysUsesWorldOrientation'),
        (2, 'knockDownAlways'),
        (3, 'knockDownByFormular'),
        (4, 'ignoreLosCheck'),
        (5, 'pushExplosionSourceRefOnly'),
        (6, 'ignoreImageSpaceSwap'),
        (7, 'chain'),
        (8, 'noControllerVibration'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelString('FULL','full'),
        MelModel(),
        MelFid('EITM','objectEffect'),
        MelFid('MNAM','imageSpaceModifier'),
        MelStruct('DATA','6I5f2I',(FID,'light',None),(FID,'sound1',None),(FID,'sound2',None),
                  (FID,'impactDataSet',None),(FID,'placedObject',None),(FID,'spawnProjectile',None),
                  'force','damage','radius','isRadius','verticalOffsetMult',
                  (ExplTypeFlags,'flags',0L),'soundLevel',
            ),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreDebr(MelRecord):
    """Debris record."""
    classType = 'DEBR'

    ExplTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'hasCollissionData'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelGroups('models',
            MelStruct('DATA','BsB','percentage','modelFilename',(ExplTypeFlags,'flags',0L),),
            MelBase('MODT','modt_p'),
        ),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreImgs(MelRecord):
    """Imgs Item"""
    classType = 'IMGS'

    FlstDofFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (16384, 'radius0'),
            (16672, 'radius1'),
            (16784, 'radius2'),
            (16848, 'radius3'),
            (16904, 'radius4'),
            (16936, 'radius5'),
            (16968, 'radius6'),
            (17000, 'radius7'),
            (16576, 'noSkyRadius0'),
            (16736, 'noSkyRadius1'),
            (16816, 'noSkyRadius2'),
            (16880, 'noSkyRadius3'),
            (16920, 'noSkyRadius4'),
            (16952, 'noSkyRadius5'),
            (16984, 'noSkyRadius6'),
            (17016, 'noSkyRadius7'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBase('ENAM','eman_p'),
        MelStruct('HNAM','9f','eyeAdaptSpeed','bloomBlurRadius','bloomThreshold','bloomScale',
                  'receiveBloomThreshold','white','sunlightScale','skyScale',
                  'eyeAdaptStrength',),
        MelStruct('CNAM','3f','Saturation','Brightness','Contrast',),
        MelStruct('TNAM','4f','tintAmount','tintRed','tintGreen','tintBlue',),
        MelStruct('DNAM','3f2sH','dofStrength','dofDistance','dofRange',
                  (FlstDofFlags,'flags',0L),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# IMAD ------------------------------------------------------------------------
#------------------------------------------------------------------------------
class MreFlst(MelRecord):
    """Flst Item"""
    classType = 'FLST'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFids('LNAM','formIDInList'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MrePerk(MelRecord):
    """Perk Item"""
    classType = 'PERK'

    PerkEntryPointFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'calculateWeaponDamage'),
            (1, 'calculateMyCriticalHitChance'),
            (2, 'calculateMyCriticalHitDamage'),
            (3, 'calculateMineExplodeChance'),
            (4, 'adjustLimbDamage'),
            (5, 'adjustBookSkillPoints'),
            (6, 'modRecoveredHealth'),
            (7, 'getShouldAttack'),
            (8, 'modBuyPrices'),
            (9, 'addLeveledListOnDeath'),
            (10, 'getMaxCarryWeight'),
            (11, 'modAddictionChance'),
            (12, 'modAddictionDuration'),
            (13, 'modPositiveChemDuration'),
            (14, 'activate'),
            (15, 'ignoreRunningDuringDetection'),
            (16, 'ignoreBrokenLock'),
            (17, 'modEnemyCriticalHitChance'),
            (18, 'modSneakAttackMult'),
            (19, 'modMaxPlaceableMines'),
            (20, 'modBowZoom'),
            (21, 'modRecoverArrowChance'),
            (22, 'modSkillUse'),
            (23, 'modTelekinesisDistance'),
            (24, 'modTelekinesisDamageMult'),
            (25, 'modTelekinesisDamage'),
            (26, 'modBashingDamage'),
            (27, 'modPowerAttackStamina'),
            (28, 'modPowerAttackDamage'),
            (29, 'modSpellMagnitude'),
            (30, 'modSpellDuration'),
            (31, 'modSecondaryValueWeight'),
            (32, 'modArmorWeight'),
            (33, 'modIncomingStagger'),
            (34, 'modTargetStagger'),
            (35, 'modAttackDamage'),
            (36, 'modIncomingDamage'),
            (37, 'modTargetDamageResistance'),
            (38, 'modSpellCost'),
            (39, 'modPercentBlocked'),
            (40, 'modShieldDeflectArrowChance'),
            (41, 'modIncomingSpellMagnitude'),
            (42, 'modIncomingSpellDuration'),
            (43, 'modPlayerIntimidation'),
            (44, 'modPlayerReputation'),
            (45, 'modFavorPoints'),
            (46, 'modBribeAmount'),
            (47, 'modDetectionLight'),
            (48, 'modDetectionMovement'),
            (49, 'modSoulGemRecharge'),
            (50, 'setSweepAttack'),
            (51, 'applyCombatHitSpell'),
            (52, 'applyBashingSpell'),
            (53, 'applyReanimateSpell'),
            (54, 'setBooleanGraphVariable'),
            (55, 'modSpellCastingSoundEvent'),
            (56, 'modPickpocketChance'),
            (57, 'modDetectionSneakSkill'),
            (58, 'modFallingDamage'),
            (59, 'modLockpickSweetSpot'),
            (60, 'modSellPrices'),
            (61, 'canPickpocketEquippedItem'),
            (62, 'modLockpickLevelAllowed'),
            (63, 'setLockpickStartingArc'),
            (64, 'setProgressionPicking'),
            (65, 'makeLockpicksUnbreakable'),
            (66, 'modAlchemyEffectiveness'),
            (67, 'applyWeaponSwingSpell'),
            (68, 'modCommandedActorLimit'),
            (69, 'applySneakingSpell'),
            (70, 'modPlayerMagicSlowdown'),
            (71, 'modWardMagickaAbsorptionPct'),
            (72, 'modInitialIngredientEffectsLearned'),
            (73, 'purifyAlchemyIngredients'),
            (74, 'filterActivation'),
            (75, 'canDualCastSpell'),
            (76, 'modTemperingHealth'),
            (77, 'modEnchantmentPower'),
            (78, 'modSoulPctCapturedtoWeapon'),
            (79, 'modSoulGemEnchanting'),
            (80, 'mod#AppliedEnchantmentsAllowed'),
            (81, 'setActivateLabel'),
            (82, 'modShoutOK'),
            (83, 'modPoisonDoseCount'),
            (84, 'shouldApplyPlacedItem'),
            (85, 'modArmorRating'),
            (86, 'modLockpickingCrimeChance'),
            (87, 'modIngredientsHarvested'),
            (88, 'modSpellRangeTargetLoc.'),
            (89, 'modPotionsCreated'),
            (90, 'modLockpickingKeyRewardChance'),
            (91, 'allowMountActor'),
        ))

    # 'Run Immediately',
    # 'Replace Default'
    PerkScriptFlagsFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'runImmediately'),
            (1, 'replaceDefault'),
        ))
    
    # {0} 'None',
    # {1} 'Float',
    # {2} 'Float/AV,Float',
    # {3} 'LVLI',
    # {4} 'SPEL,lstring,flags',
    # {5} 'SPEL',
    # {6} 'string',
    # {7} 'lstring'
    PerkFunctionParameterFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'none'),
            (1, 'float'),
            (2, 'float/AV,Float'),
            (3, 'lVLI'),
            (4, 'sPEL,lstring,flags'),
            (5, 'sPEL'),
            (6, 'string'),
            (7, 'lstring'),
        ))
    
    # {0} 'Unknown 0',
    # {1} 'Set Value',  // EPFT=1
    # {2} 'Add Value', // EPFT=1
    # {3} 'Multiply Value', // EPFT=1
    # {4} 'Add Range To Value', // EPFT=2
    # {5} 'Add Actor Value Mult', // EPFT=2
    # {6} 'Absolute Value', // no params
    # {7} 'Negative Absolute Value', // no params
    # {8} 'Add Leveled List', // EPFT=3
    # {9} 'Add Activate Choice', // EPFT=4
    # {10} 'Select Spell', // EPFT=5
    # {11} 'Select Text', // EPFT=6
    # {12} 'Set to Actor Value Mult', // EPFT=2
    # {13} 'Multiply Actor Value Mult', // EPFT=2
    # {14} 'Multiply 1 + Actor Value Mult', // EPFT=2
    # {15} 'Set Text' // EPFT=7
    PerkFunctionFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'unknown0'),
            (1, 'setValue'),
            (2, 'addValue'),
            (3, 'multiplyValue'),
            (4, 'addRangeToValue'),
            (5, 'addActorValueMult'),
            (6, 'absoluteValue'),
            (7, 'negativeAbsoluteValue'),
            (8, 'addLeveledList'),
            (9, 'addActivateChoice'),
            (10, 'selectSpell'),
            (11, 'selectText'),
            (12, 'settoActorValueMult'),
            (13, 'multiplyActorValueMult'),
            (14, 'multiply1+ActorValueMult'),
            (15, 'setText'),
        ))
    
    # 'Quest + Stage',
    # 'Ability',
    # 'Entry Point'
    PerkEffectTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'quest+Stage'),
            (1, 'ability'),
            (2, 'entryPoint'),
        ))
    
    PerkHiddenFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'false'),
            (1, 'true'),
        ))
    
    PerkPlayableFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'false'),
            (1, 'true'),
        ))
    
    PerkTraitFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'false'),
            (1, 'true'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelLString('FULL','full'),
        MelLString('DESC','description'),
        MelIcons(),
        MelConditions(),
        MelStruct('DATA','5B',(PerkTraitFlags,'trait',0L),'level','numRanks',
                  (PerkPlayableFlags,'playable',0L),(PerkHiddenFlags,'hidden',0L),),
        MelFid('NNAM','nextPerk',),

        # Sorted Struct: wbRStructsSK('Effects', 'Effect', [0, 1], [
        MelGroup('effects',
            MelStruct('PRKE','3B',(PerkEffectTypeFlags,'type',0L),'rank','priority',),
            # Needs Union Decider: wbUnion(DATA, 'Effect Data', wbPerkDATADecider, [
            # 1- MelStruct('DATA','IB3s',(FID,'quest'),'questStage','unused',),
            # 2- MelFid('DATA','ability',),
            # 3- MelStruct('DATA','3B',(PerkEntryPointFlags,'entryPoint',0L),(PerkFunctionFlags,'function',0L),
            #          'perkConditionTabCount',),
            MelBase('DATA','effectData',),     
        ),

        # Sorted Struct: wbRStructsSK('Perk Conditions', 'Perk Condition', [0], [
        MelGroup('perkConditions',
            MelStruct('PRKC','I','runOnTabIndex'),
            MelConditions(),
        ),

        MelGroup('functionParameters',
            MelStruct('EPFT','I',(PerkFunctionParameterFlags,'flags',0L),),
            # EPF2 is a Null terminated string with no length Byte
            MelLString('EPF2','buttonLabel'),
            MelStruct('EPF3','B3s',(PerkScriptFlagsFlags,'flags',0L),'unknown',),

            # case(EPFT) of
            # 1: EPFD=float
            # 2: EPFD=float,float
            # 3: EPFD=LVLI
            # 4: EPFD=SPEL, EPF2=lstring, EPF3=int32 flags
            # 5: EPFD=SPEL
            # 6: EPFD=string
            # 7: EPFD=lstring
            # Needs Union Decider: wbUnion(EPFD, 'Data', wbEPFDDecider, [
            # The following variables are not duplicated, they need to be these names
            # 1- MelBase('EPFD','unknown'),
            # 2- MelStruct('EPFD','f','oneFloat',),
            # 3- MelStruct('EPFD','2f','float1','float2',),
            # 4- MelFid('EPFD','I','leveledItem',),
            # 5- MelFid('EPFD','I','spell',),
            # 6- MelFid('EPFD','I','spell',),
            # 7- MelString('EPFD','text'),
            # The following 'Text' is a Null terminated string with no length Byte
            # 8- MelLString('EPFD','text'),
            # 9- MelStruct('EPFD','If','actorValue','float',),
            MelBase('EPFD','functionParametersData',),     
        ),
        # End Marker
        MelNull('PRKF'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreBptd(MelRecord):
    """Bptd Item"""
    classType = 'BPTD'

    BptdDamageFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'severable'),
            (1, 'ikData'),
            (2, 'ikDataBipedData'),
            (3, 'explodable'),
            (4, 'ikDataIsHead'),
            (5, 'ikDataHeadtracking'),
            (6, 'toHitChanceAbsolute'),
        ))

    BptdPartTypes = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'torso'),
            (1, 'head'),
            (2, 'eye'),
            (3, 'lookAt'),
            (4, 'flyGrab'),
            (5, 'saddle'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelLString('BPTN','partName'),
        MelString('PNAM','poseMatching'),
        MelString('BPNN','partNode'),
        MelString('BPNT','vatsTarget'),
        MelString('BPNI','startNode'),
        MelStruct('BPND','f3Bb2BH2I2f3I7f2I2BHf','damageMult',(BptdDamageFlags,'flags',0L),
            (BptdPartTypes,'flags',0L),'healthPcnt','actorValue','toHitChance',
            'explodableExplosionChancePcnt','explodableDebrisCount',(FID,'explodableDebris'),
            (FID,'explodableExplosion'),'trackingMaxAngle','explodableDebrisScale',
            'severableDebrisCount',(FID,'severableDebris'),(FID,'severableExplosion'),
            'severableDebrisScale','transx','transy','transz','rotx','roty','rotz',
            (FID,'severableImpactDataset'),(FID,'explodableImpactDataset'),'severableDecalCount',
            'explodableDecalCount','unknown','limbReplacementScale',
        ),
        MelString('NAM1','limbReplacementModel'),
        MelString('NAM4','goreEffectsTargetBone'),
        MelBase('NAM5','textFileHashes'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAddn(MelRecord):
    """Addon"""
    classType = 'ADDN'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelModel(),
        MelBase('DATA','data_p'),
        MelOptStruct('SNAM','I',(FID,'ambientSound')),
        MelBase('DNAM','addnFlags'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAvif(MelRecord):
    """ActorValue Information record."""
    classType = 'AVIF'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelString('DESC','description'),
        MelString('ANAM','abbreviation'),
        MelBase('CNAM','cnam_p'),
        MelStruct('AVSK','4f','skillUseMult','skillOffsetMult','skillImproveMult','skillImproveOffset',),
        MelGroups('perkTree',
            MelFid('PNAM', 'perk',),
            MelBase('FNAM','fnam_p'),
            MelStruct('XNAM','I','perkGridX'),
            MelStruct('YNAM','I','perkGridY'),
            MelStruct('HNAM','f','horizontalPosition'),
            MelStruct('VNAM','f','verticalPosition'),
            MelFid('SNAM','associatedSkill',),
            MelStructs('CNAM','I','connections','lineToIndex',),
            MelStruct('INAM','I','index',),
        ),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreCams(MelRecord):
    """Cams Type"""
    classType = 'CAMS'

    CamsActionFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'actionShoot'),
            (1, 'actionFly'),
            (2, 'actionHit'),
            (3, 'actionZoom'),
        ))

    CamsLocationFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'locationAttacker'),
            (1, 'locationProjectile'),
            (2, 'locationTarget'),
            (3, 'locationLeadActor'),
        ))

    CamsTargetFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'targetAttacker'),
            (1, 'targetProjectile'),
            (2, 'targetTarget'),
            (3, 'targetLeadActor'),
        ))

    CamsFlagsFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'positionFollowsLocation'),
            (1, 'rotationFollowsTarget'),
            (2, 'dontFollowBone'),
            (3, 'firstPersonCamera'),
            (4, 'noTracer'),
            (5, 'startAtTimeZero'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelStruct('SNAM','4I7f',(CamsActionFlags,'flags',0L),(CamsLocationFlags,'flags',0L),
                  (CamsTargetFlags,'flags',0L),(CamsFlagsFlags,'flags',0L),'timeMultPlayer',
                  'timeMultTarget','timeMultGlobal','maxTime','minTime','targetPctBetweenActors',
                  'nearTargetDistance',),
        MelFid('MNAM','imageSpaceModifier',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()
#------------------------------------------------------------------------------
class MreCpth(MelRecord):
    """Camera Path"""
    classType = 'CPTH'

    # mustHaveCS = Must Have Camera Shots
    CameraZoomFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'mustHaveCSDefault'),
        (1, 'mustHaveCSDisable'),
        (2, 'mustHaveCSShotList'),
        (128, 'default'),
        (129, 'disable'),
        (130, 'shotList'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelConditions(),
        MelFids('ANAM','relatedCameraPaths',),
        MelStruct('DATA','B',(CameraZoomFlags,'cameraZoom',0L),),
        MelFids('SNAM','cameraShots',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreVtyp(MelRecord):
    """Vtyp Item"""
    classType = 'VTYP'

    # {0x00} 'Magic Casting',
    # {0x01} Not Used
    # {0x02} 'Allow Default Dialog',
    # {0x04} 'Female',
    VtypTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'male'),
            (1, 'allowDefaultDialog'),
            (2, 'female'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DNAM','B',(VtypTypeFlags,'flags',0L),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMatt(MelRecord):
    """Material Type Record."""
    classType = 'MATT'

    MattTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'stairMaterial'),
            (1, 'arrowsStick'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('PNAM', 'materialParent',),
        MelString('MNAM','materialName'),
        MelStruct('CNAM','3f','red','green','blue',),
        MelStruct('BNAM','f','buoyancy',),
        MelStruct('FNAM','I',(MattTypeFlags,'flags',0L),),
        MelFid('HNAM', 'havokImpactDataSet',),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreIpct(MelRecord):
    """Impact record."""
    classType = 'IPCT'

    IpctEffectOrientation = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'Surface Normal'),
            (1, 'Projectile Vector'),
            (2, 'Projectile Reflection'),
        ))

    IpctTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'No Decal Data'),
        ))

    IpctResultFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'Default'),
            (1, 'Destroy'),
            (2, 'Bounce'),
            (3, 'Impale'),
            (4, 'Stick'),
        ))

    IpctSoundLevel = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'Loud'),
            (1, 'Normal'),
            (2, 'Silent'),
            (3, 'Very Loud'),
        ))

    IpctParaTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'parallax'),
        (1, 'alphaBlending'),
        (2, 'alphaTesting'),
        (3, 'noSubtextures'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelStruct('DATA','fIffIBBH','effectDuration',(IpctEffectOrientation,'orientationFlags',0L),
                  'angleThreshold','placementRadius', (IpctSoundLevel,'soundLevel',0L),
                  (IpctTypeFlags,'flags',0L),(IpctResultFlags,'resultFlags',0L),'unknown1'
                  ),
        MelStruct('DODT','7f8B','minWidth','maxWidth','minHeight','maxHeight',
                  'depth','shininess','parallaxScale','parallaxPasses',(IpctParaTypeFlags,'para_flags',0L),
                  'unknown_01','unknown_02','red','green','blue','unused',),
        MelFid('DNAM','textureSet'),
        MelFid('ENAM','secondarytextureSet'),
        MelFid('SNAM','sound1'),
        MelFid('NAM1','sound2'),
        MelFid('NAM2','hazard'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreIpds(MelRecord):
    """Ipds Item"""
    classType = 'IPDS'
    melSet = MelSet(
        MelString('EDID','eid'),
        # This is a repeating subrecord of 8 bytes, 2 FormIDs First is MATT second is IPCT
        MelGroups('data',
            MelStruct('PNAM','2I',(FID,'material'), (FID,'impact')),
            ),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreEczn(MelRecord):
    """Encounter Zone record."""
    classType = 'ECZN'

    EcznTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'neverResets'),
            (1, 'matchPCBelowMinimumLevel'),
            (2, 'disableCombatBoundary'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DATA','2I2bBb',(FID,'owner',None),(FID,'location',None),'rank','minimumLevel',
                  (EcznTypeFlags,'flags',0L),('maxLevel',null1)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreLctn(MelRecord):
    """Location"""
    classType = 'LCTN'

    melSet = MelSet(
        MelString('EDID','eid'),

        MelGroups('actorCellPersistentReference',
            MelStruct('ACPR','2I2h',(FID,'actor'),(FID,'location'),'gridX','gridY',),
            ),
        MelGroups('locationCellPersistentReference',
            MelStruct('LCPR','2I2h',(FID,'actor'),(FID,'location'),'gridX','gridY',),
            ),
        # From Danwguard.esm, Does not follow similar previous patterns
        MelFidList('RCPR','referenceCellPersistentReference',),

        MelGroups('actorCellUnique',
            MelStruct('ACUN','3I',(FID,'actor'),(FID,'eef'),(FID,'location'),),
            ),
        MelGroups('locationCellUnique',
            MelStruct('LCUN','3I',(FID,'actor'),(FID,'ref'),(FID,'location'),),
            ),
        # in Unofficial Skyrim patch
        MelFidList('RCUN','referenceCellUnique',),

        MelGroups('actorCellStaticReference',
            MelStruct('ACSR','3I2h',(FID,'locRefType'),(FID,'marker'),(FID,'location'),
                  'gridX','gridY',),
            ),
        MelGroups('locationCellEncounterCell',
            MelStruct('LCSR','3I2h',(FID,'locRefType'),(FID,'marker'),(FID,'location'),
                  'gridX','gridY',),
            ),
        # Seen in Open Cities
        MelFidList('RCSR','referenceCellStaticReference',),

        # MelStruct('ACEC','I',(FID,'Actor'), RepeatingArray('2h','gridX','gridY',)),
        MelBase('ACEC','actorCellEncounterCell',),
        # MelStruct('LCEC','I',(FID,'Actor'), RepeatingArray('2h','gridX','gridY',)),
        MelBase('LCEC','locationCellEncounterCell',),
        # MelStruct('ACEC','I',(FID,'Actor'), RepeatingArray('2h','gridX','gridY',)),
        # Seen in Open Cities
        MelBase('RCEC','referenceCellEncounterCell',),

        MelFidList('ACID','actorCellMarkerReference',),
        MelFidList('LCID','locationCellMarkerReference',),

        MelGroups('actorCellEnablePoint',
            MelStruct('ACEP','2I2h',(FID,'Actor'),(FID,'Ref'),'gridX','gridY',),
            ),
        MelGroups('locationCellEnablePoint',
            MelStruct('LCEP','2I2h',(FID,'Actor'),(FID,'Ref'),'gridX','gridY',),
            ),

        MelLString('FULL','full'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelFid('PNAM','parentLocation',),
        MelFid('NAM1','music',),
        MelFid('FNAM','unreportedCrimeFaction',),
        MelFid('MNAM','worldLocationMarkerRef',),
        MelStruct('RNAM','f','worldLocationRadius',),
        MelFid('NAM0','horseMarkerRef',),
        MelColorN(),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMesg(MelRecord):
    """Message Record."""
    classType = 'MESG'

    MesgTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'messageBox'),
            (1, 'autoDisplay'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('DESC','description'),
        MelString('FULL','full'),
        # 'INAM' leftover
        MelFid('INAM','iconUnused'),
        MelFid('QNAM','materialParent'),
        MelStruct('DNAM','I',(MesgTypeFlags,'flags',0L),),
        # Don't Show
        MelStruct('TNAM','I','displayTime',),
        MelGroups('menuButtons',
        	MelLString('ITXT','buttonText'),
        	MelConditions(),
        	),
    )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreDobj(MelRecord):
    """Default Object Manager"""
    classType = 'DOBJ'
    melSet = MelSet(
        MelGroups('objects',
            MelStruct('DNAM','4sI','objectUse',(FID,'objectID',None),),
            ),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MelLgtmData(MelStruct):
    def __init__(self,type='DALC'):
        MelStruct.__init__(self,type,'=4B4B4B4B4B4B4Bf',
                           'red','green','blue','unknown', # 'X+'
                           'red','green','blue','unknown', # 'X-'
                           'red','green','blue','unknown', # 'Y+'
                           'red','green','blue','unknown', # 'Y-'
                           'red','green','blue','unknown', # 'Z+'
                           'red','green','blue','unknown', # 'Z-'
                           'red','green','blue','unknown', # Specular Color Values
                           'fresnelPower' # Fresnel Power
                           )

class MreLgtm(MelRecord):
    """Lgtm Item"""
    classType = 'LGTM'

    melSet = MelSet(
        MelString('EDID','eid'),
        # 92 Bytes
        MelStruct('DATA','4B4B4B2f2i3f8I4B3fI',
            'red','green','blue','unknown',
            'red','green','blue','unknown',
            'red','green','blue','unknown',
            'fogNear','fogFar',
            'dirRotXY','dirRotZ',
            'directionalFade','fogClipDist','fogPower',
            'unknown','unknown','unknown','unknown',
            'unknown','unknown','unknown','unknown',
            'red','green','blue','unknown',
            'fogMax',
            'lightFaceStart','lightFadeEnd',
            'unknown',),
        # 32 Bytes
        MelLgtmData(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# If Syntax Correct, Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMusc(MelRecord):
    """Music type record."""
    classType = 'MUSC'

    MuscTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0,'playsOneSelection'),
            (1,'abruptTransition'),
            (2,'cycleTracks'),
            (3,'maintainTrackOrder'),
            (4,'unknown5'),
            (5,'ducksCurrentTrack'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('FNAM','I',(MuscTypeFlags,'flags',0L),),
        # Divided by 100 in TES5Edit, probably for editing only
        MelStruct('PNAM','2H','priority','duckingDB'),
        MelStruct('WNAM','f','fadeDuration'),
        MelFids('TNAM','musicTracks'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# If Syntax Correct, Verified Correct for Skyrim 1.8
# Need to check if TNAM can have more then one FormID if so MelFidList
#------------------------------------------------------------------------------
class MreFstp(MelRecord):
    """Footstep"""
    classType = 'FSTP'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('DATA','impactSet'),
        MelString('ANAM','tag'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreFsts(MelRecord):
    """Footstep Set."""
    classType = 'FSTS'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('XCNT','5I','walkForward','runForward','walkForwardAlt',
                  'runForwardAlt','walkForwardAlternate2',
            ),
        MelFids('DATA','footstepSets'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
# Need to check if DATA can have more then one FormID if so MelFidList
#------------------------------------------------------------------------------
class MreSmbn(MelRecord):
    """Story Manager Branch Node"""
    classType = 'SMBN'

    SmbnNodeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'Random'),
        (1,'noChildWarn'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('PNAM','parent',),
        MelFid('SNAM','child',),
        MelStruct('CITC','I','conditionCount'),
        MelConditions(),
        MelStruct('DNAM','I',(SmbnNodeFlags,'nodeFlags',0L),),
        MelBase('XNAM','xnam_p'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreSmqn(MelRecord):
    """Story Manager Quest Node"""
    classType = 'SMQN'

    # "Do all" = "Do all before repeating"
    SmqnQuestFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'doAll'),
        (1,'sharesEvent'),
        (2,'numQuestsToRun'),
    ))

    SmqnNodeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'Random'),
        (1,'noChildWarn'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('PNAM','parent',),
        MelFid('SNAM','child',),
        MelStruct('CITC','I','conditionCount'),
        MelConditions(),
        MelStruct('DNAM','2H',(SmqnNodeFlags,'nodeFlags',0L),(SmqnQuestFlags,'questFlags',0L),),
        MelStruct('XNAM','I','maxConcurrentQuests'),
        MelStruct('MNAM','I','numQuestsToRun'),
        MelStruct('QNAM','I','questCount'),
        MelGroups('quests',
            MelFid('NNAM','quest',),
            MelBase('FNAM','fnam_p'),
            MelStruct('RNAM','f','hoursUntilReset'),
            )
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreSmen(MelRecord):
    """Story Manager Event Node"""
    classType = 'SMEN'

    SmenNodeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'Random'),
        (1,'noChildWarn'),
    ))

    # ENAM is four chars with no length byte, like AIPL, or CHRR
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('PNAM','parent',),
        MelFid('SNAM','child',),
        MelStruct('CITC','I','conditionCount'),
        MelConditions(),
        MelStruct('DNAM','I',(SmenNodeFlags,'nodeFlags',0L),),
        MelBase('XNAM','xnam_p'),
        MelString('ENAM','type'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreDlbr(MelRecord):
    """Dialog Branch"""
    classType = 'DLBR'

    DialogBranchFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'topLevel'),
        (1,'blocking'),
        (2,'exclusive'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('QNAM','quest',),
        MelStruct('TNAM','I','unknown'),
        MelStruct('DNAM','I',(DialogBranchFlags,'flags',0L),),
        MelFid('SNAM','startingTopic',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMust(MelRecord):
    """Music Track"""
    classType = 'MUST'

    TrackTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'topLevel'),
        (1,'blocking'),
        (2,'exclusive'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('CNAM','I',(TrackTypeFlags,'trackType',0L),),
        MelStruct('FLTV','f','duration'),
        MelStruct('DNAM','I','fadeout'),
        MelString('ANAM','trackFilename'),
        MelString('BNAM','finaleFilename'),
        MelStructA('FNAM','f','cuePoints'),
        MelStruct('LNAM','2fI','loopBegins','loopEnds','loopCount',),
        MelStruct('CITC','I','conditionCount'),
        MelConditions(),
        MelFids('SNAM','tracks',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreDlvw(MelRecord):
    """Dialog View"""
    classType = 'DLVW'

    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('QNAM','quest',),
        MelFids('BNAM','branches',),
        MelGroups('unknownTNAM',
        	MelBase('TNAM','unknown',),
        	),
        MelBase('ENAM','unknownENAM'),
        MelBase('DNAM','unknownDNAM'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreWoop(MelRecord):
    """Word of Power"""
    classType = 'WOOP'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        # TNAM is a Null terminated string with no length Byte
        MelString('TNAM','translation'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreShou(MelRecord):
    """Shout Records"""
    classType = 'SHOU'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('FULL','full'),
        MelFids('MDOB','menuDisplayObject'),
        MelString('DESC','description'),
        # Don't sort
        MelGroups('wordsOfPower',
        	MelStruct('SNAM','2If',(FID,'word',None),(FID,'spell',None),'recoveryTime',),
        	),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreEqup(MelRecord):
    """Equp Item"""
    classType = 'EQUP'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFids('PNAM','canBeEquipped'),
        # DATA is either True Of False
        MelStruct('DATA','I','useAllParents'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreRela(MelRecord):
    """Relationship"""
    classType = 'RELA'

    RelationshipFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'Unknown 1'),
        (1,'Unknown 2'),
        (2,'Unknown 3'),
        (3,'Unknown 4'),
        (4,'Unknown 5'),
        (5,'Unknown 6'),
        (6,'Unknown 7'),
        (7,'Secret'),
    ))
    
    RelationshipRank = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'Lover'),
        (1,'Ally'),
        (2,'Confidant'),
        (3,'Friend'),
        (4,'Acquaitance'),
        (5,'Rival'),
        (6,'Foe'),
        (7,'Enemy'),
        (8,'Archnemesis'),
    ))
    
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DATA','IIHBBI',(FID,'parent'),(FID,'child'),(RelationshipRank,'rankFlags',0L),
                  'unknown',(RelationshipFlags,'relaFlags',0L),(FID,'associationType'),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreScen(MelRecord):
    """Scene"""
    classType = 'SCEN'
    
    # {0x00000001} 'Unknown 1',
    # {0x00000002} 'Unknown 2',
    # {0x00000004} 'Unknown 3',
    # {0x00000008} 'Unknown 4',
    # {0x00000010} 'Unknown 5',
    # {0x00000020} 'Unknown 6',
    # {0x00000040} 'Unknown 7',
    # {0x00000080} 'Unknown 8',
    # {0x00000100} 'Unknown 9',
    # {0x00000200} 'Unknown 10',
    # {0x00000400} 'Unknown 11',
    # {0x00000800} 'Unknown 12',
    # {0x00001000} 'Unknown 13',
    # {0x00002000} 'Unknown 14',
    # {0x00003000} 'Unknown 15',
    # {0x00004000} 'Face Target',
    # {0x00010000} 'Looping',
    # {0x00020000} 'Headtrack Player'
    ScenFlags5 = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'unknown1'),
            (1, 'unknown2'),
            (2, 'unknown3'),
            (3, 'unknown4'),
            (4, 'unknown5'),
            (5, 'unknown6'),
            (6, 'unknown7'),
            (7, 'unknown8'),
            (8, 'unknown9'),
            (9, 'unknown10'),
            (10, 'unknown11'),
            (11, 'unknown12'),
            (12, 'unknown13'),
            (13, 'unknown14'),
            (14, 'unknown15'),
            (15, 'faceTarget'),
            (16, 'looping'),
            (17, 'headtrackPlayer'),
        ))
    
    # 'Dialogue',
    # 'Package',
    # 'Timer'
    ScenFlags4 = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'dialogue'),
            (1, 'package'),
            (2, 'timer'),
        ))
    
    # 'Death Pause (unsused)',
    # 'Death End',
    # 'Combat Pause',
    # 'Combat End',
    # 'Dialogue Pause',
    # 'Dialogue End',
    # 'OBS_COM Pause',
    # 'OBS_COM End'
    ScenFlags3 = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'deathPauseunsused'),
            (1, 'deathEnd'),
            (2, 'combatPause'),
            (3, 'combatEnd'),
            (4, 'dialoguePause'),
            (5, 'dialogueEnd'),
            (6, 'oBS_COMPause'),
            (7, 'oBS_COMEnd'),
        ))
    
    # 'No Player Activation',
    # 'Optional'
    ScenFlags2 = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'noPlayerActivation'),
            (1, 'optional'),
        ))
    
    # 'Begin on Quest Start',
    # 'Stop on Quest End',
    # 'Unknown 3',
    # 'Repeat Conditions While True',
    # 'Interruptible'
    ScenFlags1 = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'beginonQuestStart'),
            (1, 'stoponQuestEnd'),
            (2, 'unknown3'),
            (3, 'repeatConditionsWhileTrue'),
            (4, 'interruptible'),
        ))
    
    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelStruct('FNAM','I',(ScenFlags1,'flags',0L),),
        MelGroups('phases',
            MelNull('HNAM'),
            MelString('NAM0','name',),
            MelGroup('startConditions',
                MelConditions(),
                ),
            MelNull('NEXT'),
            MelGroup('completionConditions',
                MelConditions(),
                ),
            # BEGIN leftover from earlier CK versions
            MelGroup('unused',
                MelBase('SCHR','schr_p'),
                MelBase('SCDA','scda_p'),
                MelBase('SCTX','sctx_p'),
                MelBase('QNAM','qnam_p'),
                MelBase('SCRO','scro_p'),
                ),
            MelNull('NEXT'),
            MelGroup('unused',
                MelBase('SCHR','schr_p'),
                MelBase('SCDA','scda_p'),
                MelBase('SCTX','sctx_p'),
                MelBase('QNAM','qnam_p'),
                MelBase('SCRO','scro_p'),
                ),
            # End leftover from earlier CK versions
        MelStruct('WNAM','I','editorWidth',),
        # Marker Phase End
        MelNull('HNAM'),
        ),

        MelGroups('actors',
            MelStruct('ALID','I','actorID',),
            MelStruct('LNAM','I',(ScenFlags2,'flags2',0L),),
            MelStruct('DNAM','I',(ScenFlags3,'flags3',0L),),
            ),
        MelGroups('actions',
            MelStruct('ANAM','H',(ScenFlags4,'flags',0L),),
            MelString('NAM0','name',),
            MelStruct('ALID','I','actorID',),
            MelBase('LNAM','lnam_p',),
            MelStruct('INAM','I','index',),
            MelStruct('FNAM','I',(ScenFlags5,'flags',0L),),
            MelStruct('SNAM','I','startPhase',),
            MelStruct('ENAM','I','endPhase',),
            MelStruct('SNAM','f','timerSeconds',),
            MelFids('PNAM','packages'),
            MelStruct('ALID','I',(FID,'Topic'),),
            MelStruct('ALID','I','headtrackActorID',),
            MelStruct('ALID','I','loopingMax',),
            MelStruct('ALID','I','loopingMin',),
            MelStruct('ALID','I','emotionType',),
            MelStruct('ALID','I','emotionValue',),
            # BEGIN leftover from earlier CK versions
            MelGroup('unused',
                MelBase('SCHR','schr_p'),
                MelBase('SCDA','scda_p'),
                MelBase('SCTX','sctx_p'),
                MelBase('QNAM','qnam_p'),
                MelBase('SCRO','scro_p'),
                ),
            # End leftover from earlier CK versions
            MelNull('ANAM'),
        ),
        # BEGIN leftover from earlier CK versions
        MelGroup('unused',
            MelBase('SCHR','schr_p'),
            MelBase('SCDA','scda_p'),
            MelBase('SCTX','sctx_p'),
            MelBase('QNAM','qnam_p'),
            MelBase('SCRO','scro_p'),
            ),
        MelNull('NEXT'),
        MelGroup('unused',
            MelBase('SCHR','schr_p'),
            MelBase('SCDA','scda_p'),
            MelBase('SCTX','sctx_p'),
            MelBase('QNAM','qnam_p'),
            MelBase('SCRO','scro_p'),
            ),
        # End leftover from earlier CK versions

        MelFid('PNAM','quest',),
        MelStruct('INAM','I','actionCount'),
        MelBase('VNAM','vnam_p'),
        MelConditions(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAstp(MelRecord):
    """Astp record (Association type)"""
    classType = 'ASTP'

    # DATA Flags
    # {0x00000001} 'Related'
    AstpTypeFlags = bolt.Flags(0L,bolt.Flags.getNames('related'))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('MPRT','maleParent'),
        MelString('FPRT','femaleParent'),
        MelString('MCHT','maleChild'),
        MelString('FCHT','femaleChild'),
        MelStruct('DATA','I',(AstpTypeFlags,'flags',0L)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreOtft(MelRecord):
    """Otft Item"""
    classType = 'OTFT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelFidList('INAM','items'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreArto(MelRecord):
    """Arto record (Art effect object)"""
    classType = 'ARTO'

    #{0x00000001} 'Magic Casting',
    #{0x00000002} 'Magic Hit Effect',
    #{0x00000004} 'Enchantment Effect'
    ArtoTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'magic_casting'),
            (1, 'magic_hit_effect'),
            (2, 'enchantment_effect'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelModel(),
        MelStruct('DNAM','I',(ArtoTypeFlags,'flags',0L)),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMato(MelRecord):
    """Material Object Records"""
    classType = 'MATO'

    MatoTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'singlePass'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelModel(),
        MelGroups('wordsOfPower',
        	MelBase('SNAM','propertyData',),
        	),
        MelStruct('DATA','11fI','falloffScale','falloffBias','noiseUVScale',
                  'materialUVScale','projectionVectorX','projectionVectorY',
                  'projectionVectorZ','normalDampener',
                  'singlePassColor','singlePassColor',
                  'singlePassColor',(MatoTypeFlags,'flags',0L),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMovt(MelRecord):
    """Movt Item"""
    classType = 'MOVT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelString('MNAM','mnam_n'),
        MelStruct('SPED','11f','leftWalk','leftRun','rightWalk','rightRun',
                  'forwardWalk','forwardRun','backWalk','backRun',
                  'rotateInPlaceWalk','rotateInPlaceRun',
                  'rotateWhileMovingRun'),
        MelStruct('INAM','3f','directional','movementSpeed','rotationSpeed'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreSndr(MelRecord):
    """Movt Item"""
    classType = 'SNDR'

    # $00 , 'None',
    # $08 , 'Loop',
    # $10 , 'Envelope Fast',
    # $20 , 'Envelope Slow'
    SndrTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'none'),
            (1, 'loop'),
            (2, 'envelopeFast'),
            (3, 'envelopeSlow'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBase('CNAM','cnam_p'),
        MelFid('GNAM','category',),
        MelFid('SNAM','alternateSoundFor',),
        MelGroups('soundFiles',
        	MelString('ANAM','fileName',),
        	),
        MelFid('ONAM','outputModel',),
        MelString('FNAM','string'),
        MelConditions(),
        MelStruct('LNAM','4B','unknown1',(SndrTypeFlags,'looping',0L),'unknown2',
                  'rumbleSendValue',),
        MelStruct('BNAM','4BH','pctFrequencyShift','pctFrequencyVariance','priority',
                  'dbVariance','staticAttenuation',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreDual(MelRecord):
    """Dual Cast Data"""
    classType = 'DUAL'

    DualCastDataFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'hitEffectArt'),
        (1,'projectile'),
        (2,'explosion'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelStruct('DATA','6I',(FID,'projectile'),(FID,'explosion'),(FID,'effectShader'),
                  (FID,'hitEffectArt'),(FID,'impactDataSet'),(DualCastDataFlags,'flags',0L),),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreSnct(MelRecord):
    """Sound Category"""
    classType = 'SNCT'

    SoundCategoryFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'muteWhenSubmerged'),
        (1,'shouldAppearOnMenu'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelStruct('FNAM','I',(SoundCategoryFlags,'flags',0L),),
        MelFid('PNAM','parent',),
        MelStruct('VNAM','H','staticVolumeMultiplier'),
        MelStruct('UNAM','H','defaultMenuValue'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MelSopmData(MelStruct):
    def __init__(self,type='ONAM'):
        MelStruct.__init__(self,type,'=24B',
                           'ch0_l','ch0_r','ch0_c','ch0_lFE','ch0_rL','ch0_rR','ch0_bL','ch0_bR',
                           'ch1_l','ch1_r','ch1_c','ch1_lFE','ch1_rL','ch1_rR','ch1_bL','ch1_bR',
                           'ch2_l','ch2_r','ch2_c','ch2_lFE','ch2_rL','ch2_rR','ch2_bL','ch2_bR',
                           )

class MreSopm(MelRecord):
    """Sound Output Model"""
    classType = 'SOPM'
    
    # 'Uses HRTF',
    # 'Defined Speaker Output'
    SopmTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'usesHRTF'),
            (1, 'definedSpeakerOutput'),
        ))
    
    # 'Attenuates With Distance',
    # 'Allows Rumble'
    SopmFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'attenuatesWithDistance'),
            (1, 'allowsRumble'),
        ))
    
    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('NAM1','B2sB',(SopmFlags,'flags',0L),'unknown','reverbSendpct',),
        MelBase('FNAM','fnam_p'),
        MelStruct('MNAM','I',(SopmTypeFlags,'flags',0L),),
        MelBase('CNAM','cnam_p'),
        MelBase('SNAM','snam_p'),
        MelSopmData(),
        # MelStruct('ANAM','4s2fB','unknown','minDistance','maxDistance','curve', dumpRemaining='extraData',),
        MelBase('ANAM','anam_p'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreColl(MelRecord):
    """Collision Layer"""
    classType = 'COLL'

    CollisionLayerFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'triggerVolume'),
        (1,'sensor'),
        (2,'navmeshObstacle'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        # DESC is a Null terminated string with no length Byte
        MelString('DESC','description'),
        MelStruct('BNAM','I','layerID'),
        MelStruct('FNAM','=4B','red','green','blue','unused'),
        MelStruct('GNAM','I',(CollisionLayerFlags,'flags',0L),), 
        MelString('MNAM','name',),
        MelStruct('INTV','I','interactablesCount'),
        MelFidList('CNAM','collidesWith',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreClfm(MelRecord):
    """Clfm Item"""
    classType = 'CLFM'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelColorN(),
        MelStruct('FNAM','I','playable'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreRevb(MelRecord):
    """Reverb Parameters"""
    classType = 'REVB'

    melSet = MelSet(
        MelString('EDID','eid'),
        MelStruct('DATA','HHbbbbBBBBBB','decayTimeMS','hfReferenceHZ','roomFilter',
                  'hfRoomFilter','reflections','reverbAmp','decayHFRatio',
                  'reflectDelayMS','reverbDelayMS','diffusion','density',
                  'unknown',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreGras(MelRecord):
    """Grass record."""
    classType = 'GRAS'

    GrasTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'vertexLighting'),
            (1, 'uniformScaling'),
            (2, 'fitToSlope'),
        ))

    GrasWaterDistFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'aboveAtLeast'),
        (0,'aboveAtMost'),
        (0,'belowAtLeast'),
        (0,'belowAtMost'),
        (0,'eitherAtLeast'),
        (0,'eitherAtMost'),
        (0,'eitherAtMostAbove'),
        (0,'eitherAtMostBelow'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelModel(),
        MelStruct('DATA','4B2HI4f4B','density','minSlope','maxSlope',
                  ('unused1',null1),'waterDistance',('unused2',null2),
                  (GrasWaterDistFlags,'flags',0L),'posRange',
                  'heightRange','colorRange','wavePeriod',
                  (GrasTypeFlags,'flags',0L),('unused3',null3),('unused4',null3),
                  ('unused5',null3),
                  ),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreIdle(MelRecord):
    """Idle record."""
    classType = 'IDLE'

    IdleTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'parent'),
            (1, 'sequence'),
            (2, 'noAttacking'),
            (3, 'blocking'),
        ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelConditions(),
        MelString('DNAM','filename'),
        MelString('ENAM','animationEvent'),
        # Needs Syntax check.  This is an array of 'ANAM' with two FormIDs
        # 'Parent' and 'Previous ID'
        MelGroups('idleAnimations',
            MelStruct('ANAM','II',(FID,'parent'),(FID,'prevId'),),
            ),
        MelStruct('DATA','4BH','loopMin','loopMax',(IdleTypeFlags,'flags',0L),
                  'animationGroupSection','replayDelay',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Needs Syntax check but otherwise, Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
#------------------------------------------------------------------------------
class MreInfo(MelRecord):
    """Dialog response"""
    classType = 'INFO'
    
    # 'Use Emotion Animation'
    InfoResponsesFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'useEmotionAnimation'),
        ))
    
    # {0} 'Neutral',
    # {1} 'Anger',
    # {2} 'Disgust',
    # {3} 'Fear',
    # {4} 'Sad',
    # {5} 'Happy',
    # {6} 'Surprise',
    # {7} 'Puzzled'
    EmotionTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'neutral'),
            (1, 'anger'),
            (2, 'disgust'),
            (3, 'fear'),
            (4, 'sad'),
            (5, 'happy'),
            (6, 'surprise'),
            (7, 'puzzled'),
        ))
    
    # 'None',
    # 'Small',
    # 'Medium',
    # 'Large'
    FavorLevelFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'none'),
            (1, 'small'),
            (2, 'medium'),
            (3, 'large'),
        ))
    
    # {0x0001} 'Goodbye',
    # {0x0002} 'Random',
    # {0x0004} 'Say once',
    # {0x0008} 'Unknown 4',
    # {0x0010} 'Unknown 5',
    # {0x0020} 'Random end',
    # {0x0040} 'Invisible continue',
    # {0x0080} 'Walk Away',
    # {0x0100} 'Walk Away Invisible in Menu',
    # {0x0200} 'Force subtitle',
    # {0x0400} 'Can move while greeting',
    # {0x0800} 'No LIP File',
    # {0x1000} 'Requires post-processing',
    # {0x2000} 'Audio Output Override',
    # {0x4000} 'Spends favor points',
    # {0x8000} 'Unknown 16'
    EnamResponseFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'goodbye'),
            (1, 'random'),
            (2, 'sayonce'),
            (3, 'unknown4'),
            (4, 'unknown5'),
            (5, 'randomend'),
            (6, 'invisiblecontinue'),
            (7, 'walkAway'),
            (8, 'walkAwayInvisibleinMenu'),
            (9, 'forcesubtitle'),
            (10, 'canmovewhilegreeting'),
            (11, 'noLIPFile'),
            (12, 'requirespostprocessing'),
            (13, 'audioOutputOverride'),
            (14, 'spendsfavorpoints'),
            (15, 'unknown16'),
        ))
    
    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBase('DATA','data_p'),
        MelStruct('ENAM','2H',(EnamResponseFlags,'flags',0L),'resetHours',),
        MelFid('TPIC','topic',),
        MelFid('PNAM','previousINFO',),
        MelStruct('CNAM','I',(FavorLevelFlags,'flags',0L),),
        MelFids('TCLT','response',),
        MelFid('DNAM','responseData',),
        # {>>> Unordered, CTDA can appear before or after LNAM <- REQUIRES CONFIRMATION <<<}
        MelGroups('responses',
            MelStruct('TRDT','II4sB3sIB3s',(EmotionTypeFlags,'flags',0L),'emotionValue',
                      'unused','responsenumber','unused',(FID,'sound'),
                      (InfoResponsesFlags,'flags',0L),'unused',),   
            MelString('NAM1','responseText'),
            MelString('NAM2','scriptNotes'),
            MelString('NAM3','edits'),
            MelFid('SNAM','idleAnimations:Speaker',),
            MelFid('LNAM','idleAnimations:Listener',),
            ),

        MelConditions(),

        MelGroups('responses',
            MelBase('SCHR','unknown1'),
            MelFid('QNAM','unknown2'),
            MelNull('NEXT'),
            ),
        MelLString('RNAM','prompt'),
        MelFid('ANAM','speaker',),
        MelFid('TWAT','walkAwayTopic',),
        MelFid('ONAM','audioOutputOverride',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreIngr(MelRecord):
    """INGR (ingredient) record."""
    classType = 'INGR'

    IngrTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'No auto-calculation'),
            (1, 'Food item'),
            (2, 'Unknown 3'),
            (3, 'Unknown 4'),
            (4, 'Unknown 5'),
            (5, 'Unknown 6'),
            (6, 'Unknown 7'),
            (7, 'Unknown 8'),
            (8, 'References Persist'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelString('FULL','full'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelModel(),
        MelIcons(),
        MelFid('ETYP','equipmentType',),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        MelStruct('DATA','if','value','weight'),
        MelStruct('ENIT','iI','ingrValue',(IngrTypeFlags,'flags',0L),),
        MelEffects(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreKeym(MelRecord):
    """KEYM Key records."""
    classType = 'KEYM'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelString('FULL','full'),
        MelModel(),
        MelIcons(),
        MelDestructible(),
        MelFid('SCRI','script'),
        MelFid('YNAM','soundPickUp'),
        MelFid('ZNAM','soundDrop'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelStruct('DATA','if','value','weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
# Commented out for performance reasons. Slows down loading quite a bit.
# If Bash ever wants to be able to add masters to a mod, this minimal definition is required
# It has to be able to convert the formIDs found in BTXT, ATXT, and VTEX to not break the mod
#class MreLand(MelRecord):
#    """Land structure. Part of exterior cells."""
#    ####Could probably be loaded via MelStructA,
#    ####but little point since it is too complex to manipulate
#    classType = 'LAND'
#    melSet = MelSet(
#        MelBase('DATA','data_p'),
#        MelBase('VNML','normals_p'),
#        MelBase('VHGT','heights_p'),
#        MelBase('VCLR','vertexColors_p'),
#        MelStructs('BTXT','IBBh','baseTextures', (FID,'texture'), 'quadrant', 'unused1', 'layer'),
#        MelGroups('alphaLayers',
#            MelStruct('ATXT','IBBh',(FID,'texture'), 'quadrant', 'unused1', 'layer'),
#            MelStructA('VTXT','H2Bf', 'opacities', 'position', 'unused1', 'opacity'),
#        ),
#        MelFidList('VTEX','vertexTextures'),
#    )
#    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()
# --
# Taken from Wrye Flash for FNV, Needs update for Skyrim
#------------------------------------------------------------------------------
class MreLigh(MelRecord):
    """Light"""
    classType = 'LIGH'
    
    # {0x00000001} 'Dynamic',
    # {0x00000002} 'Can be Carried',
    # {0x00000004} 'Negative',
    # {0x00000008} 'Flicker',
    # {0x00000010} 'Unknown',
    # {0x00000020} 'Off By Default',
    # {0x00000040} 'Flicker Slow',
    # {0x00000080} 'Pulse',
    # {0x00000100} 'Pulse Slow',
    # {0x00000200} 'Spot Light',
    # {0x00000400} 'Shadow Spotlight',
    # {0x00000800} 'Shadow Hemisphere',
    # {0x00001000} 'Shadow Omnidirectional',
    # {0x00002000} 'Portal-strict'
    LighTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'dynamic'),
            (1, 'canbeCarried'),
            (2, 'negative'),
            (3, 'flicker'),
            (4, 'unknown'),
            (5, 'offByDefault'),
            (6, 'flickerSlow'),
            (7, 'pulse'),
            (8, 'pulseSlow'),
            (9, 'spotLight'),
            (10, 'shadowSpotlight'),
            (11, 'shadowHemisphere'),
            (12, 'shadowOmnidirectional'),
            (13, 'portalstrict'),
        ))
    
    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelModel(),
        MelDestructible(),
        MelLString('FULL','full'),
        MelIcons(),
        MelStruct('DATA','iI4BI6fIf','time','radius',
                  'red','green','blue','unknown',
                  (LighTypeFlags,'flags',0L),'falloffExponent','fOV','nearClip',
                  'fePeriod','feIntensityAmplitude','feMovementAmplitude',
                  'value','weight',),
        MelStruct('FNAM','f','fadevalue',),
        MelFid('SNAM','sound',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreLscr(MelRecord):
    """Load screen."""
    classType = 'LSCR'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelIcons(),
        MelString('DESC','text'),
        MelConditions(),
        MelFid('NNAM','loadingScreenNIF'),
        MelStruct('SNAM','f','initialScale',),
        MelStruct('RNAM','3h','rotGridY','rotGridX''rotGridZ',),
        MelStruct('ONAM','2h','rotOffsetMin','rotOffsetMax',),
        MelStruct('XNAM','3h','transGridY','transGridX''transGridZ',),
        MelString('MOD2','cameraPath'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreLtex(MelRecord):
    """Landscape Texture."""
    classType = 'LTEX'

    melSet = MelSet(
        MelString('EDID','eid'),
        MelFid('TNAM','textureSet',),
        MelFid('MNAM','materialType',),
        MelStruct('HNAM','BB','friction','restitution',),
        MelStruct('SNAM','BB','textureSpecularExponent',),
        MelFids('GNAM','grasses'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreLeveledList(MreLeveledListBase):
    """Skryim Leveled item/creature/spell list."""

    class MelLevListLvlo(MelGroups):
        def __init__(self):
            MelGroups.__init__(self,'entries',
                MelStruct('LVLO','=3I','level',(FID,'listId',None),('count',1)),
                MelCoed(),
                )
        def dumpData(self,record,out):
            out.packSub('LLCT','B',len(record.entries))
            MelGroups.dumpData(self,record,out)

    __slots__ = MreLeveledListBase.__slots__

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreLvln(MreLeveledList):
    classType = 'LVLN'
    copyAttrs = ('chanceNone','model','modt_p',)

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelStruct('LVLD','B','chanceNone'),
        MelStruct('LVLF','B',(MreLeveledListBase._flags,'flags',0L)),
        MelOptStruct('LVLG','I',(FID,'glob')),
        MelNull('LLCT'),
        MreLeveledList.MelLevListLvlo(),
        MelString('MODL','model'),
        MelBase('MODT','modt_p'),
        )
    __slots__ = MreLeveledList.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreLvli(MreLeveledList):
    classType = 'LVLI'
    copyAttrs = ('chanceNone','glob',)

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelStruct('LVLD','B','chanceNone'),
        MelStruct('LVLF','B',(MreLeveledListBase._flags,'flags',0L)),
        MelOptStruct('LVLG','I',(FID,'glob')),
        MelNull('LLCT'),
        MreLeveledList.MelLevListLvlo(),
        )
    __slots__ = MreLeveledList.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreLvsp(MreLeveledList):
    classType = 'LVSP'
    copyAttrs = ('chanceNone',)

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelStruct('LVLD','B','chanceNone'),
        MelStruct('LVLF','B',(MreLeveledListBase._flags,'flags',0L)),
        MelNull('LLCT'),
        MreLeveledList.MelLevListLvlo(),
        )
    __slots__ = MreLeveledList.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMgef(MelRecord):
    """Mgef Item"""
    classType = 'MGEF'

    MgefActorValueEnum = bolt.Flags(0L,bolt.Flags.getNames(
        (-1, 'None'),
        (0, 'Aggresion'),
        (1, 'Confidence'),
        (2, 'Energy'),
        (3, 'Morality'),
        (4, 'Mood'),
        (5, 'Assistance'),
        (6, 'One-Handed'),
        (7, 'Two-Handed'),
        (8, 'Archery'),
        (9, 'Block'),
        (10, 'Smithing'),
        (11, 'HeavyArmor'),
        (12, 'LightArmor'),
        (13, 'Pickpocket'),
        (14, 'Lockpicking'),
        (15, 'Sneak'),
        (16, 'Alchemy'),
        (17, 'Speech'),
        (18, 'Alteration'),
        (19, 'Conjuration'),
        (20, 'Destruction'),
        (21, 'Illusion'),
        (22, 'Restoration'),
        (23, 'Enchanting'),
        (24, 'Health'),
        (25, 'Magicka'),
        (26, 'Stamina'),
        (27, 'HealRate'),
        (28, 'MagickaRate'),
        (29, 'StaminaRate'),
        (30, 'SpeedMult'),
        (31, 'InventoryWeight'),
        (32, 'CarryWeight'),
        (33, 'CriticalChance'),
        (34, 'MeleeDamage'),
        (35, 'UnarmedDamage'),
        (36, 'Mass'),
        (37, 'VoicePoints'),
        (38, 'VoiceRate'),
        (39, 'DamageResist'),
        (40, 'PoisonResist'),
        (41, 'ResistFire'),
        (42, 'ResistShock'),
        (43, 'ResistFrost'),
        (44, 'ResistMagic'),
        (45, 'ResistDisease'),
        (46, 'unknown46'),
        (47, 'unknown47'),
        (48, 'unknown48'),
        (49, 'unknown49'),
        (50, 'unknown50'),
        (51, 'unknown51'),
        (52, 'unknown52'),
        (53, 'Paralysis'),
        (54, 'Invisibility'),
        (55, 'NightEye'),
        (56, 'DetectLifeRange'),
        (57, 'WaterBreathing'),
        (58, 'WaterWalking'),
        (59, 'unknown59'),
        (60, 'Fame'),
        (61, 'Infamy'),
        (62, 'JumpingBonus'),
        (63, 'WardPower'),
        (64, 'RightItemCharge'),
        (65, 'ArmorPerks'),
        (66, 'ShieldPerks'),
        (67, 'WardDeflection'),
        (68, 'Variable01'),
        (69, 'Variable02'),
        (70, 'Variable03'),
        (71, 'Variable04'),
        (72, 'Variable05'),
        (73, 'Variable06'),
        (74, 'Variable07'),
        (75, 'Variable08'),
        (76, 'Variable09'),
        (77, 'Variable10'),
        (78, 'BowSpeedBonus'),
        (79, 'FavorActive'),
        (80, 'FavorsPerDay'),
        (81, 'FavorsPerDayTimer'),
        (82, 'LeftItemCharge'),
        (83, 'AbsorbChance'),
        (84, 'Blindness'),
        (85, 'WeaponSpeedMult'),
        (86, 'ShoutRecoveryMult'),
        (87, 'BowStaggerBonus'),
        (88, 'Telekinesis'),
        (89, 'FavorPointsBonus'),
        (90, 'LastBribedIntimidated'),
        (91, 'LastFlattered'),
        (92, 'MovementNoiseMult'),
        (93, 'BypassVendorStolenCheck'),
        (94, 'BypassVendorKeywordCheck'),
        (95, 'WaitingForPlayer'),
        (96, 'One-HandedModifier'),
        (97, 'Two-HandedModifier'),
        (98, 'MarksmanModifier'),
        (99, 'BlockModifier'),
        (100, 'SmithingModifier'),
        (101, 'HeavyArmorModifier'),
        (102, 'LightArmorModifier'),
        (103, 'PickpocketModifier'),
        (104, 'LockpickingModifier'),
        (105, 'SneakingModifier'),
        (106, 'AlchemyModifier'),
        (107, 'SpeechcraftModifier'),
        (108, 'AlterationModifier'),
        (109, 'ConjurationModifier'),
        (110, 'DestructionModifier'),
        (111, 'IllusionModifier'),
        (112, 'RestorationModifier'),
        (113, 'EnchantingModifier'),
        (114, 'One-HandedSkillAdvance'),
        (115, 'Two-HandedSkillAdvance'),
        (116, 'MarksmanSkillAdvance'),
        (117, 'BlockSkillAdvance'),
        (118, 'SmithingSkillAdvance'),
        (119, 'HeavyArmorSkillAdvance'),
        (120, 'LightArmorSkillAdvance'),
        (121, 'PickpocketSkillAdvance'),
        (122, 'LockpickingSkillAdvance'),
        (123, 'SneakingSkillAdvance'),
        (124, 'AlchemySkillAdvance'),
        (125, 'SpeechcraftSkillAdvance'),
        (126, 'AlterationSkillAdvance'),
        (127, 'ConjurationSkillAdvance'),
        (128, 'DestructionSkillAdvance'),
        (129, 'IllusionSkillAdvance'),
        (130, 'RestorationSkillAdvance'),
        (131, 'EnchantingSkillAdvance'),
        (132, 'LeftWeaponSpeedMultiply'),
        (133, 'DragonSouls'),
        (134, 'CombatHealthRegenMultiply'),
        (135, 'One-HandedPowerModifier'),
        (136, 'Two-HandedPowerModifier'),
        (137, 'MarksmanPowerModifier'),
        (138, 'BlockPowerModifier'),
        (139, 'SmithingPowerModifier'),
        (140, 'HeavyArmorPowerModifier'),
        (141, 'LightArmorPowerModifier'),
        (142, 'PickpocketPowerModifier'),
        (143, 'LockpickingPowerModifier'),
        (144, 'SneakingPowerModifier'),
        (145, 'AlchemyPowerModifier'),
        (146, 'SpeechcraftPowerModifier'),
        (147, 'AlterationPowerModifier'),
        (148, 'ConjurationPowerModifier'),
        (149, 'DestructionPowerModifier'),
        (150, 'IllusionPowerModifier'),
        (151, 'RestorationPowerModifier'),
        (152, 'EnchantingPowerModifier'),
        (153, 'DragonRend'),
        (154, 'AttackDamageMult'),
        (155, 'HealRateMult'),
        (156, 'MagickaRateMult'),
        (157, 'StaminaRateMult'),
        (158, 'WerewolfPerks'),
        (159, 'VampirePerks'),
        (160, 'GrabActorOffset'),
        (161, 'Grabbed'),
        (162, 'unknown162'),
        (163, 'ReflectDamage'),
    ))

    MgefGeneralFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (1, 'Hostile'),
        (2, 'Recover'),
        (3, 'Detrimental'),
        (4, 'snapToNavmesh'),
        (5, 'noHitEvent'),
        (6, 'unknown6'),
        (7, 'unknown7'),
        (8, 'unknown8'),
        (9, 'dispellWithKeywords'),
        (10, 'noDuration'),
        (11, 'noMagnitude'),
        (12, 'noArea'),
        (13, 'fxPersist'),
        (14, 'unknown14'),
        (15, 'goryVisuals'),
        (16, 'hideInUI'),
        (17, 'unknown17'),
        (18, 'No Recast'),
        (19, 'unknown19'),
        (20, 'unknown20'),
        (21, 'unknown21'),
        (22, 'powerAffectsMagnitude'),
        (23, 'powerAffectsDuration'),
        (24, 'unknown24'),
        (25, 'unknown25'),
        (26, 'unknown26'),
        (27, 'Painless'),
        (28, 'noHitEffect'),
        (29, 'noDeathDispel'),
        (30, 'unknown30'),
        (31, 'unknown31'),
        (32, 'unknown32'),
    ))

    MgefCastTypeEnum = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'constantEffect'),
        (1, 'fireandForget'),
        (2, 'concentrate'),
        (3, 'scroll'),
    ))

    MgefDeliveryTypeEnum = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'self'),
        (1, 'contact'),
        (2, 'aimed'),
        (3, 'targetActor'),
        (3, 'targetLocation'),
    ))

    MgefSoundLevelEnum = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'loud'),
        (1, 'normal'),
        (2, 'silent'),
        (3, 'veryLoud'),
    ))

    MgefEffectArchetype = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'ValueModifier'),
        (1, 'Script'),
        (2, 'Dispel'),
        (3, 'CureDisease'),
        (4, 'Absorb'),
        (5, 'DualValueModifier'),
        (6, 'Calm'),
        (7, 'Demoralize'),
        (8, 'Frenzy'),
        (9, 'Disarm'),
        (10, 'CommandSummoned'),
        (11, 'Invisibility'),
        (12, 'Light'),
        (13, 'unknown13'),
        (14, 'unknown14'),
        (15, 'Lock'),
        (16, 'Open'),
        (17, 'BoundWeapon'),
        (18, 'SummonCreature'),
        (19, 'DetectLife'),
        (20, 'Telekinesis'),
        (21, 'Paralysis'),
        (22, 'Reanimate'),
        (23, 'SoulTrap'),
        (24, 'TurnUndead'),
        (25, 'Guide'),
        (26, 'WerewolfFeed'),
        (27, 'CureParalysis'),
        (28, 'CureAddiction'),
        (29, 'CurePoison'),
        (30, 'Concussion'),
        (31, 'ValueandParts'),
        (32, 'AccumulateMagnitude'),
        (33, 'Stagger'),
        (34, 'PeakValueModifier'),
        (35, 'Cloak'),
        (36, 'Werewolf'),
        (37, 'SlowTime'),
        (38, 'Rally'),
        (39, 'EnhanceWeapon'),
        (40, 'SpawnHazard'),
        (41, 'Etherealize'),
        (42, 'Banish'),
        (43, 'SpawnScriptedRef'),
        (44, 'Disguise'),
        (45, 'GrabActor'),
        (46, 'VampireLord'),
    ))

    MgefSoundsTypeEnum = bolt.Flags(0L,bolt.Flags.getNames(
      (0, 'sheatheDraw'),
      (1, 'charge'),
      (2, 'ready'),
      (3, 'release'),
      (3, 'concentrationCastLoop'),
      (3, 'onHit'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelLString('FULL','full'),
        MelFid('MDOB','harvestIngredient'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelStruct('DATA','IfI2i2If4I4f6Ii3IfIf7I2f',
        (MgefGeneralFlags,'flags',0L),'baseCost',(FID,'assocItem'),(MgefActorValueEnum,'magicSkill'),
        (MgefActorValueEnum,'resistValue'),'unknown',(FID,'castingLight'),'taperWeight',(FID,'hitShader'),
        (FID,'enchantShader'),'minimumSkillLevel','spellmakingArea',
        'spellmakingCastingTime','taperCurve','taperDuration','secondAvWeight',
        (MgefEffectArchetype,'flags',0L),(MgefActorValueEnum,'flags',0L),(FID,'projectile'),
        (FID,'explosion'),(MgefCastTypeEnum,'castingType',0L),(MgefDeliveryTypeEnum,'delivery',0L),
        (MgefActorValueEnum,'secondActorValue',0L),(FID,'castingArt'),(FID,'hitEffectArt'),(FID,'impactData'),
        'skillUsageMultiplier',(FID,'dualCastingArt'),'dualCastingScale',(FID,'enchantArt'),
        'unknown', 'unknown',(FID,'equipAbility'),(FID,'imageSpaceModifier'),(FID,'perkToApply'),
        (MgefSoundLevelEnum,'castingSoundLevel',0L),'scriptEffectAiScore','scriptEffectAiDelayTime'
        ),
        MelFids('ESCE','effects'),
        MelStruct('SNDD','2I',(MgefSoundsTypeEnum,'flags',0L),'soundsType'),
        MelLString('DNAM','full'),
        MelConditions(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreMisc(MelRecord):
    """Misc. Item"""
    classType = 'MISC'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelIcons(),
        MelDestructible(),
        MelOptStruct('YNAM','I',(FID,'pickupSound')),
        MelOptStruct('ZNAM','I',(FID,'dropSound')),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelStruct('DATA','=If','value','weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreAppa(MelRecord):
    """Appa record (Alchemical Apparatus)"""
    classType = 'APPA'
    AppaTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0, 'novice'),
        (1, 'apprentice'),
        (2, 'journeyman'),
        (3, 'expert'),
        (4, 'master'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelIcons(),
        MelDestructible(),
        MelFid('YNAM','pickupSound'),
        MelFid('ZNAM','dropSound'),
        MelStruct('QUAL','I',(AppaTypeFlags,'flags',0L)),
        MelLString('DESC','description'),
        MelStruct('DATA','If','value','weight'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreCobj(MelRecord):
    """Constructible Object record (recipies)"""
    classType = 'COBJ'
    isKeyedByEid = True # NULL fids are acceptible

    class MelCobjCnto(MelGroups):
        def __init__(self):
            MelGroups.__init__(self,'components',
                MelStruct('CNTO','=2I',(FID,'item',None),'count'),
                MelCoed(),
                )

        def dumpData(self,record,out):
            # Only write the COCT/CNTO/COED subrecords if count > 0
            out.packSub('COCT','I',len(record.components))
            MelGroups.dumpData(self,record,out)

    melSet = MelSet(
        MelString('EDID','eid'),
        MelNull('COCT'), # Handled by MelCobjCnto
        MelCobjCnto(),
        MelConditions(),
        MelFid('CNAM','resultingItem'),
        MelFid('BNAM','craftingStation'),
        MelStruct('NAM1','H','resultingQuantity'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreNpc_(MelRecord):
    """Npc"""
    classType = 'NPC_'
    class MelNpcCnto(MelGroups):
        def __init__(self):
            MelGroups.__init__(self,'container',
                MelStruct('CNTO','=2I',(FID,'item',None),'count'),
                MelCoed(),
                )

        def dumpData(self,record,out):
            # Only write the COCT/CNTO/COED subrecords if count > 0
            out.packSub('COCT','I',len(record.container))
            MelGroups.dumpData(self,record,out)

    melSet = MelSet(MelString('EDID', 'eid'),
                    MelVmad(),
                    MelBounds(),
                    MelStruct('ACBS', '<IHHHHHHHHHH', 'base_flags', 'base_magicka', 'base_stamina',
                              'base_level', 'base_minlevel', 'base_maxlevel', 'base_speed', 'base_disposition',
                              'base_template_flags', 'base_health', 'base_bleedout'),
                    MelStructs('SNAM', '<IB3s', 'factions', (FID, 'faction'), 'rank', 'snam_unused'),
                    MelOptStruct('INAM', '<I', (FID, 'deathitem')),
                    MelOptStruct('VTCK', '<I', (FID, 'voicetype')),
                    MelOptStruct('TPLT', '<I', (FID, 'template')),
                    MelStruct('RNAM', '<I', (FID, 'race')),
                    MelDestructible(),
                    MelNull('SPCT'),
                    MelSpells('SPLO', '<I', 'spells', (FID, 'spell')),
                    MelOptStruct('WNAM', '<I', (FID, 'worm_armor')),
                    MelOptStruct('ANAM', '<I', (FID, 'away_model_name')),
                    MelOptStruct('ATKR', '<I', (FID, 'attack_race')),
                    MelStructs('ATKD', '<ffIIfffIfff', 'attack_data', 'damage', 'chance',
                               (FID, 'spell'), 'flags', 'angle',
                               'angle', 'stagger', 'type',
                               'knockdown', 'recovery_time', 'fatigue'),
                    MelStrings('ATKE', 'attack_events'),
                    MelOptStruct('SPOR', '<I', (FID, 'spectator')),
                    MelOptStruct('OCOR', '<I', (FID, 'observe')),
                    MelOptStruct('GWOR', '<I', (FID, 'guard_warn')),
                    MelOptStruct('ECOR', '<I', (FID, 'combat')),
                    MelNull('PRKZ'),
                    MelPerks('PRKR', '<IB3s', 'perks', (FID, 'perk'), 'rank', 'unused'),
                    MelNull('COCT'),
                    MelNpcCnto(),
                    MelStruct('AIDT', '<BBBBBBBBIII', 'aggression', 'confidence',
                              'engergy', 'morality', 'mood', 'assistance', 'ai_flags',
                              'ai_unknown', 'warn', 'warn_attack', 'attack'),
                    MelStructs('PKID', '<I', 'ai_packages', (FID, 'package')),
                    MelNull('KSIZ'),
                    MelKeywords('KWDA', '<I', (FID, 'keywords')),
                    MelStruct('CNAM', '<I', (FID, 'class')),
                    MelLString('FULL', 'full'),
                    MelLString('SHRT', 'short_alias'),
                    MelBase('DATA', 'marker'),
                    MelStruct('DNAM', '<2B3H2sfB3s', 'base_skills', 'mod_skills',
                              'calc_health', 'calc_magicka', 'calc_stamina', 'dnam_unused1',
                              'far_away_distance', 'geared_up_weapons', 'dnam_unused2'),
                    MelStructs('PNAM', '<I', 'head_part_addons', (FID, 'addon')),
                    MelOptStruct('HCLF', '<I', (FID, 'hair_color')),
                    MelOptStruct('ZNAM', '<I', (FID, 'combat_style')),
                    MelOptStruct('GNAM', '<I', (FID, 'gifts')),
                    MelStruct('NAM5', '<H', 'NAM5'),
                    MelStruct('NAM6', '<f', 'height'),
                    MelStruct('NAM7', '<f', 'weight'),
                    MelStruct('NAM8', '<I', 'sound_level'),
                    MelGroups('event_sound',
                              MelStruct('CSDT', '<I', 'sound_type'),
                              MelGroups('sound',
                                        MelStruct('CSDI', '<I', (FID, 'sound')),
                                        MelStruct('CSDC', '<B', 'chance')
                                       )
                             ),
                    MelOptStruct('CSCR', '<I', (FID, 'audio_template')),
                    MelOptStruct('DOFT', '<I', (FID, 'default_outfit')),
                    MelOptStruct('SOFT', '<I', (FID, 'sleep_outfit')),
                    MelOptStruct('DPLT', '<I', (FID, 'default_package')),
                    MelOptStruct('CRIF', '<I', (FID, 'crime_faction')),
                    MelOptStruct('FTST', '<I', (FID, 'face_texture')),
                    MelOptStruct('QNAM', '<fff', 'skin_tone_r' ,'skin_tone_g', 'skin_tone_b'),
                    MelOptStruct('NAM9', '<ffffffffffffffffff4s', 'nose_long', 'nose_up',
                                 'jaw_up', 'jaw_wide', 'jaw_forward', 'cheeks_up', 'cheeks_back',
                                 'eyes_up', 'eyes_out', 'brows_up', 'brows_out', 'brows_forward',
                                 'lips_up', 'lips_out', 'chin_wide', 'chin_down', 'chin_underbite',
                                 'eyes_back', 'nam9_unused'),
                    MelGroups('face_parts',
                              MelStruct('NAMA', '<iiii', 'nose', 'unknown', 'eyes', 'mouth')),
                    MelGroups('face_tint_layer',
                              MelStruct('TINI', '<H', 'tint_item'),
                              MelStruct('TINC', '<4B', 'r', 'g', 'b' ,'a'),
                              MelStruct('TINV', '<i', 'tint_value'),
                              MelStruct('TIAS', '<h', 'unknown'),
                              ),)
# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# PACK ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# QUST ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# RACE ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# REFR ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# REGN ------------------------------------------------------------------------
#------------------------------------------------------------------------------
class MreSoun(MelRecord):
    """Soun Item"""
    classType = 'SOUN'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        # FNAM Leftover, Unused
        MelString('FNAM','fxPath'),
        # SNDD Leftover, Unused
        MelBase('SNDD','soundData'),
        MelFid('SDSC','soundDescriptor'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
# class MreSpel(MelRecord,MreHasEffects):
class MreSpel(MelRecord):
    """Spell record."""
    classType = 'SPEL'

    # currently not used for Skyrim needs investigated to see if TES5Edit does this
    # class SpellFlags(Flags):
    #     """For SpellFlags, immuneSilence activates bits 1 AND 3."""
    #     def __setitem__(self,index,value):
    #         setter = Flags.__setitem__
    #         setter(self,index,value)
    #         if index == 1:
    #             setter(self,3,value)

    # flags = SpellFlags(0L,Flags.getNames
    SpelTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        ( 0,'manualCostCalc'),
        ( 1,'unknown2'),
        ( 2,'unknown3'),
        ( 3,'unknown4'),
        ( 4,'unknown5'),
        ( 5,'unknown6'),
        ( 6,'unknown7'),
        ( 7,'unknown8'),
        ( 8,'unknown9'),
        ( 9,'unknown10'),
        (10,'unknown11'),
        (11,'unknown12'),
        (12,'unknown13'),
        (13,'unknown14'),
        (14,'unknown15'),
        (15,'unknown16'),
        (16,'unknown17'),
        (17,'pcStartSpell'),
        (18,'unknown19'),
        (19,'areaEffectIgnoresLOS'),
        (20,'ignoreResistance'),
        (21,'noAbsorbReflect'),
        (22,'unknown23'),
        (23,'noDualCastModification'),
        (24,'unknown25'),
        (25,'unknown26'),
        (26,'unknown27'),
        (27,'unknown28'),
        (28,'unknown29'),
        (29,'unknown30'),
        (30,'unknown31'),
        (31,'unknown32'),
         ))

    SpelTypeCast = bolt.Flags(0L,bolt.Flags.getNames(
        ( 0,'spell'),
        ( 1,'disease'),
        ( 2,'power'),
        ( 3,'lesserPower'),
        ( 4,'ability'),
        ( 5,'poison'),
        ( 6,'unknown6'),
        ( 7,'unknown7'),
        ( 8,'unknown8'),
        ( 9,'unknown9'),
        (10,'addiction'),
        (11,'voice'),
         ))

    SpelCastEffectType = bolt.Flags(0L,bolt.Flags.getNames(
        ( 0,'constantEffect'),
        ( 1,'fireAndForget'),
        ( 2,'concentration'),
        ( 3,'scroll'),
         ))

    SpelCastTargetType = bolt.Flags(0L,bolt.Flags.getNames(
        ( 0,'self'),
        ( 0,'touch'),
        ( 0,'aimed'),
        ( 0,'targetActor'),
        ( 0,'targetLocation'),
         ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelLString('FULL','full'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelFid('MDOB', 'menuDisplayObject'),
        MelFid('ETYP', 'equipmentType'),
        MelLString('DESC','description'),
        MelStruct('SPIT','3If2I2fI','baseCost',(SpelTypeFlags,'flags',0L),(SpelTypeCast,'spellType',0L),
                  'chargeTime',(SpelCastEffectType,'castType',0L),(SpelCastTargetType,'targetType',0L),
                  'castDuration','range',(FID,'halfCostPerk'),
            ),
        MelEffects(),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreScrl(MelRecord):
    """Scroll record."""
    classType = 'SCRL'

    ScrollTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'spell'),
        (1,'disease'),
        (2,'power'),
        (3,'lesserPower'),
        (4,'ability'),
        (5,'poison'),
        (6,'unknown6'),
        (7,'unknown7'),
        (8,'unknown8'),
        (9,'unknown9'),
        (10,'addiction'),
        (11,'voice'),
    ))

    ScrollDataFlags = bolt.Flags(0L,bolt.Flags.getNames(
        (0,'manualCostCalc'),
        (1,'unknown2'),
        (2,'unknown3'),
        (3,'unknown4'),
        (4,'unknown5'),
        (5,'unknown6'),
        (6,'unknown7'),
        (7,'unknown8'),
        (8,'unknown9'),
        (9,'unknown10'),
        (10,'unknown11'),
        (11,'unknown12'),
        (12,'unknown13'),
        (13,'unknown14'),
        (14,'unknown15'),
        (15,'unknown16'),
        (16,'unknown17'),
        (17,'pcStartSpell'),
        (18,'unknown19'),
        (19,'areaEffectIgnoresLOS'),
        (20,'ignoreResistance'),
        (21,'noAbsorbReflect'),
        (22,'unknown23'),
        (23,'noDualCastModification'),
        (24,'unknown25'),
        (25,'unknown26'),
        (26,'unknown27'),
        (27,'unknown28'),
        (28,'unknown29'),
        (29,'unknown30'),
        (30,'unknown31'),
        (31,'unknown32'),
    ))

    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelString('FULL','full'),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelFids('MDOB','menuDisplayObject'),
        MelFid('ETYP','equipmentType',),
        MelString('DESC','description'),
        MelModel(),
        MelDestructible(),
        MelFid('YNAM','soundPickUp',),
        MelFid('ZNAM','soundDrop',),
        MelStruct('DATA','If','itemValue','itemWeight',),
        MelStruct('SPIT','IIIfIIffI','baseCost',(ScrollDataFlags,'dataFlags',0L),
                  (ScrollTypeFlags,'typeFlags',0L),'chargeTime','castType','targetType',
                  'castDuration','range',(FID,'halfCostPerk'),),
        MelGroups('effects',
            MelFid('EFID', 'baseEffect',),
            MelStruct('EFIT','fII','magnitude','area','duration',),
            )
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreStat(MelRecord):
    """Static model record."""
    classType = 'STAT'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelModel(),
        MelStruct('DNAM','fI','maxAngle30to120',(FID,'material'),),
        # Contains null-terminated mesh filename followed by random data up to 260 bytes
        MelBase('MNAM','distantLOD'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
# MNAM Should use a custom unpacker if needed for the patcher otherwise MelBase
#------------------------------------------------------------------------------
class MreTree(MelRecord):
    """Tree Item"""
    classType = 'TREE'
    melSet = MelSet(
        MelString('EDID','eid'),
        MelBounds(),
        MelModel(),
        MelFid('PFIG','harvestIngredient'),
        MelFid('SNAM','harvestSound'),
        MelStruct('PFPC','4B','spring','summer','fall','wsinter',),
        MelLString('FULL','full'),
        MelStruct('CNAM','ff8Iff','trunkFlexibility','branchFlexibility'
        'unknown1','unknown2','unknown3','unknown4','unknown5','unknown6','unknown7','unknown8',
        'leafAmplitude','leafFrequency'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreFlor(MelRecord):
    """Flor Item"""
    classType = 'FLOR'

    melSet = MelSet(
        MelString('EDID','eid'),
        MelVmad(),
        MelBounds(),
        MelLString('FULL','full'),
        MelModel(),
        MelDestructible(),
        MelNull('KSIZ'),
        MelKeywords('KWDA','keywords'),
        MelBase('PNAM','unknown01'),
        MelLString('RNAM','activateTextOverride'),
        MelBase('FNAM','unknown02'),
        MelFid('PFIG','harvestIngredient'),
        MelFid('SNAM','harvestSound'),
        MelStruct('PFPC','4B','spring','summer','fall','winter',),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
class MreWatr(MelRecord):
    """Water"""
    classType = 'WATR'
    
    WatrTypeFlags = bolt.Flags(0L,bolt.Flags.getNames(
            (0, 'causesDamage'),
        ))
    
    melSet = MelSet(
        MelString('EDID','eid'),
        MelLString('FULL','full'),
        MelGroups('unused',
            MelString('NNAM','noiseMap',),
            ),
        MelStruct('ANAM','B','opacity'),
        MelStruct('FNAM','B',(WatrTypeFlags,'flags',0L),),
        MelBase('MNAM','unused1'),
        MelFid('TNAM','material',),
        MelFid('SNAM','openSound',),
        MelFid('XNAM','spell',),
        MelFid('INAM','imageSpace',),
        MelStruct('DATA','H','damagePerSecond'),
        MelStruct('DNAM','7f4s2f3Bs3Bs3B5s43f','unknown','unknown','unknown',
                  'unknown','specularPropertiesSunSpecularPower',
                  'waterPropertiesReflectivityAmount',
                  'waterPropertiesFresnelAmount','unknown',
                  'fogPropertiesAboveWaterFogDistanceNearPlane',
                  'fogPropertiesAboveWaterFogDistanceFarPlane',
                  # Shallow Color
                  'red_sc','green_sc','blue_sc','unknown_sc',
                  # Deep Color
                  'red_dc','green_dc','blue_dc','unknown_dc',
                  # Reflection Color
                  'red_rc','green_rc','blue_rc','unknown_rc',
                  'unknown','unknown','unknown','unknown','unknown',
                  'displacementSimulatorStartingSize',
                  'displacementSimulatorForce','displacementSimulatorVelocity',
                  'displacementSimulatorFalloff','displacementSimulatorDampner',
                  'unknown','noisePropertiesNoiseFalloff',
                  'noisePropertiesLayerOneWindDirection',
                  'noisePropertiesLayerTwoWindDirection',
                  'noisePropertiesLayerThreeWindDirection',
                  'noisePropertiesLayerOneWindSpeed',
                  'noisePropertiesLayerTwoWindSpeed',
                  'noisePropertiesLayerThreeWindSpeed',
                  'unknown','unknown','fogPropertiesAboveWaterFogAmount',
                  'unknown','fogPropertiesUnderWaterFogAmount',
                  'fogPropertiesUnderWaterFogDistanceNearPlane',
                  'fogPropertiesUnderWaterFogDistanceFarPlane',
                  'waterPropertiesRefractionMagnitude',
                  'specularPropertiesSpecularPower',
                  'unknown','specularPropertiesSpecularRadius',
                  'specularPropertiesSpecularBrightness',
                  'noisePropertiesLayerOneUVScale',
                  'noisePropertiesLayerTwoUVScale',
                  'noisePropertiesLayerThreeUVScale',
                  'noisePropertiesLayerOneAmplitudeScale',
                  'noisePropertiesLayerTwoAmplitudeScale',
                  'noisePropertiesLayerThreeAmplitudeScale',
                  'waterPropertiesReflectionMagnitude',
                  'specularPropertiesSunSparkleMagnitude',
                  'specularPropertiesSunSpecularMagnitude',
                  'depthPropertiesReflections','depthPropertiesRefraction',
                  'depthPropertiesNormals','depthPropertiesSpecularLighting',
                  'specularPropertiesSunSparklePower',
                  ),
        MelBase('GNAM','unused2'),
        # Linear Velocity
        MelStruct('NAM0','3f','linv_x','linv_y','linv_z',),
        # Angular Velocity
        MelStruct('NAM1','3f','andv_x','andv_y','andv_z',),
        MelString('NAM2','noiseTexture'),
        MelString('NAM3','unused3'),
        MelString('NAM4','unused4'),
        )
    __slots__ = MelRecord.__slots__ + melSet.getSlotsUsed()

# Verified Correct for Skyrim 1.8
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# WEAP ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# WRLD ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Marker for organization please don't remove ---------------------------------
# WTHR ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Unused records, they have empty GRUP in skyrim.esm---------------------------
# CLDC ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Unused records, they have empty GRUP in skyrim.esm---------------------------
# HAIR ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Unused records, they have empty GRUP in skyrim.esm---------------------------
# PWAT ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Unused records, they have empty GRUP in skyrim.esm---------------------------
# RGDL ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Unused records, they have empty GRUP in skyrim.esm---------------------------
# SCOL ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# Unused records, they have empty GRUP in skyrim.esm---------------------------
# SCPT ------------------------------------------------------------------------
#------------------------------------------------------------------------------
# These have undefined FormIDs Do not merge them
#
#       MreNavi, MreNavm, 
#------------------------------------------------------------------------------
# These need syntax revision but can be merged once that is corrected
#
#       MreDial, MreAchr, MreCell, MreLctn, mreInfo
#------------------------------------------------------------------------------
# Mergeable record types
mergeClasses = (
        MreAact, MreActi, MreAddn, MreAlch, MreAmmo, MreAnio, MreAppa, MreArma,
        MreArmo, MreArto, MreAspc, MreAstp, MreAvif, MreBook, MreBptd, MreCams,
        MreClas, MreClfm, MreClmt, MreCobj, MreColl, MreCont, MreCpth, MreCsty,
        MreDebr, MreDlbr, MreDlvw, MreDobj, MreDoor, MreDual, MreEczn, MreEfsh,
        MreEnch, MreEqup, MreExpl, MreEyes, MreFact, MreFlor, MreFlst, MreFstp,
        MreFsts, MreFurn, MreGlob, MreGmst, MreGras, MreHazd, MreHdpt, MreIdle,
        MreIdlm, MreImgs, MreIngr, MreIpct, MreIpds, MreKeym, MreKywd, MreLcrt, 
		MreLgtm, MreLigh, MreLscr, MreLtex, MreLvli, MreLvln, MreLvsp, MreMato,
		MreMatt, MreMesg, MreMgef, MreMisc, MreMovt, MreMstt, MreMusc, MreMust,
		MreNpc_, MreOtft, MreProj, MreRela, MreRevb, MreRfct, MreScrl, MreScen,
		MreSlgm, MreSmbn, MreSmen, MreSmqn, MreSnct, MreSndr, MreSopm, MreSoun,
		MreSpel, MreSpgd, MreStat, MreTact, MreTree, MreTxst, MreVtyp, MreWatr, 
		MreWoop,
    )

#--Extra read/write classes
readClasses = ()
writeClasses = ()

def init():
    # Due to a bug with py2exe, 'reload' doesn't function properly.  Instead of
    # re-executing all lines within the module, it acts like another 'import'
    # statement - in otherwords, nothing happens.  This means any lines that
    # affect outside modules must do so within this function, which will be
    # called instead of 'reload'
    brec.ModReader.recHeader = RecordHeader

    #--Record Types
    brec.MreRecord.type_class = dict((x.classType,x) for x in (
        MreAact, MreActi, MreAddn, MreAlch, MreAmmo, MreAnio, MreAppa, MreArma,
        MreArmo, MreArto, MreAspc, MreAstp, MreAvif, MreBook, MreBptd, MreCams,
        MreClas, MreClfm, MreClmt, MreCobj, MreColl, MreCont, MreCpth, MreCsty,
        MreDebr, MreDlbr, MreDlvw, MreDobj, MreDoor, MreDual, MreEczn, MreEfsh,
        MreEnch, MreEqup, MreExpl, MreEyes, MreFact, MreFlor, MreFlst, MreFstp,
        MreFsts, MreFurn, MreGlob, MreGmst, MreGras, MreHazd, MreHdpt, MreIdle,
        MreIdlm, MreImgs, MreIngr, MreIpct, MreIpds, MreKeym, MreKywd, MreLcrt, 
		MreLgtm, MreLigh, MreLscr, MreLtex, MreLvli, MreLvln, MreLvsp, MreMato,
		MreMatt, MreMesg, MreMgef, MreMisc, MreMovt, MreMstt, MreMusc, MreMust,
		MreNpc_, MreOtft, MreProj, MreRela, MreRevb, MreRfct, MreScrl, MreScen,
		MreSlgm, MreSmbn, MreSmen, MreSmqn, MreSnct, MreSndr, MreSopm, MreSoun,
		MreSpel, MreSpgd, MreStat, MreTact, MreTree, MreTxst, MreVtyp, MreWatr, 
		MreWoop,
        MreHeader,
    ))

    #--Simple records
    brec.MreRecord.simpleTypes = (set(brec.MreRecord.type_class) -
        set(('TES4')))
