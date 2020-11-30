mkdir -p build

echo "-- Compiling the project"
lmake build --verbose

echo "-- Cleaning up obj files"
lmake clean

echo "-- Executing test program"
lmake exec