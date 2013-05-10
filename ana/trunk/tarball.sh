#!/bin/bash

# reduce the size of TrigFTKAna and make a tarball

ROOTDIR=$PWD

function compact() {
    rm -f libxml2-2.8.0.tar.bz2
    rm -rf lhapdf-5.8.8/{src,PDFsets.index,pyext,m4,libtool,include,examples,aclocal.m4,ccwrap,config}
    rm -rf autom4te.cache
    rm -f CommonAnalysis/RootCore.par
    rm -rf results result
    find . -type f -name \*.a | xargs rm -f
    find . -type f -name \*.o | xargs rm -f
    find . -type f -name \*.d | xargs rm -f
    find . -maxdepth 1 -mindepth 1 -type f -name \*root\* | xargs rm -f
    #strip ana/ana_* 2>/dev/null
}

bname=`basename $PWD`
if [ "${bname}" != "TrigFTKAna" ]; then
    echo "ERROR: must be inside TrigFTKAna directory"
    exit 1
fi

tmp=/tmp/antonk/TAR
rm -rf ${tmp}
mkdir -p ${tmp}

echo "Starting tarballing: `date`"
cp -a ../TrigFTKAna ${tmp}
cd ${tmp}/TrigFTKAna

if [ "$PWD" != "/tmp/antonk/TAR/TrigFTKAna" ]; then
    echo "BAD BAD BAD, something is not working"
fi

compact

if [ ! -s ana/ana_wasym ]; then
    echo "ERROR: cannot find ana/ana_wasym"
    exit 1
fi

cd ..
tar cfj TrigFTKAna.tar.bz2 TrigFTKAna
#rm -rf TrigFTKAna

ls -lsh $PWD/TrigFTKAna.tar.bz2
echo "Finished tarballing: `date`"
echo $PWD/TrigFTKAna.tar.bz2

cd ${ROOTDIR}
