#!/usr/bin/env python

"""
Fitting floating background normalization
(e.g., QCD)
"""
import math,sys,re,array
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
    c = ROOT.TCanvas(name,name,1024,768)
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

  def doFitTF(s):
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
    fit.SetRangeX(s.fitmin,s.fitmax) # choose MET fit range
    if False: # debugging
      s.dump_plot([data,s.fixed,s.free[0]])
    # set up extra parameters. frac0 = EWK (fixed), frac1 = QCD (free)
    assert fit.GetFitter()
    if False:
      [ fit.Constrain(i,0.0,1.0) for i in xrange(0, mc.GetSize()) ] # constrain fractions to be between 0 and 1
    else:
      ewkfrac = s.fixed.Integral(s.fitmin,s.fitmax)/data.Integral(s.fitmin,s.fitmax)
      fixpars = False
      if ewkfrac>1.0:
        print 'WARNING: SuFit::doFitTF EWK background already exceeds DATA',ewkfrac,s.fixed.Integral(),data.Integral()
        ewkfrac = 0.95
        fixpars = False
      qcdfrac = 1.0-ewkfrac
      fit.GetFitter().SetParameter(0,"ewkfrac",ewkfrac,0.01,0.0,1.0);
      fit.GetFitter().SetParameter(1,"qcdfrac",qcdfrac,0.01,0.0,1.0);
      if fixpars:
        fit.GetFitter().FixParameter(0);
      if False:
        arglist = array.array('d',[1000,0.001])  #ncalls,tolerance
        fit.GetFitter().ExecuteCommand("MIGRAD",arglist,2);
      print 'INFO: SuFit::doFitTF parameter defaults:',ewkfrac,qcdfrac
    # start the fits
    print 'Starting fits...'
    s.status = fit.Fit()      # perform the fit
    s.nfits = 1
    if s.status!=0:
      print 'WARNING: repeating (N=2) the QCD normalization fit'
      s.status = fit.Fit()
      s.nfits += 1
      if s.status!=0:
        print 'WARNING: repeating (N=3) the QCD normalization fit'
        s.status = fit.Fit()
        s.nfits += 1
        if s.status!=0:
          print 'WARNING: repeating (N=4) the QCD normalization fit'
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
      return
    else:
      print 'INFO: fit converged after',s.nfits,'iterations'
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
    s.scales[-1] *= s.data.Integral()
    s.scales[-1] /= s.free[0].Integral() # original qcd fraction
    s.scalesE.append( float(error) )
    s.scalesE[-1] *= s.data.Integral()
    s.scalesE[-1] /= s.free[0].Integral() # original qcd fraction
    # EWK FIXED NORMALIZATION VALUES:
    fit.GetResult(0, value, error);
    # fractions
    s.Wfractions.append( float(value) )
    s.WfractionsE.append( float(error) )
    # scales
    s.Wscales.append( float(value) )
    s.Wscales[-1] *= s.data.Integral()
    s.Wscales[-1] /= s.fixed.Integral() # original ewk fraction
    s.WscalesE.append( float(error) )
    s.WscalesE[-1] *= s.data.Integral()
    s.WscalesE[-1] /= s.fixed.Integral() # original ewk fraction
    # fit quality
    s.chi2.append( fit.GetChisquare() )
    #s.chi2.append( data.Chi2Test( s.fit.GetPlot(), "UW CHI2" )  )
    s.ndf.append( fit.GetNDF() )
    print 'Chi2 =','%8f'%s.chi2[-1],' NDF =',s.ndf[-1]

  def doFit(s):
    """ do fit and return weights """
    
    # clear results from previous fit
    assert s.data and s.fixed and len(s.free)>0, 'SuFit has not been supplied with all required input histograms'
    
    # clean out old model and datahist
    if s.dataHist:  s.dataHist.Clear()
    if s.model: s.model.Clear()
    s.fractions = []
    s.scales = []
    s.fractionsE = []
    s.scalesE = []
    s.Wfractions = []
    s.Wscales = []
    s.WfractionsE = []
    s.WscalesE = []

    var = s.w.var(s.vnames[0])
    s.fitmin,s.fitmax = s.data.FindFixBin(var.getMin()) , s.data.FindFixBin(var.getMax())
    s.plotmin,s.plotmax = s.fitmin,s.fitmax

    # create RooDataHist for data
    s.dataHist = RooDataHist( "dataHist" , "dataHist" , s.ArgList() , s.data )
    
    # create RooDataHists / RooHistPdfs for the free backgrounds
    s.freeHists = freeHists = []
    s.freeTemplates = freeTemplates = []
    for ift in xrange(0,len(s.free)):
      histName = s.free[ift].GetName()
      tempName = s.free[ift].GetName()
      histName += "Hist"
      tempName += "Template"
      freeHists.append( RooDataHist(histName,histName,s.ArgList(),s.free[ift].Clone()) )
      freeTemplates.append( RooHistPdf(tempName,tempName, s.ArgSet(), freeHists[-1]) )

    assert len(freeTemplates)==1,'ERROR: due to pyroot limitations, multiple free templates are not supported yet'
    
    # create RooDataHist / RooHistPdf for fixed backgrounds
    s.fixedHist = fixedHist = RooDataHist( "fixedHist" , "fixedHist" , s.ArgList() , s.fixed.Clone() )
    s.fixedTemplate = fixedTemplate = RooHistPdf( "fixedTemplate" , "fixedTemplate" , s.ArgSet() , fixedHist )

    # set up fraction vars
    assert len(s.free)==1,'This version only supports 1D fits in one variable'
    for ift in xrange(0,len(s.free)):
      varName = s.free[ift].GetName()
      varName += "Frac"
      s.fnames.append(varName)
      s.w.factory('%s[0.1,0,1]'%varName)

    print "SuFit:    %d templates vs. %d fVars"%(2 , len(s.fnames))

    # define the model and perform fit
    s.model = RooAddPdf( "model" , "model" , RooArgList(freeTemplates[0],fixedTemplate) , s.fArgList() )
    #s.model = RooAddPdf( "model" , "model" , RooArgList(fixedTemplate,freeTemplates[0]) , s.fArgList() )
    getattr(s.w,'import')(s.model) 
    s.fit = s.model.fitTo( s.dataHist )
    s.model.Print( "t" )

    # set the _weights variable
    for fname in s.fnames:
      s.fractions.append( s.w.var(fname).getVal() )
      s.scales.append( s.fractions[-1] )
      s.fractionsE.append( s.w.var(fname).getError() )
      s.scalesE.append( s.fractionsE[-1] )
      # EWK fraction is simply (1-f)
      s.Wfractions.append( 1.0 - s.w.var(fname).getVal() )
      s.Wscales.append( s.Wfractions[-1] )
      # absolute error on w fraction is the same, since err(1-x) = err(x)
      s.WfractionsE.append( s.fractionsE[-1] )
      s.WscalesE.append( s.fractionsE[-1] )
    for ival in xrange(0,len(s.scales)):
      # scale factor for original qcd
      s.scales[ival] *= s.data.Integral()
      s.scales[ival] /= s.free[ival].Integral()
      s.scalesE[ival] *= s.data.Integral()
      s.scalesE[ival] /= s.free[ival].Integral()
      # scale factor for original ewk
      s.Wscales[ival] *= s.data.Integral()
      s.Wscales[ival] /= s.fixed.Integral()
      s.WscalesE[ival] *= s.data.Integral()
      s.WscalesE[ival] /= s.fixed.Integral()
      if True:
        print "Free Background (fraction & scale):",fname
        print s.fractions[ival],'+/-',s.fractionsE[ival]
        print s.scales[ival],'+/-',s.scalesE[ival]

    # export final histograms [unscaled]
    s.hfixed = s.fixed.Clone()
    s.hfree = s.free[0].Clone()
    # fit model
    if True:
      hmodel = s.model.createHistogram( s.w.var(s.vnames[0]).GetName() , s.data.GetNbinsX() )
      scalefactor = 1.0*s.data.Integral()/hmodel.Integral()
      hmodel.Scale( scalefactor );
      #s.hmodel = hmodel
      h1,h2 = s.hfixed.Clone(),s.hfree.Clone()
      h1.Scale(s.Wscales[0])
      h2.Scale(s.scales[0])
      h1.Add(h2)
      s.hmodel = h1
    
    # set unused variables [compatibility]
    s.status=0
    s.ndf.append( s.data.GetNbinsX() )
    s.chi2.append( s.data.Chi2Test( s.hmodel, "UW CHI2" )  )
    
    # return final weights
    return s.scales

  def drawFits(s,title='random',logscale=False):
    """ Draw fits in all variables """
    res = []
    for ivar in xrange(len(s.vnames)):
      res += s.drawFit(ivar,title,logscale=logscale)
    return res

  def drawFitsTF(s,title='random',logscale=False):
    """ Draw fits in all variables - using the TFractionFitter version """
    res = []
    for ivar in xrange(len(s.vnames)):
      res += s.drawFitTF(ivar,title,logscale=logscale)
    return res

  def drawFit(s,ivar,title='random',logscale=False):
    """ Draw the fit for ivar's variable """
    import SuCanvas
    s.canvas = canvas = SuCanvas.SuCanvas()
    canvas.buildRatio(1024,768)
    # main plot area
    canvas.cd_plotPad()
    s.frame = frame = s.w.var(s.vnames[ivar]).frame()
    RooAbsData.plotOn(s.dataHist, frame , RF.Name("dataHist") )
    s.model.plotOn( frame , RF.Name("modelHist") , RF.LineColor(46) , RF.DrawOption("L") ) # RF.MoveToBack()
    ipattern = 3001
    s.model.plotOn( frame , RF.Name("fixedHist") , RF.Components("fixedTemplate") , RF.DrawOption("F") ,
                    RF.FillColor(8) , RF.LineColor(8) , RF.FillStyle( ipattern ) )
    for ift in xrange(0,len(s.free)):
      tempName = s.free[ift].GetName()
      tempName += "Template"
      # hmmmm is this even normalized correctly??
      s.model.plotOn( frame , RF.Name( s.free[ift].GetName() ) , RF.Components( tempName ) , RF.DrawOption("F") ,
                      RF.Normalization(1.0/s.scales[ift]),
                      RF.FillColor( s.free[ift].GetFillColor() ) , RF.LineColor( s.free[ift].GetLineColor() ) )
    frame.GetXaxis().SetTitleOffset( canvas.getXtitleOffset() )
    frame.GetYaxis().SetTitleOffset( canvas.getYtitleOffset() )
    frame.GetXaxis().SetTitle( s.w.var(s.vnames[ivar]).GetName() )
    frame.GetXaxis().SetTitleColor(ROOT.kBlack)
    frame.GetXaxis().SetLabelSize( canvas.getLabelSize() )
    frame.GetXaxis().SetTitleSize( canvas.getAxisTitleSize() )
    frame.GetXaxis().CenterTitle()
    frame.GetYaxis().SetTitle( "entries / GeV" )
    frame.GetYaxis().SetLabelSize( canvas.getLabelSize() )
    frame.GetYaxis().SetTitleSize( canvas.getAxisTitleSize() )
    frame.GetYaxis().CenterTitle()
    frame.SetTitle("")
    frame.Draw("9")
    
    s.key = key = ROOT.TLegend(0.6, canvas.getY2()-(0.03*(3+len(s.free))) , canvas.getX2() , canvas.getY2() )
    key.SetTextSize( canvas.getLegendTextSize() )
    key.SetBorderSize( 0 )
    key.SetFillColor( ROOT.kWhite )
    key.AddEntry( "dataHist" ,"data","pe")
    key.AddEntry( "modelHist" , "fit model" , "l" )
    key.AddEntry( "fixedHist" , "EWK" , "f" )
    for ift in xrange(len(s.free)):
      key.AddEntry( s.free[ift].GetName() , s.free[ift].getLegendName() , "f" )

    ybot = canvas.getY2()-(0.03*(3+len(s.free)))
    s.fractext = fractext = ROOT.TPaveText( 0.6 , ybot-0.3 , canvas.getX2() , ybot-0.1 , "BL NDC" )
    fractext.SetFillStyle( 0 ) # hollow
    fractext.SetBorderSize( 0 )
    fractext.SetMargin( 0 )
    for ift,frac in enumerate(s.fractions):
      fractext.AddText( 'Frac. %s = %.3f #pm %.3f %%'%(s.free[ift].getLegendName(),frac*100.0,s.fractionsE[ift]*100.0) )
      fractext.AddText( 'Scale %s = %.3f #pm %.3f'%(s.free[ift].getLegendName(),s.scales[ift],s.scalesE[ift]) )
    key.Draw("9");
    fractext.Draw("9");

    # ratio
    canvas.cd_ratioPad()
    hdata = s.dataHist.createHistogram( s.w.var(s.vnames[ivar]).GetName() )
    assert hdata
    hmodel = s.model.createHistogram( s.w.var(s.vnames[ivar]).GetName() , hdata.GetNbinsX() )
    assert hmodel
    scalefactor = 1.0*hdata.Integral()/hmodel.Integral()
    hmodel.Scale( scalefactor );
    s.hratio,s.href = hdata.Clone('hratio'),hdata.Clone('href')
    s.hratio.Divide(hmodel)
    canvas.drawRefLine(s.href)
    canvas.drawRatio(s.hratio)
    # finalize
    canvas.update()
    return canvas,frame

  def drawFitTF(s,ivar,title='random',logscale=False):
    """ Draw the fit for ivar's variable - TFractionFitter version """

    import SuCanvas
    s.canvas = canvas = SuCanvas.SuCanvas()
    canvas.buildRatio(1024,768)

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
    data.SetMarkerSize(1.0)
    data.SetFillStyle(0)
    #ROOT.gStyle.SetErrorX(0.001)
    data.Draw('X0 A SAME')

    # prettify
    obj = stack
    if obj:
      obj.GetXaxis().SetRange(s.plotmin,s.plotmax)
      obj.GetXaxis().SetTitleOffset( canvas.getXtitleOffset() )
      obj.GetYaxis().SetTitleOffset( canvas.getYtitleOffset() )
      obj.GetXaxis().SetTitle( s.w.var(s.vnames[ivar]).GetName() )
      obj.GetXaxis().SetTitleColor(ROOT.kBlack)
      obj.GetXaxis().SetLabelSize( canvas.getLabelSize() )
      obj.GetXaxis().SetTitleSize( canvas.getAxisTitleSize() )
      obj.GetXaxis().CenterTitle()
      obj.GetYaxis().SetTitle( "entries / GeV" )
      obj.GetYaxis().SetLabelSize( canvas.getLabelSize() )
      obj.GetYaxis().SetTitleSize( canvas.getAxisTitleSize() )
      obj.GetYaxis().CenterTitle()
    
    s.key = key = ROOT.TLegend(0.6, canvas.getY2()-(0.03*(3+len(s.free))) , canvas.getX2() , canvas.getY2() )
    key.SetTextSize( canvas.getLegendTextSize() )
    key.SetBorderSize( 0 )
    key.SetFillColor( ROOT.kWhite )
    key.AddEntry( data ,"data","pe")
    key.AddEntry( model , "fit model" , "l" )
    key.AddEntry( fixed , "EWK" , "f" )
    key.AddEntry( free , s.free[ift].getLegendName() , "f" )

    ybot = canvas.getY2()-(0.03*(3+len(s.free)))
    s.fractext = fractext = ROOT.TPaveText( 0.6 , ybot-0.3 , canvas.getX2() , ybot-0.1 , "BL NDC" )
    fractext.SetFillStyle( 0 ) # hollow
    fractext.SetBorderSize( 0 )
    fractext.SetMargin( 0 )
    for ift,frac in enumerate(s.fractions):
      if s.fractions[ift]!=0 and s.scales[ift]!=0:
        fractext.AddText( '%s Frac. = %.3f #pm %.2f%%'%(s.free[ift].getLegendName(),s.fractions[ift],s.fractionsE[ift]/s.fractions[ift]*100.0 if abs(s.fractions[ift])>1e-6 else 0) )
        fractext.AddText( '%s Scale = %.3f #pm %.2f%%'%(s.free[ift].getLegendName(),s.scales[ift],s.scalesE[ift]/s.scales[ift]*100.0 if abs(s.scales[ift])>1e-6 else 0) )
      if s.Wfractions[ift]!=0 and s.Wscales[ift]!=0:
        fractext.AddText( '%s Frac. = %.3f #pm %.2f%%'%('EWK',s.Wfractions[ift],s.WfractionsE[ift]/s.Wfractions[ift]*100.0 if s.Wfractions[ift]!=0 else 0) )
        fractext.AddText( '%s Scale = %.3f #pm %.2f%%'%('EWK',s.Wscales[ift],s.WscalesE[ift]/s.Wscales[ift]*100.0 if s.Wscales[ift]!=0 else 0) )
      if s.ndf[ift]!=0:
        fractext.AddText( 'CHI2 = %.1f  CHI2/NDF=%.1f'%(s.chi2[ift],s.chi2[ift]/s.ndf[ift]) )
    key.Draw("9");
    fractext.Draw("9");

    if logscale:
      print 'Applying log scale'
      ROOT.gPad.SetLogy(ROOT.kTRUE)

    # ratio
    canvas.cd_ratioPad()
    if s.status==0:
      hmodel = model.Clone()
      #scalefactor = 1.0*data.Integral()/model.Integral()
      #hmodel.Scale( scalefactor ); # scale model to actual data integral (not needed!)
      s.hratio,s.href = data.Clone('hratio'),data.Clone('href')
      s.hratio.Divide(hmodel)
      canvas.drawRefLine(s.href)
      s.href.GetXaxis().SetRange(s.plotmin,s.plotmax)
      canvas.drawRatio(s.hratio)
      s.hratio.Draw("AP same")
    # finalize
    canvas.update()
    if False: # debugging:
      canvas.SaveAs('SYS.png')
    return canvas,None

gbg_tmp = []
if __name__=='__main__':
  f = SuFit()
  f.addFitVar( 'met', 0 , 100 , 'met' );
  # get histograms
  f1 = ROOT.TFile.Open('/share/t3data3/antonk/ana/ana_v29D_04292012_DtoM_unfold_stacoCB_all/mc_pythia_wmunu/root_mc_pythia_wmunu.root')
  f2 = ROOT.TFile.Open('/share/t3data3/antonk/ana/ana_v29D_04292012_DtoM_unfold_stacoCB_all/mc_pythia_bbmu15x/root_mc_pythia_bbmu15x.root')
  hfixed = f1.Get('dg/nominal/st_w_final/metfit/met')
  hfixed.Scale(1/40.0)
  hfree = f2.Get('dg/nominal/st_w_final/metfit/met')
  hdata = hfixed.Clone()
  hdata.Add(hfree,1.47)
  # run SuFit
  hdata.getLegendName = lambda : 'Data'
  hfixed.getLegendName = lambda : 'Fixed'
  hfree.getLegendName = lambda : 'QCD float'
  f.setDataBackgrounds(hdata,hfixed,hfree)
  gbg_tmp.append( f.doFit() )
  gbg_tmp.append( f.drawFits('random') )
