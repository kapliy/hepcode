#!/bin/bash

# Script for adding together a bunch of root files in a directory into a new root file merged.root
# Since it takes a lot of memory to load more than ~10 root files into memory with hadd,
# the idea here is to not load in more than 10 at a time.
maxfiles=10

if [ ! $# == 1 ]; then
    echo "USAGE: just pass in the directory containing root files"
fi
mergedir="$1"

# produce a list of all the original files to merge
totalfiles=0
for i in ${mergedir}/*.root; do
    let totalfiles++
    totalfilelist[$totalfiles]=$i
done

# check that there is at least one file
if [ `ls ${mergedir}/*.root | wc -l` -eq 0 ]; then
    echo "no root files found in that directory"
    return
fi

# check that the directory doesn't already contain merged.root
if [ -e ${mergedir}/merged.root ]; then
    echo "merged.root already exists in that directory"
    return
fi

# begin merging the files in stages
mergestage=0
finished=0
while [ $finished -eq 0 ]; do
    # produce vector of all files to merge in this stage
    totalfiles=0
    if [ $mergestage -eq 0 ]; then
	for i in ${mergedir}/*.root; do
	    let totalfiles++
	    filelist[$totalfiles]=$i
	done
    else
	for i in ${mergedir}/.mergejob_stage${mergestage}_part*.root; do
	    let totalfiles++
	    filelist[$totalfiles]=$i
	done
    fi
    let mergestage++
    # if only one file, just copy it to merged.root
    if [ $totalfiles -eq 1 ]; then
	cp ${filelist[1]} ${mergedir}/merged.root
	finished=1
    else
	liststr=""
	mergepart=1
	for (( i=1; i<=${totalfiles}; i++ )); do
	    liststr="${filelist[$i]} ${liststr}"
	    if [ $i -eq $totalfiles -o $(( $i % $maxfiles )) -eq 0 ]; then
		echo "merging part $mergepart in stage $mergestage"
		hadd ${mergedir}/.mergejob_stage${mergestage}_part${mergepart}.root $liststr
		liststr=""
		let mergepart++
	    fi
	done
    fi
done

# cleanup all the pieces
rm ${mergedir}/.mergejob_stage*_part*.root

echo "DONE merging"
echo "  root -l ${mergedir}/merged.root"
