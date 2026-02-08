TARGET = appleir.dll
OBJECTS = appleir.o ir.o led.o keymap.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) libusb-1.0.a -static-libgcc -shared -o $@

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)