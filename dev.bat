@echo off
set VIXEN_ROOT=e:\Projects\FoxBox\FENNEC64
set BUILD_DIR=%VIXEN_ROOT%\build
set WINLIBS_DIR=C:\winlibs\mingw64
set PATH=%VIXEN_ROOT%\bin;%BUILD_DIR%;%WINLIBS_DIR%\bin;%PATH%
set INCLUDE=%VIXEN_ROOT%\src\include;%INCLUDE%
@echo Vixen environment activated.
@echo on