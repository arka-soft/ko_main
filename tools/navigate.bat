cd /d %~dp0

REM Check if you are in the "build" directory
if "%cd%"=="%~dp0\build" (
  cd ..
  cd src
)

REM Check if you are in the "ko_main" directory
if "%cd%"=="%~dp0\ko_main" (
  cd src
)

REM Check if you are in the "tools" directory
if "%cd%"=="%~dp0\tools" (
  cd ..
  cd src
)