#! /bin/bash
rm -rf CMakeFiles  CMakeCache.txt    cmake_install.cmake  Makefile
cmake ..
make
make install

rm -rf *.so 
rm -rf test 
