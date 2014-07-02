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

lbl=10042013_maxisoMC                 # got rid of z0 (v54/v56). Upgraded ScaleFactorProvider, pointed to new Max's iso files
lbl=10042013_maxisoMC2d               # v57. save as v56, but using 2d isoMC
lbl=11282013_dinosisoMC               # updating to the latest iso SF from Dinos (finalized Up/Down systematics)

lbl=07012014_slc6                     # should be same as dinos. SLC6 & new reweighting lib

common_opts="--release ${release} --save-ntuples 7 --apply-pileup --pileup-scale 1.0 --data-range DtoM"

muname[0]="stacoCB"
muname[1]="stacoLoose"
muname[100]="muidCB"

coredir=/share/t3data3/$USER/ana

export X509_USER_PROXY=/home/$USER/.globus/tmp.proxy
echo "Setting up X509 proxy"
(source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet && source ${ATLAS_LOCAL_ROOT_BASE}/packageSetups/atlasLocalFAXSetup.sh --faxtoolsVersion dynamic && voms-proxy-init -pwstdin -voms atlas -valid 999:0 -out ${X509_USER_PROXY} < /home/$USER/setup/info 2>&1 && echo CERTIFICATE_ISSUED)
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
