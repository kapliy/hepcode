#!/bin/bash

# configuration options for QCD fitting, or any other scripts

input=/share/t3data3/antonk/ana/ana_v29I_12052012_edboard_anywmt20_d03p5_stacoCB_all
DB=DB_12052012_ALL.vd0
out=QCDD0

input=/share/t3data3/antonk/ana/ana_v29I_12052012_edboard_anywmt20_stacoCB_all
DB=DB_12052012_ALL.v1
out=QCD

input=/share/t3data3/antonk/ana/ana_v29I_12232012_edboard_newtrigSF_stacoCB_all/pt20/
DB=DB_12232012_ALL.v1
out=QCD

input=/share/t3data3/antonk/ana/ana_v29I_01032013_edboard_newtrigSF_sumw2_stacoCB_all/pt20
DB=DB_01032012_ALL.v1
out=QCD

INDB=`echo $DB | sed -e 's#DB#IN#g'`
OUTDB=`echo $DB | sed -e 's#DB#OUT#g'`
iq=0
bgsig=5
bgqcd=4
