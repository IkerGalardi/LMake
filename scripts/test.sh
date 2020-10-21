#!/bin/sh

make || echo "Compilation error, stopping tests..."

# Do internal tests
build/lmake --internal-test

# Do complete test
cd build/test/lmake
../../lmake build >> ../test_res

# TODO: Compare the results
