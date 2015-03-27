# Copyright (c) 2015 FAT-GYFT, MIT License
# All rights reserved

SRC=$(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp)
OBJECTS=$(SRC:.cpp=.o)

PROG=bin/lutcc

CC=g++
CC_FLAGS=-pedantic -Wall -W -Wextra -std=c++11
TARGET ?= release

ifeq ($(TARGET), release)
    CC_FLAGS +=-DNDEBUG
else
    CC_FLAGS +=-DDEBUG
endif

LD=g++
LD_FLAGS=-g

LIBS=-lboost_regex -lboost_program_options
LIB_PATH=-L/usr/local/lib

INC=
INC_PATH=-I/usr/local/include

.PHONY: clean test

$(PROG): $(OBJECTS)
	mkdir -p bin
	$(LD) $(LD_FLAGS) $(INC_PATH) $(LIB_PATH) $(OBJECTS) $(LIBS) -o $(PROG)

test: $(PROG)
	@(cd test; ./mktest.sh)

%.o: %.cpp
	@echo Compiling $<...
	$(CC) -c $< $(CC_FLAGS) $(INC_PATH) -o $@
	@echo Compiled $< successfully

clean:
	rm -f $(OBJECTS) $(PROG) core
