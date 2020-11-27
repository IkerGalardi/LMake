#!/bin/sh

cd test

echo "[+] Running internal test"
cd internal
sh test.sh
cd ..

echo 
echo "[+] Running find test"
cd find
sh test.sh
cd ..

echo 
echo "[+] Running compilation test"
cd compile
sh test.sh
cd ..

echo 
echo "[+] Running linking test"
cd link
sh test.sh
cd ..

echo 
echo "[+] Running full project test"
cd full_project
sh test.sh
cd ..