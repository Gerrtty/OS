#!/bin/bash

file_to_write=$1
dir_name=$2
extension=$3

if [ -f $file_to_write ]; then
   rm $file_to_write
fi

f=/$dir_name/*;
filenames=$(basename "$f")
for file in $filenames; do
	ex=${file##*.}
	if [ $ex != $extension ]; then continue; fi
	echo ${file};
done > $file_to_write