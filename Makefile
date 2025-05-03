RAYLIB ?= ./external/raylib-5.5/src/ 
CFLAGS = -m64 -std=c99 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual


desktop:
	gcc src/main.c -O2 -I $(RAYLIB) -L $(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o floatUML $(CFLAGS) -fsanitize=undefined

clean:
	rm floatUML
