#!/bin/bash

tag=FEB09
#for m in wstack zstack truth unfold asym; do
for m in wstack; do
    echo "Starting ${m}"
    ./TODAY.sh -o ${tag}_${m} -m ${m} &> LOG.${tag}.${m}
    echo "Ending ${m}"
done;
