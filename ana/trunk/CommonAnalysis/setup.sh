#!/bin/bash

#RootCore: atlasoff/PhysicsAnalysis/D3PDTools/RootCore/trunk
# atlasinst/Institutes/CERN/ElectroweakBosons/trunk/RootCore/ScaleFactorProvider

# selects a snapshot of tags. Use "packages_tags.txt" to use the latest available tags.
packages=packages.txt
if [ "$#" -ge "2" ]; then
    packages=$2
fi

if [ -d RootCore ]; then
    cd RootCore;
    if [ "$1" ==  "configure" ]; then
	./configure
    fi
    source scripts/setup.sh
    cd ..
else
    echo "Cannot find RootCore/ in current directory"
    exit 1
fi;

unset SVNROOT
export CERN_USER=$USER
export ROOTCORE_NCPUS=16
export ROOTCORE_VERBOSE=1

# Feel free to add yourself if your CERN username differs from UCT3 username
if [ "${CERN_USER}" == "antonk" ]; then
    export CERN_USER=kapliy
elif [ "${CERN_USER}" == "hlli" ]; then
    export CERN_USER=hli
elif [ "${CERN_USER}" == "melachri" ]; then
    export CERN_USER=cmelachr
fi

# This is really stupid, but RootCore + TEnv cannot automatically handle the relative paths correctly.
# So, we need to fix up a few paths in JetUncertainties. This may need to be updated from time to time!
function fixup_paths() {
    if [ -f JetUncertainties/share/InsituJES2011_AllNuisanceParameters.config ]; then
	(cd JetUncertainties/share/ && svn revert InsituJES2011_AllNuisanceParameters.config)
	sed -i -e 's#JESUncertainty_Preliminary.root#CommonAnalysis/RootCore/data/JetUncertainties/JESUncertainty_Preliminary.root#g' JetUncertainties/share/InsituJES2011_AllNuisanceParameters.config
    fi
    if [ -f JetUncertainties/share/MultijetJES_Preliminary.config ]; then
	(cd JetUncertainties/share/ && svn revert MultijetJES_Preliminary.config)
	sed -i -e "s#MJESUncertainty_Preliminary.root#CommonAnalysis/RootCore/data/JetUncertainties/MJESUncertainty_Preliminary.root#g" -e 's#../analysisPlots#CommonAnalysis/JetUncertainties/analysisPlots#g' JetUncertainties/share/MultijetJES_Preliminary.config
    fi
    # and something similar for Final 2011 tag
    if [ -f JetUncertainties/share/JES_2011/Final/InsituJES2011_AllNuisanceParameters.config ]; then
	(cd JetUncertainties/share/JES_2011/Final && svn revert InsituJES2011_AllNuisanceParameters.config )
	sed -i -e 's#JES_2011/Final/JESUncertainty_2011.root#CommonAnalysis/RootCore/data/JetUncertainties/JES_2011/Final/JESUncertainty_2011.root#g' JetUncertainties/share/JES_2011/Final/InsituJES2011_AllNuisanceParameters.config
    fi
    if [ -f JetUncertainties/share/JES_2011/Final/MultijetJES_2011.config ]; then
	(cd JetUncertainties/share/JES_2011/Final && svn revert MultijetJES_2011.config )
	sed -i -e 's#JES_2011/Final/MJESUncertainty_2011.root#CommonAnalysis/RootCore/data/JetUncertainties/JES_2011/Final/MJESUncertainty_2011.root#g' -e 's#analysisInputs/UnknownFlavourComp.root#CommonAnalysis/RootCore/data/JetUncertainties/analysisInputs/UnknownFlavourComp.root#g' JetUncertainties/share/JES_2011/Final/MultijetJES_2011.config
    fi
}

if [ "$#" -lt "1" ]; then
    echo "Sourcing the RootCore environment "
    echo "ADDITIONAL COMMANDS: $0 [clean|checkout|update|compile|build]"
elif [ "$1" ==  "clean" ]; then
    $ROOTCOREDIR/scripts/clean.sh ${packages}
    rm -f RootCore.par
elif [ "$1" ==  "cleanall" ]; then
    $ROOTCOREDIR/scripts/clean.sh ${packages}
    find . -maxdepth 1 -mindepth 1 -type d \( ! -name RootCore -a ! -name .svn \) | xargs rm -rf
    rm -f RootCore.par
elif [ "$1" ==  "compact" ]; then
    rm -f RootCore.par
    find . -type f -name \*.a | xargs rm -f
    find . -type f -name \*.o | xargs rm -f
elif [ "$1" ==  "checkout" -o "$1" == "co" ]; then
    $ROOTCOREDIR/scripts/checkout.sh ${packages}
    fixup_paths
elif [ "$1" ==  "link" ]; then
    # re-creates all symlinks - useful for condor, since it lets us avoid recompilation
    $ROOTCOREDIR/scripts/link.sh ${packages}
elif [ "$1" ==  "update" -o "$1" == "up" ]; then
    $ROOTCOREDIR/scripts/update.sh ${packages}
    fixup_paths
elif [ "$1" ==  "compile" -o "$1" == "make" -o "$1" == "gmake" -o "$1" ==  "build" -o "$1" == "all" ]; then
    $ROOTCOREDIR/scripts/build.sh ${packages}
    fixup_paths
    cd $ROOTCOREDIR/lib && rlibmap -f -o libBootstrapGenerator.rootmap -l libBootstrapGenerator.so -c ../../BootstrapGenerator/Root/LinkDef.h && echo "Info: created libBootstrapGenerator.rootmap"
elif [ "$1" ==  "status" -o "$1" == "st" ]; then
    $ROOTCOREDIR/scripts/status.sh ${packages}
fi
