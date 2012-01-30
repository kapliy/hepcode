#!/bin/bash
source bashmap.sh

#note: saw better agreement with d0sig<3.0 AND fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5

wpre_preiso='l_pt>20.0 && fabs(l_eta)<2.4 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5' #old
wpre_preiso='l_pt>20.0 && fabs(l_eta)<2.4 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && fabs(d0sig)<10.0' #WW/WZ (d0sig)<3.0
wpre_jordan="${wpre_preiso} && ptiso20/l_pt<0.1"
wpre_peter="${wpre_preiso} && ptiso30<1.125 && etiso30<1.125" #old
wpre_peter="${wpre_preiso} && ptiso30/l_pt<0.15 && etiso30corr/l_pt<0.14" #WW/WZ
zpre_preiso='lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && fabs(lP_eta)<2.4 && fabs(lP_pt_id-lP_pt_exms)/lP_pt_id<0.5    &&     lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0 && fabs(lN_eta)<2.4 && fabs(lN_pt_id-lN_pt_exms)/lN_pt_id<0.5    &&     Z_m>70 && Z_m<110 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0' #old
zpre_preiso='lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && fabs(lP_eta)<2.4 && fabs(lP_d0sig)<10.0    &&    lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0 && fabs(lN_eta)<2.4 && fabs(lN_d0sig)<10.0    &&     Z_m>70 && Z_m<110 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0' #WW/WZ (d0sig<3.0)
zpre_jordan="${zpre_preiso} && lP_ptiso20/lP_pt<0.1 && lN_ptiso20/lN_pt<0.1"
zpre_peter="${zpre_preiso} && lP_ptiso30<1.125 && lP_etiso30<1.125 && lN_ptiso30<1.125 && lN_etiso30<1.125" #old
zpre_peter="${zpre_preiso} && lP_ptiso30/lP_pt<0.15 && lP_etiso30corr/lP_pt<0.14 && lN_ptiso30/lN_pt<0.15 && lN_etiso30corr/lN_pt<0.14" #WW/WZ

# MC11B
commonB="--input /share/ftkdata1/antonk/ana_v28HB_01212012_DtoM_jetupd_stacoCB_all/"
# MC11C
commonC="--input /share/ftkdata1/antonk/ana_v28HC_01212012_DtoM_jetupd_stacoCB_all/"
common="${commonC}"

###############################################################################
# Parse command line
###############################################################################
mode=none
while getopts ":i:o:m:" optionslist; do
    case $optionslist in
	i ) common="--input $OPTARG"
	    echo "Input folder: $OPTARG";;
	o ) common="${common} -o $OPTARG"
	    echo "Output folder: $OPTARG";;
	m ) mode=$OPTARG
	    echo "Mode: $mode";;
    esac
done;


function run_d0_stacks () {
    refl="--refline 0.5,3.0"
    eval ./stack2.py ${common}  -b --var 'd0' --bin '100,-0.1,0.1' -t ${tag} $@ ${refl} &
    eval ./stack2.py ${common}  -b --var 'd0sig' --bin '100,-5.0,5.0' -t ${tag} $@ ${refl} &
    eval ./stack2.py ${common}  -b --var 'z0' --bin '100,-1.0,1.0' -t ${tag} $@ ${refl}
    eval ./stack2.py ${common}  -b --var 'fabs\(l_pt_id-l_pt_exms\)/l_pt_id' --bin '50,0,2' -t ${tag} $@ &
}
function run_w_quick_stacks () {
    eval ./stack2.py ${common}  -b --var 'nvtxs_all' --bin '20,0,20' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'l_pt' --bin '50,0,100' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'met' --bin '50,0,200' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'w_mt' --bin '50,0,200' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'l_eta' --bin '50,-2.5,2.5' -t ${tag} $@ &
}
function run_w_stacks () {
    eval ./stack2.py ${common}  -b --var 'nvtxs_all' --bin '20,0,20' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'l_pt' --bin '50,0,100' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'met' --bin '50,0,200' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'w_mt' --bin '50,0,200' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'l_eta' --bin '50,-2.5,2.5' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'w_pt' --bin '50,0,200' -t ${tag} $@ &
}
function run_w_asym () {
    eval ./stack2.py ${common}  -b --var "\"fabs(l_eta)\"" --bin '25,0.0,2.5' -t ${tag} $@ &
    eval ./stack2.py ${common}  -b --var 'l_eta' --bin '50,-2.5,2.5' -t ${tag} $@ &
}
function run_w_asym_min () {
    eval ./stack2.py ${common}  -b --var "\"fabs(l_eta)\"" --bin '25,0.0,2.5' -t ${tag} $@ &
}
function run_z_stacks () {
    eval ./stack2.py ${common} --ntuple z -b --var 'nvtxs_all' --bin '20,0,20' -t ${tag} $@ &
    eval ./stack2.py ${common} --ntuple z -b --var 'lP_pt' --bin '50,0,100' -t ${tag} $@ &
    eval ./stack2.py ${common} --ntuple z -b --var 'lN_pt' --bin '50,0,100' -t ${tag} $@ &
    eval ./stack2.py ${common} --ntuple z -b --var 'met' --bin '50,0,200' -t ${tag} $@ &
    eval ./stack2.py ${common} --ntuple z -b --var 'Z_m' --bin '50,70,110' -t ${tag} $@ &
    eval ./stack2.py ${common} --ntuple z -b --var 'fabs\(lP_phi-lN_phi\)' --bin '50,0,6.3' -t ${tag} $@ &
}

