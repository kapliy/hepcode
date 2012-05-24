#!/usr/bin/env python
import re,math
import ROOT

def rand_name(ln=10):
  import random,string
  return ''.join(random.choice(string.ascii_uppercase + string.digits) for x in range(ln))

class PlotOptions:
    """ Handles colors and marker styles for various MC generators """
    msize = 1.5
    def __init__(s):
        s.n = 0
        s.names = []
        s.labels = []
        s.colors = []
        s.sizes = []
        s.styles = []
        s.cuts = []
        s.ratios = []
    def ntot(s):
        assert s.n == len(s.cuts)
        return s.n
    def add(s,name,label,color=None,size=0.7,style=20,cut=None,ratio=None):
        """ Add one sample """
        s.names.append(name)
        s.labels.append(label)
        s.colors.append( color if color else s.autocolor(len(s.cuts)) )
        s.sizes.append(size*s.msize)
        s.styles.append(style)
        s.cuts.append(cut)
        s.ratios.append(ratio)
        s.n+=1
    def prefill_data(s):
        """ if we also plan to overlay the data """
        s.add('datasub','Data',color=1,style=21)
    def prefill_mc(s):
        """ pre-fill with all available MC samples """
        s.add('pythia','Pythia(MRSTMCal)')
        #s.add('pythia','Pythia(MRSTMCal->CTEQ6L1)',cut='lha_cteq6ll')
        #s.add('sherpa','Sherpa(CTEQ6L1)')
        s.add('alpgen_herwig','Alpgen/Herwig(CTEQ6L1)',ratio=True)
        #s.add('alpgen_pythia','Alpgen/Pythia(CTEQ6L1)')
        s.add('mcnlo','MC@NLO(CT10)',ratio=True)
        #s.add('powheg_herwig','PowHeg/Herwig(CT10)')
        s.add('powheg_pythia','PowHeg/Pythia(CT10)',ratio=True)
    @staticmethod
    def autocolor(i):
        """ choose a reasonable sequence of colors """
        colorlist = [2,3,4,5,6,20,28,41,46]
        return colorlist[i] if i<len(colorlist) else (1 if i==11 else i)

