# -*- coding: utf-8 -*-
#
# bait/presenter/impl/diff_engine.py
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

import Queue
import logging
import re

from . import filters
from ... import model
from ... import presenter
from ...presenter import view_commands


_logger = logging.getLogger(__name__)

_IS_VISIBLE_IDX = 0
_MATCHES_SEARCH = 1
_PRED_NODE_ID_IDX = 2
_ATTRIBUTES_IDX = 3
_CHILDREN_IDX = 4


def _visit_tree(rootNodeId, tree, quitEarlyIfFalseFn, visitor):
    # walk the tree breadth first starting from the children of the specified node
    childNodeIds = Queue.Queue()
    rootNodeData = tree[rootNodeId]
    if len(rootNodeData) > _CHILDREN_IDX:
        for childNodeId in rootNodeData[_CHILDREN_IDX]:
            childNodeIds.put(childNodeId)
    while not childNodeIds.empty():
        if quitEarlyIfFalseFn and quitEarlyIfFalseFn():
            _logger.debug("halting tree walk: early quit condition raised")
            break
        nodeId = childNodeIds.get()
        nodeData = tree[childNodeId]
        if not visitor.visit(nodeId, nodeData):
            _logger.debug("visitor returned False, skipping node children")
            continue
        # if this node has children, add them to the processing queue
        if len(nodeData) > _CHILDREN_IDX:
            for childNodeId in nodeData[_CHILDREN_IDX]:
                childNodeIds.put(childNodeId)

def _get_package_icon_id(packageNodeAttributes):
    if packageNodeAttributes.isCorrupt:
        if packageNodeAttributes.isArchive:
            return view_commands.IconIds.INSTALLER_UNINSTALLABLE
        else:
            return view_commands.IconIds.PROJECT_UNINSTALLABLE
    if packageNodeAttributes.hasMismatched:
        if packageNodeAttributes.isArchive:
            if packageNodeAttributes.hasWizard:
                return view_commands.IconIds.INSTALLER_MISMATCHED_WIZ
            else:
                return view_commands.IconIds.INSTALLER_MISMATCHED
        else:
            if packageNodeAttributes.hasWizard:
                return view_commands.IconIds.PROJECT_MISMATCHED_WIZ
            else:
                return view_commands.IconIds.PROJECT_MISMATCHED
    if packageNodeAttributes.hasMissing:
        if packageNodeAttributes.isArchive:
            if packageNodeAttributes.hasWizard:
                return view_commands.IconIds.INSTALLER_MISSING_WIZ
            else:
                return view_commands.IconIds.INSTALLER_MISSING
        else:
            if packageNodeAttributes.hasWizard:
                return view_commands.IconIds.PROJECT_MISSING_WIZ
            else:
                return view_commands.IconIds.PROJECT_MISSING
    if packageNodeAttributes.hasMatched:
        if packageNodeAttributes.isArchive:
            if packageNodeAttributes.hasWizard:
                return view_commands.IconIds.INSTALLER_MATCHES_WIZ
            else:
                return view_commands.IconIds.INSTALLER_MATCHES
        else:
            if packageNodeAttributes.hasWizard:
                return view_commands.IconIds.PROJECT_MATCHES_WIZ
            else:
                return view_commands.IconIds.PROJECT_MATCHES
    # has no other attributes; must be empty
    if packageNodeAttributes.isArchive:
        if packageNodeAttributes.hasWizard:
            return view_commands.IconIds.INSTALLER_EMPTY_WIZ
        else:
            return view_commands.IconIds.INSTALLER_EMPTY
    else:
        if packageNodeAttributes.hasWizard:
            return view_commands.IconIds.PROJECT_EMPTY_WIZ
        else:
            return view_commands.IconIds.PROJECT_EMPTY

def _get_font_style_mask(treeNodeAttributes):
    fontStyleMask = view_commands.FontStyleIds.NONE
    if treeNodeAttributes.isNew:
        fontStyleMask |= view_commands.FontStyleIds.BOLD
    if treeNodeAttributes.isCorrupt | treeNodeAttributes.isDirty | \
       treeNodeAttributes.isUnrecognized | treeNodeAttributes.hasMissingDeps | \
       treeNodeAttributes.updateAvailable:
        fontStyleMask |= view_commands.FontStyleIds.ITALICS
    return fontStyleMask

