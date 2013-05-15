#!/usr/bin/env python

"""
Computes a C_W scale factor using the unfolding TTree
"""

import os,sys,re,math,glob
import common
import ROOT
ROOT.gROOT.SetBatch(ROOT.kTRUE)
ROOT.TH1.SetDefaultSumw2(ROOT.kTRUE)

assert len(sys.argv)>=2,"USAGE: %s '/path/to/unfolding_files_00*.root'"%(sys.argv[0],)
patterns = sys.argv[1:]
files = []
for pattern in patterns:
    files += glob.glob(pattern)
for ff in files:
    assert os.path.exists(ff) and os.path.isfile(ff),'Bad file: %s'%ff
if len(files)==0:
    print 'ERROR: no files found'
    print sys.argv
    sys.exit(0)
bdir = os.path.basename(os.path.dirname(files[0]))
print 'Opening sample:',bdir,'with',len(files),'files'

path = 'physics_Nominal'
T = ROOT.TChain(path,path)
[T.AddFile(ff,ROOT.TChain.kBigNumber,path) for ff in files]

def effi(numer,denom):
    """ Symmetric binomial errors for efficiency calculation
    Note: resultant efficiency & error are multiplied by 100%
    """
    assert denom>0.0
    assert numer>=0.0
    efficiency = (1.0*numer)/(1.0*denom)
    err = efficiency*(1.-efficiency)/denom
    err = math.sqrt(err) if err>=0.0 else 0.0
    return efficiency*100.0,err*100.0

if __name__ == '__main__':
    c = ROOT.TCanvas('c','c',800,600)

    print 'Working on truth...'
    T.Draw('Truth_PtW>>htru','(Truth_Is_Fiducial)*(Truth_Mc_Weight)')
    htru = ROOT.htru
    
    print 'Working on reco...'
    T.Draw('Truth_PtW>>hrec','(Truth_Is_Fiducial && Reco_isReconstructed)*(Reco_Weight)')
    hrec = ROOT.hrec

    den,num = htru.Integral(),hrec.Integral()
    eff,err = effi(num,den)
    print '%s:\t Reconstructed %.2f / %.2f W- events in fiducial volume'%(bdir,num,den)
    print '%s:\t Efficiency = %.5f +/- %.5f %%'%(bdir,eff,err)
