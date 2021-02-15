#!/bin/sh

make

cd test

lmake_executable="../build/lmake"

echo "[+] Running internal test"
cd internal 
sh test.sh ../$lmake_executable
cd ..

echo 
echo "[+] Running full project test"
cd full_project
sh test.sh ../$lmake_executable
cd ..

echo 
echo "[+] Running variable changing test"
cd chvar
sh test.sh ../$lmake_executable
cd ..

echo 
echo "[+] Running regex test"
cd regex
sh test.sh ../$lmake_executable
cd ..