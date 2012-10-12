#!/usr/bin/env python

""" A version that uses modified libMETUtility that accepts references.
i.e., all arrays are constructed by hand!
"""

import sys,math
import ROOT
ROOT.gROOT.SetBatch(1)
ROOT.gROOT.ProcessLine('.L loader.C+')
true,false = True,False

# configure METUtility package:
ROOT.gSystem.Load('/home/antonk/d3pd/MissingETUtility/StandAlone/libMETUtility.so')
metutil_vv = ROOT.METUtility()
metutil_vv.setCellFix( false );
metutil_vv.doForwardEtaCut( false );
metutil_vv.setPileUpUncertainty( 0.1 );

class P:
    def __init__(s):
        s.attrs = []
        pass
    def clear(s):
        for a in s.attrs:
            getattr(s,a).clear()
p = P()
# original version:
for v in ('vmu_pt','vmu_eta','vmu_phi','vmu_wet','vmu_wpx','vmu_wpy','vmu_msqoverp','vmu_mstheta','vmu_msphi','vmu_q'):
    p.attrs.append(v)
    setattr(p,v,ROOT.std.vector('float')())
for v in ('vmu_status',):
    p.attrs.append(v)
    setattr(p,v,ROOT.std.vector('unsigned short')())
# vector-of-vector version:
for v in ('vvmu_wet','vvmu_wpx','vvmu_wpy'):
    p.attrs.append(v)
    setattr(p,v,ROOT.std.vector('vector<float>')())
for v in ('vvmu_status',):
    p.attrs.append(v)
    setattr(p,v,ROOT.std.vector('vector<unsigned int>')())

fname='file.root'
f = ROOT.TFile.Open(fname)
t = f.physics
t.ls()

nentries = t.GetEntries()
nproc = min(nentries,100)
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
        s.px += _et/1000.0*math.cos(_phi)
        s.py += _et/1000.0*math.sin(_phi)
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
    athena = AnaMet()
    p.clear()
    metutil_vv.reset()
    has_nmu=False
    for imu in xrange(t.mu_staco_MET_n):
        if not t.mu_staco_MET_statusWord[imu].size()==1:
            has_nmu=True
            #print 'EVT',evt,'muon',imu,': statusword size = ',t.mu_staco_MET_statusWord[imu].size()
            #print t.mu_staco_MET_statusWord[imu][0],t.mu_staco_MET_statusWord[imu][1]
        msp = abs(1.0/t.mu_staco_ms_qoverp[imu])
        mspt = msp*math.sin(t.mu_staco_ms_theta[imu])
        mspx,mspy = mspt*math.cos(t.mu_staco_ms_phi[imu]),mspt*math.sin(t.mu_staco_ms_phi[imu])
        mep = abs(1.0/t.mu_staco_me_qoverp[imu])
        mept = mep*math.sin(t.mu_staco_me_theta[imu])
        mepx,mepy = mept*math.cos(t.mu_staco_me_phi[imu]),mept*math.sin(t.mu_staco_me_phi[imu])
        idp = abs(1.0/t.mu_staco_id_qoverp[imu])
        idpt = idp*math.sin(t.mu_staco_id_theta[imu])
        idpx,idpy = idpt*math.cos(t.mu_staco_id_phi[imu]),idpt*math.sin(t.mu_staco_id_phi[imu])
        iep = abs(1.0/t.mu_staco_ie_qoverp[imu])
        iept = iep*math.sin(t.mu_staco_ie_theta[imu])
        iepx,iepy = iept*math.cos(t.mu_staco_ie_phi[imu]),iept*math.sin(t.mu_staco_ie_phi[imu])
        px = t.mu_staco_px[imu]
        py = t.mu_staco_py[imu]
        pt = t.mu_staco_pt[imu]
        st = status = t.mu_staco_MET_statusWord[imu][0]
        wpx = t.mu_staco_MET_wpx[imu][0]
        wpy = t.mu_staco_MET_wpy[imu][0]
        wet = t.mu_staco_MET_wet[imu][0]
        # METUtil package variables:
        p.vmu_status.push_back(st)
        p.vmu_wet.push_back(t.mu_staco_MET_wet[imu][0])
        p.vmu_wpx.push_back(t.mu_staco_MET_wpx[imu][0])
        p.vmu_wpy.push_back(t.mu_staco_MET_wpy[imu][0])
        if True: #vector<vector> version
            vmu_statusA = ROOT.std.vector('unsigned int')()
            vmu_wetA = ROOT.std.vector('float')()
            vmu_wpxA = ROOT.std.vector('float')()
            vmu_wpyA = ROOT.std.vector('float')()
            [vmu_statusA.push_back(v) for v in t.mu_staco_MET_statusWord[imu]]
            [vmu_wetA.push_back(v) for v in t.mu_staco_MET_wet[imu]]
            [vmu_wpxA.push_back(v) for v in t.mu_staco_MET_wpx[imu]]
            [vmu_wpyA.push_back(v) for v in t.mu_staco_MET_wpy[imu]]
            p.vvmu_status.push_back(vmu_statusA)
            p.vvmu_wet.push_back(vmu_wetA)
            p.vvmu_wpx.push_back(vmu_wpxA)
            p.vvmu_wpy.push_back(vmu_wpyA)
        p.vmu_pt.push_back(t.mu_staco_pt[imu])
        p.vmu_eta.push_back(t.mu_staco_eta[imu])
        p.vmu_phi.push_back(t.mu_staco_phi[imu])
        # extra
        p.vmu_msqoverp.push_back(t.mu_staco_ms_qoverp[imu])
        p.vmu_mstheta.push_back(t.mu_staco_ms_theta[imu])
        p.vmu_msphi.push_back(t.mu_staco_ms_phi[imu])
        p.vmu_q.push_back(t.mu_staco_charge[imu])
        # correction from my reimplementation of what's in METUtil package:
        if (st & REFMUON): continue
        if (st & MUID): continue
        if (st & DEFAULT):
            athena.add1(-px*wpx,-py*wpy,pt*wet,0,1)
        elif (st & SPECTRO):
            athena.add1(-mspx*wpx,-mspy*wpy,mspt*wet,0,1)
        elif (st & TRACK):
            athena.add1(-idpx*wpx,-idpy*wpy,idpt*wet,0,1)

    metutil_vv.setMuonParameters( p.vmu_pt , p.vmu_eta , p.vmu_phi , p.vvmu_wet , p.vvmu_wpx , p.vvmu_wpy , p.vvmu_status );
    metutil_vv.setExtraMuonParameters( p.vmu_msqoverp , p.vmu_mstheta , p.vmu_msphi , p.vmu_q );
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
