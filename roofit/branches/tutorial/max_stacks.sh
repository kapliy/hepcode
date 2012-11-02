#!/bin/bash

# Control plot stacks

input=/share/t3data3/antonk/ana/ana_v29I_10212012_unfold_stacoCB_all  # max's trigger SF
input=/share/t3data3/antonk/ana/ana_v29I_10292012_unfold_newTrigWZPTnoPol_stacoCB_all # new trigger SF

# eta plots for different MC
if [ "0" == "1" ]; then
    ./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd 3 --refline 0.94,1.06 --qcdsource 'OUT_10122012_ALL.v1.eta.1D.root:qcd_MCAverage' &
    ./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t PowhegHerwig -m control_stack --bgsig 4 --bgewk 5 --bgqcd 3 --refline 0.94,1.06 --qcdsource 'OUT_10122012_ALL.v1.eta.1D.root:qcd_MCAverage' &
    ./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t McAtNlo -m control_stack --bgsig 1 --bgewk 5 --bgqcd 3 --refline 0.94,1.06 --qcdsource 'OUT_10122012_ALL.v1.eta.1D.root:qcd_MCAverage' &
fi

# abseta plots for different MC
if [ "0" == "1" ]; then
    ./stack2.py -b --input ${input} --hsource "lepton_absetav" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd 3 --refline 0.94,1.06 --qcdsource 'OUT_10122012_ALL.v1.abseta.1D.root:qcd_MCAverage' &
    ./stack2.py -b --input ${input} --hsource "lepton_absetav" -o CONTROL -t PowhegHerwig -m control_stack --bgsig 4 --bgewk 5 --bgqcd 3 --refline 0.94,1.06 --qcdsource 'OUT_10122012_ALL.v1.abseta.1D.root:qcd_MCAverage' &
    ./stack2.py -b --input ${input} --hsource "lepton_absetav" -o CONTROL -t McAtNlo -m control_stack --bgsig 1 --bgewk 5 --bgqcd 3 --refline 0.94,1.06 --qcdsource 'OUT_10122012_ALL.v1.abseta.1D.root:qcd_MCAverage' &
fi

# W kinematic variables
if [ "0" == "1" ]; then
    ./stack2.py -b --input ${input} --hsource "wmt" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd 3 --refline 0.81,1.19 &
    ./stack2.py -b --input ${input} --hsource "wpt" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd 3 --refline 0.81,1.19 &
    ./stack2.py -b --input ${input} --hsource "met" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd 3 --refline 0.81,1.19 &
fi

# Lepton pT
if [ "1" == "1" ]; then
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd 3 --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegHerwig -m control_stack --bgsig 4 --bgewk 5 --bgqcd 3 --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t McAtNlo -m control_stack --bgsig 1 --bgewk 5 --bgqcd 3 --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia_sc -m control_stack --bgsig 5 --bgewk 5 --bgqcd 3 --refline 0.91,1.09 --qcdscale 0.195 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia_qcdsub -m control_stack --bgsig 5 --bgewk 5 --bgqcd 4 --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia_qcdmc -m control_stack --bgsig 5 --bgewk 5 --bgqcd 0 --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia_isowind --isofail IsoWind20 -m control_stack --bgsig 5 --bgewk 5 --bgqcd 3 --refline 0.91,1.09 &
fi


echo "Please wait..."
wait
