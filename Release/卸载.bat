@echo off

REM  --> Check for permissions
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    if exist "%temp%\getadmin.vbs" ( del "%temp%\getadmin.vbs" )
    pushd "%CD%"
    CD /D "%~dp0"

if %PROCESSOR_ARCHITECTURE%==x86 (
	set DLL=Webp2bmp.dll
) else (
	set DLL=x64\Webp2bmp64.dll
)

del unregistered 2> NUL

regsvr32 /u /s "%~dp0%DLL%"

if exist unregistered (
	echo Uninstallation successful.
    del unregistered 2> NUL
) else (
	echo Uninstallation failed.
REM   regsvr32 /u "%~dp0%DLL%"
REM	exit /B 1
)

taskkill /f /IM explorer.exe
start explorer.exe

exit /B 1

pause