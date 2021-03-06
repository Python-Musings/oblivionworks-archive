; Wrye Bash.nsi
;-------------------------------- Includes:
    !include MUI2.nsh
    !include LogicLib.nsh
    !include nsDialogs.nsh
    !include WordFunc.nsh

    ;--Information passed by the packaging script
    !ifndef WB_NAME
        !define WB_NAME "Wrye Bash 292"
    !endif
    !ifndef WB_FILEVERSION
        !define WB_FILEVERSION "0.2.9.2"
    !endif
;-------------------------------- Basic Installer Info:
    Name "${WB_NAME}"
    OutFile "${WB_NAME} -- Installer.exe"
    ; Request application privileges for Windows Vista
    RequestExecutionLevel admin
    VIProductVersion ${WB_FILEVERSION}
    VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "Wrye Bash"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "CompanyName" "Wrye Bash development team"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "� Wrye"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "Installer for ${WB_NAME}"
    VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${WB_FILEVERSION}"
    SetCompressor /SOLID lzma
;-------------------------------- Variables:
    Var Dialog
    Var Label
    Var Empty
    Var True
    Var Path_OB
    Var Path_Nehrim
    Var Path_Ex1
    Var Path_Ex2

    ;Game specific Data:
    Var Check_OB
    Var Check_Nehrim
    Var Check_Extra
    Var Check_Ex1
    Var Check_Ex2
    Var CheckState_OB
    Var CheckState_Nehrim
    Var CheckState_Extra
    Var CheckState_Ex1
    Var CheckState_Ex2
    Var Check_OB_Py
    Var Check_Nehrim_Py
    Var Check_Ex1_Py
    Var Check_Ex2_Py
    Var CheckState_OB_Py
    Var CheckState_Nehrim_Py
    Var CheckState_Ex1_Py
    Var CheckState_Ex2_Py
    Var Check_OB_Exe
    Var Check_Nehrim_Exe
    Var Check_Ex1_Exe
    Var Check_Ex2_Exe
    Var CheckState_OB_Exe
    Var CheckState_Nehrim_Exe
    Var CheckState_Ex1_Exe
    Var CheckState_Ex2_Exe
    Var Reg_Value_OB_Py
    Var Reg_Value_Nehrim_Py
    Var Reg_Value_Ex1_Py
    Var Reg_Value_Ex2_Py
    Var Reg_Value_OB_Exe
    Var Reg_Value_Nehrim_Exe
    Var Reg_Value_Ex1_Exe
    Var Reg_Value_Ex2_Exe
    ;Var CheckState_RemoveUserFiles
    ;Var Check_RemoveUserFiles
    Var Check_Python
    Var Check_wx
    Var Check_pywin32
    Var Check_Comtypes
    Var CheckState_Python
    Var CheckState_wx
    Var CheckState_pywin32
    Var CheckState_Comtypes
    Var PathDialogue_OB
    Var PathDialogue_Nehrim
    Var PathDialogue_Ex1
    Var PathDialogue_Ex2
    Var Browse_OB
    Var Browse_Nehrim
    Var Browse_Ex1
    Var Browse_Ex2
    Var Check_Readme
    Var Check_DeleteOldFiles
    Var Function_Browse
    Var Function_Extra
    Var Function_DirPrompt
    Var unFunction_Browse
    Var Python_Path
    Var Python_Ver
    Var Python_Comtypes
    Var Python_pywin32
    Var Python_wx
    Var Requirements
    Var PythonVersionInstall
    Var ExeVersionInstall
    Var Check_msvc
    Var Link_Python
    Var Link_Comtypes
    Var Link_vcredist
    Var Link_wx
    Var Link_pywin32
    Var MinVersion_Comtypes
    Var MinVersion_wx
    Var MinVersion_pywin32
;-------------------------------- Page List:
  !insertmacro MUI_PAGE_WELCOME
  Page custom PAGE_INSTALLLOCATIONS PAGE_INSTALLLOCATIONS_Leave
  Page custom PAGE_REQUIREMENTS PAGE_REQUIREMENTS_Leave
  !insertmacro MUI_PAGE_COMPONENTS
  !insertmacro MUI_PAGE_INSTFILES
  Page custom PAGE_FINISH PAGE_FINISH_Leave

  !insertmacro MUI_UNPAGE_WELCOME
  UninstPage custom un.PAGE_SELECT_GAMES un.PAGE_SELECT_GAMES_Leave
  !insertmacro MUI_UNPAGE_INSTFILES
;-------------------------------- Initialize Variables as required:
    Function un.onInit
        StrCpy $Empty ""
        StrCpy $True "True"
        ReadRegStr $Path_OB              HKLM "Software\Wrye Bash" "Oblivion Path"
        ReadRegStr $Path_Nehrim          HKLM "Software\Wrye Bash" "Nehrim Path"
        ReadRegStr $Path_Ex1             HKLM "Software\Wrye Bash" "Extra Path 1"
        ReadRegStr $Path_Ex2             HKLM "Software\Wrye Bash" "Extra Path 2"
        ReadRegStr $Reg_Value_OB_Py      HKLM "Software\Wrye Bash" "Oblivion Python Version"
        ReadRegStr $Reg_Value_Nehrim_Py  HKLM "Software\Wrye Bash" "Nehrim Python Version"
        ReadRegStr $Reg_Value_Ex1_Py     HKLM "Software\Wrye Bash" "Extra Path 1 Python Version"
        ReadRegStr $Reg_Value_Ex2_Py     HKLM "Software\Wrye Bash" "Extra Path 2 Python Version"
        ReadRegStr $Reg_Value_OB_Exe     HKLM "Software\Wrye Bash" "Oblivion Standalone Version"
        ReadRegStr $Reg_Value_Nehrim_Exe HKLM "Software\Wrye Bash" "Nehrim Standalone Version"
        ReadRegStr $Reg_Value_Ex1_Exe    HKLM "Software\Wrye Bash" "Extra Path 1 Standalone Version"
        ReadRegStr $Reg_Value_Ex2_Exe    HKLM "Software\Wrye Bash" "Extra Path 2 Standalone Version"
        FunctionEnd
    Function .onInit
        StrCpy $Empty ""
        StrCpy $True "True"
        StrCpy $MinVersion_Comtypes '0.6.2'
        StrCpy $MinVersion_wx '2.8.10'
        StrCpy $MinVersion_pywin32 '213'
        ReadRegStr $Path_OB              HKLM "Software\Wrye Bash" "Oblivion Path"
        ReadRegStr $Path_Nehrim          HKLM "Software\Wrye Bash" "Nehrim Path"
        ReadRegStr $Path_Ex1             HKLM "Software\Wrye Bash" "Extra Path 1"
        ReadRegStr $Path_Ex2             HKLM "Software\Wrye Bash" "Extra Path 2"
        ReadRegStr $Reg_Value_OB_Py      HKLM "Software\Wrye Bash" "Oblivion Python Version"
        ReadRegStr $Reg_Value_Nehrim_Py  HKLM "Software\Wrye Bash" "Nehrim Python Version"
        ReadRegStr $Reg_Value_Ex1_Py     HKLM "Software\Wrye Bash" "Extra Path 1 Python Version"
        ReadRegStr $Reg_Value_Ex2_Py     HKLM "Software\Wrye Bash" "Extra Path 2 Python Version"
        ReadRegStr $Reg_Value_OB_Exe     HKLM "Software\Wrye Bash" "Oblivion Standalone Version"
        ReadRegStr $Reg_Value_Nehrim_Exe HKLM "Software\Wrye Bash" "Nehrim Standalone Version"
        ReadRegStr $Reg_Value_Ex1_Exe    HKLM "Software\Wrye Bash" "Extra Path 1 Standalone Version"
        ReadRegStr $Reg_Value_Ex2_Exe    HKLM "Software\Wrye Bash" "Extra Path 2 Standalone Version"

        ${If} $Path_OB == $Empty
            ReadRegStr $Path_OB HKLM "Software\Bethesda Softworks\Oblivion" "Installed Path"
            ${If} $Path_OB == $Empty
                ReadRegStr $Path_OB HKLM "SOFTWARE\Wow6432Node\Bethesda Softworks\Oblivion" "Installed Path"
            ${EndIf}
        ${Else}
            StrCpy $CheckState_OB ${BST_CHECKED}
        ${EndIf}
        ${If} $Path_Nehrim == $Empty
            ReadRegStr $Path_Nehrim HKLM "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Nehrim - At Fate's Edge_is1" "InstallLocation"
        ${Else}
            StrCpy $CheckState_Nehrim ${BST_CHECKED}
        ${EndIf}
        ${If} $Path_Ex1 != $Empty
            StrCpy $CheckState_Extra ${BST_CHECKED}
            StrCpy $CheckState_Ex1 ${BST_CHECKED}
        ${EndIf}
        ${If} $Path_Ex2 != $Empty
            StrCpy $CheckState_Extra ${BST_CHECKED}
            StrCpy $CheckState_Ex2 ${BST_CHECKED}
        ${EndIf}
        FunctionEnd
