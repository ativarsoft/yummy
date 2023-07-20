#!/bin/bash -e

cp -f /usr/bin/{avcodec-60.dll,avdevice-60.dll,avfilter-9.dll,avformat-60.dll,avutil-58.dll} .
zip yummy-latest.zip yummy.exe in_ffmpeg.dll out_null.dll avcodec-60.dll avdevice-60.dll avfilter-9.dll avformat-60.dll avutil-58.dll

