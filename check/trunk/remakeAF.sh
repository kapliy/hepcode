#!/bin/bash

# a version for Atlfast II files

# if reading from LOCALGROUPDISK and XROOTD
#tag=v1_29g
#CDIR=/atlas/uct3/data/users/antonk/NTUPLE/BACKUP/${tag}
#RDIR=/pnfs/uchicago.edu/atlaslocalgroupdisk/user/kapliy/UCNTUP

# if reading from LOCALGROUPDISK and LOCAL DIR
tag=AFII.v1_29i
RDIR=/share/t3data3/antonk/NTUPLE/${tag}
CDIR=/pnfs/uchicago.edu/atlaslocalgroupdisk/user/kapliy/UCNTUP

rm -f af_* pythia_* alpgen_* jimmy_* acer_* mcnlo_* herwig_* powheg_* sherpa_* {B..M}

function list_dir () {
    SOURCEDIR=$1
    OUTNAME=$2
    RFLAG=$3
    if [ "$RFLAG" -eq "$RFLAG" ] 2>/dev/null; then # numeric number = MC channel
	find ${SOURCEDIR}/user*mc11*${RFLAG}*AOD*mc11c.${tag}* -maxdepth 2 -type f -name \*root\* 2>/dev/null | sort >> ${OUTNAME}
    else  # letter = data period
	find ${SOURCEDIR}/user*data11*eriod${RFLAG}*Muons*AOD*data.${tag}* -maxdepth 2 -type f -name \*root\* 2>/dev/null | sort >> ${OUTNAME}
    fi
}
function run () {
    OUTNAME=$1
    RFLAG=$2
    rm -f ${OUTNAME}
    list_dir ${CDIR} ${OUTNAME} ${RFLAG}
    list_dir ${RDIR} ${OUTNAME} ${RFLAG}
    # duplicate removal. RDIR (xrootd) takes precedence over CDIR (dcache)
    mv ${OUTNAME} ${OUTNAME}.tmp
    awk 'function bn(p) { n=split(p,a,"/"); return a[n]; } {CACHE[bn($1)]=$1} END{for(key in CACHE) print CACHE[key];}' ${OUTNAME}.tmp | sort > ${OUTNAME}
    echo "Working on: ${OUTNAME} : `wc -l < ${OUTNAME}.tmp` -> `wc -l < ${OUTNAME}`"
    rm -f ${OUTNAME}.tmp
}

run af_pythia_zmumu 106047
run af_alpgen_herwig_wmunu_np0 107690
run af_alpgen_herwig_wmunu_np1 107691
run af_alpgen_herwig_wmunu_np2 107692
run af_alpgen_herwig_wmunu_np3 107693
run af_alpgen_herwig_wmunu_np4 107694
run af_alpgen_herwig_wmunu_np5 107695

# clean up empty files
for f in *; do 
    if [ ! -s $f ]; then 
	rm $f; 
    fi; 
done