;-------------------------------- Custom Installation Pages and their Functions:
    Function PAGE_REQUIREMENTS
        !insertmacro MUI_HEADER_TEXT $(PAGE_REQUIREMENTS_TITLE) $(PAGE_REQUIREMENTS_SUBTITLE)

        nsDialogs::Create 1018
            Pop $Dialog
        ${If} $Dialog == error
            Abort
        ${EndIf}

        IntOp $0 0 + 0
        ${NSD_CreateLabel} 0 $0u 100% 32u "Checking for requirements (Python && some Python addons for Python version of Wrye Bash, MS Visual C ++ redist for Standalone Executable Wrye Bash Version). This can quite easily give a false detection that you need some of those since if they aren't installed with the exact same version(s)/location(s) that this is guessing it may not find it."
        IntOp $0 $0 + 34
        ${If} $PythonVersionInstall == $True
            ReadRegStr $Python_Path HKLM "SOFTWARE\Python\PythonCore\2.6\InstallPath" ""
            ${If} $Python_Path != $Empty
                StrCpy $Python_Ver "26"
            ${Else}
                ReadRegStr $Python_Path HKLM "SOFTWARE\Python\PythonCore\2.5\InstallPath" ""
                ${If} $Python_Path != $Empty
                    StrCpy $Python_Ver "25"
                ${Else}
                    ReadRegStr $Python_Path HKLM "SOFTWARE\Python\PythonCore\2.7\InstallPath" ""
                    ${If} $Python_Path != $Empty
                        StrCpy $Python_Ver "27"
                    ${EndIf}
                ${EndIf}
            ${EndIf}
            ;Detect Python Components:
            ${If} $Python_Path != $Empty
                StrCpy $Python_Comtypes "1"
                ;Detect Comtypes:
                IfFileExists "$Python_Path\Lib\site-packages\comtypes\__init__.py" 0 +10
                    FileOpen $0 "$Python_Path\Lib\site-packages\comtypes\__init__.py" r
                    FileRead $0 $1
                    FileRead $0 $1
                    FileRead $0 $1
                    FileRead $0 $1
                    FileRead $0 $1
                    FileRead $0 $1
                    FileClose $0
                    StrCpy $Python_Comtypes $1 5 -8
                    ${VersionConvert} $Python_Comtypes "" $Python_Comtypes
                ${If} $Python_Ver == "25"
                    ReadRegStr $Python_wx HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wxPython2.8-ansi-py25_is1" "DisplayVersion"
                    ReadRegStr $1         HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\pywin32-py2.5" "DisplayName"
                    StrCpy $Python_pywin32 $1 3 -3
                ${ElseIf} $Python_Ver == "26"
                    ReadRegStr $Python_wx HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wxPython2.8-ansi-py26_is1" "DisplayVersion"
                    ReadRegStr $1         HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\pywin32-py2.6" "DisplayName"
                    StrCpy $Python_pywin32 $1 3 -3
                ${ElseIf} $Python_Ver == "27"
                    ReadRegStr $Python_wx HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\wxPython2.8-ansi-py27_is1" "DisplayVersion"
                    ReadRegStr $1         HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\pywin32-py2.7" "DisplayName"
                    StrCpy $Python_pywin32 $1 3 -3
                ${EndIf}
                ${VersionCompare} $MinVersion_pywin32 $Python_pywin32 $Python_pywin32
                ${VersionConvert} $Python_wx "+" $Python_wx
                ${VersionCompare} $MinVersion_wx $Python_wx $Python_wx
            ${EndIf}
            ${If} $Python_Path != $Empty
            ${AndIf} $Python_Comtypes != "1"
            ${AndIf} $Python_pywin32 != "1"
            ${AndIf} $Python_wx != "1"
                StrCpy $Requirements "Met"
               ${NSD_CreateLabel} 0 $0u 100% 16u "Congratulations the installer detects that you have a full install of all the Python prerequisites already! Please click 'Next' to continue."
                Pop $Label
                IntOp $0 $0 + 16
            ${Else}
                ${NSD_CreateLabel} 0 $0u 100% 40u "The installer cannot find the following required components. It is recommended (as in Wrye Bash probably won't work otherwise) that you either manually download and install them or that you let this installer download them for you and execute them so that you have to do the minimum work. Please check the component(s) that you are fine with this installer downloading and installing; or use the provided links to manually download and install."
                    Pop $Label
                IntOp $0 $0 + 41
                ${If} $Python_Path == $Empty
                    ${NSD_CreateCheckBox} 0 $0u 40% 13u "Python 2.6.6"
                        Pop $Check_Python
                        ${NSD_SetState} $Check_Python $CheckState_Python
                    IntOp $0 $0 + 2
                    ${NSD_CreateLink} 40% $0u 60% 8u  "Python.org 2.6.6 Download Page" ;http://www.python.org/download/releases/2.6.6/
                        Pop $Link_Python
                        ${NSD_OnClick} $Link_Python onClick_Link
                    IntOp $0 $0 + 11
                ${EndIf}
                ${If} $Python_wx == "1"
                    ${NSD_CreateCheckBox} 0 $0u 40% 13u "wxPython 2.8.12.0-ansi"
                        Pop $Check_wx
                        ${NSD_SetState} $Check_wx $CheckState_wx
                    IntOp $0 $0 + 2
                    ${NSD_CreateLink} 40% $0u 60% 8u  "wxPython.org Download Page" ;http://www.wxpython.org/download.php#stable
                        Pop $Link_wx
                        ${NSD_OnClick} $Link_wx onClick_Link
                    IntOp $0 $0 + 11
                ${EndIf}
                ${If} $Python_Comtypes == "1"
                    ${NSD_CreateCheckBox} 0 $0u 40% 13u "Python Comtypes 0.6.2"
                        Pop $Check_Comtypes
                        ${NSD_SetState} $Check_Comtypes $CheckState_Comtypes
                    IntOp $0 $0 + 2
                    ${NSD_CreateLink} 40% $0u 60% 8u "Comtypes' Sourceforge Download Page" ;http://sourceforge.net/projects/comtypes/files/comtypes/0.6.2/
                        Pop $Link_Comtypes
                        ${NSD_OnClick} $Link_Comtypes onClick_Link
                    IntOp $0 $0 + 11
                ${EndIf}
                ${If} $Python_pywin32 == "1"
                    ${NSD_CreateCheckBox} 0 $0u 40% 13u "Python Extensions for Windows (PyWin32)"
                        Pop $Check_pywin32
                        ${NSD_SetState} $Check_pywin32 $CheckState_pywin32
                    IntOp $0 $0 + 2
                    ${NSD_CreateLink} 40% $0u 60% 8u  "PyWin32 216 Sourceforge Download Page" ;http://sourceforge.net/projects/pywin32/files/pywin32/Build216/
                        Pop $Link_pywin32
                        ${NSD_OnClick} $Link_pywin32 onClick_Link
                    IntOp $0 $0 + 11
                ${EndIf}
            ${EndIf}
        ${EndIf}
        ${If} $ExeVersionInstall == $True
            StrCpy $9 ''
            IfFileExists "$SYSDIR\MSVCR90.DLL" 0 +2
                StrCpy $9 "Installed"
            ${If} $9 == $Empty
                ${NSD_CreateLabel} 0 $0u 40% 17u "MSVC Redistributable 2008 (Must Manually Download && Install)"
                    Pop $Check_msvc
                IntOp $0 $0 + 2
                ${NSD_CreateLink} 40% $0u 60% 8u  "MSVC 2008 Redist Download Page" ;http://www.microsoft.com/downloads/details.aspx?familyid=a5c84275-3b97-4ab7-a40d-3802b2af5fc2
                    Pop $Link_vcredist
                    ${NSD_OnClick} $Link_vcredist onClick_Link
                IntOp $0 $0 + 11
            ${EndIf}
        ${EndIf}
        nsDialogs::Show

        FunctionEnd
    Function PAGE_REQUIREMENTS_Leave
        ${If} $Requirements != "Met"
            ${NSD_GetState} $Check_Python $CheckState_Python
            ${NSD_GetState} $Check_wx $CheckState_wx
            ${NSD_GetState} $Check_Comtypes $CheckState_Comtypes
            ${NSD_GetState} $Check_pywin32 $CheckState_pywin32

            ${If} $CheckState_Python == ${BST_CHECKED}
                SetOutPath "$TEMP"
                NSISdl::download http://www.python.org/ftp/python/2.6.6/python-2.6.6.msi "$TEMP\Python-2.6.6.msi"
                Pop $R0
                ${If} $R0 == "success"
                    Exec '"$TEMP\Python-2.6.6.msi" /quiet'
                ${Else}
                    MessageBox MB_OK "Python download failed, please try running installer again or manually downloading."
                ${EndIf}
            ${EndIf}
            ${If} $CheckState_wx == ${BST_CHECKED}
                ${If} $Python_Ver == "27"
                    NSISdl::download http://downloads.sourceforge.net/project/wxpython/wxPython/2.8.12.0/wxPython2.8-win32-ansi-2.8.12.0-py27.exe?r=http%3A%2F%2Fwww.wxpython.org%2Fdownload.php&ts=1307976102&use_mirror=cdnetworks-us-2 "$TEMP\wxPython.msi"
                ${ElseIf} $Python_Ver == "25" ;last version of wxPython to suport py2.5x is 2.8.11 so download that version if you're using Python 2.5x
                    NSISdl::download http://downloads.sourceforge.net/project/wxpython/wxPython/2.8.11.0/wxPython2.8-win32-ansi-2.8.11.0-py25.exe?r=http%3A%2F%2Fwxpython.org%2Fdownload.php&ts=1291222636&use_mirror=superb-sea2 "$TEMP\wxPython.msi"
                ${Else}
                    NSISdl::download http://downloads.sourceforge.net/project/wxpython/wxPython/2.8.12.0/wxPython2.8-win32-ansi-2.8.12.0-py26.exe?r=http%3A%2F%2Fwww.wxpython.org%2Fdownload.php&ts=1307976086&use_mirror=surfnet "$TEMP\wxPython.msi"
                ${EndIf}
                Pop $R0
                ${If} $R0 == "success"
                    Exec '"$TEMP\wxPython.msi" /VERYSILENT'
                ${Else}
                    MessageBox MB_OK "wxPython download failed, please try running installer again or manually downloading."
                ${EndIf}
            ${EndIf}
            ${If} $CheckState_Comtypes == ${BST_CHECKED}
                NSISdl::download http://downloads.sourceforge.net/project/comtypes/comtypes/0.6.2/comtypes-0.6.2.win32.exe?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fcomtypes%2F&ts=1291561083&use_mirror=softlayer "$TEMP\comtypes-0.6.2.win32.exe"
                Pop $R0
                ${If} $R0 == "success"
                    Exec '"$TEMP\comtypes-0.6.2.win32.exe"'
                ${Else}
                    MessageBox MB_OK "Comtypes download failed, please try running installer again or manually downloading."
                ${EndIf}
            ${EndIf}
            ${If} $CheckState_pywin32 == ${BST_CHECKED}
                ${If} $Python_Ver == "27"
                    NSISdl::download http://downloads.sourceforge.net/project/pywin32/pywin32/Build216/pywin32-216.win32-py2.7.exe?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fpywin32%2Ffiles%2Fpywin32%2FBuild216%2F&ts=1307976171&use_mirror=cdnetworks-us-1 "$TEMP\pywin32-216.win32.exe"
                ${ElseIf} $Python_Ver == "25"
                    NSISdl::download http://downloads.sourceforge.net/project/pywin32/pywin32/Build216/pywin32-216.win32-py2.5.exe?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fpywin32%2Ffiles%2Fpywin32%2FBuild216%2F&ts=1307976176&use_mirror=voxel "$TEMP\pywin32-216.win32.exe"
                ${Else}
                    NSISdl::download http://downloads.sourceforge.net/project/pywin32/pywin32/Build216/pywin32-216.win32-py2.6.exe?r=http%3A%2F%2Fsourceforge.net%2Fprojects%2Fpywin32%2Ffiles%2Fpywin32%2FBuild216%2F&ts=1307976169&use_mirror=cdnetworks-us-2 "$TEMP\pywin32-216.win32.exe"
                ${EndIf}
                Pop $R0
                ${If} $R0 == "success"
                    Exec '"$TEMP\pywin32-216.win32.exe"'
                ${Else}
                    MessageBox MB_OK "PyWin32 download failed, please try running installer again or manually downloading."
                ${EndIf}
            ${EndIf}
        ${EndIf}
        FunctionEnd
    Function PAGE_INSTALLLOCATIONS
        !insertmacro MUI_HEADER_TEXT $(PAGE_INSTALLLOCATIONS_TITLE) $(PAGE_INSTALLLOCATIONS_SUBTITLE)
        GetFunctionAddress $Function_Browse OnClick_Browse
        GetFunctionAddress $Function_Extra OnClick_Extra
        nsDialogs::Create 1018
            Pop $Dialog

        ${If} $Dialog == error
            Abort
            ${EndIf}

        ${NSD_CreateLabel} 0 0 100% 16u "Please select game(s)/extra location(s) to install Wrye Bash to.$\nAlso select which version(s) to install (python (default) and/or standalone executable version$\n(beta but doesn't require Python and Python addons))."
            Pop $Label
            IntOp $0 0 + 17
        ${If} $Path_OB != $Empty
            ${NSD_CreateCheckBox} 0 $0u 33% 13u "Oblivion"
                Pop $Check_OB
                ${NSD_SetState} $Check_OB $CheckState_OB
            ${NSD_CreateCheckBox} 33% $0u 33% 13u "Python"
                Pop $Check_OB_Py
                ${If} $Reg_Value_OB_Py == $True
                    ${NSD_SetState} $Check_OB_Py  ${BST_CHECKED}
                ${EndIf}
            ${NSD_CreateCheckBox} 66% $0u 34% 13u "Standalone Executable"
                Pop $Check_OB_Exe
                ${If} $Reg_Value_OB_Exe == $True
                    ${NSD_SetState} $Check_OB_Exe  ${BST_CHECKED}
                ${EndIf}
                IntOp $0 $0 + 13
            ${NSD_CreateDirRequest} 0 $0u 90% 13u "$Path_OB"
                Pop $PathDialogue_OB
            ${NSD_CreateBrowseButton} -10% $0u 5% 13u "..."
                Pop $Browse_OB
                nsDialogs::OnClick $Browse_OB $Function_Browse
            IntOp $0 $0 + 13
        ${EndIf}
        ${If} $Path_Nehrim != $Empty
            ${NSD_CreateCheckBox} 0 $0u 33% 13u "Nehrim"
                Pop $Check_Nehrim
                ${NSD_SetState} $Check_Nehrim $CheckState_Nehrim
            ${NSD_CreateCheckBox} 33% $0u 33% 13u "Python"
                Pop $Check_Nehrim_Py
                ${If} $Reg_Value_Nehrim_Py == $True
                    ${NSD_SetState} $Check_Nehrim_Py  ${BST_CHECKED}
                ${EndIf}
            ${NSD_CreateCheckBox} 66% $0u 34% 13u "Standalone Executable"
                Pop $Check_Nehrim_Exe
                ${If} $Reg_Value_Nehrim_Exe == $True
                    ${NSD_SetState} $Check_Nehrim_Exe  ${BST_CHECKED}
                ${EndIf}
                IntOp $0 $0 + 13
            ${NSD_CreateDirRequest} 0 $0u 90% 13u "$Path_Nehrim"
                Pop $PathDialogue_Nehrim
            ${NSD_CreateBrowseButton} -10% $0u 5% 13u "..."
                Pop $Browse_Nehrim
                nsDialogs::OnClick $Browse_Nehrim $Function_Browse
            IntOp $0 $0 + 13
        ${EndIf}
        ${NSD_CreateCheckBox} 0 $0u 100% 13u "Install to extra locations"
            Pop $Check_Extra
            ${NSD_SetState} $Check_Extra $CheckState_Extra
            nsDialogs::OnClick $Check_Extra $Function_Extra
            IntOp $0 $0 + 13
            ${NSD_CreateCheckBox} 0 $0u 33% 13u "Extra Location #1:"
                Pop $Check_Ex1
                ${NSD_SetState} $Check_Ex1 $CheckState_Ex1
                ${NSD_CreateCheckBox} 33% $0u 33% 13u "Python"
                    Pop $Check_Ex1_Py
                    ${If} $Reg_Value_Ex1_Py == $True
                        ${NSD_SetState} $Check_Ex1_Py  ${BST_CHECKED}
                    ${EndIf}
                ${NSD_CreateCheckBox} 66% $0u 34% 13u "Standalone Executable"
                    Pop $Check_Ex1_Exe
                    ${If} $Reg_Value_Ex1_Exe == $True
                        ${NSD_SetState} $Check_Ex1_Exe  ${BST_CHECKED}
                    ${EndIf}
                IntOp $0 $0 + 13
                ${NSD_CreateDirRequest} 0 $0u 90% 13u "$Path_Ex1"
                    Pop $PathDialogue_Ex1
                ${NSD_CreateBrowseButton} -10% $0u 5% 13u "..."
                    Pop $Browse_Ex1
                    nsDialogs::OnClick $Browse_Ex1 $Function_Browse
                IntOp $0 $0 + 13
            ${NSD_CreateCheckBox} 0 $0u 33% 13u "Extra Location #2:"
                Pop $Check_Ex2
                ${NSD_SetState} $Check_Ex2 $CheckState_Ex2
                ${NSD_CreateCheckBox} 33% $0u 33% 13u "Python"
                    Pop $Check_Ex2_Py
                    ${If} $Reg_Value_Ex2_Py == $True
                        ${NSD_SetState} $Check_Ex2_Py  ${BST_CHECKED}
                    ${EndIf}
                ${NSD_CreateCheckBox} 66% $0u 34% 13u "Standalone Executable"
                    Pop $Check_Ex2_Exe
                    ${If} $Reg_Value_Ex2_Exe == $True
                        ${NSD_SetState} $Check_Ex2_Exe  ${BST_CHECKED}
                    ${EndIf}
                IntOp $0 $0 + 13
                ${NSD_CreateDirRequest} 0 $0u 90% 13u "$Path_Ex2"
                    Pop $PathDialogue_Ex2
                ${NSD_CreateBrowseButton} -10% $0u 5% 13u "..."
                    Pop $Browse_Ex2
                    nsDialogs::OnClick $Browse_Ex2 $Function_Browse
        ${If} $CheckState_Extra != ${BST_CHECKED}
            ShowWindow $Check_Ex1 ${SW_HIDE}
            ShowWindow $Check_Ex1_Py ${SW_HIDE}
            ShowWindow $Check_Ex1_Exe ${SW_HIDE}
            ShowWindow $PathDialogue_Ex1 ${SW_HIDE}
            ShowWindow $Browse_Ex1 ${SW_HIDE}
            ShowWindow $Check_Ex2 ${SW_HIDE}
            ShowWindow $Check_Ex2_Py ${SW_HIDE}
            ShowWindow $Check_Ex2_Exe ${SW_HIDE}
            ShowWindow $PathDialogue_Ex2 ${SW_HIDE}
            ShowWindow $Browse_Ex2 ${SW_HIDE}
        ${EndIf}
        nsDialogs::Show
        FunctionEnd
    Function PAGE_INSTALLLOCATIONS_Leave
        ; Game paths
        ${NSD_GetText} $PathDialogue_OB $Path_OB
        ${NSD_GetText} $PathDialogue_Nehrim $Path_Nehrim
        ${NSD_GetText} $PathDialogue_Ex1 $Path_Ex1
        ${NSD_GetText} $PathDialogue_Ex2 $Path_Ex2
        ; Game states
        ${NSD_GetState} $Check_OB $CheckState_OB
        ${NSD_GetState} $Check_Nehrim $CheckState_Nehrim
        ${NSD_GetState} $Check_Extra $CheckState_Extra
        ${NSD_GetState} $Check_Ex1 $CheckState_Ex1
        ${NSD_GetState} $Check_Ex2 $CheckState_Ex2
        ; Python states
        ${NSD_GetState} $Check_OB_Py $CheckState_OB_Py
            ${If} $CheckState_OB_Py == ${BST_CHECKED}
                StrCpy $PythonVersionInstall "True"
            ${Endif}
        ${NSD_GetState} $Check_Nehrim_Py $CheckState_Nehrim_Py
            ${If} $CheckState_Nehrim_Py == ${BST_CHECKED}
                StrCpy $PythonVersionInstall "True"
            ${Endif}
        ${NSD_GetState} $Check_Ex1_Py $CheckState_Ex1_Py
            ${If} $CheckState_Ex1_Py == ${BST_CHECKED}
                StrCpy $PythonVersionInstall "True"
            ${Endif}
        ${NSD_GetState} $Check_Ex2_Py $CheckState_Ex2_Py
            ${If} $CheckState_Ex2_Py == ${BST_CHECKED}
                StrCpy $PythonVersionInstall "True"
            ${Endif}
        ; Standalone states
        ${NSD_GetState} $Check_OB_Exe $CheckState_OB_Exe
            ${If} $CheckState_OB_Exe == ${BST_CHECKED}
                StrCpy $ExeVersionInstall "True"
            ${Endif}
        ${NSD_GetState} $Check_Nehrim_Exe $CheckState_Nehrim_Exe
            ${If} $CheckState_Nehrim_Exe == ${BST_CHECKED}
                StrCpy $ExeVersionInstall "True"
            ${Endif}
        ${NSD_GetState} $Check_Ex1_Exe $CheckState_Ex1_Exe
            ${If} $CheckState_Ex1_Exe == ${BST_CHECKED}
                StrCpy $ExeVersionInstall "True"
            ${Endif}
        ${NSD_GetState} $Check_Ex2_Exe $CheckState_Ex2_Exe
            ${If} $CheckState_Ex2_Exe == ${BST_CHECKED}
                StrCpy $ExeVersionInstall "True"
            ${Endif}
        FunctionEnd
    Function PAGE_FINISH
        !insertmacro MUI_HEADER_TEXT $(PAGE_FINISH_TITLE) $(PAGE_FINISH_SUBTITLE)

        ReadRegStr $Path_OB HKLM "Software\Wrye Bash" "Oblivion Path"
        ReadRegStr $Path_Nehrim HKLM "Software\Wrye Bash" "Nehrim Path"
        ReadRegStr $Path_Ex1 HKLM "Software\Wrye Bash" "Extra Path 1"
        ReadRegStr $Path_Ex2 HKLM "Software\Wrye Bash" "Extra Path 2"

        nsDialogs::Create 1018
            Pop $Dialog
        ${If} $Dialog == error
            Abort
        ${EndIf}

        IntOp $0 0 + 0
        ${NSD_CreateLabel} 0 0 100% 16u "Please select which Wrye Bash Version(s) and game(s)/extra location(s) that Wrye Bash$\nis installed to that you want to run Wrye Bash from right now:"
            Pop $Label
        IntOp $0 0 + 17
        ${If} $Path_OB != $Empty
            ${NSD_CreateCheckBox} 0 $0u 100% 8u "Oblivion"
                Pop $Check_OB
            IntOp $0 $0 + 9
        ${EndIf}
        ${If} $Path_Nehrim != $Empty
            ${NSD_CreateCheckBox} 0 $0u 100% 8u "Nehrim:"
                Pop $Check_Nehrim
            IntOp $0 $0 + 9
        ${EndIf}
        ${If} $Path_Ex1 != $Empty
            ${NSD_CreateCheckBox} 0 $0u 100% 8u "Extra 1:"
                Pop $Check_Ex1
            IntOp $0 $0 + 9
        ${EndIf}
        ${If} $Path_Ex2 != $Empty
            ${NSD_CreateCheckBox} 0 $0u 100% 8u "Extra 2:"
                Pop $Check_Ex2
            IntOp $0 $0 + 9
        ${EndIf}
		IntOp $0 $0 + 9
        IntOp $1 0 + 0
        ${NSD_CreateCheckBox} $1% $0u 25% 8u "View Readme"
            Pop $Check_Readme
            ${NSD_SetState} $Check_Readme ${BST_CHECKED}
            IntOp $1 $1 + 25
        ${NSD_CreateCheckBox} $1% $0u 75% 8u "Delete files from old Bash versions"
            Pop $Check_DeleteOldFiles
            ${NSD_SetState} $Check_DeleteOldFiles ${BST_CHECKED}
        nsDialogs::Show
        FunctionEnd
    Function PAGE_FINISH_Leave
        ${NSD_GetState} $Check_OB $CheckState_OB
        ${NSD_GetState} $Check_Nehrim $CheckState_Nehrim
        ${NSD_GetState} $Check_Ex1 $CheckState_Ex1
        ${NSD_GetState} $Check_Ex2 $CheckState_Ex2

        ${If} $CheckState_OB == ${BST_CHECKED}
            SetOutPath "$Path_OB\Mopy"
            ExecShell "open" '"$Path_OB\Mopy\Wrye Bash Launcher.pyw"'
        ${EndIf}
        ${If} $CheckState_Nehrim == ${BST_CHECKED}
            SetOutPath "$Path_Nehrim\Mopy"
            ExecShell "open" '"$Path_Nehrim\Mopy\Wrye Bash Launcher.pyw"'
        ${EndIf}
        ${If} $CheckState_Ex1 == ${BST_CHECKED}
            SetOutPath "$Path_Ex1\Mopy"
            ExecShell "open" '"$Path_Ex1\Mopy\Wrye Bash Launcher.pyw"'
        ${EndIf}
        ${If} $CheckState_Ex2 == ${BST_CHECKED}
            SetOutPath "$Path_Ex2\Mopy"
            ExecShell "open" '"$Path_Ex2\Mopy\Wrye Bash Launcher.pyw"'
        ${EndIf}
        ${NSD_GetState} $Check_Readme $0
        ${If} $0 == ${BST_CHECKED}
            ${If} $Path_OB != $Empty
                ExecShell "open" "$Path_OB\Mopy\Wrye Bash.html"
            ${ElseIf} $Path_Nehrim != $Empty
                ExecShell "open" "$Path_Nehrim\Mopy\Wrye Bash.html"
            ${ElseIf} $Path_Ex1 != $Empty
                ExecShell "open" "$Path_Ex1\Mopy\Wrye Bash.html"
            ${ElseIf} $Path_Ex2 != $Empty
                ExecShell "open" "$Path_Ex2\Mopy\Wrye Bash.html"
            ${EndIf}
        ${EndIf}
        ${NSD_GetState} $Check_DeleteOldFiles $0
        ${If} $0 == ${BST_CHECKED}
            ${If} $Path_OB != $Empty
                Delete "$Path_OB\Data\Docs\Bashed Lists.txt"
                Delete "$Path_OB\Data\Docs\Bashed Lists.html"
                Delete "$Path_OB\Mopy\Data\Actor Levels\*"
                Delete "$Path_OB\Data\Ini Tweaks\Autosave, Never.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Autosave, ~Always.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Border Regions, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Border Regions, ~Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Fonts 1, ~Default.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Fonts, ~Default.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Grass, Fade 4k-5k.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Intro Movies, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Intro Movies, Normal.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Joystick, ~Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Joystick, ~Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Local Map Shader, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Local Map Shader, ~Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Music, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Music, Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Refraction Shader, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Refraction Shader, ~Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 1.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 2.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 3.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 5.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Screenshot, ~Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Screenshot, ~ENabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\ShadowMapResolution, 1024.ini"
                Delete "$Path_OB\Data\Ini Tweaks\ShadowMapResolution, 256 [default].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 8.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 16.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 24.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 48.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 64.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 96.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 128.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 192.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound, Enabled.ini"
                RMDir "$Path_OB\Mopy\Data\Actor Levels"
                Delete "$Path_OB\Data\Bash Patches\Crowded Cities 15_Alternate_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Cities 15_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Cities 30_Alternate_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Cities 30_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Roads Revamped_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Roads Revisited_Alternate_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Roads Revisited_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\PTRoamingNPCs_Names.csv"
            ${EndIf}
            ${If} $Path_Nehrim != $Empty
                Delete "$Path_Nehrim\Data\Docs\Bashed Lists.txt"
                Delete "$Path_Nehrim\Data\Docs\Bashed Lists.html"
                Delete "$Path_Nehrim\Mopy\Data\Actor Levels\*"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Autosave, Never.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Autosave, ~Always.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Border Regions, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Border Regions, ~Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Fonts 1, ~Default.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Fonts, ~Default.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Grass, Fade 4k-5k.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Intro Movies, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Intro Movies, Normal.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Joystick, ~Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Joystick, ~Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Local Map Shader, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Local Map Shader, ~Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Music, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Music, Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Refraction Shader, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Refraction Shader, ~Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 1.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 2.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 3.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 5.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Screenshot, ~Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Screenshot, ~ENabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\ShadowMapResolution, 1024.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\ShadowMapResolution, 256 [default].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 8.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 16.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 24.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 48.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 64.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 96.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 128.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 192.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound, Enabled.ini"
                RMDir "$Path_Nehrim\Mopy\Data\Actor Levels"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Cities 15_Alternate_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Cities 15_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Cities 30_Alternate_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Cities 30_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Roads Revamped_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Roads Revisited_Alternate_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Roads Revisited_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\PTRoamingNPCs_Names.csv"
            ${EndIf}
            ${If} $Path_Ex1 != $Empty
                Delete "$Path_Ex1\Data\Docs\Bashed Lists.txt"
                Delete "$Path_Ex1\Data\Docs\Bashed Lists.html"
                Delete "$Path_Ex1\Mopy\Data\Actor Levels\*"
                Delete "$Path_Ex1\Data\Ini Tweaks\Autosave, Never.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Autosave, ~Always.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Border Regions, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Border Regions, ~Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Fonts 1, ~Default.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Fonts, ~Default.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Grass, Fade 4k-5k.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Intro Movies, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Intro Movies, Normal.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Joystick, ~Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Joystick, ~Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Local Map Shader, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Local Map Shader, ~Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Music, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Music, Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Refraction Shader, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Refraction Shader, ~Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 1.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 2.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 3.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 5.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Screenshot, ~Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Screenshot, ~ENabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\ShadowMapResolution, 1024.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\ShadowMapResolution, 256 [default].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 8.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 16.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 24.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 48.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 64.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 96.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 128.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 192.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound, Enabled.ini"
                RMDir "$Path_Ex1\Mopy\Data\Actor Levels"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Cities 15_Alternate_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Cities 15_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Cities 30_Alternate_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Cities 30_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Roads Revamped_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Roads Revisited_Alternate_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Roads Revisited_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\PTRoamingNPCs_Names.csv"
            ${EndIf}
            ${If} $Path_Ex2 != $Empty
                Delete "$Path_Ex2\Data\Docs\Bashed Lists.txt"
                Delete "$Path_Ex2\Data\Docs\Bashed Lists.html"
                Delete "$Path_Ex2\Mopy\Data\Actor Levels\*"
                Delete "$Path_Ex2\Data\Ini Tweaks\Autosave, Never.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Autosave, ~Always.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Border Regions, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Border Regions, ~Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Fonts 1, ~Default.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Fonts, ~Default.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Grass, Fade 4k-5k.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Intro Movies, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Intro Movies, Normal.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Joystick, ~Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Joystick, ~Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Local Map Shader, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Local Map Shader, ~Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Music, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Music, Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Refraction Shader, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Refraction Shader, ~Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 1.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 2.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 3.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 5.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Screenshot, ~Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Screenshot, ~ENabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\ShadowMapResolution, 1024.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\ShadowMapResolution, 256 [default].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 8.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 16.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 24.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 48.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 64.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 96.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 128.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 192.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, ~ [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels,  [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound, Enabled.ini"
                RMDir "$Path_Ex2\Mopy\Data\Actor Levels"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Cities 15_Alternate_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Cities 15_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Cities 30_Alternate_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Cities 30_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Roads Revamped_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Roads Revisited_Alternate_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Roads Revisited_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\PTRoamingNPCs_Names.csv"
            ${EndIf}
        ${EndIf}
        FunctionEnd
    Function OnClick_Browse
        Pop $0
        ${If} $0 == $Browse_OB
            StrCpy $1 $PathDialogue_OB
        ${ElseIf} $0 == $Browse_Nehrim
            StrCpy $1 $PathDialogue_Nehrim
        ${ElseIf} $0 == $Browse_Ex1
            StrCpy $1 $PathDialogue_Ex1
        ${ElseIf} $0 == $Browse_Ex2
            StrCpy $1 $PathDialogue_Ex2
        ${EndIf}
        ${NSD_GetText} $1 $Function_DirPrompt
        nsDialogs::SelectFolderDialog /NOUNLOAD "Please select a target directory" $Function_DirPrompt
        Pop $0

        ${If} $0 == error
            Abort
        ${EndIf}

        ${NSD_SetText} $1 $0
        FunctionEnd
    Function OnClick_Extra
        Pop $0
        ${NSD_GetState} $0 $CheckState_Extra
        ${If} $CheckState_Extra == ${BST_UNCHECKED}
            ShowWindow $Check_Ex1 ${SW_HIDE}
            ShowWindow $Check_Ex1_Py ${SW_HIDE}
            ShowWindow $Check_Ex1_Exe ${SW_HIDE}
            ShowWindow $PathDialogue_Ex1 ${SW_HIDE}
            ShowWindow $Browse_Ex1 ${SW_HIDE}
            ShowWindow $Check_Ex2 ${SW_HIDE}
            ShowWindow $Check_Ex2_Py ${SW_HIDE}
            ShowWindow $Check_Ex2_Exe ${SW_HIDE}
            ShowWindow $PathDialogue_Ex2 ${SW_HIDE}
            ShowWindow $Browse_Ex2 ${SW_HIDE}
        ${Else}
            ShowWindow $Check_Ex1 ${SW_SHOW}
            ShowWindow $Check_Ex1_Py ${SW_SHOW}
            ShowWindow $Check_Ex1_Exe ${SW_SHOW}
            ShowWindow $PathDialogue_Ex1 ${SW_SHOW}
            ShowWindow $Browse_Ex1 ${SW_SHOW}
            ShowWindow $Check_Ex2 ${SW_SHOW}
            ShowWindow $Check_Ex2_Py ${SW_SHOW}
            ShowWindow $Check_Ex2_Exe ${SW_SHOW}
            ShowWindow $PathDialogue_Ex2 ${SW_SHOW}
            ShowWindow $Browse_Ex2 ${SW_SHOW}
        ${EndIf}
        FunctionEnd
    Function OnClick_Link
        Pop $0
        ${If} $0 == $Link_Comtypes
            ExecShell "open" "http://sourceforge.net/projects/comtypes/files/comtypes/0.6.2/"
        ${ElseIf} $0 == $Link_wx
            ExecShell "open" "http://www.wxpython.org/download.php#stable"
        ${ElseIf} $0 == $Link_Python
            ExecShell "open" "http://www.python.org/download/releases/2.6.6/"
        ${ElseIf} $0 == $Link_pywin32
            ExecShell "open" "http://sourceforge.net/projects/pywin32/files/pywin32/Build216/"
        ${ElseIf} $0 == $Link_vcredist
            ExecShell "open" "http://www.microsoft.com/downloads/details.aspx?familyid=a5c84275-3b97-4ab7-a40d-3802b2af5fc2"
        ${EndIf}

        ${If} $0 == error
            Abort
        ${EndIf}
        FunctionEnd
