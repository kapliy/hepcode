#!/usr/bin/env python

# a bunch of global binning constants, as well as legend names

ptbins = [20,25,30,35,40,45,50,120]
mansetabins = [-2.40,-2.18,-1.95,-1.74,-1.52,-1.37,-1.05,-0.84,-0.63,-0.42,-0.21,0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]
absetabins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]
setabins    = [-zz for zz in absetabins[1:]]
setabins.reverse()
setabins += (absetabins)

def EB(b,etamode=2):
    """ Converts opts.ieta to a SuData/TH2::Project specifier (2:2)
    Note that ieta counts from 1, following ROOT TH1 bin counting convention
    """
    etabins = absetabins if etamode==2 else setabins
    if b=='ALL': return ':x:1:%d'%(len(etabins)-1)
    elif b>0 and b <= len(etabins)-1: return ':x:%d:%d'%(b,b)
    assert False,'Unknown eta bin: %s (max=%s)'%(b,len(etabins)-1)
    return None
def REB(b,etamode=2):
    """ Reverse map of eta bins, where input b counts from 0
    """
    if b=='ALL': return b
    return b+1
def PB(b):
    """ Converts opts.ipt to a SuData/TH2::Project specifier (2:2), merging neighboring pt bins
    Note that ipt counts from 1, following ROOT TH1 bin counting convention
    """
    if b=='ALL20': return ':y:1:%d'%(len(ptbins)-1)
    elif b=='ALL25': return ':y:2:%d'%(len(ptbins)-1)
    elif b>0 and b<=len(ptbins)-1: return ':y:%d:%d'%(b,b) # Dec 2012: do not merge bins
    elif b>0 and b<=3: return ':y:%d:%d'%(b,b)   # 30 .. 35
    elif b==4: return ':y:4:5'           # 35 .. 45
    elif b==5: return ':y:6:7'           # 45 .. 120
    assert False,'Unknown pt bin: %s (max=%s)'%(b,len(ptbins)-1)
    return None
def RPB(b):
    """ Reverse map of pt bins, where input b counts from 0
    """
    if b=='ALL20': return b
    elif b=='ALL25': return b
    elif b<=10: return b+1 # FIXME: do not merge bins
    elif b<=2: return b+1
    elif b in (3,4): return 3+1
    elif b in (5,6): return 4+1
    assert False,'Unkown pt bin: %s'%b
    return b

def zpre_samebin_eta():
    """ a utility function that creates a long pre string for Z selection, forcing both muons to be in the same bin """
    b = setabins
    out = []
    for i,ib in enumerate(b[:-1]):
        out.append(   '(lP_eta>=%.2f && lP_eta<=%.2f && lN_eta>=%.2f && lN_eta<=%.2f)'%(b[i],b[i+1],b[i],b[i+1])   )
    return '('+(' || '.join(out))+')'

