# I'm only using cross compilation here. This might cause issues.

CC = i686-w64-mingw32-gcc
CFLAGS = -Wall

TARGET = remote.exe
OBJECTS = remote.o ir.o led.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) libusb-1.0.a -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)