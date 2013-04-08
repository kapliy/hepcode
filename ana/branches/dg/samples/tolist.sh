#!/bin/bash

#for f in wasymmetry29I*dat; do ./tolist.sh $f > /home/antonk/slim/mc11c/$f ; done

# converts TrigFTKAna samples list to plain list
#cat $1 | grep DPETER | sed -e 's#DPETER #root://xrddc.mwt2.org:1096#g'
cat $1 | grep DPETER | awk '{prefix=""; if(match($2,"/atlas/uct3/data/")) prefix="root://uct3-xrd.mwt2.org//"; if(match($2,"/pnfs/uchicago.edu/")) prefix="root://xrddc.mwt2.org:1096"; printf("%s%s\n",prefix,$2);}'
