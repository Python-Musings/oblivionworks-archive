# -*- coding: utf-8 -*-
# src/bolt/Translations.py
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

"""This module handles translation functions for Wrye Bash."""

"""TODO:
      1) Store the .txt files in Mopy\l10n
         Store the compiled .mo files in the users local app data
         directory.  This will avoid bash trying to create new files
         in its own directory, for if Bash is installed in a UAC location
      2) Actually implement the "update from old txt" part of 'Dump'
   """

# Imports ----------------------------------------------------------------------
import locale
import gettext
import shutil
import sys
import os
import traceback
import subprocess

#--Setup locale
if locale.getlocale() == (None,None):
    locale.setlocale(locale.LC_ALL,'')

# Functions --------------------------------------------------------------------
def Dump(language,outPath,*files):
    """Dumps translatable string from *files to a new txt file in outPath,
       named based on language.  If an already existing translation file exists
       for that language, the new one will be updated with any matching strings
       from the previous one."""
    #--Determine files to dump
    if not files:
        # No files specified.  Assume this file is located in root/src/bolt
        # and we'll dump every .py file in root recursively
        files = []
        mopy = os.path.dirname(os.path.dirname(os.path.dirname(
                               os.path.realpath(__file__))))
        for root,dirs,fnames in os.walk(mopy):
            for file in files:
                body,ext = os.path.splitext(file)
                ext = os.path.normcase(ext)
                if ext == '.py':
                    files.append(os.path.join(root,file))
    #--Output files
    outTxt = '%sNEW.txt' % language
    fullTxt = os.path.join(outPath,outTxt)
    tmpTxt = os.path.join(outPath,'%NEW.tmp' % language)
    oldTxt = os.path.join(outPath,'%s.txt' % language)
    #--First dump a fresh translation file
    args = ['p','-a','-o',fullTxt]
    args.extend(files)
    if hasattr(sys,'frozen'):
        # Frozen app, the tool scripts aren't accessable
        # Instead, they're included in the package as an
        # importable module
        import pygettext
        old_argv = sys.argv[:]
        sys.argv = args
        pygettext.main()
        sys.argv = old_argv
    else:
        p = os.path.join(sys.prefix,'Tools','i18n','pygettext.py')
        args[0] = p
        subprocess.call(args,shell=True)
    return outTxt

def Install(language=None,path=None):
    """Install translation for language.  If language is not specified,
       installs a translation for the default language."""
    if not language:
        language = locale.getlocale()[0].split('_',1)[0]
    path = path if path else 'l10n'
    if language.lower() == 'german':
        language = 'de'
    txt,po,mo = (os.path.join(path,language+ext)
                 for ext in ('.txt','.po','.mo'))
    #--Test for no translation for the language
    if not os.path.exists(txt) and not os.path.exists(mo):
        if language.lower() != 'english':
            print('No translation file for language:', language)
        trans = gettext.NullTranslations()
    else:
        try:
            # See if translation needs to be recompiled
            if not os.path.exists(mo) or (os.path.getmtime(txt) >
                                          os.path.getmtime(mo)):
                shutil.copy(txt,po)
                args= ['m','-o',mo,po]
                if hasattr(sys,'frozen'):
                    # Same thing as for 'Dump' for frozen
                    # apps.
                    import msgfmt
                    old_argv = sys.argv[:]
                    sys.argv = args
                    msgfmt.main()
                    sys.argv = old_argv
                else:
                    m = os.path.join(sys.prefix,'Tools','i18n','msgfmt.py')
                    args[0] = m
                    subprocess.call(args,shell=True)
                os.remove(po)
            # Create GNU translations
            with open(mo,'rb') as ins:
                trans = gettext.GNUTranslations(ins)
        except:
            print('Error loading translation file for', language)
            traceback.print_exc()
            trans = gettext.NullTranslations()
    # Install
    trans.install()