#!/bin/bash

ntuple=29L
ntuple=29G
ntuple=29J

lbl=12032012_edboard       # include CalJets plots. Include debug_studies stuff in MC. Fixed met/lar cleaning: removed hard pt cut in lar, removed eta cut in both. Added back overlap removal.
lbl=12042012_edboard       # fixed big bug: powheg_herwig samples were reweighted via my 2d histos. Added correct set of NLO pdf weights. Only apply pdf reweighting to NLO samples. Reweight alpgen rapidity. MCNLO Z fix. 
lbl=12052012_edboard_25GeV # muon pt cut = 25 GeV
lbl=12052012_edboard_anywmt20 # anyfit relaxes wmt from 40 to 20
lbl=12052012_edboard_anywmt20_d03p5 # muon pt cut = 25 GeV; anyfit relaxes wmt from 40 to 20. With d0sig cut

lbl=01032013_edboard_newtrigSF_sumw2 # updated trigger SF from Carl; returned sumw2 in all histograms

lbl=01042013_edboard # added intgrated cross-section (via a large eta bin)
lbl=01052013_edboard # added intgrated cross-section (via a large eta bin)  same
lbl=01072013_edboard # added intgrated cross-section (via a large eta bin). Added response matrix for eta with ONE bin.
lbl=01102013_edboard # running data with pt>25 cut (for cutflow)

lbl=01152013_paper   # large rewrite of sf systematics: all st_w_XXX are renamed; wptw and pdf are moved to st_w folder; saving PDF vars for each CT10 member.
lbl=01152013_paper_fix  # retrying MC runs in an attempt to fix broken systematics. dgadd -u1

lbl=01182013_paper  # latest trigger phi corrections (works for multiple muons!). dgadd -u5 (again). stability fix from adrian.

lbl=01252013_paper  # added back jer (one-sided). added mcp unscaled and datascaled histograms. added uint - like int, but bin=0..1
lbl=01292013_paper  # just a typo fix in uint histograms
lbl=01292013_paper_noC   # disable C by setting sigma 2.5 ->10k
lbl=01292013_paper_noCK  # disable all MCP scaling
lbl=01292013_paper_oldC  # revert to old MCP C, which was used to derive SFs
lbl=01292013_paper_fineCK  # my earlier attempt to provide KC in bins of measurements (warning: large errors)
lbl=01292013_paper_anyfit  # just adding back the "anyfit" category, plus the total met correction
lbl=01292013_paper_anyfit_d0  # just adding back the "anyfit" category, plus the total met correction. PLUS d0 cut
lbl=01292013_paper_rawmet  # raw met (without propagation of energy scale uncertainties)
lbl=01292013_paper_higgsmcp  # MCP corrections from Higgs group

lbl=01292013_paper_ztrigboth  # multiply trigger SF from each muon
lbl=01292013_paper_revC  # reverse C corrections (change sign)
lbl=01292013_paper_anyfit_L  # re-running period L with full stats

lbl=02222013_paper # should be the same. removed anyfit, updated egammaUtils, added pythia11-data rw systematic target. adding new items to ntuple to debug A/C differences. added avgmu to ntuple
lbl=02232013_paper # added trigphi to ntuple. added trigall to ntuple (requires AND of trigger for all muons)
lbl=02282013_paper # added per-muon trigger weights. removed barrel-trigger words from ntuple


lbl=03012013_paper # start using new nominal W/Z reweighting target (based on Z-to-data)
lbl=03062013_paper # add additional trigger matching words to W (for debugging); nmuons20 and nmuons25; mcp trigger SF

lbl=03062013_paper # new version of ntuple (J): trigger matching cone=0.15; match muGirl and muGirlTag ONLY.
lbl=03152013_paper # removed different pdf systematics. Downloaded missing wmunu and zmumu MC (powheg+pythia)

lbl=03172013_paper # exclude bad bin (but not in ntuple)
lbl=03172013_paper_mu18 # exclude bad bin; use another mu18 trigger

lbl=04032013_nomg  # trying Carl's first version of non-MG trigger SFs
lbl=04032013_nomg_1000toys  # re-running with 1000 toy MCs

lbl=04032013_nomg_condor   # 3rd condor version

lbl=04232013_condor        # same as 04032013, but: added DtoK and LtoM systematics. added pileupUp and pileupDown systematics. commented out ntuple studies of A/C side. fixed double-saving of TNtuples. removed int, leaving only unint. - BAD RUN (files on t3data)
lbl=04242013_condor        # same as 04032013, but: added DtoK and LtoM systematics. added pileupUp and pileupDown systematics. commented out ntuple studies of A/C side. fixed double-saving of TNtuples. removed int, leaving only unint.
lbl=04252013               # added missing DtoK and LtoM histograms
lbl=04252013_rep           # just repeating with memory and speed optimizations

lbl=05102013_newmet    # updated met util (soft terms systematics). Use separate random seed for each period range for trigger SF.
lbl=05122013_newmet    # decoupled random seeds for mu+ and mu- per Jan's advice. Set both nvtx and avgmu in metutil
lbl=05132013_newmet    # fixed toy mc seed per agreement with Max. [never finished]
lbl=05142013_newmet    # fixed toy mc seed per agreement with Max. Independent toy MCs (total = 3000). removed eta x pt unfolding. commented out DtoK and LtoM stuff. full set of bootstrapping histograms in data
lbl=05142013_newmet_qindepSF  # trigger SF independent of charge

