#!/usr/bin/env sh

export SRC_DIR="./src"
export DST_DIR="./_build"

export CC=clang
export CFLAGS="-std=c89 -O0 -g -I./include -I./src -I./dpd/include "
export LDFLAGS=""

export CCBuild="sh ./scripts/cc.sh"

#-- BEGIN buld target -----------------------------------------------

export SRC_LIST="./src/c4mk.c"
export TRG="c4mk"
$CCBuild 

export SRC_LIST="./src/getopt.c ./tests/test_getopt.c "
export TRG="test_getopt"
$CCBuild 

export SRC_LIST="./src/log.c ./tests/test_log.c "
export TRG="test_log"
$CCBuild 

#-- END buld target -----------------------------------------------

echo [OK]
