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

# input=/share/t3data3/antonk/ana/ana_v29I_05202013_fullstat_ret_stacoCB_all/pt20 #gone
# DB=DB_05202013_fstat.v32  # fixed problems with bootstrap

# input=/atlas/uct3/data/users/antonk/ana/ana_v29I_06092013_recoSFphi_stacoCB_all/pt20 #gone
# DB=DB_06092013_recoSFphi.v33 # reco SF in eta x phi, rather than eta x pt

# input=/atlas/uct3/data/users/antonk/ana/ana_v29I_06092013_recoSFphi_ret_stacoCB_all/pt20 #gone
# DB=DB_06092013_recoSFphi.v34 # reco SF in eta x phi, rather than eta x pt

# input=/share/t3data3/antonk/ana/ana_v29I_06132013_recoSFpt_stacoCB_all/pt20 #gone
# DB=DB_06132013_recoSFpt.v35  # same as v31, but incorporating all fixes, and including anyfit

# input=/share/t3data3/antonk/ana/ana_v29I_06172013_recoBOTH_withZ_stacoCB_all/pt20
# DB=DB_06172013_recoBOTH_withZ.v36  # fixed bug that lost some signal files. nominal is SFpt, but added a new st_w_bla option to do phi. Also: added z plots. BUGGY - missing files

# input=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_stacoCB_all/pt20   # disabled trig matching. updated reco SF eta-phi class (correct errors). Added deltaPhi(soft,all-but-soft). Added a new systematic - reweight signal MC to data (Max's file).
# DB=DB_06192013_recoBOTH_withZ.v37
# #input=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_recoPhi_stacoCB_all/pt20   # v37, but with eta x phi nominal reco SF
# #DB=DB_06192013_recoBOTH_withZ_recoPhi.v38

# input=/share/t3data3/antonk/ana/ana_v29I_06192013_recoBOTH_withZ_anyfit_stacoCB_all/pt20  # reco x phi used by mistake as nominal!
# DB=DB_06192013_recoBOTH_withZ_anyfit.v39  # added anyfit to make MET plots. updated to new iso SFs. added trigphi, effphi, eff*trigphi

input=/share/t3data3/antonk/ana/ana_v29I_06252013_newrecoSF_rep_stacoCB_all/pt20
DB=DB_06252013_newrecoSF.v40    # calo vs ID factored out
#input=/share/t3data3/antonk/ana/ana_v29I_06252013_newrecoSF_MG_stacoCB_all/pt20
#DB=DB_06252013_newrecoSF_mg.v41 # re-run

input=/share/t3data3/antonk/ana/ana_v29I_06292013_postSMmtg_stacoCB_all/pt20
DB=DB_06292013_postSMmtg.v41  #run z ntuples (for matrix method studies). Add 1 sigma up/down stat. uncertainties for reco and trig. added smart_killer

input=/share/t3data3/antonk/ana/ana_v29I_07012013_postSMmtg_datarw_stacoCB_all/pt20
DB=DB_07012013_postSMmtg_datarw.v42 # updated Max's data-rw class - it now uses smoothed histograms
DB=DB_07012013_postSMmtg_datarw.v43 # added recoProbe to reco SF. added SFstat to stacks. added MCModeling. qcd in stack plots increased from 4% to 10%

input=/share/t3data3/antonk/ana/ana_v29I_07082013_postSMmtg_recofix_stacoCB_all/pt20
DB=DB_07082013_postSMmtg_recofix.v44   # fixed missing reco toy SF

input=/share/t3data3/antonk/ana/ana_v29I_07112013_postSMmtg_datarw_stacoCB_all/pt20
DB=DB_07112013_postSMmtg_datarw.v45   # fixed data rw: use QCD estimates from pt x eta bins

input=/share/t3data3/antonk/ana/ana_v29I_07122013_postSMmtg_datarw_stacoCB_all/pt20
DB=DB_07122013_postSMmtg_datarw.v46   # Updated to new lumi tag and ttbar xsec. fixed data rw: max said previous version was inverted

input=/share/t3data3/antonk/ana/ana_v29I_07122013_postSMmtg_datarw_ret_stacoCB_all/pt20
DB=DB_07122013_postSMmtg_datarw.v47   # re-run without retries. Implementing QCD fits with ewk and top up/down variations
#v48 - same as v47, but doing baysian unfolding as nominal for 1D and 2D

input=/share/t3data3/antonk/ana/ana_v29I_08282013_isoMC_cwdebug_stacoCB_all/pt20
DB=DB_08282013_isoMC.v50   # iso SFs separate for powhegHerig and mcNLO (per Max, reduces theory systematics). AFTER uct3 xrood deletions!
DB=DB_08282013_isoMC.v51   # same, but playing with a breakdown of MC's into sub-components (WW,WZ,ZZ,single-top,ttbar)

input=/share/t3data3/antonk/ana/ana_v29I_09132013_isoMC_cwdebug_iso20_stacoCB_all/pt20
DB=DB_08282013_isoMC.v52   # same, but using 20% fluctuations in iso SF (for Max)

input=/share/t3data3/antonk/ana/ana_v29I_09132013_isoMC_cwdebug_iso2d_ret_stacoCB_all/pt20
DB=DB_08282013_isoMC.v53   # same, but using 2d iso SF and no variations (for Max)

input=/share/t3data3/antonk/ana/ana_v29I_09202013_isoMC_reweights_stacoCB_all/pt20
DB=DB_09202013_reweights.v54   # revert to v50/v51, but add some extra histograms

# v55: adding z0 cut. DO NOT USE
#input=/share/t3data3/antonk/ana/ana_v29I_09302013_addz0_stacoCB_all/pt20
#DB=DB_09302013_addz0.v55       # added mu z0 cut for cosmics

input=/share/t3data3/antonk/ana/ana_v29I_09202013_isoMC_reweights_stacoCB_all/pt20
DB=DB_09202013_reweights.v56   # same as v54 (and v50/v51): per-MC pt-only iso SF. Fixing qcdsys3d.py problem with broken-down components and top.

input=/share/t3data3/antonk/ana/ana_v29I_10042013_maxisoMC_stacoCB_all/pt20
DB=DB_10042013_maxisoMC.v57    # same as v56, but upgraded ScaleFactorProvider & new iso ROOT files

# input=/share/t3data3/antonk/ana/ana_v29I_10042013_maxisoMC2d_stacoCB_all/pt20
# DB=DB_10042013_maxisoMC2d.v58   # same as v57, but using 2d iso SFs

input=/share/t3data3/antonk/ana/ana_v29I_11282013_dinosisoMC_stacoCB_all/pt20
DB=DB_11282013_dinosisoMC.v60     # nominal, using latest iso SF from Dinos. Nominal and errors from Dinos, MC@NLO and PowhegHerwig from Max. pt-only.

#############
# Fall 2014: recovering the code
#############
input=/share/t3data3/antonk/ana/ana_v29I_07022014_slc6_stacoCB_all/pt20
DB=DB_07022014_dinosisoMC.v70   # repeating v60 after migrating to SLC6

input=/share/t3data3/antonk/ana/ana_v29I_09092014_slc6_3dmc_stacoCB_all/pt20
DB=DB_09092014_dinosisoMC.v71   # per Max's email, running with 2D iso SF. Note that only nominal fits changed.



INDB=`echo $DB | sed -e 's#DB#IN#g'`
OUTDB=`echo $DB | sed -e 's#DB#OUT#g'`
iq=0
bgsig=5
bgqcd=4
