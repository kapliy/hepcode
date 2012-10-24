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
      s.errors = []
    def ntot(s):
      assert s.n == len(s.cuts)
      return s.n
    def any_ratios(s):
      return any(s.ratios)
    def disable_ratios(s):
      s.ratios = [False]*len(s.ratios)
    def add(s,name,label,color=None,size=0.7,style=20,cut=None,ratio=None,err=0):
      """ Add one sample """
      s.names.append(name)
      s.labels.append(label)
      s.colors.append( color if color else s.autocolor(len(s.cuts)) )
      s.sizes.append(size*s.msize)
      s.styles.append(style)
      s.cuts.append(cut)
      s.ratios.append(ratio)
      s.errors.append(err)
      s.n+=1
    def prefill_data(s,err=0):
      """ if we also plan to overlay the data """
      s.add('datasub','Data',color=1,style=21,err=err)
    def prefill_mc(s,err=0):
      """ pre-fill with all available MC samples """
      #s.add('sig_pythia','Pythia(MRSTMCal)',err=err)
      #s.add('sig_pythia','Pythia(MRSTMCal->CTEQ6L1)',cut='lha_CT10') #cut='lha_cteq6ll')
      #s.add('sig_sherpa','Sherpa(CTEQ6L1)')
      #s.add('sig_alpgen_herwig','Alpgen/Herwig(CTEQ6L1)',ratio=True,err=err)
      #s.add('sig_alpgen_pythia','Alpgen/Pythia(CTEQ6L1)',ratio=True,err=err)
      s.add('sig_powheg_pythia','PowHeg/Pythia(CT10)',ratio=True,err=err)
      s.add('sig_powheg_herwig','PowHeg/Herwig(CT10)',ratio=True,err=err)
      s.add('sig_mcnlo','MC@NLO(CT10)',ratio=True,err=err)
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
    s.savename = SuCanvas.cleanse(name)
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

  @staticmethod
  def set_uncert_style(obj,dored=True):
    """ set style for uncertainties """
    if not obj: return
    obj.SetFillColor( ROOT.kYellow );
    obj.SetFillStyle( 3001 );
    obj.SetMarkerSize( 0 );
    if dored:
      obj.SetLineColor( ROOT.kRed );
      obj.SetMarkerColor( ROOT.kRed );
    obj.SetLineStyle( 2 );
    obj.SetLineWidth( 2 );

  def drawRatio(s,ratio,hash=True,mstyle=2):
    """ Draws a ratio histogram """
    ratio.SetFillColor( ROOT.kBlack );
    ratio.SetFillStyle( 3154 );
    ratio.SetMarkerStyle( mstyle );
    if hash:
      ratio.Draw("AP E2 same");
    else:
      ratio.Draw("AP same");

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
    s.buildRatio()
    s.cd_plotPad()
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
    s.cd_ratioPad()
    s.hratio,s.href = data.Clone("hratio"),data.Clone("href")
    s.hratio.Divide(stack.GetStack().Last())
    s.drawRefLine(s.href)
    s.drawRatio(s.hratio)
    s.hratio.Draw("AP same")
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
  @staticmethod
  def WRatio(hplus,hminus,title='ratio'):
    """ W+ / W-  workhorse function with correct errors """
    hsum = hplus.Clone(hplus.GetName()+" ratio")
    hsum.Divide(hminus)
    hsum.SetTitle(title)
    return hsum

  @staticmethod
  def from_slices(hpts,heta,imin=1,imax=-1):
    """ Builds a histogram from a collection of 1D histograms (e.g., vs pT) in eta slices
    imin and imax give a range of bins in pT histogram that gets projected on final eta plot
    heta is a dummy abseta histogram that's used to bootstrap binning for the final histogram
    """
    if imax<0: imax = hpts[0].GetNbinsX()
    hout = heta.Clone()
    hout.Reset()
    for ieta in xrange(1,hout.GetNbinsX()+1): # loop over TH1 eta bins that we're going to set. no overflow/underflow!
      i = ieta-1 # array index for hpts
      assert hpts[i]
      val = 0
      err2 = 0
      for ipt in xrange(imin,imax+1):
        assert ipt <= hpts[i].GetNbinsX()+1
        val += hpts[i].GetBinContent(ipt)
        err2 += pow(hpts[i].GetBinError(ipt),2)
      hout.SetBinContent(ieta,val)
      hout.SetBinError(ieta,math.sqrt(err2))
    return hout

  def plotAsymmetryFromComponents(s,hdPOS,hdNEG,hmcPOS,hmcNEG,leg=None):
    """ DEPRECATED: wrapper to make a complete plot of charge asymmetry for data vs mc
    from individual charge-dependent histograms """
    s.data.append((hdPOS,hdNEG,hmcPOS,hmcNEG,leg))
    s.buildRatio()
    s.cd_plotPad()
    # mc
    s.hmcasym = s.WAsymmetry(hmcPOS,hmcNEG)
    s.hmcasym.SetLineColor(ROOT.kBlue)
    s.hmcasym.Draw()
    s.FixupHisto(s.hmcasym)
    s.hmcasym.GetYaxis().SetRangeUser(0.0,0.5)
    s.hmcasym.GetYaxis().SetTitle('Asymmetry')
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
        if not hsys.h: continue
        h = hsys.h.Clone()
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
    elif height=='ratio':
      hs[0].GetYaxis().SetRangeUser(0,2.0);
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
    elif height=='ratio':
      hs[0].GetYaxis().SetRangeUser(0,2.0);
    else:
      hs[0].GetYaxis().SetRangeUser(0,maxh*float(height));
    if True:
      leg.Draw("same")
    s.update()

  def plotAny(s,hplots,M=None,height=1.7,leg=None,title=None,xtitle=None):
    """ A generic function to plot several SuPlot's.
    M is: a PlotOptions object describing formatting and colors
    In this version, systematic error mode is encoded inside M
    Height is: Max*height. Or asym = 0..0.5
    """
    if M:
      assert M.ntot()==len(hplots),'Size mismatch between SuPlots and PlotOptions'
    if not leg:
      leg = ROOT.TLegend(0.55,0.70,0.88,0.88,'Legend',"brNDC")
    if title!=None: leg.SetHeader(title)
    s.data.append( (hplots,leg) )
    if M and M.any_ratios():
      s.buildRatio()
      s.cd_plotPad()
    else:
      s.buildDefault(width=1024,height=800)
      s.cd_canvas();
    # plot actual histograms
    hs = []
    for i,hplot in enumerate(hplots):
      h0 = hplot.nominal().stack.GetStack().Last() if hplot.nominal().stack else hplot.nominal_h()
      htot = None
      assert h0
      if M and M.errors[i]: # total error (including systematics)
        ht = hplot.clone()
        htot = ht.update_errors()
        s.data.append(htot)
      h = h0.Clone()
      hs.append(h)
      s.data.append(h)
      if M:
        h.SetLineColor(M.colors[i])
        h.SetMarkerColor(M.colors[i])
        h.SetMarkerStyle(M.styles[i])
        h.SetMarkerSize(M.sizes[i])
        leg.AddEntry(h,M.labels[i],'LP')
        if htot:
          htot.SetFillColor(M.colors[i])
          htot.SetFillStyle(3004)
      else:
        h.SetLineColor( PlotOptions.autocolor(i) )
      h.Draw() if i==0 else h.Draw('A SAME')
      if htot:
        htot.Draw('A same E2')
      s.FixupHisto(h)
    maxh = max([h.GetMaximum() for h in hs])
    if height=='asym':
      hs[0].GetYaxis().SetRangeUser(0,0.5);
    elif height=='ratio':
      hs[0].GetYaxis().SetRangeUser(0,2.0);
    elif height<3.0:
      hs[0].GetYaxis().SetRangeUser(0,maxh*float(height));
    else:
      hs[0].GetYaxis().SetRangeUser(0,height);
    if xtitle!=None: hs[0].GetXaxis().SetTitle(xtitle)
    if True:
      leg.Draw("same")
    # draw ratios with respect to the first histogram
    if M and M.any_ratios():
      s.cd_ratioPad();
      for i,h in enumerate(hs):
        hratio,href = hs[i].Clone("hratio"),hs[i].Clone("href")
        hratio.Divide(hs[0])
        if i==0:
          s.drawRefLine(href)
        [hratio.SetBinError(ii,0) for ii in xrange(0,hratio.GetNbinsX()+2)]
        s.drawRatio(hratio,hash=False,mstyle=20)
        s.data.append( (hratio,href) )
    s.update()

  def plotStack(s,hstack,hdata,leg=None,height=1.5,mode=0,pave=True):
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
      s.htot = htot = hstack.update_errors()
      s.hsys = hsys = hstack.update_errors(sysonly=True)
      s.set_uncert_style(htot)
      htot.Draw('A SAME E2')
      s.FixupHisto(htot)
      s.data.append((htot,hsys))
    # data
    data.SetMarkerSize(1.0)
    data.Draw("AP same")
    s.FixupHisto(data)
    # calculate QCD fraction
    if leg:
      leg.Draw("same")
      x1,x2,y1,y2 = leg.GetX1(),leg.GetX2(),leg.GetY1(),leg.GetY2() # 0.55 0.88 0.7 0.88
      qcdfrac = hstack.nominal().stack_bg_frac()
      if pave and qcdfrac!=None:
        s.fractext = fractext = ROOT.TPaveText( x1 , y1-0.07 , x2 , y1-0.01 , "BR NDC" )
        fractext.SetFillStyle( 0 ) # hollow
        fractext.SetBorderSize( 0 )
        fractext.SetMargin( 0 )
        try:
          staterr = hstack.nominal().scales[3]/hstack.nominal().scales[2]
        except:
          staterr = 0
        fractext.AddText( 'QCD Frac. = %.3f #pm %.2f%%'%(qcdfrac,staterr*100.0) )
        fractext.Draw('same')
    # ratio
    s.cd_ratioPad();
    s.hratio,s.href = data.Clone("hratio"),data.Clone("href")
    s.hratio.Divide(stack.GetStack().Last())
    s.drawRefLine(s.href)
    if mode==1:
      s.hsysr = hsysr = data.Clone("hratio_sys")
      hsysr.Divide(s.hsys)
      s.set_uncert_style(hsysr,dored=False)
      # symmetrize the systematic error around ratio=1.0
      [hsysr.SetBinContent(ii,1.0) for ii in xrange(0,hsysr.GetNbinsX()+2)]
      hsysr.Draw('A SAME E2')
      s.data.append(hsysr)
    s.drawRatio(s.hratio)
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
                   
  def buildDefault(s,width=600,height=600,title=None):
    s._ratioDrawn = False;
    if not title:
      title = rand_name()
    s._canvas = ROOT.TCanvas( "canvas"+title , "canvas"+title , width , height )
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

  def buildRatio(s,width=600,height=600,title=None):
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
