#!/bin/bash

hadd -T unfold_histo_powheg_pythia_min.root mc_powheg_pythia_wminmunu/*unfold* &> LOG.histo.powheg_pythia_min &
hadd -T unfold_histo_powheg_pythia_plus.root mc_powheg_pythia_wplusmunu/*unfold* &> LOG.histo.powheg_pythia_plus &

hadd -T unfold_histo_powheg_herwig_min.root mc_powheg_herwig_wminmunu/*unfold* &> LOG.histo.powheg_herwig_min &
hadd -T unfold_histo_powheg_herwig_plus.root mc_powheg_herwig_wplusmunu/*unfold* &> LOG.histo.powheg_herwig_plus &

hadd -T unfold_histo_mcnlo_min.root mc_mcnlo_wminmunu/*unfold* &> LOG.histo.mcnlo_min &
hadd -T unfold_histo_mcnlo_plus.root mc_mcnlo_wplusmunu/*unfold* &> LOG.histo.mcnlo_plus &

# Disable TTree merging since they are so huge
if [ "0" == "1" ]; then
    hadd unfold_tree_powheg_pythia_min.root mc_powheg_pythia_wminmunu/*unfold* &> LOG.tree.powheg_pythia_min &
    hadd unfold_tree_powheg_pythia_plus.root mc_powheg_pythia_wplusmunu/*unfold* &> LOG.tree.powheg_pythia_plus &
    
    hadd unfold_tree_powheg_herwig_min.root mc_powheg_herwig_wminmunu/*unfold* &> LOG.tree.powheg_herwig_min &
    hadd unfold_tree_powheg_herwig_plus.root mc_powheg_herwig_wplusmunu/*unfold* &> LOG.tree.powheg_herwig_plus &
    
    hadd unfold_tree_mcnlo_min.root mc_mcnlo_wminmunu/*unfold* &> LOG.tree.mcnlo_min &
    hadd unfold_tree_mcnlo_plus.root mc_mcnlo_wplusmunu/*unfold* &> LOG.tree.mcnlo_plus &
fi;

echo "Wait for jobs to finish..."
wait
