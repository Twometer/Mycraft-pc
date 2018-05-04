@echo off
xcopy /y /d "libs\*.dll" "Debug"
xcopy /y /d /s res Debug