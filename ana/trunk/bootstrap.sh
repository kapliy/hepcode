#!/bin/bash

# ATTENTION: set TRIGFTKANA_FTKSIM_DIR environment variable if you want
# to link libftk_classes.so from TrigFTKSim

# When this script is sourced without any additional arguments,
# it will set up a base version of TrigFTKAna *without* external
# RootCore packages under CommonAnalysis (good for ana_standard_overview).
# If you need these external RootCore packages (eg, for ana_wzjets),
# you need to source this script like this: "source bootstrap.sh 1"
# HINT: use "source bootstrap.sh 2" to force recompilation of ALL dependencies.

# default to 32-bit build
if [ -z $uct3_64 ]; then
    export uct3_64=0
fi
# Antonio's mac build
if [ -z $macosx_64 ]; then
    export macosx_64=0
    if [ "`file /bin/bash | grep x86_64 | cut -b 29-34`" == "x86_64" ]; then
	echo Building 64-bit on macosx
	export macosx_64=1
    fi
fi

mflagcheck=m32
if [ "${uct3_64}" -eq 1 -o "${macosx_64}" -eq 1 ]; then
    mflagcheck=m64
fi

# if the user forgot to source scripts/dependencies.sh:
if [ -z "${TRIGFTKANA_BOOST_DIR}" ]; then
    source scripts/dependencies.sh 1
fi

# YOU CAN HARDCODE TrigFTKSim LOCATION HERE:
#export TRIGFTKANA_FTKSIM_DIR=""
# This bit must be set manually before calling bootstrap if TrigFTKSim
# is built in 64bit mode.
# if [ -z "${TRIGFTKANA_FTKSIM_DIR}" ]; then
#     TRIGFTKANA_FTKSIM_DIR=${PWD}/../TrigFTKSim
# fi
#export TRIGFTKANA_FTKSIM_DIR=

CONFIGFTKSIM=""
if [ -n "${TRIGFTKANA_FTKSIM_DIR}" ]; then
    CONFIGFTKSIM="--with-ftksim=${TRIGFTKANA_FTKSIM_DIR} "
fi

if [ ! -d "${TRIGFTKANA_FTKSIM_DIR}" ]; then
    echo "Building without TrigFTKSim directory."
elif [ ! -f "${TRIGFTKANA_FTKSIM_DIR}/standalone/libftk_classes.so" ]; then
    echo "WARNING: [${TRIGFTKANA_FTKSIM_DIR}/standalone/libftk_classes.so] does not exist. Linking will fail."
    exit 1
else
    nbits=`file ${TRIGFTKANA_FTKSIM_DIR}/standalone/libftk_classes.so | awk '{print $3}' | cut -d '-' -f 1`
    if [ ${uct3_64} -eq 0 ]; then
	if [ ${nbits} -ne 32 ]; then
	    echo "WARNING: [${TRIGFTKANA_FTKSIM_DIR}/standalone/libftk_classes.so] is not a 32-bit library. Linking will fail."
	    exit 1
	fi
    else
	if [ ${nbits} -ne 64 ]; then
	    echo "WARNING: [${TRIGFTKANA_FTKSIM_DIR}/standalone/libftk_classes.so] is not a 64-bit library. Linking will fail. You need to manually set '${TRIGFTKANA_FTKSIM_DIR}' to a 64-bit build before calling bootstrap.sh."
	    exit 1
	fi
    fi
fi

function symlink_lib() {
    sofile=$1
    target=`basename ${sofile}`
    if [ ! -f ${sofile} ]; then
	if [ -f ${sofile}.0 ]; then
	    ln -fs ${sofile}.0 ${target}
	elif [ -f ${sofile}.1 ]; then
	    ln -fs ${sofile}.1 ${target}
	elif [ -f ${sofile}.2 ]; then
	    ln -fs ${sofile}.2 ${target}
	else
	    echo "ERROR: cannot locate ANY version of this library: ${sofile}"
	    exit 1
	fi
    fi
}

echo "Symlinking libg2c.so and libz.so ..."
# libg2c and libz.so have to end with .so, not .so.0 or .so.1
rm -f libg2c.so libz.so
lib=lib
if [ ${uct3_64} -eq 1 ]; then lib=lib64; fi
#symlink_lib /usr/${lib}/libg2c.so
symlink_lib /usr/${lib}/libz.so

#echo "Building libfrtbegin.a..."
rm -f libfrtbegin.a deps/frtbegin.o
#make -C deps/

echo "List of local libraries:"
ls -l *.{a,so} 2>/dev/null

if [ -f /usr/share/libtool/ltmain.sh ]; then 
    ln -fs /usr/share/libtool/ltmain.sh .
fi
# gcc -dynamic for libtool

