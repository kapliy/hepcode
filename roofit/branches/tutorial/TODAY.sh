#!/bin/bash
source bashmap.sh

#./stack2.py -b -t TBNEW --var 'lY_eta' --bin '50,-2.5,2.5' -m101 --ntuple z
wpre_jordan='l_pt>20.0 && fabs(l_eta)<2.4 && ptiso20/l_pt<0.1 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'
wpre_peter='l_pt>20.0 && fabs(l_eta)<2.4 && ptiso40<2.0 && etiso40<2.0 && met>25.0 && w_mt>40.0 && idhits==1 && fabs(z0)<10. && fabs(d0sig)<10. && fabs(l_pt_id-l_pt_exms)/l_pt_id<0.5'
common="--input /share/ftkdata1/antonk/ana_v27_0930_all_stacoCB_10GeV/"
common="--input /share/ftkdata1/antonk/ana_v27_0930_all_stacoCB_10GeV_mc11pu/"
common="--input /share/ftkdata1/antonk/ana_v26_0930_all_stacoCB_10GeV/"
common="--input /share/ftkdata1/antonk/ana_v26_1008_all_stacoCB_test/" # new ntuple: correct efficiency

common="${common} --qcd AUTO"

# W stack histos
function run_d0_stacks () {
    refl="--refline 0.5,3.0"
    eval ./stack2.py ${common}  -b --var 'd0' --bin '100,-0.1,0.1' -t ${tag} $@ ${refl} &
    eval ./stack2.py ${common}  -b --var 'd0sig' --bin '100,-5.0,5.0' -t ${tag} $@ ${refl} &
    eval ./stack2.py ${common}  -b --var 'z0' --bin '100,-1.0,1.0' -t ${tag} $@ ${refl}
}
function run_w_stacks () {
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

# QCD studies: comparing shapes after inversion of certain cuts
# QCD template: bbar
# TODO - work in progress!
if [ "0" -eq "1" ]; then
    pre="${wpre_jordan}"
    m=920
    # default plots:
    cut="mcw*puw"
    tag=QCD
    ./stack2.py ${common} -b -m${m} --var 'w_mt' --bin '50,0,200' -t ${tag} --pre "${pre}" --cut "${cut}" &
    wait
fi

# stack plots and single-MC asymmetry
if [ "0" -eq "1" ]; then
    i=0
    gput tagss ${i} J_st0_q2 "--pre \"${wpre_jordan}\" --cut \"mcw*puw\" --charge 2"
    ((i++))
    gput tagss ${i} J_st0_q0 "--pre \"${wpre_jordan}\" --cut \"mcw*puw\" --charge 0"
    ((i++))
    gput tagss ${i} J_st0_q1 "--pre \"${wpre_jordan}\" --cut \"mcw*puw\" --charge 1"
    ((i++))
    gput tagss ${i} J_st3_q2 "--pre \"${wpre_jordan}\" --cut \"mcw*puw\" --bgsig 3 --charge 2"
    ((i++))
    gput tagss ${i} P_stack "--pre \"${wpre_peter}\" --cut \"mcw*puw\" "
    ((i++))
    # run all jobs
    i=0
    for itag in `gkeys tagss`; do
	tag=`ggeta tagss $itag`
	opts=`ggetb tagss $itag`
	run_w_stacks -m1 "${opts}"
	if [ "${i}" -eq "0" ]; then
	    run_d0_stacks -m1 "${opts}"
	    run_w_asym -m12 "${opts}"
	fi;
	wait
	((i++))
	break
    done

    echo DONE
fi

# truth plots for multiple generators
if [ "0" -eq "1" ]; then
    pre="${wpre_jordan}"
    m=1

    cut="mcw*puw"
    m=922
    tag=QALL
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 "
    tag=QPOS
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 0 "
    wait
    tag=QNEG
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 1 "

    cut="mcw*puw"
    m=921
    tag=ASYM
    run_w_asym  "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 1 "
    wait
    
    echo DONE
fi

# creation and verification of unfolding efficiency histograms
if [ "0" -eq "1" ]; then
    pre="${wpre_jordan}"
    cut="mcw*puw"
    common="${common} --effroot oct09_eff.root"

    if [ "1" -eq "1" ]; then
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

    m=2
    tag=EFFDO_q2_pythia
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 "
    tag=EFFDO_q2_alpgen
    run_w_stacks "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --charge 2 --bgsig 2"
    wait
    
fi;

# FINAL OUTPUT:
# reco-level asymmetry plots for multiple generators
# also allows to perform a correction to particle level
if [ "0" -eq "1" ]; then
    pre="${wpre_jordan}"
    cut="mcw*puw"
    m=923

    i=0
    # detector level
    #gput tagsF ${i} RASYM  "--pre \"${pre}\" --cut \"${cut}\" -m ${m} "
    ((i++))
    # unfolded to particle level
    #gput tagsF ${i} UASYM  "--pre \"${pre}\" --cut \"${cut}\" -m ${m} --effroot oct09_eff.root"
    ((i++))
    # additional variations - for run
    gput tagsF ${i} RASYM_nj0  "--pre \"${pre} && njets==0\" --cut \"${cut}\" -m ${m} "
    ((i++))
    gput tagsF ${i} RASYM_nj1  "--pre \"${pre} && njets==1\" --cut \"${cut}\" -m ${m} "
    ((i++))
    gput tagsF ${i} RASYM_nj2  "--pre \"${pre} && njets==2\" --cut \"${cut}\" -m ${m} "
    ((i++))
    gput tagsF ${i} RASYM_wmt4060  "--pre \"${pre} && w_mt>40 && w_mt<80\" --cut \"${cut}\" -m ${m} "
    ((i++))
    gput tagsF ${i} RASYM_wmt4080  "--pre \"${pre} && w_mt>40 && w_mt<80\" --cut \"${cut}\" -m ${m} "
    ((i++))
    gput tagsF ${i} RASYM_wmt80120  "--pre \"${pre} && w_mt>80 && w_mt<120\" --cut \"${cut}\" -m ${m} "
    ((i++))
    gput tagsF ${i} RASYM_wpt0015  "--pre \"${pre} && w_pt>0 && w_pt<15\" --cut \"${cut}\" -m ${m} "
    ((i++))
    gput tagsF ${i} RASYM_wpt1580  "--pre \"${pre} && w_pt>15 && w_pt<80\" --cut \"${cut}\" -m ${m} "
    ((i++))
    gput tagsF ${i} RASYM_met4080  "--pre \"${pre} && met>40 && met<80\" --cut \"${cut}\" -m ${m} "
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


# reco-level asymmetry: systematic variations
if [ "0" -eq "1" ]; then
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
if [ "0" -eq "1" ]; then
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

# Z stack histos
if [ "0" -eq "1" ]; then
    tag=zplots
    # TODO: rewrite using tag
    #./stack2.py -m1 --ntuple z -b --var 'Z_m' --bin '100,60,120' -t ${tag}_zm --pre '(lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0) && Z_m<150.0 && (lP_q*lN_q)<0 && lP_ptiso40<2.0 && lP_etiso40<2.0 && lN_ptiso40<2.0 && lN_etiso40<2.0'
    ./stack2.py -m1 --ntuple z -b --var 'lP_pt' --bin '32,10,140' -t ${tag}_lPpt_BEF --pre '(lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>10.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0) && Z_m>86 && Z_m<96 && (lP_q*lN_q)<0 && lN_ptiso20/lN_pt<0.1'
    ./stack2.py -m1 --ntuple z -b --var 'lP_pt' --bin '32,10,140' -t ${tag}_lPpt_AFT --pre '(lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>10.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0) && Z_m>86 && Z_m<96 && (lP_q*lN_q)<0 && lN_ptiso20/lN_pt<0.1 && lP_ptiso40<2.0'
    wait
    echo DONE
fi

# Z tag and probe
if [ "0" -eq "1" ]; then
    ncut='mcw*puw'
    # Select cuts against which we measure efficiency:
    i=0
    BEF="lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>10.0 && fabs(lX_eta)<2.4 && lY_idhits==1 && fabs(lN_z0)<10. && lY_pt>10.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso40<2.0 && lX_etiso40<2.0"
    gput tags ${i} etiso40  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_etiso40<2.0\""
    ((i++))
    gput tags ${i} ptiso40  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_ptiso40<2.0\""
    ((i++))
    BEF='lX_idhits==1 && fabs(lP_z0)<10. && lX_pt>10.0 && fabs(lX_eta)<2.4 && fabs(lN_z0)<10. && lY_pt>10.0 && fabs(lY_eta)<2.4 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso40<2.0 && lX_etiso40<2.0'
    gput tags ${i} mcphits  "--prebef \"${BEF}\" --preaft \"${BEF} && lY_idhits==1\""
    ((i++))
    # run all jobs
    for itag in `gkeys tags`; do  # cuts
	tag=`ggeta tags $itag`
	opts=`ggetb tags $itag`
	for bgsig in 0 3; do  # Pythia or Alpgen MC
	    for m in 101 102; do  # Use BG subtraction?
		eval ./stack2.py ${common} ${opts} --bgsig ${bgsig} --cut ${ncut} -m${m} --ntuple z -b --var 'lY_pt'  --bin '32,10,140' -t ${tag}_pt_MC${bgsig}&
		eval ./stack2.py ${common} ${opts} --bgsig ${bgsig} --cut ${ncut} -m${m} --ntuple z -b --var 'lY_eta' --bin '30,-2.5,2.5' -t ${tag}_eta_MC${bgsig} &
		break
	    done
	    break
	done
	wait
    done
    wait
    echo DONE
fi
