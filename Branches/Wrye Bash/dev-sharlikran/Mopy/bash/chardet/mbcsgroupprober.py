######################## BEGIN LICENSE BLOCK ########################
# The Original Code is Mozilla Universal charset detector code.
#
# The Initial Developer of the Original Code is
# Netscape Communications Corporation.
# Portions created by the Initial Developer are Copyright (C) 2001
# the Initial Developer. All Rights Reserved.
#
# Contributor(s):
#   Mark Pilgrim - port to Python
#   Shy Shalom - original C code
#   Proofpoint, Inc.
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
# 02110-1301  USA
######################### END LICENSE BLOCK #########################

from charsetgroupprober import CharSetGroupProber
from utf8prober import UTF8Prober
from sjisprober import SJISProber
from eucjpprober import EUCJPProber
from gb2312prober import GB2312Prober
from euckrprober import EUCKRProber
from big5prober import Big5Prober
from euctwprober import EUCTWProber

class MBCSGroupProber(CharSetGroupProber):
    def __init__(self):
        CharSetGroupProber.__init__(self)
        self._mProbers = [ \
            UTF8Prober(),
            SJISProber(),
            EUCJPProber(),
            GB2312Prober(),
            EUCKRProber(),
            Big5Prober()
            #EUCTWProber(),
            ]
        self.reset()

# Local modifications
# 
# 12/20/2011 by Lojack for Wrye Bash:
#   removed the EUC-TW prober, due to Python 2.7.2 not having the EUC-TW codec.
