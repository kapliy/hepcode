#!/bin/bash

# debugging A-C side asymmetry in some of the measurement bins.

source config.sh
#input=/share/t3data3/antonk/ana/ana_v29I_02232013_paper_stacoCB_all/pt20
#input=/share/t3data3/antonk/ana/ana_v29I_03012013_paper_stacoCB_all/pt20  # adds per-trigger weights

source _binning.sh
fin=${input}

bgqcd=0

# compare QCD0 vs QCD4 vs QCD4-BINS (we want to use QCD0 simplification!)
if [ 0 -eq 1 ]; then
    for q in 0 1 ; do
	./stack2.py -q ${q} -o ACSIDE --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q0 -m acside --bgqcd 0 --qcdscale 1.0 --lnofits &
	#./stack2.py -q ${q} -o ACSIDE --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q4 -m acside --bgqcd 4 --lnofits &
	#./stack2.py -q ${q} -o ACSIDE --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q4BINS -m acside --bgqcd 0 --qcdsource "${OUTDB}.eta.1D.pt25.root:qcd_Nominal" --lnofits --nomonly &
    done
    wait
fi

# ==========###############==========
# ========== W PRE STRINGS ==========
# ==========###############==========
wpre='ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
wpre2='ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
wpre3='ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
wpre4='ptiso40/l_pt<0.1 && l_pt>15.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
# removing cuts
wnoiso='met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
wnonmu='ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0'
wnocuts='fabs(l_eta)<2.4 && idhits==1'
wpreNOMETMT='ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
# other
wpre35='ptiso40/l_pt<0.1 && met>25.0 && l_pt>35.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
wpre2535='ptiso40/l_pt<0.1 && met>25.0 && l_pt>25 && l_pt<35.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
wpreidNOMETMT='ptiso40/l_pt_id<0.1 && l_pt_id>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
wpreIDMS='ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1 && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'
wzlike='ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==2'
# ==========###############==========
# ========== Z PRE STRINGS ==========
# ==========###############==========
# zs
zpreN='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2'
zpreTB='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lP_trigEF<0.2 && lN_trigEF<0.2'
zpre="${zpreTB}"
zprenowind='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lP_trigEF<0.2 && lN_trigEF<0.2'
zpre35='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>35.0 && fabs(lP_eta)<2.4 && lN_pt>35.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lP_trigEF<0.2 && lN_trigEF<0.2'
zpre2535='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lQ_pt>25.0 && lQ_pt<35.0 && fabs(lP_eta)<2.4 && lO_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lP_trigEF<0.2 && lN_trigEF<0.2'
zpreIDMS='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lP_trigEF<0.2 && lN_trigEF<0.2   &&   fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5 && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5'
# only require probe to trigger
zpre2='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lQ_trigEF<0.2'
zpre2inv='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lQ_trigEF<0.2 && lO_trigEF>0.2'
zpre2t='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lQ_trigEF<0.05' # tight trig match
zpre2noq='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && nmuons==2 && lQ_trigEF<0.2' # no q

# Z ntuple
if [ 0 -eq 1 ]; then
    for q in P N; do
	qo=P; if [ "${q}" == "P" ]; then qo=N; fi;
	# nominal
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZNT_NOM &
	# trig match (both muons)
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TMATCHBOTH &
	# trig match (both muons) + njets>=1 (to decouple Z phi correlations)
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && njets>=1" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TMATCHJETS &
        # trig match (both muons) + other muon forced in barrel (to decorrelate)
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && fabs(l${qo}_eta)<1.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TMATCHOBARREL &
	# trig match + same bin
	#./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && ZETASAME" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_SAME_TMATCHBOTH &
	# inside phi quadrants
	if [ 1 -eq 1 ]; then
	    ./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>0 && l${q}_phi<3.14/2.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TB_C0 &
	    ./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>3.14/2.0 && l${q}_phi<3.14" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TB_C1 &
	    ./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>-3.14 && l${q}_phi<-3.14/2.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TB_C2 &
	    ./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>-3.14/2.0 && l${q}_phi<0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TB_C3 &
	fi
    done
    wait
fi

# A/C plot for W ntuple before period I
if [ 0 -eq 1 ]; then
    for q in 0 1; do
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -d D,E,F,G,H --lumi 1257000 -t WI &
    done
    wait
fi

