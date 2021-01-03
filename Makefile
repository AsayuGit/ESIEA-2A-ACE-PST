INCLUDES = \
		-I include/ \
		-I include/System/ \
		-I include/Scenes/ \
		-I include/Characters/ \
		-I include/Menus/ \
		-I include/Sound/

WINDOWS_INCLUDES = -I /usr/local/x86_64-w64-mingw32/include/
WINDOWS_LIBS = -L /usr/local/x86_64-w64-mingw32/lib/

SOURCES = \
		src/*.c \
		src/System/*.c \
		src/Scenes/*.c \
		src/Characters/*.c \
		src/Menus/*.c \
		src/Sound/*.c

FILENAME = ACE

all:
	clear
	gcc -o $(FILENAME) $(SOURCES) $(INCLUDES) $$(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_mixer

legacy:
	clear
	gcc -D _SDL -o $(FILENAME) $(SOURCES) $(INCLUDES) $$(sdl-config --cflags --libs) -lSDL_image -lSDL_mixer

windows:
	clear
	x86_64-w64-mingw32-gcc -o $(FILENAME).exe $(SOURCES) $(INCLUDES) $(WINDOWS_INCLUDES) $(WINDOWS_LIBS) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -mwindows

StaticWindows:
	clear
	x86_64-w64-mingw32-gcc -o $(FILENAME).exe $(SOURCES) $(INCLUDES) $(WINDOWS_INCLUDES) $(WINDOWS_LIBS) -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -mwindows -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc

dependencies:
	sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

clean:
	rm ACE

run:
	./$(FILENAME)