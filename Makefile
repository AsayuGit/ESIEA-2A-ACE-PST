# Recursive Wildcard function
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

# Remove duplicate function
uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1))) 

# C Compilers
CC = gcc
WINCC = x86_64-w64-mingw32-gcc

# Compile / Link Flags
CFLAGS = -c -Wall
LDFLAGS = $$(sdl2-config --libs) $$(xml2-config --libs) -lSDL2_image -lSDL2_mixer

# Main target and filename of the executable
OUT = ACE

# Build Directory
BUILD_DIR = build

# List of all the .c source files to compile
SRC = $(call rwildcard,,*.c)

# List of all the .o object files to produce
OBJ = $(patsubst %,$(BUILD_DIR)/%,$(SRC:%.c=%.o))
OBJ_DIR = $(call uniq, $(dir $(OBJ)))

# List of all includes directory
INCLUDES = $(patsubst %, -I %, $(call uniq, $(dir $(call rwildcard,,*.h)))) $$(sdl2-config --cflags) $$(xml2-config --cflags)

all: $(OBJ_DIR) $(OUT)

$(OBJ_DIR):
	mkdir -p $@

$(BUILD_DIR)/%.o: %.c
	@echo "Compiling $<"
	$(CC) $(CFLAGS) $< $(INCLUDES) -o $@

$(OUT): $(OBJ)
	@echo "Linking $<"
	$(CC) -o $(OUT) $^ $(LDFLAGS)

clean:
	rm -rf $(BUILD_DIR) $(OUT)

rebuild: clean all

run:
	./$(OUT)