# This script sets BOOST_LDFLAGS,BOOST_CXXFLAGS,DCAP_LDFLAGS,DCAL_CXXFLAGS from athena environment
# Note that you must have sourced athena setup.sh -tag=whatever, so that SITEROOT is defined!
# VERIFIED TO WORK WITH ALL ATHENA VERSIONS BETWEEN 13.0.40 and 15.6.1

# Make sure athena environment has been bootstrapped
if [ -z $uct3_64 ]; then 
    uct3_64=0
    if [ "`hostname | cut -d '.' -f 1 | cut -d '-' -f 1`" == "uct3" ] && [ "`uname -m`" == "x86_64" ]; then
	echo Building 64-bit on uct3
	uct3_64=1
    elif [ -n "$SITEROOT" ]; then
	echo "SITEROOT is $SITEROOT"
    else
	echo "SITEROOT (root of athena installation) is not set on this node. Exiting..."
	if [ "$#" -gt "0" ]; then exit 2; fi;
    fi
fi

# Make sure dcache is present and export compiler flags
if [ ${uct3_64} -ne 1 ]; then
    echo "Trying to find libdcap shipped with athena:"
    export DCAPDIR=`cd $SITEROOT/sw/lcg/external/dcache_client/*/*/dcap/ && pwd`
else
    export DCAPDIR=/opt/d-cache/dcap
fi
DCAP_INCDIR="${DCAPDIR}/include"
export DCAP_CXXFLAGS="-I${DCAP_INCDIR}"
if [ ${uct3_64} -ne 1 ]; then
    DCAP_LDDIR="${DCAPDIR}/lib"
else
    DCAP_LDDIR="${DCAPDIR}/lib64"
fi
export DCAP_LDPRELOAD_ATH="${DCAP_LDDIR}/libpdcap.so"  # usually won't work
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${DCAP_LDDIR}
export DCAP_LDFLAGS="-L${DCAP_LDDIR} -ldcap"
if [ -s "${DCAP_INCDIR}/dcap.h" ]; then
    echo "Good, libdcap is found:"
    echo "DCAP_CXXFLAGS = ${DCAP_CXXFLAGS}"
    echo "DCAP_LDFLAGS = ${DCAP_LDFLAGS}"
    echo "DCAP_LDPRELOAD_ATH = ${DCAP_LDPRELOAD_ATH}"
else
    echo "OOPS, cannot get libdcap. Something is very wrong with this node's setup"
    if [ "$#" -gt "0" ]; then exit 2; fi;
fi;

# Make sure boost c++ is present and export compiler flags
if [ ${uct3_64} -ne 1 ]; then
    echo "Trying to find boost libraries shipped with athena:"
    export BOOSTDIR=`cd $SITEROOT/sw/lcg/external/Boost/*/*/ && pwd`
else
    export BOOSTDIR=/share/root/boost_1_39_0-x86_64-SLC5_gcc34
fi
BOOST_INCDIR=`cd ${BOOSTDIR}/include/boost-*/ && pwd`
export BOOST_CXXFLAGS="-I${BOOST_INCDIR}"
export BOOST_VER=`echo ${BOOST_CXXFLAGS} | awk 'BEGIN{FS="-"}; { print $NF; }'`
BOOST_LDDIR="${BOOSTDIR}/lib"
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${BOOST_LDDIR}
BOOST_IOLIB=`cd ${BOOST_LDDIR}; /bin/ls libboost_iostreams-gcc*-mt-1*.so | sed -e 's#.so##' -e 's#lib##'`
BOOST_PRLIB=`cd ${BOOST_LDDIR}; /bin/ls libboost_program_options-gcc*-mt-1*.so | sed -e 's#.so##' -e 's#lib##'`
BOOST_FSLIB=`cd ${BOOST_LDDIR}; /bin/ls libboost_filesystem-gcc*-mt-1*.so | sed -e 's#.so##' -e 's#lib##'`
export BOOST_LDFLAGS="-L${BOOST_LDDIR} -l${BOOST_IOLIB} -l${BOOST_PRLIB} -l${BOOST_FSLIB}"
if [ -s "${BOOST_INCDIR}/boost/iostreams/operations.hpp" -a -n "${BOOST_IOLIB}" -a -n "${BOOST_PRLIB}" -a -n "${BOOST_FSLIB}" ]; then
    echo "Good, boost version ${BOOST_VER} is found:"
    echo "BOOST_CXXFLAGS = ${BOOST_CXXFLAGS}"
    echo "BOOST_LDFLAGS = ${BOOST_LDFLAGS}"
else
    echo "OOPS, cannot find boost. Something is very wrong with this node's setup"
    if [ "$#" -gt "0" ]; then exit 2; fi;
fi;

# Alias to variables used in TrigFTKAna
export TRIGFTKANA_BOOST_DIR=${BOOSTDIR}
export TRIGFTKANA_DCAP_DIR=${DCAPDIR}

# Set up ROOT 
if [ ${uct3_64} -eq 1 ]; then
    ROOTSYS=/share/root/root-5.22.00f-x86_64-slc5-gcc34-dbg
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$ROOTSYS/lib
    export PATH=${PATH}:$ROOTSYS/bin
fi

export uct3_64=${uct3_64}
