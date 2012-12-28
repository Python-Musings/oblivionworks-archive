# -*- coding: utf-8 -*-
# src/barg.py
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

"""This module parses the command line for options."""

# Imports ----------------------------------------------------------------------
import argparse
import src.bass as bass

#-------------------------------------------------------------------------------
def parse():
    parser = argparse.ArgumentParser(prog='Wrye Bash',
                                     add_help=False)
    parser.add_argument('-d','--debug',
                        dest='debug',
                        action='store_true',
                        default=False,
                        help='enabled debug mode')
    parser.add_argument('-p','--portable',
                        dest='portable',
                        action='store_true',
                        default=False,
                        help='enable portable mode')
    bass.opts,extra = parser.parse_known_args()