;-------------------------------- The Installation Sections:
    Section "Wrye Bash (required)" Main
        SectionIn RO

        ${If} $CheckState_OB == ${BST_CHECKED}
            ; Install resources:
            ${If} Path_OB != $Empty
                SetOutPath $Path_OB
                File /r /x "*svn*" /x "*.tmp" /x "*.nsi" /x "*.bat" /x "*.py*" /x "mopy\w9xpopen.exe" /x "Mopy\Wrye Bash.exe" "Data" "Mopy"
                ; Write the installation path into the registry
                WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Oblivion Path" "$Path_OB"
                ${If} $CheckState_OB_Py == ${BST_CHECKED}
                    SetOutPath "$Path_OB\Mopy"
                    File /r "Mopy\*.py" "Mopy\*.pyw"
                    ; Write the installation path into the registry
                    WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Oblivion Python Version" "True"
                ${Else}
                    ${If} $Reg_Value_OB_Py == $Empty ; ie don't overwrite it if it is installed but just not being installed that way this time.
                        WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Oblivion Python Version" ""
                    ${EndIf}
                ${EndIf}
                ${If} $CheckState_OB_Exe == ${BST_CHECKED}
                    SetOutPath "$Path_OB\Mopy"
                    File /r "mopy\w9xpopen.exe" "Mopy\Wrye Bash.exe"
                    ; Write the installation path into the registry
                    WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Oblivion Standalone Version" "True"
                ${Else}
                    ${If} $Reg_Value_OB_Exe == $Empty ; ie don't overwrite it if it is installed but just not being installed that way this time.
                        WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Oblivion Standalone Version" ""
                    ${EndIf}
                ${EndIf}
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Nehrim == ${BST_CHECKED}
            ; Install resources:
            ${If} Path_Nehrim != $Empty
                SetOutPath $Path_Nehrim
                File /r /x "*svn*" /x "*.tmp" /x "*.nsi" /x "*.bat" /x "*.py*" /x "mopy\w9xpopen.exe" /x "Mopy\Wrye Bash.exe" "Data" "Mopy"
                ; Write the installation path into the registry
                WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Nehrim Path" "$Path_Nehrim"
                ${If} $CheckState_Nehrim_Py == ${BST_CHECKED}
                    SetOutPath "$Path_Nehrim\Mopy"
                    File /r "Mopy\*.py" "Mopy\*.pyw"
                    ; Write the installation path into the registry
                    WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Nehrim Python Version" "True"
                ${Else}
                    ${If} $Reg_Value_Nehrim_Py == $Empty ; ie don't overwrite it if it is installed but just not being installed that way this time.
                        WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Nehrim Python Version" ""
                    ${EndIf}
                ${EndIf}
                ${If} $CheckState_Nehrim_Exe == ${BST_CHECKED}
                    SetOutPath "$Path_Nehrim\Mopy"
                    File /r "mopy\w9xpopen.exe" "Mopy\Wrye Bash.exe"
                    ; Write the installation path into the registry
                    WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Nehrim Standalone Version" "True"
                ${Else}
                    ${If} $Reg_Value_Nehrim_Exe == $Empty ; ie don't overwrite it if it is installed but just not being installed that way this time.
                        WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Nehrim Standalone Version" ""
                    ${EndIf}
                ${EndIf}
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Ex1 == ${BST_CHECKED}
            ; Install resources:
            ${If} Path_Ex1 != $Empty
                SetOutPath $Path_Ex1
                File /r /x "*svn*" /x "*.tmp" /x "*.nsi" /x "*.bat" /x "*.py*" /x "mopy\w9xpopen.exe" /x "Mopy\Wrye Bash.exe" "Data" "Mopy"
                ; Write the installation path into the registry
                WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 1" "$Path_Ex1"
                ${If} $CheckState_Ex1_Py == ${BST_CHECKED}
                    SetOutPath "$Path_Ex1\Mopy"
                    File /r "Mopy\*.py" "Mopy\*.pyw"
                    ; Write the installation path into the registry
                    WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 1 Python Version" "True"
                ${Else}
                    ${If} $Reg_Value_Ex1_Py == $Empty ; ie don't overwrite it if it is installed but just not being installed that way this time.
                        WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 1 Python Version" ""
                    ${EndIf}
                ${EndIf}
                ${If} $CheckState_Ex1_Exe == ${BST_CHECKED}
                    SetOutPath "$Path_Ex1\Mopy"
                    File /r "mopy\w9xpopen.exe" "Mopy\Wrye Bash.exe"
                    ; Write the installation path into the registry
                    WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 1 Standalone Version" "True"
                ${Else}
                    ${If} $Reg_Value_Ex1_Exe == $Empty ; ie don't overwrite it if it is installed but just not being installed that way this time.
                        WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 1 Standalone Version" ""
                    ${EndIf}
                ${EndIf}
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Ex2 == ${BST_CHECKED}
            ; Install resources:
            ${If} Path_Ex2 != $Empty
                SetOutPath $Path_Ex2
                File /r /x "*svn*" /x "*.tmp" /x "*.nsi" /x "*.bat" /x "*.py*" /x "mopy\w9xpopen.exe" /x "Mopy\Wrye Bash.exe" "Data" "Mopy"
                ; Write the installation path into the registry
                WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 2" "$Path_Ex2"
                ${If} $CheckState_Ex2_Py == ${BST_CHECKED}
                    SetOutPath "$Path_Ex2\Mopy"
                    File /r "Mopy\*.py" "Mopy\*.pyw"
                    ; Write the installation path into the registry
                    WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 2 Python Version" "True"
                ${Else}
                    ${If} $Reg_Value_Ex2_Py == $Empty ; ie don't overwrite it if it is installed but just not being installed that way this time.
                        WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 2 Python Version" ""
                    ${EndIf}
                ${EndIf}
                ${If} $CheckState_Ex2_Exe == ${BST_CHECKED}
                    SetOutPath "$Path_Ex2\Mopy"
                    File /r "mopy\w9xpopen.exe" "Mopy\Wrye Bash.exe"
                    ; Write the installation path into the registry
                    WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 2 Standalone Version" "True"
                ${Else}
                    ${If} $Reg_Value_Ex2_Exe == $Empty ; ie don't overwrite it if it is installed but just not being installed that way this time.
                        WriteRegStr HKLM "SOFTWARE\Wrye Bash" "Extra Path 2 Standalone Version" ""
                    ${EndIf}
                ${EndIf}
            ${EndIf}
        ${EndIf}
        ; Write the uninstall keys for Windows
        SetOutPath "$COMMONFILES\Wrye Bash"
        WriteRegStr HKLM "Software\Wrye Bash" "Installer Path" "$EXEPATH"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash" "DisplayName" "Wrye Bash"
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash" "UninstallString" '"$COMMONFILES\Wrye Bash\uninstall.exe"'
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash" "URLInfoAbout" 'http://www.tesnexus.com/downloads/file.php?id=22368'
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash" "HelpLink" 'http://forums.bethsoft.com/index.php?/topic/1145445-relz-wrye-bash-thead-55/'
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash" "Publisher" 'Wrye & Wrye Bash Development Team'
        WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash" "DisplayVersion" '${WB_FILEVERSION}'
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash" "NoModify" 1
        WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash" "NoRepair" 1
        CreateDirectory "$COMMONFILES\Wrye Bash"
        WriteUninstaller "$COMMONFILES\Wrye Bash\uninstall.exe"
        SectionEnd
    Section "Start Menu Shortcuts" Shortcuts_SM

        CreateDirectory "$SMPROGRAMS\Wrye Bash"
        CreateShortCut "$SMPROGRAMS\Wrye Bash\Uninstall.lnk" "$COMMONFILES\Wrye Bash\uninstall.exe" "" "$COMMONFILES\Wrye Bash\uninstall.exe" 0

        ${If} $CheckState_OB == ${BST_CHECKED}
            ${If} Path_OB != $Empty
                SetOutPath $Path_OB\Mopy
                CreateShortCut "$SMPROGRAMS\Wrye Bash\Wrye Bash - Oblivion.lnk" "$Path_OB\mopy\Wrye Bash Launcher.pyw" "" "$PATH_OB\mopy\Wrye Bash Launcher.pyw" 0
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Nehrim == ${BST_CHECKED}
            ${If} Path_Nehrim != $Empty
                SetOutPath $Path_Nehrim\Mopy
                CreateShortCut "$SMPROGRAMS\Wrye Bash\Wrye Bash - Nehrim.lnk" "$Path_Nehrim\mopy\Wrye Bash Launcher.pyw" "" "$Path_Nehrim\mopy\Wrye Bash Launcher.pyw" 0
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Ex1 == ${BST_CHECKED}
            ${If} Path_Ex1 != $Empty
                SetOutPath $Path_Ex1\Mopy
                CreateShortCut "$SMPROGRAMS\Wrye Bash\Wrye Bash - Extra 1.lnk" "$Path_Ex1\mopy\Wrye Bash Launcher.pyw" "" "$Path_Ex1\mopy\Wrye Bash Launcher.pyw" 0
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Ex2 == ${BST_CHECKED}
            ${If} Path_Ex2 != $Empty
                SetOutPath $Path_Ex2\Mopy
                CreateShortCut "$SMPROGRAMS\Wrye Bash\Wrye Bash - Extra 2.lnk" "$Path_Ex2\mopy\Wrye Bash Launcher.pyw" "" "$Path_Ex2\mopy\Wrye Bash Launcher.pyw" 0
            ${EndIf}
        ${EndIf}
        SectionEnd
    Section "Batch Files" Batch_Files

        ${If} $CheckState_OB == ${BST_CHECKED}
            ${If} Path_OB != $Empty
                SetOutPath $Path_OB
                File /r "*.bat"
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Nehrim == ${BST_CHECKED}
            ${If} Path_Nehrim != $Empty
                SetOutPath $Path_Nehrim
                File /r "*.bat"
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Ex1 == ${BST_CHECKED}
            ${If} Path_Ex1 != $Empty
                SetOutPath $Path_Ex1
                File /r "*.bat"
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Ex2 == ${BST_CHECKED}
            ${If} Path_Ex2 != $Empty
                SetOutPath $Path_Ex2
                File /r "*.bat"
            ${EndIf}
        ${EndIf}
    SectionEnd
