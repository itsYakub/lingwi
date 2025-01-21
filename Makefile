CC = cc
CFLAGS = -Wall -Wextra -Werror -Wpedantic
SRC = ./src/lingwi-main.c ./src/lingwi-core.c ./src/lingwi-opt.c ./src/lingwi-trans.c
LFLAGS = -lcurl
IFLAGS = -I./inc/

TARGET = lingwi

all: build install clean

.PHONY: build install rebuild reinstall clean remove

build:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LFLAGS) $(IFLAGS)

install: $(TARGET)
ifneq ($(shell id -u), 0)
		cp $(TARGET) ~/.local/bin
else
		cp $(TARGET) /usr/bin
endif

rebuild: clean build

reinstall: clean build install clean

clean:
	rm -f $(TARGET)

remove: clean
ifneq ($(shell id -u), 0)
		rm -f ~/.local/bin/$(TARGET)
else
		rm -f /usr/bin/$(TARGET)
endif
