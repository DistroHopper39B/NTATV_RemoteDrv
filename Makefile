# I'm only using cross compilation here. This might cause issues.

.NOTPARALLEL: $(SUBDIRS)

CC = i686-w64-mingw32-gcc
CFLAGS := -Wall -I$(realpath ./include)

SUBDIRS := lib/ cli/ gui/

export CC
export CFLAGS
export DLL

all clean: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: all $(SUBDIRS)