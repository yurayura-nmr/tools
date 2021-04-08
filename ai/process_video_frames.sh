#!/bin/bash

# Using the simple frame-by-frame method.
# Seems to require some amount of memory.

for filename in *.png; do
    for ((i=0; i<=3; i++)); do
        cp $filename image.png
        ./a.out
        mv upscaled.png hq_$filename
        echo "[ Done ] ", $filename
        #./MyProgram.exe "$filename" "Logs/$(basename "$filename" .txt)_Log$i.txt"
    done
done
