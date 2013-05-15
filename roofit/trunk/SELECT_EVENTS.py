#!/usr/bin/env python
"""
awk 'NF==7 && $2!="Row"{print $4,$6}' selected_events.dat | sort > events.txt
"""

files = [
    '/share/ftkdata1/antonk/ana_v26_0830_all_stacoCB_fixmetcln/data_periodB/root_data_periodB.root',
    '/share/ftkdata1/antonk/ana_v26_0830_all_stacoCB_fixmetcln/data_periodD/root_data_periodD.root',
    '/share/ftkdata1/antonk/ana_v26_0830_all_stacoCB_fixmetcln/data_periodE/root_data_periodE.root',
    '/share/ftkdata1/antonk/ana_v26_0830_all_stacoCB_fixmetcln/data_periodF/root_data_periodF.root',
    '/share/ftkdata1/antonk/ana_v26_0830_all_stacoCB_fixmetcln/data_periodG/root_data_periodG.root',
    '/share/ftkdata1/antonk/ana_v26_0830_all_stacoCB_fixmetcln/data_periodH/root_data_periodH.root',
    '/share/ftkdata1/antonk/ana_v26_0830_all_stacoCB_fixmetcln/data_periodI/root_data_periodI.root'
    ]
path='dg/st_z_final/ntuple'

outname = "selected_events.dat"

# cut for a probe to FAIL isolation
raw_cut_all = "lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1 && lY_etiso40>2.0"

# cut for a probe to FAIL isolation AND be in low tail
raw_cut_lo = "lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1 && lY_etiso40>2.0 && lY_pt<=30.0"
raw_cut_hi = "lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>20.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>20.0 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lX_ptiso20/lX_pt<0.1 && lY_etiso40>2.0 && lY_pt>50.0"

raw_cut = raw_cut_lo

QMAPZ = {}
POS,NEG,ALL=range(3)
QMAPZ[POS] = (0,'POS','lP','mu+')
QMAPZ[NEG] = (1,'NEG','lN','mu-')

import re,os,sys
import ROOT

nt = ROOT.TChain(path,path)
[nt.AddFile(ff,ROOT.TChain.kBigNumber,path) for ff in files]
print nt.GetEntries()
iqt=NEG
iqp=POS
cut = raw_cut.replace('lX',QMAPZ[iqt][2]).replace('lY',QMAPZ[iqp][2])                                                                                   

nt.GetPlayer().SetScanRedirect(True)
os.unlink(outname)
nt.GetPlayer().SetScanFileName(outname)
nt.Scan('run:event',cut)
cmd = """awk 'NF==7 && $2!="Row"{print $4,$6}' selected_events.dat | grep 186456 > events186456.txt"""
os.system(cmd)
print 'DONE'
