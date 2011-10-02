#!/usr/bin/env python

"""
Efficiency errors from:
https://twiki.cern.ch/twiki/bin/view/LHCb/TrackingEffErrorEstimate
"""

import ROOT
from math import sqrt,fabs

class SuEfficiency():
    def __init__(s,eopt=ROOT.TEfficiency.kFFC):
        s.pEff = ROOT.TEfficiency()
        s.pEff.SetStatisticOption(eopt)

    def signal(s,ntot,atot,CL=0.6827,debug=False,erravg=False):
        """ Calculates efficiency error bands for signal only case """
        ntot = int(ntot)
        atot = int(atot)
        if atot>ntot: atot=ntot #sanity checks

        pMeas,totErrUpp,totErrLow = [0.0]*3
        # Convert CL to # of sigma's
        sigma = ROOT.Math.gaussian_quantile(0.5*(1+CL), 1.0)*1.0
        # Calculate measured efficiency
        pMeas = 1.0*atot / ntot
        assert pMeas<=1.0

        totErrLow = sigma*sqrt(atot*(1.0-pMeas))/ntot;
        totErrUpp = sigma*sqrt(atot*(1.0-pMeas))/ntot;  

        # If error close to boundary use FC errors
        if ntot < 2000 and (5.0*totErrUpp+pMeas>= 1.0 or pMeas-5.0*totErrLow<=0.0 ):
            # Error from asig (FC binomial error)
            totErrLow = pMeas - s.pEff.FeldmanCousins( ntot, atot, CL, False);
            totErrUpp = s.pEff.FeldmanCousins( ntot, atot, CL, True) - pMeas;

        # Output the limits
        if debug:
            print "The efficiency is",pMeas,"+",totErrUpp,"-",totErrLow," (",100.0*CL,"% CL)"
        if erravg:
            return pMeas,(fabs(totErrUpp)+fabs(totErrLow))/2.0
        return pMeas,totErrUpp,totErrLow
        
    def signal_bg(s,ntot,atot,nbkg,abkg,CL=0.6827,debug=False,erravg=False):
        """ Calculates efficiency error bands for signal+background case """
        ntot = int(ntot)
        atot = int(atot)
        if atot>ntot: atot=ntot #sanity checks
        nbkg = int(nbkg)
        abkg = int(abkg)
        if abkg>nbkg: abkg=nbkg #sanity checks
        
        pMeas,totErrUpp,totErrLow = [0.0]*3
        # Convert CL to # of sigma's
        sigma = ROOT.Math.gaussian_quantile(0.5*(1+CL), 1.0)*1.0
        # Apply physical bound
        if abkg > nbkg: abkg = nbkg
        # Calculate background efficiency 
        bkgEff = 0.0 if nbkg == 0.0 else 1.0*abkg / nbkg;
        
        # Calculate nsig and asig and apply bounds
        nsig = ntot-nbkg # number of measured total events
        if nsig < 1: nsig = 1
        asig = atot-abkg # number of measured accepted events
        if asig < 0: asig = 0
        if asig > nsig: asig = nsig
    
        # Calculate measured efficiency
        pMeas = 1.0*asig / nsig
    
        # Calculate binomial signal errors
        errAsigLow = 1.0*sigma*sqrt(asig*(1.0-pMeas))/nsig
        errAsigUpp = 1.0*sigma*sqrt(asig*(1.0-pMeas))/nsig

        # If error close to boundary use FC errors
        if nsig < 2000 and (5.0*errAsigUpp+pMeas >= 1.0 or pMeas - 5.0*errAsigLow <= 0.0 ):
            # Error from asig (FC binomial error)
            errAsigLow = pMeas - s.pEff.FeldmanCousins( nsig, asig, CL, False)
            errAsigUpp = s.pEff.FeldmanCousins( nsig, asig, CL, True) - pMeas
        if debug:
            print "Binomial efficiency error: +",errAsigUpp," -",errAsigLow

        # Error from abkg (FC poisson error)
        pPoisson = ROOT.TFeldmanCousins(CL)
        varNbkgLow = sigma*sqrt( nbkg )
        varNbkgUpp = sigma*sqrt( nbkg )
        if nbkg < 25:
            varNbkgLow = 1.0*nbkg - pPoisson.CalculateLowerLimit(nbkg, 0 )
            varNbkgUpp = pPoisson.CalculateUpperLimit(nbkg, 0 ) - 1.0*nbkg
        effNbkgUpp = (asig + bkgEff*varNbkgUpp)*1.0/(nsig + varNbkgUpp )
        if nsig <= varNbkgLow: varNbkgLow = nsig*1.0 - 1
        effNbkgLow = (asig - bkgEff*varNbkgLow)*1.0/(nsig - varNbkgLow )
        if effNbkgLow < 0.0: effNbkgLow = 0.0
        errNbkgLow = pMeas-effNbkgUpp if effNbkgUpp < effNbkgLow else pMeas-effNbkgLow
        errNbkgUpp = effNbkgLow - pMeas if effNbkgUpp < effNbkgLow else effNbkgUpp - pMeas

        if debug:
            print "Poissonian background error: +",errNbkgUpp," -",errNbkgLow

        # Error from abkg 
        errAbkgLow = 0.0 if nbkg == 0.0 else sigma*sqrt(abkg*(1.0-bkgEff))/nbkg
        errAbkgUpp = 0.0 if nbkg == 0.0 else sigma*sqrt(abkg*(1.0-bkgEff))/nbkg
        # If error close to boundary use FC errors
        if( nbkg < 2000 and (3.0/sigma*errAbkgUpp+bkgEff >= 1.0 or bkgEff - 3.0/sigma*errAbkgLow <= 0.0 ) ):
            errAbkgLow = bkgEff - s.pEff.FeldmanCousins( nbkg, abkg, CL, False)
            errAbkgUpp = s.pEff.FeldmanCousins( nbkg, abkg, CL, True) - bkgEff
        # Write errors in terms of the efficiency that we are interested in
        errAbkgLow *= 1.0*nbkg / nsig;
        errAbkgUpp *= 1.0*nbkg / nsig;

        if debug:
            print "Binomial background efficiency error: +",errAbkgLow," -",errAbkgUpp

        # Add all errors quadratically
        totErrLow = sqrt( 0.0+pow(errAsigLow,2)+pow(errNbkgLow,2)+pow(errAbkgUpp,2) )
        if pMeas - totErrLow < 0.0: totErrLow = pMeas
        totErrUpp = sqrt( pow(errAsigUpp,2)+pow(errNbkgUpp,2)+pow(errAbkgLow,2) )
        if totErrUpp + pMeas > 1.0: totErrUpp = 1.0 - pMeas

        # Simulate asymtotic behaviour
        delta = 1.0*(atot-abkg)/(ntot-nbkg) - pMeas;
        if delta > 0.0: totErrLow = totErrLow - 0.5*delta
        if delta < 0.0: totErrUpp = totErrUpp + 0.5*delta

        # Output the limits
        if debug:
            print "The efficiency is ",pMeas, " +",totErrUpp," -",totErrLow," (",100.0*CL,"% CL)"
        if erravg:
            return pMeas,(fabs(totErrUpp)+fabs(totErrLow))/2.0
        return pMeas,totErrUpp,totErrLow

if __name__=='__main__':
    """
    Binomial efficiency error: +0.0224239 -0.0282281
    Poissonian background error: +0.00555569 -0.00454554
    Binomial background efficiency error: +0.0300007 -0.0300007
    The efficiency is 0.95 +0.0378647 -0.0414431 (68.27% CL)
    and
    The efficiency is 0.95 +0.0224239 -0.0282281 (68.27% CL)
    """
    eff = SuEfficiency()
    res1 = eff.signal_bg(200, 185, 100, 90,debug=True)
    res2 = eff.signal(100,95,debug=True)
