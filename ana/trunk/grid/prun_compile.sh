#!/bin/bash

ROOTDIR=`pwd`
skip_compile=$1
export uct3_64=0

# extract the tarball prepared by the build job
if [ -n "${skip_compile}" ]; then
    if [ -s "all_compiled_code.tbz2" ]; then
	echo "Extracting compiled TrigFTKAna tarball..."
	tar xfj all_compiled_code.tbz2
    else
	echo "ERROR: cannot find the tarball with compiled code:"
	ls -l ${ROOTDIR}
	echo "Build job failed?"
	exit 1
    fi
fi;

# source the grid environment
if [ -f $OSG_GRID/setup.sh ]; then 
    echo "Sourcing $OSG_GRID/setup.sh"
    source $OSG_GRID/setup.sh;
fi
if [ -f $OSG_APP/atlas_app/atlaswn/setup.sh ]; then
    echo "Sourcing $OSG_APP/atlas_app/atlaswn/setup.sh"
    source $OSG_APP/atlas_app/atlaswn/setup.sh
fi
if [ -f /share/wn-client/setup.sh ]; then
    echo "Sourcing /share/wn-client/setup.sh"
    source /share/wn-client/setup.sh;
fi
if [ -f /osg/app/atlas_app/atlaswn/setup.sh ]; then
    echo "Sourcing /osg/app/atlas_app/atlaswn/setup.sh"
    source /osg/app/atlas_app/atlaswn/setup.sh;
fi

echo "HOSTNAME = $HOSTNAME"
hostname -f
if [ "$OSG_APP" = "" -a -f /opt/exp_soft/atlas/ddm/latest/setup.sh ]; then
  echo setting up for lcg grid
  echo "Sourcing /opt/exp_soft/atlas/ddm/latest/setup.sh"
  source /opt/exp_soft/atlas/ddm/latest/setup.sh
fi 
if [ -f $VO_ATLAS_SW_DIR/local/setup.sh ]; then
    echo "Sourcing $VO_ATLAS_SW_DIR/local/setup.sh"
    echo "VO_ATLAS_SW_DIR=$VO_ATLAS_SW_DIR"
    source $VO_ATLAS_SW_DIR/local/setup.sh;
fi

echo "Memory summary:"
free -m
echo "DQ2_LOCAL_SITE_ID = $DQ2_LOCAL_SITE_ID"
if [ ! "$DQ2_LOCAL_SITE_ID" ]; then
    echo DQ2_LOCAL_SITE_ID not defined \(use prun_jobs.sh default\)
    DQ2_LOCAL_SITE_ID=ROAMING;
    echo "DQ2_LOCAL_SITE_ID = $DQ2_LOCAL_SITE_ID";
fi
echo "PandaSite = $PandaSite"
echo "Proxy variable X509_USER_PROXY: $X509_USER_PROXY"
echo "voms-proxy-info:"
voms-proxy-info -all 2>&1
echo "grid-proxy-info -all:"
grid-proxy-info -all 2>&1
echo "which python:"
which python
file -L $(which python)

if [ ! -n "${skip_compile}" ]; then
    echo "which dccp:"
    which dccp
    echo "which gcc"
    which gcc
    file -L $(which gcc)
    gcc --version
    echo "gcc search dirs:"
    gcc -print-search-dirs 2>&1
    echo "which g++"
    which g++
    file -L $(which g++)
    g++ --version
    echo "g++ search dirs:"
    g++ -print-search-dirs 2>&1
    echo "which dq2-get:"
    which dq2-get
fi;

# SETTING UP DQ2 (this is non-trivial due to 32-bit vs 64-bit issues)
echo "Checking which version of python is able to import _lfc.so:"
echo "Trying default-environment python:"
PYTHON=$(which python);
# if import succeeds, nothing is done (i.e. we keep using default python)
# if import fails, we check if /usr/bin/python works instead
$PYTHON -c "import _lfc" &>python_error_msg || if [ 1 ]; then
    echo "WARNING: $PYTHON failed to import _lfc.so:"
    # we sed away ImportError, because PandaMonitor would report it as an actual error:
    sed -e 's#ImportError:#WARNING:#g' python_error_msg
    PYTHON=/usr/bin/python
    echo "Trying LFC import with $PYTHON..."
    $PYTHON -c "import _lfc"  2>&1 || if [ 1 ]; then 
	echo "WARNING: failed to import _lfc.so using either version of python"; 
	echo "WARNING: dq2-get will fail"
    fi;
fi;
# Wrapper function for dq2-get:
function DQ2_GET_CMD () {
    # see if -p dcap should be used:
    DQ2_FORCE_DCAP=`extractOption DQ2_FORCE_DCAP`
    local dq2_extras=""
    if [ "${DQ2_FORCE_DCAP}" == "" ]; then
	dq2_extras=""
    else
	dq2_extras="-p dcap"
    fi;
    # see if python path should be cleared of SITEROOT garbage
    OLDPYTHONPATH=$PYTHONPATH
    if [ "$PYTHON" == "/usr/bin/python" ]; then
	PYTHONPATH=`cleanedPythonpath`
    fi;
    time $PYTHON $(which dq2-get) ${dq2_extras} $@ 2>&1
    PYTHONPATH=$OLDPYTHONPATH
}
# Wrapper function for dq2-ls:
function DQ2_LS_CMD () {
    # see if python path should be cleared of SITEROOT garbage
    OLDPYTHONPATH=$PYTHONPATH
    if [ "$PYTHON" == "/usr/bin/python" ]; then
	PYTHONPATH=`cleanedPythonpath`
    fi;
    $PYTHON $(which dq2-ls) $@
    PYTHONPATH=$OLDPYTHONPATH
}

