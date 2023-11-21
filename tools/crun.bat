@echo off
cls
set PROJECT_DIR=%~dp0..
set BUILD_DIR=%PROJECT_DIR%\build

pushd %BUILD_DIR%
%BUILD_DIR%/syscore.exe 
popd



