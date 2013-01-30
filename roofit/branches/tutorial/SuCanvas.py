#!/usr/bin/env python
import re,math,os
from common import rand_name
import ROOT

class SuCanvas:
    """ Jordan's canvas class for plotting
    convolved with Sebastian TStyle for W inclusive paper
    """

    cgStyle = None
    g_ratio_frac = 0.3;
    g_lin_main_y_title_offset = 1.5;
    g_lin_ratio_y_title_offset = 1.5;
    g_log_main_y_title_offset = 1.3;
    g_log_ratio_y_title_offset = g_lin_ratio_y_title_offset;
    g_ratio_x_title_offset = 3.4;
    g_text_size = 21; # in pixel
    g_smaller_text_size = g_text_size*0.85; # in pixel
    g_text_font = 43; # force font style 4 and size specification in pixels (font%10==3)
    g_text_size_legend = None; # over-ride (in pixel)
    g_text_size_pave = None;   # over-ride (in pixel)
    g_marker_size = 0.8
    # should have no influence because of fixed text and symbol size
    g_legend_width = 0.2;
    # same symbol width and height for legends with different number of entries
    g_legend_height_per_entry = 0.1;
    g_legend_symbol_width = 0.05;
    # default location for TLegend left-top corner
    g_legend_x1_ndc = 0.55
    g_legend_y2_ndc = 0.9
    
    _refLineMin = 0.81
    _refLineMax = 1.19
    
    savedir = './'
    savetypes = ['png',]
    savetag = ''

    def __init__(s,name='plot'):
        s.data = []
        s._ratioDrawn = False
        s._ratioName = "Data / MC"
        s.savename = SuCanvas.cleanse(name)
        s._mlogy = False
        s._rlogy = False
        s._plotPad,s._coverPad,s._ratioPad,s._canvas = None,None,None,None

    def namebase(s):
        return s.savename
    @staticmethod
    def best_legend_and_height(h,default_height=2.15):
        """ returns legend x1 and y2, along with histogram SetMaximum scale """
        nbins = h.GetNbinsX()
        frange = range(1,nbins)
        lrange = range(1,nbins/2)
        rrange = range(nbins/2,nbins)
        maxbin = h.GetMaximumBin()
        avgmax = sum([ h.GetBinContent(i) for i in xrange(1,h.GetNbinsX()) ])/h.GetNbinsX()
        avgmaxL = sum([ h.GetBinContent(i) for i in lrange ])/len(lrange)
        avgmaxR = sum([ h.GetBinContent(i) for i in rrange ])/len(rrange)
        fullmax = h.GetMaximum()
        halfmax = fullmax*0.5
        fullmin = h.GetMinimum()
        leftmin = min([ h.GetBinContent(i) for i in lrange ])
        rightmin = min([ h.GetBinContent(i) for i in rrange ])
        leftmax = max([ h.GetBinContent(i) for i in lrange ])
        rightmax = max([ h.GetBinContent(i) for i in rrange ])
        if all( [ h.GetBinContent(i)<halfmax for i in lrange ] ):
            print 'LegendFinder: Found histogram mostly on right side'
            return 0.12,None,1
        elif all( [ h.GetBinContent(i)<halfmax for i in rrange ] ):
            print 'LegendFinder: Found histogram mostly on left side'
            return 0.55,None,1
        elif abs(fullmax - fullmin) < avgmax*0.3 or True:
            if all( [ abs(h.GetBinContent(i)-avgmaxR)<avgmaxR*0.3 for i in rrange ] ):
                print 'LegendFinder: Found histogram that evenly fills right half of the canvas:'
                return 0.55,0.65,1
            elif all( [ abs(h.GetBinContent(i)-avgmaxL)<avgmaxL*0.3 for i in lrange ] ):
                print 'LegendFinder: Found histogram that evenly fills left half of the canvas:'
                return 0.12,0.65,1
        print 'LegendFinder: Found histogram that precludes smart placement of the legend'
        return 0.55,None,default_height

    @staticmethod
    def ConfigureText(fractext, text_x1=None, text_y2=None):
        """ A TPaveText, usually under the legend """
        if text_x1==None: text_x1 = SuCanvas.g_legend_x1_ndc
        if text_y2==None: text_y2 = SuCanvas.g_legend_y2_ndc
        text_x2 = text_x1 + SuCanvas.g_legend_width;
        text_y1 = text_y2 - SuCanvas.g_legend_height_per_entry*fractext.GetSize();
        #print 'INFO: TPaveText coordinates: x=[%.2f..%.2f] y=[%.2f..%.2f]'%(text_x1,text_x2,text_y1,text_y2)
        fractext.SetX1NDC(text_x1);
        fractext.SetX2NDC(text_x2);
        fractext.SetY1NDC(text_y1);
        fractext.SetY2NDC(text_y2);
        fractext.SetFillStyle( 0 ) # hollow
        fractext.SetBorderSize( 0 )
        fractext.SetMargin( 0 )
        fractext.SetTextAlign(12);
        fractext.SetTextFont(SuCanvas.g_text_font);
        fractext.SetTextSize(SuCanvas.g_text_size_pave if SuCanvas.g_text_size_pave else SuCanvas.g_smaller_text_size);

    @staticmethod
    def ConfigureLegend(legend, legend_x1=None, legend_y2=None):
        """ Sebastian legend configuration """
        if legend_x1==None: legend_x1 = SuCanvas.g_legend_x1_ndc
        if legend_y2==None: legend_y2 = SuCanvas.g_legend_y2_ndc
        legend_x2 = legend_x1 + SuCanvas.g_legend_width;
        legend_y1 = legend_y2 - SuCanvas.g_legend_height_per_entry*legend.GetNRows();
        #print 'INFO: TLegend coordinates: x=[%.2f..%.2f] y=[%.2f..%.2f]'%(legend_x1,legend_x2,legend_y1,legend_y2)
        legend.SetX1NDC(legend_x1);
        legend.SetX2NDC(legend_x2);
        legend.SetY1NDC(legend_y1);
        legend.SetY2NDC(legend_y2);
        legend.SetTextAlign(12);
        legend.SetMargin(SuCanvas.g_legend_symbol_width/SuCanvas.g_legend_width);
        legend.SetTextFont(SuCanvas.g_text_font);
        legend.SetTextSize(SuCanvas.g_text_size_legend if SuCanvas.g_text_size_legend else SuCanvas.g_smaller_text_size);
        legend.SetFillStyle(0);
        #legend.SetFillColor(10)
        legend.SetBorderSize(0);

    def ConfigureAxisMain(s,stack,noratio=False):
        """ Configures the axis either for a stack or a histogram in the main pad """
        logy = s._mlogy
        if stack.InheritsFrom('THStack'):
            stack.SetMinimum(1 if logy else 0) # this was gone Nov22 - Dec20
        obj = stack.GetHistogram() if stack.InheritsFrom('THStack') else stack
        obj.GetXaxis().SetLabelFont(SuCanvas.g_text_font)
        obj.GetXaxis().SetLabelSize(SuCanvas.g_text_size)
        obj.GetXaxis().SetTitleFont(SuCanvas.g_text_font)
        obj.GetXaxis().SetTitleSize(SuCanvas.g_text_size)
        if noratio==False:
            obj.GetXaxis().SetLabelSize(0);
        stack.GetYaxis().SetLabelFont(SuCanvas.g_text_font)
        stack.GetYaxis().SetLabelSize(SuCanvas.g_text_size)
        stack.GetYaxis().SetTitleFont(SuCanvas.g_text_font)
        stack.GetYaxis().SetTitleSize(SuCanvas.g_text_size)
        stack.GetYaxis().SetTitleOffset(SuCanvas.g_log_main_y_title_offset if logy else SuCanvas.g_lin_main_y_title_offset);
        # modify divisions
        #stack.GetYaxis().SetNdivisions(6,0,0);
        # avoid ticks covered by filled histograms
        if noratio==False:
            s._plotPad.RedrawAxis('g');
        else:
            s._canvas.RedrawAxis('g');

    def ConfigureAxisRatio(s,h_ratio=None):
        """ Configures the axis either for a histogram in the ratio pad """
        if not h_ratio: return
        logy = s._rlogy
        h_ratio.GetYaxis().SetLabelFont(SuCanvas.g_text_font)
        h_ratio.GetYaxis().SetLabelSize(SuCanvas.g_text_size)
        h_ratio.GetYaxis().SetTitleFont(SuCanvas.g_text_font)
        h_ratio.GetYaxis().SetTitleSize(SuCanvas.g_text_size)
        h_ratio.GetXaxis().SetLabelFont(SuCanvas.g_text_font)
        h_ratio.GetXaxis().SetLabelSize(SuCanvas.g_text_size)
        h_ratio.GetXaxis().SetTitleFont(SuCanvas.g_text_font)
        h_ratio.GetXaxis().SetTitleSize(SuCanvas.g_text_size)
        h_ratio.GetYaxis().CenterTitle(ROOT.kTRUE);
        h_ratio.GetYaxis().SetTitleOffset(SuCanvas.g_log_ratio_y_title_offset if logy else SuCanvas.g_lin_ratio_y_title_offset);
        h_ratio.GetXaxis().SetTitleOffset(SuCanvas.g_ratio_x_title_offset);
        # the factor is a guess
        h_ratio.GetXaxis().SetTickLength(h_ratio.GetXaxis().GetTickLength()*2.1);
        # modify divisions
        if abs(h_ratio.range[0] - 0.9)<0.01 and abs(h_ratio.range[1]- 1.12)<0.01:
            h_ratio.GetYaxis().SetNdivisions(8,5,0)
        else:
            h_ratio.GetYaxis().SetNdivisions(10,5,0);
        # avoid ticks covered by filled histograms
        s._plotPad.RedrawAxis('g');
        s._ratioPad.RedrawAxis('g');

    def ConfigureAxis(s,stack, h_ratio=None):
        noratio = (h_ratio==None)
        s.ConfigureAxisMain(stack,noratio)
        s.ConfigureAxisRatio(h_ratio)

    @staticmethod
    def ControlPlotStyle():
        font=SuCanvas.g_text_font
        tsize=SuCanvas.g_text_size
        marker_size = SuCanvas.g_marker_size
        cgStyle = ROOT.TStyle("ControlPlot","ControlPlot style");
        # use plain black on white colors
        icol=0; # WHITE
        cgStyle.SetFrameBorderMode(icol);
        cgStyle.SetFrameFillColor(icol);
        cgStyle.SetCanvasBorderMode(icol);
        cgStyle.SetCanvasColor(icol);
        cgStyle.SetPadBorderMode(icol);
        cgStyle.SetPadColor(icol);
        cgStyle.SetStatColor(icol);
        #cgStyle.SetFillColor(icol); # don't use: white fill color for *all* objects
        # set the paper & margin sizes
        cgStyle.SetPaperSize(20,26);
        # set margin sizes
        cgStyle.SetPadTopMargin(0.05);
        cgStyle.SetPadRightMargin(0.05);
        cgStyle.SetPadBottomMargin(0.16);
        cgStyle.SetPadLeftMargin(0.16);
        # set title offsets (for axis label)
        cgStyle.SetTitleXOffset(1.4);
        cgStyle.SetTitleYOffset(1.4);
        # use large fonts
        #Int_t font=72; # Helvetica italics
        #Int_t font=42; # Helvetica
        #Double_t tsize=0.05;
        cgStyle.SetTextFont(font);
        cgStyle.SetTextSize(tsize);
        cgStyle.SetLabelFont(font,"x");
        cgStyle.SetTitleFont(font,"x");
        cgStyle.SetLabelFont(font,"y");
        cgStyle.SetTitleFont(font,"y");
        cgStyle.SetLabelFont(font,"z");
        cgStyle.SetTitleFont(font,"z");
        cgStyle.SetLabelSize(tsize,"x");
        cgStyle.SetTitleSize(tsize,"x");
        cgStyle.SetLabelSize(tsize,"y");
        cgStyle.SetTitleSize(tsize,"y");
        cgStyle.SetLabelSize(tsize,"z");
        cgStyle.SetTitleSize(tsize,"z");
        # use bold lines and markers
        cgStyle.SetMarkerStyle(20);
        cgStyle.SetMarkerSize(marker_size);
        cgStyle.SetHistLineWidth(2);
        cgStyle.SetLineStyleString(2,"[12 12]"); # postscript dashes
        # get rid of X error bars
        #cgStyle.SetErrorX(0.001);
        # get rid of error bar caps
        cgStyle.SetEndErrorSize(0.);
        # do not display any of the standard histogram decorations
        cgStyle.SetOptTitle(0);
        #cgStyle.SetOptStat(1111);
        cgStyle.SetOptStat(0);
        #cgStyle.SetOptFit(1111);
        cgStyle.SetOptFit(0);
        # put tick marks on top and RHS of plots
        cgStyle.SetPadTickX(1);
        cgStyle.SetPadTickY(1);
        ROOT.gROOT.SetStyle("ControlPlot");
        ROOT.gROOT.ForceStyle();
        return cgStyle;

    @staticmethod
    def set_uncert_style(obj,color=None):
        """ set style for uncertainties """
        if not obj: return
        obj.SetFillColor( color );
        obj.SetMarkerSize( 0 );
        obj.SetLineWidth( 0 );
        return
        obj.SetFillStyle( 3001 );
        if False:
            obj.SetLineColor( color );
            obj.SetMarkerColor( color );
        obj.SetLineStyle( 2 );
        obj.SetLineWidth( 2 );

    def drawLuminosity(s,x,y):
        lumi = ROOT.TString("4.6 fb^{-1}")
        prefix=ROOT.TString(r"#lower[-0.2]{#scale[0.6]{#int}}Ldt = ")
        s.tex = tex = ROOT.TLatex(x, y, (prefix+lumi).Data());
        tex.SetNDC();
        tex.SetTextFont(SuCanvas.g_text_font);
        tex.SetTextSize(SuCanvas.g_smaller_text_size);
        tex.SetLineWidth(0);
        tex.Draw();

    def drawRatio(s,hratio,same=False,yrange=None):
        if same:
            hratio.Draw("pe A SAME")
        else:
            hratio.Draw("pe")
            hratio.GetYaxis().SetNdivisions(6,0,0)
            if yrange:
                assert type(yrange)==type([]) and len(yrange)==2, 'ERROR: drawRatio yrange must be a list [min,max]'
                hratio.GetYaxis().SetRangeUser(yrange[0],yrange[1])
                hratio.range = (yrange[0],yrange[1])
            else:
                hratio.GetYaxis().SetRangeUser(s._refLineMin,s._refLineMax)
                hratio.range = (s._refLineMin,s._refLineMax)
            hratio.GetYaxis().SetTitle(s._ratioName)

    def drawUBand(s):
        """ Draw statistical and systematic uncertainty bands """
        cur = ROOT.gDirectory;
        s._ratioPad.cd();

        xmin = 0.1;
        width = 0.2;
        xmax = xmin + width;

        ymin = 0.1;
        height = 0.1;
        ymax = ymin + height;

        offset = 1.2*width;

        align = 11;

        s.band1 = band1 = ROOT.TLegend(xmin, ymin, xmax, ymax);
        band1.SetFillColor(0);
        band1.SetFillStyle(0);
        band1.SetBorderSize(0);
        band1.SetLineColor(0);
        band1.SetTextFont(s.g_text_font);
        band1.SetTextSize(s.g_smaller_text_size);
        s.bandh1 = h1 = ROOT.TH1F("h1_"+s.namebase(),"",1,0,1);
        h1.SetFillColor(ROOT.kYellow);
        h1.SetLineColor(ROOT.kYellow);
        band1.AddEntry(h1," MC Stat. Unc.", "F").SetTextAlign(align);
        band1.Draw();
        s.band2 = band2 = ROOT.TLegend(xmin+offset, ymin, xmax+offset, ymax);
        band2.SetFillColor(0);
        band2.SetFillStyle(0);
        band2.SetBorderSize(0);
        band2.SetLineColor(0);
        band2.SetTextFont(s.g_text_font);
        band2.SetTextSize(s.g_smaller_text_size);
        s.bandh2 = h2 = ROOT.TH1F("h2_"+s.namebase(),"",1,0,1);
        h2.SetFillColor(ROOT.kGreen);
        h2.SetLineColor(ROOT.kGreen);
        band2.AddEntry(h2," MC Stat. #oplus Syst. Unc.", "F").SetTextAlign(align);
        band2.Draw();
        cur.cd();
        
    def drawRatioHash(s,ratio,hash=True,mstyle=2):
        """ Draws a ratio histogram """
        ratio.SetFillColor( ROOT.kBlack );
        ratio.SetFillStyle( 3154 );
        ratio.SetMarkerStyle( mstyle );
        if hash:
            ratio.Draw("AP E2 same");
        else:
            ratio.Draw("AP same");

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
                   
    def buildJordanDefault(s,width=600,height=600,title=None):
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

    def buildJordanRatio(s,width=600,height=600,title=None):
        """ old version by Jordan """
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
        # constants
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
 
    def buildDefault(s,width=600,height=600,title=None,margin=0.05,mlogy=False):
        """ Sebastian version """
        if not title:
            title = rand_name()
        s._mlogy = mlogy
        s._canvas = ROOT.TCanvas( 'canvasd'+title , 'canvasd'+title , width , height );
        main_frac = 1.0
        left_bottom_margin = 3*margin;
        # since default canvas has no ratio pad, we need a bottom margin
        #s._canvas.SetBottomMargin(0.02);
        s._canvas.SetRightMargin(margin);
        s._canvas.SetLeftMargin(left_bottom_margin);
        s._canvas.SetTopMargin(margin/main_frac);
        s._canvas.cd();
        if mlogy:
            s._canvas.SetLogy(mlogy)

    def buildRatio(s,width=600,height=600,title=None,margin=0.05,mlogy=False,rlogy=False):
        """ Sebastian version """
        if not title:
            title = rand_name()
        s._mlogy = mlogy
        s._rlogy = rlogy
        ratio_frac = SuCanvas.g_ratio_frac
        main_frac = 1-ratio_frac;
        s._canvas = ROOT.TCanvas( 'canvasm'+title , 'canvasm'+title , width , height );
        s._plotPad = ROOT.TPad( title+"plotPad" , title+"plotPad" , 0 , ratio_frac , 1 , 1 );
        s._ratioPad = ROOT.TPad( title+"ratioPad" , title+"ratioPad" , 0 , 0 , 1 , ratio_frac );
        s._coverPad = None
        left_bottom_margin = 2.1*margin;
        # no gap between main and ratio pad
        s._plotPad.SetBottomMargin(0.02);
        s._ratioPad.SetTopMargin(0);
        s._plotPad.SetRightMargin(margin);
        s._ratioPad.SetRightMargin(margin);
        s._plotPad.SetLeftMargin(left_bottom_margin);
        s._ratioPad.SetLeftMargin(left_bottom_margin);
        s._plotPad.SetTopMargin(margin/main_frac);
        s._ratioPad.SetBottomMargin(left_bottom_margin/ratio_frac);
        s._ratioPad.SetGrid(0,2);
        s._canvas.cd();
        s._plotPad.Draw();
        s._canvas.cd();
        s._ratioPad.Draw();
        s._plotPad.cd();
        if mlogy:
            s._plotPad.SetLogy(mlogy)
            
    @staticmethod
    def cleanse(name2):
        return re.sub(r'[^\w]', '_', name2)
    def SaveSelf(s):
        """ auto-save version """
        assert s.savedir
        assert s.savename
        assert type(s.savetypes)==type([]) and len(s.savetypes)>0
        savetag = s.savetag+'_' if s.savetag!='' else ''
        return s.SaveAs(savetag+s.savename,s.savetypes,s.savedir)
    def SaveAs(s,name2,exts,DIR='./',verbose=True):
        name = s.cleanse(name2)
        c = s._canvas
        print 'SuCanvas: saving plot %s.%s'%(name,exts[0])
        [ c.SaveAs(DIR+'%s.%s'%(name,ext)) for ext in exts ]

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

    # get vars
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

    def plotAny(s,hplots,M=None,height=1.5,mode=0,
                rebin=1.0,drawopt='',
                xaxis_info=None,
                mlogy=False,rlogy=False,
                pave=None):
        """ A generic function to plot several SuPlot's.
        M is: a PlotOptions object describing formatting and colors
        Height is: Max*height. Or asym = 0..0.5
        mode=0 - nominal only
        mode=1 - apply systematics
        """
        if M:
            assert M.ntot()==len(hplots),'Size mismatch between SuPlots(%d) And PlotOptions(%d)'%(M.ntot(),len(hplots))
        leg = ROOT.TLegend()
        if M and M.title!=None:
            leg.SetHeader(M.title)
        s.data.append( (hplots,leg) )
        if M and M.any_ratios():
            s.buildRatio(mlogy=mlogy,rlogy=rlogy)
            s.cd_plotPad()
        else:
            s.buildDefault(mlogy=mlogy)
            s.cd_canvas();
        # plot actual histograms
        hs = []
        hdraw = None
        for i,hplot in enumerate(hplots):
            if hasattr(hplot,'nominal'):
                h0 = hplot.nominal_stack(rebin).GetStack().Last() if hplot.nominal().stack else hplot.nominal_h(rebin)
            else:
                h0 = hplot.Clone(hplot.GetName()+'_reb')
                h0.has_systematics = lambda : False
                if rebin!=1.0:
                    h0.Rebin(int(rebin))
            htot = None
            assert h0
            # total error (including systematics)
            if M and M.errors[i] and hplot.has_systematics():
                ht = hplot.clone()
                htot = ht.update_errors(rebin=rebin)
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
                    #h.SetLineColor( PlotOptions.autocolor(i) )
                    if i==0:
                        if drawopt=='HIST CP E0':
                            h.Draw('HIST CP')
                            s.data.append( h.DrawClone('A SAME CP')  )
                        else:
                            h.Draw(drawopt)
                        hdraw = h
                    else:
                        if drawopt=='HIST CP E0':
                            h.Draw('A SAME HIST CP')
                            s.data.append( h.DrawClone('A SAME CP')  )
                        else:
                            h.Draw('A SAME '+drawopt)
            if htot:
                htot.Draw('A same E2')
        # axis labeling
        xaxis_label = None
        if xaxis_info!=None:
            assert len(xaxis_info)>=2,'Incorrect format of xaxis_info: [xaxis_label , xaxis_units , ...]'
            xaxis_label,xaxis_units = xaxis_info[0],xaxis_info[1]
            if xaxis_label:
                hdraw.GetXaxis().SetTitle( xaxis_label )
            if xaxis_units==None:
                hdraw.GetYaxis().SetTitle( "Entries" )
            else:
                bin_width = hdraw.GetXaxis().GetBinWidth(1)
                hdraw.GetYaxis().SetTitle( "Entries / %.1f %s" % (bin_width,xaxis_units) )
                xaxis_label += ' [%s]'%xaxis_units
            if len(xaxis_info)>=4: # y axis information
                if xaxis_info[3]==None:
                    hdraw.GetYaxis().SetTitle( xaxis_info[2] )
                else:
                    hdraw.GetYaxis().SetTitle( xaxis_info[2] + ' [%s]'%xaxis_info[3])
        # axis ranges
        maxh = max([h.GetMaximum() for h in hs])
        if type(height)==type([]) and len(height)==2:
            hs[0].GetYaxis().SetRangeUser(height[0],height[1]);
        elif type(height)==type([]) and len(height)>2:
            # array of bounds
            inrange = [ v for v in height[1:] if v >= maxh*2.3 ]
            hmax = maxh*2.3
            if len(inrange)>0:
                hmax = inrange[0]
            hs[0].GetYaxis().SetRangeUser(height[0],hmax);
        elif height=='asym':
            hs[0].GetYaxis().SetRangeUser(0,0.5);
        elif height=='ratio':
            hs[0].GetYaxis().SetRangeUser(0,2.0);
        elif type(height)==type('abs') and height[0:3]=='abs':
            hmax = float(height[3:])
            hs[0].GetYaxis().SetRangeUser(0,hmax);
        elif height<3.0: # relative scale factor
            hs[0].GetYaxis().SetRangeUser(0,maxh*float(height));
        else:            # absolute scale
            hs[0].GetYaxis().SetRangeUser(0,height);
        # legend
        s.ConfigureLegend(leg)
        leg.Draw("same")
        if pave:
            assert type(pave)==type([]),'ERROR: pave must be given as a python list of lines'
            s.pavetext = pavetext = ROOT.TPaveText()
            for ltext in pave:
                pavetext.AddText( ltext )
            s.ConfigureText(pavetext,text_x1 = leg.GetX2NDC()+0.2)
            pavetext.Draw()
        # draw ratios with respect to the first histogram
        hdrawratio = None
        if M and M.any_ratios():
            s.cd_ratioPad();
            for i,h in enumerate(hs):
                print 'Working on:',i,h.GetName()
                hratio,href = h.Clone("hratio"),h.Clone("href")
                hratio.Divide(hs[0])
                s.data.append( (hratio,href) )
                if not M.ratios[i]: continue
                #[hratio.SetBinError(ii,0) for ii in xrange(0,hratio.GetNbinsX()+2)]
                s.drawRatio(hratio , hdrawratio!=None )
                if not hdrawratio:
                    hdrawratio = hratio
                    if xaxis_label:
                        hratio.GetXaxis().SetTitle( xaxis_label )
        s.ConfigureAxis(hdraw, hdrawratio)
        s.update()

    def plotStack(s,hstack,hdata,leg=None,height=1.5,mode=0,pave=True,
                  rebin=1.0,norm=False,norm_sys_from_nominal=False,
                  xaxis_info=None,
                  leg_x1=None,leg_y2=None,
                  lumi_x=None,lumi_y=None,
                  mlogy=False,rlogy=False):
        """ Wrapper to make a complete plot of stack and data overlayed - SuData version
        if norm==True, the stack is rescaled to Nominal data counts
        if norm_sys_from_nominal=True,  systematics normalized to nominal (rather than to data)
        mode=0 - nominal only
        mode=1 - nominal + systematic bands
        """
        s.buildRatio(mlogy=mlogy,rlogy=rlogy);
        s.cd_plotPad();
        data = hdata.nominal_h(rebin)
        stack , stackScale , stackOrder =  hstack.nominal().get_stack(rebin, data if norm==True else None,order=1)
        stackH = stack.GetStack().Last()
        # create the legend from the legend specifier
        if leg!=None:
            alleg = leg[:]
            daleg = [l for l in leg if l[0]=='data']
            if len(daleg)==0:
                daleg.append( ['data','data 2011 (#sqrt{s} = 7 TeV)','LP'] )
            mcleg = [l for l in leg if l[0]!='data']
            leg = ROOT.TLegend()
            leg.AddEntry(data,daleg[0][1],daleg[0][2])
            NBG = stack.GetHists().GetSize()
            if NBG!=len(mcleg) or NBG!=len(stackOrder):
                print 'Stack size:',NBG
                print 'Legend list:',len(alleg),alleg
                print 'MC legend list:',len(mcleg),mcleg
                print 'stackOrder list:',len(stackOrder),stackOrder
            assert NBG == len(mcleg) == len(stackOrder), 'Different number of elements in stack and legend label array'
            for ii in reversed(xrange(0,NBG)):
                zi = stackOrder.index(ii)
                htmp = stack.GetHists().At(zi)
                leg.AddEntry(htmp,mcleg[ii][1],mcleg[ii][2])
        s.data.append((stack,data,leg))
        # mc
        stack.Draw() # "HIST"
        xaxis_label = None
        xaxis_range = None
        if xaxis_info!=None:
            assert len(xaxis_info)>=2,'Incorrect format of xaxis_info: [xaxis_label , xaxis_units , ...]'
            xaxis_label,xaxis_units = xaxis_info[0],xaxis_info[1]
            if xaxis_units==None:
                stack.GetYaxis().SetTitle( "Entries" )
            else:
                bin_width = stack.GetHistogram().GetXaxis().GetBinWidth(1)
                stack.GetYaxis().SetTitle( "Entries / %.1f %s" % (bin_width,xaxis_units) )
                xaxis_label += ' [%s]'%xaxis_units
            if len(xaxis_info)>2:
                xaxis_range = xaxis_info[2]
                assert len(xaxis_range)==2
                # convert to bin range
                xaxis_range = [ stack.GetHistogram().FindFixBin(xaxis_range[0]) , stack.GetHistogram().FindFixBin(xaxis_range[1]) ]
        if xaxis_range!=None:
            stack.GetXaxis().SetRange(*xaxis_range)
        # systematics and special variations
        HDATA = [ data.Clone(), ] # no-error
        [HDATA[0].SetBinError(ii,0) for ii in xrange(0,HDATA[0].GetNbinsX()+2)]
        HMC = [stackH.Clone(),None,None]    # no-error , MC stat error , MC tot error
        [HMC[0].SetBinError(ii,0) for ii in xrange(0,HMC[0].GetNbinsX()+2)]
        if mode==1 and hstack.has_systematics():
            if norm_sys_from_nominal:     # systematics normalized to nominal scale factor
                s.hsys = HMC[1] = hstack.update_errors(sysonly=True,rebin=rebin,scale=stackScale)
                s.htot = HMC[2] = hstack.update_errors(rebin=rebin,scale=stackScale)
            else:       # systematics normalized to data every time (i.e., only consider shape differences)
                if norm:
                    dint = data.Integral()
                    s.hsys = HMC[1] = hstack.update_errors(sysonly=True,rebin=rebin,scale=dint,renorm=True)
                    s.htot = HMC[2] = hstack.update_errors(rebin=rebin,scale=dint,renorm=True)
                else:
                    assert False,'Why are you trying to run this?'
                    s.hsys = HMC[1] = hstack.update_errors(sysonly=True,rebin=rebin,scale=1)
                    s.htot = HMC[2] = hstack.update_errors(rebin=rebin,scale=1)
        s.data.append((HDATA,HMC))
        # data
        data.Draw("pe same")
        # configure legend and QCD fraction pavebox
        if leg:
            s.cd_plotPad();
            s.ConfigureLegend(leg,leg_x1,leg_y2)
            leg.Draw()
            qcdfrac = hstack.nominal().stack_bg_frac()
            if pave and qcdfrac!=None:
                s.fractext = fractext = ROOT.TPaveText()
                try:
                    staterr = hstack.nominal().scales[3]/hstack.nominal().scales[2]
                except:
                    staterr = 0
                fractext.AddText( 'QCD Frac. = %.3f #pm %.2f%%'%(qcdfrac,staterr*100.0) )
                s.ConfigureText(fractext,text_x1=leg_x1,text_y2 = leg.GetY1NDC()-0.02)
                fractext.Draw()
        # configure legend
        if not lumi_x: lumi_x = leg.GetX1NDC() + 0.2
        if not lumi_y: lumi_y = leg.GetY1NDC() + 0.02 + 0.15 + 0.03
        s.drawLuminosity(lumi_x,lumi_y)
        # ratio
        s.cd_ratioPad();
        hratio = s.hratio = data.Clone("hratio")
        if mode==0:
            hratio.Divide(stackH) # stat error data/MC combined
        else:
            hratio.Divide(HMC[0]) # stat data error only
        s.drawRatio(hratio)
        if xaxis_label:
            hratio.GetXaxis().SetTitle( xaxis_label )
        if mode==1 and hstack.has_systematics():
            # stat MC error only
            s.hyellow = hyellow = HDATA[0].Clone("hratio_yellow")
            hyellow.Divide(stackH)
            [hyellow.SetBinContent(ii,1.0) for ii in xrange(0,hyellow.GetNbinsX()+2)]
            s.set_uncert_style(hyellow,color=ROOT.kYellow)
            # stat MC error + systematics
            s.hgreen = hgreen = HDATA[0].Clone("hratio_green")
            hgreen.Divide(HMC[2])
            [hgreen.SetBinContent(ii,1.0) for ii in xrange(0,hgreen.GetNbinsX()+2)]
            s.set_uncert_style(hgreen,color=ROOT.kGreen)
            # draw
            hgreen.Draw('A SAME E2')
            hyellow.Draw('A SAME E2')
            s.drawRatio(hratio,same=True)
            s.data.append( (hyellow,hgreen) )
            # draw systematic legend
            s.drawUBand()
        # axis parameters
        logy = s._mlogy
        stack.SetMaximum(stack.GetMaximum()*10*height if logy else stack.GetMaximum()*height)
        if xaxis_range!=None:
            hratio.GetXaxis().SetRange(*xaxis_range)
        s.ConfigureAxis(stack, hratio)
        # fin
        s.update()

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

