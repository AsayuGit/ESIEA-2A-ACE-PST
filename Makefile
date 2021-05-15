# Recursive Wildcard function
rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2)$(filter $(subst *,%,$2),$d))

# Remove duplicate function
uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1))) 

# C Compilers
CC = gcc
WINCC = x86_64-w64-mingw32-gcc

# Compile / Link Flags
CFLAGS += -c -Wall -std=c89 -pedantic-errors -g
LDFLAGS = $$(sdl2-config --libs) $$(xml2-config --libs) -lSDL2_image -lSDL2_mixer

LEGACYLDFLAGS = "$$(sdl-config --libs) $$(xml2-config --libs) -lSDL_image -lSDL_mixer -lSDL_gfx"
WINLDFLAGS = "-L /usr/x86_64-w64-mingw32/lib/ -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lxml2 -lz -llzma -lm -mwindows"

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
INCLUDES = $(patsubst %, -I %, $(call uniq, $(dir $(call rwildcard,,*.h))))
LIBS = $$(sdl2-config --cflags) $$(xml2-config --cflags)
LEGACYLIBS = "-I/usr/include/SDL -D_GNU_SOURCE=1 -D_REENTRANT -I/usr/include/libxml2"

WINLIBS = "-I /usr/x86_64-w64-mingw32/include/libxml2/"

# Number of therads available 
CPUS = $(nproc)

multi:
	@$(MAKE) -j$(CPUS) --no-print-directory all

all: $(OBJ_DIR) $(OUT)

$(OBJ_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o: %.c
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $< $(INCLUDES) $(LIBS) -o $@

$(OUT): $(OBJ)
	@echo "Linking $@"
	@$(CC) -o $(OUT) $^ $(LDFLAGS)

clean:
	@echo "Cleaning Build"
	@rm -rf $(BUILD_DIR) $(OUT) $(OUT).exe

rebuild: clean
	@$(MAKE) -j$(CPUS) --no-print-directory all

run:
	./$(OUT)

legacy:
	@$(MAKE) --no-print-directory rebuild CFLAGS="-c -Wall -std=c89 -D _SDL -g" LIBS=$(LEGACYLIBS) LDFLAGS=$(LEGACYLDFLAGS)

windows:
	@$(MAKE) --no-print-directory rebuild CC=$(WINCC) OUT=ACE.exe LIBS=$(WINLIBS) LDFLAGS+=$(WINLDFLAGS)