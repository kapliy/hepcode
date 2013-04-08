#!/bin/bash

export TRIGFTKANA_DIR=`pwd`
# cd /share/ftkdata1/boveia/testana/17.0.1
# source ./setup_environment.sh
# eval $(perl -I/share/ftkdata/boveia/perl/lib/perl5 -Mlocal::lib=/share/ftkdata/boveia/perl)
cd $TRIGFTKANA_DIR
export uct3_64=2
export no_trigftksim=1
export DCACHEDIR=/opt/d-cache
export DCAPBINDIR=${DCACHEDIR}/dcap/bin
export PATH=${DCAPBINDIR}:$PATH
export SRMBINDIR=${DCACHEDIR}/srm/bin
export PATH=${SRMBINDIR}:$PATH
export LD_LIBRARY_PATH=${DCACHEDIR}/dcap/lib:${DCACHEDIR}/srm/lib:$LD_LIBRARY_PATH

export SITEROOT=/nfs/osg/app/atlas_app/atlas_rel/17.0.1
export ROOTSYS=/osg/app/atlas_app/atlas_rel/17.0.1/sw/lcg/app/releases/ROOT/5.28.00b/i686-slc5-gcc43-opt/root
export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
export PATH=$ROOTSYS/bin:$PATH

# start by sourcing environment to allow for pbs submission
if [ -z "${SITEROOT}" ]; then
    export SITEROOT=/nfs/osg/app/atlas_app/atlas_rel/17.0.1
fi
if [ ! -d "$ROOTSYS" ]; then
    export ROOTSYS=/osg/app/atlas_app/atlas_rel/17.0.1/sw/lcg/app/releases/ROOT/5.28.00b/i686-slc5-gcc43-opt/root
fi
echo Using ROOTSYS=${ROOTSYS}

export LD_LIBRARY_PATH=$ROOTSYS/lib:$LD_LIBRARY_PATH
export PATH=$ROOTSYS/bin:$PATH


if [ -z "${uct3_64}" ]; then
    export uct3_64=0
    if [ "`hostname | cut -d '.' -f 1 | cut -d '-' -f 1`" == "uct3" ] && [ "`uname -m`" == "x86_64" ]; then
	    export uct3_64=1
    fi
fi
BOOST_FROM_LD_LIBRARY=$(for qwe in `echo $LD_LIBRARY_PATH | sed -e "s,:, ,g" `; do echo $qwe | grep -i boost; done | head -n 1 | sed -e "s,lib/\?$,,") 
DCAP_FROM_LD_LIBRARY=$(for qwe in `echo $LD_LIBRARY_PATH | sed -e "s,:, ,g" `; do echo $qwe | grep -i dcap; done | head -n 1 | sed -e "s,lib/\?$,,") 
if( [ -e "${SITEROOT}" ] ); then
    export TRIGFTKANA_BOOST_DIR=$(for qwe in `echo ${SITEROOT}/sw/lcg/external/Boost/*/*/ `; do echo $qwe; done | head -n 1)
    export TRIGFTKANA_BOOST_LIB_DIR=${TRIGFTKANA_BOOST_DIR}/lib
    export TRIGFTKANA_BOOST_INC_DIR=$(for qwe in `echo ${TRIGFTKANA_BOOST_DIR}/include/boost*`; do echo $qwe; done | head -n 1)
elif( [ -e "${BOOSTDIR}" ] ); then
    export TRIGFTKANA_BOOST_DIR=${BOOSTDIR}
    export TRIGFTKANA_BOOST_LIB_DIR=${TRIGFTKANA_BOOST_DIR}/lib
    export TRIGFTKANA_BOOST_INC_DIR=${TRIGFTKANA_BOOST_DIR}/include
elif [ "$BOOST_FROM_LD_LIBRARY" ] && [ $BoostVERS ]; then
    candidatedir=${TRIGFTKANA_BOOST_DIR}/include/boost-`echo $BoostVERS | sed -e "s,\.,_," | sed -e "s,\..*,,"`
    # this fails if BoostVERS is not defined
    if [ -e "$candidatedir" ]; then 
        export TRIGFTKANA_BOOST_DIR=$BOOST_FROM_LD_LIBRARY;
        export TRIGFTKANA_BOOST_LIB_DIR=${TRIGFTKANA_BOOST_DIR}/lib
        export TRIGFTKANA_BOOST_INC_DIR=$candidatedir
    fi
