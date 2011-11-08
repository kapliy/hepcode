#!/usr/bin/env python

"""
http://alxr.usatlas.bnl.gov/lxr-stb6/source/atlas/Generators/

mc11_7TeV.106044.PythiaWmunu_no_filter.merge.AOD.e815_s1272_s1274_r2730_r2700/
+MRST+MC+fit+-+alpha_S+k_T^2
/afs/cern.ch/atlas/software/releases/17.0.5/AtlasSimulation/17.0.5/InstallArea/jobOptions/MC11JobOptions/MC11.106044.PythiaWmunu_no_filter.py
Pythia.Tune_Name=ATLAS_20110002

CTEQ66
MSTW2008lo80cl
MRST2007lomod
HERAPDF15NNLOEIG

In the case you want to run Pythia with the LHAPDF structure functions you need to set
the mstp(52), mstp(54) and mstp(56) variables equal to 2 and the mstp(51), mstp(53) and
mstp(55) ones to the LHAPDF set/member index (see the documentation of the Genera-
tors/Lhapdf i package for the LHAPDF set/member index settings).
"""

import lhapdf
LHPDF,LHGRID=0,1

pdfsets = ['HERAPDF10_EIG','MSTW2008nlo68cl','NNPDF21_100','cteq66','MRST2007lomod']

pdfname_orig=pdfsets[3] #kristin (MC@NLO?)
pdfname_orig=pdfsets[4] #pythia
pdfname_reweight = pdfsets[1]

lhapdf.initPDFSetByName(1,pdfname_orig,LHGRID);
lhapdf.initPDFSetByName(2,pdfname_reweight,LHGRID);

num1 = lhapdf.numberPDF(1)+1
num2 = lhapdf.numberPDF(2)+1
print num1,num2

lhapdf.initPDF(1,0)

partonX1=0.1
eventQ=66
partonID1=3
p1density_orig = lhapdf.xfx(1,partonX1, eventQ, partonID1);
print p1density_orig
#p2density_orig = lhapdf.xfx(1,partonX2, eventQ, partonID2);
