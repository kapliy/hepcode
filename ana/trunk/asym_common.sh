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

lbl=06282013_postSMmtg              # same as below, but very buggy. not sure why. job taking 25 GB of RAM, jobs hanging on hadd, segfaulting on hadd
lbl=06292013_postSMmtg              # run z ntuples (for matrix method studies). Add 1 sigma up/down stat. uncertainties for reco and trig. noExit added. 06/29: added smart_killer. removed noExit
lbl=07012013_postSMmtg_datarw       # updated Max's data-rw class - it now uses smoothed histograms
lbl=07082013_postSMmtg_recofix      # fixed reco toy MC (they were broken)
lbl=07112013_postSMmtg_datarw       # another fix to max rw (v5)
lbl=07122013_postSMmtg_datarw       # Updated to new lumi tag and ttbar xsec. fixed data rw: max said previous version was inverted
lbl=07122013_postSMmtg_datarw_ret   # just re-running from scratch

lbl=08282013_isoMC_cwdebug            # isolation scale factors are re-run on per-MC basis. Added CW decomposition
lbl=09102013_isoMC_cwdebug            # Added more complete CW decomposition, including a couple of particularly-bad bins in eta/pt
lbl=09132013_isoMC_cwdebug_iso20      # iso SF use 20% variations (test for max)
lbl=09132013_isoMC_cwdebug_iso2d      # iso SF use 2d eta-pt; no sys. variations (test for max)
lbl=09132013_isoMC_cwdebug_iso2d_ret  # iso SF use 2d eta-pt; no sys. variations (test for max)

lbl=09202013_isoMC_reweights          # revert to normal per-MC isolation. Added some reweighting plots for thesis (final selection). Same as 08282013_isoMC_cwdebug!
lbl=09302013_addz0                    # added muon z0 to PV cut of 10 mm. v55

lbl=10042013_maxisoMC                 # got rid of z0 (v54/v56). Upgraded ScaleFactorProvider, pointed to new Max's iso files

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
