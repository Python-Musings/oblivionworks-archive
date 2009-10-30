;Created by Raziel23x


#SingleInstance Ignore

; If %A_IsAdmin%

; {
; }
; else
; {
; Msgbox 0, Can't Run, You must be an Administrator to Run Raziel23x's Oblivion Toolset
; ExitApp
; }

; If A_OSVersion in WIN_NT4,WIN_95,WIN_98,WIN_ME,WIN_2000
; {
 ;    MsgBox 0, Can't Run, You need Windows XP or Windows Vista to Run Raziel23x's Oblivion Toolset
 ;    ExitApp
; }


SplitPath, A_ScriptFullPath ,,,,, root

IfExist %A_ScriptDir%\BOSS\BOSS.bat
{
Gui, Add, Button, x26 y40 w254 h60 , Office 2007 &Enterprise
Gui, Add, Picture, x18 y105 , %A_ScriptDir%\images\access.bmp
Gui, Add, Picture, x46 y105 , %A_ScriptDir%\images\excel.bmp
Gui, Add, Picture, x77 y105 , %A_ScriptDir%\images\groove.bmp
Gui, Add, Picture, x106 y105 , %A_ScriptDir%\images\infopath.bmp
Gui, Add, Picture, x136 y105 , %A_ScriptDir%\images\onenote.bmp
Gui, Add, Picture, x167 y105 , %A_ScriptDir%\images\outlook.bmp
Gui, Add, Picture, x198 y105 , %A_ScriptDir%\images\powerpoint.bmp
Gui, Add, Picture, x229 y105 , %A_ScriptDir%\images\publisher.bmp
Gui, Add, Picture, x259 y105 , %A_ScriptDir%\images\word.bmp
}
else
{
Gui, Add, Button, x26 y40 w254 h60 +Disabled, Office 2007 &Enterprise
Gui, Add, Text, x26 y110 w30 h20 , Error:
Gui, Add, Text, x56 y110 w200 h20 , Installation files not found
}