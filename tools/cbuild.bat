@echo off
cls
set PROJECT_DIR=%~dp0..
set BUILD_DIR=%PROJECT_DIR%\build
set SRC_DIR=%PROJECT_DIR%\src

setlocal EnableDelayedExpansion
set "startTime=%time: =0%"

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

@REM LAUNCHER
g++ -g %SRC_DIR%\syscore\syscore.cpp -o %BUILD_DIR%\syscore.exe 

echo Compilation completed

@REM CALCULATE ELAPSED TIME
set "endTime=%time: =0%"
rem Get elapsed time:
set "end=!endTime:%time:~8,1%=%%100)*100+1!"  &  set "start=!startTime:%time:~8,1%=%%100)*100+1!"
set /A "elap=((((10!end:%time:~2,1%=%%100)*60+1!%%100)-((((10!start:%time:~2,1%=%%100)*60+1!%%100), elap-=(elap>>31)*24*60*60*100"

rem Convert elapsed time to HH:MM:SS:CC format:
set /A "cc=elap%%100+100,elap/=100,ss=elap%%60+100,elap/=60,mm=elap%%60+100,hh=elap/60+100"

echo Elapsed:  %ss:~1%%time:~8,1%%cc:~1%