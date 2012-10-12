#!/usr/bin/env python

import sys,math
from math import cos,sin,sqrt
import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gROOT.ProcessLine('.L loader.C+')
true,false = True,False
GeV = 1000.0

#ROOT.gSystem.Load('/usr/lib64/libxml2.so')
ROOT.gSystem.Load('/cvmfs/atlas.cern.ch/repo/sw/software/x86_64-slc5-gcc43-opt/17.6.0/sw/lcg/app/releases/ROOT/5.34.00/x86_64-slc5-gcc43-opt/root/lib/libXMLParser.so')
ROOT.gSystem.Load('/home/antonk/TrigFTKAna/CommonAnalysis/GoodRunsLists/StandAlone/libGoodRunsLists.so')

name='/home/antonk/TrigFTKAna/good_run_lists/asym_data11_7TeV.pro10.DtoM.xml'
grlR = ROOT.Root.TGoodRunsListReader()
grl = ROOT.Root.TGoodRunsList()
grlR.SetXMLFile(name)
grlR.Interpret()
