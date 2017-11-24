#!/usr/bin/make

# Author: Shuhei Hayakawa

target	= EpicsJsRoot

bin_dir		= bin
src_dir 	= src
include_dir	= include
build_dir	= $(src_dir)/build

root_include	= $(shell root-config --cflags)
root_libs	= $(shell root-config --glibs) -lRHTTP

CXX	= g++ -std=c++0x
CFLAGS	= -O2 -Wall
#DFLAGS	+= -DDEBUG
FLAGS	= $(CFLAGS) $(DFLAGS) $(root_include) -I. -I$(include_dir)
LIB	= $(root_libs)

sources		= $(wildcard $(src_dir)/*.cc)
headeres	= $(wildcard $(include_dir)/*.hh)
dependencies	= $(sources:$(src_dir)/%.cc=$(build_dir)/%.d)
objects		= $(sources:$(src_dir)/%.cc=$(build_dir)/%.o)

#_______________________________________________________________________________
.PHONY: all clean

all: $(bin_dir)/$(target)

-include $(dependencies)

$(bin_dir)/$(target): $(objects)
	@ echo === Linking $@
	@ mkdir -p $(bin_dir)
	$(CXX) $(FLAGS) -o $@ $^ $(LIB)

$(build_dir)/%.o: $(src_dir)/%.cc
	@ echo === Compiling $@ ...
	@ mkdir -p $(build_dir)
	$(CXX) $(FLAGS) -o $@ -MMD -c $<

clean:
	@echo === Cleaning up ...
	@rm -rf $(build_dir)
	@find . -name "*~" -exec \rm -fv \{\} \;
