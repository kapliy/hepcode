#!/bin/bash

python qcdsys3d.py 1 20 2 && python qcdsys3d.py 0 20 2   &&   python qcdsys3d.py 1 25 2 && python qcdsys3d.py 0 25 2   &&   python qcdsys3d.py 2 20 2   &&   echo "ALL DONE"
# use eta, not |eta| - to study A/C differences
python qcdsys3d.py 1 25 1

# a version with splitted ww,wz,zz,ttbar,stop
python qcdsys3d.py 1 20 2 split && python qcdsys3d.py 0 20 2 split  &&   python qcdsys3d.py 1 25 2 split && python qcdsys3d.py 0 25 2 split   &&   python qcdsys3d.py 2 20 2 split   &&   python qcdsys3d.py 1 25 1 split  &&  echo "ALL DONE"


