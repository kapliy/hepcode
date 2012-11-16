#!/bin/bash

# cross-check with Wc/W paper
# note1: this uses ntuples, and can thus be used to try out different cutflows.
# note2: nominal histogram ONLY
# note3: can try out different qcd fits

input=/share/t3data3/antonk/ana/ana_v29I_11072012_unfold_stacoCB_all/

# --refline 0.5,1.5
#--refline 0.94,1.06

# Wc/W selection
bgqcd=4
iz=0
for q in 0 1; do
    for bgsig in 5 ; do
	for var in "--var l_eta --bin 10,0.0,2.5 --hsource lepton_etav" "--var l_pt --bin 120,0,120 --hsource l_pt" "--var met --bin 100,0,100 --hsource met" "--var w_mt --bin 120,0,120 --hsource w_mt"; do
	    echo "CHARGE $q BGSIG ${bgsig} : [${var}]"
	    vname=`echo ${var} | awk '{print $2}'`
	    if [ "0" == "Wc/W paper" ]; then
		./stack2.py --input ${input} -b ${var} --charge ${q} -o STACKNTWc -t plot_${vname}_${bgsig} --pre "fabs(z0)<1.0 && fabs(d0sig)<2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'fabs(z0)<1.0 && fabs(d0sig)<2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'fabs(z0)<1.0 && fabs(d0sig)>2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'fabs(z0)<1.0 && fabs(d0sig)<2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'fabs(z0)<1.0 && fabs(d0sig)>2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*vxw*ls1w*ls2w*effw*isow*trigw" -m one_plot_nt --bgsig ${bgsig} --bgewk ${bgsig} --bgqcd ${bgqcd} --refline 0.8,1.2 &> LOG.XCHECK.${iz} &
		((iz++))
	    fi
	    wmt=40; if [ "$vname" == "w_mt" ]; then wmt=0; fi
	    if [ "winclusive20" == "winclusive20" ]; then
		./stack2.py --input ${input} -b ${var} --charge ${q} -o STACKNT20 -t plot_${vname}_${bgsig} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNQ "ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons<2" --preFN "ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preFQ "ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2" -m one_plot_nt --bgsig ${bgsig} --bgewk ${bgsig} --bgqcd ${bgqcd} --refline 0.8,1.2 &> LOG.XCHECK.inc20.${iz} &
		((iz++))
	    fi
	    if [ "winclusive25" == "winclusive25" ]; then
		./stack2.py --input ${input} -b ${var} --charge ${q} -o STACKNT25 -t plot_${vname}_${bgsig} --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN "ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNQ "ptiso20/l_pt>0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons<2" --preFN "ptiso40/l_pt<0.1 && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preFQ "ptiso20/l_pt>0.1 && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2" -m one_plot_nt --bgsig ${bgsig} --bgewk ${bgsig} --bgqcd ${bgqcd} --refline 0.8,1.2 &> LOG.XCHECK.inc25.${iz} &
		((iz++))
	    fi
	done
    done
done

echo "Waiting for ${iz} jobs"
wait
