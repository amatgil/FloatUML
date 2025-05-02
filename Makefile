RAYLIB ?= ./external/raylib-5.5/src/

desktop:
	gcc src/main.c -I $(RAYLIB) -L $(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o floatUML
