#!/usr/bin/env python

"""
Fitting floating background normalization
(e.g., QCD)
"""
import math,sys,re
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
    
    s.fractions = []
    s.scales = []
    s.fractionsE = []
    s.scalesE = []

    # optional: same for EWK template
    s.Wfractions = []
    s.Wscales = []
    s.WfractionsE = []
    s.WscalesE = []

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
  def doFitTF(s):
    """ A version of doFit using TFractionFitter
    This supposedlt takes into account uncertainties on the model (but doesn't)
    I also think EWK contribution is allowed to float, too
    """
    assert s.data and s.fixed and len(s.free)==1, 'SuFit has not been supplied with all required input histograms'
    # clear results from previous fit
    s.fractions = []
    s.scales = []
    s.fractionsE = []
    s.scalesE = []
    # FractionFitter
    data = s.data
    mc = ROOT.TObjArray(2) # MC histograms are put into this array
    mc.Add(s.fixed.Clone())
    [mc.Add(ff.Clone()) for ff in s.free]
    s.fit = fit = ROOT.TFractionFitter(data, mc);  # initialise
    [ fit.Constrain(i,0.0,1.0) for i in xrange(0, mc.GetSize()) ] # constrain fractions to be between 0 and 1
    var = s.w.var(s.vnames[0])
    s.fitmin,s.fitmax = s.data.FindFixBin(var.getMin()) , s.data.FindFixBin(var.getMax())
    fit.SetRangeX(s.fitmin,s.fitmax) # choose MET fit range
    s.status = fit.Fit()      # perform the fit
    s.nfits = 1
    if s.status!=0:
      print 'WARNING: repeating the QCD normalization fit'
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
      print 'ERROR: fit failed to converge'
      return
    # prepare the results
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
    s.model.fitTo( s.dataHist )
    s.model.Print( "t" )
    
    # set the _weights variable
    for fname in s.fnames:
      s.fractions.append( s.w.var(fname).getVal() )
      s.scales.append( s.w.var(fname).getVal() )
      if True:
        s.fractionsE.append( s.w.var(fname).getError() )
        s.scalesE.append( s.w.var(fname).getError() )
    for ival in xrange(0,len(s.scales)):
      s.scales[ival] *= s.data.Integral()
      s.scales[ival] /= s.free[ival].Integral()
      if True:
        s.scalesE[ival] *= s.data.Integral()
        s.scalesE[ival] /= s.free[ival].Integral()
      if True:
        print "Free Background (fraction & scale):",fname
        print s.fractions[ival],'+/-',s.fractionsE[ival]
        print s.scales[ival],'+/-',s.scalesE[ival]
    # return final weights
    return s.scales

  def drawFits(s,title='random'):
    """ Draw fits in all variables """
    res = []
    for ivar in xrange(len(s.vnames)):
      res += s.drawFit(ivar,title)
    return res

  def drawFitsTF(s,title='random'):
    """ Draw fits in all variables - using the TFractionFitter version """
    res = []
    for ivar in xrange(len(s.vnames)):
      res += s.drawFitTF(ivar,title)
    return res

  def drawFit_noratio(s,ivar,title='random'):
    """ Draw the fit for ivar's variable - this one only makes the default plot, without the ratio"""
    import SuCanvas
    s.canvas = canvas = SuCanvas.SuCanvas()
    canvas.buildDefault(1024,768)
    cc = s.canvas.cd_canvas()
    cc.cd(1)
    s.frame = frame = s.w.var(s.vnames[ivar]).frame()
    RooAbsData.plotOn(s.dataHist, frame , RF.Name("dataHist") )
    if True:
      s.model.plotOn( frame , RF.Name("modelHist") , RF.LineColor(46) , RF.DrawOption("L") ) # RF.MoveToBack()
      ipattern = 3001
      s.model.plotOn( frame , RF.Name("fixedHist") , RF.Components("fixedTemplate") , RF.DrawOption("F") ,
                      RF.FillColor(8) , RF.LineColor(8) , RF.FillStyle( ipattern ) )
      for ift in xrange(0,len(s.free)):
        tempName = s.free[ift].GetName()
        tempName += "Template"
        s.model.plotOn( frame , RF.Name( s.free[ift].GetName() ) , RF.Components( tempName ) , RF.DrawOption("F") , 
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
    return canvas,frame

  def drawFit(s,ivar,title='random'):
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

  def drawFitTF(s,ivar,title='random'):
    """ Draw the fit for ivar's variable - TFractionFitter version """

    import SuCanvas
    s.canvas = canvas = SuCanvas.SuCanvas()
    canvas.buildRatio(1024,768)

    # main plot area: draw {data,model,fixed,qcd}
    canvas.cd_plotPad()
    fit = s.fit
    # data
    s.hdata = data = s.data.Clone()
    data.SetMarkerStyle(1)
    data.SetFillStyle(0)
    #ROOT.gStyle.SetErrorX(0.001)
    data.Draw('X0')
    data.GetXaxis().SetRange(s.fitmin,s.fitmax)
    if s.status==0:
      # model
      s.hmodel = model = s.fit.GetPlot().Clone()
      model.SetLineColor(46)
      model.SetFillStyle(0)
      model.Draw('A SAME')
      # fixed
      ipattern = 3001
      s.hfixed = fixed = fit.GetMCPrediction(0).Clone()
      fixed.SetLineColor(8)
      fixed.SetFillColor(8)
      fixed.SetFillStyle(ipattern)
      fixed.Draw('A SAME H')
      # free (qcd)
      ift = 0
      s.hfree = free = fit.GetMCPrediction(1).Clone()
      free.SetFillColor(s.free[ift].GetFillColor())
      free.SetLineColor(s.free[ift].GetFillColor())
      free.SetFillStyle(1001)
      free.Draw('A SAME H')

    # prettify
    data.GetXaxis().SetTitleOffset( canvas.getXtitleOffset() )
    data.GetYaxis().SetTitleOffset( canvas.getYtitleOffset() )
    data.GetXaxis().SetTitle( s.w.var(s.vnames[ivar]).GetName() )
    data.GetXaxis().SetTitleColor(ROOT.kBlack)
    data.GetXaxis().SetLabelSize( canvas.getLabelSize() )
    data.GetXaxis().SetTitleSize( canvas.getAxisTitleSize() )
    data.GetXaxis().CenterTitle()
    data.GetYaxis().SetTitle( "entries / GeV" )
    data.GetYaxis().SetLabelSize( canvas.getLabelSize() )
    data.GetYaxis().SetTitleSize( canvas.getAxisTitleSize() )
    data.GetYaxis().CenterTitle()
    
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
      fractext.AddText( 'Frac. %s = %.3f #pm %.3f %%'%(s.free[ift].getLegendName(),frac*100.0,s.fractionsE[ift]*100.0) )
      fractext.AddText( 'Scale %s = %.3f #pm %.3f'%(s.free[ift].getLegendName(),s.scales[ift],s.scalesE[ift]) )
    key.Draw("9");
    fractext.Draw("9");

    # ratio
    canvas.cd_ratioPad()
    if s.status==0:
      hmodel = model.Clone()
      scalefactor = 1.0*data.Integral()/model.Integral()
      hmodel.Scale( scalefactor ); # scale model to actual data integral (needed?)
      s.hratio,s.href = data.Clone('hratio'),data.Clone('href')
      s.hratio.Divide(hmodel)
      canvas.drawRefLine(s.href)
      canvas.drawRatio(s.hratio)
      s.hratio.GetXaxis().SetRange(s.fitmin,s.fitmax)
    # finalize
    canvas.update()
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
