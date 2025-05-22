@echo off

:: PowerShellスクリプトのパスを指定
set "PS_SCRIPT=%~dp0internal\ConvertHDRtoCubemap.ps1"

:loop
if "%~1"=="" goto end

:: 1つずつ PowerShell に渡す
powershell -NoProfile -ExecutionPolicy Bypass -File "%PS_SCRIPT%" "%~1"

shift
goto loop

:end
pause