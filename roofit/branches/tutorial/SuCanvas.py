#!/usr/bin/env python
import re,math
import ROOT

class SuCanvas:
  """ Jordan's canvas class for plotting """

  def FixupHisto(s,h):
    h.GetXaxis().SetTitleOffset( s.getXtitleOffset() );
    h.GetYaxis().SetTitleOffset( s.getYtitleOffset() );
    h.GetXaxis().SetLabelSize( s.getLabelSize() );
    h.GetYaxis().SetLabelSize( s.getLabelSize() );
    h.GetXaxis().SetTitleSize( s.getAxisTitleSize() );
    h.GetYaxis().SetTitleSize( s.getAxisTitleSize() );
    h.GetYaxis().CenterTitle();
    h.GetXaxis().CenterTitle();
    return h

  def drawRatio(s,ratio):
    """ Draws a ratio histogram """
    ratio.SetFillColor( ROOT.kBlack );
    ratio.SetFillStyle( 3154 );
    ratio.SetMarkerStyle( 2 );
    ratio.Draw("AP E2 same");

  def drawRefLine(s,refLine,xtitle=None):
    """ Draws a reference line """
    for ibin in range (1,refLine.GetNbinsX()+1):
      refLine.SetBinContent( ibin , 1.0 )
    refLine.SetLineColor( ROOT.kAzure-7 );
    refLine.SetFillColor( 10 );
    refLine.SetMinimum( 0.5 )
    refLine.SetMaximum( 1.5 )
    refLine.GetXaxis().SetLabelSize( s.getRatioLabelSize() );
    refLine.GetYaxis().SetLabelSize( s.getRatioLabelSize() );
    refLine.GetXaxis().SetTitleSize( s.getRatioAxisTitleSize() );
    refLine.GetYaxis().SetTitleSize( s.getRatioAxisTitleSize() );
    refLine.GetXaxis().SetTitleOffset( s.getRatioXtitleOffset() );
    refLine.GetYaxis().SetTitleOffset( s.getRatioYtitleOffset() );
    refLine.GetYaxis().CenterTitle();
    refLine.GetXaxis().CenterTitle();
    refLine.GetYaxis().SetNdivisions( 406 );
    if xtitle:
      refLine.GetXaxis().SetTitle( xtitle );
    refLine.GetYaxis().SetTitle( "data / MC" );
    refLine.SetLineWidth( 3 );
    refLine.SetLineStyle( ROOT.kDashed );
    refLine.Draw( "hist" );

  def plotStackHisto(s,stack,data,leg=None):
    """ Wrapper to make a complete plot of stack and data overlayed """
    s.buildRatio();
    s.cd_plotPad();
    # mc
    stack.Draw("HIST")
    s.FixupHisto(stack)
    stack.SetMinimum(0.1)
    maximum = max((data.GetMaximum(),stack.GetMaximum()))
    stack.SetMaximum(maximum*1.5)
    #data
    data.SetMarkerSize(1.0)
    data.Draw("AP same")
    s.FixupHisto(data)
    if leg:
      leg.Draw("same")
    # ratio
    s.cd_ratioPad();
    s.hratio,s.href = data.Clone("hratio"),data.Clone("href")
    s.hratio.Divide(stack.GetStack().Last())
    s.drawRefLine(s.href)
    s.drawRatio(s.hratio)
    s.hratio.Draw("AP same");
    s.update()

  def WAsymmetry_old(s,hplus,hminus,title='asymmetry'):
    """ (W+ - W-)/(W+ + W-) workhorse function """
    hsum = hplus.Clone(hplus.GetName()+" asymmetry")
    hsum.Add(hminus,-1.0)
    hdiff = hplus.Clone(hplus.GetName()+" asymmetry_diff")
    hdiff.Add(hminus)
    hsum.Divide(hdiff)
    hsum.SetTitle(title)
    #TODO : correct the errors via 2*sqrt(Np*Nm/(Np+Nm)^3)
    return hsum

  def asym_error(s,Np,sNp,Nm,sNm):
    """ Error propagation formula for (N+ - N-)/(N+ + N-) """
    t = (Np+Nm)**2
    if t==0:
      return 0
    res2 = (2*Nm/t)**2 * sNp**2 + (-2*Np/t)**2 * sNm**2
    return math.sqrt(res2) if res2>0 else 0.0
  def asym_value(s,Np,sNp,Nm,sNm):
    """ returns (N+ - N-)/(N+ + N-) """
    res = (Np-Nm)/(Np+Nm) if Np+Nm>0 else 0
    return res
  def WAsymmetry(s,hplus,hminus,title='asymmetry'):
    """ (W+ - W-)/(W+ + W-) workhorse function with correct errors """
    hsum = hplus.Clone(hplus.GetName()+" asymmetry")
    hsum.Reset()
    for i in xrange(1,hsum.GetNbinsX()+1):
      Np  = hplus.GetBinContent(i)
      sNp = hplus.GetBinError(i)
      Nm  = hminus.GetBinContent(i)
      sNm = hminus.GetBinError(i)
      hsum.SetBinContent(i,s.asym_value(Np,sNp,Nm,sNm))
      hsum.SetBinError(i,s.asym_error(Np,sNp,Nm,sNm))
    hsum.SetTitle(title)
    return hsum

  def plotAsymmetry(s,hdPOS,hdNEG,hmcPOS,hmcNEG,leg=None):
    """ Wrapper to make a complete plot of charge asymmetry for data vs mc """
    s.buildRatio();
    s.cd_plotPad();
    # mc
    s.hmcasym = s.WAsymmetry(hmcPOS,hmcNEG)
    s.hmcasym.SetLineColor(ROOT.kBlue)
    s.hmcasym.Draw()
    s.FixupHisto(s.hmcasym)
    s.hmcasym.GetYaxis().SetRangeUser(0.0,0.5);
    s.hmcasym.GetYaxis().SetTitle('Asymmetry');
    # data
    s.hdasym = s.WAsymmetry(hdPOS,hdNEG)
    s.hdasym.SetMarkerSize(1.0)
    s.hdasym.Draw("AP same")
    s.FixupHisto(s.hdasym)
    if leg:
      leg.Draw("same")
    # ratio
    s.cd_ratioPad();
    s.hratio,s.href = s.hdasym.Clone("hratio"),s.hdasym.Clone("href")
    s.hratio.Divide(s.hmcasym)
    s.drawRefLine(s.href)
    s.drawRatio(s.hratio)
    s.hratio.Draw("AP same");
    s.update()

  def plotTagProbe(s,hda_bef,hda_aft,hmc_bef,hmc_aft,xtitle='var'):
    """ Makes a large tag-and-probe canvas """
    s.buildDefault(width=1200,height=600)
    s.hda_r = [None for i in xrange(2)]
    s.hmc_r = [None for i in xrange(2)]
    s.hsc_r = [None for i in xrange(2)]
    for iq in xrange(2):
      # colors and style
      hmc_aft[iq].SetLineColor(ROOT.kBlue)
      hmc_aft[iq].SetMarkerColor(ROOT.kBlue)
      hmc_aft[iq].SetMarkerSize(0.4);
      hmc_bef[iq].SetLineColor(ROOT.kBlue)
      hmc_bef[iq].SetMarkerColor(ROOT.kBlue)
      hmc_bef[iq].SetMarkerSize(0.4);
      hda_aft[iq].SetLineColor(ROOT.kRed)
      hda_aft[iq].SetMarkerColor(ROOT.kRed)
      hda_aft[iq].SetMarkerSize(0.6);
      hda_bef[iq].SetLineColor(ROOT.kRed)
      hda_bef[iq].SetMarkerColor(ROOT.kRed)
      hda_bef[iq].SetMarkerSize(0.6);
      # ratios
      s.hda_r[iq] = hda_aft[iq].Clone()
      s.hda_r[iq].Divide(hda_aft[iq],hda_bef[iq],1.0,1.0,"B")
      s.hmc_r[iq] = hmc_aft[iq].Clone()
      s.hmc_r[iq].Divide(hmc_aft[iq],hmc_bef[iq],1.0,1.0,"B")
      s.hsc_r[iq] = s.hda_r[iq].Clone()
      s.hsc_r[iq].Divide(s.hda_r[iq],s.hmc_r[iq],1.0,1.0,"")
      s.hsc_r[iq].SetLineColor(ROOT.kBlack)
      s.hsc_r[iq].SetMarkerColor(ROOT.kBlack)
      s.hsc_r[iq].SetMarkerSize(0.4);
    c = s.cd_canvas()
    c.Divide(2,2)
    c.cd(1)
    s.hda_r[0].Draw("")
    s.hmc_r[0].Draw("A same")
    s.hda_r[0].GetYaxis().SetRangeUser(0.50,1.10);
    s.hda_r[0].GetYaxis().SetTitle('mu+ efficiency');
    s.hda_r[0].GetXaxis().SetTitle(xtitle);
    c.cd(2)
    s.hda_r[1].Draw("")
    s.hmc_r[1].Draw("A same")
    s.hda_r[1].GetYaxis().SetRangeUser(0.50,1.10);
    s.hda_r[1].GetYaxis().SetTitle('mu- efficiency');
    s.hda_r[1].GetXaxis().SetTitle(xtitle);
    c.cd(3)
    s.hsc_r[0].Draw("")
    s.hsc_r[0].GetYaxis().SetRangeUser(0.90,1.10);
    s.hsc_r[0].GetYaxis().SetTitle('mu+ scale factor (data/MC)');
    s.hsc_r[0].GetXaxis().SetTitle(xtitle);
    s.l0 = ROOT.TGraph(2)
    s.l0.SetPoint(0,s.hsc_r[0].GetXaxis().GetXmin(),1.0)
    s.l0.SetPoint(1,s.hsc_r[0].GetXaxis().GetXmax(),1.0)
    s.l0.SetLineWidth(1)
    s.l0.SetLineColor(ROOT.kRed)
    s.l0.SetLineStyle(2)
    s.l0.Draw('l')
    c.cd(4)
    s.hsc_r[1].Draw("")
    s.hsc_r[1].GetYaxis().SetRangeUser(0.90,1.10);
    s.hsc_r[1].GetYaxis().SetTitle('mu- scale factor (data/MC)');
    s.hsc_r[1].GetXaxis().SetTitle(xtitle);
    s.l1 = ROOT.TGraph(2)
    s.l1.SetPoint(0,s.hsc_r[0].GetXaxis().GetXmin(),1.0)
    s.l1.SetPoint(1,s.hsc_r[0].GetXaxis().GetXmax(),1.0)
    s.l1.SetLineWidth(1)
    s.l1.SetLineColor(ROOT.kRed)
    s.l1.SetLineStyle(2)
    s.l1.Draw('l')
                   
  def __init__(s):
    s._ratioDrawn = False

  def buildDefault(s,title='default',width=800,height=600):
    s._ratioDrawn = False;
    s._canvas = ROOT.TCanvas( title , title , 0 , 0 , width , height );
    s._canvas.SetFillColor( ROOT.kWhite );
    s._canvas.GetFrame().SetBorderMode( 0 );
    s._canvas.GetFrame().SetBorderSize( 0 );
    s._canvas.SetTopMargin( 0.05 );
    s._canvas.SetBottomMargin( 0.1 );
    s._canvas.SetRightMargin( 0.05 );
    s._canvas.SetLeftMargin( 0.1 );
    s._canvas.cd();
    s._plotPad = ROOT.TPad()
    s._ratioPad = ROOT.TPad()
    s._coverPad = ROOT.TPad()
    s._labelSize = 0.025;
    s._axisTitleSize = 0.025;
    s._legendTextSize = 0.025;
    s._legendVertSpacing = 0.03;
    s._paveTextSize = 0.025;
    s._xTitleOffset = 1.25;
    s._yTitleOffset = 2.;

  def update(s):
    for c in (s._plotPad,s._coverPad,s._ratioPad,s._canvas):
      c.Modified()
      c.Update()

  def buildRatio(s):
    s._canvas = ROOT.TCanvas( "canvas" , "ratio" , 200 , 10 , 800 , 800 );
    s._canvas.SetFillColor( ROOT.kWhite );
    s._canvas.GetFrame().SetBorderMode( 0 );
    s._canvas.GetFrame().SetBorderSize( 0 );
    s._canvas.SetTopMargin( 0.05 ); # set this so getY2 works
    s._ratioDrawn = True;
    # pad containing the main plot + legend
    s._plotPad = ROOT.TPad( "plotPad" , "plotPad" , 0. , 0. , 1. , 1. );
    s._plotPad.SetBottomMargin( 0.25 );
    s._plotPad.SetTopMargin( 0.05 );
    s._plotPad.SetRightMargin( 0.05 );
    s._plotPad.SetLeftMargin( 0.1 );
    s._plotPad.SetFillColor( ROOT.kWhite );
    s._plotPad.Draw();
    # pad covering the bottom number
    s._coverPad = ROOT.TPad( "coverPad" , "coverPad" , 0. , 0.24 , 0.0975 , 0.28 );
    s._coverPad.SetBottomMargin( 0. );
    s._coverPad.SetTopMargin( 0. );
    s._coverPad.SetLeftMargin( 0. );
    s._coverPad.SetRightMargin( 0. );
    s._coverPad.SetFillColor( ROOT.kWhite );
    s._coverPad.Draw();
    # pad containing ratio histogram
    s._ratioPad = ROOT.TPad( "ratioPad" , "ratioPad" , 0. , 0. , 1. , 0.25 );
    s._ratioPad.SetBottomMargin( 0.275 );
    s._ratioPad.SetTopMargin( 0. );
    s._ratioPad.SetLeftMargin( 0.1 );
    s._ratioPad.SetRightMargin( 0.05 );
    s._ratioPad.SetFillColor( ROOT.kWhite );
    s._ratioPad.Draw();
    s._plotPad.cd();
    
    s._labelSize = 0.025;
    s._axisTitleSize = 0.025;
    s._ratioLabelSize = 0.1;
    s._ratioAxisTitleSize = 0.1;
    s._legendTextSize = 0.025;
    s._legendVertSpacing = 0.03;
    s._paveTextSize = 0.025;
    s._xTitleOffset = 1.25;
    s._yTitleOffset = 2.;
    s._ratioXtitleOffset = 1.;
    s._ratioYtitleOffset = 0.5;

  def SaveAs(s,name2,ext='png'):
    name = re.sub(r'[^\w]', '_', name2)
    c = s._canvas
    c.SaveAs('%s.%s'%(name,ext))
    if ext=='eps':
        c.SaveAs('%s.%s'%(name,'png'))
  def saveAs( s, fullname ):
    """ save the canvas """
    s._canvas.SaveAs( fullname );
  def saveAsEps( s, filename ):
    filename +=  ".eps" 
    s._canvas.SaveAs( filename );
  def saveAsPng( s, filename ):
    filename += ".png"
    s._canvas.SaveAs( filename.c_str() );
    
  def clear(s):
    """ clear the canvas """
    s._canvas.Clear()

  # cd into different pads
  def cd_canvas(s):
    return s._canvas.cd()
  def cd_plotPad(s):
    assert s._plotPad
    return s._plotPad.cd()
  def cd_ratioPad(s):
    assert s._ratioPad
    return s._ratioPad.cd()

  # get vats
  def getCanvas(s):
    return s._canvas
  def getPlotPad(s):
    return s._plotPad
  def getRatioPad():
    return s._ratioPad
  def getLabelSize(s):
    return s._labelSize
  def getAxisTitleSize(s):
    return s._axisTitleSize
  def getLegendTextSize(s):
    return s._legendTextSize
  def getLegendVertSpacing(s):
    return s._legendVertSpacing
  def getPaveTextSize(s):
    return s._paveTextSize
  def getRatioLabelSize(s):
    return s._ratioLabelSize
  def getRatioAxisTitleSize(s):
    return s._ratioAxisTitleSize
  def getXtitleOffset(s):
    return s._xTitleOffset
  def getYtitleOffset(s):
    return s._yTitleOffset
  def getRatioXtitleOffset(s):
    return s._ratioXtitleOffset;
  def getRatioYtitleOffset(s):
    return s._ratioYtitleOffset
  def getY2(s):
    return 1. - float( s._canvas.GetTopMargin() ) - 0.01;
  def getY1(s):
    return float( s._canvas.GetBottomMargin() + 0.01 );
  def getX1(s):
    return float( s._canvas.GetLeftMargin() + 0.01 );
  def getX2(s):
    return 1. - float( s._canvas.GetRightMargin() ) - 0.01;
