
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
