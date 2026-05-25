# I'm only using cross compilation here. This might cause issues.

.NOTPARALLEL: $(SUBDIRS)

TOP := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BINDIR := $(dir $(realpath bin))

CC = i686-w64-mingw32-gcc
CFLAGS := -Wall -fms-extensions -I$(realpath ./include)
OBJCOPY = i686-w64-mingw32-objcopy

ROOTDIR = $(PWD)

SUBDIRS := lib/ cli/

export TOP
export CC
export CFLAGS
export OBJCOPY
export BINDIR

all clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: all $(SUBDIRS)
