#!/bin/bash

# configuration options for QCD fitting, or any other scripts

input=/share/t3data3/antonk/ana/ana_v29I_12052012_edboard_anywmt20_d03p5_stacoCB_all
DB=DB_12052012_ALL.vd0
out=QCDD0

input=/share/t3data3/antonk/ana/ana_v29I_12052012_edboard_anywmt20_stacoCB_all
DB=DB_12052012_ALL.v1
out=QCD

input=/share/t3data3/antonk/ana/ana_v29I_01052013_edboard_stacoCB_all/pt20
DB=DB_01052013_ALL.v1

input=/share/t3data3/antonk/ana/ana_v29I_01072013_edboard_stacoCB_all/pt20
DB=DB_01072013_ALL.v3

input=/share/t3data3/antonk/ana/ana_v29I_01182013_paper_stacoCB_all/pt20
DB=DB_01182013_ALL.v6

input=/share/t3data3/antonk/ana/ana_v29I_01182013_paper_stacoCB_all/pt20
DB=DB_01182013_ALL.v7

input=/share/t3data3/antonk/ana/ana_v29I_01182013_paper_stacoCB_all/pt20
DB=DB_01182013_ALL.v8

input=/share/t3data3/antonk/ana/ana_v29I_01252013_paper_stacoCB_all/pt20
DB=DB_01252013_ALL.v9

#input=/share/t3data3/antonk/ana/ana_v29I_01252013_paper_stacoCB_all/pt20
#DB=DB_01252013_ALL.rerun.v9
#input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_stacoCB_all/pt20 # just a rerun
#DB=DB_01292013_ALL.v10
#input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_noCK_stacoCB_all/pt20/
#DB=DB_01292013_ALL.v10noCK
#input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_noC_stacoCB_all/pt20/
#DB=DB_01292013_ALL.v10noC
#input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_oldC_stacoCB_all/pt20
#DB=DB_01292013_ALL.v10oldC
#input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_fineCK_stacoCB_all/pt20
#DB=DB_01292013_ALL.v10fineCK
#input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_anyfit_d0_stacoCB_all/pt20
#DB=DB_01292013_ALL.rerun.d0.v10
#input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_higgsmcp_stacoCB_all/pt20 # higgs mcp corrections
#DB=DB_01292013_ALL.higgsmcp.v10
#input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_anyfit_stacoCB_all/pt20 # rerun (met debugging added)
#DB=DB_01292013_ALL.rerun.v10
input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_anyfit_L_stacoCB_all/pt20 # fixed missing statistics in periodL
DB=DB_01292013_ALL.rerun.v11

input=/share/t3data3/antonk/ana/ana_v29I_02222013_paper_stacoCB_all/pt20  # should be the same. removed anyfit, updated egammaUtils, added pythia11-data rw systematic target. adding new items to ntuple to debug A/C differences. added avgmu to ntuple
DB=DB_02222013_ALL.v12

input=/share/t3data3/antonk/ana/ana_v29I_02232013_paper_stacoCB_all/pt20  #added trigphi to ntuple. added trigall to ntuple (requires AND of trigger for all muons)
DB=DB_02232013_ALL.v12

# TODO: add powheg+pythia wpt target
input=/share/t3data3/antonk/ana/ana_v29I_03012013_paper_stacoCB_all/pt20
DB=DB_03012013_ALL.v13

INDB=`echo $DB | sed -e 's#DB#IN#g'`
OUTDB=`echo $DB | sed -e 's#DB#OUT#g'`
iq=0
bgsig=5
bgqcd=4
