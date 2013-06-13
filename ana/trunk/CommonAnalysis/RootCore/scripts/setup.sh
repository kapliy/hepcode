if test "x$ROOTSYS" = "x"
then
    export ROOTSYS=`root-config --prefix`
    test $? -ne 0 && echo no valid root version found, please set up root && return 1
fi

test "x$ROOTSYS" != "x/cvmfs/atlas.cern.ch/repo/sw/software/x86_64-slc5-gcc43-opt/17.6.0/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc43-opt/root" && echo ROOTSYS=$ROOTSYS not /cvmfs/atlas.cern.ch/repo/sw/software/x86_64-slc5-gcc43-opt/17.6.0/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc43-opt/root && echo please set up correct root version or follow the instructions at && echo \ \ https://twiki.cern.ch/twiki/bin/viewauth/Atlas/RootCore#If_Something_Goes_Wrong && return 1

export ROOTCOREDIR=/home/antonk/TrigFTKAna/CommonAnalysis/RootCore
test "x$ROOTCOREBIN" = "x" && export ROOTCOREBIN=/home/antonk/TrigFTKAna/CommonAnalysis/RootCore
test \! -d "$ROOTCOREBIN" && echo output directory not found, please create it && echo "  mkdir $ROOTCOREBIN" && return 1

export PATH=$ROOTCOREBIN/bin:$PATH
export LD_LIBRARY_PATH=$ROOTCOREBIN/lib:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$ROOTCOREBIN/lib:$DYLD_LIBRARY_PATH
export PYTHONPATH=$ROOTCOREBIN/python:$PYTHONPATH

mkdir -p $ROOTCOREBIN/bin
test -f $ROOTCOREBIN/bin/rc || ln -sf $ROOTCOREDIR/scripts/rc.sh $ROOTCOREBIN/bin/rc
true
