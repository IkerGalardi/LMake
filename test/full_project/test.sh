mkdir -p build

echo "-- Compiling the project"
$1 build --verbose

echo "-- Cleaning up obj files"
$1 clean

echo "-- Executing test program"
$1 exec