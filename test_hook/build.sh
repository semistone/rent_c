#!/bin/sh
gcc -Wall  -o hook.so hook.c  -fPIC -shared -ldl
gcc -o test_gethost test_gethost.c
export LD_PRELOAD=`pwd`/hook.so
./test_gethost tw.yahoo.com
