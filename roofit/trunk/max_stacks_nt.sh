#!/bin/bash

# cross-check with Wc/W paper
# note1: this uses ntuples, and can thus be used to try out different cutflows.
# note2: nominal histogram ONLY
# note3: can try out different qcd fits

instructions="""
dest=~/SupportingDocument/talk/Wmn/figures
cp -a plot_l_eta_5_NEG_default_Q4S5X5Y5Z5_met_0to60.pdf $dest/qcdFIT_Wmin_met_0to60.pdf
cp -a plot_w_mt_5_stack_w_mt_NEG.pdf $dest/stack_Wmin_wmt.pdf
cp -a plot_met_5_stack_met_NEG.pdf $dest/stack_Wmin_met.pdf
cp -a plot_l_pt_5_stack_l_pt_NEG.pdf $dest/stack_Wmin_lpt.pdf
cp -a plot_l_eta_5_stack_lepton_etav_NEG.pdf $dest/stack_Wmin_eta.pdf
cp -a plot_w_mt_5_stack_w_mt_POS.pdf $dest/stack_Wplus_wmt.pdf
cp -a plot_met_5_stack_met_POS.pdf $dest/stack_Wplus_met.pdf
cp -a plot_l_pt_5_stack_l_pt_POS.pdf $dest/stack_Wplus_lpt.pdf
cp -a plot_l_eta_5_stack_lepton_etav_POS.pdf $dest/stack_Wplus_eta.pdf
"""

source config.sh

isofail=IsoWind20
isofail=IsoFail20
bgsig=5
bgewk=5
cut="mcw*puw*wzptw*vxw*ls1w*ls2w*znlow*alpy*effw*isow*trigw"
out="STACKNT_ISOFAIL20"

# refline axis range
refline15=" --refline 0.86,1.24"
refline10=" --refline 0.91,1.09"
refline5=" --refline 0.941,1.059"

# Wc/W selection
bgqcd=4
#bgqcd=0

iz=0
for q in 0 1; do
    for bgsig in $bgsig ; do
	for var in "--var l_eta --bin 10,-2.5,2.5 --hsource lepton_etav" "--var l_pt --bin 95,25,120 --hsource l_pt" "--var met --bin 100,0,100 --hsource met" "--var w_mt --bin 80,40,120 --hsource wmt" "--var w_mt --bin 120,0,120 --hsource wmt_extended" "--var w_pt --bin 120,0,120 --hsource wpt"  "--var l_phi --bin 31,-3.14,3.14 --hsource l_phi"; do
	    echo "CHARGE $q BGSIG ${bgsig} : [${var}]"
	    vname=`echo ${var} | awk '{print $2}'`
	    if [ "0" == "Wc/W paper" ]; then
		./stack2.py --input ${input} -b ${var} --charge ${q} -o "${out}Wc" -t plot_${vname}_${bgsig} --pre "fabs(z0)<1.0 && fabs(d0sig)<2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'fabs(z0)<1.0 && fabs(d0sig)<2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'fabs(z0)<1.0 && fabs(d0sig)>2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'fabs(z0)<1.0 && fabs(d0sig)<2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'fabs(z0)<1.0 && fabs(d0sig)>2.5 && njets<=2 && ptiso40/l_pt<0.1 && etiso40corr/l_pt<0.2 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "${cut}" -m one_plot_nt --bgsig ${bgsig} --bgewk ${bgsig} --bgqcd ${bgqcd} ${refline} &> LOG.XCHECK.${iz} &
		((iz++))
	    fi
	    if [ "DISABLE winclusive20" == "winclusive20" ]; then
		./stack2.py --input ${input} -b ${var} --charge ${q} -o "${out}20" -t plot_${vname}_${bgsig} --pre "ptiso40/l_pt<0.1 && met>${met} && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN "ptiso40/l_pt<0.1 && met>${met} && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNQ "ptiso20/l_pt>0.1 && met>${met} && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons<2" --preFN "ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preFQ "ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2" --cut "${cut}" -m one_plot_nt --bgsig ${bgsig} --bgewk ${bgewk} --bgqcd ${bgqcd} ${refline} &> LOG.XCHECK.inc20.${iz} &
		((iz++))
	    fi
	    refline="${refline10}"; if [ "$vname" == "l_eta" ]; then refline="${refline5}"; fi
	    wmt=40; if [ "$vname" == "w_mt" ]; then wmt=0; fi
	    met=25; if [ "$vname" == "met" ]; then met=0; fi
	    if [ "winclusive25" == "winclusive25" ]; then
		./stack2.py --input ${input} -b ${var} --charge ${q} -o "${out}25" -t plot_${vname}_${bgsig} --pre "IsoNominal && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN "IsoNominal && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNQ "IsoWind20 && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons<2" --preFN "IsoNominal && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preFQ "IsoWind20 && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2" --cut "${cut}" -m one_plot_nt --bgsig ${bgsig} --bgewk ${bgewk} --bgqcd ${bgqcd} ${refline} &> LOG.XCHECK.inc25.${iz} &
		((iz++))
	    fi
	    # special plot: met , but w_mt cut relaxed to 20 GeV
	    if [ "$vname" == "met" ]; then
		wmt=20
		met=0
		./stack2.py --input ${input} -b ${var} --charge ${q} -o "${out}25" -t plot_${vname}_wmt20ft40_${bgsig} --pre "IsoNominal && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN "IsoNominal && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNQ "${isofail} && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons<2" --preFN "IsoNominal && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preFQ "${isofail} && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2" --cut "${cut}" -m one_plot_nt --bgsig ${bgsig} --bgewk ${bgewk} --bgqcd ${bgqcd} ${refline} &> LOG.XCHECK.inc25.${iz} &
		((iz++))
		./stack2.py --input ${input} -b ${var} --charge ${q} -o "${out}25" -t plot_${vname}_wmt20ft20_${bgsig} --pre "IsoNominal && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN "IsoNominal && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNQ "${isofail} && met>${met} && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons<2" --preFN "IsoNominal && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preFQ "${isofail} && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>${wmt} && idhits==1 && fabs(z0)<10.0 && nmuons<2" --cut "${cut}" -m one_plot_nt --bgsig ${bgsig} --bgewk ${bgewk} --bgqcd ${bgqcd} ${refline} &> LOG.XCHECK.inc25.${iz} &
		((iz++))
	    fi
	done
    done
done

echo "Waiting for ${iz} jobs"
wait
