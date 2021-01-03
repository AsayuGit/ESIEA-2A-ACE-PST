INCLUDES = \
		-I include/ \
		-I include/System/ \
		-I include/Scenes/ \
		-I include/Characters/ \
		-I include/Menus/ \
		-I include/Sound/

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

dependencies:
	sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

clean:
	rm ACE

run:
	./$(FILENAME)