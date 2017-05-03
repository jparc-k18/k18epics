#!/usr/bin/make

SRC_DIR     = src
BIN_DIR     = bin

CXX     = g++
CFLAGS  = -O2 -Wall
FLAGS   = $(CFLAGS) -I.
LIB     =

root_config	:= root-config
root_include	:= $(shell $(root_config) --cflags)
root_lib	:= $(shell $(root_config) --libs)

FLAGS	+= $(root_include)
LIB	+= $(root_lib)

#_______________________________________________________________________________
all: $(BIN_DIR)/epics_alarm

$(BIN_DIR)/epics_alarm: $(SRC_DIR)/epics_alarm.o
	@echo "Linking $@"
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(FLAGS) -o $@ $^ $(LIB)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo "Compiling $<"
	@$(CXX) $(FLAGS) -c $< -o $@

clean:
	@echo "Cleaning up ..."
	@rm -f $(BIN_DIR)/*
	@rm -f $(SRC_DIR)/*.o

