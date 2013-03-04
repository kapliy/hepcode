#!/bin/bash

# Highly parallel file replicator from DQ2 grid datasets to an xrootd directory

NTHREADS=25           # maximum number of parallel dq2-get/xrdcp threads
PATTERN=".root"       # pattern for files that you want downloaded
HASHCHECK=0           # enable a hash check of previously-downloaded files (SUPER SLOW!)

# CACHENAME is an optional file that lists full paths to already-downloaded files
# These already-downloaded files are skipped, assuming they have the right size
# Use this option if, for example, you staged *some* of the files via DaTRI
# and trying to download only what's missing.
# PS: file paths in cache HAVE to be different from destination directory of dq2xrdcp.sh!
CACHENAME='cache.dat'

# python mini-program to calculate adler32 checksums
adler32="""
BLOCKSIZE=32*1024*1024
import sys
from zlib import adler32
fp=open(sys.argv[1])
val = 1
while True:
   data = fp.read(BLOCKSIZE)
   if not data:
       break
   val = adler32(data, val)
if val < 0:
   val += 2**32
print\"ad:\"+hex(val)[2:10].zfill(8).lower()
"""

# copies a local file to xrootd
function copy_file_to_xrootd () {
    local origfile destpath origmd5 destmd5 destfile bname destfilexrd origsize destsize newsize
    local CPCMD do_overwrite attempt xrootdout newmd5 HOST MAXATTEMPTS failed_size failed_hash
    MAXATTEMPTS=9
    HOST=uct3-xrd.mwt2.org/
    origfile=$1
    destpath=$2
    origmd5=`md5sum ${origfile} | cut -d ' ' -f 1`
    bname=`basename ${origfile}`
    destfile=`echo ${destpath}/${bname} | sed -e 's#///#/#g' | sed -e 's#//#/#g'`
    destfilexrd=root://${HOST}/${destfile}
    origsize=`stat -c %s ${origfile}`
    echo "xrdcp -> ${origfile} ${origsize} ${origmd5}"
    CPCMD="xrdcp -s -f -md5 ${origfile} ${destfilexrd}"
    for attempt in `seq -w ${MAXATTEMPTS}`; do
	do_overwrite=1
	# check if a file already exists
	test -f "${destfile}" && {
	    destsize=`stat -c %s ${destfile}`
	    # check size
	    if [ "${origsize}" == "${destsize}" ]; then
		do_overwrite=0
	    fi;
	    if [ "${do_overwrite}" -eq "1" ]; then
		echo "OVERWRITE WARNING: replacing file ${destfile} old_size=${destsize} newsize=${origsize}"
		rm -f ${destfile}
	    fi
	}
	if [ "${do_overwrite}" -eq "1" ]; then
	    # make sure the user is actually requesting a copy to xrootd. If not, then use simple linux "cp"
	    if [ `echo ${destpath} | grep "/atlas/uct3/"` ]; then
		xrootdout=`${CPCMD}`
  	        # xrootd-to-VFS driver has a synchronization issue, so wait a bit
		sleep 3
		newmd5=`echo $xrootdout | cut -d ' ' -f 2`
		if [ ! "${origmd5}" == "${newmd5}" ]; then
		    echo "XRDCP HASH WARNING (retry #${attempt}): ${origmd5}!=${newmd5} for ${bname}"
		    rm -f ${destfile}
		    sleep 5
		    continue
		fi;
	    else
		cp -u ${origfile} ${destfile}
	    fi
	fi;
	newsize=`stat -c %s ${destfile} 2>/dev/null`;
	if [ ! "${origsize}" == "${newsize}" ]; then
	    echo "XRDCP SIZE WARNING (retry #${attempt}): ${origsize}!=${newsize} for ${bname}"
	    rm -f ${destfile}
	    sleep 5
	    continue
	else
	    return
	fi
    done;
    # delete file at destination if the transfer failed despite all retries
    echo "XRDCP ERROR: ${CPCMD}"
    rm -f ${destfile}
}

