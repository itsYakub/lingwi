CC = cc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g

SRC = main.c
LIBS = -lcurl

TARGET = lingwi

all: build install clean

.PHONY: build install rebuild reinstall clean remove

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

install: $(TARGET)
ifneq ($(shell id -u), 0)
		cp $(TARGET) ~/.local/bin
else
		cp $(TARGET) /usr/bin
endif


rebuild: clean build

reinstall: clean build install clean

clean:
	rm $(TARGET)

remove:
ifneq ($(shell id -u), 0)
		rm -f ~/.local/bin/$(TARGET)
else
		rm -f /usr/bin/$(TARGET)
endif