# QCD data-driven methods
# TODO - work in progress! Need to integrate!
if [ "$mode" == "matrix" ]; then
    common="${common}"
    pre="${wpre_jordan}"
    m=matrix_2010inc
    # default plots:
    cut="mcw*puw"
    tag=QCD
    ./stack2.py ${common} -b -m${m} --var 'l_eta' --bin '20,-2.5,2.5' -t ${tag} --pre "${pre}" --cut "${cut}" --charge 0 &
    #./stack2.py ${common} -b -m${m} --var 'met' --bin '20,25,100' -t ${tag} --pre "${pre}" --cut "${cut}" --charge 0 &
    wait
fi

# Z MCP studies
# TODO - work in progress!
# TODO: merge in a single framework to do these kinds of plots; fix gaussian fit 
if [ "$mode" == "MCP" ]; then
    m=1013
    m=1012
    m=1111
    i=0
    gput tagzmcp ${i} ZMCP_default "--pre \"${zpre_jordan}\" --cut \"mcw*puw\""
    ((i++))
    i=0
    for itag in `gkeys tagzmcp`; do
	tag=`ggeta tagzmcp $itag`
	opts=`ggetb tagzmcp $itag`
	eval ./stack2.py ${common} -m${m} --ntuple z -b --var 'Z_m' --bin '50,70,110' --lvar lY_eta --lbin '1,-2.5,2.5' -t ${tag} ${opts} --func gaus &
	wait
	((i++))
    done

    wait
    echo DONE
fi

# stack plots and single-MC asymmetry
if [ "$mode" == "w" ]; then
    common="${common} --qcd AUTO"
    i=0
    cut="mcw*puw"
    gput tagss ${i} WJ_pythia_q2 "--pre \"${wpre_jordan}\" --cut \"${cut}\" --charge 2"
    ((i++))
    gput tagss ${i} WJ_pythia_q1 "--pre \"${wpre_jordan}\" --cut \"${cut}\" --charge 1"
    ((i++))
    gput tagss ${i} WJ_pythia_q0 "--pre \"${wpre_jordan}\" --cut \"${cut}\" --charge 0"
    ((i++))
    gput tagss ${i} WJ_alpgen_q2 "--pre \"${wpre_jordan}\" --cut \"${cut}\" --bgsig 3 --charge 2"
    ((i++))
    gput tagss ${i} WJ_alpgen_q1 "--pre \"${wpre_jordan}\" --cut \"${cut}\" --bgsig 3 --charge 1"
    ((i++))
    gput tagss ${i} WJ_alpgen_q0 "--pre \"${wpre_jordan}\" --cut \"${cut}\" --bgsig 3 --charge 0"
    ((i++))
    gput tagss ${i} WJ_mcnlo_q2 "--pre \"${wpre_jordan}\" --cut \"${cut}\" --bgsig 1 --charge 2"
    ((i++))
    gput tagss ${i} WP_pythia_q2 "--pre \"${wpre_peter}\" --cut \"${cut}\" --charge 2"
    ((i++))
    gput tagss ${i} WP_alpgen_q2 "--pre \"${wpre_peter}\" --cut \"${cut}\" --bgsig 3 --charge 2"
    ((i++))
    # run all jobs
    i=0
    for itag in `gkeys tagss`; do
	tag=`ggeta tagss $itag`
	opts=`ggetb tagss $itag`
	run_w_quick_stacks -m1 "${opts}"
	run_w_stacks -m1 "${opts}"
	if [ "${i}" -eq "1" ]; then
	    run_d0_stacks -m1 "${opts}"
	    run_w_asym -m12 "${opts}"
	    true
	fi;
	wait
	((i++))
    done
    wait
    echo DONE
fi

