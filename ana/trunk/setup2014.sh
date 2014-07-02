ROOTDIR=$HOME
LOCDIR=results/ana_wasym
RESHOST=root://uct3-xrd.mwt2.org/
export X509_USER_PROXY=${ROOTDIR}/tmp.proxy

export uct3_64=1
export ROOTCORE_NCPUS=24


source ${ROOTDIR}/TrigFTKAna/scripts/dependencies.sh &> /dev/null
export ROOTCOREDIR=${ROOTDIR}/TrigFTKAna/CommonAnalysis/RootCore
export ROOTCOREBIN=${ROOTDIR}/TrigFTKAna/CommonAnalysis/RootCore
#$ROOTCOREDIR/scripts/link.sh
echo $ROOTCOREDIR
export PATH=$ROOTCOREBIN/bin:$PATH
export LD_LIBRARY_PATH=$ROOTCOREBIN/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$ROOTCOREBIN/lib:$DYLD_LIBRARY_PATH
export PYTHONPATH=$ROOTCOREBIN/python:$PYTHONPATH
export LD_LIBRARY_PATH=${ROOTDIR}/TrigFTKAna/LHAPDF-6.1.3/LHAPDF/lib:${LD_LIBRARY_PATH}
#export LD_LIBRARY_PATH=${ROOTDIR}/TrigFTKAna/LIBXML/:${LD_LIBRARY_PATH}
