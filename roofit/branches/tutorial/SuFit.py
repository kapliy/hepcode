#!/usr/bin/env python

"""
Fitting floating background normalization
(e.g., QCD)
"""
import math,sys,os,re,array
import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooDataSet,RooAbsData,RooFormulaVar,RooRealVar
from ROOT import RooHistPdf,RooAddPdf #RooGlobalFunc
from ROOT import RooFit as RF

def quadrature(v1,v2):
  return math.sqrt(v1*v1+v2*v2)
def safe_divide(num,den):
  return num*1.0/den if den!=0 else sys.float_info.max
fabs = math.fabs

class SuFit:
  """ Float background fit class """
  def __init__(s):
    """ constructor """
    s.data = None
    s.fixed = None
    s.free = []

    s.w = RooWorkspace('w',ROOT.kTRUE)
    s.vnames = []
    s.fnames = []

    # fractions and scales for QCD template
    s.fractions = []
    s.scales = []
    s.fractionsE = []
    s.scalesE = []

    # same for EWK template
    s.Wfractions = []
    s.Wscales = []
    s.WfractionsE = []
    s.WscalesE = []

    # general fit quality
    s.fit = None
    s.nfits = 0
    s.chi2 = []
    s.ndf = []

    s.model = None
    s.dataHist = None
    s.rebin = 1

  def create(s,data,fixed,free):
    s.data = data
    s.fixed = fixed
    s.free = free

  def addFitVar(s,name,lowerbound,upperbound,units):
    print name,lowerbound,upperbound,units
    sys.stdout.flush()
    s.vnames.append(name)
    s.w.factory('%s[%f,%f]'%(name,lowerbound,upperbound))

  def setDataBackgrounds(s,data,fixed,free ):
    """ define the data, background, and fituple histograms """
    s.data = data
    s.fixed = fixed
    if isinstance(free,list):
      s.free += free
    else:
      s.free.append(free)
  def blowUpErrors(s,what=0,v=10,inc=False):
    """ A little function to study errors
    what = { 0:fixed ; 1:free ; 2:data }
    if inc==True, the error is blown up MORE for larger bins """
    hh = s.fixed
    if what==1:
      hh = s.free[0]
    elif what==2:
      hh = s.data
    if inc==True:
      [hh.SetBinError(ii,hh.GetBinError(ii)*v*ii) for ii in xrange(0,hh.GetNbinsX()+2)]
    else:
      [hh.SetBinError(ii,hh.GetBinError(ii)*v) for ii in xrange(0,hh.GetNbinsX()+2)]
  def addFreeBackground(s,background):
    s.free.append( background )
  def fArgList(s):
    return RooArgList(s.w.var(s.fnames[0]))
  def ArgList(s):
    return RooArgList(s.w.var(s.vnames[0]))
  def ArgSet(s):
    return RooArgSet(s.w.var(s.vnames[0]))

  @staticmethod
  def dump_plot(h,name='SYS'):
    """ A generic mini-function to plot a collection of TObjects """
    o = []
    if isinstance(h,list) or isinstance(h,tuple):
      o = h
    else:
      o = [h,]
    assert len(o)>0
    c = ROOT.TCanvas(name,name,800,600)
    c.cd()
    [ oo.SetLineColor(i+1) for i,oo in enumerate(o) ]
    o[0].Draw()
    [ oo.Draw('A SAME') for oo in o[1:] ]
    maxh = max([htmp.GetMaximum() for htmp in o])*1.3
    o[0].GetYaxis().SetRangeUser(0,maxh)
    c.SaveAs(name+'.png')

  @staticmethod
  def first_nonzero_bin(h,bmin=1):
    """ Returns first bin at which this and next bin have a non-zero value
    Underflow/overflow bins are explicitly ignored
    """
    for i in xrange(bmin+1,h.GetNbinsX()+1):
      if h.GetBinContent(i-1)>0 and h.GetBinContent(i)>0:
        return i-1 # i  # set to i to skip the first bin, too!
    print 'WARNING: failed to find a non-zero bin in first_nonzero_bin. Proceeding starting with first bin...'
    return bmin

  @staticmethod
  def exclude_zero_bins(f,hs=[],limit=0):
    """ f is a TFractionFitter object. hs[] is a collection of histograms
    This function modifies the TFractionFitter object to exclude bins
    where at least one of the hs[] have no entries (or negative entries)
    """
    if len(hs)==0:
      return
    nbins = hs[0].GetNbinsX() + 1
    nexcl = 0
    for ibin in xrange(0,nbins):
      if any( [h.GetBinContent(ibin)<=limit for h in hs] ):
        f.ExcludeBin(ibin)
        nexcl += 1
    if nexcl>0:
      print 'WARNING: TFractionFitter excluded %d out of %d bins due to low statistics (nentries<=%d)'%(nexcl,nbins,limit)
    else:
      print 'INFO: TFractionFitter does not need to exclude any bins'
    return True

  def doFitTF(s,EXCLUDE_ZERO_BINS=None):
    """ A version of doFit using TFractionFitter
    This is supposed to take into account uncertainties on the model (but doesn't)
    Note: EWK contribution is allowed to float, too
    """
    assert s.data and s.fixed and len(s.free)==1, 'SuFit has not been supplied with all required input histograms'
    # clear results from previous fit
    s.fractions = []
    s.scales = []
    s.fractionsE = []
    s.scalesE = []
    s.nfits = 0
    # set up TFractionFitter and fit range
    data = s.data
    mc = ROOT.TObjArray(2) # MC histograms are put into this array
    mc.Add(s.fixed.Clone())
    [mc.Add(ff.Clone()) for ff in s.free]
    s.fit = fit = ROOT.TFractionFitter(data, mc);  # initialise
    var = s.w.var(s.vnames[0])
    s.fitmin,s.fitmax = s.data.FindFixBin(var.getMin()) , s.data.FindFixBin(var.getMax())
    s.plotmin,s.plotmax = s.fitmin,s.fitmax
    s.fitmin = SuFit.first_nonzero_bin(data,s.fitmin)
    print 'INFO: SuFit::doFitTF fit range:',s.vnames[0],s.fitmin,s.fitmax
    sys.stdout.flush()
    fit.SetRangeX(s.fitmin,s.fitmax) # choose MET fit range
    if EXCLUDE_ZERO_BINS!=None:
      SuFit.exclude_zero_bins( fit , [s.fixed,s.free[0],s.data] , EXCLUDE_ZERO_BINS )
    if False: # debugging
      s.dump_plot([data,s.fixed,s.free[0]])
    # set up extra parameters. frac0 = EWK (fixed), frac1 = QCD (free)
    assert fit.GetFitter()
    if False:
      [ fit.Constrain(i,0.0,1.0) for i in xrange(0, mc.GetSize()) ] # constrain fractions to be between 0 and 1
    else:
      ewkfrac = s.fixed.Integral(s.fitmin,s.fitmax)/data.Integral(s.fitmin,s.fitmax)
      if True: # get away from the edges
        if ewkfrac>0.80: ewkfrac = 0.80
        if ewkfrac<0.20: ewkfrac = 0.20
      if ewkfrac>1.0: # legacy, should never happen
        print 'WARNING: SuFit::doFitTF EWK background already exceeds DATA',ewkfrac,s.fixed.Integral(s.fitmin,s.fitmax),data.Integral(s.fitmin,s.fitmax)
        sys.stdout.flush()
        ewkfrac = 0.95
      qcdfrac = 1.0-ewkfrac
      fit.GetFitter().SetParameter(0,"ewkfrac",ewkfrac,0.01,0.0,1.0);
      fit.GetFitter().SetParameter(1,"qcdfrac",qcdfrac,0.01,0.0,1.0);
      if False:
        fit.GetFitter().FixParameter(0);
      if False:
        arglist = array.array('d',[1000,0.001])  #ncalls(1000),tolerance(0.001)
        fit.GetFitter().ExecuteCommand("MIGRAD",arglist,2);
      print 'INFO: SuFit::doFitTF parameter defaults: EWK frac = %.2f QCD frac = %.2f'%(ewkfrac,qcdfrac)
      sys.stdout.flush()
    # start the fits
    print 'Starting fits...'
    sys.stdout.flush()
    s.status = fit.Fit()      # perform the fit
    s.nfits += 1
    if s.status!=0:
      print 'WARNING: repeating (N=2) the QCD normalization fit'
      sys.stdout.flush()
      s.status = fit.Fit()
      s.nfits += 1
      if s.status!=0:
        print 'WARNING: repeating (N=3) the QCD normalization fit; resetting starting point to (0.5,0.5)'
        sys.stdout.flush()
        fit.GetFitter().SetParameter(0,"ewkfrac",0.5,0.01,0.0,1.0);
        fit.GetFitter().SetParameter(1,"qcdfrac",0.5,0.01,0.0,1.0);
        s.status = fit.Fit()
        s.nfits += 1
        if s.status!=0:
          print 'WARNING: repeating (N=4) the QCD normalization fit'
          sys.stdout.flush()
          s.status = fit.Fit()
          s.nfits += 1
    if s.status!=0:
      s.fractions.append( 0 )
      s.fractionsE.append( 0 )
      s.scales.append( 0 )
      s.scalesE.append( 0 )
      s.Wfractions.append( 0 )
      s.WfractionsE.append( 0 )
      s.Wscales.append( 0 )
      s.WscalesE.append( 0 )
      s.chi2.append(0)
      s.ndf.append(0)
      print 'ERROR: fit failed to converge'
      sys.stdout.flush()
      return
    else:
      print 'INFO: fit converged after',s.nfits,'iterations'
      sys.stdout.flush()
    # save all templates
    s.hfixed = fit.GetMCPrediction(0).Clone()
    s.hfree = fit.GetMCPrediction(1).Clone()
    s.hmodel = s.fit.GetPlot().Clone()
    # prepare numerical the results
    value=ROOT.Double(); error=ROOT.Double()
    # QCD FLOAT NORMALIZATION VALUES:
    fit.GetResult(1, value, error);
    # fractions
    s.fractions.append( float(value) )
    s.fractionsE.append( float(error) )
    # scales
    s.scales.append( float(value) )
    s.scales[-1] *= s.data.Integral(s.fitmin,s.fitmax)
    s.scales[-1] /= s.free[0].Integral(s.fitmin,s.fitmax) # original qcd fraction
    s.scalesE.append( float(error) )
    s.scalesE[-1] *= s.data.Integral(s.fitmin,s.fitmax)
    s.scalesE[-1] /= s.free[0].Integral(s.fitmin,s.fitmax) # original qcd fraction
    # EWK FIXED NORMALIZATION VALUES:
    fit.GetResult(0, value, error);
    # fractions
    s.Wfractions.append( float(value) )
    s.WfractionsE.append( float(error) )
    # scales
    s.Wscales.append( float(value) )
    s.Wscales[-1] *= s.data.Integral(s.fitmin,s.fitmax)
    s.Wscales[-1] /= s.fixed.Integral(s.fitmin,s.fitmax) # original ewk fraction
    s.WscalesE.append( float(error) )
    s.WscalesE[-1] *= s.data.Integral(s.fitmin,s.fitmax)
    s.WscalesE[-1] /= s.fixed.Integral(s.fitmin,s.fitmax) # original ewk fraction
    # fit quality
    s.chi2.append( fit.GetChisquare() )
    #s.chi2.append( data.Chi2Test( s.fit.GetPlot(), "UW CHI2" )  )
    s.ndf.append( fit.GetNDF() )
    print 'INFO: EWK frac = %.2f  QCD frac = %.2f'%(s.fractions[-1],s.Wfractions[-1])
    print 'INFO: Chi2 =','%8f'%s.chi2[-1],' NDF =',s.ndf[-1]
    sys.stdout.flush()

  def drawFitsTF(s,title='random',logscale=False):
    """ Draw fits in all variables - using the TFractionFitter version """
    res = []
    for ivar in xrange(len(s.vnames)):
      res += s.drawFitTF(ivar,title,logscale=logscale)
    return res

  def drawFitTF(s,ivar,title='random',logscale=False):
    """ Draw the fit for ivar's variable - TFractionFitter version """
    import SuCanvas
    s.canvas = canvas = SuCanvas.SuCanvas()
    canvas.buildRatio()
    canvas._xaxisName = s.vnames[0] + ' [GeV]'
    # main plot area: draw {data,model,fixed,qcd}
    canvas.cd_plotPad()
    fit = s.fit
    stack = None
    if s.status==0:
      # fixed - must be scaled
      ipattern = 3001
      fixed = s.hfixed
      fixed.SetLineColor(8)
      fixed.SetFillColor(8)
      fixed.SetFillStyle(ipattern)
      fixed.Scale(s.Wscales[0])
      #fixed.Draw('A SAME H')
      # free (qcd) - must be scaled
      ift = 0
      ipattern=1001
      free = s.hfree
      free.SetFillColor(s.free[ift].GetFillColor())
      free.SetLineColor(s.free[ift].GetFillColor())
      free.SetFillStyle(ipattern)
      free.Scale(s.scales[0])
      #free.Draw('A SAME H')
      # make a stack
      s.stack = stack = ROOT.THStack('qcd_stack','Final QCD stack')
      stack.Add(fixed)
      stack.Add(free)
      stack.Draw('HIST')
      # model - already scaled to expectation
      model = s.hmodel
      model.SetLineColor(46)
      model.SetFillStyle(0)
      model.Draw('A SAME')
    # data
    s.hdata = data = s.data.Clone()
    data.SetMarkerStyle(8)
    data.SetFillStyle(0)
    data.Draw('X0 A SAME')

    # y axis
    canvas.cd_plotPad()
    stack.SetMaximum(stack.GetMaximum()*2)

    # prettify
    obj = stack
    if obj:
      obj.GetXaxis().SetRange(s.plotmin,s.plotmax)
      obj.GetXaxis().SetTitle( s.w.var(s.vnames[ivar]).GetName() )
      bin_width = data.GetXaxis().GetBinWidth(1)
      obj.GetYaxis().SetTitle( "Events / %.2f GeV" % bin_width )
    
    #s.key = key = ROOT.TLegend(0.6, canvas.getY2()-(0.03*(3+len(s.free))) , canvas.getX2() , canvas.getY2() )
    s.key = key = ROOT.TLegend()
    key.AddEntry( data ,"data","pe")
    key.AddEntry( model , "fit model" , "l" )
    key.AddEntry( fixed , "EWK" , "f" )
    key.AddEntry( free , s.free[ift].getLegendName() , "f" )
    canvas.ConfigureLegend(key)
    key.Draw("9");
    canvas.update()

    s.fractext = fractext = ROOT.TPaveText()
    for ift,frac in enumerate(s.fractions):
      if s.fractions[ift]!=0 and s.scales[ift]!=0:
        fractext.AddText( '%s Frac. = %.3f #pm %.2f%%'%(s.free[ift].getLegendName(),s.fractions[ift],s.fractionsE[ift]/s.fractions[ift]*100.0 if abs(s.fractions[ift])>1e-6 else 0) )
        fractext.AddText( '%s Scale = %.3f #pm %.2f%%'%(s.free[ift].getLegendName(),s.scales[ift],s.scalesE[ift]/s.scales[ift]*100.0 if abs(s.scales[ift])>1e-6 else 0) )
      if s.Wfractions[ift]!=0 and s.Wscales[ift]!=0:
        fractext.AddText( '%s Frac. = %.3f #pm %.2f%%'%('EWK',s.Wfractions[ift],s.WfractionsE[ift]/s.Wfractions[ift]*100.0 if s.Wfractions[ift]!=0 else 0) )
        fractext.AddText( '%s Scale = %.3f #pm %.2f%%'%('EWK',s.Wscales[ift],s.WscalesE[ift]/s.Wscales[ift]*100.0 if s.Wscales[ift]!=0 else 0) )
      if s.ndf[ift]!=0:
        fractext.AddText( 'CHI2=%.1f  CHI2/NDF=%.1f'%(s.chi2[ift],s.chi2[ift]/s.ndf[ift]) )
    canvas.ConfigureText(fractext,text_y2 = key.GetY1NDC()-0.05)
    fractext.Draw("9");
    canvas.update()
    
    if logscale:
      print 'Applying log scale'
      ROOT.gPad.SetLogy(ROOT.kTRUE)

    # ratio
    canvas.cd_ratioPad()
    if s.status==0:
      hmodel = None
      if False: # this hmodel only includes bins that were actually part of the TFractionFitter fit
        hmodel = s.hmodel.Clone()
      else:     # this includes all bins in the original range of the histograms
        hmodel = s.hfixed.Clone()
        hmodel.Add( s.hfree )
      #scalefactor = 1.0*data.Integral(s.fitmin,s.fitmax)/model.Integral(s.fitmin,s.fitmax)
      #hmodel.Scale( scalefactor ); # scale model to actual data integral (not needed!)
      s.hratio = data.Clone('hratio')
      s.hratio.Divide(hmodel)
      canvas.drawRatio(s.hratio)
      canvas.ConfigureAxis(stack, s.hratio)
      s.hratio.GetXaxis().SetRange(s.plotmin,s.plotmax)
    # finalize
    canvas.update()
    if False: # debugging:
      canvas.SaveAs('SYS.png')
    return canvas,None
