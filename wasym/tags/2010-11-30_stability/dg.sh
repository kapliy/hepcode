#!/bin/bash
/home/antonk/TrigFTKAna/plot/dgplot -q -b $1 $2 | grep '+' | awk 'NF>15{print $0}'