lbl=05182013_fullstat  # update to full mcnlo and powhegHerig stat. new egammaUtils: remove wpol constraint and apply jan's new experimental wpol reweights (mcnlo->powhegherwig or mcnlo/powhegherwig->powhegpythia) to reduce CW syst. fix bootstrapping to 1000 replicas and only run on baseline Nominal data.
lbl=05202013_fullstat      # fixed two bugs in bootstrap: int25 was wrong, and 2d used the wrong type cast
lbl=05202013_fullstat_ret  # retry before trying out running over OSG nodes
lbl=05202013_fullstat_noC  # removed curvature correction
lbl=06092013_recoSFphi     # MCP trunk scale factors (phi only)
lbl=06092013_recoSFphi_ret # updated RootCore, added compiler optimizations, unrolled response matrix loop; increased nsplits for sig, added anyfit

lbl=06132013_recoSFpt      # re-running same-old version, but after all updates, using latest MuonEfficiencyCorrections, but old eta x pt txt file
lbl=06132013_recoSFphi     # phi version (re-run)

lbl=06172013_recoBOTH_withZ # nominal is SFpt, but added a new st_w_bla option to do phi. Also: running z plots

lbl=06192013_recoBOTH_withZ # removed Z ntuple. disabled trig matching. updated reco SF class (correct errors). Added deltaPhi(soft,all-but-soft). Added a new systematic - reweight signal MC to data (Max's file).
lbl=06192013_recoBOTH_withZ_recoPhi  # Eta-phi SF are nominal
lbl=06192013_recoBOTH_withZ_pt25 # rerunning pt25 case with actual full statistics

lbl=06192013_recoBOTH_withZ_anyfit  # reco=(eta,phi)added anyfit to make MET plots. updated to new iso SFs. added trigphi, effphi, eff*trigphi
lbl=06192013_recoBOTH_withZ_rep     # reco=(eta,phi) add back trigger matching and z ntuple information
lbl=06192013_recoBOTH_withZ_etaonly # reco=(eta) fixed a bug that reverted etaonly flag  #NEVER RAN

lbl=06252013_newrecoSF              # new meaning of systematic: full difference between ID and CALO probes
lbl=06252013_newrecoSF_rep          # just a re-run: previous run was affected by some xrootd problem (just in case)
lbl=06252013_newrecoSF_MG           # MG version
lbl=06252013_newrecoSF_MG_rep       # MG version - rerunning to fix root_mc_powheg_pythia_wminmunu.root

lbl=06282013_postSMmtg              # same as below, but very buggy. not sure why. job taking 25 GB of RAM, jobs hanging on hadd, segfaulting on hadd
lbl=06292013_postSMmtg              # run z ntuples (for matrix method studies). Add 1 sigma up/down stat. uncertainties for reco and trig. noExit added. 06/29: added smart_killer. removed noExit
lbl=07012013_postSMmtg_datarw       # updated Max's data-rw class - it now uses smoothed histograms
lbl=07082013_postSMmtg_recofix      # fixed reco toy MC (they were broken)
lbl=07112013_postSMmtg_datarw       # another fix to max rw (v5)
lbl=07122013_postSMmtg_datarw       # Updated to new lumi tag and ttbar xsec. fixed data rw: max said previous version was inverted
lbl=07122013_postSMmtg_datarw_ret   # just re-running from scratch

lbl=08282013_isoMC_cwdebug            # isolation scale factors are re-run on per-MC basis. Added CW decomposition
lbl=09102013_isoMC_cwdebug            # Added more complete CW decomposition, including a couple of particularly-bad bins in eta/pt
lbl=09132013_isoMC_cwdebug_iso20      # iso SF use 20% variations (test for max)
lbl=09132013_isoMC_cwdebug_iso2d      # iso SF use 2d eta-pt; no sys. variations (test for max)
lbl=09132013_isoMC_cwdebug_iso2d_ret  # iso SF use 2d eta-pt; no sys. variations (test for max)

lbl=09202013_isoMC_reweights          # revert to normal per-MC isolation. Added some reweighting plots for thesis (final selection). Same as 08282013_isoMC_cwdebug!
lbl=09302013_addz0                    # added muon z0 to PV cut of 10 mm. v55

function rerun20() {
    submit_sample wasymmetry29I_mc_alpgen_herwig_wmunu_np0.dat
    submit_sample wasymmetry29I_mc_alpgen_herwig_wmunu_np2.dat
    submit_sample wasymmetry29I_mc_mcnlo_ttbar.dat
    submit_sample wasymmetry29I_mc_mcnlo_wt.dat
    submit_sample wasymmetry29I_mc_powheg_pythia_wminmunu.dat
    submit_sample wasymmetry29I_mc_powheg_pythia_zmumu.dat
    submit_sample wasymmetry29I_mc_pythia_zmumu.dat
}
function rerun25() {
    submit_sample wasymmetry29I_mc_powheg_pythia_wminmunu.dat
}

