#!/bin/bash

if test \! -e configured
then
    echo failed to find $name installation
    exit 1
fi

incdir=`$ROOTCOREDIR/scripts/get_field.sh configured incdir`
libdir=`$ROOTCOREDIR/scripts/get_field.sh configured libdir`
incname=`$ROOTCOREDIR/scripts/get_field.sh configured incname`
libname=`$ROOTCOREDIR/scripts/get_field.sh configured libname`

if test "${incdir:0:1}" == "%"
then
    incdir=`pwd | xargs dirname`${incdir:1}
fi
if test "${libdir:0:1}" == "%"
then
    libdir=`pwd | xargs dirname`${libdir:1}
fi

if test "$incdir" != ""
then
    for inc in $incname
    do
	ln -sf $incdir/$inc $ROOTCOREBIN/include/$inc || exit $?
    done
fi
if test "$libdir" != ""
then
    for mylib in $libname
    do
	ln -sf $libdir/$mylib* $ROOTCOREBIN/lib/ || exit $?
	cd $ROOTCOREBIN/lib
	for lib in `ls | grep -E -e \\.dylib$`
	do
	    newlib=`echo $lib | grep $mylib | sed 's/\.dylib$/.so/'`
	    test \! -e $newlib && ln -s $lib $newlib
	done
	for lib in `ls | grep -E -e '-[0-9.]*\.'`
	do
	    newlib=`echo $lib | grep $mylib | sed 's/-[0-9.]*\././'`
	    test \! -e $newlib && ln -s $lib $newlib
	done
    done
fi

true