def _get_foreground_color(treeNodeAttributes):
    if treeNodeAttributes.isHidden and \
       not treeNodeAttributes.isInstalled and not treeNodeAttributes.isNotInstalled:
        return view_commands.ForegroundColorIds.DISABLED
    if getattr(treeNodeAttributes, hasSubpackages, False):
        return view_commands.ForegroundColorIds.HAS_SUBPACKAGES
    return None

def _get_highlight_color(treeNodeAttributes):
    if treeNodeAttributes.isCorrupt | treeNodeAttributes.isUnrecognized:
        return view_commands.HighlightColorIds.ERROR
    if treeNodeAttributes.hasMissingDeps:
        return view_commands.HighlightColorIds.MISSING_DEPENDENCY
    if treeNodeAttributes.isDirty:
        return view_commands.HighlightColorIds.DIRTY
    return None

def _add_node(nodeId, nodeData, tree, viewCommandQueue, isUpdate=False):
    # walk the tree backwards to find a visible predecessor
    predNodeId = nodeData[_PRED_NODE_ID_IDX]
    predNodeData = tree[predNodeId]
    while predNodeId is not None and not predNodeData[_IS_VISIBLE_IDX]:
        predNodeId = predNodeData[_PRED_NODE_ID_IDX]
        predNodeData = tree[predNodeId]
    # craft visual style
    style = view_commands.Style(_get_font_style_mask(nodeAttributes),
                                _get_foreground_color(nodeAttributes),
                                _get_highlight_color(nodeAttributes),
                                None, None)
    # add node to view widget
    if nodeAttributes.nodeType is model.NodeTypes.PACKAGE:
        # craft package-specific style elements
        if not nodeAttributes.isHidden:
            style.checkboxState = nodeAttributes.isInstalled
        style.iconId = _get_package_icon_id(nodeAttributes)
        if isUpdate:
            viewCommand = view_commands.UpdatePackage
        else:
            viewCommand = view_commands.AddPackage
        viewCommandQueue.put(viewCommand(nodeAttributes.label, nodeId,
                                         nodeAttributes.parentNodeId, predNodeId, style))
    elif nodeAttributes.nodeType is model.NodeTypes.GROUP:
        if isUpdate:
            viewCommand = view_commands.UpdateGroup
        else:
            viewCommand = view_commands.AddGroup
        viewCommandQueue.put(viewCommands(nodeAttributes.label, nodeId,
                                          nodeAttributes.parentNodeId, predNodeId, style))
    else:
        raise TypeError(
            "unexpected node type: %s" % nodeAttributes.nodeType)


class _MarkNotVisibleVisitor:
    def visit(self, nodeId, nodeData):
        # if this node is already invisible, do not recurse into children
        if not nodeData[_IS_VISIBLE_IDX]:
            return False
        nodeData[_IS_VISIBLE_IDX] = False
        return True

class _SyncVisibleNodesVisitor:
    def __init__(self, tree, visibleNodeIds, viewCommandQueue):
        self._tree = tree
        self._visibleNodeIds = visibleNodeIds
        self._viewCommandQueue = viewCommandQueue

    def visit(self, nodeId, nodeData):
        isVisible = nodeId in self._visibleNodeIds
        if isVisible != nodeData[_IS_VISIBLE_IDX]:
            if isVisible:
                _add_node(nodeId, nodeData, self._tree, self._viewCommandQueue)
                nodeData[_IS_VISIBLE_IDX] = True
            else:
                # removes entire tree rooted at nodeId
                self._viewCommandQueue.put(
                    view_commands.RemovePackagesTreeNode(nodeId))
                # walk tree downstream and mark not visible
                _visit_tree(nodeId, self._tree, None, _MarkNotVisibleVisitor())
                nodeData[_IS_VISIBLE_IDX] = False
        return True


class _MakeTreeVisibleVisitor:
    def __init__(self, tree, viewCommandQueue):
        self._tree = tree
        self._viewCommandQueue = viewCommandQueue

    def visit(self, nodeId, nodeData):
        if not nodeData[_IS_VISIBLE_IDX]:
            _add_node(nodeId, nodeData, self._tree, self._viewCommandQueue)
            nodeData[_IS_VISIBLE_IDX] = True
        return True