# if bootstrap.sh was called with a command line argument,
# TrigFTKAna will be built with all external RootCore dependencies
# (under CommonAnalysis directory)
if [ "$#" -ge "1" ]; then
    # RooUnfold
    if [ 1 -eq 0 -a -d RooUnfold ]; then  # disabled now
	cd RooUnfold
	echo "Attempting to compile RooUnfold..."
	if [ "$1" == "2" ]; then
	    echo "Re-compiling RooUnfold packages because bootstrap.sh was run via: bootstrap.sh $1"
	    make clean
	fi;
	make -j10 
	if [ "$?" -eq "0" ]; then
	    echo "Successfully compiled RooUnfold package!"
	    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$PWD
	fi
	cd ../
    fi
    # LHAPDF
    LHAPDF="lhapdf-5.8.8"
    if [ -d ${LHAPDF} ]; then
	cd ${LHAPDF}
	if [ "$1" == "2" ]; then
	    echo "Attempting to compile ${LHAPDF}..."
	    echo "Re-compiling ${LHAPDF} packages because bootstrap.sh was run via: bootstrap.sh $1"
	    ./compile.sh
	fi;
	if [ "$?" -eq "0" ]; then
	    echo "Successfully compiled ${LHAPDF} package!"
	    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:$PWD/LHAPDF
	fi
	cd ../
    fi
    # RootCore stuff
    if [ -d CommonAnalysis ]; then
	cd CommonAnalysis
        # prepare RootCore itself
	if [ -d RootCore ]; then
	    cd RootCore
	    # configure the first time
	    if [ ! -s config.log ]; then
		echo "Configuring RootCore for the first time..."
		./configure 
	    # or it RootCore directory was changed since last configuration
	    else
		OLDDIR=`grep ROOT_CORE_DIR config.log | awk 'BEGIN{FS="="}{print $2}' | sed -e "s#'##g"`
		if [ "${OLDDIR}" != "$PWD" ]; then
		    echo "Re-configuring RootCore in a new folder ($OLDDIR -> $PWD)"
		    ./configure
		else
		    echo "RootCore is already configured; reading settings from: ${OLDDIR}"
		fi
	    fi
	    unset ROOTCOREBIN
	    source scripts/setup.sh
	    cd ../
	    # update packages list under RootCore/packages
	    $ROOTCOREDIR/scripts/find_packages.sh
	fi
        # check out and build all RootCore packages
	./setup.sh configure
	if [ "$1" == "2" ]; then
	    echo "Re-compiling RootCore packages because bootstrap.sh was run via: bootstrap.sh $1"
	    ./setup.sh clean
	fi
	./setup.sh build
	#./setup.sh compact
	cd ../
    fi
    # libXML (only for special releases)
    LIBXML=libxml2-2.8.0
    if [ "$1" == "2" -a "${AtlasVersion}" == "17.6.0.1" -a -f "${LIBXML}.tar.bz2" ]; then
	echo "(Re)compiling libXML2..."
	rm -rf LIBXML
	tar xfj ${LIBXML}.tar.bz2
	if [ -d LIBXML ]; then
	    cd LIBXML
	    ./compile.sh
	    if [ "$?" != "0" ]; then
		echo "ERROR: failed to recompile libxml2"
		exit 1
	    fi
	    if [ ! -f LIBXML/lib/libxml2.so ]; then
		echo "ERROR: cannot locate LIBXML/lib/libxml2.so"
		exit 1
	    fi
	    cp LIBXML/lib/libxml2.so libxml2.so.2
	    rm -rf LIBXML ${LIBXML} compile.sh
	    cd -
	fi
    fi
fi

aclocal -I ./m4
autoconf
touch NEWS AUTHORS ChangeLog README
autoheader
automake -a
# variables set in setup.sh
if [ ${uct3_64} -eq 1 ]; then
    echo ./configure --with-boost=${TRIGFTKANA_BOOST_DIR} --with-dcap=${TRIGFTKANA_DCAP_DIR} ${CONFIGFTKSIM} --with-rootsys=${ROOTSYS} --enable-64-bit
    ./configure --with-boost=${TRIGFTKANA_BOOST_DIR} --with-dcap=${TRIGFTKANA_DCAP_DIR} ${CONFIGFTKSIM} --with-rootsys=${ROOTSYS} --enable-64-bit
elif [ ${macosx_64} -eq 1 ]; then
    CXX=g++-4.2 ./configure --with-boost=${TRIGFTKANA_BOOST_DIR} --with-rootsys=${ROOTSYS} --enable-64-bit
else
    echo ./configure --with-boost=${TRIGFTKANA_BOOST_DIR} --with-dcap=${TRIGFTKANA_DCAP_DIR} ${CONFIGFTKSIM} --with-rootsys=${ROOTSYS}
    ./configure --with-boost=${TRIGFTKANA_BOOST_DIR} --with-dcap=${TRIGFTKANA_DCAP_DIR} ${CONFIGFTKSIM} --with-rootsys=${ROOTSYS}
fi
