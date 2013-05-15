#!/usr/bin/env python

""" Studying acceptance with different generators and PDF reweighting targets """

import sys,os,re,array
import common
import ROOT
ROOT.gROOT.SetBatch(True)
ROOT.TH1.SetDefaultSumw2()
from binning import *

def make_habseta(name='habseta_template'):
    """ makes an instance of abseta histogram with detector-motivated binning """
    bins = absetabins
    a = array.array('f',bins)
    return ROOT.TH1D(name,name,len(a)-1,a)

DIR = '/share/t3data3/antonk/ana/ana_v29I_11192012_edboard_nophi_stacoCB_all'

Q = 'plus'
if len(sys.argv)>1:
    Q = sys.argv[1]
assert Q in ('plus','min'), 'Usage: %s [plus/min]'%sys.argv[0]
tQ = 'POS' if Q=='plus' else 'NEG'

GENS = [ 'powheg_pythia','powheg_herwig', 'mcnlo' ]
FNAMES = [ 'unfold_tree_%s_%s.root'%(GEN,Q) for GEN in GENS ]

TNAMES = []
TNAMES.append( 'Nominal' )
TNAMES.append( 'NominalPdfMSTW' )
TNAMES.append( 'NominalPdfNNPDF' )
TNAMES.append( 'NominalPdfHERA' )
TNAMES.append( 'NominalWptSherpa' )

FILES = [ ROOT.TFile.Open('%s/%s'%(DIR,fname),'READ') for fname in FNAMES ]
assert all(FILES)

LABELS = GENS + TNAMES[1:]
TREES = [ FILE.Get('physics_%s'%(TNAMES[0])) for FILE in FILES ]
TREES += [ FILES[0].Get('physics_%s'%(TNAME)) for TNAME in TNAMES[1:] ]
assert all(TREES)

# data retrieval
ACC = []
PTC = []
cache = ROOT.TFile.Open('cache/acceptance.root','UPDATE')
for i,t in enumerate(TREES):
    print 'Processing:',LABELS[i]
    HISTOS = []
    NAMES = []
    NAMES.append( tQ + '_' + LABELS[i]+'_num25' )
    NAMES.append( tQ + '_' + LABELS[i]+'_num20' )
    NAMES.append( tQ + '_' + LABELS[i]+'_num' )
    NAMES.append( tQ + '_' + LABELS[i]+'_den' )
    CUTS = []
    CUTS.append( 'Truth_Mc_Weight*(Truth_MtW>40)*(Truth_PtNu>25)*(abs(Truth_EtaLep)<=2.4)*(Truth_PtLep>25)' )
    CUTS.append( 'Truth_Mc_Weight*(Truth_MtW>40)*(Truth_PtNu>25)*(abs(Truth_EtaLep)<=2.4)*(Truth_PtLep>20)' )
    CUTS.append( 'Truth_Mc_Weight*Truth_Is_Fiducial' )
    CUTS.append( 'Truth_Mc_Weight' )
    if cache.Get(NAMES[0]):
        HISTOS = [ cache.Get(NAME) for NAME in NAMES ]
    else:
        cache.cd()
        for j,NAME in enumerate(NAMES):
            h = make_habseta(NAME)
            t.Draw("abs(Truth_EtaLep)>>%s"%NAME,CUTS[j])
            h.Write()
            HISTOS.append(h)
    # acceptance
    hACC = HISTOS[2].Clone()
    hACC.Divide(HISTOS[3])
    ACC.append(hACC)
    # acceptance of pt=25 GeV cut
    hPTC = HISTOS[0].Clone()
    hPTC.Divide(HISTOS[2])
    PTC.append(hPTC)

# plotting
from SuCanvas import *
SuCanvas.savedir = './'
SuCanvas.savetypes = ['png','pdf']
SuCanvas.g_lin_ratio_y_title_offset = 1.7
SuCanvas.g_marker_size = 0.9
SuCanvas.g_legend_x1_ndc = 0.45
SuCanvas.g_text_size = 18
SuCanvas.g_legend_height_per_entry = 0.043
SuCanvas.g_lin_ratio_y_title_offset = 2.0
SuCanvas.g_lin_main_y_title_offset = 2.0
SuCanvas.cgStyle = SuCanvas.ControlPlotStyle()

SuCanvas._refLineMin = 0.99
SuCanvas._refLineMax = 1.02

M = PlotOptions()
M.msize = 1.1
size = SuCanvas.g_marker_size
M.add('Nominal','Powheg+Pythia (CT10)',ratio=True,size=size,style=None,color=1)
M.add('Nominal','Powheg+Herwig (CT10)',ratio=True,size=size,style=None)
M.add('Nominal','MC@NLO (CT10)',ratio=True,size=size,style=None)
M.add('Nominal','Powheg+Pythia (MSTW)',ratio=True,size=size,style=None)
M.add('Nominal','Powheg+Pythia (NNPDF)',ratio=True,size=size,style=None)
M.add('Nominal','Powheg+Pythia (HERA)',ratio=True,size=size,style=None)
M.add('Nominal','Powheg+Pythia (Sherpa wpt)',ratio=True,size=size,style=None)

xaxis_info = LABELMAP['lepton_absetav']

c = SuCanvas('acceptance_%s_total'%tQ)
c._ratioName = "Systematic / Nominal"
c.plotAny(ACC,M=M,height=[0.55,0.85],xaxis_info=xaxis_info)
c.SaveSelf()

c2 = SuCanvas('acceptance_%s_25GeV'%tQ)
c2._ratioName = "Systematic / Nominal"
c2.plotAny(PTC,M=M,height=[0.85,1.0],xaxis_info=xaxis_info)
c2.SaveSelf()

cache.Close()
os._exit(0)
