#!/bin/bash

source_name=$1
build_name=$2

output=$(gcc -o $build_name.exe $source_name 2>&1)
if [ $? != 0 ]; then
    echo -e "Error:\n$output"
else
    ./$build_name.exe
fi