# W ntuple
if [ 0 -eq 1 ]; then
    for q in 0 1; do
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOM_Q0 &
	#./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso20/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_ISO20_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOISO_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOMETMT_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_raw>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_raw>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METRAW_Q0 &
	#./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_ichep>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_ichep>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METICHEP_Q0 &
	#./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_ichep_corr>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_ichep_corr>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METICHEPCORR_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_locmuon>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_locmuon>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METLOCMUON_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_locmuon_corr>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_locmuon_corr>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METLOCMUONCORR_Q0 &
	wait
    done
fi


function makewnt() {
    tmpfin=/share/t3data3/antonk/ana/$1/pt20
    tag=$2
    bgsig=$3
    for q in 0 1 ; do
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${tmpfin}  -m acside_nt --bgqcd 0 --bgsig ${bgsig} --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WSYSNT_${tag}_Q0 &
	if [ "${bgsig}" == "2" ]; then
	    ./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${tmpfin}  -m acside_nt --bgqcd 0 --bgsig ${bgsig} --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*vxw*ls1w*ls2w*effw*isow*trigw" -t WSYSNT_${tag}_ALPNW_Q0 &
	fi
    done
}
function makew() {
    tmpfin=/share/t3data3/antonk/ana/$1/pt20
    tag=$2
    bgsig=$3
    for q in 0 1 ; do
	./stack2.py -q ${q} -o ACSIDE --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${tmpfin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t WSYS_${tag}_Q0 -m acside --bgqcd 0 --bgsig ${bgsig} --qcdscale 1.0 --lnofits &
    done
}
# W: different generators
if [ 0 -eq 1 ]; then
    makew ana_v29I_02222013_paper_stacoCB_all BGSIG1 1
    makew ana_v29I_02222013_paper_stacoCB_all BGSIG2 2
    makew ana_v29I_02222013_paper_stacoCB_all BGSIG4 4
    makew ana_v29I_02222013_paper_stacoCB_all BGSIG5 5
    wait
fi
# W: various systematic variations, such as MCP scale
if [ 0 -eq 1 ]; then
    makew ana_v29I_01292013_paper_rawmet_stacoCB_all RAWMET 5
    makew ana_v29I_01292013_paper_anyfit_d0_stacoCB_all D0 5
    makew ana_v29I_01292013_paper_fineCK_stacoCB_all MCPFINE 5
    makew ana_v29I_01292013_paper_oldC_stacoCB_all MCPOLD 5
    makew ana_v29I_01292013_paper_noC_stacoCB_all MCPNOC 5
    makew ana_v29I_01292013_paper_noCK_stacoCB_all MCPNOCK 5
    makew ana_v29I_01292013_paper_higgsmcp_stacoCB_all MCPHIGGS 5
    wait
fi

# Z ntuple: plotting MET-related distributions in each eta bin
if [ 0 -eq 1 ]; then
    # inclusive
    #./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource met --var met --bin 50,0,50 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZDB_NOM &
    for q in P N; do
        # eta/phi (no trigger reqs)
	./stack2.py --norm -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_eta --var l${q}_eta --bin 40,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZDB_NOM &
	./stack2.py --rebin 1 --norm -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_phi --var l${q}_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZDB_NOM &
        # eta/phi (trigger)
	./stack2.py --norm -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_eta --var l${q}_eta --bin 40,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_TB &
	./stack2.py --rebin 1 --norm -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_phi --var l${q}_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_TB &
        # eta (trigger, in phi quadrants)
	./stack2.py --norm -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>0 && l${q}_phi<3.14/2.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_TB_C0 &
	./stack2.py --norm -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>3.14/2.0 && l${q}_phi<3.14" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_TB_C1 &
	./stack2.py --norm -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>-3.14 && l${q}_phi<-3.14/2.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_TB_C2 &
	./stack2.py --norm -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>-3.14/2.0 && l${q}_phi<0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_TB_C3 &
    done
    wait
fi

# W ntuple: studying MET and MET corrections, plus plots in each eta bin
if [ 0 -eq 1 ]; then
    BIN10C="&& l_eta<-1.95 && l_eta>-2.18"
    BIN10A="&& l_eta>1.95 && l_eta<2.18"
    for q in 0 1; do
	./stack2.py --rebin 1 --norm -q ${q} -o ACSIDE --hsource l_phi --var l_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOM &
	./stack2.py --rebin 1--norm -q ${q} -o ACSIDE --hsource l_phi --var l_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOM &
	./stack2.py --norm -q ${q} -o ACSIDE --hsource l_eta --var l_eta --bin 40,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOM &
	./stack2.py --norm -q ${q} -o ACSIDE --hsource l_eta --var l_eta --bin 40,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOM &
	# eta plots without met or wmt
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN 'ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOMETMT_Q0 &
	# global phi, corrected with trigger phi dependence
	./stack2.py --rebin 1 --norm -q ${q} -o ACSIDE --hsource l_phi --var l_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw*trigphiw" -t WDB_TRIGPHI &
	./stack2.py --rebin 1 --norm -q ${q} -o ACSIDE --hsource l_phi --var l_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw*trigphiw" -t WDB_TRIGPHI &
	# phi quadrants [A/C]
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN "${wpre} && l_phi>0 && l_phi<3.14/2.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOM_C0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN "${wpre} && l_phi>3.14/2.0 && l_phi<3.14" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOM_C1 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN "${wpre} && l_phi>-3.14 && l_phi<-3.14/2.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOM_C2 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN "${wpre} && l_phi>-3.14/2.0 && l_phi<0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOM_C3 &
	# met
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met --var met --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met --var met --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	# met (low pt)
	#./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met --var met --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre4} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A_LOWPT &
	#./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met --var met --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre4} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C_LOWPT &
	# met-lochad
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met_lochad --var met_lochad --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met_lochad --var met_lochad --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	# met-muonboy (uncorrected)
	#./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met_muonboy --var met_muonboy --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	#./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met_muonboy --var met_muonboy --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	# difference between MET and muon
	#./stack2.py -q ${q} -o ACSIDE --hsource delta_muonboy_muon --var dmet_muonboy_muon --bin 40,0,2 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	#./stack2.py -q ${q} -o ACSIDE --hsource delta_muonboy_muon --var dmet_muonboy_muon --bin 40,0,2 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	wait
    done
    wait
fi


# makes a few plots for W and Z in the "bad" bin.
function badbinW() {
    local IBIN BPRE cvar side q thebin BSTR TAG rvars
    # 0.75,1.275
    IBIN="$1"
    BPRE="$2"
    TAG="$3"
    XTRA="$4"
    rvars="eta"  #"phi eta"
    for cvar in ${rvars}; do
	for side in A C; do
	    thebin="40,-3.15,3.15"
	    if [ "${cvar}" == "eta" ]; then thebin=`V_getbins ${IBIN} ${side}`; fi
	    for q in 0 1 ; do
		BSTR=`W_getpre ${IBIN} ${side}`
		./stack2.py ${XTRA} --rebin 1 -q ${q} -o ACSIDE --hsource l_${cvar} --var l_${cvar} --bin ${thebin} --refline 0.65,1.375 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${BPRE} ${BSTR}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t W${TAG}_${IBIN}_${side} &> logs/LOG.badbinW${TAG}.${IBIN}.${cvar}.${side}.${q} &
	    done
	done
    done
}
function badbinZ() {
    local IBIN BPRE cvar side q thebin BSTR TAG
    IBIN="$1"
    BPRE="$2"
    TAG="$3"
    XTRA=""
    rvars="eta"  #"phi eta"
    for cvar in ${rvars}; do
	for side in A C; do
	    thebin="40,-3.15,3.15"
	    if [ "${cvar}" == "eta" ]; then thebin=`V_getbins ${IBIN} ${side}`; fi
	    for q in P N ; do
		qo=P; if [ "${q}" == "P" ]; then qo="N"; else qo="P"; fi;
		BSTR=`Z_getpre ${IBIN} ${q} ${side}`
		FPRE=`echo ${BPRE} | sed -e "s#lO#l${qo}#g" -e "s#lQ#l${q}#g"`
		./stack2.py ${XTRA} --rebin 1 -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_${cvar} --var l${q}_${cvar} --bin ${thebin} --refline 0.65,1.375 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${FPRE} ${BSTR}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t Z${TAG}_${IBIN}_${side} &> logs/LOG.badbinZ${TAG}.${IBIN}.${cvar}.${side}.${q} &
	    done
	done
    done
}
function badbinZ2() { # using per-muon trigger weights
    local IBIN BPRE cvar side q thebin BSTR TAG
    IBIN="$1"
    BPRE="$2"
    TAG="$3"
    XTRA=""
    for cvar in phi eta; do
	for side in A C; do
	    thebin="40,-3.15,3.15"
	    if [ "${cvar}" == "eta" ]; then thebin=`V_getbins ${IBIN} ${side}`; fi
	    for q in P N ; do
		qo=P; if [ "${q}" == "P" ]; then qo="N"; else qo="P"; fi;
		BSTR=`Z_getpre ${IBIN} ${q} ${side}`
		FPRE=`echo ${BPRE} | sed -e "s#lO#l${qo}#g" -e "s#lQ#l${q}#g"`
		trigw=`echo lQ_trigw | sed -e "s#lO#l${qo}#g" -e "s#lQ#l${q}#g"`
		./stack2.py ${XTRA} --rebin 1 -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_${cvar} --var l${q}_${cvar} --bin ${thebin} --refline 0.65,1.375 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${FPRE} ${BSTR}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*${trigw}" -t Z${TAG}_${IBIN}_${side} &> logs/LOG.badbinZ${TAG}.${IBIN}.${cvar}.${side}.${q} &
	    done
	done
    done
}

if [ 0 -eq 1 ]; then
    # bad bins: 10,9,8, 4

    # njets>=1 and nometmt in bin 10
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wpre} && njets>=1" "njets"
	badbinW 10 "${wpreNOMETMT}" "nometmt"
	echo "w 10 with njets>=1 and nomet"; wait
	badbinZ 10 "${zpre} && njets>=1" "njets"
	badbinW 9 "${wpreNOMETMT}" "nometmt"
	echo "10,9 with njets>=1 and nomet"; wait
        # nometmt in other bad bins
	badbinW 8 "${wpreNOMETMT}" "nometmt"
	badbinW 4 "${wpreNOMETMT}" "nometmt"
	echo "9,8,4 and nometmt"; wait
        # nometmt + ptid
	badbinW 10 "${wpreidNOMETMT}" "nometmtid"
	badbinW 9 "${wpreidNOMETMT}" "nometmtid"
	echo "10,9 and nometmt + ptid"; wait
	badbinW 8 "${wpreidNOMETMT}" "nometmtid"
	badbinW 4 "${wpreidNOMETMT}" "nometmtid"
	echo "8,4 and nometmt + ptid"; wait
    fi

    # ID-MS quality requirement
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wpreIDMS}" "idms"
	badbinZ 10 "${zpreIDMS}" "idms"
	echo "10 with IDMS cut"; wait
	badbinW 9 "${wpreIDMS}" "idms"
	badbinZ 9 "${zpreIDMS}" "idms"
	echo "9 with IDMS cut"; wait
	badbinW 8 "${wpreIDMS}" "idms"
	badbinZ 8 "${zpreIDMS}" "idms"
	echo "8 with IDMS cut"; wait
	badbinW 4 "${wpreIDMS}" "idms"
	badbinZ 4 "${zpreIDMS}" "idms"
	echo "4 with IDMS cut"; wait
    fi

    # noiso
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wnoiso}" "noiso"
	badbinW 9 "${wnoiso}" "noiso"
	echo "10,9 with noiso cut"; wait
	badbinW 8 "${wnoiso}" "noiso"
	badbinW 4 "${wnoiso}" "noiso"
	echo "8,4 with noiso cut"; wait
    fi
    # nonmu
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wnonmu}" "nonmu"
	badbinW 9 "${wnonmu}" "nonmu"
	echo "10,9 with nonmu cut"; wait
	badbinW 8 "${wnonmu}" "nonmu"
	badbinW 4 "${wnonmu}" "nonmu"
	echo "8,4 with nonmu cut"; wait
    fi
    # nocuts
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wnocuts}" "nocuts"
	badbinW 9 "${wnocuts}" "nocuts"
	echo "10,9 with nocuts cut"; wait
	badbinW 8 "${wnocuts}" "nocuts"
	badbinW 4 "${wnocuts}" "nocuts"
	echo "8,4 with nocuts cut"; wait
    fi
    
    # pt>35 in bin 10,9,8,4
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wpre35}" "pt35"
	badbinZ 10 "${zpre35}" "pt35"
	echo "10 with pt=35 cut"; wait
	badbinW 9 "${wpre35}" "pt35"
	badbinZ 9 "${zpre35}" "pt35";
	echo "9 with pt=35 cut"; wait
	badbinW 8 "${wpre35}" "pt35"
	badbinZ 8 "${zpre35}" "pt35"
	echo "8 with pt=35 cut"; wait
	badbinW 4 "${wpre35}" "pt35"
	badbinZ 4 "${zpre35}" "pt35"
	echo "4 with pt=35 cut"; wait
    fi

    # 25<pt<35 in bin 10,9,8,4
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wpre2535}" "pt2535"
	badbinZ 10 "${zpre2535}" "pt2535"
	echo "10 with pt=2535 cut"; wait
	badbinW 9 "${wpre2535}" "pt2535"
	badbinZ 9 "${zpre2535}" "pt2535";
	echo "9 with pt=2535 cut"; wait
	badbinW 8 "${wpre2535}" "pt2535"
	badbinZ 8 "${zpre2535}" "pt2535"
	echo "8 with pt=2535 cut"; wait
	badbinW 4 "${wpre2535}" "pt2535"
	badbinZ 4 "${zpre2535}" "pt2535"
	echo "4 with pt=2535 cut"; wait
    fi
    
    # additional Z variations
    if [ 1 -eq 1 ]; then
	badbinZ 10 "${zpre} && fabs(lO_eta)<1.0" "lObarrel"
	badbinZ 10 "${zpre} && fabs(lO_eta)>1.0" "lOendcap"
	echo "10 with lO in barrel/endcap"; wait
        # no Z window cut
	badbinZ 10 "${zprenowind}" "nowind"
	badbinZ 9 "${zprenowind}" "nowind"
	echo "10,9 with nowind"; wait
	badbinZ 8 "${zprenowind}" "nowind"
	badbinZ 4 "${zprenowind}" "nowind"
	echo "8,4 with nowind"; wait
    fi

    # asking Z probe muon to be in a particular phi region
    if [ 1 -eq 1 ]; then
	badbinZ 10 "${zpre} && lQ_phi>0 && lQ_phi<3.14/2.0" "lQ1"
	badbinZ 10 "${zpre} && lQ_phi>3.14/2.0 && lQ_phi<3.14" "lQ2"
	echo "Z Q1 and Q2"; wait
	badbinZ 10 "${zpre} && lQ_phi>-3.14 && lQ_phi<-3.14/2.0" "lQ3"
	badbinZ 10 "${zpre} && lQ_phi>-3.14/2.0 && lQ_phi<0" "lQ4"
	echo "Z Q3 and Q4"; wait
    fi
    # asking W muon to be in a particular phi region
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wpre} && l_phi>0 && l_phi<3.14/2.0" "lQ1" "--norm"
	badbinW 10 "${wpre} && l_phi>3.14/2.0 && l_phi<3.14" "lQ2" "--norm"
	echo "W Q1 and Q2"; wait
	badbinW 10 "${wpre} && l_phi>-3.14 && l_phi<-3.14/2.0" "lQ3" "--norm"
	badbinW 10 "${wpre} && l_phi>-3.14/2.0 && l_phi<0" "lQ4" "--norm"
	echo "W Q3 and Q4"; wait
    fi
    if [ 1 -eq 1 ]; then
	badbinW 8 "${wpre} && l_phi>0 && l_phi<3.14/2.0" "lQ1" "--norm"
	badbinW 8 "${wpre} && l_phi>3.14/2.0 && l_phi<3.14" "lQ2" "--norm"
	echo "W Q1 and Q2"; wait
	badbinW 8 "${wpre} && l_phi>-3.14 && l_phi<-3.14/2.0" "lQ3" "--norm"
	badbinW 8 "${wpre} && l_phi>-3.14/2.0 && l_phi<0" "lQ4" "--norm"
	echo "W Q3 and Q4"; wait
    fi
    if [ 1 -eq 1 ]; then
	badbinW 9 "${wpre} && l_phi>0 && l_phi<3.14/2.0" "lQ1" "--norm"
	badbinW 9 "${wpre} && l_phi>3.14/2.0 && l_phi<3.14" "lQ2" "--norm"
	echo "W Q1 and Q2"; wait
	badbinW 9 "${wpre} && l_phi>-3.14 && l_phi<-3.14/2.0" "lQ3" "--norm"
	badbinW 9 "${wpre} && l_phi>-3.14/2.0 && l_phi<0" "lQ4" "--norm"
	echo "W Q3 and Q4"; wait
    fi
    if [ 1 -eq 1 ]; then
	badbinW 4 "${wpre} && l_phi>0 && l_phi<3.14/2.0" "lQ1" "--norm"
	badbinW 4 "${wpre} && l_phi>3.14/2.0 && l_phi<3.14" "lQ2" "--norm"
	echo "W Q1 and Q2"; wait
	badbinW 4 "${wpre} && l_phi>-3.14 && l_phi<-3.14/2.0" "lQ3" "--norm"
	badbinW 4 "${wpre} && l_phi>-3.14/2.0 && l_phi<0" "lQ4" "--norm"
	echo "W Q3 and Q4"; wait
    fi

    # trying out W on a subset of data periods
    if [ 1 -eq 1 ]; then
	badbinW 10 "${wpre}" "pDtoH" "-d D,E,F,G,H --norm"  #--lumi 1257000 
	badbinW 10 "${wpre}" "pItoK" "-d I,J,K --norm"  #--lumi 1257000 
	echo "10 W D-K"; wait
	badbinW 10 "${wpre}" "pItoI" "-d I --norm"  #--lumi 1257000 
	badbinW 10 "${wpre}" "pKtoK" "-d K --norm"  #--lumi 1257000
	echo "10 W I-I and K-K"; wait
	badbinW 10 "${wpre}" "pLtoL" "-d L --norm"  #--lumi 1257000 
	badbinW 10 "${wpre}" "pMtoM" "-d M --norm"  #--lumi 1257000
	echo "10 W L-M"; wait
    fi
    if [ 1 -eq 1 ]; then
	badbinW 8 "${wpre}" "pDtoH" "-d D,E,F,G,H --norm"  #--lumi 1257000 
	badbinW 8 "${wpre}" "pItoK" "-d I,J,K --norm"  #--lumi 1257000 
	echo "8 W D-K"; wait
	badbinW 8 "${wpre}" "pItoI" "-d I --norm"  #--lumi 1257000 
	badbinW 8 "${wpre}" "pKtoK" "-d K --norm"  #--lumi 1257000
	echo "8 W I-I and K-K"; wait
	badbinW 8 "${wpre}" "pLtoL" "-d L --norm"  #--lumi 1257000 
	badbinW 8 "${wpre}" "pMtoM" "-d M --norm"  #--lumi 1257000
	echo "8 W L-M"; wait
    fi
    if [ 1 -eq 1 ]; then
	badbinW 9 "${wpre}" "pDtoH" "-d D,E,F,G,H --norm"  #--lumi 1257000 
	badbinW 9 "${wpre}" "pItoK" "-d I,J,K --norm"  #--lumi 1257000 
	echo "9 W D-K"; wait
	badbinW 9 "${wpre}" "pItoI" "-d I --norm"  #--lumi 1257000 
	badbinW 9 "${wpre}" "pKtoK" "-d K --norm"  #--lumi 1257000
	echo "9 W I-I and K-K"; wait
	badbinW 9 "${wpre}" "pLtoL" "-d L --norm"  #--lumi 1257000 
	badbinW 9 "${wpre}" "pMtoM" "-d M --norm"  #--lumi 1257000
	echo "9 W L-M"; wait
    fi
    if [ 1 -eq 1 ]; then
	badbinW 4 "${wpre}" "pDtoH" "-d D,E,F,G,H --norm"  #--lumi 1257000 
	badbinW 4 "${wpre}" "pItoK" "-d I,J,K --norm"  #--lumi 1257000 
	echo "4 W D-K"; wait
	badbinW 4 "${wpre}" "pItoI" "-d I --norm"  #--lumi 1257000 
	badbinW 4 "${wpre}" "pKtoK" "-d K --norm"  #--lumi 1257000
	echo "4 W I-I and K-K"; wait
	badbinW 4 "${wpre}" "pLtoL" "-d L --norm"  #--lumi 1257000 
	badbinW 4 "${wpre}" "pMtoM" "-d M --norm"  #--lumi 1257000
	echo "4 W L-M"; wait
    fi

    # Making W Z-like
    if [ 1 -eq 1 ]; then
	badbinZ 10 "${zpre}"
	badbinZ2 10 "${zpre2}" "tprobe"
	badbinZ2 10 "${zpre2t}" "tprobet"
	echo "10 z tag not triggered"; wait
	badbinZ2 10 "${zpre2noq}" "tprobenoq"
	echo "10 no q, tag not triggered"; wait
	badbinW 10 "${wpre}"
	badbinW 10 "${wzlike}" "zlike"
	echo "10 w z-like"; wait
    fi

    # Z events with trigger inversion (e.g., other leg in barrel crack)
    if [ 1 -eq 1 ]; then
	badbinZ2 10 "${zpre2inv}" "tinv"
	badbinZ2 9 "${zpre2inv}" "tinv"
	echo "10,9 trigger-elsewhere"; wait
	badbinZ2 8 "${zpre2inv}" "tinv"
	badbinZ2 4 "${zpre2inv}" "tinv"
	echo "8,4 trigger-elsewhere"; wait
    fi
    

    # RUN W
    if [ 1 -eq 1 ]; then
	badbinW 1 "${wpre}"
	badbinW 2 "${wpre}"
	echo "1 2"; wait;
	badbinW 3 "${wpre}"
	badbinW 4 "${wpre}"
	echo "3 4"; wait;
	badbinW 5 "${wpre}"
	badbinW 6 "${wpre}"
	echo "5 6"; wait;
	badbinW 7 "${wpre}"
	badbinW 8 "${wpre}"
	echo "7 8"; wait;
	badbinW 9 "${wpre}"
	badbinW 10 "${wpre}"
	badbinW 11 "${wpre}"
	echo "9 10 11"; wait;
    fi

    # RUN Z
    if [ 1 -eq 1 ]; then
	badbinZ 1 "${zpre}"
	badbinZ 2 "${zpre}"
	echo "1 2"; wait;
	badbinZ 3 "${zpre}"
	badbinZ 4 "${zpre}"
	echo "3 4"; wait;
	badbinZ 5 "${zpre}"
	badbinZ 6 "${zpre}"
	echo "5 6"; wait;
	badbinZ 7 "${zpre}"
	badbinZ 8 "${zpre}"
	echo "7 8"; wait;
	badbinZ 9 "${zpre}"
	badbinZ 10 "${zpre}"
	badbinZ 11 "${zpre}"
	echo "9 10 11"; wait;
    fi

    echo "ALL DONE"
