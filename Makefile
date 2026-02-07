# I'm only using cross compilation here. This might cause issues.

CC = i686-w64-mingw32-gcc
CFLAGS = -Wall

TARGET_APP = remote.exe
TARGET_LIB = appleir.dll
OBJECTS_APP = remote.o
OBJECTS_LIB = lib/ir.o lib/led.o lib/keymap.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET_LIB) $(TARGET_APP)

$(TARGET_APP): $(OBJECTS_APP)
	$(CC) $(OBJECTS_APP) -L. -lappleir -o $@

$(TARGET_LIB): $(OBJECTS_LIB)
	$(CC) $(OBJECTS_LIB) lib/libusb-1.0.a -static-libgcc -shared -o $@

clean:
	rm -f $(OBJECTS_APP) $(OBJECTS_LIB) $(TARGET_APP) $(TARGET_LIB)