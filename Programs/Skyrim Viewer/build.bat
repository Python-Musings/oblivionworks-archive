copy "package\setup.py" setup.py

"C:\Program Files (x86)\Python\2.7\python.exe" setup.py py2exe

move /y "dist\Skyrim Viewer.exe" "Skyrim Viewer.exe"
move /y "dist\python27.dll" "python27.dll"

rmdir /s /q dist
rmdir /s /q build
del setup.py

package\reshacker.exe -addoverwrite "Skyrim Viewer.exe", "Skyrim Viewer.exe", "images/viewer.ico", icon, 101, 0
package\reshacker.exe -addoverwrite "SKyrim Viewer.exe", "Skyrim Viewer.exe", "images/stop.ico", icon, 102, 1

package\upx.exe -9 "Skyrim Viewer.exe"
package\upx.exe -9 "python27.dll"

del "Skyrim Viewer.7z"
del "Skyrim Viewer - Program.7z"
del "Skyrim Viewer - Record Definitions.7z"
package\7z.exe a -mx9 "Skyrim Viewer.7z" "records\*.*" @package\files.txt
package\7z.exe a -mx9 "Skyrim Viewer - Record Definitions.7z" "records\*.*"

::del python27.dll
::del "Skyrim Viewer.exe"

pause