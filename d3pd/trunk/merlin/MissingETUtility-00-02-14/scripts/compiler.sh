#!/bin/bash

cd $TestArea/Reconstruction/MissingETUtility/cmt
make -f Makefile.Standalone

cd $TestArea/Reconstruction/MissingETUtility/run/JetUncertainties/cmt
make -f Makefile.Standalone

cd $TestArea/Reconstruction/MissingETUtility/run/JetResolution/cmt
make -f Makefile.Standalone


#for when these compile with minimal error
#cd $TestArea/Reconstruction/MissingETUtility/run/MuonMomentumCorrections/cmt
#make -f Makefile.Standalone

#cd $TestArea/Reconstruction/MissingETUtility/run/egammaAnalysisUtils/cmt
#make -f Makefile.Standalone

cd $TestArea/Reconstruction/MissingETUtility/run