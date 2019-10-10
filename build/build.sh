#! /bin/bash
rm -rf CMakeFiles  CMakeCache.txt    cmake_install.cmake  Makefile
rm -rf ../container/CMakeFiles  ../container/CMakeCache ../container/cmake_install.cmake 
cmake ..
make
make install

rm -rf *.so 
rm -rf test 
