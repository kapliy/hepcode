#!/usr/bin/env python

"""
Checks number of Z candidates per event.
Particularly, used to investigate difference between Pythia and Alpgen MC

Pythia:
1330380
1330347

Alpgen:
1964544
1964521
"""

import sys,re,glob,os
import ROOT

pattern = '/share/ftkdata1/antonk/ana_v26_0930_all_stacoCB_10GeV/mc_zmumu/root_mc_zmumu.root'
pattern = '/share/ftkdata1/antonk/ana_v26_0930_all_stacoCB_10GeV/mc_jimmy_zmumu_np*/root*.root'

cut='lP_idhits==1 && fabs(lP_z0)<10. && lP_pt>10.0 && lN_idhits==1 && fabs(lN_z0)<10. && lN_pt>10.0 && Z_m>81.0 && Z_m<101.0 && (lP_q*lN_q)<0 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>2.0 && lP_ptiso40<2.0 && lP_etiso40<2.0'


nt = ROOT.TChain('dg/st_z_final/ntuple','dg/st_z_final/ntuple')
for p in glob.glob(pattern):
    nt.Add(p)

outname='Z_EVENTS_ORIG.dat'
if os.path.exists(outname):
    os.unlink(outname)
nt.GetPlayer().SetScanRedirect(True)
nt.GetPlayer().SetScanFileName(outname)
nt.Scan('event',cut)

cmds = ["""awk 'NF==5 && $2!="Row"{print $4}' Z_EVENTS_ORIG.dat > Z_EVENTS.dat"""]
cmds.append('cat Z_EVENTS.dat | wc -l')
cmds.append('uniq Z_EVENTS.dat | wc -l')
[os.system(cmd) for cmd in cmds]