# Make sure autotools are available
echo "Looking for aclocal:"
aclocal_exec=`which aclocal`
if [ -n ${aclocal_exec} ] && `test -s "${aclocal_exec}"`; then
    echo "Good, aclocal found"
else
    echo "ERROR: Oops, this computer node doesn't have aclocal (and probably the entire GNU autotools). Exiting..."
    exit 1;
fi;

# Make sure ROOT is present
if [ -n "${ROOTSYS}" ]; then
    echo "Root variable ROOTSYS: $ROOTSYS"
else
    echo "ROOTSYS is not set on this node. Exiting..."
    exit 1;
fi

# In case TrigFTK* packages have already been compiled (e.g, in the build job),
# allow to skip these steps:
if [ -n "${skip_compile}" ]; then
    echo "Using pre-compiled TrigFTK* packages"
    cd TrigFTKAna/grid && source ./dependencies.sh 1 && cd -
else
    # Retrieving and compiling TrigFTKSim (if present)
    if [ -s TrigFTKSim.tar.bz2 ]; then
	echo "Extracting your version of TrigFTKSim executable"
	tar xfj TrigFTKSim.tar.bz2
	if [ -s ${ROOTDIR}/TrigFTKSim/config/map_file/raw_11L.pmap ]; then
	    echo "Good, config files and maps were extracted successfully"
	else
	    echo "ERROR: Cannot find map_file/raw_11L.pmap "
	    find ${ROOTDIR}/TrigFTKSim/config/map_file/
	    exit 1
	fi;
	if [ -s ${ROOTDIR}/TrigFTKSim/scripts/ftk.py ]; then
	    echo "Good, run scripts and templates were extracted successfully"
	else
	    echo "ERROR: Cannot find ftk.py "
	    find ${ROOTDIR}/TrigFTKSim/scripts/
	    exit 1
	fi;
	cd TrigFTKSim/standalone
	if [ -s bootstrap.sh ]; then
	    echo "Ready to compile in standalone/ directory"
	else
	    echo "ERROR: cannot find bootstrap.sh script in ${PWD}"
	    exit 1
	fi;
	source ./bootstrap.sh
	gmake -f Makefile.grid.mk clean 2>&1
	gmake -f Makefile.grid.mk 2>&1
	gmake -f Makefile.grid.mk compact 2>&1
	ls
	cd ${ROOTDIR}
	for app in TrigFTKSim/standalone/{road_finder,road_merger,track_fitter,track_merger,patmerge,libftk_classes.so}; do
	    if [ -s ${app} ]; then
		echo "Good, executable `basename ${app}` was created in ${app}"
		ldd ${app}
	    else
		echo "ERROR: cannot find executable `basename ${app}`"
		echo "Directory listing:"
		ls `dirname ${app}`
		echo "ldconfig:"
		/sbin/ldconfig -p
		exit 1
	    fi;
	done
	echo "DONE COMPILING TrigFTKSim EXECUTABLES"
    fi;

    # Retrieving and compiling TrigFTKAna (if present)
    cd ${ROOTDIR}
    if [ -s TrigFTKAna.tar.bz2 ]; then
	echo "Extracting your version of TrigFTKAna executable"
	tar xfj TrigFTKAna.tar.bz2
	cd TrigFTKAna/
	if [ -s bootstrap.sh ]; then
	    echo "Ready to compile TrigFTKAna"
	else
	    echo "ERROR: cannot find bootstrap.sh script in ${PWD}"
	    exit 1
	fi;
	source ./grid/dependencies.sh 1
	sed -i -e 's#1.39.0#1.42.0#g' configure.ac
	sh ./bootstrap.sh
	gmake clean 2>&1
        # only compile what we need:
	gmake -j4
	# strip all binary files (to save space)
	find . -type f -exec strip {} \; 2>/dev/null
	ls
	cd ${ROOTDIR}
	cd TrigFTKAna/ana/
	nfound=0
	for app in ana_wjets ana_zjets ana_wzjets ana_top ana_wasym ana_example ana_standard_overview ana_b_tagging ana_pgs ana_vtx ana_hww ana_user; do
	    if [ -s ${app} ]; then
		echo "Good, executable `basename ${app}` was created in ${app}"
		((nfound++));
	    else
		echo "WARNING: cannot find executable `basename ${app}`"
		ls `dirname ${app}`
	    fi;
	done
	cd ${ROOTDIR}
	if [ "$nfound" == "0" ]; then
	    echo "ERROR: no executable compiled successfully"
	    exit 1
	fi;
	echo "DONE COMPILING TrigFTKAna EXECUTABLES"
    else
	echo "WARNING: can't find TrigFTKAna.tar.bz2. Analysis jobs will surely fail."
    fi;
fi;

# tar up compiled code in the end of the build job (to reduce log verbosity)
# alternatively, untar it in the beginning of analysis jobs
if [ ! -n "${skip_compile}" ]; then
    cd ${ROOTDIR}
    tar cfj all_compiled_code.tbz2 TrigFTK*
    rm -rf TrigFTK*
    echo "Final tarball with compiled code:"
    ls -l all_compiled_code.tbz2
fi;
