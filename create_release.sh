#!/bin/bash -e

#cp -f /mingw32/bin/{avcodec-60.dll,avdevice-60.dll,avfilter-9.dll,avformat-60.dll,avutil-58.dll} .
#zip yummy-latest.zip yummy.exe in_ffmpeg.dll out_null.dll avcodec-60.dll avdevice-60.dll avfilter-9.dll avformat-60.dll avutil-58.dll

# ffmpeg depends on many codec libraries out there
cp -f /mingw32/bin/* .
zip yummy-latest.zip yummy.exe *.dll