# label maps
LABELMAP = {}
LABELMAP['lepton_etav'] = ['#eta',None]
LABELMAP['leptonP_etav'] = ['#mu^{+} #eta',None]
LABELMAP['leptonN_etav'] = ['#mu^{-} #eta',None]
LABELMAP['l_eta'] = ['#eta',None]
LABELMAP['lP_eta'] = ['#eta',None]
LABELMAP['lN_eta'] = ['#eta',None]
LABELMAP['lepton_ptiso40r'] = ['Iso p_{T}^{cone40} / p_{T}',None]
LABELMAP['lepton_etavHP'] = ['#eta',None]
LABELMAP['lepton_absetav'] = ['|#eta|',None]
LABELMAP['leptonP_absetav'] = ['#mu^{+} |#eta|',None]
LABELMAP['leptonN_absetav'] = ['#mu^{-} |#eta|',None]
LABELMAP['fabs(l_eta)'] = ['|#eta|',None]
LABELMAP['fabs(lP_eta)'] = ['|#eta|',None]
LABELMAP['fabs(lN_eta)'] = ['|#eta|',None]
LABELMAP['wmt'] = ['m_{T}^{W}','GeV']
LABELMAP['w_mt'] = LABELMAP['wmt']
LABELMAP['Z_m'] = ['m_{Z}','GeV']
LABELMAP['z_m'] = LABELMAP['Z_m']
LABELMAP['met'] = ['E_{T}^{Miss}','GeV']
LABELMAP['lpt'] = ['p_{T}','GeV']
LABELMAP['l_pt'] = ['p_{T}','GeV']
LABELMAP['lP_pt'] = ['p_{T}','GeV']
LABELMAP['lN_pt'] = ['p_{T}','GeV']
LABELMAP['wpt'] = ['p_{T}^{W}','GeV']
LABELMAP['w_pt'] = ['p_{T}^{W}','GeV']
LABELMAP['zpt'] = ['p_{T}^{Z}','GeV']
LABELMAP['Z_pt'] = LABELMAP['zpt']
LABELMAP['nvtxs_all'] = ['Number of vertices',None]
LABELMAP['lepton_phi'] = ['#phi',None]
LABELMAP['l_phi'] = ['#phi',None]
LABELMAP['lP_phi'] = ['#mu^{+} #phi',None]
LABELMAP['lN_phi'] = ['#mu^{-} #phi',None]
LABELMAP['vxz0'] = ['Reconstructed primary vertex z_{0}','mm']
LABELMAP['vxz0_unw'] = LABELMAP['vxz0']
LABELMAP['TH_vxz0'] = LABELMAP['vxz0']
LABELMAP['TH_vxz0_unw'] = LABELMAP['vxz0']
LABELMAP['tr_vxz0'] = ['Truth primary vertex z_{0}','mm']
LABELMAP['tr_vxz0_unw'] = LABELMAP['tr_vxz0']
LABELMAP['avgmu'] = ['Number of interactions, <mu>',None]
LABELMAP['avgmu_unw'] = LABELMAP['avgmu']
LABELMAP['TH_avgmu'] = LABELMAP['avgmu']
LABELMAP['TH_avgmu_unw'] = LABELMAP['avgmu']
LABELMAP['nvtx_grltrig'] = ['Number of vertices with >=3 tracks',None]
LABELMAP['nvtx_grltrig_unw'] = LABELMAP['nvtx_grltrig']
LABELMAP['TH_nvtx'] = LABELMAP['nvtx_grltrig']
LABELMAP['TH_nvtx_unw'] = LABELMAP['nvtx_grltrig']
# 3d plots
LABELMAP['d3_eta_lpt_met:x'] = LABELMAP['met']
LABELMAP['d3_abseta_lpt_met:x'] = LABELMAP['met']
LABELMAP['d3_eta_lpt_met:y'] = LABELMAP['l_eta']
LABELMAP['d3_abseta_lpt_met:y'] = LABELMAP['l_eta']
LABELMAP['d3_eta_lpt_wmt:x'] = LABELMAP['wmt']
LABELMAP['d3_abseta_lpt_wmt:x'] = LABELMAP['wmt']
LABELMAP['d3_eta_lpt_wpt:x'] = LABELMAP['wpt']
LABELMAP['d3_abseta_lpt_wpt:x'] = LABELMAP['wpt']
LABELMAP['d3_eta_lpt_lpt:x'] = LABELMAP['lpt']
LABELMAP['d3_abseta_lpt_lpt:x'] = LABELMAP['lpt']
LABELMAP['d3_eta_lpt_phi:x'] = LABELMAP['l_phi']
LABELMAP['d3_abseta_lpt_phi:x'] = LABELMAP['l_phi']
LABELMAP['d3_eta_lpt_eta:x'] = LABELMAP['l_eta']
LABELMAP['d3_abseta_lpt_eta:x'] = LABELMAP['l_eta']

def match_labelmap(x):
    if x in LABELMAP:
        return LABELMAP[x][:]
    elif x[:14+2] in LABELMAP:
        return LABELMAP[x[:14+2]]
    elif x[:17+2] in LABELMAP:
        return LABELMAP[x[:17+2]]
    return None

# cut maps
CUTMAP = {}
CUTMAP['all_events'] = 'All events'
CUTMAP['passes_ntuple'] = 'Preselection (8 GeV muon)'
CUTMAP['passes_e_grl'] = 'Good run list'
CUTMAP['passes_e_lar_cleaning'] = 'LAr failure'
CUTMAP['passes_e_primary_vertex'] = 'Primary vertex'
CUTMAP['passes_e_trigger'] = 'Trigger'
CUTMAP['passes_w_qual'] = 'Combined muon + quality cuts'
CUTMAP['passes_w_pt'] = '$p_T > 20$\,\GeV'
CUTMAP['passes_w_pt_25'] = '$p_T > 25$\,\GeV'
CUTMAP['passes_w_eta'] = '$|\eta| < 2.4$'
CUTMAP['passes_w_iso'] = 'Isolation'
CUTMAP['passes_w_nmuons'] = 'Exactly one muon'
CUTMAP['passes_w_metclean'] = '$E_T^{Miss}$ cleaning'
CUTMAP['passes_w_larhole'] = 'LAr hole jet veto'
CUTMAP['passes_w_met'] = '$E_T^{Miss} > 25$\,\GeV'
CUTMAP['passes_w_mt'] = '$m_T > 40$\,\GeV '
CUTMAP['passes_w_pos'] = '$W^{+} \\rightarrow \mu^{+} \\nu$'
CUTMAP['passes_w_neg'] = '$W^{-} \\rightarrow \mu^{-} \\nu$'

if __name__ == '__main__':
    print len(absetabins)
    print absetabins
    print len(setabins)
    print setabins
    print zpre_samebin_eta()

