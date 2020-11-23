#!/bin/bash

dir_name=$1
output_file_name=$2

sum_size=0

for file in `find $dir_name -type f`
do
	size=$(stat -c%s "$file")
	sum_size=$((sum_size + size))
done

echo $sum_size > $output_file_name.txt