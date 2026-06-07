#!/usr/bin/env sh

mkdir -p $(dirname "${DST_DIR}/$TRG")

$CC $CFLAGS $SRC_LIST -o ${DST_DIR}/$TRG  && echo ${TRG} [OK]
