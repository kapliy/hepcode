#!/bin/bash

# Looking for missing files in LOCALGROUPDISK

for f in `cat MISSING.dat`; do
    fname=`basename $f`
    fcache=`grep $fname cache.dat`
    if [ -z "$fcache" ]; then fcache=NONE; fi
    echo $fname $fcache
done
