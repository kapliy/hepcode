#!/bin/bash

cat  /home/antonk/TrigFTKAna/CommonAnalysis/MuonMomentumCorrections/share/Data11_Scale_KC_values_staco_Rel17.dat | awk 'NF==1{print $0}NF>1{printf("%.2f $\\pm$ %.2f & %.2f $\\pm$ %.2f \\\\\n",$1,$2,$4,$5)}'
