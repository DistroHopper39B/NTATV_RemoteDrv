.PHONY: $(TARGET) $(TARGET_WIN)

TARGET = atvclient.exe
TARGET_WIN = atvclientw.exe
OBJECTS = atvclient.o

all: $(TARGET) $(TARGET_WIN)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -L$(TOP)/lib -Wl,-subsystem,console -lappleir -o $@

$(TARGET_WIN): $(TARGET)
	$(OBJCOPY) --subsystem windows $^ $@

clean:
	rm -f $(TARGET) $(TARGET_WIN) $(OBJECTS)
