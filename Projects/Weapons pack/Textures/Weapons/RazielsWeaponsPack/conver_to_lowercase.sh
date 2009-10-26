#!/bin/sh
#
# To make this Bourne shell script operative apply once:
#  chmod 700 namedown
#  rehash

# Alter the general path for BSD vs. Sys V compatibility
#
if [ -d /usr/ucb ] ; then
  PATH=/usr/ucb:$PATH ; export PATH
fi

case $# in

  0)
    echo
    echo "====================================================="
    echo "namedown, Convert file names to lower case and ; to ."
    echo "By Hannu Hirvonen and Timo Salmi Sun 18-Mar-2001"
    echo "http://www.uwasa.fi/~ts/ and http://www.uwasa.fi/~hh/"
    echo "====================================================="
    echo
    echo "Usage: namedown [FILENAME(S)]"
    echo
    echo "Converts file names only.  Directory names are not affected"
    echo
    ;;

  *)
    for oldname in $*
    do
      newfile=`basename ${oldname} | tr '[A-Z;]' '[a-z.]'`
      dirname=`dirname ${oldname}`
      newname="${dirname}/${newfile}"
      oldname="${dirname}/`basename ${oldname}`"
#Don't convert a file into itself
      if [ "${newname}" = "${oldname}" ]; then
        echo > /dev/null
#Don't convert directory names
      elif [ -d "${oldname}" ]; then
        echo > /dev/null
#Don't convert if the file does not exist
      elif [ ! -f "${oldname}" ]; then
        echo > /dev/null
#Don't overwrite existing files
      elif [ -f "${newname}" ]; then
        echo "${oldname} not converted, file ${newname} already exists" 2>&1
#Don't move to subdirectories if they happen to exist
      elif [ -d "${newname}" ]; then
        echo "${oldname} not converted, directory ${newname} already exists" 2>&1
#Do it
      else
        mv "${oldname}" "${newname:-${oldname}}"
        echo "File ${oldname} converted to file ${newname:-${oldname}}"
      fi
   done
    ;;
esac
