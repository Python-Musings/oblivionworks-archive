# -*- coding: utf-8 -*-
# src/dirs.py
#
# GPL License and Copyright Notice =============================================
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
#  Wrye Bash copyright (C) 2005, 2006, 2007, 2008, 2009 Wrye
#
# ==============================================================================

"""Initialize all the directories Wrye Bash will possibly need."""

# Imports ----------------------------------------------------------------------
from src.bolt import Path
from src.bolt.Path import GPath, PathUnion

# Globals ----------------------------------------------------------------------
# dirs holds the list of all directories setup.  Keys are string objects
dirs = {}

def InitDirs():
    # Initialize directories.  To make sure Bash follows symlinks and stuff
    # corectly, we'll use the 'realpath' portion of this.

    # app - directory that Wrye Bash is installed to.  Older version of Bash
    #       this was the path to the *game*.  Now it's Wrye Bash, mostly
    #       because I'm debating dropping the 'Mopy' folder, in which case
    #      'mopy' as a key to the Bash folder doesn't make sense.
    dirs['app'] = Path.getcwd().realpath

    # src - source directory.  Probably won't need this since I'm shuffling
    #       directories around
    ##dirs['src'] = dirs['app'].join('src')

    # bin - directory with compiled items (dlls, exes, etc)
    dirs['bin'] = dirs['app'].join('bin')

    # user - User's My Documents directory
    dirs['user'] = Path.Personal

    # user.bash - Wrye Bash subdirectory of user's driectory
    dirs['user.bash'] = dirs['user'].join('Wrye Bash')

    # l10n - contains the uncompiled translation files shipped with bash
    dirs['l10n'] = PathUnion(dirs['app'].join('l10n'),
                             dirs['user.bash'].join('l10n'))
    