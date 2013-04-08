#!/bin/bash

#ana/ana_wasym --noROOT --release 17 -s max_data.dat -n100000 --save-ntuples 7 --muon-offset 1 --muon-type 0 --muon-trig 18 --data-range DtoM --apply-pileup --pt 20 &
ana/ana_wasym --noROOT --release 17 -s max_wplus.dat -n100000 --save-ntuples 7 --muon-offset 1 --muon-type 0 --muon-trig 18 --data-range DtoM --apply-pileup --pt 20 &
#ana/ana_wasym --noROOT --release 17 -s max_wminus.dat -n100000 --save-ntuples 7 --muon-offset 1 --muon-type 0 --muon-trig 18 --data-range DtoM --apply-pileup --pt 20

wait
