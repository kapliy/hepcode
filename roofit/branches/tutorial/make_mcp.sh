#!/bin/bash

./db_dumpscales.py -m15 -a0 > ~/mcp/MuonMomentumCorrections/share/Data11_Scale_kpkm_values_staco_Rel17.dat
./db_dumpscales.py -m15 -a1 > ~/mcp/MuonMomentumCorrections/share/Data11_Scale_kpkm_values_muid_Rel17.dat

./db_dumpscales.py -m15 -a0 -s > ~/mcp/MuonMomentumCorrections/share/Data11_Scale_KC_values_staco_Rel17.dat
./db_dumpscales.py -m15 -a1 -s > ~/mcp/MuonMomentumCorrections/share/Data11_Scale_KC_values_muid_Rel17.dat
