# I'm only using cross compilation here. This might cause issues.

.NOTPARALLEL: $(SUBDIRS)

TOP := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
BINDIR := $(dir $(realpath bin))

CC = i686-w64-mingw32-gcc
CFLAGS := -Wall -fms-extensions -I$(realpath ./include)
OBJCOPY = i686-w64-mingw32-objcopy
WINDRES = i686-w64-mingw32-windres

ROOTDIR = $(PWD)

SUBDIRS := lib/ app/

export TOP
export CC
export CFLAGS
export OBJCOPY
export BINDIR
export WINDRES

all clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: all $(SUBDIRS)
