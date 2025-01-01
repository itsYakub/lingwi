#!/bin/bash

if [ $(id -u) -ne 0 ]; then
	rm -f $HOME/.local/bin/lingwi
else
	rm -f /usr/bin/lingwi
fi
