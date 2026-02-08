TARGET = remote.exe
OBJECTS = remote.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -L../lib -lappleir -o $@

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)