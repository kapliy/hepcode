#!/bin/bash

# DQ2 to XROOTD

./dq2xrdcp.sh /atlas/uct3/data/users/$USER/NTUPLE/v1_29i/v1_i29i/ `cat LIST`

echo DONE
