INCLUDES = \
		-I include/ \
		-I include/System/ \
		-I include/Scenes/ \
		-I include/Characters/

SOURCES = \
		src/*.c \
		src/System/*.c \
		src/Scenes/*.c \
		src/Characters/*.c

FILENAME = ACE

all:
	clear
	gcc -o $(FILENAME) $(SOURCES) $(INCLUDES) $$(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_mixer

legacy:
	clear
	gcc -D SDL -o $(FILENAME) $(SOURCES) $(INCLUDES) -lSDL -lSDL_image -lSDL_mixer

dependencies:
	sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

run: all
	./$(FILENAME)