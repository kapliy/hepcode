#!/bin/bash
cd $TestArea/Reconstruction/MissingETUtility/run

#perhaps do this in one go, use svn to put them in macro, then compile
svn co $SVNROOT/Reconstruction/Jet/JetUncertainties/tags/JetUncertainties-00-03-05-01 JetUncertainties
svn co $SVNROOT/Reconstruction/Jet/JetResolution/tags/JetResolution-00-00-09 JetResolution
#this thing doesn't compile correctly
svn co $SVNROOT/Reconstruction/egamma/egammaAnalysis/egammaAnalysisUtils/tags/egammaAnalysisUtils-00-02-72 egammaAnalysisUtils
# so using this one
#svn co $SVNGRP/CombPerf/EGamma/Calibrations/EnergyRescalerTool/tags/EnergyRescalerTool-00-00-09 EnergyRescalerTool

# The trunk is needed so that the accessor functions for the scale uncertainties are available
svn co $SVNROOT/PhysicsAnalysis/MuonID/MuonIDAnalysis/MuonMomentumCorrections/tags/MuonMomentumCorrections-00-04-01 MuonMomentumCorrections
