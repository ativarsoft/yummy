#!/bin/bash -e

PLUGINS=yummy.exe \
  in_ffmpeg.dll \
  out_null.dll

zip yummy-latest.zip $(PLUGINS)