# downloads a file from dq2 and invokes copy_file_to_xrootd
# Following a dq2-get, the file integrity is manually verified via adler32 hash
function copy_file_from_dq2() {
    local MAXATTEMPTS fstring to nelm ds fname origsize origfile origadler origmd5
    local cachesize cachefile cacheadler cachemd5
    local destpath destfile destadler bname attempt do_overwrite
    MAXATTEMPTS=9
    fstring=$1
    to=$2
    nelm=`echo $fstring | awk 'BEGIN{FS="__:__"};{print NF}'`
    if [ ! "${nelm}" == "4" ]; then
	echo "FATAL ERROR: dq2-ls string returned ${fname} from dataset ${ds}, which has the wrong format"
	return
    fi
    ds=`echo $fstring  | awk 'BEGIN{FS="__:__"};{print $1}'`
    fname=`echo $fstring  | awk 'BEGIN{FS="__:__"};{print $2}'`
    origsize=`echo $fstring  | awk 'BEGIN{FS="__:__"};{print $3}'`
    origadler=`echo $fstring  | awk 'BEGIN{FS="__:__"};{print $4}'`
    origfile=${ds}/${fname}
    destpath=${to}/${ds}/
    bname=`basename ${origfile}`
    destfile=`echo ${destpath}/${bname} | sed -e 's#///#/#g' | sed -e 's#//#/#g'`
    #echo ${origfile} ${destfile}
    do_overwrite=1
    # check against the user-supplied cache of already-downloaded files
    cachesize="NONE"
    if [ -f "$CACHENAME" ]; then
	cachefile=`grep --color=never $origfile $CACHENAME 2>/dev/null`
	test -f "${cachefile}" && {
	    cachesize=`stat -c %s ${cachefile}`
	    # since pre-downloaded cache is usually on tier2 dcache, skip adler check and only use size check
	    if [ "${origsize}" == "${cachesize}" ]; then
		do_overwrite=0
		echo "CACHEFILE FOUND INFO: found in cache: ${ds} - ${fname} -> ${cachefile}"
		# clean up a copy on xrootd, since we already have one in cache
		if [ -f "${destfile}" ]; then
		    echo "CACHEFILE CLEANUP INFO: deleting duplicate ${destfile} -> ${cachefile}"
		    rm -f ${destfile}
		fi
		sleep 1
	    else
		echo "CACHEFILE SIZE WARNING: ${ds} - ${fname} - ${cachefile} : size ${origsize} ${cachesize}"
	    fi
	}
    fi;
    # check if the file already exists and has the right size (and optionally, adler hash) at the destination
    destsize="NONE"
    destadler="NONE"
    test -f "${destfile}" && {
	destsize=`stat -c %s ${destfile}`
	if [ "${origsize}" == "${destsize}" ]; then
	    do_overwrite=${HASHCHECK}
	    if [ "${HASHCHECK}" == "1" ]; then
		destadler=`python -c "${adler32}" ${destfile}`
		if [ "${origadler}" == "${destadler}" ]; then
		    do_overwrite=0
		fi
	    fi
	fi
	if [ "${do_overwrite}" == "1" ]; then
	    rm -f ${destfile}
	    sleep 1
	fi;
    }
    # (re-)download the file
    echo "processing -> $ds/$fname $origsize $origadler"
    if [ "${do_overwrite}" == "1" ]; then
	gtgo=0
	echo "dq2 -> $ds/$fname $origsize $origadler"
	for attempt in `seq -w ${MAXATTEMPTS}`; do
	    dq2-get -L ROAMING -f ${fname} ${ds} &> /dev/null
	    newsize="NONE"
	    newadler="NONE"
	    # check that the file exists
	    test -f "${origfile}" && {
		newsize=`stat -c %s ${origfile}`
		newadler=`python -c "${adler32}" ${origfile}`
	    }
	    # check file size wrt DQ2 metadata
	    if [ ! "${origsize}" == "${newsize}" ]; then
		echo "DQ2 SIZE WARNING (retry #${attempt}): ${ds} - ${fname} : size ${origsize} ${newsize}"
		rm -f ${origfile}
		sleep 30
		continue
	    fi;
 	    # check adler32 hash wrt DQ2 metadata
	    if [ ! "${origadler}" == "${newadler}" ]; then
		echo "DQ2 HASH WARNING (retry #${attempt}): ${ds} - ${fname} : hash ${origadler} ${newadler}"
		rm -f ${origfile}
		sleep 5
		continue
	    fi;
	    # if all tests passed, we are done!
	    gtgo=1
	    break
 	done
	# if dq2-get succeeded, copy the file to xrootd
	if [ "${gtgo}" -eq "1" ] ; then
	    mkdir ${destpath} 2>/dev/null && sleep 1
	    copy_file_to_xrootd ${origfile} ${destpath}
	else
	    echo "DQ2 ERROR: ${ds} - ${fname}"
	fi
    fi
    rm -f ${origfile}
    rmdir ${ds} &>/dev/null
}

