# -*- coding: utf-8 -*-
#
# bait/presenter/view_commands.py
#
# GPL License and Copyright Notice ============================================
#  This file is part of Wrye Bash.
#
#  Wrye Bash is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Wrye Bash is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with Wrye Bash.  If not, see <http://www.gnu.org/licenses/>.
#
#  Wrye Bash Copyright (C) 2011 Myk Taylor
#
# =============================================================================

# command IDs
ADD_GROUP = 1
ADD_PACKAGE = 2
EXPAND_GROUP = 3
CLEAR_PACKAGES = 4
SET_FILTER_STATS = 5
STATUS_UPDATE = 6
SET_PACKAGE_LABEL = 7
ADD_FILE = 8
SET_DATA_STATS = 9
CLEAR_FILES = 10
SET_FILE_DETAILS = 11
SELECT_PACKAGES = 12
EXPAND_DIR = 13
SELECT_FILES = 14
SET_STYLE_MAPS = 15
SET_PACKAGE_INFO = 16

# style values
FONT_STYLE_BOLD_FLAG = 1
FONT_STYLE_ITALICS_FLAG = 2
TEXT_DISABLED = 1
TEXT_HAS_INACTIVE_OVERRIDDE = 2
HIGHLIGHT_ERROR = 3
HIGHLIGHT_MISSING_DEPENDENCY = 4
HIGHLIGHT_DIRTY = 5
ICON_PROJECT_MATCHES = 1
ICON_PROJECT_MISMATCHED = 2
ICON_PROJECT_MISSING = 3
ICON_PROJECT_EMPTY = 4
ICON_PROJECT_UNINSTALLABLE = 5
ICON_INSTALLER_MATCHES = 6
ICON_INSTALLER_MISMATCHED = 7
ICON_INSTALLER_MISSING = 8
ICON_INSTALLER_EMPTY = 9
ICON_INSTALLER_UNINSTALLABLE = 10


class ViewCommandStyle:
    def __init__(self, fontStyleMask=None, textColorId=None, hilightColorId=None, checkboxState=None, iconId=None):
        self.fontStyleMask = fontStyleMask
        self.textColorId = textColorId
        self.hilightColorId = hilightColorId
        self.checkboxState = checkboxState
        self.iconId = iconId


# command classes
class ViewCommand:
    '''The commandId refers to the subclass type.  The requestId is the id of
    the associated request that instigated this command.  If this command was
    not sent in response to a request, it will be set to None'''
    def __init__(self, commandId, requestId):
        self.commandId = commandId
        self.requestId = requestId

class _AddNode(ViewCommand):
    '''Adds a node to a tree
       The parent node will either have been previously sent or will be None,
       meaning top-level.  The predecessor node will either have been previously
       sent or be None, meaning "first child of parent"'''
    def __init__(self, commandId, label, nodeId, parentNodeId, predecessorNodeId, style=None, requestId=None):
        ViewCommand.__init__(self, commandId, requestId)
        self.label = label
        self.nodeId = nodeId
        self.parentNodeId = parentNodeId
        self.predecessorNodeId = predecessorNodeId
        self.style = style

class AddGroup(_AddNode):
    '''Adds a group node to the packages tree'''
    def __init__(self, label, nodeId, parentNodeId, predecessorNodeId, style=None, requestId=None):
        _AddNode.__init__(self, ADD_GROUP, label, nodeId, parentNodeId, predecessorNodeId, style, requestId)

class AddPackage(_AddNode):
    '''Adds a package node to the packages tree'''
    def __init__(self, label, nodeId, parentNodeId, predecessorNodeId, style=None, requestId=None):
        _AddNode.__init__(self, ADD_PACKAGE, label, nodeId, parentNodeId, predecessorNodeId, style, requestId)

class ExpandGroup(ViewCommand):
    def __init__(self, nodeId, requestId=None):
        ViewCommand.__init__(self, EXPAND_GROUP, requestId)
        self.nodeId = nodeId

class ExpandDir(ViewCommand):
    def __init__(self, nodeId, requestId=None):
        ViewCommand.__init__(self, EXPAND_DIR, requestId)
        self.nodeId = nodeId

class ClearPackages(ViewCommand):
    def __init__(self, requestId=None):
        ViewCommand.__init__(self, CLEAR_PACKAGES, requestId)

