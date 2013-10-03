#!/bin/bash

rm -f mg_to_muid_rw.root
for f in *.txt; do
    ./make.py $f
done
./ratios.py
