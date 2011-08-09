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

    s.varList = RooArgList()
    s.varSet = RooArgSet()

    s.w = RooWorkspace('w',kTRUE)
    
    s.fList = RooArgList()
    s.bglist = RooArgList()

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
    s.varList.addClone( RooRealVar( name , name , 1.0*lowerbound , 1.0*upperbound , units ) )
    s.varSet.addClone( RooRealVar( name , name , 1.0*lowerbound , 1.0*upperbound , units ) )

  def clearFitVars(s):
    s.varList.removeAll()
    s.varSet.removeAll()
  
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

  def doFit(s):
    """ do fit and return weights """
    # clear results from previous fit
    s.bglist.removeAll()
    s.fList.removeAll()
    # check to make sure s.varList is not NULL
    assert s.data and s.fixed and len(s.free)>0, 'SuFit has not been supplied with all required input histograms'
    assert s.varList.getSize()>0, "SuFit has not variables in varList"
    print s.varList.getSize()
    
    # clean out old model and datahist
    if s.dataHist:  s.dataHist.Clear()
    if s.model: s.model.Clear()
    if s.fList: s.fList.Clear()
    s.fractions = []
    s.scales = []

    # create RooDataHist for data
    print s.varList,s.data
    s.dataHist = RooDataHist( "dataHist" , "dataHist" , s.varList , s.data )

    # create RooDataHists / RooHistPdfs for the free backgrounds
    freeHists = []
    freeTemplates = []
    for ift in xrange(0,len(s.free)):
      histName = s.free[ift].GetName()
      tempName = s.free[ift].GetName()
      histName += "Hist"
      tempName += "Template"
      freeHists.append( RooDataHist(histName,histName,s.varList,s.free[ift]) )
      freeTemplates.append( RooHistPdf(tempName,tempName,s.varList, freeHists[-1]) )
      s.bglist.addClone( freeTemplates[-1] )

    # create RooDataHist / RooHistPdf for fixed backgrounds
    fixedHist = RooDataHist( "fixedHist" , "fixedHist" , s.varList , fixed.Clone() )
    fixedTemplate = RooHistPdf( "fixedTemplate" , "fixedTemplate" , s.varSet , fixedHist )
    s.bglist.addClone( fixedTemplate )

    # set up fraction vars
    freeVars = []
    for ift in xrange(0,len(s.free)):
      varName = s.free[ift].GetName()
      varName.append( "Frac" )
      freeVars.append( RooRealVar( varName , varName , 0 , 1 ) )
      freeVars[-1].setVal( 0.1 )
      s.fList.addClone( freeVars[-1] )
      
    print "SuFit:    %d templates vs. %d fVars"%(s.bglist.getSize() , s.fList.getSize())
    s.bglist.Print("V")
    s.fList.Print("V")

    # define the model and perform fit
    s.model = RooAddPdf( "model" , "model" , s.bglist , s.fList )
    s.model.fitTo( s.dataHist )
    s.fList.Print( "V" )
    
    # set the _weights variable
    ifrac = 0
    for ifrac in xrange(0,s.fList.getSize()):
      s.fractions.append( s.fList.at(ifrac).getVal() )
      s.scales.append( s.fList.at(ifrac).getVal() )
    for ival in xrange(0,len(s.scales)):
      s.scales[ival] *= s.data.Integral()
      s.scales[ival] /= s.free[ival].Integral()
      if True:
        print "Free Background (fraction & scale):" , s.fList.at(ival).GetName()
        print s.fractions[ival]
        print s.scales[ival]
    # return final weights
    return s.scales

def drawFits(s,name):
  """ Draw fits in all variables """
  res = []
  for ivar in xrange(s.varList.getSize()):
    res += s.drawFit(name,ivar)
  return res
    
def drawFit(s,name,ivar):
  """ Draw the fit for ivar's variable """
  s.canvas = canvas = SuCanvas()
  canvas.buildDefault()
  s.frame = frame = s.varList.at(ivar).frame()
  s.dataHist.plotOn( frame , RF.Name("dataHist") )
  s.model.plotOn( frame , RF.Name("modelHist") , RF.LineColor(46) , RF.DrawOption("L") ) # RF.MoveToBack()
  ipattern = 3001
  s.model.plotOn( frame , RF.Name("fixedHist") , RF.Components("fixedTemplate") , RF.DrawOption("F") ,
                  RF.FillColor(8) , RF.LineColor(8) , RF.FillStyle( ipattern ) )
  for ift in xrange(0,len(s.free)):
    tempName = s.free[ift].GetName()
    tempName += "Template"
    s.model.plotOn( frame , RF.Name( s.free[ift].GetName() ) , RF.Components( tempName ) , RF.DrawOption("F") , 
                    RF.FillColor( s.free[ift].getFillColor() ) , RF.LineColor( s.free[ift].getLineColor() ) )
  frame.GetXaxis().SetTitleOffset( canvas.getXtitleOffset() )
  frame.GetYaxis().SetTitleOffset( canvas.getYtitleOffset() )
  frame.GetXaxis().SetTitle( s.varList.at(ivar).GetName() )
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
    
  s.key = key = ROOT.TLegend(0.6, canvas.getY2()-(0.03*(3+s.free.size())) , canvas.getX2() , canvas.getY2() )
  key.SetTextSize( canvas.getLegendTextSize() )
  key.SetBorderSize( 0 )
  key.SetFillColor( ROOT.kWhite )
  key.AddEntry( "dataHist" ,"data","pe")
  key.AddEntry( "modelHist" , "fit model" , "l" )
  key.AddEntry( "fixedHist" , "fixed backgrounds" , "f" )
  for ift in xrange(0,s.free.size()):
    key.AddEntry( s.free[ift].GetName() , s.free[ift].getLegendName() , "f" )

  s.fractext = fractext = ROOT.TPaveText( 0.13 , 0.94 - ( 0.04 * float(len(s.fractions)) ) , 0.5 , 0.94 , "BL NDC" )
  fractext.SetFillStyle( 0 ) # hollow
  fractext.SetBorderSize( 0 )
  fractext.SetMargin( 0 )
  for ift,frac in enumerate(s.fractions):
    fractext.AddText( '%s = %.3f'%(s.free[ift].getLegendName(),frac) )

  key.Draw("9");
  fractext.Draw("9");
  return canvas,frame
