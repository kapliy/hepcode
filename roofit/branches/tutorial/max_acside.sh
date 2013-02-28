#!/bin/bash

# debugging A-C side asymmetry at various levels:
source config.sh
fin=${input}
fintr=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_ztrigboth_stacoCB_all/pt20
finraw=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_rawmet_stacoCB_all/pt20

bgqcd=0

# compare QCD0 vs QCD4 vs QCD4-BINS (we want to use QCD0 simplification!)
if [ 1 -eq 1 ]; then
    for q in 0 1 ; do
	./stack2.py -q ${q} -o ACSIDE --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q0 -m acside --bgqcd 0 --qcdscale 1.0 --lnofits &
	./stack2.py -q ${q} -o ACSIDE --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q4 -m acside --bgqcd 4 --lnofits &
	./stack2.py -q ${q} -o ACSIDE --hsource d3_eta_lpt_met:y:2:-1:z:0:-1 --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} --lvar d3_abseta_lpt_met:x:0:-1:y:2:-1 --lbin 100,0,60 --lrebin 2 -t W_NOM_Q4BINS -m acside --bgqcd 0 --qcdsource "${OUTDB}.eta.1D.pt25.root:qcd_Nominal" --lnofits --nomonly &
    done
    wait
fi

# Z ntuple
zpreN='lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2'
zpreTB='lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2 && lP_trigEF<0.2 && lN_trigEF<0.2'
if [ 1 -eq 1 ]; then
    for q in P N; do
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZNT_NOM &
	# require both muons to be in the same eta bin
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreN} && ZETASAME" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t ZNT_SAME &
	# trig match (both muons)
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TMATCHBOTH &
	# trig match (both muons) + njets>=1 (to decouple Z phi correlations)
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && njets>=1" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TMATCHJETS &
	# trig match + same bin
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && ZETASAME" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_SAME_TMATCHBOTH &
	# inside phi quadrants
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>0 && l${q}_phi<3.14/2.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TB_C0 &
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>3.14/2.0 && l${q}_phi<3.14" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TB_C1 &
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>-3.14 && l${q}_phi<-3.14/2.0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TB_C2 &
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource lepton${q}_etav --var l${q}_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin} -m acside_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_phi>-3.14/2.0 && l${q}_phi<0" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZNT_TB_C3 &
    done
    wait
fi

# W ntuple
if [ 1 -eq 1 ]; then
    for q in 0 1; do
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOM_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_NOMETMT_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_raw>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_raw>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METRAW_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_ichep>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_ichep>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METICHEP_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_ichep_corr>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_ichep_corr>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METICHEPCORR_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_locmuon>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_locmuon>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METLOCMUON_Q0 &
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m acside_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && met_locmuon_corr>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && wmt_locmuon_corr>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WNT_METLOCMUONCORR_Q0 &
    done
    wait
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
if [ 1 -eq 1 ]; then
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
        # phi (trigger) (un-normalized, in bad eta bin)
	./stack2.py --rebin 1 -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_phi --var l${q}_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_eta>1.95 && l${q}_eta<2.18" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_A &
	./stack2.py --rebin 1 -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_phi --var l${q}_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_eta<-1.95 && l${q}_eta>-2.18" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_C &
        # eta (trigger) (un-normalized, in bad eta bin)
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_eta --var l${q}_eta --bin 40,1.95,2.18 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_eta>1.95 && l${q}_eta<2.18" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_A &
	./stack2.py -q 2 -b -o ACSIDE --ntuple z --hsource l${q}_eta --var l${q}_eta --bin 40,-2.18,-1.95 --refline 0.85,1.175 -b --input ${fin} -m one_plot_nt --qcdscale 1.0 --bgqcd 0 --lnofits --nomonly --preNN "${zpreTB} && l${q}_eta<-1.95 && l${q}_eta>-2.18" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw" -t ZDB_C &
    done
    wait
fi

# W ntuple: studying MET and MET corrections, plus plots in each eta bin
if [ 1 -eq 1 ]; then
    BIN10C="&& l_eta<-1.95 && l_eta>-2.18"
    BIN10A="&& l_eta>1.95 && l_eta<2.18"
    for q in 0 1; do
	wpre='ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
	wpre2='ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
	wpre3='ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
	wpre4='ptiso40/l_pt<0.1 && l_pt>15.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
	./stack2.py --rebin 1 --norm -q ${q} -o ACSIDE --hsource l_phi --var l_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOM &
	./stack2.py --rebin 1--norm -q ${q} -o ACSIDE --hsource l_phi --var l_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOM &
	./stack2.py --norm -q ${q} -o ACSIDE --hsource l_eta --var l_eta --bin 40,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOM &
	./stack2.py --norm -q ${q} -o ACSIDE --hsource l_eta --var l_eta --bin 40,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOM &
	# eta plots without met or wmt
	./stack2.py -q ${q} -o ACSIDE --hsource lepton_etav --var l_eta --bin 10,-2.5,2.5 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --preNN 'ptiso40/l_pt<0.1 && l_pt>25.0 && fabs(l_eta)<2.4 && idhits==1 && fabs(z0)<10.0 && nmuons==1' --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_NOMETMT_Q0 &
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
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met --var met --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre4} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A_LOWPT &
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met --var met --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre4} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C_LOWPT &
	# met-lochad
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met_lochad --var met_lochad --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met_lochad --var met_lochad --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	# met-muonboy (uncorrected)
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met_muonboy --var met_muonboy --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	./stack2.py --rebin 4 -q ${q} -o ACSIDE --hsource met_muonboy --var met_muonboy --bin 100,0,100 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	# difference between MET and muon
	./stack2.py -q ${q} -o ACSIDE --hsource delta_muonboy_muon --var dmet_muonboy_muon --bin 40,0,2 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	./stack2.py -q ${q} -o ACSIDE --hsource delta_muonboy_muon --var dmet_muonboy_muon --bin 40,0,2 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre3} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	# muon phi
	./stack2.py --rebin 1 -q ${q} -o ACSIDE --hsource l_phi --var l_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	./stack2.py --rebin 1 -q ${q} -o ACSIDE --hsource l_phi --var l_phi --bin 40,-3.15,3.15 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	# muon eta
	./stack2.py -q ${q} -o ACSIDE --hsource l_eta --var l_eta --bin 40,1.95,2.18 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre} ${BIN10A}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_A &
	./stack2.py -q ${q} -o ACSIDE --hsource l_eta --var l_eta --bin 40,-2.18,-1.95 --refline 0.85,1.175 -b --input ${fin}  -m one_plot_nt --bgqcd 0 --qcdscale 1.0 --lnofits --nomonly --preNN "${wpre} ${BIN10C}" --cut "mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigw" -t WDB_C &
	wait
    done
    wait
fi