class _AddToSetVisitor:
    def __init__(self, nodeSet):
        self._nodeSet = nodeSet

    def visit(self, nodeId, nodeData):
        self._nodeSet.add(nodeId)
        return True

class _SearchVisitor:
    def __init__(self, tree, expression):
        self.matchedNodeIds = set()
        self._tree = tree
        self._expression = expression

    def visit(self, nodeId, nodeData):
        # if this node is already in the set, don't process this node or its children
        if nodeId in self.matchedNodeIds:
            return False
        # if this node has no attributes yet, it doesn't match (and it has no children,
        # so don't recurse)
        if len(nodeData) <= _ATTRIBUTES_IDX:
            return False
        nodeAttributes = nodeData[_ATTRIBUTES_IDX]
        # if this node matches, add its parentage and downstream tree
        if self._expression.search(nodeAttributes.label):
            # get parentage
            parentNodeId = nodeAttributes.parentNodeId
            while parentNodeId is not None:
                if nodeId in self.matchedNodeIds:
                    break
                self.matchedNodeIds.add(parentNodeId)
                # if the parent didn't have attributes, we wouldn't have gotten this far
                # down in the tree to begin with, so we can assume the parent node's
                # attributes are not None here
                parentNodeId = self._tree[parentNodeId][_ATTRIBUTES_IDX].parentNodeId
            # get downstream nodes
            _visit_tree(nodeId, self._tree, None, _AddToSetVisitor(self.matchedNodeIds))
            self.matchedNodeIds.add(nodeId)
            # we've already processed the children; no need to recurse
            return False
        # keep searching: one of its children might still match
        return True


class _DiffEngine:
    """Subclasses are thread safe as long as each individual method is only called from a
    single thread.  Different methods can be called simultaneously from different threads.
    Implementation notes:
    ensure attributres exist before children to satisfy algorithm invariants when tracing
    parentage
    """

    def __init__(self, generalTabManager, viewCommandQueue):
        self.loadRequestQueue = Queue.Queue() # tuples of (updateTypeMask, nodeId)
        self._generalTabManager = generalTabManager
        self._viewCommandQueue = viewCommandQueue
        self._pendingFilterMask = presenter.FilterIds.NONE
        self._selectedNodes = set()
        self._tree = {} # nodeId -> [isVisible, matchesSearch, predNodeId, attributes, children]

    def set_pending_filter_mask(self, filterMask):
        self._pendingFilterMask = filterMask


