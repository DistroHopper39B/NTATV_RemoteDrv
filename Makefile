# I'm only using cross compilation here. This might cause issues.

.NOTPARALLEL: $(SUBDIRS)

TOP := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

CC = i686-w64-mingw32-gcc
CFLAGS := -Wall -fms-extensions -I$(realpath ./include)

ROOTDIR = $(PWD)

SUBDIRS := lib/ cli/

export TOP
export CC
export CFLAGS
export DLL

all clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: all $(SUBDIRS)