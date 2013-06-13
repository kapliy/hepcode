#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

rm -rf $ROOTCOREBIN/bin
mkdir $ROOTCOREBIN/bin
test -f $ROOTCOREBIN/bin/rc || ln -sf $ROOTCOREDIR/scripts/rc.sh $ROOTCOREBIN/bin/rc
rm -rf $ROOTCOREBIN/include
mkdir $ROOTCOREBIN/include
rm -rf $ROOTCOREBIN/python
mkdir $ROOTCOREBIN/python
rm -rf $ROOTCOREBIN/user_scripts
mkdir $ROOTCOREBIN/user_scripts
rm -rf $ROOTCOREBIN/data
mkdir $ROOTCOREBIN/data
rm -rf $ROOTCOREBIN/lib
mkdir $ROOTCOREBIN/lib

rm -f $ROOTCOREBIN/preload
touch $ROOTCOREBIN/preload
rm -f $ROOTCOREBIN/load
touch $ROOTCOREBIN/load

parallel=""
test "$ROOTCORE_NCPUS" != "" && parallel="-j $ROOTCORE_NCPUS"

mkdir -p "$ROOTCOREBIN/RootCore"
file="$ROOTCOREBIN/RootCore/Packages.h"
rm -f "$file-"
echo '#ifndef ROOTCORE_PACKAGES_H' >"$file-"
echo '#define ROOTCORE_PACKAGES_H' >>"$file-"
echo >>"$file-"
echo '// This file contains one define statement for each package detected by' >>"$file-"
echo '// RootCore.  It is meant to allow to write package A in a form that' >>"$file-"
echo '// it uses the services of package B when available, but otherwise' >>"$file-"
echo '// skips it without failing.  For this to work properly you need to list' >>"$file-"
echo '// package B in the PACKAGE_TRYDEP of package A.' >>"$file-"
echo >>"$file-"
for pkg in RootCore `cat $pkgfile`
do
    echo "#define ROOTCORE_PACKAGE_`basename $pkg`" >>"$file-"
done
echo >>"$file-"
echo '#endif' >>"$file-"
if test -f "$file" && cmp --quiet "$file" "$file-"
then
    rm -f "$file-"
else
    cp -f "$file-" "$file"
fi
ln -s "$ROOTCOREBIN/RootCore" "$ROOTCOREBIN/include/RootCore"

for pkg in `cat $pkgfile`
do
    name=`basename $pkg`
    echo compiling $pkg
    if cd $pkg/cmt
    then
	true
    else
	echo "failed to enter package $pkg"
	exit 2
    fi
    if test -f precompile.RootCore
    then
	if test \! -x precompile.RootCore
	then
	    echo execute flag not set on precompile.RootCore, please run
	    echo   chmod +x $pkg/cmt/precompile.RootCore
	    exit 5
	fi
	if ./precompile.RootCore
	then
	    true
	else
	    echo failed to precompile package $pkg
	    exit 6
	fi
    fi
    if make $parallel -f Makefile.RootCore $*
    then
	true
    else
	echo "failed to compile package $pkg"
	exit 3
    fi
    $ROOTCOREDIR/scripts/link_package.sh $pkg || exit $?
    test "`$ROOTCOREDIR/scripts/get_field.sh $pkg/cmt/Makefile.RootCore PACKAGE_NOCC`" != "1" && \
	echo $pkg | sed 's/.*\///' >>$ROOTCOREBIN/load
    for lib in `$ROOTCOREDIR/scripts/get_field.sh Makefile.RootCore PACKAGE_PRELOAD`
    do
	echo lib$lib >>$ROOTCOREBIN/preload
    done
done

rm -f $ROOTCOREBIN/load_packages_success
if $ROOTCOREDIR/scripts/root.sh -l -b -q $ROOTCOREDIR/scripts/load_packages.C && test -f $ROOTCOREBIN/load_packages_success
then
    true
else
    echo "failed to test load libraries"
    exit 4
fi
