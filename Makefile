CC = cc
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g

SRC = main.c
LIBS = -lcurl

TARGET = lingwi

all: build install clean

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

install:
	cp $(TARGET) ~/.local/bin

clean:
	rm $(TARGET)

remove:
	rm ~/.local/bin/$(TARGET)
