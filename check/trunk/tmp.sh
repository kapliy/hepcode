#!/bin/bash

tag=v1_29g

RDIR=/atlas/uct3/data/users/antonk/NTUPLE/${tag}

# if reading from LOCALGROUPDISK
#CDIR=/pnfs/uchicago.edu/atlaslocalgroupdisk/user/kapliy/UCNTUP
# if reading from tier3 XROOTD
CDIR=/atlas/uct3/data/users/antonk/NTUPLE/BACKUP/${tag}

rm -f pythia_* alpgen_* jimmy_* acer_* mcnlo_* herwig_* powheg_* sherpa_* {B..M}

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

# MC@NLO
run mcnlo_zmumu 106088

# clean up empty files
for f in *; do 
    if [ ! -s $f ]; then 
	rm $f; 
    fi; 
done
