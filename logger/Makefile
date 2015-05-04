### Dirs
SRC_DIR     = src
BIN_DIR     = bin

### ROOT
RTINCLUDE = $(shell root-config --cflags)
RTLIBS    = $(shell root-config --glibs)

### Flags
CXX     = g++
CFLAGS  = -O2 -Wall
FLAGS   = $(CFLAGS) $(RTINCLUDE) -I.
LIB     = $(RTLIBS)


all: $(BIN_DIR)/epics_logger

$(BIN_DIR)/epics_logger: $(SRC_DIR)/epics_logger.o
	@echo Linking $@
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(FLAGS) -o $@ $^ $(LIB) 

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cc
	@echo Compiling $< ...
	@$(CXX) $(FLAGS) -c $< -o $@ 

clean:
	@echo Cleaning up ...
	@rm -f $(BIN_DIR)/*
	@rm -f $(SRC_DIR)/*.o