# Z stack histos
if [ "$mode" == "z" ]; then
    i=0
    cut="mcw*puw"
    gput tagzz ${i} ZJ_pythia_uncut "--pre \"${zpre_preiso}\" --cut \"${cut}\""
    ((i++))
    gput tagzz ${i} ZJ_pythia_all "--pre \"${zpre_jordan}\" --cut \"${cut}\""
    ((i++))
    gput tagzz ${i} ZP_pythia_all "--pre \"${zpre_peter}\" --cut \"${cut}\""
    ((i++))
    # note that alpgen gives a much better agreement for Zs!
    gput tagzz ${i} ZJ_alpgen_all "--pre \"${zpre_jordan}\" --cut \"${cut}\" --bgsig 3"
    ((i++))
    gput tagzz ${i} ZP_alpgen_all "--pre \"${zpre_peter}\" --cut \"${cut}\" --bgsig 3"
    ((i++))
    gput tagzz ${i} ZJ_mcnlo_all "--pre \"${zpre_jordan}\" --cut \"${cut}\" --bgsig 1"
    ((i++))
    # run all jobs
    i=0
    for itag in `gkeys tagzz`; do
	tag=`ggeta tagzz $itag`
	opts=`ggetb tagzz $itag`
	run_z_stacks -m1 "${opts}"
	wait
	((i++))
    done
    wait
    echo DONE
fi

# truth plots for multiple generators
if [ "$mode" == "truth" ]; then
    common="${common} --qcd AUTO"
    pre="${wpre_jordan}"
    m=1

    cut="mcw*puw"
    m=922
    tag=QALL
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 "
    tag=QPOS
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 0 "
    tag=QNEG
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 1 "
    wait

    cut="mcw*puw"
    m=921
    tag=ASYM
    run_w_asym  "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 1 "
    wait
    
    echo DONE
fi

# creation and verification of unfolding efficiency histograms
if [ "$mode" == "unfold" ]; then
    common="${common} --qcd AUTO"
    pre="${wpre_jordan}"
    cut="mcw*puw"
    common="${common} --effroot asymmetry_eff.root"

    # make eff histogram from Pythia?
    if [ "dome" == "dome" ]; then
	m=100
	tag=EFF_q0
	run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 0 "
	run_w_asym "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 0 "
	tag=EFF_q1
	run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 1 "
	run_w_asym "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 1 "
	tag=EFF_q2
	run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 "
	run_w_asym "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 "
	wait
    fi;
    # make eff histogram from Alpgen?
    if [ "dome" == "domeNOT" ]; then
	m=100
	tag=EFF_q0
	run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 0 --bgsig 3"
	run_w_asym "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 0 --bgsig 3"
	tag=EFF_q1
	run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 1 --bgsig 3"
	run_w_asym "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 1 --bgsig 3"
	tag=EFF_q2
	run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 --bgsig 3"
	run_w_asym "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 --bgsig 3"
	wait
    fi;

    m=2
    tag=EFFDO_q2_pythia
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 "
    tag=EFFDO_q2_alpgen
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 --bgsig 3"
    tag=EFFDO_q2_mcnlo
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 --bgsig 1"
    wait
    
fi;

# FINAL OUTPUT:
# reco-level asymmetry plots for multiple generators
# also allows to perform a correction to particle level
if [ "$mode" == "final" ]; then
    common="${common} --qcd AUTO"
    pre="${wpre_jordan}"
    cut="mcw*puw"
    m=923

    i=0
    # detector level
    gput tagsF ${i} RASYM  "--pre \"${pre}\" --cut \"${cut}\" -m ${m} "
    ((i++))
    # unfolded to particle level
    gput tagsF ${i} UASYM  "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --effroot asymmetry_eff.root"
    ((i++))
    # additional variations - for run
    if [ "2" -eq "2" ]; then
	gput tagsF ${i} RASYM_nj0  "--pre \"${pre} && njets==0\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_nj1  "--pre \"${pre} && njets==1\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_nj2  "--pre \"${pre} && njets==2\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_lpt2050  "--pre \"${pre} && l_pt>20 && l_pt<50\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_lpt5080  "--pre \"${pre} && l_pt>50 && l_pt<80\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_lpt80200  "--pre \"${pre} && l_pt>80 && l_pt<200\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_wpt0015  "--pre \"${pre} && w_pt>0 && w_pt<15\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_wpt1580  "--pre \"${pre} && w_pt>15 && w_pt<80\" --cut \"${cut}\" -m ${m} "
	((i++))
    fi
    if [ "1" -eq "1" ]; then
	gput tagsF ${i} RASYM_met4080  "--pre \"${pre} && met>40 && met<80\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_wmt4060  "--pre \"${pre} && w_mt>40 && w_mt<80\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_wmt4080  "--pre \"${pre} && w_mt>40 && w_mt<80\" --cut \"${cut}\" -m ${m} "
	((i++))
	gput tagsF ${i} RASYM_wmt80120  "--pre \"${pre} && w_mt>80 && w_mt<120\" --cut \"${cut}\" -m ${m} "
	((i++))
    fi;

    # run all jobs
    for itag in `gkeys tagsF`; do  # cuts
	tag=`ggeta tagsF $itag`
	opts=`ggetb tagsF $itag`
	run_w_asym_min "${opts}"
    done

    wait
    echo DONE
