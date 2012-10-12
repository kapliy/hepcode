#!/usr/bin/env python

""" a version for FlatNtupler """ 

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
for v in ('vmu_pt','vmu_eta','vmu_phi','vmu_msqoverp','vmu_mstheta','vmu_msphi','vmu_q'):
    p.attrs.append(v)
    setattr(p,v,ROOT.std.vector('float')())
# vector-of-vector version:
for v in ('vvmu_wet','vvmu_wpx','vvmu_wpy'):
    p.attrs.append(v)
    setattr(p,v,ROOT.std.vector('vector<float>')())
for v in ('vvmu_status',):
    p.attrs.append(v)
    setattr(p,v,ROOT.std.vector('vector<unsigned int>')())

fname='flatntuple.root'
f = ROOT.TFile.Open(fname)
t = f.tree
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
        s.px += _px
        s.py += _py
        s.et += _et
    def add2(s,_et,_phi,_sumet):
        """ pt-phi version """
        s.px += _et*math.cos(_phi)
        s.py += _et*math.sin(_phi)
        s.et += _sumet
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
    old_muonboy = AnaMet()
    old_muonboy.add2(t.met_muonboy,t.met_muonboy_phi,t.met_muonboy_sumet)
    old_muid = AnaMet()
    old_muid.add2(t.met_muid,t.met_muid_phi,t.met_muid_sumet)
    old_refmuon = AnaMet()
    old_refmuon.add2(t.met_refmuon,t.met_refmuon_phi,t.met_refmuon_sumet)
    new_muonboy = AnaMet()
    new_muid = AnaMet()
    new_refmuon = AnaMet()
    athena = AnaMet()
    p.clear()
    metutil_vv.reset()
    has_nmu=False
    for imu in xrange(t.nmu):
        # staco only:
        if not t.mu_type[imu]==0: continue
        mspt = t.mu_ptms[imu]
        mspx,mspy = mspt*math.cos(t.mu_phims[imu]),mspt*math.sin(t.mu_phims[imu])
        mept = t.mu_ptexms[imu]
        mepx,mepy = mept*math.cos(t.mu_phiexms[imu]),mept*math.sin(t.mu_phiexms[imu])
        idpt = t.mu_ptid[imu]
        idpx,idpy = idpt*math.cos(t.mu_phiid[imu]),idpt*math.sin(t.mu_phiid[imu])
        iept = t.mu_ptexid[imu]
        iepx,iepy = idpt*math.cos(t.mu_phiexid[imu]),idpt*math.sin(t.mu_phiexid[imu])
        pt = t.mu_pt[imu]
        px = pt*math.cos(t.mu_phi[imu])
        py = pt*math.sin(t.mu_phi[imu])
        st = status = int(t.mu_MET_status[imu])
        wpx = t.mu_MET_wpx[imu]
        wpy = t.mu_MET_wpy[imu]
        wet = t.mu_MET_wet[imu]
        # METUtil package variables:
        if True: #vector<vector> version
            vmu_statusA = ROOT.std.vector('unsigned int')()
            vmu_wetA = ROOT.std.vector('float')()
            vmu_wpxA = ROOT.std.vector('float')()
            vmu_wpyA = ROOT.std.vector('float')()
            [vmu_statusA.push_back(int(v)) for v in (t.mu_MET_status[imu],t.mu_MET_status2[imu])]
            [vmu_wetA.push_back(v) for v in (t.mu_MET_wet[imu],t.mu_MET_wet2[imu])]
            [vmu_wpxA.push_back(v) for v in (t.mu_MET_wpx[imu],t.mu_MET_wpx2[imu])]
            [vmu_wpyA.push_back(v) for v in (t.mu_MET_wpy[imu],t.mu_MET_wpy2[imu])]
            p.vvmu_status.push_back(vmu_statusA)
            p.vvmu_wet.push_back(vmu_wetA)
            p.vvmu_wpx.push_back(vmu_wpxA)
            p.vvmu_wpy.push_back(vmu_wpyA)
        p.vmu_pt.push_back(t.mu_pt[imu])
        p.vmu_eta.push_back(t.mu_eta[imu])
        p.vmu_phi.push_back(t.mu_phi[imu])
        # extra
        msp = mspt*math.cosh(t.mu_etams[imu])
        msqoverp = t.mu_qms[imu]/msp
        p.vmu_msqoverp.push_back(msqoverp)
        p.vmu_mstheta.push_back(2*math.atan(math.e**(t.mu_etams[imu])))
        p.vmu_msphi.push_back(t.mu_phims[imu])
        p.vmu_q.push_back(t.mu_qms[imu])
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
    #print 'RefMuon %#2d %d\t%d :'%(evt+1,t.run,t.event),old_refmuon,' | ',new_refmuon,'***',(old_refmuon==new_refmuon)
    print 'MuonBoy %#2d %d\t%d :'%(evt+1,t.run,t.event),old_muonboy,' | ',new_muonboy,'***',(old_muonboy==new_muonboy)
    if (not (old_muonboy==new_muonboy)):
        if not (old_muonboy==new_muonboy):
            toterr +=1
        print ' ST = |UNKNOWN|DEFAULT|SPECTRO|TRACK|REFMUON|MUID|'
        for imu in xrange(t.nmu):
            if not t.mu_type[imu]==0: continue
            pt = t.mu_pt[imu]
            px = pt*math.cos(t.mu_phi[imu])
            py = pt*math.sin(t.mu_phi[imu])
            print '----> MUON %d : px,py = %.5f\t%.5f'%(imu,px,py)
            print '--> status:',['%s'%status_mask(int(v)) for v in (t.mu_MET_status[imu],t.mu_MET_status2[imu])]
            print '--> wet:',['%.5f'%v for v in (t.mu_MET_wet[imu],t.mu_MET_wet2[imu])]
            print '--> wpx:',['%.5f'%v for v in (t.mu_MET_wpx[imu],t.mu_MET_wpx2[imu])]
            print '--> wpy:',['%.5f'%v for v in (t.mu_MET_wpy[imu],t.mu_MET_wpy2[imu])]
print 'TOTAL ERRORS:',toterr
