#!/bin/sh

echo "[CHVAR] Existing variable"
$1 test VAR=changed

echo "[CHVAR] Non existant variable"
$1 test_no_var VARIABLE=changed