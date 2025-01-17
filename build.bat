@echo off
setlocal enabledelayedexpansion

mkdir libs 2>nul

echo Building x86 versions...
mkdir build-x86 2>nul
cd build-x86
cmake -A Win32 ..
cmake --build . --config Release
cmake --build . --config Debug
cd ..

echo Building x64 versions...
mkdir build-x64 2>nul
cd build-x64
cmake -A x64 ..
cmake --build . --config Release
cmake --build . --config Debug
cd ..

echo Copying libraries to libs directory...
copy "build-x86\Release\TrampHook.lib" "libs\TrampHook-x86-md.lib"
copy "build-x86\Debug\TrampHook.lib" "libs\TrampHook-x86-mdd.lib"
copy "build-x64\Release\TrampHook.lib" "libs\TrampHook-x64-md.lib"
copy "build-x64\Debug\TrampHook.lib" "libs\TrampHook-x64-mdd.lib"

echo Build complete! Libraries are in the libs directory:
dir /b libs
pause