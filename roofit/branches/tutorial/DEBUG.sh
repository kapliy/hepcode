#!/bin/bash

# 04/25/2013: additional qcdfit studies
fin=/atlas/uct3/data/users/antonk/ana/ana_v29I_04252013_rep_stacoCB_all/pt20

###fin=/atlas/uct3/data/users/antonk/ana/ana_v29I_04252013_stacoCB_all/pt20

rm -f CRAP.db
./stack2.py --extra CRAP --exit --input ${fin} -b --charge 1 --lvar d3_abseta_lpt_met --lbin 50,0,40 --isofail IsoWind20 -o QCD -t Q1 -m qcdfit --bgsig 5 --bgewk 5 --bgtau 2 --bgqcd 4 --ipt 2 --var d3_abseta_lpt_met --xsecerr 0 --etamode 2 --rebin 4 --ieta 2
