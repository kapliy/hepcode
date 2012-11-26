#!/usr/bin/env python

""" EXAMPLE OF PDF REWEIGHTING USING D3PD EVENTS """

# EDIT THE PATH TO D3PD FILE:
fname =  "/home/antonk/d3pd/MAX/D3PD/mc11_7TeV.108298.PowHegWplusmunuPythia.merge.NTUP_SMWZ.e825_s1372_s1370_r3043_r2993_p833_tid742962_00/NTUP_SMWZ.742962._000396.root.1";

import os
import ROOT
ROOT.gROOT.SetBatch(1)

# LOAD D3PD FILE
f = ROOT.TFile.Open(fname,'READ'); assert f.IsOpen,'Unable to open file %s'%fname
t = f.Get('physics'); assert t,'Unable to find tree "physics" in the D3PD file'

# LOAD LHAPDF LIBRARY
ROOT.gSystem.Load('LHAPDF/lib/libLHAPDF.so')
ROOT.gInterpreter.AddIncludePath(os.path.join(os.getcwd(),'LHAPDF','include'))
ROOT.gROOT.ProcessLine('.L PDFReweightTool.hpp+')

# SET UP LHAPDF
rw = ROOT.PDFReweightTool()
if False:
    # Optional: specify a folder with LHAPdf files. If not specified, it will use the default $LHAPATH from athena
    rw.SetLHAPATH("/cvmfs/atlas.cern.ch/repo/sw/software/17.0.6/sw/lcg/external/MCGenerators/lhapdf/5.8.5/share/PDFsets")
# Initialize up to 4 PDF families. You can get set numbers at: http://lhapdf.hepforge.org/pdfsets
rw.AddPDFSet(10800,"CT10.LHgrid")
rw.AddPDFSet(21100,"MSTW2008nlo68cl.LHgrid")
rw.AddPDFSet(192800,"NNPDF21_100.LHgrid")
rw.Initialize()

for ievt in xrange(0,5):
    t.GetEntry(ievt)
    if t.mcevt_n>0:
        t.mcevt_pdf_scale.at(0)
        print '==========','EVENT',ievt,'=========='
        for iset in xrange(1,rw.NPDFSets()+1):
            print '----------','PDFSET%d'%iset,'----------'
            rw.GetEventWeight(iset,
                              t.mcevt_pdf_scale.at(0),
                              t.mcevt_pdf_id1.at(0),
                              t.mcevt_pdf_id2.at(0),
                              t.mcevt_pdf_x1.at(0),
                              t.mcevt_pdf_x2.at(0),
                              t.mcevt_pdf1.at(0),
                              t.mcevt_pdf2.at(0),
                              True);
