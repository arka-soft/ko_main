@echo off
cls
set PROJECT_DIR=%~dp0..
set BUILD_DIR=%PROJECT_DIR%\build
set SRC_DIR=%PROJECT_DIR%\src

if not exist %BUILD_DIR% mkdir %BUILD_DIR%

@REM DYNAMIC COMPONENTS
g++ -g -shared %SRC_DIR%\config\config.cpp -o %BUILD_DIR%\config_master.dll

@REM LAUNCHER
@REM (NOTE: This will say 'cannot open file' if you compile while running the launcher, that's fine.) 
g++ -g %SRC_DIR%\syscore\syslauncher.cpp -o %BUILD_DIR%\syslauncher.exe 
