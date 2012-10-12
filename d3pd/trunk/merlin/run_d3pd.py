#!/usr/bin/env python

"""
A standalone program that recomputes the MuonTotal term from D3PDs
"""

# MAKE SURE TO UPDATE SYMLINK SO IT POINTS TO THIS FILE
fname='NTUP_SMWZ.655691._000002.root.1'
# (from mc11_7TeV.107664.AlpgenJimmyZmumuNp4_pt20.merge.NTUP_SMWZ.e835_s1299_s1300_r3043_r2993_p833_tid655691_00)

import sys,math
from math import cos,sin
import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gROOT.ProcessLine('.L loader.C+')
true,false = True,False

# configure METUtility package:
ROOT.gSystem.Load('MissingETUtility-00-02-14/StandAlone/libMETUtility.so')
metutil = ROOT.METUtility()
metutil.setCellFix( false );
metutil.doForwardEtaCut( false );
metutil.setPileUpUncertainty( 0.1 );

f = ROOT.TFile.Open(fname)
t = f.physics
t.ls()

nentries = t.GetEntries()
nproc = min(nentries,100)
toterr=0

class AnaMet:
    """ A small re-implementation of MET class with usual semantics
    All units are automatically converted to GeV
    """
    def __init__(s):
        s.px = 0
        s.py = 0
        s.et = 0
    def etx(s):
        return s.px
    def ety(s):
        return s.py
    def sumet(s):
        return s.et
    def add1(s,_px,_py,_et,crap1,crap2):
        """ px-py version """
        s.px += _px/1000.0
        s.py += _py/1000.0
        s.et += _et/1000.0
    def add2(s,_et,_phi,_sumet):
        """ pt-phi version """
        s.px += _et/1000.0*cos(_phi)
        s.py += _et/1000.0*sin(_phi)
        s.et += _sumet/1000.0
    def __eq__(s,other):
        """ A function that checks for equality of two AnaMet objects (to within 1 MeV) """
        if abs(s.px-other.px)>0.001: return False
        if abs(s.py-other.py)>0.001: return False 
        if abs(s.et-other.et)>0.001: return False
        if False:
            if '%.2f'%s.px != '%.2f'%other.px: return False
            if '%.2f'%s.py != '%.2f'%other.py: return False
            if '%.2f'%s.et != '%.2f'%other.et: return False
        return True
    def pad(s,f):
        """ Adds white space padding for prettier printing """
        if f<=-1000:
            return ''
        elif f<=-100:
            return ''
        elif f<=-10:
            return ' '
        elif f<0:
            return '  '
        elif f<10:
            return '   '
        elif f<100:
            return '  '
        elif f<1000:
            return ' '
        return ''
    def __str__(s):
        res = '%s%.5f\t%s%.5f\t%s%.5f'%(s.pad(s.etx()),s.etx(),s.pad(s.ety()),s.ety(),s.pad(s.sumet()),s.sumet())
        return res

UNKNOWN           = 0x0000
DEFAULT           = 0x0001
SPECTRO           = 0x0002
TRACK             = 0x0004
REFMUON           = 0x0008
MUID              = 0x0010
def status_mask(st):
    """ Unfold the status word """
    return '%d: %d%d%d%d%d%d'%(st,st&UNKNOWN!=0,st&DEFAULT!=0,st&SPECTRO!=0,st&TRACK!=0,st&REFMUON!=0,st&MUID!=0)

for evt in xrange(nproc):
    t.GetEntry(evt)
    assert t.mu_staco_n == t.mu_staco_MET_n
    # grab the pre-computed version of MET_MuonBoy
    old_muonboy = AnaMet()
    old_muonboy.add2(t.MET_MuonBoy_et,t.MET_MuonBoy_phi,t.MET_MuonBoy_sumet)
    new_muonboy = AnaMet()
    # re-calculate MET_MuonBoy from components
    metutil.reset()
    metutil.setMuonParameters(t.mu_staco_pt, t.mu_staco_eta, t.mu_staco_phi, t.mu_staco_MET_wet, t.mu_staco_MET_wpx, t.mu_staco_MET_wpy, t.mu_staco_MET_statusWord);
    metutil.setExtraMuonParameters(t.mu_staco_ms_qoverp, t.mu_staco_ms_theta, t.mu_staco_ms_phi, t.mu_staco_charge);
    m = metutil.getMissingET( "MuonTotal" )
    new_muonboy.add2(m.et(), m.phi(),m.sumet())
    # print the results and compare the two terms (pre-computed one vs METUtils)
    print 'MuonBoy %#2d %d\t%d :'%(evt+1,t.RunNumber,t.EventNumber),old_muonboy,' | ',new_muonboy,'***',(old_muonboy==new_muonboy)
    # in case of disagreement, print a verbose summary of muons in this event:
    if (not (old_muonboy==new_muonboy)):
        toterr +=1
        print ' ST = |UNKNOWN|DEFAULT|SPECTRO|TRACK|REFMUON|MUID|'
        for imu in xrange(t.mu_staco_MET_n):
            print '----> MUON %d : px,py = %.5f\t%.5f'%(imu,t.mu_staco_px[imu]/1000.0,t.mu_staco_py[imu]/1000.0)
            print '--> status:',['%s'%status_mask(v) for v in t.mu_staco_MET_statusWord[imu]]
            print '--> wet:',['%.5f'%v for v in t.mu_staco_MET_wet[imu]]
            print '--> wpx:',['%.5f'%v for v in t.mu_staco_MET_wpx[imu]]
            print '--> wpy:',['%.5f'%v for v in t.mu_staco_MET_wpy[imu]]
print 'TOTAL ERRORS:',toterr
