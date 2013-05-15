#!/usr/bin/env python

# draws trigger matching cone

wpre2='ptiso40/l_pt<0.1 && met>25.0 && l_pt>25.0 && fabs(l_eta)<2.4 && w_mt>40.0 && idhits==1 && fabs(z0)<10.0 && nmuons==1'
wpre='%s && l_trigEF<0.15'%(wpre2)
zpre='lP_ptiso40/lP_pt<0.1 && lN_ptiso40/lN_pt<0.1   &&    lP_pt>25.0 && fabs(lP_eta)<2.4 && lN_pt>25.0 && fabs(lN_eta)<2.4    &&    lP_idhits==1 && fabs(lP_z0)<10.   &&   lN_idhits==1 && fabs(lN_z0)<10.   &&   Z_m>70 && Z_m<110    &&    fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0 && nmuons==2'  #  && lP_trigEF<0.15 && lN_trigEF<0.15
DEFCUT='mcw*puw*wzptw*znlow*alpy*vxw*ls1w*ls2w*effw*isow*trigallw'

import common
import sys,os,ROOT
from optparse import OptionParser
ROOT.gROOT.SetBatch(True)

parser = OptionParser()
parser.add_option('-f',"--input",dest="input",
                  type="string", default='data_periodD/root_data_periodD.root',
                  help="Input file")
parser.add_option('-t',"--tag",dest="tag",
                  type="string", default='dataD',
                  help="Tag for final plots")
parser.add_option('-n',"--ntuple",dest="ntuple",
                  type="string", default='w',
                  help="Ntuple: w or z")
parser.add_option('-q',"--charge",dest="charge",
                  type="int", default=0,
                  help="Charge: 0 or 1")
parser.add_option('-m',"--mode",dest="mode",
                  type="int", default=0,
                  help="Run mode")
parser.add_option('-b',"--bin",dest="bin",
                  type="string", default='',
                  help="Limit to a particular eta bin")
parser.add_option("--fail", default=False,
                  action="store_true",dest="fail",
                  help="flags whether we want tag to fail in z events")
parser.add_option('-w',"--weight", default=False,
                  action="store_true",dest="weight",
                  help="flags whether we should apply weights in TTree::Draw")
(opts, args) = parser.parse_args()
mode = opts.mode
fin = opts.input
assert os.path.isfile(fin)
ntuple = opts.ntuple
assert ntuple in ('w','z')
q = opts.charge
assert q in (0,1)
out = '%s_%s_%s%s'%(opts.tag,ntuple,'POS' if q==0 else 'NEG','_tagfail' if opts.fail else '')
cut = DEFCUT if opts.weight else '1==1'

f = ROOT.TFile.Open(fin,'READ')
t = f.Get('dg/Nominal/st_%s_final/ntuple'%ntuple); assert t

gbg = []
def G(x,color=ROOT.kRed):
    gr1 = ROOT.TGraph(2)
    fmax = x
    gr1.SetPoint(0,fmax,c.GetFrame().GetY1())
    gr1.SetPoint(1,fmax,c.GetFrame().GetY2())
    gr1.SetLineColor(color)
    gr1.SetLineStyle(2)
    gr1.Draw('L')
    gbg.append(gr1)

QMAP = {}
if ntuple=='w':
    QMAP[0] = 'l_q>0'
    QMAP[1] = 'l_q<0'
else:
    QMAP[0] = 'lP_eta<100'
    QMAP[1] = 'lP_eta<100'

# dump to text file
if mode==1:
    fout = 'dump_'+out+'.dat'
    t.SetScanField(0)
    pre = wpre2 if ntuple=='w' else zpre+' && lP_trigEF<0.15 && lN_trigEF<0.15'
    if ntuple=='z' and opts.fail: # force tag to fail; no req. on probe
        pre = zpre+' && lN_trigEF>0.15' if q==0 else zpre+' && lP_trigEF>0.15'
    pre += opts.bin
    print pre
    v1 = 'l_eta' if ntuple=='w' else ('lP_eta' if q==0 else 'lN_eta')
    v2 = 'l_phi' if ntuple=='w' else ('lP_phi' if q==0 else 'lN_phi')
    t.GetPlayer().SetScanFileName(fout)
    t.GetPlayer().SetScanRedirect(True)
    t.Scan("run:event:%s:%s"%(v1,v2),'(%s)*(%s)*(%s)'%(pre,QMAP[q],cut))
    
# TProfile
if mode==0:
    print opts
    c = ROOT.TCanvas('c','c',600,600)
    c.cd()
    pr = ROOT.TProfile('matchcone_in_eta','Profile of match distance vs eta',100,-2.5,2.5,0,0.15)
    v1 = 'l_trigEF' if ntuple=='w' else ('lP_trigEF' if q==0 else 'lN_trigEF')
    v2 = 'l_eta' if ntuple=='w' else ('lP_eta' if q==0 else 'lN_eta')
    pre = wpre if ntuple=='w' else zpre+' && lP_trigEF<0.15 && lN_trigEF<0.15'
    if ntuple=='z' and opts.fail: # force tag to fail
        pre = zpre+' && lP_trigEF<0.15 && lN_trigEF>0.15' if q==0 else zpre+' && lP_trigEF>0.15 && lN_trigEF<0.15'
    pre += opts.bin
    t.Draw("%s:%s>>matchcone_in_eta"%(v1,v2),'(%s)*(%s)*(%s)'%(pre,QMAP[q],cut),"prof")
    pr.GetYaxis().SetRangeUser(0.0,1.2e-3);
    c.Modified(); c.Update()
    if q==0 or q==1:
        G(-2.18,ROOT.kRed)
        G(-1.95,ROOT.kRed)
        G(2.18,ROOT.kRed)
        G(1.95,ROOT.kRed)
    if q==0:
        G(-1.74,ROOT.kBlue)
        G(-1.52,ROOT.kBlue)
        G(1.74,ROOT.kBlue)
        G(1.52,ROOT.kBlue)
    c.SaveAs('prof_'+out+'.png')
    c.SaveAs('prof_'+out+'.pdf')

if False:
    t.Draw("l_trigEF>>h(1000,0,0.2)","l_eta>-2.18 && l_eta<-1.95")
    ROOT.gPad.SetLogy(1)
    ROOT.gPad.Modified()
    ROOT.gPad.SaveAs(fout+'_Cside.png')
if False:
    t.Draw("l_trigEF>>h(1000,0,0.2)","l_eta<2.18 && l_eta>1.95")
    ROOT.gPad.SetLogy(1)
    ROOT.gPad.Modified()
    ROOT.gPad.SaveAs(fout+'_Aside.png')

os._exit(0)