class SelectPackages(ViewCommand):
    '''nodeIds will be a list of node Ids to select'''
    def __init__(self, nodeIds, requestId=None):
        ViewCommand.__init__(self, SELECT_PACKAGES, requestId)
        self.nodeIds = nodeIds

class SelectFiles(ViewCommand):
    '''nodeIds will be a list of node Ids to select'''
    def __init__(self, nodeIds, requestId=None):
        ViewCommand.__init__(self, SELECT_FILES, requestId)
        self.nodeIds = nodeIds

class SetFilterStats(ViewCommand):
    def __init__(self, filterId, current, total, requestId=None):
        ViewCommand.__init__(self, SET_FILTER_STATS, requestId)
        self.filterId = filterId
        self.current = current
        self.total = total

class SetDataStats(ViewCommand):
    def __init__(self, activePlugins, totalPlugins, knownFiles, totalFiles, requestId=None):
        ViewCommand.__init__(self, SET_DATA_STATS, requestId)
        self.activePlugins = activePlugins
        self.totalPlugins = totalPlugins
        self.knownFiles = knownFiles
        self.totalFiles = totalFiles

class _SetText(ViewCommand):
    '''If text is None, it means nothing is selected.  If text is the
    empty string, it means there is no text to display for that item'''
    def __init__(self, commandId, text, requestId=None):
        ViewCommand.__init__(self, commandId, requestId)
        self.text = text

class StatusUpdate(_SetText):
    def __init__(self, text, requestId=None):
        _SetText.__init__(self, STATUS_UPDATE, text, requestId)

class SetPackageLabel(_SetText):
    """None means no package is selected, a blank label means multiple packages are selected"""
    def __init__(self, text, requestId=None):
        _SetText.__init__(self, SET_PACKAGE_LABEL, text, requestId)

class AddFile(_AddNode):
    '''Adds a subpackage/file/directory node to the file tree'''
    def __init__(self, label, nodeId, parentNodeId, predecessorNodeId, style=None, requestId=None):
        _AddNode.__init__(self, ADD_FILE, label, nodeId, parentNodeId, predecessorNodeId, style, requestId)

class ClearFiles(ViewCommand):
    def __init__(self, requestId=None):
        ViewCommand.__init__(self, CLEAR_FILES, requestId)

class SetFileDetails(_SetText):
    def __init__(self, text, requestId=None):
        _SetText.__init__(self, SET_FILE_DETAILS, text, requestId)

class SetStyleMaps(ViewCommand):
    '''Contains the map from style IDs to RGB tuples (for colors) and file resources (for images)'''
    def __init__(self, colorMap, checkedIconMap, uncheckedIconMap, requestId=None):
        ViewCommand.__init__(self, SET_STYLE_MAPS, requestId)
        self.colorMap = colorMap
        self.checkedIconMap = checkedIconMap
        self.uncheckedIconMap = uncheckedIconMap

class SetPackageInfo(ViewCommand):
    """sets the data for one of the package info tabs
    For the General tab: data is a dictionary that has the following keys:
      isArchive: boolean indicating whether the package is an archive
      isHidden: boolean indicating whether the package is hidden
      isInstalled: boolean indicating whether the package is installed
      packageSize: a string representing the package size in the installers directory
      contentsSize: a string representing the cumulative size of the package contents
      lastModifiedTimestamp: a string representing the last modification date
      The following additional keys are optional, interpreted as 0 if absent
      numFiles, numDirty, numOverridden, numSkipped,
      numSelectedMatched, numSelectedMismatched, numSelectedOverridden, numSelectedMissing, numTotalSelected,
      numUnselectedMatched, numUnselectedMismatched, numUnselectedOverridden, numUnselectelectedMissing, numTotalUnselected,
      numTotalMatched, numTotalMismatched, numTotalOverridden, numTotalMissing, numTotalSelectable
    For the Dirty tab: data is a list of tuples: (actionType, path), where actionType is one of
      the dirty filter IDs
    For the Conflicts tab, TODO: define     
    """
    def __init__(self, tabId, data, requestId=None):
        ViewCommand.__init__(self, SET_PACKAGE_INFO, requestId)
        self.tabId = tabId
        self.data = data