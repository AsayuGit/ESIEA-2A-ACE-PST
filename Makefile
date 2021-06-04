#   Mia's Legacy is a Ace Attorney fangame taking place directly
#   after the first game in the serie. All code in this repo have
#   been written from scratch in ANSI C using SDL and libxml2.
#   This game is designed to run on Linux Windows and the og Xbox
#
#   Copyright (C) 2021 Killian RAIMBAUD (killian.rai@gmail.com)
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; version 2 of the License.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.



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