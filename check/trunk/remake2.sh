#!/bin/bash

# careful: check for grep -v in list_dir

if [ "1" -eq "1" ]; then
    tag=v1_m29i
    RDIR=/atlas/uct3/data/users/antonk/NTUPLE/v1_29i/${tag}
    CDIR=/pnfs/uchicago.edu/atlaslocalgroupdisk/user/kapliy/UCNTUP
fi

rm -f af_alpgen_* af_pythia_* pythia_* alpgen_* jimmy_* acer_* mcnlo_* herwig_* powheg_* sherpa_* {B..M}

function list_dir () {
    SOURCEDIR=$1
    OUTNAME=$2
    RFLAG=$3
    if [ "$RFLAG" -eq "$RFLAG" ] 2>/dev/null; then # numeric number = MC channel
	find ${SOURCEDIR}/user*mc11*${RFLAG}*AOD*mc11c.${tag}* -maxdepth 2 -type f -name \*root\* 2>/dev/null | sort >> ${OUTNAME}
    else  # letter = data period
	# BUGGY SUBMISSION:
	#find ${SOURCEDIR}/user*data11*eriod${RFLAG}*Muons*AOD*mc11c.${tag}* -maxdepth 2 -type f -name \*root\* 2>/dev/null | grep -v ret2 | sort >> ${OUTNAME}
	# CORRECT:
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
    if [ -f "${OUTNAME}" ]; then
	mv ${OUTNAME} ${OUTNAME}.tmp
	awk 'function bn(p) { n=split(p,a,"/"); return a[n]; } {CACHE[bn($1)]=$1} END{for(key in CACHE) print CACHE[key];}' ${OUTNAME}.tmp | sort > ${OUTNAME}
	echo "Working on: ${OUTNAME} : `wc -l < ${OUTNAME}.tmp` -> `wc -l < ${OUTNAME}`"
	rm -f ${OUTNAME}.tmp
    fi
}


######################################
# FIXME: just running what's necessary
######################################
run mcnlo_wminmunu 106084
run mcnlo_wplusmunu 106083
run powheg_herwig_wminmunu 113185
run powheg_herwig_wplusmunu 113187
######################################
exit 0; # FIXME
######################################

for per in {D..M}; do
    run ${per} ${per}
done

# ttbar sample is HUGE, cut it by x5:
run mcnlo_ttbar_tmp 105200
head -n200 mcnlo_ttbar_tmp > mcnlo_ttbar
rm -f mcnlo_ttbar_tmp

run alpgen_herwig_wmunu_np0 107690
run alpgen_herwig_wmunu_np1 107691
run alpgen_herwig_wmunu_np2 107692
run alpgen_herwig_wmunu_np3 107693
run alpgen_herwig_wmunu_np4 107694
run alpgen_herwig_wmunu_np5 107695

run alpgen_herwig_wtaunu_np0 107700
run alpgen_herwig_wtaunu_np1 107701
run alpgen_herwig_wtaunu_np2 107702
run alpgen_herwig_wtaunu_np3 107703
run alpgen_herwig_wtaunu_np4 107704
run alpgen_herwig_wtaunu_np5 107705

# run alpgen_herwig_zmumu_np0 107660
# run alpgen_herwig_zmumu_np1 107661
# run alpgen_herwig_zmumu_np2 107662
# run alpgen_herwig_zmumu_np3 107663
# run alpgen_herwig_zmumu_np4 107664
# run alpgen_herwig_zmumu_np5 107665

run alpgen_herwig_ztautau_np0 107670
run alpgen_herwig_ztautau_np1 107671
run alpgen_herwig_ztautau_np2 107672
run alpgen_herwig_ztautau_np3 107673
run alpgen_herwig_ztautau_np4 107674
run alpgen_herwig_ztautau_np5 107675

if [ "0" -eq "1" ]; then
    run alpgen_herwig_wgamma_np0 117410
    run alpgen_herwig_wgamma_np1 117411
    run alpgen_herwig_wgamma_np2 117412
    run alpgen_herwig_wgamma_np3 117413
    run alpgen_herwig_wgamma_np4 117414
    run alpgen_herwig_wgamma_np5 117415
    run pythia_wgammamunu 128851
    run pythia_wgammataunu 128852
    run pythia_zgammamumu 108324
    run pythia_zgammatautau 108325
fi

if [ "0" -eq "1" ]; then
    run alpgen_herwig_wmunubb_np0 107280
    run alpgen_herwig_wmunubb_np1 107281
    run alpgen_herwig_wmunubb_np2 107282
    run alpgen_herwig_wmunubb_np3 107283
    run alpgen_herwig_wmunucc_np0 117284
    run alpgen_herwig_wmunucc_np1 117285
    run alpgen_herwig_wmunucc_np2 117286
    run alpgen_herwig_wmunucc_np3 117287
    run alpgen_herwig_wmunuc_np0 117293
    run alpgen_herwig_wmunuc_np1 117294
    run alpgen_herwig_wmunuc_np2 117295
    run alpgen_herwig_wmunuc_np3 117296
    run alpgen_herwig_wmunuc_np4 117297
