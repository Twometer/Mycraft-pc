REM This script copies all resources and libraries to the output folder
@echo off
xcopy /y /d "libs\*.dll" "Debug"
xcopy /y /d /s res Debug
xcopy /y /d "libs\*.dll" "Release"
xcopy /y /d /s res Release