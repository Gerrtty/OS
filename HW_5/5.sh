#!/bin/bash

file_to_write=$1
dir_name=$2
extension=$3

if [ -f $file_to_write ]; then
   rm $file_to_write
fi

for f in $HOME$dir_name/*; do
	filename=$(basename "$f")
	ex=${filename##*.}
	if [ $ex != $extension ]; then continue; fi
	echo ${filename};

done > $file_to_write