fi

# mc11c too buggy
if [ "0" -eq "1" ]; then
    run alpgen_pythia_wmunu_np0 117690
    run alpgen_pythia_wmunu_np1 117691
    run alpgen_pythia_wmunu_np2 117692
    run alpgen_pythia_wmunu_np3 117693
    run alpgen_pythia_wmunu_np4 117694
    run alpgen_pythia_wmunu_np5 117695
fi;

# Pythia
if [ "1" == "0" ]; then
    run pythia_wtaunu 107054
    run pythia_wmunu 106044
    run pythia_dyan 108319
    run pythia_ztautau 106052
fi
run pythia_zmumu 106047
run pythia_ccmu15x 106059
run pythia_bbmu15x 108405

# MC@NLO
run mcnlo_wminmunu 106084
run mcnlo_wplusmunu 106083
run mcnlo_zmumu 106088
run mcnlo_dyan 147217
#run mcnlo_wmintaunu 108329
#run mcnlo_wplustaunu 108328
#run mcnlo_ztautau 106062
# PowHeg+Pythia
run powheg_pythia_wminmunu 108301
run powheg_pythia_wplusmunu 108298
run powheg_pythia_zmumu 108304
run powheg_pythia_dyan 129807
#run powheg_pythia_wmintaunu 107391
#run powheg_pythia_wplustaunu 107390
#run powheg_pythia_ztautau 107392
# PowHeg+Herwig
run powheg_herwig_wminmunu 113185
run powheg_herwig_wplusmunu 113187
run powheg_herwig_zmumu 126007
run powheg_herwig_dyan 129809
#run powheg_herwig_wmintaunu 107394
#run powheg_herwig_ztautau 107395


# Sherpa
if [ "0" -eq "1" ]; then
    run sherpa_zmumu 114610
    run sherpa_wmunu 114612
    run sherpa_wtaunu 118960
    run sherpa_ztautau 118961
fi

# JX samples
if [ "0" -eq "1" ]; then
    run herwig_J0 113204
    run herwig_J1 113205
    run herwig_J2 113206
    run herwig_J3 113207
    run herwig_J4 113208
    run herwig_J5 113209
    run herwig_J6 113210
    
    run pythia_J7 105016
    run pythia_J8 105017
    run pythia_J0 105009
    run pythia_J1 105010
    run pythia_J2 105011
    run pythia_J3 105012
    run pythia_J4 105013
    run pythia_J5 105014
    run pythia_J6 105015
fi

# diboson alpgen
if [ "0" -eq "1" ]; then
    run alpgen_herwig_ww_np0 107100
    run alpgen_herwig_ww_np1 107101
    run alpgen_herwig_ww_np2 107102
    run alpgen_herwig_ww_np3 107103
    run alpgen_herwig_wz_np0 107104
    run alpgen_herwig_wz_np1 107105
    run alpgen_herwig_wz_np2 107106
    run alpgen_herwig_wz_np3 107107
    run alpgen_herwig_zz_np0 107108
    run alpgen_herwig_zz_np1 107109
    run alpgen_herwig_zz_np2 107110
    run alpgen_herwig_zz_np3 107111
fi;

# diboson pythia and herwig
if [ "1" -eq "1" ]; then
    run herwig_ww 105985
    run herwig_zz 105986
    run herwig_wz 105987
    #run pythia_ww 128970
    #run pythia_zz 128971
    #run pythia_wz 128972
fi;

# single top
if [ "1" -eq "1" ]; then
#     run acer_schan_munu 117364
#     run acer_schan_taunu 117365
#     run acer_tchan_munu 117361
#     run acer_tchan_taunu 117362
#     run acer_wt 105500
    # MC@NLO version
    run mcnlo_schan_munu 108344
    run mcnlo_schan_taunu 108345
    run mcnlo_tchan_munu 108341
    run mcnlo_tchan_taunu 108342
    run mcnlo_wt 108346
fi

# WW signal
if [ "0" -eq "1" ]; then
    run mcnlo_WpWm_enumunu 105922
    run mcnlo_WpWm_enutaunu 105923
    run mcnlo_WpWm_munumunu 105924
    run mcnlo_WpWm_munuenu 105925
    run mcnlo_WpWm_munutaunu 105926
    run mcnlo_WpWm_taunutaunu 105927
    run mcnlo_WpWm_taunuenu 105928
    run mcnlo_WpWm_taunumunu 105929
fi;

# clean up empty files
for f in *; do 
    if [ ! -s $f ]; then 
	rm $f; 
    fi; 
done
