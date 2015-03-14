These programs and utilities are used to manipulate ATLAS ntuples, run event selection, plot various distributions for W/Z boson candidates, perform unfolding, etc.

# W/Z Analysis Code #
  * **[ana/trunk](https://code.google.com/p/hepcode/source/browse/#svn%2Fana%2Ftrunk)** - C++ / HTCondor analysis package that runs over 5-10 TB of slimmed AOD ntuples and generates a few trees and several thousand histograms with W and Z event candidates.
  * **[roofit/trunk](https://code.google.com/p/hepcode/source/browse/#svn%2Froofit%2Ftrunk)** - Python / pyROOT post-analysis package that performs statistical interpretation of the data and creates control plots, formatted tables, cutflows, and a host of other things needed for the W/Z cross-section measurement paper.

# General-Interest Packages #
  * **[lha/trunk](https://code.google.com/p/hepcode/source/browse/#svn%2Flha%2Ftrunk)** - a PDF reweighing class that wraps LHAPDF fortran code. Provides both C++ and Python interfaces.
  * **[xrootd/trunk](https://code.google.com/p/hepcode/source/browse/#svn%2Fxrootd%2Ftrunk)** - a highly parallel data replicator between global HEP grid (DQ2) and local XROOTD or NFS storage.

# Specialized Packages #
  * **check/trunk** - constructs lists of FlatNtupler ntuples, validates event counts, and quickly looks at individual events
  * **d3pd/trunk** - performs basic W/Z cutflow using FlatNtupler vs D3PDs (in Python)
  * **evgen/trunk** - creates 1D or 2D reweighing histograms between different generators
  * **fitter** - performs toy experiments to evaluate bias and pulls on TFractionFitter template fits
  * **powerpoint/trunk** - a LaTex-beamer presentation template
  * **slim/trunk** - a distributed ntuple slimming framework for PBS
  * **wmunu/trunk** - a simple python-based event looper