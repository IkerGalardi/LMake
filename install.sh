#!/bin/sh

# Compile if necessary
make config=release 

# Copy the binary to /bin
sudo cp build/lmake /usr/bin/lmake

echo "For uninstalling just remove the lmake binary from /usr/bin"
