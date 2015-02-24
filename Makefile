# Copyright (c) 2015 Gustave Monod
# All rights reserved

SRC=$(wildcard *.cpp */*.cpp */*/*.cpp)
O_FILES=$(SRC:.cpp=.o)

PROG=bin/lutcc

CC=g++
CC_FLAGS=-g -Wall -W -Wextra

LD=g++
LD_FLAGS=-g

LIBS=-lboost_regex-mt
LIB_PATH=-L/usr/local/lib

INC=
INC_PATH=-I/usr/local/include

.PHONY: clean

$(PROG): $(O_FILES)
	$(LD) $(LD_FLAGS) $(INC_PATH) $(LIB_PATH) $(O_FILES) $(LIBS) -o $(PROG)

%.o: %.cpp
	@echo Compiling $<...
	$(CC) -c $< $(CC_FLAGS) $(INC_PATH) -o $@
	@echo Compiled $< successfully

clean:
	rm -f $(O_FILES) $(PROG) core
