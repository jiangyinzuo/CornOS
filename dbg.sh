#!/usr/bin/env sh

make gdb &
sleep 1
gdb -q -x tools/gdbinit