# require uct3-s1 or uct3-s3
if [ "${HOSTNAME}" != "uct3-s1.uchicago.edu" -a "${HOSTNAME}" != "uct3-s3.uchicago.edu" ]; then
    echo "ERROR: must run on uct3-s1.uchicago.edu"
    exit 2
fi

# require PWD = /tmp/blabla (never dq2-get over NFS!)
echo $PWD | grep -q '/tmp' || {
    echo "ERROR: PWD=$PWD is not under /tmp. Never download to HOME or t3dataX drives."
    exit 2
}

# require exactly two arguments
if [ "$#" -lt "2" ]; then
    if [ "$#" -eq "0" ]; then
	echo "USAGE: $0 /atlas/uct3/destination/ user.nickname.container1/ user.nickname.container2 <...>"
    fi
    # if the script is sourced, we want to "return"
    return 0 &> /dev/null
    # if the script is invoked, we want to "exit"
    exit 0
fi

to=$1
shift
containers=$@

echo ${to} | grep -q '/atlas/uct3/' || {
    echo "INFO: destination is not XROOT (must begin with '/atlas/uct3/')"
}

if [ "$PWD" == "$HOME" ]; then
    echo "WARNING: better to run from /share/ftkdata, but let's continue anyway"
fi

voms-proxy-info -exists -valid 2:2 &>/dev/null || {
    echo "ERROR: cannot find grid proxy"
    exit 1
}

# post-process input arguments
to=`echo ${to}/ | sed -e 's#//#/#g'`
echo "Copying to ${to} the following datasets/containers:"
echo ${containers}
date

# copy
i=0
declare -a allfiles
for container in ${containers}; do
    if [ "${container:(-1)}" == "/" ]; then
	datasets=`dq2-list-datasets-container ${container} 2>/dev/null | sort`
    else
	datasets=${container}
    fi
    zc=0
    for dataset in ${datasets}; do
	z=0
	for file in `dq2-ls -L ROAMING -f -e ${dataset} 2>/dev/null | grep ${PATTERN} | awk -v ds=${dataset} '{printf("%s__:__%s__:__%s__:__%s\n",ds,$3,$6,$5);}'`; do
	    allfiles[$i]=${file}
	    ((i++))
	    ((z++))
	    ((zc++))
	done
	echo "INFO: dataset ${dataset} = ${z} files"
    done
    echo "INFO: container ${container} = ${zc} files"
done
echo "INFO: total = ${i} files"
mkdir -p ${to}
(for f in ${allfiles[@]}; do echo $f; done) | sort | uniq | xargs -i -n1 -P${NTHREADS} bash -c "source $0 dummy; copy_file_from_dq2 {} ${to}"

wait
date
echo "DONE"
