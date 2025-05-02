#!/usr/bin/env sh

# Compile raylib
(cd external/raylib-5.5/src/ && make PLATFORM=PLATFORM_DESKTOP)

# Compile project
make desktop
