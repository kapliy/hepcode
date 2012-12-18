#!/bin/bash

# configuration options for QCD fitting, or any other scripts

input=/share/t3data3/antonk/ana/ana_v29I_12052012_edboard_anywmt20_stacoCB_all
DB=DB_12052012_ALL.v1
INDB=`echo $DB | sed -e 's#DB#IN#g'`
OUTDB=`echo $DB | sed -e 's#DB#OUT#g'`

out=QCD
iq=0
bgsig=5
bgqcd=4
