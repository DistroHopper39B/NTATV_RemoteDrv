TARGET = atvclient.exe
OBJECTS = atvclient.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -L$(TOP)/lib -Wl,-subsystem,windows -lappleir -o $@

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)