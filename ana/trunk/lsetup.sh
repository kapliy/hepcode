athver=17.6.0.1
athver=19.0.2.1
export uct3_64=1

export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
echo "Setting up ${mbits}-bit release ${athver} from: ${atlasrepo}"
athbase=`echo $athver | awk 'BEGIN{FS="."}; NF==3{print $0}; NF==4{printf("%d.%d.%d\n",$1,$2,$3)}'`
workdir=$HOME/work/${mbits}/$athver
mkdir -p $workdir
pushd $workdir
asetup ${athver},64,here #,slc5
export XrdSecGSISRVNAMES=uct2-s5.uchicago.edu\|uct2-s6.uchicago.edu\|uct2-s20.uchicago.edu
export XrdSecGSISRVNAMES='uct2-*.uchicago.edu'
#localSetupEmi
popd
