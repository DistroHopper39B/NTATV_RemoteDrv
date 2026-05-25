TARGET = appleir.dll
OBJECTS = appleir.o ir.o led.o # keymap.o

all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -Wl,-subsystem,windows -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) libusb-1.0.a -static-libgcc -shared -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)
