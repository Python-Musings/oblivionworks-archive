; Wrye Python 08.nsi
;  Installs Wrye Bash Python 2.7 prerequisites

;--------------------------------
; Includes

    !include "MUI2.nsh"

;--------------------------------
; Basic Installer Info

    Name "Wrye Python 08"
    OutFile "Wrye Python 08.exe"
    InstallDir "$TEMP\PythonInstallers"
    RequestExecutionLevel admin	    ; Request Admin rights for Vista/7

    VIProductVersion 7.0.0.0
    VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Wrye Python 08"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Wrye Bash development team"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Python 2.7 installer for Wrye Bash"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" ""
    VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "8.0"

    SetCompressor /SOLID lzma

;--------------------------------
; Page List

  !define MUI_HEADERIMAGE
  !define MUI_HEADERIMAGE_BITMAP "$PLUGINSDIR\wrye_monkey_150x57.bmp"
  !define MUI_HEADERIMAGE_RIGHT
  !define MUI_WELCOMEFINISHPAGE_BITMAP "$PLUGINSDIR\wrye_monkey_164x314.bmp"
  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
; The Installation Sections

    Section "python-2.7.3" SecPython
        SetOutPath $INSTDIR
        File "python-2.7.3.msi"
        HideWindow
        ExecWait '"msiexec" /i "$INSTDIR\python-2.7.3.msi"'
        BringToFront
    SectionEnd

    Section "wxPython 2.8.12.1" SecwxPython
        SetOutPath $INSTDIR
        File "wxPython2.8-win32-unicode-2.8.12.1-py27.exe"
        HideWindow
        ExecWait "$INSTDIR\wxPython2.8-win32-unicode-2.8.12.1-py27.exe"
        BringToFront
    SectionEnd

    Section "PyWin32 2.18" SecPyWin32
        SetOutPath $INSTDIR
        File "pywin32-218.win32-py2.7.exe"
        HideWindow
        ExecWait "$INSTDIR\pywin32-218.win32-py2.7.exe"
        BringToFront
    SectionEnd

    Section "ComTypes 0.6.2" SecComTypes
        SetOutPath $INSTDIR
        File "comtypes-0.6.2.win32.exe"
        HideWindow
        ExecWait "$INSTDIR\comtypes-0.6.2.win32.exe"
        BringToFront
    SectionEnd
    
    Section "Cleanup installers" SecCleanup
        Delete "$INSTDIR\*.*"
        RMDir $INSTDIR
    SectionEnd        

;--------------------------------
; Descriptions

LangString DESC_SecPython ${LANG_ENGLISH} "The main install for Python. It provides the base interpreter/compiler and the basic libraries."
LangString DESC_SecwxPython ${LANG_ENGLISH} "This is the GUI library that Wrye Bash uses."
LangString DESC_SecPyWin32 ${LANG_ENGLISH} "This is used by BAIN Wizards to detect what version of script extenders/graphics extenders you have."
LangString DESC_SecComTypes ${LANG_ENGLISH} "This is needed for the PM Archives tab."
LangString DESC_SecCleanup ${LANG_ENGLISH} "Delete the installers after running them."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecPython} $(DESC_SecPython)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecwxPython} $(DESC_SecwxPython)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecPyWin32} $(DESC_SecPyWin32)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecComTypes} $(DESC_SecComTypes)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCleanup} $(DESC_SecCleanup)
!insertmacro MUI_FUNCTION_DESCRIPTION_END