class PlotOptions:
    """ Handles colors and marker styles for various MC generators """
    msize = 1.5
    def __init__(s,title=None):
      s.n = 0
      s.title = title
      s.names = []
      s.labels = []
      s.colors = []
      s.sizes = []
      s.styles = []
      s.cuts = []
      s.ratios = []
      s.errors = []
      s.icolor = 0
      s.istyle = 0
    def ntot(s):
      assert s.n == len(s.cuts)
      return s.n
    def any_ratios(s):
      return any(s.ratios)
    def disable_ratios(s):
      s.ratios = [False]*len(s.ratios)
    def ad(s,label,color=None,size=0.7,style=None,cut=None,r=None,err=0):
      """ Add one sample.
      NOTE: previously, this had the following default: style=20 """
      return s.add(label,label,color,size,style,cut,r,err)
    def add(s,name,label,color=None,size=0.7,style=None,cut=None,r=None,err=0):
      """ Add one sample.
      NOTE: previously, this had the following default: style=20 """
      s.names.append(name)
      s.labels.append(label)
      s.colors.append( color if color else s.autocolor(s.icolor) )
      s.sizes.append(size*s.msize)
      s.styles.append( style if style else s.autostyle(s.istyle) )
      s.cuts.append(cut)
      s.ratios.append(r)
      s.errors.append(err)
      if color==None: s.icolor+=1
      if style==None: s.istyle+=1
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
      colorlist = [2,8,4,ROOT.kOrange,6,20,28,41,46]
      colorlist+= [2,8,4,ROOT.kOrange,6,20,28,41,46]
      colorlist+= [2,8,4,ROOT.kOrange,6,20,28,41,46]
      return colorlist[i] if i<len(colorlist) else (1 if i==11 else i)
    @staticmethod
    def autostyle(i):
      """ choose a reasonable sequence of styles """
      #style = [20,21,22,23,33,34]
      style = range(24,28+1) + [30,31,32] + [21,22,23]
      style += style
      style += style
      return style[i] if i<len(style) else (1 if i==11 else i)
