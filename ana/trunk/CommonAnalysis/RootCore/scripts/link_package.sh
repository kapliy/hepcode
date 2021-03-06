#!/bin/bash

source "`dirname $0`/preamble.sh" "$0"

pkg=$1
name=`basename $pkg`

test -d $pkg/$name && rm -f $ROOTCOREBIN/include/$name && ln -sf $pkg/$name $ROOTCOREBIN/include/
test -d $pkg/python && rm -f $ROOTCOREBIN/python/$name && ln -sf $pkg/python $ROOTCOREBIN/python/$name
test -d $pkg/scripts && rm -f $ROOTCOREBIN/user_scripts/$name && ln -sf $pkg/scripts $ROOTCOREBIN/user_scripts/$name
(test -d $pkg/data && rm -f $ROOTCOREBIN/data/$name && ln -sf $pkg/data $ROOTCOREBIN/data/$name) || (test -d $pkg/share && rm -f $ROOTCOREBIN/data/$name && ln -sf $pkg/share $ROOTCOREBIN/data/$name)

if test "$ROOTCOREOBJ" == ""
then
    objdir=$pkg
else
    objdir=$ROOTCOREOBJ/$name
fi

test -f $objdir/StandAlone/lib${name}.so && rm -f $ROOTCOREBIN/lib/lib${name}.so && ln -sf $objdir/StandAlone/lib${name}.so $ROOTCOREBIN/lib/
if test -d $objdir/bin
then
    for file in $objdir/bin/*
    do
      test -f $file && rm -f $ROOTCOREBIN/bin/`basename $file` && ln -sf $file $ROOTCOREBIN/bin/
    done
fi

if test -f $pkg/cmt/link.RootCore
then
    cd $pkg/cmt
    if test \! -x link.RootCore
    then
	echo execute flag not set on link.RootCore, please run
	echo   chmod +x $pkg/cmt/link.RootCore
	exit 1
    fi
    if ./link.RootCore
    then
	true
    else
	echo failed to link package $pkg
	exit 1
    fi
  
fi
true
