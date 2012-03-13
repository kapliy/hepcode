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
  def addFreeBackground(s,background):
    s.free.append( background )
  def fArgList(s):
    return RooArgList(s.w.var(s.fnames[0]))
  def ArgList(s):
    return RooArgList(s.w.var(s.vnames[0]))
  def ArgSet(s):
    return RooArgSet(s.w.var(s.vnames[0]))

  def doFit(s):
    """ do fit and return weights """
    # clear results from previous fit
    assert s.data and s.fixed and len(s.free)>0, 'SuFit has not been supplied with all required input histograms'
    
    # clean out old model and datahist
    if s.dataHist:  s.dataHist.Clear()
    if s.model: s.model.Clear()
    s.fractions = []
    s.scales = []

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
    for ival in xrange(0,len(s.scales)):
      s.scales[ival] *= s.data.Integral()
      s.scales[ival] /= s.free[ival].Integral()
      if True:
        print "Free Background (fraction & scale):",fname
        print s.fractions[ival]
        print s.scales[ival]
    # return final weights
    return s.scales

  def drawFits(s,title='random'):
    """ Draw fits in all variables """
    res = []
    for ivar in xrange(len(s.vnames)):
      res += s.drawFit(ivar,title)
    return res

  def drawFit(s,ivar,title='random'):
    """ Draw the fit for ivar's variable """
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
      fractext.AddText( 'Frac. %s = %.3f'%(s.free[ift].getLegendName(),frac) )
      fractext.AddText( 'Scale %s = %.3f'%(s.free[ift].getLegendName(),s.scales[ift]) )
    key.Draw("9");
    fractext.Draw("9");
    return canvas,frame
