CC = cc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g

SRC = main.c
LIBS = -lcurl

TARGET = lingwi

all: build install clean

.PHONY: build install rebuild reinstall clean remove

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

install:
	cp $(TARGET) ~/.local/bin

rebuild: clean build

reinstall: clean build install clean

clean:
	rm $(TARGET)

remove:
	rm ~/.local/bin/$(TARGET)