;-------------------------------- Custom Uninstallation Pages and their Functions:
    Function un.PAGE_SELECT_GAMES
        !insertmacro MUI_HEADER_TEXT $(PAGE_INSTALLLOCATIONS_TITLE) $(unPAGE_SELECT_GAMES_SUBTITLE)
        GetFunctionAddress $unFunction_Browse un.OnClick_Browse

        nsDialogs::Create 1018
            Pop $Dialog
        ${If} $Dialog == error
            Abort
            ${EndIf}

        ${NSD_CreateLabel} 0 0 100% 8u "Please select which game(s)/extra location(s) and version(s) to uninstall Wrye Bash from:"
        Pop $Label

        IntOp $0 0 + 9
        ${If} $Path_OB != $Empty
            ${NSD_CreateCheckBox} 0 $0u 100% 13u "&Oblivion"
                Pop $Check_OB
                ${NSD_SetState} $Check_OB $CheckState_OB
            IntOp $0 $0 + 13
            ${NSD_CreateDirRequest} 0 $0u 90% 13u "$Path_OB"
                Pop $PathDialogue_OB
            ${NSD_CreateBrowseButton} -10% $0u 5% 13u "..."
                Pop $Browse_OB
                nsDialogs::OnClick $Browse_OB $Function_Browse
            IntOp $0 $0 + 13
        ${EndIf}
        ${If} $Path_Nehrim != $Empty
            ${NSD_CreateCheckBox} 0 $0u 100% 13u "Nehrim"
                Pop $Check_Nehrim
                ${NSD_SetState} $Check_Nehrim $CheckState_Nehrim
            IntOp $0 $0 + 13
            ${NSD_CreateDirRequest} 0 $0u 90% 13u "$Path_Nehrim"
                Pop $PathDialogue_Nehrim
            ${NSD_CreateBrowseButton} -10% $0u 5% 13u "..."
                Pop $Browse_Nehrim
                nsDialogs::OnClick $Browse_Nehrim $Function_Browse
            IntOp $0 $0 + 13
        ${EndIf}
        ${If} $Path_Ex1 != $Empty
            ${NSD_CreateCheckBox} 0 $0u 100% 13u "Extra Location 1"
                Pop $Check_Ex1
                ${NSD_SetState} $Check_Ex1 $CheckState_Ex1
            IntOp $0 $0 + 13
            ${NSD_CreateDirRequest} 0 $0u 90% 13u "$Path_Ex1"
                Pop $PathDialogue_Ex1
            ${NSD_CreateBrowseButton} -10% $0u 5% 13u "..."
                Pop $Browse_Ex1
                nsDialogs::OnClick $Browse_Ex1 $Function_Browse
            IntOp $0 $0 + 13
        ${EndIf}
        ${If} $Path_Ex2 != $Empty
            ${NSD_CreateCheckBox} 0 $0u 100% 13u "Extra Location 2"
                Pop $Check_Ex2
                ${NSD_SetState} $Check_Ex2 $CheckState_Ex2
            IntOp $0 $0 + 13
            ${NSD_CreateDirRequest} 0 $0u 90% 13u "$Path_Ex2"
                Pop $PathDialogue_Ex2
            ${NSD_CreateBrowseButton} -10% $0u 5% 13u "..."
                Pop $Browse_Ex2
                nsDialogs::OnClick $Browse_Ex2 $Function_Browse
            IntOp $0 $0 + 13
        ${EndIf}
        ;${NSD_CreateCheckBox} 0 $0u 100% 13u "Uninstall userfiles/Bash data."
        ;    Pop $Check_RemoveUserFiles
        ;    ${NSD_SetState} $Check_RemoveUserFiles ${BST_CHECKED}
        nsDialogs::Show
        FunctionEnd
    Function un.PAGE_SELECT_GAMES_Leave
        ${NSD_GetText} $PathDialogue_OB $Path_OB
        ${NSD_GetText} $PathDialogue_Nehrim $Path_Nehrim
        ${NSD_GetText} $PathDialogue_Ex1 $Path_Ex1
        ${NSD_GetText} $PathDialogue_Ex2 $Path_Ex2
        ${NSD_GetState} $Check_OB $CheckState_OB
        ${NSD_GetState} $Check_Nehrim $CheckState_Nehrim
        ${NSD_GetState} $Check_Extra $CheckState_Extra
        ${NSD_GetState} $Check_Ex1 $CheckState_Ex1
        ${NSD_GetState} $Check_Ex2 $CheckState_Ex2
        FunctionEnd
    Function un.OnClick_Browse
        Pop $0
        ${If} $0 == $Browse_OB
            StrCpy $1 $PathDialogue_OB
        ${ElseIf} $0 == $Browse_Nehrim
            StrCpy $1 $PathDialogue_Nehrim
        ${ElseIf} $0 == $Browse_Ex1
            StrCpy $1 $PathDialogue_Ex1
        ${ElseIf} $0 == $Browse_Ex2
            StrCpy $1 $PathDialogue_Ex2
        ${EndIf}
        ${NSD_GetText} $1 $Function_DirPrompt
        nsDialogs::SelectFolderDialog /NOUNLOAD "Please select a target directory" $Function_DirPrompt
        Pop $0

        ${If} $0 == error
            Abort
        ${EndIf}

        ${NSD_SetText} $1 $0
        FunctionEnd
