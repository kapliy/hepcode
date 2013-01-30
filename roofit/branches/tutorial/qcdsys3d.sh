#!/bin/bash

python qcdsys3d.py 2 20   &&   python qcdsys3d.py 1 20 && python qcdsys3d.py 0 20   &&   python qcdsys3d.py 1 25 && python qcdsys3d.py 0 25 && echo "ALL DONE"

# experimental: 
python qcdsys3d.py 1 25 1
