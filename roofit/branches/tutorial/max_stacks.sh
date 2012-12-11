#!/bin/bash

# Control plot stacks

input=/share/t3data3/antonk/ana/ana_v29I_11072012_unfold_stacoCB_all/
input=/share/t3data3/antonk/ana/ana_v29I_12042012_edboard_stacoCB_all

bgqcd=4
OUTPR=OUT_12042012_ALL.v2
OUTPR=OUT_12102012_ALL.v2

# eta plots for different MC
if [ "1" == "0" ]; then
    ./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTPR}.eta.1D.root:qcd_MCAverage" &
    #./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t PowhegHerwig -m control_stack --bgsig 4 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTPR}.eta.1D.root:qcd_MCAverage" &
    #./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t McAtNlo -m control_stack --bgsig 1 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTPR}.eta.1D.root:qcd_MCAverage" &
fi
if [ "0" == "1" ]; then # double ratio
    ./stack2.py -b --input ${input} --hsource "lepton_etav" -o CONTROL -t PowhegPythia_dratio -m control_stack_dratio --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTPR}.eta.1D.root:qcd_MCAverage" &
fi

# abseta plots for different MC
if [ "1" == "1" ]; then
    ./stack2.py -b --input ${input} --hsource "lepton_absetav" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTPR}.abseta.1D.root:qcd_Nominal"
    #./stack2.py -b --input ${input} --hsource "lepton_absetav" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTPR}.abseta.1D.root:qcd_MCAverage"
    
    #./stack2.py -b --input ${input} --hsource "lepton_absetav" -o CONTROL -t PowhegHerwig -m control_stack --bgsig 4 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTPR}.abseta.1D.root:qcd_MCAverage" &
    #./stack2.py -b --input ${input} --hsource "lepton_absetav" -o CONTROL -t McAtNlo -m control_stack --bgsig 1 --bgewk 5 --bgqcd ${bgqcd} --refline 0.94,1.06 --qcdsource "${OUTPR}.abseta.1D.root:qcd_MCAverage" &
fi

# W kinematic variables [ nominal ]
if [ "0" == "1" ]; then
    ./stack2.py -b --input ${input} --hsource "wmt" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.81,1.19 &
    ./stack2.py -b --input ${input} --hsource "wpt" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.81,1.19 &
    ./stack2.py -b --input ${input} --hsource "met" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.81,1.19 &
fi

# W kinematic variables [alpgen]
if [ "0" == "1" ]; then
    ./stack2.py -b --input ${input} --hsource "wmt" -o CONTROL -t AlpgenHerwig -m control_stack --bgsig 2 --bgewk 2 --bgqcd ${bgqcd} --refline 0.81,1.19 &
    ./stack2.py -b --input ${input} --hsource "wpt" -o CONTROL -t AlpgenHerwig -m control_stack --bgsig 2 --bgewk 2 --bgqcd ${bgqcd} --refline 0.81,1.19 &
    ./stack2.py -b --input ${input} --hsource "met" -o CONTROL -t AlpgenHerwig -m control_stack --bgsig 2 --bgewk 2 --bgqcd ${bgqcd} --refline 0.81,1.19 &
fi

# Lepton pT
if [ "0" == "1" ]; then
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegHerwig -m control_stack --bgsig 4 --bgewk 5 --bgqcd ${bgqcd} --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t McAtNlo -m control_stack --bgsig 1 --bgewk 5 --bgqcd ${bgqcd} --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia_sc -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.91,1.09 --qcdscale 0.195 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia_qcdsub -m control_stack --bgsig 5 --bgewk 5 --bgqcd 4 --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia_qcdmc -m control_stack --bgsig 5 --bgewk 5 --bgqcd 0 --refline 0.91,1.09 &
    ./stack2.py -b --input ${input} --hsource "lpt" -o CONTROL -t PowhegPythia_isowind --isofail IsoWind20 -m control_stack --bgsig 5 --bgewk 5 --bgqcd ${bgqcd} --refline 0.91,1.09 &
fi


echo "Please wait..."
wait
