#!/usr/bin/env sh

# Compile raylib
if [ $1 == "web" ]; then
	(cd external/raylib-5.5/src/ && make PLATFORM=PLATFORM_WEB);
	make web
else
	(cd external/raylib-5.5/src/ && make PLATFORM=PLATFORM_DESKTOP);
	make desktop
fi

