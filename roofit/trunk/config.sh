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

# input=/share/t3data3/antonk/ana/ana_v29I_01292013_paper_anyfit_L_stacoCB_all/pt20 # fixed missing statistics in periodL
# DB=DB_01292013_ALL.rerun.v11
# input=/share/t3data3/antonk/ana/ana_v29I_02222013_paper_stacoCB_all/pt20  # should be the same. removed anyfit, updated egammaUtils, added pythia11-data rw systematic target. adding new items to ntuple to debug A/C differences. added avgmu to ntuple
# DB=DB_02222013_ALL.v12
# input=/share/t3data3/antonk/ana/ana_v29I_02232013_paper_stacoCB_all/pt20  #added trigphi to ntuple. added trigall to ntuple (requires AND of trigger for all muons)
# DB=DB_02232013_ALL.v12

input=/share/t3data3/antonk/ana/ana_v29I_03012013_paper_stacoCB_all/pt20  #start using new nominal W/Z reweighting target (based on Z-to-data)
DB=DB_03012013_ALL.v13   # excludes wpt pythia8 as systematic
DB=DB_03012013_ALL.v14   # includes wpt pythia8 AND sherpa as systematics

#input=/share/t3data3/antonk/ana/ana_v29I_03062013_paper_stacoCB_all/pt20
#DB=DB_03062013_ALL.v15   # no change - just extra triggers for ntuple

# first version with J
#input=/share/t3data3/antonk/ana/ana_v29J_03062013_paper_stacoCB_all/pt20
#DB=DB_03062013_ALL.v16   # trigger matching fixed; v29J ntuples
# fixed bad bin
#input=/share/t3data3/antonk/ana/ana_v29J_03172013_paper_stacoCB_all/pt20
#DB=DB_03172013_ALL.v17   # trigger matching fixed; fixed bad bin
#input=/share/t3data3/antonk/ana/ana_v29J_03172013_paper_mu18_stacoCB_all/pt20
#DB=DB_03172013_ALL.v18   # use mu18 (non-MG) triggers - only useful for W plots

# first version of non-MG trigger scale factors from Carl
input=/share/t3data3/antonk/ana/ana_v29I_04032013_nomg_stacoCB_all/pt20
DB=DB_04032013_NOMG.v20  # 100 toys
DB=DB_04032013_NOMG.v21  # 1000 toys
DB=DB_04032013_NOMG.v22  # 1000 toys; returning w mT fits; fixing wpt reweighting
DB=DB_04032013_NOMG.v23  # ditto, but using QCD fit error as QCD stat uncertainty

# first version running via condor
input=/atlas/uct3/data/users/antonk/ana/ana_v29I_04242013_condor_stacoCB_all/pt20
DB=DB_04242013_NOMG.v24  # proper hadd
DB=DB_04242013_NOMG.v25  # proper hadd; reduce wmt by trying ranges; adding: pileupdown

input=/share/t3data3/antonk/ana/ana_v29I_04252013_rep_stacoCB_all/pt20
DB=DB_04242013_NOMG.v26  # proper hadd; reduce wmt by trying ranges; adding: DtoK and LtoM; pileupdown

input=/share/t3data3/antonk/ana/ana_v29I_05142013_newmet_stacoCB_all/pt20
DB=DB_05142013_NOMG.v27  # updated met util (soft terms systematics). Set both nvtx and avgmu in metutil. 3000 MC toys with stat decoupling of charges and regions. removed eta x pt unfolding. commented out DtoK and LtoM stuff. full set of bootstrapping histograms in data. Total QCD uncertainty updated in qcdsys.py to make correct errors in max_latex.py

# experimental:
#input=/share/t3data3/antonk/ana/ana_v29I_05142013_newmet_qindepSF_stacoCB_all/pt20
#DB=DB_05142013_qindepsf.v28   # charge-independent trigger scale factors

input=/share/t3data3/antonk/ana/ana_v29I_05182013_fullstat_stacoCB_all/pt20
DB=DB_05182013_fstat.v29  # update to full mcnlo and powhegHerig stat. new egammaUtils: remove wpol constraint and apply jan's new experimental wpol reweights (mcnlo->powhegherwig or mcnlo/powhegherwig->powhegpythia) to reduce CW syst. fix bootstrapping to 1000 replicas and only run on baseline Nominal data.
DB=DB_05182013_fstat.v30  # adding w polarization histograms to output root files

input=/share/t3data3/antonk/ana/ana_v29I_05202013_fullstat_ret_stacoCB_all/pt20 #gone
DB=DB_05202013_fstat.v32  # fixed problems with bootstrap

input=/atlas/uct3/data/users/antonk/ana/ana_v29I_06092013_recoSFphi_stacoCB_all/pt20 #gone
DB=DB_06092013_recoSFphi.v33 # reco SF in eta x phi, rather than eta x pt

input=/atlas/uct3/data/users/antonk/ana/ana_v29I_06092013_recoSFphi_ret_stacoCB_all/pt20 #gone
DB=DB_06092013_recoSFphi.v34 # reco SF in eta x phi, rather than eta x pt

input=/share/t3data3/antonk/ana/ana_v29I_06132013_recoSFpt_stacoCB_all/pt20 #gone
DB=DB_06132013_recoSFpt.v35  # same as v31, but incorporating all fixes, and including anyfit

input=/share/t3data3/antonk/ana/ana_v29I_06172013_recoBOTH_withZ_stacoCB_all/pt20
DB=DB_06172013_recoBOTH_withZ.v36  # fixed bug that lost some signal files. nominal is SFpt, but added a new st_w_bla option to do phi. Also: added z plots. BUGGY - missing files

input=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_stacoCB_all/pt20   # disabled trig matching. updated reco SF eta-phi class (correct errors). Added deltaPhi(soft,all-but-soft). Added a new systematic - reweight signal MC to data (Max's file).
DB=DB_06192013_recoBOTH_withZ.v37
#input=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_recoPhi_stacoCB_all/pt20   # v37, but with eta x phi nominal reco SF
#DB=DB_06192013_recoBOTH_withZ_recoPhi.v38

input=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_anyfit_stacoCB_all/pt20
DB=DB_06192013_recoBOTH_withZ_anyfit.v39  # added anyfit to make MET plots. updated to new iso SFs. added trigphi, effphi, eff*trigphi

INDB=`echo $DB | sed -e 's#DB#IN#g'`
OUTDB=`echo $DB | sed -e 's#DB#OUT#g'`
iq=0
bgsig=5
bgqcd=4
