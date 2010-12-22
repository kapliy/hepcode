import ROOT

#lvl = 'nvtx_grl_trig'
lvl = 'nvtx_presel'
#lvl = 'nvtx_qual'
#lvl = 'nvtx_final'

f = ROOT.TFile.Open('pileup_weight.root','r')
g1 = f.GetDirectory('data.root').GetDirectory('dg').GetDirectory('dg').GetDirectory('dg')
g2 = f.GetDirectory('mc_wmunu.root').GetDirectory('dg').GetDirectory('dg')
hdata = g1.Get(lvl)
hmc = g2.Get(lvl)
for h in (hdata,hmc):
    h.SetDirectory(0)
hdata.Scale(1/hdata.Integral())
hmc.Scale(1/hmc.Integral())

for i in xrange(2,12):
    nd = hdata.GetBinContent(i)
    nm = hmc.GetBinContent(i)
    print '_data[%d] = %f;'%(i-1,nd/nm)
