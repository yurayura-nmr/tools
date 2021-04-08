#!/bin/bash

# Using the simple frame-by-frame method.
# Seems to require some amount of memory.

for filename in *.png; do
    cp $filename image.png
    ./a.out
    mv upscaled.png hq_$filename
    echo "[ Done ] ", $filename   
done
