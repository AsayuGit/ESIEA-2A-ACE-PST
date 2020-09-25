INCLUDES = \
		-I include/ \
		-I include/System/ \
		-I include/Scenes/

SOURCES = \
		src/*.c \
		src/System/*.c

FILENAME = ACE

all:
	gcc -o $(FILENAME) $(SOURCES) $(INCLUDES) $$(sdl2-config --cflags --libs) -lSDL2_image -lSDL2_mixer

legacy:
	gcc -D SDL -o $(FILENAME) $(SOURCES) $(INCLUDES) -lSDL -lSDL_image -lSDL_mixer

dependencies:
	sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-mixer-dev

run: all
	./$(FILENAME)