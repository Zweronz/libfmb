@echo off
setlocal enabledelayedexpansion

set OUTPUT=FoursakenRepacker.exe
set SOURCES=
for %%f in (src\*.c) do set SOURCES=!SOURCES! %%f

g++ main.c %SOURCES% -Wall -g -o %OUTPUT% -I "include"

if %errorlevel% equ 0 (
    echo Build succeeded! Output: %OUTPUT%
) else (
    echo Build failed.
    exit /b %errorlevel%
)

endlocal
pause