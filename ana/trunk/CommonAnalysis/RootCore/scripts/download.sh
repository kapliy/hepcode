#!/bin/bash

set -e

url="$1"
file="$2"

test "$url" == "" -o "$file" == "" && echo insufficient arguments for download.sh && exit 1

mkdir -p `dirname $file`
if test \! -f $file
then
    if test "`which curl`" != ""
    then
 	echo downloading $url
 	if curl $url >$file
 	then
 	    true
 	else
 	    echo failed to download
 	    rm -f $file
 	    exit 1
 	fi
    elif test "`which wget`" != ""
    then
 	echo downloading $url
 	if (cd `dirname $file` && wget $url)
 	then
 	    true
 	else
 	    echo failed to download
	    rm -f $file
 	    exit 1
 	fi
    else
 	echo $name not found, please download from
 	echo "  $url"
 	exit 1
    fi
fi
