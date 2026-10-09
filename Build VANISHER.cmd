@echo off
setlocal
title Build VANISHER - Created by ALNASRIF HALIDDIN
powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0Build-VANISHER.ps1"
set "vanisherResult=%errorlevel%"
echo.
set /p "vanisherPause=Press Enter to close this window... "
exit /b %vanisherResult%
