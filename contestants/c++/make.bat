@echo off
pushd %~dp0
setlocal EnableExtensions EnableDelayedExpansion

call %~dp0\premake5.exe --file=%~dp0\premake5.lua vs2017

if defined VisualStudioVersion (
    rem vcvarsall has been called already, don't need to do anything ourselves
) else (
    set vcvarsall_location_1="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
    set vcvarsall_location_2="%ProgramFiles%\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
	set vcvarsall_location_3="%ProgramFiles(x86)%\Microsoft Visual Studio\2017\WDExpress\VC\Auxiliary\Build\vcvarsall.bat"
    set vcvarsall_location_4="%ProgramFiles%\Microsoft Visual Studio\2017\WDExpress\VC\Auxiliary\Build\vcvarsall.bat"
    set vcvarsall_location_5="%ProgramFiles(x86)%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
    set vcvarsall_location_6="%ProgramFiles%\Microsoft Visual Studio 14.0\VC\vcvarsall.bat"
    set vcvarsall_location_count=6

    for /L %%i in (!vcvarsall_location_count!, -1, 1) do (
        set vcvarsall_location_candidate=!vcvarsall_location_%%i!
        if exist !vcvarsall_location_candidate! set vcvarsall_location=!vcvarsall_location_candidate!
    )

    if not defined vcvarsall_location (
        echo Failed to find vcvarsall.bat in any of the known places. You have two options:
        echo 1^) Preferred: run vcvarsall.bat yourself before running this script. Check out https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line for more information.
        echo 2^) Find where vcvarsall.bat file is on your system and add it to the list of locations in this batch file.
        pause
        exit /b 1
    )

    reg query "HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment" /v PROCESSOR_ARCHITECTURE | find /i "x86" > nul
    if !ERRORLEVEL! == 0 (
        set vcvarsall_architecture=x86
    ) else (
        set vcvarsall_architecture=amd64
    )

    set VSCMD_START_DIR=%CD%
    call !vcvarsall_location! !vcvarsall_architecture!
)

set CONFIGURATION=Debug
call msbuild.exe .vs\MyBot.sln /t:Build /p:Configuration=%CONFIGURATION%;Platform=Win32 /m:8
copy /Y .vs\MyBot.exe .

popd
echo All Done.
