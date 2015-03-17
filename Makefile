# Copyright (c) 2015 FAT-GYFT, MIT License
# All rights reserved

SRC=$(wildcard src/*.cpp src/*/*.cpp src/*/*/*.cpp)
O_FILES=$(SRC:.cpp=.o)

PROG=bin/lutcc

CC=g++
CC_FLAGS=-g -Wall -W -Wextra -std=c++11

LD=g++
LD_FLAGS=-g

LIBS=-lboost_regex -lboost_program_options
LIB_PATH=-L/usr/local/lib

INC=
INC_PATH=-I/usr/local/include

.PHONY: clean debug

$(PROG): $(O_FILES)
	mkdir -p bin
	$(LD) $(LD_FLAGS) $(INC_PATH) $(LIB_PATH) $(O_FILES) $(LIBS) -o $(PROG)

debug: CC_FLAGS := $(CC_FLAGS) -DDEBUG
debug: $(PROG)

%.o: %.cpp
	@echo Compiling $<...
	$(CC) -c $< $(CC_FLAGS) $(INC_PATH) -o $@
	@echo Compiled $< successfully

clean:
	rm -f $(O_FILES) $(PROG) core
