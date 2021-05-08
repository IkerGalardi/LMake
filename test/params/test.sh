#!/bin/sh

echo "-- lmake_set_compiler"
$1 test_noparams1

echo "-- lmake_set_compiler_flags"
$1 test_noparams2

echo "-- lmake_set_compiler_out"
$1 test_noparams3

echo "-- lmake_set_compile"
$1 test_noparams4

echo "-- lmake_set_linker"
$1 test_noparams5

echo "-- lmake_set_linker_flags"
$1 test_noparams6

echo "-- lmake_set_linker_out"
$1 test_noparams7

echo "-- lmake_set_link"
$1 test_noparams8

echo "-- lmake_chdir"
$1 test_noparams9

echo "-- lmake_exec"
$1 test_noparams10

echo "-- lmake_error"
$1 test_noparams11

echo "-- lmake_find"
$1 test_noparams12