;-------------------------------- The Uninstallation Code:
    Section "Uninstall"

        ; Remove registry keys
        ;DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash"
        ;

        ; Remove files and Directories - Directories are only deleted if empty.
        ${If} $CheckState_OB == ${BST_CHECKED}
            ${If} $Path_OB != $Empty
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Oblivion Path"
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Oblivion Python Version"
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Oblivion Standalone Version"
                ;First delete OLD version files:
                Delete "$Path_OB\Data\Docs\Bashed Lists.txt"
                Delete "$Path_OB\Data\Docs\Bashed Lists.html"
                Delete "$Path_OB\Mopy\uninstall.exe"
                Delete "$Path_OB\Data\Ini Tweaks\Autosave, Never.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Autosave, ~Always.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Border Regions, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Border Regions, ~Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Fonts 1, ~Default.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Fonts, ~Default.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Grass, Fade 4k-5k.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Intro Movies, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Intro Movies, Normal.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Joystick, ~Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Joystick, ~Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Local Map Shader, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Local Map Shader, ~Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Music, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Music, Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Refraction Shader, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Refraction Shader, ~Enabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 1.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 2.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 3.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 5.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Screenshot, ~Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Screenshot, ~ENabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\ShadowMapResolution, 1024.ini"
                Delete "$Path_OB\Data\Ini Tweaks\ShadowMapResolution, 256 [default].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 8.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 16.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 24.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 48.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 64.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 96.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 128.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 192.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, ~ [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels,  [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound, Disabled.ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound, Enabled.ini"
                Delete "$Path_OB\Data\Bash Patches\Crowded Cities 15_Alternate_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Cities 15_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Cities 30_Alternate_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Cities 30_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Roads Revamped_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Roads Revisited_Alternate_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Crowded Roads Revisited_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\PTRoamingNPCs_Names.csv"
                ;then delete the rest of it:
                Delete "$Path_OB\Data\ArchiveInvalidationInvalidated!.bsa"
                Delete "$Path_OB\Data\Bash Patches\Assorted to Cobl.csv"
                Delete "$Path_OB\Data\Bash Patches\Assorted_Exhaust.csv"
                Delete "$Path_OB\Data\Bash Patches\Bash_Groups.csv"
                Delete "$Path_OB\Data\Bash Patches\Bash_MFact.csv"
                Delete "$Path_OB\Data\Bash Patches\ShiveringIsleTravellers_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\TamrielTravellers_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Guard_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Kmacg94_Exhaust.csv"
                Delete "$Path_OB\Data\Bash Patches\P1DCandles_Formids.csv"
                Delete "$Path_OB\Data\Bash Patches\OOO_Potion_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Random_NPC_Alternate_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Random_NPC_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\Rational_Names.csv"
                Delete "$Path_OB\Data\Bash Patches\TI to Cobl_Formids.csv"
                Delete "$Path_OB\Data\Bash Patches\taglist.txt"
                Delete "$Path_OB\Data\Bash Patches\OOO, 1.23 Mincapped_NPC_Levels.csv"
                Delete "$Path_OB\Data\Bash Patches\OOO, 1.23 Uncapped_NPC_Levels.csv"
                Delete "$Path_OB\Data\Docs\Bash Readme Template.html"
                Delete "$Path_OB\Data\Docs\Bash Readme Template.txt"
                Delete "$Path_OB\Data\Docs\wtxt_sand_small.css"
                Delete "$Path_OB\Data\Docs\wtxt_teal.css"
                Delete "$Path_OB\Data\Ini Tweaks\Autosave, Never [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Autosave, ~Always [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Border Regions, Disabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Border Regions, ~Enabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Fonts 1, ~Default [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Fonts, ~Default [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Grass, Fade 4k-5k [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Grass, ~Fade 2k-3k [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Intro Movies, Disabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Intro Movies, ~Normal [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Joystick, Disabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Joystick, ~Enabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Local Map Shader, Disabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Local Map Shader, ~Enabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Music, Disabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Music, ~Enabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Refraction Shader, Disabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Refraction Shader, ~Enabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 1 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 2 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 3 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Save Backups, 5 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Screenshot, Enabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Screenshot, ~Disabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\ShadowMapResolution, 10 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\ShadowMapResolution, ~256 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\ShadowMapResolution, 1024 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 24 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, ~32 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 128 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 16 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 192 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 48 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 64 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 8 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound Card Channels, 96 [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound, Disabled [Oblivion].ini"
                Delete "$Path_OB\Data\Ini Tweaks\Sound, ~Enabled [Oblivion].ini"
                Delete "$Path_OB\Mopy\*.bat"
                Delete "$Path_OB\Mopy\7z.dll"
                Delete "$Path_OB\Mopy\7z*exe"
                Delete "$Path_OB\Mopy\Rename_CBash.dll"
                Delete "$Path_OB\Mopy\CBash.dll"
                Delete "$Path_OB\Mopy\ScriptParser.p*"
                Delete "$Path_OB\Mopy\Wrye Bash Debug.p*"
                Delete "$Path_OB\Mopy\Wrye Bash Launcher.p*"
                Delete "$Path_OB\Mopy\Wrye Bash.html"
                Delete "$Path_OB\Mopy\Wrye Bash.txt"
                Delete "$Path_OB\Mopy\balt.p*"
                Delete "$Path_OB\Mopy\barb.p*"
                Delete "$Path_OB\Mopy\bash.p*"
                Delete "$Path_OB\Mopy\bash_default.ini"
                Delete "$Path_OB\Mopy\bash_default_Russian.ini"
                Delete "$Path_OB\Mopy\bash.ini"
                Delete "$Path_OB\Mopy\basher.p*"
                Delete "$Path_OB\Mopy\bashmon.p*"
                Delete "$Path_OB\Mopy\belt.p*"
                Delete "$Path_OB\Mopy\bish.p*"
                Delete "$Path_OB\Mopy\bolt.p*"
                Delete "$Path_OB\Mopy\bosh.p*"
                Delete "$Path_OB\Mopy\bush.p*"
                Delete "$Path_OB\Mopy\cint.p*"
                Delete "$Path_OB\Mopy\gpl.txt"
                Delete "$Path_OB\Mopy\w9xpopen.exe"
                Delete "$Path_OB\Mopy\Wrye Bash.exe"
                Delete "$Path_OB\Mopy\wizards.html"
                Delete "$Path_OB\Mopy\wizards.txt"
                Delete "$Path_OB\Mopy\Data\Italian.txt"
                Delete "$Path_OB\Mopy\Data\Oblivion_ids.pkl"
                Delete "$Path_OB\Mopy\Data\Russian.txt"
                Delete "$Path_OB\Mopy\Data\de.txt"
                Delete "$Path_OB\Mopy\Data\pt_opt.txt"
                Delete "$Path_OB\Mopy\Data\Actor Levels\OOO, 1.23 Mincapped.csv"
                Delete "$Path_OB\Mopy\Data\Actor Levels\OOO, 1.23 Uncapped.csv"
                Delete "$Path_OB\Mopy\Extras\*.*"
                Delete "$Path_OB\Mopy\Images\*.*"
                Delete "$Path_OB\Mopy\Wizard Images\*.*"
                RMDir "$Path_OB\Mopy\Wizard Images"
                RMDir "$Path_OB\Mopy\Images"
                RMDir "$Path_OB\Mopy\Data\Actor Levels"
                RMDir "$Path_OB\Mopy\Data"
                RMDir "$Path_OB\Mopy\Extras"
                RMDir "$Path_OB\Mopy"
                RMDir "$Path_OB\Data\Docs"
                RMDir "$Path_OB\Data\Bash Patches"
                Delete "$SMPROGRAMS\Wrye Bash\*oblivion*"
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Nehrim == ${BST_CHECKED}
            ${If} $Path_Nehrim != $Empty
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Nehrim Path"
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Nehrim Python Version"
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Nehrim Standalone Version"
                ;First delete OLD version files:
                Delete "$Path_Nehrim\Data\Docs\Bashed Lists.txt"
                Delete "$Path_Nehrim\Data\Docs\Bashed Lists.html"
                Delete "$Path_Nehrim\Mopy\uninstall.exe"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Autosave, Never.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Autosave, ~Always.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Border Regions, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Border Regions, ~Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Fonts 1, ~Default.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Fonts, ~Default.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Grass, Fade 4k-5k.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Intro Movies, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Intro Movies, Normal.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Joystick, ~Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Joystick, ~Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Local Map Shader, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Local Map Shader, ~Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Music, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Music, Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Refraction Shader, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Refraction Shader, ~Enabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 1.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 2.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 3.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 5.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Screenshot, ~Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Screenshot, ~ENabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\ShadowMapResolution, 1024.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\ShadowMapResolution, 256 [default].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 8.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 16.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 24.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 48.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 64.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 96.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 128.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 192.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, ~ [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels,  [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound, Disabled.ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound, Enabled.ini"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Cities 15_Alternate_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Cities 15_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Cities 30_Alternate_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Cities 30_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Roads Revamped_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Roads Revisited_Alternate_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Crowded Roads Revisited_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\PTRoamingNPCs_Names.csv"
                ;then delete the rest of it:
                Delete "$Path_Nehrim\Data\ArchiveInvalidationInvalidated!.bsa"
                Delete "$Path_Nehrim\Data\Bash Patches\Assorted to Cobl.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Assorted_Exhaust.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Bash_Groups.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Bash_MFact.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\ShiveringIsleTravellers_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\TamrielTravellers_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Guard_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Kmacg94_Exhaust.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\P1DCandles_Formids.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\OOO_Potion_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Random_NPC_Alternate_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Random_NPC_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\Rational_Names.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\TI to Cobl_Formids.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\taglist.txt"
                Delete "$Path_Nehrim\Data\Bash Patches\OOO, 1.23 Mincapped_NPC_Levels.csv"
                Delete "$Path_Nehrim\Data\Bash Patches\OOO, 1.23 Uncapped_NPC_Levels.csv"
                Delete "$Path_Nehrim\Data\Docs\Bash Readme Template.html"
                Delete "$Path_Nehrim\Data\Docs\Bash Readme Template.txt"
                Delete "$Path_Nehrim\Data\Docs\wtxt_sand_small.css"
                Delete "$Path_Nehrim\Data\Docs\wtxt_teal.css"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Autosave, Never [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Autosave, ~Always [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Border Regions, Disabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Border Regions, ~Enabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Fonts 1, ~Default [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Fonts, ~Default [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Grass, Fade 4k-5k [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Grass, ~Fade 2k-3k [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Intro Movies, Disabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Intro Movies, ~Normal [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Joystick, Disabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Joystick, ~Enabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Local Map Shader, Disabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Local Map Shader, ~Enabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Music, Disabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Music, ~Enabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Refraction Shader, Disabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Refraction Shader, ~Enabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 1 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 2 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 3 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Save Backups, 5 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Screenshot, Enabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Screenshot, ~Disabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\ShadowMapResolution, 10 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\ShadowMapResolution, ~256 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\ShadowMapResolution, 1024 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 24 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, ~32 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 128 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 16 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 192 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 48 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 64 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 8 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound Card Channels, 96 [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound, Disabled [Oblivion].ini"
                Delete "$Path_Nehrim\Data\Ini Tweaks\Sound, ~Enabled [Oblivion].ini"
                Delete "$Path_Nehrim\Mopy\*.bat"
                Delete "$Path_Nehrim\Mopy\7z.dll"
                Delete "$Path_Nehrim\Mopy\7z*exe"
                Delete "$Path_Nehrim\Mopy\Rename_CBash.dll"
                Delete "$Path_Nehrim\Mopy\CBash.dll"
                Delete "$Path_Nehrim\Mopy\ScriptParser.p*"
                Delete "$Path_Nehrim\Mopy\Wrye Bash Debug.p*"
                Delete "$Path_Nehrim\Mopy\Wrye Bash Launcher.p*"
                Delete "$Path_Nehrim\Mopy\Wrye Bash.html"
                Delete "$Path_Nehrim\Mopy\Wrye Bash.txt"
                Delete "$Path_Nehrim\Mopy\balt.p*"
                Delete "$Path_Nehrim\Mopy\barb.p*"
                Delete "$Path_Nehrim\Mopy\bash.p*"
                Delete "$Path_Nehrim\Mopy\bash_default.ini"
                Delete "$Path_Nehrim\Mopy\bash_default_Russian.ini"
                Delete "$Path_Nehrim\Mopy\bash.ini"
                Delete "$Path_Nehrim\Mopy\basher.p*"
                Delete "$Path_Nehrim\Mopy\bashmon.p*"
                Delete "$Path_Nehrim\Mopy\belt.p*"
                Delete "$Path_Nehrim\Mopy\bish.p*"
                Delete "$Path_Nehrim\Mopy\bolt.p*"
                Delete "$Path_Nehrim\Mopy\bosh.p*"
                Delete "$Path_Nehrim\Mopy\bush.p*"
                Delete "$Path_Nehrim\Mopy\cint.p*"
                Delete "$Path_Nehrim\Mopy\gpl.txt"
                Delete "$Path_Nehrim\Mopy\w9xpopen.exe"
                Delete "$Path_Nehrim\Mopy\Wrye Bash.exe"
                Delete "$Path_Nehrim\Mopy\wizards.html"
                Delete "$Path_Nehrim\Mopy\wizards.txt"
                Delete "$Path_Nehrim\Mopy\Data\Italian.txt"
                Delete "$Path_Nehrim\Mopy\Data\Oblivion_ids.pkl"
                Delete "$Path_Nehrim\Mopy\Data\Russian.txt"
                Delete "$Path_Nehrim\Mopy\Data\de.txt"
                Delete "$Path_Nehrim\Mopy\Data\pt_opt.txt"
                Delete "$Path_Nehrim\Mopy\Data\Actor Levels\OOO, 1.23 Mincapped.csv"
                Delete "$Path_Nehrim\Mopy\Data\Actor Levels\OOO, 1.23 Uncapped.csv"
                Delete "$Path_Nehrim\Mopy\Extras\*.*"
                Delete "$Path_Nehrim\Mopy\Images\*.*"
                Delete "$Path_Nehrim\Mopy\Wizard Images\*.*"
                RMDir "$Path_Nehrim\Mopy\Wizard Images"
                RMDir "$Path_Nehrim\Mopy\Images"
                RMDir "$Path_Nehrim\Mopy\Data\Actor Levels"
                RMDir "$Path_Nehrim\Mopy\Data"
                RMDir "$Path_Nehrim\Mopy\Extras"
                RMDir "$Path_Nehrim\Mopy"
                RMDir "$Path_Nehrim\Data\Docs"
                RMDir "$Path_Nehrim\Data\Bash Patches"
                Delete "$SMPROGRAMS\Wrye Bash\*Nehrim*"
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Ex1 == ${BST_CHECKED}
            ${If} $Path_Ex1 != $Empty
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Extra Path 1"
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Extra Path 1 Python Version"
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Extra Path 1 Standalone Version"
                ;First delete OLD version files:
                Delete "$Path_Ex1\Data\Docs\Bashed Lists.txt"
                Delete "$Path_Ex1\Data\Docs\Bashed Lists.html"
                Delete "$Path_Ex1\Mopy\uninstall.exe"
                Delete "$Path_Ex1\Data\Ini Tweaks\Autosave, Never.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Autosave, ~Always.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Border Regions, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Border Regions, ~Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Fonts 1, ~Default.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Fonts, ~Default.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Grass, Fade 4k-5k.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Intro Movies, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Intro Movies, Normal.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Joystick, ~Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Joystick, ~Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Local Map Shader, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Local Map Shader, ~Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Music, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Music, Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Refraction Shader, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Refraction Shader, ~Enabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 1.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 2.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 3.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 5.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Screenshot, ~Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Screenshot, ~ENabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\ShadowMapResolution, 1024.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\ShadowMapResolution, 256 [default].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 8.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 16.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 24.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 48.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 64.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 96.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 128.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 192.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, ~ [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels,  [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound, Disabled.ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound, Enabled.ini"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Cities 15_Alternate_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Cities 15_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Cities 30_Alternate_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Cities 30_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Roads Revamped_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Roads Revisited_Alternate_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Crowded Roads Revisited_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\PTRoamingNPCs_Names.csv"
                ;then delete the rest of it:
                Delete "$Path_Ex1\Data\ArchiveInvalidationInvalidated!.bsa"
                Delete "$Path_Ex1\Data\Bash Patches\Assorted to Cobl.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Assorted_Exhaust.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Bash_Groups.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Bash_MFact.csv"
                Delete "$Path_Ex1\Data\Bash Patches\ShiveringIsleTravellers_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\TamrielTravellers_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Guard_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Kmacg94_Exhaust.csv"
                Delete "$Path_Ex1\Data\Bash Patches\P1DCandles_Formids.csv"
                Delete "$Path_Ex1\Data\Bash Patches\OOO_Potion_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Random_NPC_Alternate_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Random_NPC_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\Rational_Names.csv"
                Delete "$Path_Ex1\Data\Bash Patches\TI to Cobl_Formids.csv"
                Delete "$Path_Ex1\Data\Bash Patches\taglist.txt"
                Delete "$Path_Ex1\Data\Bash Patches\OOO, 1.23 Mincapped_NPC_Levels.csv"
                Delete "$Path_Ex1\Data\Bash Patches\OOO, 1.23 Uncapped_NPC_Levels.csv"
                Delete "$Path_Ex1\Data\Docs\Bash Readme Template.html"
                Delete "$Path_Ex1\Data\Docs\Bash Readme Template.txt"
                Delete "$Path_Ex1\Data\Docs\wtxt_sand_small.css"
                Delete "$Path_Ex1\Data\Docs\wtxt_teal.css"
                Delete "$Path_Ex1\Data\Ini Tweaks\Autosave, Never [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Autosave, ~Always [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Border Regions, Disabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Border Regions, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Fonts 1, ~Default [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Fonts, ~Default [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Grass, Fade 4k-5k [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Grass, ~Fade 2k-3k [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Intro Movies, Disabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Intro Movies, ~Normal [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Joystick, Disabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Joystick, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Local Map Shader, Disabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Local Map Shader, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Music, Disabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Music, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Refraction Shader, Disabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Refraction Shader, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 1 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 2 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 3 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Save Backups, 5 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Screenshot, Enabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Screenshot, ~Disabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\ShadowMapResolution, 10 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\ShadowMapResolution, ~256 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\ShadowMapResolution, 1024 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 24 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, ~32 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 128 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 16 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 192 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 48 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 64 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 8 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound Card Channels, 96 [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound, Disabled [Oblivion].ini"
                Delete "$Path_Ex1\Data\Ini Tweaks\Sound, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex1\Mopy\*.bat"
                Delete "$Path_Ex1\Mopy\7z.dll"
                Delete "$Path_Ex1\Mopy\7z*exe"
                Delete "$Path_Ex1\Mopy\Rename_CBash.dll"
                Delete "$Path_Ex1\Mopy\CBash.dll"
                Delete "$Path_Ex1\Mopy\ScriptParser.p*"
                Delete "$Path_Ex1\Mopy\Wrye Bash Debug.p*"
                Delete "$Path_Ex1\Mopy\Wrye Bash Launcher.p*"
                Delete "$Path_Ex1\Mopy\Wrye Bash.html"
                Delete "$Path_Ex1\Mopy\Wrye Bash.txt"
                Delete "$Path_Ex1\Mopy\balt.p*"
                Delete "$Path_Ex1\Mopy\barb.p*"
                Delete "$Path_Ex1\Mopy\bash.p*"
                Delete "$Path_Ex1\Mopy\bash_default.ini"
                Delete "$Path_Ex1\Mopy\bash_default_Russian.ini"
                Delete "$Path_Ex1\Mopy\bash.ini"
                Delete "$Path_Ex1\Mopy\basher.p*"
                Delete "$Path_Ex1\Mopy\bashmon.p*"
                Delete "$Path_Ex1\Mopy\belt.p*"
                Delete "$Path_Ex1\Mopy\bish.p*"
                Delete "$Path_Ex1\Mopy\bolt.p*"
                Delete "$Path_Ex1\Mopy\bosh.p*"
                Delete "$Path_Ex1\Mopy\bush.p*"
                Delete "$Path_Ex1\Mopy\cint.p*"
                Delete "$Path_Ex1\Mopy\gpl.txt"
                Delete "$Path_Ex1\Mopy\w9xpopen.exe"
                Delete "$Path_Ex1\Mopy\Wrye Bash.exe"
                Delete "$Path_Ex1\Mopy\wizards.html"
                Delete "$Path_Ex1\Mopy\wizards.txt"
                Delete "$Path_Ex1\Mopy\Data\Italian.txt"
                Delete "$Path_Ex1\Mopy\Data\Oblivion_ids.pkl"
                Delete "$Path_Ex1\Mopy\Data\Russian.txt"
                Delete "$Path_Ex1\Mopy\Data\de.txt"
                Delete "$Path_Ex1\Mopy\Data\pt_opt.txt"
                Delete "$Path_Ex1\Mopy\Data\Actor Levels\OOO, 1.23 Mincapped.csv"
                Delete "$Path_Ex1\Mopy\Data\Actor Levels\OOO, 1.23 Uncapped.csv"
                Delete "$Path_Ex1\Mopy\Extras\*.*"
                Delete "$Path_Ex1\Mopy\Images\*.*"
                Delete "$Path_Ex1\Mopy\Wizard Images\*.*"
                RMDir "$Path_Ex1\Mopy\Wizard Images"
                RMDir "$Path_Ex1\Mopy\Images"
                RMDir "$Path_Ex1\Mopy\Data\Actor Levels"
                RMDir "$Path_Ex1\Mopy\Data"
                RMDir "$Path_Ex1\Mopy\Extras"
                RMDir "$Path_Ex1\Mopy"
                RMDir "$Path_Ex1\Data\Docs"
                RMDir "$Path_Ex1\Data\Bash Patches"
                Delete "$SMPROGRAMS\Wrye Bash\*Extra 1*"
            ${EndIf}
        ${EndIf}
        ${If} $CheckState_Ex2 == ${BST_CHECKED}
            ${If} $Path_Ex2 != $Empty
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Extra Path 2"
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Extra Path 2 Python Version"
                DeleteRegValue HKLM "SOFTWARE\Wrye Bash" "Extra Path 2 Standalone Version"
                ;First delete OLD version files:
                Delete "$Path_Ex2\Data\Docs\Bashed Lists.txt"
                Delete "$Path_Ex2\Data\Docs\Bashed Lists.html"
                Delete "$Path_Ex2\Mopy\uninstall.exe"
                Delete "$Path_Ex2\Data\Ini Tweaks\Autosave, Never.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Autosave, ~Always.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Border Regions, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Border Regions, ~Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Fonts 1, ~Default.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Fonts, ~Default.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Grass, Fade 4k-5k.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Intro Movies, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Intro Movies, Normal.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Joystick, ~Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Joystick, ~Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Local Map Shader, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Local Map Shader, ~Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Music, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Music, Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Refraction Shader, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Refraction Shader, ~Enabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 1.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 2.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 3.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 5.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Screenshot, ~Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Screenshot, ~ENabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\ShadowMapResolution, 1024.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\ShadowMapResolution, 256 [default].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 8.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 16.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 24.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 48.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 64.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 96.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 128.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 192.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, ~ [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels,  [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound, Disabled.ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound, Enabled.ini"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Cities 15_Alternate_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Cities 15_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Cities 30_Alternate_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Cities 30_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Roads Revamped_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Roads Revisited_Alternate_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Crowded Roads Revisited_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\PTRoamingNPCs_Names.csv"
                ;then delete the rest of it:
                Delete "$Path_Ex2\Data\ArchiveInvalidationInvalidated!.bsa"
                Delete "$Path_Ex2\Data\Bash Patches\Assorted to Cobl.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Assorted_Exhaust.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Bash_Groups.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Bash_MFact.csv"
                Delete "$Path_Ex2\Data\Bash Patches\ShiveringIsleTravellers_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\TamrielTravellers_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Guard_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Kmacg94_Exhaust.csv"
                Delete "$Path_Ex2\Data\Bash Patches\P1DCandles_Formids.csv"
                Delete "$Path_Ex2\Data\Bash Patches\OOO_Potion_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Random_NPC_Alternate_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Random_NPC_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\Rational_Names.csv"
                Delete "$Path_Ex2\Data\Bash Patches\TI to Cobl_Formids.csv"
                Delete "$Path_Ex2\Data\Bash Patches\taglist.txt"
                Delete "$Path_Ex2\Data\Bash Patches\OOO, 1.23 Mincapped_NPC_Levels.csv"
                Delete "$Path_Ex2\Data\Bash Patches\OOO, 1.23 Uncapped_NPC_Levels.csv"
                Delete "$Path_Ex2\Data\Docs\Bash Readme Template.html"
                Delete "$Path_Ex2\Data\Docs\Bash Readme Template.txt"
                Delete "$Path_Ex2\Data\Docs\wtxt_sand_small.css"
                Delete "$Path_Ex2\Data\Docs\wtxt_teal.css"
                Delete "$Path_Ex2\Data\Ini Tweaks\Autosave, Never [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Autosave, ~Always [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Border Regions, Disabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Border Regions, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Fonts 1, ~Default [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Fonts, ~Default [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Grass, Fade 4k-5k [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Grass, ~Fade 2k-3k [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Intro Movies, Disabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Intro Movies, ~Normal [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Joystick, Disabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Joystick, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Local Map Shader, Disabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Local Map Shader, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Music, Disabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Music, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Refraction Shader, Disabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Refraction Shader, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 1 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 2 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 3 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Save Backups, 5 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Screenshot, Enabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Screenshot, ~Disabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\ShadowMapResolution, 10 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\ShadowMapResolution, ~256 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\ShadowMapResolution, 1024 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 24 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, ~32 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 128 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 16 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 192 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 48 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 64 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 8 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound Card Channels, 96 [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound, Disabled [Oblivion].ini"
                Delete "$Path_Ex2\Data\Ini Tweaks\Sound, ~Enabled [Oblivion].ini"
                Delete "$Path_Ex2\Mopy\*.bat"
                Delete "$Path_Ex2\Mopy\7z.dll"
                Delete "$Path_Ex2\Mopy\7z*exe"
                Delete "$Path_Ex2\Mopy\Rename_CBash.dll"
                Delete "$Path_Ex2\Mopy\CBash.dll"
                Delete "$Path_Ex2\Mopy\ScriptParser.p*"
                Delete "$Path_Ex2\Mopy\Wrye Bash Debug.p*"
                Delete "$Path_Ex2\Mopy\Wrye Bash Launcher.p*"
                Delete "$Path_Ex2\Mopy\Wrye Bash.html"
                Delete "$Path_Ex2\Mopy\Wrye Bash.txt"
                Delete "$Path_Ex2\Mopy\balt.p*"
                Delete "$Path_Ex2\Mopy\barb.p*"
                Delete "$Path_Ex2\Mopy\bash.p*"
                Delete "$Path_Ex2\Mopy\bash_default.ini"
                Delete "$Path_Ex2\Mopy\bash_default_Russian.ini"
                Delete "$Path_Ex2\Mopy\bash.ini"
                Delete "$Path_Ex2\Mopy\basher.p*"
                Delete "$Path_Ex2\Mopy\bashmon.p*"
                Delete "$Path_Ex2\Mopy\belt.p*"
                Delete "$Path_Ex2\Mopy\bish.p*"
                Delete "$Path_Ex2\Mopy\bolt.p*"
                Delete "$Path_Ex2\Mopy\bosh.p*"
                Delete "$Path_Ex2\Mopy\bush.p*"
                Delete "$Path_Ex2\Mopy\cint.p*"
                Delete "$Path_Ex2\Mopy\gpl.txt"
                Delete "$Path_Ex2\Mopy\w9xpopen.exe"
                Delete "$Path_Ex2\Mopy\Wrye Bash.exe"
                Delete "$Path_Ex2\Mopy\wizards.html"
                Delete "$Path_Ex2\Mopy\wizards.txt"
                Delete "$Path_Ex2\Mopy\Data\Italian.txt"
                Delete "$Path_Ex2\Mopy\Data\Oblivion_ids.pkl"
                Delete "$Path_Ex2\Mopy\Data\Russian.txt"
                Delete "$Path_Ex2\Mopy\Data\de.txt"
                Delete "$Path_Ex2\Mopy\Data\pt_opt.txt"
                Delete "$Path_Ex2\Mopy\Data\Actor Levels\OOO, 1.23 Mincapped.csv"
                Delete "$Path_Ex2\Mopy\Data\Actor Levels\OOO, 1.23 Uncapped.csv"
                Delete "$Path_Ex2\Mopy\Extras\*.*"
                Delete "$Path_Ex2\Mopy\Images\*.*"
                Delete "$Path_Ex2\Mopy\Wizard Images\*.*"
                RMDir "$Path_Ex2\Mopy\Wizard Images"
                RMDir "$Path_Ex2\Mopy\Images"
                RMDir "$Path_Ex2\Mopy\Data\Actor Levels"
                RMDir "$Path_Ex2\Mopy\Data"
                RMDir "$Path_Ex2\Mopy\Extras"
                RMDir "$Path_Ex2\Mopy"
                RMDir "$Path_Ex2\Data\Docs"
                RMDir "$Path_Ex2\Data\Bash Patches"
                Delete "$SMPROGRAMS\Wrye Bash\*Extra 2*"
            ${EndIf}
        ${EndIf}


        ;If it is a complete uninstall remove the shared data:
        ReadRegStr $Path_OB HKLM "Software\Wrye Bash" "Oblivion Path"
        ReadRegStr $Path_Nehrim HKLM "Software\Wrye Bash" "Nehrim Path"
        ReadRegStr $Path_Ex1 HKLM "Software\Wrye Bash" "Extra Path 1"
        ReadRegStr $Path_Ex2 HKLM "Software\Wrye Bash" "Extra Path 2"
        ${If} $Path_OB == $Empty
            ${If} $Path_Nehrim == $Empty
                ${If} $Path_Ex1 == $Empty
                    ${If} $Path_Ex2 == $Empty
                        DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\Wrye Bash"
                        ReadRegStr $0 HKLM "Software\Wrye Bash" "Installer Path"
                        DeleteRegKey HKLM "SOFTWARE\Wrye Bash"
                        ;Delete stupid Windows created registry keys:
                            DeleteRegKey HKCU "Software\Microsoft\Windows\CurrentVersion\App Management\ARPCache\Wrye Bash"
                            DeleteRegValue HKCR "Local Settings\Software\Microsoft\Windows\Shell\MuiCache" "$COMMONFILES\Wrye Bash\Uninstall.exe"
                            DeleteRegValue HKCU "Software\Classes\Local Settings\Software\Microsoft\Windows\Shell\MuiCache" "$COMMONFILES\Wrye Bash\Uninstall.exe"
                            DeleteRegValue HKCU "Software\Microsoft\Windows\ShellNoRoam\MuiCache" "$COMMONFILES\Wrye Bash\Uninstall.exe"
                            DeleteRegValue HKCR "Local Settings\Software\Microsoft\Windows\Shell\MuiCache" "$0"
                            DeleteRegValue HKCU "Software\Classes\Local Settings\Software\Microsoft\Windows\Shell\MuiCache" "$0"
                            DeleteRegValue HKCU "Software\Microsoft\Windows\ShellNoRoam\MuiCache" "$0"
                        Delete "$SMPROGRAMS\Wrye Bash\*.*"
                        RMDir "$SMPROGRAMS\Wrye Bash"
                        Delete "$COMMONFILES\Wrye Bash\*.*"
                        RMDir "$COMMONFILES\Wrye Bash"
                    ${EndIf}
                ${EndIf}
            ${EndIf}
        ${EndIf}
        SectionEnd
;-------------------------------- Descriptions/Subtitles/Language Strins:

  ;Language strings
  !insertmacro MUI_LANGUAGE "English"
  LangString DESC_Main ${LANG_ENGLISH} "The main Wrye Bash files."
  LangString DESC_Shortcuts_SM ${LANG_ENGLISH} "Start Menu shortcuts for the uninstaller and each launcher."
  LangString DESC_Batch_Files ${LANG_ENGLISH} "Batch files to print debug output to a text file."
  LangString PAGE_INSTALLLOCATIONS_TITLE ${LANG_ENGLISH} "Installation Location(s)"
  LangString PAGE_INSTALLLOCATIONS_SUBTITLE ${LANG_ENGLISH} "Please select main installation path for Wrye Bash and if desired extra locations to install Wrye Bash to."
  LangString PAGE_REQUIREMENTS_TITLE ${LANG_ENGLISH} "Installation Prerequisites"
  LangString PAGE_REQUIREMENTS_SUBTITLE ${LANG_ENGLISH} "Please rectify the following missing requirements"
  LangString unPAGE_SELECT_GAMES_SUBTITLE ${LANG_ENGLISH} "Please select which locations you want to uninstall Wrye Bash from."
  LangString PAGE_FINISH_TITLE ${LANG_ENGLISH} "Finished installing ${WB_NAME}"
  LangString PAGE_FINISH_SUBTITLE ${LANG_ENGLISH} "Please select post-install tasks."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${Main} $(DESC_Main)
    !insertmacro MUI_DESCRIPTION_TEXT ${Shortcuts_SM} $(DESC_Shortcuts_SM)
    !insertmacro MUI_DESCRIPTION_TEXT ${Batch_Files} $(DESC_Batch_Files)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END