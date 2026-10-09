@echo off
setlocal
title VANISHER - Created by ALNASRIF HALIDDIN
pushd "%~dp0"
if errorlevel 1 exit /b 1
set "vanisherExe=%~dp0VANISHER.exe"
if not exist "%vanisherExe%" set "vanisherExe=%~dp0Release\VANISHER.exe"
if not exist "%vanisherExe%" (
    echo VANISHER.exe was not found.
    echo Extract the complete app ZIP, or double-click Build VANISHER.cmd in the source folder.
    set "vanisherResult=1"
    goto finish
)
"%vanisherExe%"
set "vanisherResult=%errorlevel%"
:finish
echo.
set /p "vanisherPause=Press Enter to close this window... "
popd
exit /b %vanisherResult%
