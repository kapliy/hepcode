#!/bin/bash

set -e

export options_file=""
if test -f configured
then
    options_file="`$ROOTCOREDIR/scripts/get_field.sh configured options_file`"
fi
if test "$options_file" == ""
then
    options_file="`pwd`/options"
fi

if test $ROOTCOREDIR/scripts/external_compile.sh -nt configured -o $ROOTCOREDIR/scripts/external_download.sh -nt configured -o precompile.RootCore -nt configured -o install -nt configured -o options -nt configured -o $options_file -nt configured
then
    rm -f configured
fi

if test -e configured
then
    exit 0
fi

options_file=""
for alternate in `$ROOTCOREDIR/scripts/get_field.sh options alternates`
do
    if test "$options_file" == ""
    then
	alternate="`echo $alternate`"
	if test -f "$alternate"
	then
	    options_file="$alternate"
	fi
    fi
done
if test "$options_file" == ""
then
    options_file="`pwd`/options"
fi

name="`$ROOTCOREDIR/scripts/get_field.sh $options_file name`"
incname="`$ROOTCOREDIR/scripts/get_field.sh $options_file incname`"
libname="`$ROOTCOREDIR/scripts/get_field.sh $options_file libname`"

echo configuring `$ROOTCOREDIR/scripts/get_field.sh Makefile.RootCore PACKAGE`

force_local="`$ROOTCOREDIR/scripts/get_field.sh $options_file force_local`"
allow_global="`$ROOTCOREDIR/scripts/get_field.sh $options_file allow_global`"
siteroot="`$ROOTCOREDIR/scripts/get_field.sh $options_file siteroot`"
locations="`$ROOTCOREDIR/scripts/get_field.sh $options_file locations`"

if test "$force_local" == "yes"
then
    allow_global="no"
    locations=""
    siteroot=""
elif test "$force_local" != "no"
then
    echo invalid value "$force_local" for option force_local
    exit 1
fi

if test "$allow_global" == "yes"
then
    if $ROOTCOREDIR/scripts/test_cc.sh compile <test_file.C
    then
	echo "options_file = $options_file" >configured
	echo "incdir = " >>configured
	echo "libdir = " >>configured
	echo "incname = $incname" >>configured
	echo "libname = $libname" >>configured
	echo using system $name installation
	exit 0
    fi
elif test "$allow_global" != "no"
then
    echo invalid value "$allow_global" for option allow_global
    exit 1
fi


if test "$siteroot" != "" -a "$SITEROOT" != ""
then
    for libdir in `find $SITEROOT/$siteroot -type d -name lib | grep $CMTCONFIG`
    do
	echo $libdir
	maindir=`dirname "$libdir"`
	incdir=`find $maindir/include -name boost | head -n 1 | xargs dirname`
	locations="$locations $incdir:$libdir"
    done
fi

locations="$locations `pwd | xargs dirname`/local"

for baseloc in $locations
do
    loc=`eval echo $baseloc`
    if test "`echo $loc | grep :`" != ""
    then
	incdir=`echo $loc | sed 's/:.*//'`
	libdir=`echo $loc | sed 's/.*://'`
    else
	incdir=$loc/include
	libdir=$loc/lib
    fi
    if test -d "$incdir" -a -d "$libdir"
    then
	use=1
	for inc in $incname
	do
	    if test \! -e "$incdir/$inc"
	    then
		use=0
	    fi
	done
	for lib in $libname
	do
	    if test "`ls $libdir | grep ^$lib`" == ""
	    then
		use=0
	    fi
	done
	if test $use -eq 1
	then
	    echo "options_file = $options_file" >configured
	    echo "incdir = $incdir" >>configured
	    echo "libdir = $libdir" >>configured
	    echo "incname = $incname" >>configured
	    echo "libname = $libname" >>configured
	    echo using $name installation:
	    cat configured
	    exit 0
	fi
    fi
done

echo failed to find valid $name installation in $locations

if test -f ./install
then
    echo trying manual install
    if ./install
    then
	echo "options_file = $options_file" >configured
	echo "incdir = %/local/include" >>configured
	echo "libdir = %/local/lib" >>configured
	echo "incname = $incname" >>configured
	echo "libname = $libname" >>configured
	exit 0
    fi
    echo installation of $name failed
fi

false
