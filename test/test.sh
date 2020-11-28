#!/bin/sh

cd test

echo "[+] Running internal test"
cd internal
sh test.sh
cd ..

echo 
echo "[+] Running full project test"
cd full_project
sh test.sh
cd ..