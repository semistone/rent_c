#!/bin/sh
gcc -Wall  -o hook.so hook.c  -fPIC -shared -ldl
gcc -o test_gethost test_gethost.c
gcc -o test_getaddr test_getaddr.c
javac x.java
export LD_PRELOAD=`pwd`/hook.so
./test_gethost tw.yahoo.com
./test_getaddr tw.yahoo.com
java x 
