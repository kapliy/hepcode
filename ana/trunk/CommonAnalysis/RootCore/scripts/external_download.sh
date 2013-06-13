#!/bin/bash

set -e

name="`$ROOTCOREDIR/scripts/get_field.sh $options_file name`"
url="`$ROOTCOREDIR/scripts/get_field.sh $options_file url`"
archive="`$ROOTCOREDIR/scripts/get_field.sh $options_file archive`"
source="local/src/`$ROOTCOREDIR/scripts/get_field.sh $options_file src_dir`"
target="`pwd | xargs dirname`/local"

cd ..

$ROOTCOREDIR/scripts/download.sh "$url" "$archive"

mkdir -p local/src
cd local/src
tar xfz ../../$archive

cd "`$ROOTCOREDIR/scripts/get_field.sh $options_file src_dir`"

export CXXFLAGS="$CXXFLAGS $EXTRA_CFLAGS"
export CFLAGS="$CFLAGS $CXXFLAGS"
export LDFLAGS="$LDFLAGS $EXTRA_CFLAGS"