fi


# reco-level asymmetry: systematic variations
if [ "$mode" == "syst" ]; then
    common="${common} --qcd AUTO"
    pre="${wpre_jordan}"
    cut="mcw*puw"
    m=924

    i=0
    # detector level
    gput tagsF ${i} RASYM_sys  "--pre \"${pre}\" --cut \"${cut}\" -m ${m} "
    ((i++))

    # run all jobs
    for itag in `gkeys tagsF`; do  # cuts
	tag=`ggeta tagsF $itag`
	opts=`ggetb tagsF $itag`
	run_w_asym_min "${opts}"
    done

    wait
    echo DONE
fi

# W QCD FITS
if [ "$mode" == "qcd" ]; then
    common="${common} --qcd AUTO"
    #./stack2.py -b -m99 --var 'met' --bin '50,25,100' --hsource WJ/st_w_final/00_wmt/met --rebin 4
    i=0
    gput tagsq ${i} J_qcdfit  "--pre \"${wpre_jordan}\" --bin \"100,5,100\" "
    ((i++))
    gput tagsq ${i} P_qcdfit  "--pre \"${wpre_peter}\" --bin \"100,5,100\" "
    ((i++))
    # run all jobs
    for itag in `gkeys tagsq`; do  # cuts
	tag=`ggeta tagsq $itag`
	opts=`ggetb tagsq $itag`
	eval ./stack2.py ${common} -b -m99 --var 'met' ${opts} -t ${tag} &
    done
    wait
    echo DONE
fi

# Z tag and probe
if [ "$mode" == "tag" ]; then
    ncut='mcw*puw'
    # Select cuts against which we measure efficiency:
    i=0
    # iso relative
    BEF="lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>20.0 && fabs(lX_eta)<2.4 && lY_idhits==1 && fabs(lN_z0)<10. && lY_pt>20.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1"
    gput tags ${i} ptrel20  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_ptiso20/lY_pt<0.1\""
    ((i++))
    # trigger
    BEF="lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>20.0 && fabs(lX_eta)<2.4 && lY_idhits==1 && fabs(lN_z0)<10. && lY_pt>20.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1 && lY_ptiso20/lY_pt<0.1"
    gput tags ${i} trigEF  "--prebef \"${BEF}\" --preaft \"${BEF} && (lY_trigEF<0.2 || lY_trigEFb<0.2)\""
    ((i++))
    # iso absolute
    BEF="lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>20.0 && fabs(lX_eta)<2.4 && lY_idhits==1 && fabs(lN_z0)<10. && lY_pt>20.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso40<2.0 && lX_etiso40<2.0"
    #gput tags ${i} etiso40  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_etiso40<2.0\""
    ((i++))
    #gput tags ${i} ptiso40  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_ptiso40<2.0\""
    ((i++))
    # MCP (on isolated muons)
    BEF='lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>20.0 && fabs(lX_eta)<2.4 && fabs(lN_z0)<10. && lY_pt>20.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1 && lY_ptiso20/lY_pt<0.1'
    gput tags ${i} mcphits  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_idhits==1\""
    ((i++))
    # run all jobs
    for itag in `gkeys tags`; do  # cuts
	tag=`ggeta tags $itag`
	opts=`ggetb tags $itag`
	for bgsig in 0 3; do  # Pythia or Alpgen MC
	    for m in 101 103; do  # Use BG subtraction?
		eval ./stack2.py ${common} ${opts} --bgsig ${bgsig} --cut ${ncut} -m${m} --ntuple z -b --var 'lY_pt'  --bin '32,10,140' -t ${tag}_pt_MC${bgsig}&
		eval ./stack2.py ${common} ${opts} --bgsig ${bgsig} --cut ${ncut} -m${m} --ntuple z -b --var 'lY_eta' --bin '30,-2.5,2.5' -t ${tag}_eta_MC${bgsig} &
	    done
	done
	wait
    done
    wait
    echo DONE
fi
