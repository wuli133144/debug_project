#!/bin/sh

prefix=/usr/local/test
exec_prefix=/usr/local/test
libdir=${exec_prefix}/lib

LD_PRELOAD=${libdir}/libjemalloc.so.2
export LD_PRELOAD
exec "$@"
