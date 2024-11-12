#!/bin/bash

CC=cc
CFLAGS="-Wall -Wextra -Werror -Wpedantic -g"
CINPUT=main.c
COUTPUT=lingwi
CLIBS=-lcurl

$CC $CFLAGS $CINPUT -o $COUTPUT $CLIBS
