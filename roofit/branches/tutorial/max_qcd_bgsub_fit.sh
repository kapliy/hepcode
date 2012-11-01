#!/bin/bash

# Studying QCD fits with bg subtraction: why doesn't it converge so frequently?

input=/share/t3data3/antonk/ana/ana_v29I_10212012_unfold_stacoCB_all  # renamed lepton_pt for consistency

input=/share/t3data3/antonk/ana/ana_v29I_10292012_unfold_newTrigWZPTnoPol_stacoCB_all # new trigger SF


./stack2.py --input ${input} -b --charge 1 --lvar wmt --lbin 50,35,100 --isofail IsoWind20 -o TEST -t isowind_wmt35to100 -m qcdfit_2d --bgsig 5 --bgewk 2 --preNN 10 --preNQ 5 --extra HISTO_10122012_ALL/HISTO_10122012_ALL.PART.1.5.2.10.5 --var met --xsecerr 0 --etamode 2 --rebin 2 --bgqcd 4