elif( [ -e "/usr/local/boost" ] ); then
    export TRIGFTKANA_BOOST_DIR=/usr/local/boost/
    export TRIGFTKANA_BOOST_LIB_DIR=/usr/local/boost/lib
    export TRIGFTKANA_BOOST_INC_DIR=/usr/local/boost/include
elif( [ -e "${HOME}/boost" ] ); then
    export TRIGFTKANA_BOOST_DIR=${HOME}/boost/
    export TRIGFTKANA_BOOST_LIB_DIR=${HOME}/boost/lib
    export TRIGFTKANA_BOOST_INC_DIR=${HOME}/boost/include-1.42/boost-1_44
else
    echo "Warning: could not figure out path to boost libraries! default to /osg and hope for the best..."
    export TRIGFTKANA_BOOST_DIR=/osg/app/atlas_app/atlas_rel/17.0.1/sw/lcg/external/Boost/1.44.0_python2.6/i686-slc5-gcc43-opt/
    export TRIGFTKANA_BOOST_LIB_DIR=${TRIGFTKANA_BOOST_DIR}/lib
    export TRIGFTKANA_BOOST_INC_DIR=${TRIGFTKANA_BOOST_DIR}/include/boost-1_44
fi
echo TRIGFTKANA_BOOST_INC_DIR=$TRIGFTKANA_BOOST_INC_DIR;
echo TRIGFTKANA_BOOST_LIB_DIR=$TRIGFTKANA_BOOST_LIB_DIR;
if [ -e "${SITEROOT}" ]; then
    export TRIGFTKANA_DCAP_DIR=$(for qwe in `echo ${SITEROOT}/sw/lcg/external/dcache_client/*/*/dcap `; do echo $qwe; done | head -n 1)
elif [ "$DCAP_FROM_LD_LIBRARY" ]; then
    export TRIGFTKANA_DCAP_DIR=$DCAP_FROM_LD_LIBRARY;
else
    export TRIGFTKANA_DCAP_DIR=/opt/d-cache/dcap
fi
if [ ! -d ${TRIGFTKANA_DCAP_DIR} ]; then
    export TRIGFTKANA_DCAP_DIR=
fi
echo TRIGFTKANA_DCAP_DIR=$TRIGFTKANA_DCAP_DIR;
if [ ! -z no_trigftksim ]; then
    if [ ${uct3_64} -ne 1 ]; then
        export TRIGFTKANA_FTKSIM_DIR=${PWD}/../TrigFTKSim
    else
        export TRIGFTKANA_FTKSIM_DIR=${PWD}/../TrigFTKSim_64bit
    fi
fi
if [ -z "$ROOTSYS" ]; then
  export ROOTSYS=/osg/app/atlas_app/atlas_rel/17.0.1/sw/lcg/app/releases/ROOT/5.28.00b/i686-slc5-gcc43-opt/root
fi
if [ -z "$ROOT_QT_DIR" ]; then
  export ROOT_QT_DIR=/osg/app/atlas_app/atlas_rel/17.0.1/sw/lcg/app/releases/ROOT/5.28.00b/i686-slc5-gcc43-opt/root/lib/
fi
if [ -z "$PLPLOT_HOME" ]; then
    if [ -d "/share/ftkdata/boveia/plplot" ]; then
        export PLPLOT_HOME=/share/ftkdata/boveia/plplot
        export PATH=${PLPLOT_HOME}/bin:${PATH}
    fi
fi
if [ -z "$PLPLOT_LIB" ]; then
    if [ -d "/share/ftkdata/boveia/plplot/lib" ]; then
        export PLPLOT_LIB=/share/ftkdata/boveia/plplot/lib
    fi
fi
libdir=lib
if [ ${uct3_64} -eq 1 ]; then
    libdir=lib64
fi
export LD_LIBRARY_PATH=${TRIGFTKANA_BOOST_DIR}/lib:${TRIGFTKANA_DCAP_DIR}/${libdir}:${TRIGFTKANA_FTKSIM_DIR}/standalone:${PLPLOT_LIB}:$ROOTSYS/lib:$ROOT_QT_DIR:${LD_LIBRARY_PATH}
export PATH=`pwd`/ana:`pwd`/test:`pwd`/plot:$ROOTSYS:$ROOTSYS/bin:${PATH}
