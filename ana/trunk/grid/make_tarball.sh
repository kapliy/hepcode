#!/bin/bash

function usage () {
    echo "Prepares the TrigFTK* package tarballs to be shipped along with the grid job"
    echo "USAGE:"
    echo "./make_tarball TrigFTKSim /path/to/TrigFTKSim TrigFTKAna /path/to/TrigFTKAna"
    echo "Order is important - e.g., TrigFTKSim has to come before TrigFTKAna"
}
export uct3_64=0
# check that we have an even number of arguments
argdiv=`expr "$#" % 2`
if [ "$argdiv" -ne "0" ]; then
    usage
fi

# choose verbosity level:
stdout=/dev/null
#stdout=/dev/stdout

cur="$PWD"
tmp=/tmp/`whoami`  # TODO: check that /tmp is actually available in write-mode

# save the list of package names and locations
unset names
unset locations
npkg=0
i=0
for arg in $@; do
    if [ "$i" -eq "0" ]; then
	names[$npkg]=$arg
    elif [ "$i" -eq "1" ]; then
	locations[$npkg]=$arg
	((npkg++))
    else
	echo "ERROR: this cannot happen"
	exit 1;
    fi;
    ((i++))
    if [ "$i" -eq "2" ]; then i=0; fi;
done;

# loop over packages
for ii in `seq 0 $(expr $npkg - 1)`; do
    loc=${locations[$ii]}
    name=${names[$ii]}
    if [ -d ${loc} ]; then
	rm -f ${cur}/${name}.tar.bz2
	rm -rf ${tmp}/${name}
	mkdir -p ${tmp}/${name}
	
        # make a copy of current TrigFTK* tree
	cd ${loc} && tar cf ${tmp}/${name}/content.tar *
	cd ${tmp}/${name} && tar xf content.tar && rm -f content.tar;
        # cleanup object files
	if [ -d standalone ]; then
	    cd standalone
	    if [ -f "bootstrap.sh" ]; then 
		source bootstrap.sh &> ${stdout}
	    fi;
	    make clean &> ${stdout}
	    cd - &>/dev/null
	else
	    if [ -f "bootstrap.sh" ]; then
		source ./grid/dependencies.sh 1 &> ${stdout}
		sh bootstrap.sh &> ${stdout}
		make clean &> ${stdout}
		sed -i -e 's#1.39.0#1.42.0#g' configure.ac
		sed -i -e 's#plot##g' Makefile.am
		sh bootstrap.sh &> ${stdout}
		make clean &> ${stdout}
		rm -f ${FTKCLASSES}
	    fi;
	fi;
        # cleanup .svn and other user-generated files
	cd ${tmp}/${name}
	find . -type d -name \.svn | xargs rm -rf
	find . -type d -name results -maxdepth 1 | xargs rm -rf
	find . -type f -name pbsjob.sh\* | xargs rm -rf
	find . -type f -name jobinput\*xml | xargs rm -f
	find . -type f -name \.ftk_\* | xargs rm -f
        # make TrigFTK*.tar.bz2
	cd ${tmp} && tar cfj ${cur}/${name}.tar.bz2 ${name} 
    else
	echo "ERROR: couldn't find package ${name} in ${loc}"
    fi;
done;

# clean up
for name in ${names[@]}; do
    rm -rf ${tmp}/${name}
done;

# return to original PWD
cd ${cur}
