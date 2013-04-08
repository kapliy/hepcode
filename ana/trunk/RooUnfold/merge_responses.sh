#!/bin/bash

COPY=/home/antonk/TrigFTKAna/RooUnfold/copyFiles.C
HADD=/home/antonk/TrigFTKAna/RooUnfold/hadd.C

SF_PP_POS=1.25208879
SF_PP_NEG=1.18275621
SF_PH_POS=3.62766666
SF_PH_NEG=3.35023704
SF_MC_POS=4.25821344
SF_MC_NEG=3.98844780

function run() {
    name=$1
    SPOS=$2
    SNEG=$3
    FLIST="list_unfold_histo_${name}.dat"
    FOUT="unfold_histo_${name}_all.root"
    # min
    fin_nameNEG=unfold_histo_${name}_min.root
    fout_nameNEG="ALL_${fin_nameNEG}"
    QDIRNEG=NEG
    root -q -b "${COPY}(\"${fin_nameNEG}\",\"${fout_nameNEG}\",\"${QDIRNEG}\")" || { echo "FAILED"; exit 1; }
    # plus
    fin_namePOS=unfold_histo_${name}_plus.root
    fout_namePOS="ALL_${fin_namePOS}"
    QDIRPOS=POS
    root -q -b "${COPY}(\"${fin_namePOS}\",\"${fout_namePOS}\",\"${QDIRPOS}\")" || { echo "FAILED"; exit 1; }
    # merge, normalizing to lower-statistics POS samples (i.e., POS histo taken as-is, NEG histogram scaled down and added)
    echo "${fout_namePOS} ${SPOS}" > $FLIST
    echo "${fout_nameNEG} ${SNEG}" >> $FLIST
    root -b -q "${HADD}(\"${FLIST}\",\"${FOUT}\")" || { echo "FAILED"; exit 1; }
}

run mcnlo ${SF_MC_POS} ${SF_MC_NEG}
run powheg_herwig ${SF_PH_POS} ${SF_PH_NEG}
run powheg_pythia ${SF_PP_POS} ${SF_PP_NEG}
