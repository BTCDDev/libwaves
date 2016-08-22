#!/usr/bin/env bash
rm -rf o;
mkdir o;
cd o;
clang -w -c -I../src -I../src/utils -I../src/utils/include -L../src/utils ../src/*.c ../src/utils/*.c;
ar -x ../src/utils/libb2/src/.libs/libb2.a;
ar -cvq libwaves.a *.o;
cd ..;
