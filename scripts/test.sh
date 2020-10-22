#!/bin/sh

make || echo "Compilation error, stopping tests..."

# Do internal tests
build/lmake --internal-test > build/test_res.log

# Do complete test
cd build/test/lmake
../../lmake build >> ../../test_res.log