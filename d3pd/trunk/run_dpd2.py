#!/usr/bin/env python

""" A version that uses unmodified libMETUtility """

import sys,math
from math import cos,sin
import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gROOT.ProcessLine('.L loader.C+')
true,false = True,False

# configure METUtility package:
ROOT.gSystem.Load('/home/antonk/d3pd/MissingETUtility-00-02-14/StandAlone/libMETUtility.so')
metutil_vv = ROOT.METUtility()
metutil_vv.setCellFix( false );
metutil_vv.doForwardEtaCut( false );
metutil_vv.setPileUpUncertainty( 0.1 );

fname='file.root'
f = ROOT.TFile.Open(fname)
t = f.physics
t.ls()

nentries = t.GetEntries()
nproc = min(nentries,10000)
toterr=0

class AnaMet:
    """ A small re-implementation of MET class with usual """
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
        if abs(s.px-other.px)>0.001: return False
        if abs(s.py-other.py)>0.001: return False 
        if abs(s.et-other.et)>0.001: return False
        if False:
            if '%.2f'%s.px != '%.2f'%other.px: return False
            if '%.2f'%s.py != '%.2f'%other.py: return False
            if '%.2f'%s.et != '%.2f'%other.et: return False
        return True
    def pad(s,f):
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
    return '%d: %d%d%d%d%d%d'%(st,st&UNKNOWN!=0,st&DEFAULT!=0,st&SPECTRO!=0,st&TRACK!=0,st&REFMUON!=0,st&MUID!=0)

for evt in xrange(nproc):
    t.GetEntry(evt)
    assert t.mu_staco_n == t.mu_staco_MET_n
    old_muonboy = AnaMet()
    old_muonboy.add2(t.MET_MuonBoy_et,t.MET_MuonBoy_phi,t.MET_MuonBoy_sumet)
    old_muid = AnaMet()
    old_muid.add2(t.MET_Muid_et,t.MET_Muid_phi,t.MET_Muid_sumet)
    old_refmuon = AnaMet()
    old_refmuon.add2(t.MET_RefMuon_et,t.MET_RefMuon_phi,t.MET_RefMuon_sumet)
    new_muonboy = AnaMet()
    new_muid = AnaMet()
    new_refmuon = AnaMet()
    metutil_vv.reset()
    has_nmu=False
    for imu in xrange(t.mu_staco_MET_n):
        if not t.mu_staco_MET_statusWord[imu].size()==1:
            has_nmu=True
            #print 'EVT',evt,'muon',imu,': statusword size = ',t.mu_staco_MET_statusWord[imu].size()
            #print t.mu_staco_MET_statusWord[imu][0],t.mu_staco_MET_statusWord[imu][1]
    metutil_vv.setObjects("jets", t.jet_AntiKt4LCTopo_pt, t.jet_AntiKt4LCTopo_eta, t.jet_AntiKt4LCTopo_phi, t.jet_AntiKt4LCTopo_E, t.jet_AntiKt4LCTopo_MET_wet, t.jet_AntiKt4LCTopo_MET_wpx, t.jet_AntiKt4LCTopo_MET_wpy, t.jet_AntiKt4LCTopo_MET_statusWord);
    metutil_vv.setExtraJetParameters(t.jet_AntiKt4LCTopo_GCWJES, t.jet_AntiKt4LCTopo_m, t.jet_AntiKt4LCTopo_eta, t.jet_AntiKt4LCTopo_phi);
    metutil_vv.setElectronParameters(t.el_pt, t.el_eta, t.el_phi, t.el_MET_wet, t.el_MET_wpx, t.el_MET_wpy, t.el_MET_statusWord);
    metutil_vv.setMuonParameters(t.mu_staco_pt, t.mu_staco_eta, t.mu_staco_phi, t.mu_staco_MET_wet, t.mu_staco_MET_wpx, t.mu_staco_MET_wpy, t.mu_staco_MET_statusWord);
    metutil_vv.setExtraMuonParameters(t.mu_staco_ms_qoverp, t.mu_staco_ms_theta, t.mu_staco_ms_phi, t.mu_staco_charge);
    metutil_vv.setMETTerm("RefGamma", t.MET_RefGamma_et*cos(t.MET_RefGamma_phi), t.MET_RefGamma_et*sin(t.MET_RefGamma_phi), t.MET_RefGamma_sumet);
    metutil_vv.setMETTerm("RefTau", t.MET_RefTau_et*cos(t.MET_RefTau_phi), t.MET_RefTau_et*sin(t.MET_RefTau_phi), t.MET_RefTau_sumet);
    metutil_vv.setMETTerm("CellOut", t.MET_CellOut_et*cos(t.MET_CellOut_phi), t.MET_CellOut_et*sin(t.MET_CellOut_phi), t.MET_CellOut_sumet);
    metutil_vv.setMETTerm("CellOutEflow", t.MET_CellOut_Eflow_et*cos(t.MET_CellOut_Eflow_phi), t.MET_CellOut_Eflow_et*sin(t.MET_CellOut_Eflow_phi), t.MET_CellOut_Eflow_sumet);
    m = metutil_vv.getMissingET( "MuonTotal" )
    new_muonboy.add2(m.et(), m.phi(),m.sumet())
    m = metutil_vv.getMissingET( "RefMuon" )
    new_refmuon.add2(m.et(), m.phi(),m.sumet())
    #print 'RefMuon %#2d %d\t%d :'%(evt+1,t.RunNumber,t.EventNumber),old_refmuon,' | ',new_refmuon,'***',(old_refmuon==new_refmuon)
    print 'MuonBoy %#2d %d\t%d :'%(evt+1,t.RunNumber,t.EventNumber),old_muonboy,' | ',new_muonboy,'***',(old_muonboy==new_muonboy)
    if (not (old_muonboy==new_muonboy)):
        if not (old_muonboy==new_muonboy):
            toterr +=1
        print ' ST = |UNKNOWN|DEFAULT|SPECTRO|TRACK|REFMUON|MUID|'
        for imu in xrange(t.mu_staco_MET_n):
            print '----> MUON %d : px,py = %.5f\t%.5f'%(imu,t.mu_staco_px[imu]/1000.0,t.mu_staco_py[imu]/1000.0)
            print '--> status:',['%s'%status_mask(v) for v in t.mu_staco_MET_statusWord[imu]]
            print '--> wet:',['%.5f'%v for v in t.mu_staco_MET_wet[imu]]
            print '--> wpx:',['%.5f'%v for v in t.mu_staco_MET_wpx[imu]]
            print '--> wpy:',['%.5f'%v for v in t.mu_staco_MET_wpy[imu]]
print 'TOTAL ERRORS:',toterr
