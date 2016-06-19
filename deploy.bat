@echo off

set OUTPUT_DIR=deploy
set EXE_PATH=release\kernel2_compressor.exe
set LIB_DIR=%QTDIR%\bin

rem Create target directory
if not exist %OUTPUT_DIR% mkdir %OUTPUT_DIR%

rem Deploy DLLs
%LIB_DIR%\windeployqt.exe --force --release --dir %OUTPUT_DIR% --no-quick-import --no-translations --no-system-d3d-compiler --no-opengl --no-3dcore --no-3drenderer --no-webkit2 --no-angle --no-svg --no-webkit %EXE_PATH%

rem Removing unused DLLs
del /q %OUTPUT_DIR%\opengl32sw.dll

rem Deploy Exe
xcopy /y %EXE_PATH% %OUTPUT_DIR%

rem Compress Exe and DLLs. Note: DLLs in platforms/ directory should not be compressed.
upx %OUTPUT_DIR%\*.dll %OUTPUT_DIR%\*.exe
