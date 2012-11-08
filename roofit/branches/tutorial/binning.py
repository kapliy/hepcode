#!/usr/bin/env python

# a bunch of global binning constants, as well as legend names

ptbins = [20,25,30,35,40,45,50,120]
mansetabins = [-2.40,-2.18,-1.95,-1.74,-1.52,-1.37,-1.05,-0.84,-0.63,-0.42,-0.21,0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]
absetabins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]
setabins    = [-zz for zz in absetabins[1:]]
setabins.reverse()
setabins += (absetabins)

# label maps
LABELMAP = {}
LABELMAP['lepton_etav'] = ['#eta',None]
LABELMAP['l_eta'] = ['#eta',None]
LABELMAP['lP_eta'] = ['#eta',None]
LABELMAP['lN_eta'] = ['#eta',None]
LABELMAP['lepton_etavHP'] = ['#eta',None]
LABELMAP['lepton_absetav'] = ['|#eta|',None]
LABELMAP['fabs(l_eta)'] = ['|#eta|',None]
LABELMAP['fabs(lP_eta)'] = ['|#eta|',None]
LABELMAP['fabs(lN_eta)'] = ['|#eta|',None]
LABELMAP['wmt'] = ['m_{T}^{W}','GeV']
LABELMAP['w_mt'] = ['m_{T}^{W}','GeV']
LABELMAP['met'] = ['E_{T}^{Miss}','GeV']
LABELMAP['lpt'] = ['p_{T}','GeV']
LABELMAP['l_pt'] = ['p_{T}','GeV']
LABELMAP['lP_pt'] = ['p_{T}','GeV']
LABELMAP['lN_pt'] = ['p_{T}','GeV']
LABELMAP['wpt'] = ['p_{T}^{W}','GeV']
LABELMAP['w_pt'] = ['p_{T}^{W}','GeV']
LABELMAP['nvtxs_all'] = ['Number of vertices',None]
LABELMAP['lepton_phi'] = ['#phi',None]
LABELMAP['l_phi'] = ['#phi',None]

if __name__ == '__main__':
    print len(absetabins)
    print absetabins
    print len(setabins)
    print setabins
