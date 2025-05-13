#!/usr/bin/env bash

# [ msys ] distribute from MSYS for WindowsOS and WineOS .

# executable program copy
cp ../../build/app.exe .
# dependencies copy ( deps )
deplister.exe app.exe # list deps
cp /c/msys64/mingw64/bin/SDL3.dll .
cp /c/msys64/mingw64/bin/libgcc_s_seh-1.dll .
cp /c/msys64/mingw64/bin/libwinpthread-1.dll .
cp /c/msys64/mingw64/bin/libstdc++-6.dll .
