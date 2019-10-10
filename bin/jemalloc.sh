#!/bin/sh

prefix=/usr/local/test_clibrary
exec_prefix=/usr/local/test_clibrary
libdir=${exec_prefix}/lib

LD_PRELOAD=${libdir}/libjemalloc.so.2
export LD_PRELOAD
exec "$@"
