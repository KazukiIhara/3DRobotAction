@echo off
cd /d "%~dp0" || exit /b 1
cd /d ".\Tools\Premake\" || exit /b 1
premake5  vs2022
pause