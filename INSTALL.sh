#!/bin/bash

if [ $(id -u) -ne 0 ]; then
	cp ./lingwi $HOME/.local/bin
else
	cp ./lingwi /usr/bin
fi
