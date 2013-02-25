#!/bin/bash

# if reading from LOCALGROUPDISK and LOCAL DIR
if [ "0" -eq "1" ]; then
    tag=v1_29i
    RDIR=/atlas/uct3/data/users/antonk/NTUPLE/${tag}
    CDIR=/tmp/antonk
fi

if [ "1" -eq "1" ]; then
    tag=v1_29i
    tag=v1_g29i
    RDIR=/atlas/uct3/data/users/antonk/NTUPLE/v1_29i/
    CDIR=/pnfs/uchicago.edu/atlaslocalgroupdisk/user/kapliy/UCNTUP
fi

#rm -f af_alpgen_* af_pythia_* pythia_* alpgen_* jimmy_* acer_* mcnlo_* herwig_* powheg_* sherpa_* {B..M}

function list_dir () {
    SOURCEDIR=$1
    OUTNAME=$2
    RFLAG=$3
    if [ "$RFLAG" -eq "$RFLAG" ] 2>/dev/null; then # numeric number = MC channel
	find ${SOURCEDIR}/user*mc11*${RFLAG}*AOD*mc11c.${tag}* -maxdepth 2 -type f -name \*root\* 2>/dev/null | sort >> ${OUTNAME}
    else  # letter = data period
	# BUGGY SUBMISSION:
	find ${SOURCEDIR}/user*data11*eriod${RFLAG}*Muons*AOD*mc11c.${tag}* -maxdepth 2 -type f -name \*root\* 2>/dev/null | sort >> ${OUTNAME}
	# CORRECT:
	#find ${SOURCEDIR}/user*data11*eriod${RFLAG}*Muons*AOD*data.${tag}* -maxdepth 2 -type f -name \*root\* 2>/dev/null | sort >> ${OUTNAME}
    fi
}
function run () {
    OUTNAME=$1
    RFLAG=$2
    rm -f ${OUTNAME}
    list_dir ${CDIR} ${OUTNAME} ${RFLAG}
    list_dir ${RDIR} ${OUTNAME} ${RFLAG}
    # duplicate removal. RDIR (xrootd) takes precedence over CDIR (dcache)
    if [ -f "${OUTNAME}" ]; then
	mv ${OUTNAME} ${OUTNAME}.tmp
	awk 'function bn(p) { n=split(p,a,"/"); return a[n]; } {CACHE[bn($1)]=$1} END{for(key in CACHE) print CACHE[key];}' ${OUTNAME}.tmp | sort > ${OUTNAME}
	echo "Working on: ${OUTNAME} : `wc -l < ${OUTNAME}.tmp` -> `wc -l < ${OUTNAME}`"
	rm -f ${OUTNAME}.tmp
    fi
}

if [ "0" == "1" ]; then
    # MC@NLO
    run mcnlo_wminmunu 106084
    run mcnlo_wplusmunu 106083
    # PowHeg+Herwig
    run powheg_herwig_wminmunu 113185
    run powheg_herwig_wplusmunu 113187
fi

run L L

# clean up empty files
for f in *; do 
    if [ ! -s $f ]; then 
	rm $f; 
    fi; 
done