class PackagesTreeDiffEngine(_DiffEngine):
    def __init__(self, generalTabManager, packageContentsManager, viewCommandQueue):
        _DiffEngine.__init__(self, generalTabManager, viewCommandQueue)
        self._packageContentsManager = packageContentsManager
        self._pendingSearchString = None
        self._searchExpression = None
        self._filter = filters.PackagesTreeFilter(viewCommandQueue)
        self._expandedNodeIds = set()
        self._tree[model.ROOT_NODE_ID] = [True, False, None, None]

    def update_is_in_scope(self, updateType, nodeType):
        # called to determine if model updates are in scope for this diff engine
        return (updateType is model.UpdateTypes.ATTRIBUTES and \
                (nodeType is model.NodeTypes.PACKAGE or \
                 nodeType is model.NodeTypes.GROUP)) or \
               (updateType is model.UpdateTypes.CHILDREN and \
                (nodeType is model.NodeTypes.ROOT or \
                 nodeType is model.NodeTypes.GROUP))

    def could_use_update(self, updateType, nodeId, version):
        # assumes update is in scope
        # the following algorithm is thread safe in the sense that it does not mess up
        # other algorithms, being read-only.  These conditions will be checked again, of
        # course, by the thread that actually modifies the data structures
        nodeData = self._tree.get(nodeId)
        if nodeData is None:
            return False
        if updateType is model.UpdateTypes.ATTRIBUTES and len(nodeData) > _ATTRIBUTES_IDX:
            nodeAttributes = nodeData[_ATTRIBUTES_IDX]
            if nodeAttributes is not None:
                return nodeAttributes.version < version
        if updateType is model.UpdateTypes.CHILDREN and len(nodeData) > _CHILDREN_IDX:
            nodeChildren = nodeData[_CHILDREN_IDX]
            if nodeChildren is not None:
                return nodeChildren.version < version
        return True

    def set_pending_search_string(self, searchString):
        self._pendingSearchString = searchString

    def set_node_expansion(self, nodeId, isExpanded):
        if isExpanded:
            if nodeId in self._expandedNodeIds:
                _logger.warn("node already recorded as expanded: %d", nodeId)
            self._expandedNodeIds.add(nodeId)
        else:
            if nodeId not in self._expandedNodeIds:
                _logger.warn("node already recorded as collapsed: %d", nodeId)
            self._expandedNodeIds.discard(nodeId)

    def set_selected_nodes(self, nodeIds):
        self._selectedNodes = set(nodeIds)
        # TODO: inform general tab and package contents managers of selection

    def update_attributes(self, nodeId, nodeAttributes):
        nodeType == nodeAttributes.nodeType
        _logger.debug("updating %s node %d (%s) attributes to: %s",
                      nodeType, nodeId, nodeAttributes.label, nodeAttributes)
        nodeData = self._tree[nodeId]
        if len(nodeData) > _ATTRIBUTES_IDX:
            curNodeAttributes = nodeData[_ATTRIBUTES_IDX]
            if nodeAttributes.version <= curNodeAttributes.version:
                _logger.debug("stale incoming update (version %d <= %d); skipping update",
                              nodeAttributes.version, curNodeAttributes.version)
                return
        matchesSearch = self._matches_search_expression(nodeAttributes)
        nodeData[_MATCHES_SEARCH] = matchesSearch
        isVisible = self._filter.process_and_get_visibility(nodeId, nodeAttributes,
                                                            matchesSearch)
        if isVisible == nodeData[_IS_VISIBLE_IDX]:
            if isVisible:
                _add_node(nodeId, nodeData, self._tree, self._viewCommandQueue, True)
        else:
            if isVisible:
                # show parentage if not already visible
                parentNodeIds = []
                parentNodeId = nodeAttributes.parentNodeId
                while parentNodeId is not None:
                    parentNodeData = self._tree[parentNodeId]
                    if parentNodeData[_IS_VISIBLE_IDX]:
                        break
                    parentNodeIds.append(parentNodeId)
                    parentNodeId = parentNodeData[_ATTRIBUTES_IDX].parentNodeId
                while len(parentNodeIds) > 0:
                    parentNodeId = parentNodeIds.pop()
                    parentNodeData = self._tree[parentNodeId]
                    _add_node(parentNodeId, parentNodeData, self._tree,
                              self._viewCommandQueue, False)
                _add_node(nodeId, nodeData, self._tree, self._viewCommandQueue)
                nodeData[_IS_VISIBLE_IDX] = True
                # if this node specifically matched the search, ensure downstream nodes
                # are visible
                if self._searchExpression is not None and matchesSearch:
                    _visit_tree(nodeId, self._tree, None,
                                _MakeTreeVisibleVisitor(self._tree,
                                                        self._viewCommandQueue))
            else:
                # if this is a leaf node, get rid of it and any parent groups that now
                # have 0 visible children
                if len(nodeData) <= _CHILDREN_IDX or \
                   len(nodeData[_CHILDREN_IDX].children) == 0:
                    branchRootNodeIdToRemove = nodeId
                    nodeData[_IS_VISIBLE_IDX] = False
                    parentNodeId = nodeAttributes.parentNodeId
                    while parentNodeId is not model.ROOT_NODE_ID:
                        parentNodeData = self._tree[parentNodeId]
                        hasVisibleChildren = False
                        # is upstream from us, so must have children
                        for childNodeId in parentNodeData[_CHILDREN_IDX].children:
                            if self._tree[childNodeId][_IS_VISIBLE_IDX]:
                                hasVisibleChildren = True
                                break
                        if not hasVisibleChildren:
                            branchRootNodeIdToRemove = parentNodeId
                        else: break
                        parentNodeId = parentNodeData[_ATTRIBUTES_IDX].parentNodeId
                    self._viewCommandQueue.put(
                        view_commands.RemovePackagesTreeNode(branchRootNodeIdToRemove))
                else:
                    # otherwise, check its extended family to see if it should be visible
                    pass

    def update_children(self, nodeId, nodeChildren):
        nodeType == nodeAttributes.nodeType
        _logger.debug("updating %s node %d children to: %s",
                      nodeType, nodeId, nodeChildren)
        nodeData = self._tree[nodeId]
        curNodeChildren = None
        if len(nodeData) <= _ATTRIBUTES_IDX:
            raise RuntimeError("can't update children without first updating attributes")
        if len(nodeData) > _CHILDREN_IDX:
            curNodeChildren = nodeData[_CHILDREN_IDX]
            if curNodeChildren.version >= nodeChildren.version:
                _logger.debug("stale incoming update (version %d <= %d); skipping update",
                              nodeChildren.version, curNodeChildren.version)
                return
        newChildren = nodeChildren.children
        if curNodeChildren is None:
            nodeData[_CHILDREN_IDX] = nodeChildren
            if newChildren is not None:
                # add new nodes to the tree
                predChildNodeId = None
                for childNodeId in newChildren:
                    self._tree[childNodeId] = [False, predChildNodeId]
                    predChildNodeId = childNodeId
        else:
            newChildrenSet = set(newChildren)
            curChildrenSet = set(curNodeChildren.children)
            for childNodeId in curChildrenSet.difference(newChildrenSet):
                _visit_tree(childNodeId, self._tree, None, _RemoveNodeVisitor())
                self._viewCommandQueue.put(
                    view_commands.RemovePackagesTreeNode(childNodeId))
            for childNodeId in newChildrenSet.difference(curChildrenSet):
                self._tree[childNodeId] = [False]
            # fix up child graph
            predChildNodeId = None
            for childNodeId in newChildren:
                self._tree[childNodeId][_PRED_NODE_ID_IDX] = predChildNodeId
                predChildNodeId = childNodeId

    def update_filter(self, filterMask):
        _logger.debug("updating filter mask to %s", filterMask)
        def is_filter_mask_current():
            return filterMask == self._pendingFilterMask
        self._filter.set_active_mask(filterMask)
        _visit_tree(model.ROOT_NODE_ID, self._tree, is_filter_mask_current,
                   _SyncVisibleNodesVisitor(self._tree, self._filter.visibleNodeIds,
                                            self._viewCommandQueue))

    def update_search_string(self, searchString):
        _logger.debug("updating search string to %s", searchString)
        def is_search_string_current():
            return searchString == self._pendingSearchString
        if searchString is None or len(searchString) == 0:
            expression = None
            self._filter.apply_search(None)
        else:
            expression = re.compile(searchString, re.IGNORECASE)
            # walk the tree, matching nodes against the search string, stopping early if
            # the search string goes out of date
            searchVisitor = _SearchVisitor(self._tree, expression)
            _visit_tree(model.ROOT_NODE_ID, self._tree, is_search_string_current,
                       searchVisitor)
            self._filter.apply_search(searchVisitor.matchedNodeIds)
        _visit_tree(model.ROOT_NODE_ID, self._tree, is_search_string_current,
                   _SyncVisibleNodesVisitor(self._tree, self._filter.visibleNodeIds,
                                            self._viewCommandQueue))
        self._searchExpression = expression

    def _matches_search_expression(self, nodeAttributes):
        if self._searchExpression is None:
            return True
        return self._searchExpression.search(nodeAttributes.label)


#class PackageContentsTreeDiffEngine(_DiffEngine):
    #def __init__(self, generalTabManager, fileDetailsManager, viewCommandQueue):
        #_DiffEngine.__init__(self, generalTabManager, viewCommandQueue)
        #self._fileDetailsManager = fileDetailsManager
        #self._pendingDetailsTabId = presenter.DetailsTabIds.NONE
        #self._detailsTabId = presenter.DetailsTabIds.NONE

    #def set_pending_details_tab_selection(self, detailsTabId):
        #self._pendingDetailsTabId = detailsTabId
        # TODO: inform general tab manager

    #def apply_update(self, update):
        #pass
