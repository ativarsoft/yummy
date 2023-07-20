#!/bin/bash -e

cp -f /usr/lib/{avcodec-60.dll,avformat-60.dll,avutil-58.dll} .
zip yummy-latest.zip yummy.exe in_ffmpeg.dll out_null.dll avcodec-60.dll avformat-60.dll avutil-58.dll

