Skyrim Viewer 0.3.5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

What is it?

A bare bones application for viewing the structure of ESP and ESM files for
Skyrim.  It has basic record parsing (with a few probems), and parses the
Strings files associated with each plugin.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Setup:

Just extract it somewhere.  Everything else should Just Work(TM).  If not, let
me know.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Interface:

Menu:
* You can access all of the toolbar functions from here
* You can change some settings:
  - Skyrim Path: change the directory that Skyrim Viewer looks for mods
  - Load Masters: When loading mods, automatically load any masters as well
  - Load Strings: When loading mods, automatically load the strings files
  - Remove Global Config Overrides:
        Force all records to use the Global Definition for any subrecord types
        stored in ALL.ini

Toolbar:
* Open: Load some plugins!  It will default to loading those listed as active
        in your 'plugins.txt' file.
* Reload Record Definitions: Reload all the .INI files in the 'records'
        directory.  Useful for resetting any changes you have made through the
        UI, or updating the UI with any manual edits you have made.
* Save Record Definitions: Save any changes you have made through the UI to
        file.  Currently this saves EVERY record definition, even ones you
        have not changed.
* Reload strings: (Re)loads the .STRINGS, .DLSTRINGS, and .ILSTRINGS files
        associated with the mods you have open for viewing.
        with the mods you have open for viewing.

Search boxes:
* Left search box: Search the Mod list for a FormId or Editor Id
* Right search box: Search an open string table for text


~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Record Definitions:

These are stored in the .INI files in your 'records' directory.  The easies way
to modify them is through the UI. To do this, right click on a field you want
to modify.  A popup menu will appear with some options, and a preview of what
each of those options will display:

* Goto Record -> If the field can be interpreted as a FormId, and that FormId
      is found in one of the open files, this will take you to that record.
* CString -> Display as a zero-terminated string.
* PString -> Display as a Pascal string, where the first byte determines the
      string's length.
* LString -> Display as a "Long" string.  This is the format in the .DLSRINGS
      and .ILSTRINGS files, where the first 4 bytes (UInt32) determines the
      string's length.
* LStringA -> A special array type of LStrings, where the first 4 bytes (UInt32)
      is the number of strings "x".  The next "x" 4 bytes are the strings'
      lengths.  Then the "x" strings are read using those lengths.
* StringId -> Read 4 bytes as a StringId, then try to look up a string in the
      mod's string table.
* FormId -> Display as a FormId.
* FormIdA -> Display as a list of FormIds.
* Float -> Display as a floating point decimal
* UInt32 -> Display as an unsigned integer (4 bytes)
* Int32 -> Display as a signed integer (4 bytes)
* UInt16 -> Display as an unsigned integer (2 bytes)
* Int16 -> Display as a signed integer (2 bytes)
* UByte -> Display as an unsigned byte
* Byte -> Display as a signed byte
* Raw -> Display as raw data
* Custom -> Build a "compound" data type via a dialog.  The Custom dialog is
     where you define flags.
* Use Global Definition:
   - If enabled, any changed made to this subrecord definition will be saved to
     the Global Definition
   - If disabled, any changes made will be saved to the record's definition
     file.  Enabling this option will restore the Global Definition settings
     for this record type.
* Save as Global Definition:
   - Save the current subrecord definition to the Global Definition file.

You will only be able to select a display option if the data is of the required
length to be interpreted in that way.

To give the field a name, just click in the 'Name' field for it and it will
become editable.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Bugs:

- Still a few problems with loading records.  For example, no HAIR records are
  loaded at all.

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Changelog:

0.3.5: 11/16/2011
 - Included record's Editor Id in left plugin view window
 - Using the left search box above the plugin view window will now search for
   Editor Id's in addition to FormId's
 - Using the right search box you can search an open string table for text
 - Added the the ability to define flags
 - The record display will now show the record header in addition to all
   of its subrecord.

0.3.1: 11/16/2011
 - Bugfix for when the Steam directory wasn't automatically detected.
 - The error log will now be removed on startup, so only the latest errors
   will be shown.

0.3: 11/16/2011
 - Fixed the Cancel button.  To do this, the packaging method had to change
   slightly, so now you will see a 'python27.dll' in the package.
 - Added more types for displaying
 - Added the main menu, with settings
 - Added the settings to the INI
 - Settings will be saved to the INI automatically on closing
 - Added an option to sort the records by name, instead of order they occur
 - Improved the record definitions a bit.
 - Changed the was the 'Strings' suboption is displayed.  There are a lot 
   (~75,000) strings in Skyrim's strings files.  Trying to display them all
   at once cause a lot of UI lag.  They are now broken up into groups of 200.

0.2: 11/15/2011
 - Fixed record decompression
 - Added FormId resolution
 - Added multiple options to the Load dialog
 - Added the ability to specify a compound type for subrecords
 - Fixed startup when the Steam directory cannot be detected
 - Created an icon for the program

0.1: 11/14/2011
 - Initial release

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Credits:

Lojack!