fi

# 03/20/2013 studies: pileup dependence
if [ 0 -eq 1 ]; then
    # bad bins: 10,9,8, 4
    fin=/share/t3data3/antonk/ana/ana_v29J_03172013_paper_stacoCB_all/pt20 # trigger matching fixed; fixed bad bin

    # plot problematic eta bins splitting in different pile-up conditions, like different <mu> intervals
    badbinW 10 "${wpre} && avgmu<=6" "avgmu0_6"
    badbinW 8 "${wpre} && avgmu<=6" "avgmu0_6"
    wait
    badbinZ 10 "${zpre} && avgmu<=6" "avgmu0_6"
    badbinZ 8 "${zpre} && avgmu<=6" "avgmu0_6"
    echo "avgmu 0_6"
    wait

    badbinW 10 "${wpre} && avgmu>6 && avgmu<=10" "avgmu7_10"
    badbinW 8 "${wpre} && avgmu>6 && avgmu<=10" "avgmu7_10"
    wait
    badbinZ 10 "${zpre} && avgmu>6 && avgmu<=10" "avgmu7_10"
    badbinZ 8 "${zpre} && avgmu>6 && avgmu<=10" "avgmu7_10"
    echo "avgmu 0_6"
    wait

    badbinW 10 "${wpre} && avgmu>10" "avgmu11"
    badbinW 8 "${wpre} && avgmu>10" "avgmu11"
    wait
    badbinZ 10 "${zpre} && avgmu>10" "avgmu11"
    badbinZ 8 "${zpre} && avgmu>10" "avgmu11"
    echo "avgmu 0_6"
    wait
fi
# 03/20/2013 studies: mu18 trigger
if [ 0 -eq 1 ]; then

    fin=/share/t3data3/antonk/ana/ana_v29J_03172013_paper_stacoCB_all/pt20 # trigger matching fixed; fixed bad bin
    badbinW 10 "${wpre}" "MU18_MG"
    badbinW 8 "${wpre}" "MU18_MG"
    wait
    badbinZ 10 "${zpre}" "MU18_MG"
    badbinZ 8 "${zpre}" "MU18_MG"
    wait

    fin=/share/t3data3/antonk/ana/ana_v29J_03172013_paper_mu18_stacoCB_all/pt20
    badbinW 10 "${wpre}" "MU18"
    badbinW 8 "${wpre}" "MU18"
    wait
    badbinZ 10 "${zpre}" "MU18"
    badbinZ 8 "${zpre}" "MU18"
    wait

fi
