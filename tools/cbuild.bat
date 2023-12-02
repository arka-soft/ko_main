@echo off
cls
set PROJECT_DIR=%~dp0..
set BUILD_DIR=%PROJECT_DIR%\build
set SRC_DIR=%PROJECT_DIR%\src

setlocal EnableDelayedExpansion
set "startTime=%time: =0%"

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

echo ^+----------------------------------------------------------------------------------------------------------------------------+
echo ^|                                                       COMPILATION                                
echo ^+----------------------------------------------------------------------------------------------------------------------------+

@REM Define DEBUG variable
set DEBUG=0

if %DEBUG% EQU 1 (
    echo ^|  Debug:              ^|  Yes                                           
    echo ^+----------------------------------------------------------------------------------------------------------------------------+
) else ( 
    echo ^|  Debug:              ^|  No                                            
    echo ^+----------------------------------------------------------------------------------------------------------------------------+
)

@REM Compiler Flags
set COMPILER_FLAGS=-g -Wall -std=c++11 -Wno-unused-variable
if %DEBUG% EQU 1 (
    set COMPILER_FLAGS=!COMPILER_FLAGS! -DDEBUG
) else (
    set "COMPILER_FLAGS=!COMPILER_FLAGS!        "
)

echo ^|  Flags:              ^| !COMPILER_FLAGS!
echo ^+----------------------------------------------------------------------------------------------------------------------------+

@REM LAUNCHER
g++ !COMPILER_FLAGS! %SRC_DIR%\syscore\syscore.cpp -o %BUILD_DIR%\syscore.exe 2>&1

@REM Check if compilation was successful
if %errorlevel% neq 0 (
    echo ^+----------------------------------------------------------------------------------------------------------------------------+
    echo ^|  Compilation Status: ^| Failed, Check the error messages above for details
    echo ^+----------------------------------------------------------------------------------------------------------------------------+
    goto :end
) else (
    echo ^|  Compilation Status: ^| Successful
    echo ^+----------------------------------------------------------------------------------------------------------------------------+
)

@REM CALCULATE ELAPSED TIME
set "endTime=%time: =0%"
rem Get elapsed time:
set "end=!endTime:%time:~8,1%=%%100)*100+1!"  &  set "start=!startTime:%time:~8,1%=%%100)*100+1!"
set /A "elap=((((10!end:%time:~2,1%=%%100)*60+1!%%100)-((((10!start:%time:~2,1%=%%100)*60+1!%%100), elap-=(elap>>31)*24*60*60*100"

rem Convert elapsed time to HH:MM:SS:CC format:
set /A "cc=elap%%100+100,elap/=100,ss=elap%%60+100,elap/=60,mm=elap%%60+100,hh=elap/60+100"

echo ^|  Time:               ^| %mm:~1% minutes %ss:~1% seconds %cc:~1% milliseconds.
echo ^+----------------------------------------------------------------------------------------------------------------------------+

:end
