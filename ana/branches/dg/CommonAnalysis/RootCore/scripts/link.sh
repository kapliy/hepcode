#!/bin/bash

$ROOTCOREDIR/scripts/find_packages.sh || exit $?
pkgfile=$ROOTCOREBIN/packages

for pkg in `cat $pkgfile`
do
    name=`basename $pkg`
    $ROOTCOREDIR/scripts/link_package.sh $pkg
done
