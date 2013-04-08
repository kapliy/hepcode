#!/bin/bash

release=17
PT=20

submitter="./condor_wasym.py"
if [ "${MODE}" == "pbs" ]; then
    submitter="./submit_wasym.py"
fi

ntuple=29I

lbl=04012013       # condor

lbl=04032013_nomg  # trying Carl's first version of non-MG trigger SFs
lbl=04032013_nomg_1000toys  # re-running with 1000 toy MCs

common_opts="--release ${release} --save-ntuples 7 --apply-pileup --pileup-scale 1.0 --data-range DtoM"

muname[0]="stacoCB"
muname[1]="stacoLoose"
muname[100]="muidCB"

coredir=/share/t3data3/antonk/ana

echo "Setting up X509 proxy"
export X509_USER_PROXY=/home/antonk/.globus/tmp.proxy
if [ "${MODE}" == "pbs" ]; then
    (source /share/wlcg-client/setup.sh && voms-proxy-init -pwstdin -voms atlas -valid 999:0 -out ${X509_USER_PROXY} < /home/antonk/setup/info 2>&1)
fi
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

function compute_nevts_L() {
    n=$1
    sam=$2
    if [ $n -lt 5 ]; then echo 1; return; fi;
    if [ $n -lt 10 ]; then echo 5; return; fi;
    if [ $n -lt 20 ]; then echo 7; return; fi;
    if [ $n -lt 50 ]; then echo 15; return; fi;
    if [ $n -lt 100 ]; then echo 30; return; fi;
    echo 40
}

function submit_sample() {
    sample=$1
    sample_path="samples/$sample"
    tag=`echo $sample | sed -e "s#wasymmetry${ntuple}_##g" -e 's#.dat##g'`
    ntot=`cat ${sample_path} | grep -c DPETER`
    nsubs=`compute_nevts $ntot $sample`
    echo $sample $ntot $nsubs
    pdf_reweight=""
    echo $tag | grep -q lha && pdf_reweight="--pdf-reweight ${tag: -1}"
    if [ "$nsubs" -gt "0" ]; then
	${submitter} -o ${out} -s ${sample_path} -t ${tag} --options "${xtra} ${pdf_reweight} --pt ${PT}" -n ${nsubs}
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
