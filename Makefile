RAYLIB ?= ./external/raylib-5.5/src/
CFLAGS = -std=c99 -pedantic -Wall -Wshadow -Wpointer-arith -Wcast-qual -fno-omit-frame-pointer

desktop:
	gcc src/main.c -g -I $(RAYLIB) -L $(RAYLIB) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o floatUML $(CFLAGS) 

web:
    # emcc src/main.c -O2 -I $(RAYLIB) -L $(RAYLIB) -lraylib -lglfw -lGL -lm -lpthread -ldl -lrt -lX11 -o floatUML $(CFLAGS)
    #emcc -o game.html src/main.c -Os -Wall $(RAYLIB)libraylib.a -I. -I$(RAYLIB) -L. -L$(RAYLIB) -lraylib -s USE_GLFW=3 --shell-file $(RAYLIB)shell.html -DPLATFORM_WEB
	emcc -o game.html src/main.c -Os -Wall -I. -I$(RAYLIB) -L$(RAYLIB) -lraylib -s USE_GLFW=3 --shell-file $(RAYLIB)shell.html -DPLATFORM_WEB  


clean:
	rm floatUML
