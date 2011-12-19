; Wrye Python 05.nsi
;  Installs Wrye Bash Python 2.7 prerequisites

;--------------------------------
; Includes

    !include "mui2.nsh"

;--------------------------------
; Basic Installer Info

    Name "Wrye Python 05"
    OutFile "Wrye Python 05.exe"
    InstallDir "$EXEDIR\Wrye Python"
    RequestExecutionLevel admin	    ; Request Admin rights for Vista/7

    VIProductVersion 5.0.0.0
    VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Wrye Python 05"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Wrye Bash development team"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Python 2.7 installer for Wrye Bash"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" ""
    VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "5.0"

    SetCompressor /SOLID lzma

;--------------------------------
; Page List

  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH

;--------------------------------
; Languages

  !insertmacro MUI_LANGUAGE "English"

;--------------------------------
; The Installation Sections

    Section "Python 2.7.2" SecPython
        SetOutPath $INSTDIR
        File ".\2.7\python-2.7.2.msi"
        ExecWait '"msiexec" /i "$INSTDIR\python-2.7.2.msi"'
    SectionEnd

    Section "wxPython 2.8.12.1" SecwxPython
        SetOutPath $INSTDIR
        File ".\2.7\wxPython2.8-win32-unicode-2.8.12.1-py27.exe"
        ExecWait "$INSTDIR\wxPython2.8-win32-unicode-2.8.12.1-py27.exe"
    SectionEnd

    Section "PyWin32 2.16" SecPyWin32
        SetOutPath $INSTDIR
        File ".\2.7\pywin32-216.win32-py2.7.exe"
        ExecWait "$INSTDIR\pywin32-216.win32-py2.7.exe"
    SectionEnd

    Section "ComTypes 0.6.2" SecComTypes
        SetOutPath $INSTDIR
        File ".\Common\comtypes-0.6.2.win32.exe"
        ExecWait "$INSTDIR\comtypes-0.6.2.win32.exe"
    SectionEnd

    Section "Cleanup installers" SecCleanup
        Delete "$INSTDIR\python-2.7.2.msi"
        Delete "$INSTDIR\wxPython2.8-win32-unicode-2.8.12.1-py27.exe"
        Delete "$INSTDIR\pywin32-216.win32-py2.7.exe"
        Delete "$INSTDIR\comtypes-0.6.2.win32.exe"
    SectionEnd        

;--------------------------------
; Descriptions

LangString DESC_SecPython   ${LANG_ENGLISH} "The main install for Python.  It provides the base interpreter/compiler and the basic libraries."
LangString DESC_SecwxPython ${LANG_ENGLISH} "This is the GUI library that Wrye Bash uses."
LangString DESC_SecPyWin32  ${LANG_ENGLISH} "This is used by BAIN Wizards to detect what version of OBSE/OBGE you have."
LangString DESC_SecComTypes ${LANG_ENGLISH} "This is needed for the PM Archives tab."
LangString DESC_SecCleanup  ${LANG_ENGLISH} "Delete the installers after running them."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SecPython}   $(DESC_SecPython)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecwxPython} $(DESC_SecwxPython)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecPyWin32}  $(DESC_SecPyWin32)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecComTypes} $(DESC_SecComTypes)
  !insertmacro MUI_DESCRIPTION_TEXT ${SecCleanup}  $(DESC_SecCleanup)
!insertmacro MUI_FUNCTION_DESCRIPTION_END