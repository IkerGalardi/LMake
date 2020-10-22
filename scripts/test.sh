#!/bin/sh

make || echo "Compilation error, stopping tests..."

# Do internal tests
build/lmake --internal-test

# Do complete test
cd build/test/lmake
../../lmake build

# TODO: Compare the results
