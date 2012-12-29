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
#--Standard
import atexit
#--Local
from src.bolt import Path
from src.bolt.Path import GPath, PathUnion
import src.bass as bass

def _OnExit():
    """Cleans out any temporary files or directories created by Bash."""
    try:
        bass.dirs['temp'].rmtree()
    except Exception as e:
        pass

def InitDirs():
    # Initialize directories.  To make sure Bash follows symlinks and stuff
    # correctly, we'll use the 'realpath' portion of this.'
    bass.dirs = {}
    dirs = bass.dirs

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

    # appdata - User's Local App Data directoyr + Wrye Bash
    dirs['appdata'] = Path.LocalAppData.join('Wrye Bash')

    # temp - Wrye Bash's base directory for all temp files/folders this run
    dirs['temp'] = Path.makeTempdir(prefix='WryeBash_')
    # Now that our temp dir is created, register our cleanup function
    # that removes temporary directories
    atexit.register(_OnExit)

    # user.bash - Wrye Bash subdirectory of user's directory
    dirs['user.bash'] = dirs['user'].join('Wrye Bash')

    # l10n - contains the uncompiled translation files shipped with bash
    if bass.opts.portable:
        dirs['l10n'] = dirs['app'].join('l10n')
    else:
        dirs['l10n'] = PathUnion(dirs['user.bash'].join('l10n'),
                                 dirs['app'].join('l10n'),
                                 mode=PathUnion.MODE_TIMESTAMP)

    # l10n.compiled - contains the compiled translation files
    if bass.opts.portable:
        dirs['l10n.compiled'] = dirs['app'].join('l10n')
    else:
        dirs['l10n.compiled'] = dirs['appdata'].join('l10n')
