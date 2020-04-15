#! /bin/bash

cd "`dirname \"$0\"`"

rm cmake_install.cmake CMakeCache.txt Makefile assignment1
rm -rf CMakeFiles/

cmake ./

make

./assignment1