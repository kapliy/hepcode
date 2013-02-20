#!/bin/bash

# debugging A-C side asymmetry at various levels:
source config.sh

# compare QCD0 vs QCD4 vs QCD4-BINS (we want to use QCD0 simplification!)
fin=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_anyfit_stacoCB_all/pt20
if [ 0 -eq 1 ]; then
    for q in 0 1 ; do
	./stack2.py -q ${q} -o TEST --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q0 -m acside --bgqcd 0 --qcdscale 1.0 --lnofits
	./stack2.py -q ${q} -o TEST --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q4 -m acside --bgqcd 4 --lnofits
	./stack2.py -q ${q} -o TEST --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q4BINS -m acside --bgqcd 0 --qcdsource "${OUTDB}.eta.1D.pt25.root:qcd_Nominal" --lnofits --nomonly
    done
fi

# Z
fin=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_anyfit_stacoCB_all/pt20
if [ 0 -eq 1 ]; then
    for lq in P N; do
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource lepton${lq}_etav --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -t Z_NOM -m acside --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly
    done
fi

# trying the ntuple version of Z
zpreN='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2'
zpreTP='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lP_trigEF<0.2'
zpreTN='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lN_trigEF<0.2'
zpreTB='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lP_trigEF<0.2 && lN_trigEF<0.2'
zpreCL='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && fabs(lP_eta-lN_eta)<0.2'
if [ 0 -eq 1 ]; then
    for lq in P N; do
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource lepton${lq}_etav --var l${lq}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN}" --pre "${zpreN}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZNT_NOM &
	# require both muons to be close
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource lepton${lq}_etav --var l${lq}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreCL}" --pre "${zpreCL}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZNT_CLOSE &
	# require both muons to be in the same eta bin
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource lepton${lq}_etav --var l${lq}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN} && ZETASAME" --pre "${zpreN} && ZETASAME" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZNT_SAME &
	# trig match (both muons)
	finsp=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_ztrigboth_stacoCB_all/pt20
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource lepton${lq}_etav --var l${lq}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${finsp} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB}" --pre "${zpreTB}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZNT_TMATCHBOTH &
	# trig match + same bin
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource lepton${lq}_etav --var l${lq}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${finsp} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && ZETASAME" --pre "${zpreTB} && ZETASAME" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZNT_SAME_TMATCHBOTH &
    done
    wait
fi

# ntuple version of rawmet
fin=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_rawmet_stacoCB_all/pt20
if [ 0 -eq 1 ]; then
    for q in 0 1; do
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_RAWMET_Q0 &
    done
    wait
fi

# trying the ntuple version of W
fin=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_anyfit_stacoCB_all/pt20
if [ 0 -eq 1 ]; then
    for q in 0 1; do
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOM_Q0 &
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && l_pt>20.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && l_pt>20.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && l_pt>20.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOMETMT_Q0 &
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2' --preNQ 'ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && l_trigEF<0.2' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2 && l_trigEF<0.2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_TRIGMATCH_Q0 &
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_PT25_Q0 &
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met>35.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>50.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>35.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>50.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met>35.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>50.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>50.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>50.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_MET35WMT50_Q0 &  # wmt>50, too
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met_lochad_ichep>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && wmt_lochad_ichep>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met_lochad_ichep>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && wmt_lochad_ichep>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met_lochad_ichep>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && wmt_lochad_ichep>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met_lochad_ichep>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && wmt_lochad_ichep>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met_lochad_ichep>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && wmt_lochad_ichep>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METLH_Q0 &
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*isow*trigw" -t WNT_NORECSF_Q0 &
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow" -t WNT_NOTRIGSF_Q0 &
	./stack2.py -q ${q} -o TEST --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --pre "ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1" --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preNQ 'ptiso20/l_pt>0.1 && met>25.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --preFN 'ptiso40/l_pt<0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --preFQ 'ptiso20/l_pt>0.1 && met>0.0 && l_pt>20.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons<2' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w" -t WNT_NOSF_Q0 &
    done
    wait
fi


function makew() {
    fin=/share/t3data3/antonk/ana/$1/pt20
    tag=$2
    bgsig=$3
    for q in 0 1 ; do
	./stack2.py -q ${q} -o TEST --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t WSYS_${tag}_Q0 -m acside --bgqcd 0 --bgsig ${bgsig} --qcdscale 1.0 --lnofits &
    done
}

# various systematic variations, such as MCP scale
if [ 0 -eq 1 ]; then
    makew ana_v29I_01292013_paper_anyfit_stacoCB_all BGSIG1 1
    makew ana_v29I_01292013_paper_anyfit_stacoCB_all BGSIG4 4
    makew ana_v29I_01292013_paper_rawmet_stacoCB_all RAWMET 5
    makew ana_v29I_01292013_paper_anyfit_d0_stacoCB_all D0 5
    makew ana_v29I_01292013_paper_fineCK_stacoCB_all MCPFINE 5
    makew ana_v29I_01292013_paper_oldC_stacoCB_all MCPOLD 5
    makew ana_v29I_01292013_paper_noC_stacoCB_all MCPNOC 5
    makew ana_v29I_01292013_paper_noCK_stacoCB_all MCPNOCK 5
    makew ana_v29I_01292013_paper_higgsmcp_stacoCB_all MCPHIGGS 5
    wait
fi

# trying the ntuple version of Z
zpreN='lP_pt>20.0 && fabs(lP_eta)<2.4 && lN_pt>20.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2'
if [ 1 -eq 1 ]; then
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource met --var met --bin 200,0,50 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZDB_MET_ALL &
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource met --var met --bin 200,0,50 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN} && lP_eta>1.95 && lP_eta<2.18" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZDB_MET_BADBIN_POS_A &
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource met --var met --bin 200,0,50 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN} && lP_eta<-1.95 && lP_eta>-2.18" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZDB_MET_BADBIN_POS_C &
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource met --var met --bin 200,0,50 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN} && lN_eta>1.95 && lN_eta<2.18" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZDB_MET_BADBIN_NEG_A &
	./stack2.py -q 2 -b -o TEST --ntuple z --hsource met --var met --bin 200,0,50 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN} && lN_eta<-1.95 && lN_eta>-2.18" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZDB_MET_BADBIN_NEG_C &

    wait
fi
