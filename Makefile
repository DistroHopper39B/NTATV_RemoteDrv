# I'm only using cross compilation here. This might cause issues.

CC = i686-w64-mingw32-gcc
CFLAGS = -Wall

TARGET = remote.exe
OBJECTS = remote.o ir.o led.o libusb-1.0.a

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

clean:
	rm -f *.o $(TARGET)