class SuCanvas:
  """ Jordan's canvas class for plotting """

  _refLineMin = 0.5
  _refLineMax = 1.5

  savedir = './'
  savetype = 'png'
  savetag = ''

  def __init__(s,name='plot'):
    s.data = []
    s._ratioDrawn = False
    s.savename = name
    s._plotPad,s._coverPad,s._ratioPad,s._canvas = None,None,None,None

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
    refLine.SetMinimum( s._refLineMin )
    refLine.SetMaximum( s._refLineMax )
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

  def plotStackHisto_H(s,stack,data,leg=None):
    """ Wrapper to make a complete plot of stack and data overlayed - TH1 version (deprecated) """
    s.data.append((stack,data,leg))
    s.buildRatio();
    s.cd_plotPad();
    # mc
    stack.Draw("HIST")
    s.FixupHisto(stack)
    stack.SetMinimum(0.0) # was: 0.1
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
    #note: errors are incorrect here
    return hsum
  @staticmethod
  def asym_error(Np,sNp,Nm,sNm):
    """ Error propagation formula for (N+ - N-)/(N+ + N-) """
    t = (Np+Nm)**2
    if t==0:
      return 0
    res2 = (2*Nm/t)**2 * sNp**2 + (-2*Np/t)**2 * sNm**2
    return math.sqrt(res2) if res2>0 else 0.0
  @staticmethod
  def asym_value(Np,sNp,Nm,sNm):
    """ returns (N+ - N-)/(N+ + N-) """
    res = (Np-Nm)/(Np+Nm) if Np+Nm>0 else 0
    return res
  @staticmethod
  def WAsymmetry(hplus,hminus,title='asymmetry'):
    """ (W+ - W-)/(W+ + W-) workhorse function with correct errors """
    hsum = hplus.Clone(hplus.GetName()+" asymmetry")
    hsum.Reset()
    for i in xrange(1,hsum.GetNbinsX()+1):
      Np  = hplus.GetBinContent(i)
      sNp = hplus.GetBinError(i)
      Nm  = hminus.GetBinContent(i)
      sNm = hminus.GetBinError(i)
      hsum.SetBinContent(i,SuCanvas.asym_value(Np,sNp,Nm,sNm))
      hsum.SetBinError(i,SuCanvas.asym_error(Np,sNp,Nm,sNm))
    hsum.SetTitle(title)
    return hsum

  def plotAsymmetryFromComponents(s,hdPOS,hdNEG,hmcPOS,hmcNEG,leg=None):
    """ DEPRECATED: wrapper to make a complete plot of charge asymmetry for data vs mc
    from individual charge-dependent histograms """
    s.data.append((hdPOS,hdNEG,hmcPOS,hmcNEG,leg))
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

  @staticmethod
  def set_uncert_style(obj):
    """ set style for uncertainties """
    if not obj: return
    obj.SetFillColor( ROOT.kYellow );
    obj.SetFillStyle( 3001 );
    obj.SetLineColor( ROOT.kRed );
    obj.SetMarkerColor( ROOT.kRed );
    obj.SetLineStyle( 2 );
    obj.SetLineWidth( 2 );
  
  def plotOne(s,hplot,mode=0,height=1.5,leg=None,title=None):
    """ A generic function to plot an instance of SuPlot.
    Mode is: 0=nominal; 1=total errors; 2=all systematic plots
    Height is: Max*height. Or asym = 0..0.5
    """
    s.buildDefault(width=1024,height=400)
    s.cd_canvas();
    if not leg:
      leg = ROOT.TLegend(0.55,0.60,0.88,0.92,'Legend',"brNDC")
    if title!=None: leg.SetHeader(title)
    s.data.append( (hplot,leg) )
    hs = []
    if mode==1:
      ht = hplot.clone()
      h = ht.update_errors()
      hs.append(h)
      h.Draw()
      s.data.append(h)
      s.FixupHisto(h)
    else:
      for i,hsys in enumerate(hplot.flat):
        h = hsys.h.Clone()
        if not h: continue
        hs.append(h)
        s.data.append(h)
        color = PlotOptions.autocolor(i)
        h.SetLineColor(color)
        h.SetMarkerColor(color)
        leg.AddEntry(h,hsys.name,'LP')
        if i==0:
          h.Draw()
        else:
          h.Draw('A SAME')
        s.FixupHisto(h)
        if mode==0: break
    maxh = max([h.GetMaximum() for h in hs])
    if height=='asym':
      hs[0].GetYaxis().SetRangeUser(0,0.5);
    else:
      hs[0].GetYaxis().SetRangeUser(0,maxh*float(height));
    if mode==2: # draw legend for multi-systematic studies
      leg.Draw("same")
    s.update()

  def plotMany(s,hplots,M=None,mode=0,height=1.5,leg=None,title=None):
    """ A generic function to plot several SuPlot's.
    M is: a PlotOptions object describing formatting and colors
    Mode is: 0=nominal; 1=total errors
    Height is: Max*height. Or asym = 0..0.5
    """
    if M:
      assert M.ntot()==len(hplots),'Size mismatch between SuPlots and PlotOptions'
    if not leg:
      leg = ROOT.TLegend(0.55,0.70,0.88,0.88,'Legend',"brNDC")
    if title!=None: leg.SetHeader(title)
    s.data.append( (hplots,leg) )
    s.buildDefault(width=1024,height=400)
    s.cd_canvas();
    hs = []
    for i,hplot in enumerate(hplots):
      h0 = hplot.nominal().stack.GetStack().Last() if hplot.nominal().stack else hplot.nominal_h()
      assert h0
      if mode==1:
        ht = hplot.clone()
        h0 = ht.update_errors()
      if not h0: continue
      h = h0.Clone()
      hs.append(h)
      s.data.append(h)
      if M:
        h.SetLineColor(M.colors[i])
        h.SetMarkerColor(M.colors[i])
        h.SetMarkerStyle(M.styles[i])
        h.SetMarkerSize(M.sizes[i])
        leg.AddEntry(h,M.labels[i],'LP')
      else:
        h.SetLineColor( PlotOptions.autocolor(i) )
      if i==0:
        h.Draw()
      else:
        h.Draw('A SAME')
      s.FixupHisto(h)
    maxh = max([h.GetMaximum() for h in hs])
    if height=='asym':
      hs[0].GetYaxis().SetRangeUser(0,0.5);
    else:
      hs[0].GetYaxis().SetRangeUser(0,maxh*float(height));
    if True:
      leg.Draw("same")
    s.update()

  def plotStack(s,hstack,hdata,leg=None,height=1.5,mode=0):
    """ Wrapper to make a complete plot of stack and data overlayed - SuData version
    mode=0 - nominal only
    mode=1 - apply systematics
    """
    s.buildRatio();
    s.cd_plotPad();
    stack = hstack.nominal().stack.Clone()
    data = hdata.nominal_h()
    s.data.append((stack,data,leg))
    hsys = None
    # mc
    stack.Draw("HIST")
    s.FixupHisto(stack)
    stack.SetMinimum(0.0) # was: 0.1
    maximum = max((data.GetMaximum(),stack.GetMaximum()))
    stack.SetMaximum(maximum*height)
    # systematics
    if mode==1:
      hsys = hstack.update_errors()
      hsys.Draw('A SAME E3')
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

  def Matrix_loose(s,Nt,Nl,er,ef):
    """ Matrix method estimate of QCD - formula from W inclusive measurement
    This method returns the QCD histogram BEFORE the tight cut
    I.e., this gives N_QCD, pre-isolation
    """
    hsum = Nt.Clone(Nt.GetName()+"_matrix_looose")
    hsum.Reset()
    for i in xrange(1,hsum.GetNbinsX()+1):
      nis,snis = Nt.GetBinContent(i),Nt.GetBinError(i)
      nlo,snlo = Nl.GetBinContent(i),Nl.GetBinError(i)
      enon,senon = er.GetBinContent(i),er.GetBinError(i)
      eqcd,seqcd = ef.GetBinContent(i),ef.GetBinError(i)
      a = nis*1.0/nlo
      sa = a * math.sqrt( (snis/nis)**2 + (snlo/nlo)**2 )
      mean = (nlo*enon - nis) / (enon-eqcd)
      err = 0
      try:
        errM = [1/math.sqrt(nlo),senon/(enon-a) - senon/(enon-eqcd),sa/(enon-a),seqcd/(enon-eqcd)]
        err = mean * math.sqrt(sum([z*z for z in errM]))
      except:
        print 'Bad bin:',i
        err = 0
      hsum.SetBinContent(i,mean)
      hsum.SetBinError(i,err)
    return hsum
  def Matrix_tight(s,Nt,Nl,er,ef):
    """ Matrix method estimate of QCD - formula from W inclusive measurement
    This method returns the QCD histogram AFTER the tight cut
    I.e., this gives N_QCD, post-isolation
    """
    hsum = Nt.Clone(Nt.GetName()+"_matrix_tight")
    hsum.Reset()
    for i in xrange(1,hsum.GetNbinsX()+1):
      nis,snis = Nt.GetBinContent(i),Nt.GetBinError(i)
      nlo,snlo = Nl.GetBinContent(i),Nl.GetBinError(i)
      enon,senon = er.GetBinContent(i),er.GetBinError(i)
      eqcd,seqcd = ef.GetBinContent(i),ef.GetBinError(i)
      a = nis*1.0/nlo
      sa = a * math.sqrt( (snis/nis)**2 + (snlo/nlo)**2 )
      mean = eqcd * (nlo*enon - nis) / (enon-eqcd)
      err = 0
      try:
        errM = [1/math.sqrt(nlo),senon*(a-eqcd)/((enon-a)*(enon-eqcd)),sa/(enon-a),seqcd*enon/((enon-eqcd)*eqcd)]
        err = mean * math.sqrt(sum([z*z for z in errM]))
      except:
        print 'Bad bin:',i
        err = 0
      hsum.SetBinContent(i,mean)
      hsum.SetBinError(i,err)
    return hsum

  def Efficiency_old(s,hatot,hntot):
    """ Binomial errors """
    hsum = hatot.Clone()
    hsum.Divide(hatot,hntot,1.0,1.0,"B")
    return hsum
    
  def Efficiency(s,hatot,hntot):
    """ Acceptance/efficiency workhorse function with correct errors """
    return s.Efficiency_old(hatot,hntot) # per Lauren, just use Binomial errors
    import SuEfficiency
    oef = SuEfficiency.SuEfficiency()
    hsum = hntot.Clone(hntot.GetName()+"_efficiency")
    hsum.Reset()
    for i in xrange(1,hsum.GetNbinsX()+1):
      atot = hatot.GetBinContent(i)
      ntot = hntot.GetBinContent(i)
      mean,err = oef.signal(ntot,atot,erravg=True)
      hsum.SetBinContent(i,mean)
      hsum.SetBinError(i,err)
    return hsum

  def plotTagProbe(s,hda_bef,hda_aft,hmc_bef,hmc_aft,xtitle='var'):
    """ Makes a large tag-and-probe canvas """
    s.buildDefault(width=1200,height=600)
    s.hda_r = [None for i in xrange(2)]
    s.hmc_r = [None for i in xrange(2)]
    s.hsc_r = [None for i in xrange(2)]
    s.hq_r = None
    color = (ROOT.kRed,ROOT.kBlue,46,38)
    for iq in xrange(2):
      # efficiencies
      if False:
        s.hda_r[iq] = hda_aft[iq].Clone()
        s.hda_r[iq].Divide(hda_aft[iq],hda_bef[iq],1.0,1.0,"B")
        s.hmc_r[iq] = hmc_aft[iq].Clone()
        s.hmc_r[iq].Divide(hmc_aft[iq],hmc_bef[iq],1.0,1.0,"B")
      else:
        s.hda_r[iq] = s.Efficiency(hda_aft[iq],hda_bef[iq])
        s.hmc_r[iq] = s.Efficiency(hmc_aft[iq],hmc_bef[iq])
      # colors and style
      s.hmc_r[iq].SetLineColor(color[iq+2])
      s.hmc_r[iq].SetMarkerColor(color[iq+2])
      s.hmc_r[iq].SetMarkerSize(0.8);
      s.hda_r[iq].SetLineColor(color[iq])
      s.hda_r[iq].SetMarkerColor(color[iq])
      s.hda_r[iq].SetMarkerSize(1.0);
      s.hda_r[iq].SetMarkerStyle(22);
      # ratios of data to MC
      s.hsc_r[iq] = s.hda_r[iq].Clone()
      s.hsc_r[iq].Divide(s.hda_r[iq],s.hmc_r[iq],1.0,1.0,"")
      s.hsc_r[iq].SetLineColor(color[iq])
      s.hsc_r[iq].SetMarkerColor(color[iq])
      s.hsc_r[iq].SetMarkerSize(0.4);
    # ratios of ratios data/MC for mu+/mu-
    s.hq_r = s.hsc_r[0].Clone()
    s.hq_r.Divide(s.hsc_r[0],s.hsc_r[1],1.0,1.0,"")
    s.hq_r.SetLineColor(ROOT.kBlack)
    s.hq_r.SetMarkerColor(ROOT.kBlack)
    # plot on the canvas
    c = s.cd_canvas()
    c.Divide(3,2)
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
    c.cd(4)
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
    c.cd(5)
    s.hsc_r[1].Draw("")
    s.hsc_r[1].GetYaxis().SetRangeUser(0.90,1.10);
    s.hsc_r[1].GetYaxis().SetTitle('mu- scale factor (data/MC)');
    s.hsc_r[1].GetXaxis().SetTitle(xtitle);
    s.l1 = ROOT.TGraph(2)
    s.l1.SetPoint(0,s.hsc_r[0].GetXaxis().GetXmin(),1.0)
    s.l1.SetPoint(1,s.hsc_r[0].GetXaxis().GetXmax(),1.0)
    s.l1.SetLineWidth(1)
    s.l1.SetLineColor(ROOT.kBlack)
    s.l1.SetLineStyle(2)
    s.l1.Draw('l')
    c.cd(6)
    s.hq_r.Draw("")
    s.hq_r.GetYaxis().SetRangeUser(0.90,1.10);
    s.hq_r.GetYaxis().SetTitle('Ratio of scale factors: mu+/mu-');
    s.hq_r.GetXaxis().SetTitle(xtitle);
    s.lq = ROOT.TGraph(2)
    s.lq.SetPoint(0,s.hq_r.GetXaxis().GetXmin(),1.0)
    s.lq.SetPoint(1,s.hq_r.GetXaxis().GetXmax(),1.0)
    s.lq.SetLineWidth(1)
    s.lq.SetLineColor(ROOT.kBlack)
    s.lq.SetLineStyle(2)
    s.lq.Draw('l')
                   
  def buildDefault(s,width=800,height=600,title=None):
    s._ratioDrawn = False;
    if not title:
      title = rand_name()
    s._canvas = ROOT.TCanvas( "canvas"+title , "canvas"+title , 200 , 10 , 800 , 800 );
    s._canvas.SetFillColor( ROOT.kWhite );
    s._canvas.GetFrame().SetBorderMode( 0 );
    s._canvas.GetFrame().SetBorderSize( 0 );
    s._canvas.SetTopMargin( 0.05 );
    s._canvas.SetBottomMargin( 0.1 );
    s._canvas.SetRightMargin( 0.05 );
    s._canvas.SetLeftMargin( 0.1 );
    s._canvas.cd();
    #s._plotPad = ROOT.TPad()
    #s._ratioPad = ROOT.TPad()
    #s._coverPad = ROOT.TPad()
    s._labelSize = 0.025;
    s._axisTitleSize = 0.025;
    s._legendTextSize = 0.025;
    s._legendVertSpacing = 0.03;
    s._paveTextSize = 0.025;
    s._xTitleOffset = 1.25;
    s._yTitleOffset = 2.;

  def update(s):
    for c in (s._plotPad,s._coverPad,s._ratioPad,s._canvas):
      if not c: continue
      c.Modified()
      c.Update()

  def buildRatio(s,width=800,height=800,title=None):
    if not title:
      title = rand_name()
    s._canvas = ROOT.TCanvas( 'canvasm'+title , 'canvasm'+title , width , height );
    s._canvas.SetFillColor( ROOT.kWhite );
    s._canvas.GetFrame().SetBorderMode( 0 );
    s._canvas.GetFrame().SetBorderSize( 0 );
    s._canvas.SetTopMargin( 0.05 ); # set this so getY2 works
    s._ratioDrawn = True;
    # pad containing the main plot + legend
    s._plotPad = ROOT.TPad( title+"plotPad" , title+"plotPad" , 0. , 0. , 1. , 1. );
    s._plotPad.SetBottomMargin( 0.25 );
    s._plotPad.SetTopMargin( 0.05 );
    s._plotPad.SetRightMargin( 0.05 );
    s._plotPad.SetLeftMargin( 0.1 );
    s._plotPad.SetFillColor( ROOT.kWhite );
    s._plotPad.Draw();
    # pad covering the bottom number
    s._coverPad = ROOT.TPad( title+"coverPad" , title+"coverPad" , 0. , 0.24 , 0.0975 , 0.28 );
    s._coverPad.SetBottomMargin( 0. );
    s._coverPad.SetTopMargin( 0. );
    s._coverPad.SetLeftMargin( 0. );
    s._coverPad.SetRightMargin( 0. );
    s._coverPad.SetFillColor( ROOT.kWhite );
    s._coverPad.Draw();
    # pad containing ratio histogram
    s._ratioPad = ROOT.TPad( title+"ratioPad" , title+"ratioPad" , 0. , 0. , 1. , 0.25 );
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

  @staticmethod
  def cleanse(name2):
    return re.sub(r'[^\w]', '_', name2)
  def SaveSelf(s):
    """ auto-save version """
    assert s.savedir
    assert s.savename
    assert s.savetype
    return s.SaveAs(s.savetag+'_'+s.savename,s.savetype,s.savedir)
  def SaveAs(s,name2,ext='png',DIR='./'):
    name = s.cleanse(name2)
    c = s._canvas
    c.SaveAs(DIR+'%s.%s'%(name,ext))
    if ext=='eps':
        c.SaveAs(DIR+'%s.%s'%(name,'png'))
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
