#!/bin/bash

source asym_archive.sh

release=17
PT=20
NREPLICASF=1000  # nreplicas for wmunu MC

submitter="./condor_wasym.py"
if [ "${MODE}" == "pbs" ]; then
    submitter="./submit_wasym.py"
else
    # run condor_release every now and then
    if [ "$USER" == "antonk" ]; then
	crontab -r && crontab cron.tab
    fi
fi

ntuple=29I

lbl=05182013_fullstat  # update to full mcnlo and powhegHerig stat. new egammaUtils: remove wpol constraint and apply jan's new experimental wpol reweights (mcnlo->powhegherwig or mcnlo/powhegherwig->powhegpythia) to reduce CW syst. fix bootstrapping to 1000 replicas and only run on baseline Nominal data.
lbl=05202013_fullstat      # fixed two bugs in bootstrap: int25 was wrong, and 2d used the wrong type cast
lbl=05202013_fullstat_ret  # retry before trying out running over OSG nodes
lbl=05202013_fullstat_noC  # removed curvature correction
lbl=06092013_recoSFphi     # MCP trunk scale factors (phi only)
lbl=06092013_recoSFphi_ret # updated RootCore, added compiler optimizations, unrolled response matrix loop; increased nsplits for sig, added anyfit

lbl=06132013_recoSFpt      # re-running same-old version, but after all updates, using latest MuonEfficiencyCorrections, but old eta x pt txt file
lbl=06132013_recoSFphi     # phi version (re-run)

lbl=06172013_recoBOTH_withZ # nominal is SFpt, but added a new st_w_bla option to do phi. Also: running z plots

lbl=06192013_recoBOTH_withZ # removed Z ntuple. disabled trig matching. updated reco SF class (correct errors). Added deltaPhi(soft,all-but-soft). Added a new systematic - reweight signal MC to data (Max's file).
lbl=06192013_recoBOTH_withZ_recoPhi  # Eta-phi SF are nominal
lbl=06192013_recoBOTH_withZ_pt25 # rerunning pt25 case with actual full statistics

lbl=06192013_recoBOTH_withZ_anyfit  # reco=(eta,phi)added anyfit to make MET plots. updated to new iso SFs. added trigphi, effphi, eff*trigphi
lbl=06192013_recoBOTH_withZ_rep     # reco=(eta,phi) add back trigger matching and z ntuple information
lbl=06192013_recoBOTH_withZ_etaonly # reco=(eta) fixed a bug that reverted etaonly flag  #NEVER RAN

lbl=06252013_newrecoSF              # new meaning of systematic: full difference between ID and CALO probes
lbl=06252013_newrecoSF_rep          # just a re-run: previous run was affected by some xrootd problem (just in case)
lbl=06252013_newrecoSF_MG           # MG version
lbl=06252013_newrecoSF_MG_rep       # MG version - rerunning to fix root_mc_powheg_pythia_wminmunu.root

lbl=06282013_postSMmtg              # same as below, but very buggy. not sure why. job taking 25 GB of RAM, jobs hanging on hadd, segfaulting on hadd
lbl=06292013_postSMmtg              # run z ntuples (for matrix method studies). Add 1 sigma up/down stat. uncertainties for reco and trig. noExit added. 06/29: added smart_killer. removed noExit

common_opts="--release ${release} --save-ntuples 7 --apply-pileup --pileup-scale 1.0 --data-range DtoM"

muname[0]="stacoCB"
muname[1]="stacoLoose"
muname[100]="muidCB"

coredir=/share/t3data3/$USER/ana

export X509_USER_PROXY=/home/$USER/.globus/tmp.proxy
echo "Setting up X509 proxy"
(source /share/wlcg-client/setup.sh && voms-proxy-init -pwstdin -voms atlas -valid 999:0 -out ${X509_USER_PROXY} < /home/$USER/setup/info 2>&1)
echo "Ready to submit jobs"

function compute_nevts() {
    n=$1
    sam=$2
    echo $sam | egrep -q -e 'periodG|periodK' && { echo 40; return; }
    echo $sam | egrep -q -e 'periodL|periodM' && { echo 50; return; }
    echo $sam | egrep -q -e 'mc_powheg_pythia_wminmunu|mc_powheg_pythia_wplusmunu|mc_powheg_pythia_zmumu' && { echo 40; return; }
    if [ $n -lt 5 ]; then echo 1; return; fi;
    if [ $n -lt 10 ]; then echo 2; return; fi;
    if [ $n -lt 20 ]; then echo 2; return; fi;
    if [ $n -lt 50 ]; then echo 5; return; fi;
    if [ $n -lt 100 ]; then echo 20; return; fi;
    if [ $n -lt 300 ]; then echo 25; return; fi;
    if [ $n -lt 500 ]; then echo 30; return; fi;
    echo 35
}

# condor supports many many more subjobs per samples
function compute_nevts_condor() {
    n=$1
    sam=$2
    echo $sam | egrep -q -e 'periodG|periodK' && { echo 50; return; }
    echo $sam | egrep -q -e 'periodL|periodM' && { echo 100; return; }
    echo $sam | egrep -q -e 'mc_powheg_pythia_zmumu' && { echo 150; return; }
    # wminus
    echo $sam | egrep -q -e 'mc_powheg_herwig_wminmunu|mc_mcnlo_wminmunu' && { echo 100; return; }
    # wplus
    echo $sam | egrep -q -e 'mc_powheg_herwig_wplusmunu|mc_mcnlo_wplusmunu' && { echo 120; return; }
    # powheg-pythia
    echo $sam | egrep -q -e 'mc_powheg_pythia_wminmunu' && { echo 120; return; }
    echo $sam | egrep -q -e 'mc_powheg_pythia_wplusmunu' && { echo 150; return; }
    if [ $n -lt 5 ]; then echo 1; return; fi;
    if [ $n -lt 10 ]; then echo 5; return; fi;
    if [ $n -lt 20 ]; then echo 10; return; fi;
    if [ $n -lt 50 ]; then echo 10; return; fi;
    if [ $n -lt 100 ]; then echo 20; return; fi;
    if [ $n -lt 300 ]; then echo 50; return; fi;
    echo 100
}

