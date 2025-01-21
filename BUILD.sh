#!/bin/bash

CC=cc
CFLAGS="-Wall -Wextra -Werror -Wpedantic -g"
CINPUT="./src/lingwi-main.c ./src/lingwi-core.c ./src/lingwi-opt.c ./src/lingwi-trans.c"
COUTPUT=lingwi
CLIBS=-lcurl
CINCS=-I./inc/

$CC $CFLAGS $CINPUT -o $COUTPUT $CLIBS $CINCS
