# -*- coding: utf-8 -*-
# src/bolt/OneInstanceChecker.py
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

"""This module is a helper function for ensuring only one instance of an
   application is running at a time.  Adapted from:
     http://www.effbot.org/librarybook/msvcrt-example-3.py
   """

# Imports ----------------------------------------------------------------------
import time
import os
import atexit

# Globals ----------------------------------------------------------------------
pidPath = None
lockFp = None

def _OnExit():
    """Cleanup the lock file when exiting the program."""
    try:
        os.close(lockFp)
        os.remove(pidPath)
    except OSError as e:
        print(e)

def Start(lockDir=None,restarting=False,timeout=10):
    """Check for an instance of this program already running.  If there is,
       return False. Otherwise, create a lock file and return True.
       If restarting is True, will wait timeout seconds before attempting."""
    global pidPath, lockFp
    lockDir = lockDir if lockDir else os.getcwd()
    pidPath = os.path.join(lockDir,'pidFile.tmp')
    lockFp = None

    if restarting:
        # Wait timeout seconds for previous instance to close
        t = time.time()
        while (time.time()-t < timeout) and os.path.exists(pidPath):
            time.sleep(1)

    try:
        # If a pidFile exists, try to remove it.  This will fail if
        # another instance already has an open lock on it.
        if os.path.exists(pidPath):
            os.remove(pidPath)
        lockFp = os.open(pidPath,os.O_CREAT|os.O_EXCL|os.O_RDWR)
        os.write(lockFp, bytes('%d' % os.getpid(),'utf8'))
    except OSError as e:
        # Lock file couldn't be removed, another instance must
        # have a lock on it.
        msg = _('Only one instance of Wrye Bash may be run.')
        print(msg)
        import src.balt as balt
        balt.ShowError(None,msg)
        return False
    atexit.register(_OnExit)
    return True
