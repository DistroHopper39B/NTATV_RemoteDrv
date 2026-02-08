TARGET = appleirgui.exe
OBJECTS = gui.o
CFLAGS += -D_WIN32_IE=0x0500 -DWINVER=0x0500 -DUNICODE -D_UNICODE -municode

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -L../lib -lappleir -lcomctl32 -lgdi32 -municode -Wl,--subsystem,windows -o $@

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJECTS)