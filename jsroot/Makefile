#!/usr/bin/make

# Author: Shuhei Hayakawa

target	= EpicsJsRoot

color	= 1

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

echo	= echo -e

#### escape sequence ####
ifeq ($(color),1)
black		= \033[0;30;1m
red		= \033[0;31;1m
green		= \033[0;32;1m
yellow		= \033[0;33;1m
blue		= \033[0;34;1m
magenta		= \033[0;35;1m
cyan		= \033[0;36;1m
white		= \033[0;37;1m
default_color	= \033[0m
endif

#_______________________________________________________________________________
.PHONY: all clean

all: $(bin_dir)/$(target)

-include $(dependencies)

$(bin_dir)/$(target): $(objects)
	@ $(echo) "$(magenta) === Linking $@ $(default_color)"
	@ mkdir -p $(bin_dir)
	$(CXX) $(FLAGS) -o $@ $^ $(LIB)

$(build_dir)/%.o: $(src_dir)/%.cc
	@ $(echo) "$(green) === Compiling $@ ... $(default_color)"
	@ mkdir -p $(build_dir)
	$(CXX) $(FLAGS) -o $@ -MMD -c $<

clean:
	@ $(echo) "$(green) === Cleaning up ... $(default_color)"
	@ rm -rfv $(build_dir)
	@ find . -name "*~" -exec \rm -fv \{\} \;
