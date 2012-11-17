#!/usr/bin/env python

"""
Dumps histograms into latex
"""

import os,sys,re
import common
import ROOT

NBG = 'totalbg'
NQCD = 'qcd'
NSIG = 'wmunu_PowhegPythia'
NDATA = 'data'

fname = 'OUT_11022012_ALL.v2.eta.1D.root'
fin = ROOT.TFile.Open(fname,'READ')

SYS = []
SYS.append( ['MuonResMSUp','MuonResMSDown'] )
SYS.append( ['MuonResIDUp','MuonResIDDown'] )
SYS.append( ['MuonScaleKUp','MuonScaleKDown' ] )
SYS.append( ['MuonScaleCUp','MuonScaleCDown' ] )
SYS.append( ['MuonRecoSFUp','MuonRecoSFDown'] )
SYS.append( ['MuonTriggerSFUp','MuonTriggerSFDown'] )
SYS.append( ['MuonIsoSFUp','MuonIsoSFDown'] )
SYS.append( ['JetResolUp','JetResolDown'] )
SYS.append( ['JetScaleUp','JetScaleDown'] )
SYS.append( ['ResoSoftTermsUp_ptHard','ResoSoftTermsDown_ptHard'] )
SYS.append( ['ScaleSoftTermsUp_ptHard','ScaleSoftTermsDown_ptHard'] )
SYS.append( ['ScaleSoftTermsUp_ptHard','ScaleSoftTermsDown_ptHard'] )
SYS.append( ['Nominal_ewk_xsecdown','Nominal_ewk_xsecup'] )