# maximally parallel version
function compute_nevts_condor_parallelized() {
    n=$1
    sam=$2
    echo $sam | egrep -q -e 'periodG|periodK' && { echo 50; return; }
    echo $sam | egrep -q -e 'periodL|periodM' && { echo 100; return; }
    echo $sam | egrep -q -e 'mc_powheg_pythia_zmumu' && { echo 150; return; }
    # wminus
    echo $sam | egrep -q -e 'mc_powheg_herwig_wminmunu|mc_mcnlo_wminmunu' && { echo 200; return; }
    # wplus
    echo $sam | egrep -q -e 'mc_powheg_herwig_wplusmunu|mc_mcnlo_wplusmunu' && { echo 292; return; }
    # powheg-pythia
    echo $sam | egrep -q -e 'mc_powheg_pythia_wminmunu' && { echo 250; return; }
    echo $sam | egrep -q -e 'mc_powheg_pythia_wplusmunu' && { echo 362; return; }
    if [ $n -lt 5 ]; then echo 1; return; fi;
    if [ $n -lt 10 ]; then echo 5; return; fi;
    if [ $n -lt 20 ]; then echo 10; return; fi;
    if [ $n -lt 50 ]; then echo 10; return; fi;
    if [ $n -lt 100 ]; then echo 20; return; fi;
    if [ $n -lt 300 ]; then echo 50; return; fi;
    echo 100
}

function compute_replicas() {
    sam=$1
    echo $sam | egrep -q -e 'mc_powheg_pythia_wminmunu|mc_powheg_pythia_wplusmunu' && { echo ${NREPLICASF}; return; }
    echo $sam | egrep -q -e 'mc_powheg_herwig_wminmunu|mc_powheg_herwig_wplusmunu' && { echo ${NREPLICASF}; return; }
    echo $sam | egrep -q -e 'mc_mcnlo_wminmunu|mc_mcnlo_wplusmunu' && { echo ${NREPLICASF}; return; }
    echo 1
}

function submit_sample() {
    sample=$1
    sample_path="samples/$sample"
    tag=`echo $sample | sed -e "s#wasymmetry${ntuple}_##g" -e 's#.dat##g'`
    ntot=`cat ${sample_path} | grep -c DPETER`
    if [ "${MODE}" == "pbs" ]; then
	nsubs=`compute_nevts $ntot $sample`
    else
	nsubs=`compute_nevts_condor $ntot $sample`
    fi
    replicas=`compute_replicas $sample`
    echo $sample $ntot $nsubs $replicas
    if [ "$nsubs" -gt "0" ]; then
	${submitter} -o ${out} -s ${sample_path} -t ${tag} --options "${xtra} --pt ${PT} --replicas ${replicas}" -n ${nsubs}
	# forced version (to retry jobs failed in the same directory)
	#${submitter} -f -o ${out} -s ${sample_path} -t ${tag} --options "${xtra} --pt ${PT} --replicas ${replicas}" -n ${nsubs}
    fi;
    sleep 0.1
}

function submit_mc() {
    samples=`cd samples && ls -1 wasymmetry${ntuple}_mc*.dat`
    for s in ${samples}; do
	submit_sample ${s}
    done
}

function submit_data() {
    samples=`cd samples && ls -1 wasymmetry${ntuple}_data*.dat`
    for s in ${samples}; do
	submit_sample ${s}
    done
}

function submit_all() {
    submit_mc
    submit_data
}


function submit_powheg() {
    samples=`cd samples && ls -1 wasymmetry${ntuple}_mc_powheg*.dat`
    for s in ${samples}; do
	submit_sample ${s}
    done
}

function submit_zmumu() {
    submit_zmumu_all
}

function submit_zmumu1() {
    samples=`cd samples && ls -1 wasymmetry${ntuple}_mc_pythia_zmumu*.dat`
    for s in ${samples}; do
	submit_sample ${s}
    done
}

function submit_zmumu_all() {
    samples=`cd samples && ls -1 wasymmetry${ntuple}_mc_*zmu*.dat 2>/dev/null`
    for s in ${samples}; do
	submit_sample ${s}
    done
}

function submit_wmumu() {
    submit_wmumu_all
}

function submit_wmumu_all() {
    samples=`cd samples && ls -1 wasymmetry${ntuple}_mc_*wminmu*.dat wasymmetry${ntuple}_mc_*wplusmu*.dat wasymmetry${ntuple}_mc_*wmunu*.dat 2>/dev/null`
    for s in ${samples}; do
	submit_sample ${s}
    done
}

function submit_wmumu_nlo() {
    samples=`cd samples && ls -1 wasymmetry${ntuple}_mc_*wminmu*.dat wasymmetry${ntuple}_mc_*wplusmu*.dat 2>/dev/null`
    for s in ${samples}; do
	submit_sample ${s}
    done
}

function submit_wmumu_nlo_sys() {
    samples=`cd samples && ls -1 wasymmetry${ntuple}_mc_*mcnlo*wminmu*.dat wasymmetry${ntuple}_mc_*mcnlo*wplusmu*.dat wasymmetry${ntuple}_mc_*herwig*wminmu*.dat wasymmetry${ntuple}_mc_*herwig*wplusmu*.dat 2>/dev/null`
    for s in ${samples}; do
	submit_sample ${s}
    done
}
