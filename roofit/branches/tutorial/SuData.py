#/usr/bin/env python

"""
A generalized class for histogram/ntuple data access
"""

import sys,os,math,re,glob
from hashlib import md5
import ROOT
from FileLock import FileLock

# this is used to select default unfolding matrix
MAP_BGSIG = {0:'pythia',1:'mcnlo',2:'alpgen_herwig',3:'alpgen_pythia',4:'powheg_herwig',5:'powheg_pythia'}
# this is used to select specific monte-carlos for signal/qcd/etc
MAP_BGQCD = {0:'mc',1:'bb',2:'JX',3:'driven',4:'driven_sub',10:'mc_driven'}
# this is used to do generator systematics
MCMAP = {}
MCMAP['mc_powheg_pythia_wminmunu'] = { 1 : 'mc_mcnlo_wminmunu' , 4 : 'mc_powheg_herwig_wminmunu' , 5 : 'mc_powheg_pythia_wminmunu' }
MCMAP['mc_powheg_pythia_wplusmunu'] = { 1 : 'mc_mcnlo_wplusmunu' , 4 : 'mc_powheg_herwig_wplusmunu' , 5 : 'mc_powheg_pythia_wplusmunu' }
MCMAP['mc_powheg_herwig_wminmunu'] = { 1 : 'mc_mcnlo_wminmunu' , 4 : 'mc_powheg_herwig_wminmunu'  , 5 : 'mc_powheg_pythia_wminmunu' }
MCMAP['mc_powheg_herwig_wplusmunu'] = { 1 : 'mc_mcnlo_wplusmunu' , 4 : 'mc_powheg_herwig_wplusmunu' , 5 : 'mc_powheg_pythia_wplusmunu' }
MCMAP['mc_mcnlo_wminmunu'] = { 1 : 'mc_mcnlo_wminmunu' , 4 : 'mc_powheg_herwig_wminmunu' , 5 : 'mc_powheg_pythia_wminmunu' }
MCMAP['mc_mcnlo_wplusmunu'] = { 1 : 'mc_mcnlo_wplusmunu' , 4 : 'mc_powheg_herwig_wplusmunu' , 5 : 'mc_powheg_pythia_wplusmunu' }

class SuSys:
    """ Generic class that describes where to get the data for one histogram (systematic).
    Basically, it generalizes ntuple-based and histo-based data access.
    def stack(s,hname,var,bin,cut,path=None,flags=['mc'],leg=None):
    def stack2(s,sysdir,subdir,basedir,charge,hname,qcd={},flags=['mc'],leg=None,unfold=None):
    """
    QMAP = { 0 : ('/POS','POS','(l_q>0)','mu+ only','W^{+}'), 1 : ('/NEG','NEG','(l_q<0)','mu- only','W^{-}'), 2 : ('','ALL','(l_q!=0)','mu+ and mu-','W') , -1 : ('','None','1.0','','') }
    def qlist(s,v):
        """ Assign a triplet of directory names: data,MC,QCD """
        if isinstance(v,list) or isinstance(v,tuple):
            assert len(v)==3
            return v
        else:
            return [v]*3
    def __init__(s,name='Nominal',charge=2,qcd = {}, unfold={},qcderr='NOM',slice=None,
                 ntuple=None,path=None,var=None,bin=None,pre='',weight="mcw*puw*effw*trigw",
                 histo=None,sliced_1d=False,sliced_2d=False,sysdir=None,subdir=None,basedir=None,
                 bgsig=None):
        # actual histograms
        s.h = None
        s.stack = None
        # other post-filled resources: symlink to fit results
        s.scales = []
        s.fits   = []
        s.fitnames = []
        # Common resources:
        s.name = name
        s.charge = charge
        s.unfold = unfold
        s.qcd = qcd       # special map to place ourselves into a QCD-region (e.g, anti-isolation)
        s.qcderr = qcderr # controls QCD scale uncertainty: 'NOM','UP','DOWN',<float> - scale factor on nominal (e.g, 0.5 for 50%)
        s.slice = slice   # if not None, this says this SuSys refers to a particular |eta| slice
        # Experimental common resources
        s.bgsig = bgsig
        # Ntuple-based resources:
        s.ntuple = ntuple # w or z
        s.path = path
        s.var = var
        s.bin = bin
        s.pre = s.qlist(pre)
        s.weight = weight
        # Histogram-based resources
        s.histo = histo
        s.sliced_1d = sliced_1d  # from 1D histograms in eta (|eta|) slices
        s.sliced_2d = sliced_2d  # from 2D eta-pt histograms
        # these path elements are organized into lists with: [ /in/data , /in/mc , /in/isofail ]
        s.sysdir = s.qlist(sysdir)
        s.subdir = s.qlist(subdir)
        s.basedir = s.qlist(basedir)
    def eta_to_AC(s):
        """ converts an eta histogram to A/C ratio """
        assert s.h
        h = SuSample.make_habseta(s.h.GetName()+'_ACratio')
        h.Sumw2()
        assert h.GetNbinsX()*2 == s.h.GetNbinsX(),'Bin size mismatch: %d %d %d'%(h.GetNbinsX(),h.GetNbinsX()*2+1,s.h.GetNbinsX())
        hA,hC = h.Clone(h.GetName()+'_A'),h.Clone(h.GetName()+'_C')
        for ibin in xrange(1,h.GetNbinsX()+1): # loop over abseta bins
            ibinC = h.GetNbinsX() + 1 - ibin
            ibinA = h.GetNbinsX() + ibin
            #print ibin,h.GetNbinsX(),':',ibinA,ibinC
            hC.SetBinContent(ibin,s.h.GetBinContent(ibinC))
            hC.SetBinError(ibin,s.h.GetBinError(ibinC))
            hA.SetBinContent(ibin,s.h.GetBinContent(ibinA))
            hA.SetBinError(ibin,s.h.GetBinError(ibinA))
        h.Divide(hA,hC,1,1)
        s.h = h
        return h
    def gpre(s,i=None):
        """ a wrap around s.pre that searches and substitutes special shortcuts
        (hardcoded here) """
        SMAP = []
        SMAP.append(['IsoWind20m','ptiso20/l_pt>=0.12 && ptiso20/l_pt<=0.25'])
        SMAP.append(['IsoNominal','ptiso40/l_pt<0.1'])
        SMAP.append(['IsoFail20','ptiso20/l_pt>=0.1'])
        SMAP.append(['IsoFail40','ptiso40/l_pt>=0.1'])
        SMAP.append(['IsoWind20','ptiso20/l_pt>=0.1 && ptiso20/l_pt<=0.2'])
        SMAP.append(['IsoWind40','ptiso40/l_pt>=0.1 && ptiso40/l_pt<=0.2'])
        import binning
        SMAP.append(['ZETASAME',binning.zpre_samebin_eta()])
        def dosub(x):
            for V in SMAP:
                x = re.sub(V[0],V[1],x)
            return x
        assert len(s.pre)==3
        res = [dosub(pre) for pre in s.pre]
        if i!=None:
            assert i in (0,1,2)
            return res[i]
        return res
    def get_h(s,rebin=1.0):
        """ histogram accessor with rebinning option """
        if rebin==1.0:
            return s.h.Clone()
        else:
            h = s.h.Clone()
            h.Rebin(rebin)
            return h
    def get_stack(s,rebin=1,normto=None,order=None):
        """ stack accessor with rebinning/rescaling option
        order keyword allows to force a particular reordering of the stack elements [for W/Z inclusive paper]
        """
        if True:
            # an alternative version that makes a new stack
            n = s.stack.GetName()+'_cl'
            h = ROOT.THStack(n,n)
            NBG = s.stack.GetHists().GetSize()
            scale = 1
            if normto: # let's use visible range here
                htot_i = s.stack.GetHists().At(0)
                htot  = htot_i.Clone( htot_i.GetName() + '_totrenorm' )   # TList::At(NBG) = signal
                [ htot.Add( s.stack.GetHists().At(i) ) for i in xrange(1,NBG) ]
                htot.Rebin(rebin)
                normsrc = normto
                if hasattr(normto,'nominal'):
                    normsrc = normto.nominal_h()
                assert htot.GetNbinsX() == normsrc.GetNbinsX(),'Different nbins: %d and %d'%(htot.GetNbinsX(),normsrc.GetNbinsX())
                scale = normsrc.Integral() / htot.Integral()
                print 'get_stacks: rescaling stack',s.name,' with SF=%.3f'%scale
            sorder = range(0,NBG)
            # re-order stacks by area, except for signal (last one)
            if order!=None:
                assert len(sorder)>=2,'ERROR: current algorithm assumes at least two backgrounds in the stack'
                assert order==1,'ERROR: get_stack() only supports order=1 for the moment (found %s)'%(order)
                zint = [ s.stack.GetHists().At(ii).Integral() for ii in sorder ]
                zint[-1] += 1e8  # hack: always force the signal to appear first in the stack
                sorder = sorted(range(len(zint)), key=lambda k: zint[k])
                #print zint
                #print sorder
            for ii in sorder:
                htmp2 = s.stack.GetHists().At(ii)
                htmp = htmp2.Clone(htmp2.GetName()+'_rebin%s'%rebin)
                htmp.Rebin(rebin)
                if scale!=1:
                    htmp.Scale( scale )
                h.Add(htmp,'hist')
            return h,scale,sorder
    def histo_pteta(s):
        """ Converts d3_abseta_lpt_met:x:0:8:y:1:2 to human-readable form
        """
        res = ['','']
        hbase = s.histo
        if not len(hbase.split(':'))==7:
            return res
        import binning
        horig = hbase.split(':')[0]  # d2_abseta_lpt
        haxisA = hbase.split(':')[1]
        haxisB = hbase.split(':')[4]
        assert haxisA!=haxisB,'3D projection: both projection axes are the same'
        assert haxisA in ('x','y','z')
        assert haxisB in ('x','y','z')
        iminA,imaxA = [int(cc) for cc in hbase.split(':')[2:2+2]]
        iminB,imaxB = [int(cc) for cc in hbase.split(':')[5:5+2]]
        # eta
        etabins = binning.absetabins if re.search('abseta',horig) else binning.setabins
        if iminA in (0,1) and imaxA in (-1,len(etabins)-1): res[0] = 'inclusive \eta'
        else: res[0] = '%.2f < %s < %.2f'%(etabins[iminA-1],'|\eta|' if re.search('abseta',horig) else '\eta',etabins[imaxA])
        # pt
        pad = ' '*len('W+ : '+'    ')
        ptbins = binning.ptbins
        if iminB in (0,1) and imaxB in (-1,len(ptbins)-1): res[1] = pad+'p_{T} > 20'
        elif iminB==2 and imaxB==len(ptbins)-1: res[1] = pad+'p_{T} > 25'
        elif iminB==7 and imaxB==7: res[1] = pad+'p_{T} > %d'%(ptbins[iminB-1])
        else: res[1] = pad+'%d < p_{T} < %d'%(ptbins[iminB-1],ptbins[imaxB])
        return res
    def use_ntuple(s):
        """ True or False """
        return s.ntuple!=None
    def nt_prew(s,i=0,flags=None):
        """ Returns a TTree::Draw cut string for w ntuple.  Index i has the following meaning:
        i=0 - data
        i=1 - MC
        i=2 - data-driven QCD template
        """
        assert s.var
        assert s.bin
        assert s.pre
        if flags:
            if 'data' in flags:
                i=0
            elif 'driven' in flags:
                i=2
            else:
                i=1
        if s.ntuple=='w':
            print s.gpre(i)
            return '(%s)*(%s)*(%s)' % (SuSys.QMAP[s.charge][2] , s.gpre(i), s.weight)
        elif s.ntuple=='z':
            return '(%s)*(%s)' % (s.gpre(i), s.weight)
        else:
            assert False,"Unknown ntuple type: %s"%(s.ntuple)
    def nt_prez(s,i=0,flags=None):
        assert False,'Not implemented'
        return '(%s)*(%s)' % ( s.gpre(0), s.weight )
    def h_path_folder(s,i=0,flags=None):
        """ Returns the folder-path to the histogram. Index i has the following meaning:
        i=0 - data
        i=1 - MC
        i=2 - data-driven QCD template
        """
        assert s.histo
        assert s.sysdir
        assert s.subdir
        assert s.basedir
        # determine "i" from MC sample flags
        if flags:
            if 'data' in flags:
                i=0
            elif 'driven' in flags:
                i=2
            else:
                i=1
        #isowind/st_w_final/metfit/bin_0/lpt_0/POS => #isowind/st_w_final/metfit/POS/bin_0/lpt_0
        hpath = os.path.join(s.sysdir[i],s.subdir[i],s.basedir[i]) + SuSys.QMAP[s.charge][0]
        if hpath:
            hpath = re.sub('//','/',hpath) # remove double slashes
        if hpath and hpath[-1]=='/':
            hpath = hpath[:-1]
        if re.search('/bin_',hpath) or re.search('/bine_',hpath):
            helms = hpath.split('/')
            helms2 = helms[:-3] + helms[-1:] + helms[-3:-1]
            hpath = '/'.join(helms2)
        return hpath
    def h_path(s,i=0,flags=None):
        """ Returns the path to the histogram. Index i has the following meaning:
        i=0 - data
        i=1 - MC
        i=2 - data-driven QCD template
        """
        hpath = s.h_path_folder(i,flags)+ '/' + s.histo
        return hpath
    def h_path_fname(s,i=1,flags=None):
        # determine "i" from MC sample flags
        if flags:
            if 'data' in flags:
                i=0
            elif 'driven' in flags:
                i=2
            else:
                i=1
        out = s.sysdir[2]+'__'+s.sysdir[i] + '_' + s.subdir[i] + '_' + s.basedir[i] + '_' + SuSys.QMAP[s.charge][1] + '_' + s.histo
        return re.sub(r'[^\w]', '_', out)
    def unfold_get_path(s,ibin=None,ivar=None):
        path = s.unfold['sysdir'] + SuSys.QMAP[s.charge][0] + '/'
        path += s.unfold['histo']
        return path
        if ibin==None:
            path += 'abseta'
        else:
            assert ivar!=None
            path += 'bin_%d/%s'%(ibin,ivar)
        return path
    def qcd_region(s):
        """ Puts this SuSys in qcd region based on qcd map
        If the original basedir includes a subfolder, we preserve it
        """
        if s.use_ntuple():
            assert 'descr' in s.qcd and s.qcd['pre'] and len(s.qcd['pre'])==3
            s.pre = s.qcd['pre']
            s.update_var( s.qcd['var'] , ','.join([str(s.qcd['nbins']),str(s.qcd['min']),str(s.qcd['max'])]) )
            return
        basedir=''
        if len(s.basedir[2].split('/'))>1:  # nominal/lpt2025/tight
            basedir = '/' + '/'.join( s.basedir[2].split('/')[1:]) # lpt2025/tight
        # disable MET>25 or WMT>40 cut if we are actually fitting in MET/WMT
        var = s.qcd['var'].split(':')[0]
        if False: # playing with extended MET fit region
            basedir = s.qcd['anyfit']+basedir
            s.basedir = [basedir]*3
        elif var[-3:]=='wmt':
            basedir = s.qcd['wmtfit']+basedir   # wmtfit/lpt2025/tight [/POS/absetav_histo]
            s.basedir = [basedir]*3
        elif var[-6:]=='on_met': # lepton-met deltaPHI fit should be done BEFORE WMT cut
            basedir = s.qcd['wmtfit']+basedir   # wmtfit/lpt2025/tight [/POS/absetav_histo]
            s.basedir = [basedir]*3
        elif var[-3:]=='met':
            basedir = s.qcd['metfit']+basedir   # metfit/lpt2025/tight [/POS/absetav_histo]
            s.basedir = [basedir]*3
        s.histo = s.qcd['var']
        return
    def is_sliced(s):
        """ True if it is sliced - either 1d or 2d """
        return s.is_sliced_1d() or s.is_sliced_2d()
    def is_sliced_1d(s):
        """ Returns True if s.histo represents a folder of histograms in eta-slices """
        if s.sliced_1d:
            assert (re.search('bin_',s.histo) if s.histo else True)
        return s.sliced_1d
        #return re.search('bin_',s.histo) if s.histo else False
    def is_sliced_2d(s):
        """ Returns True if s.histo represents a 2d histogram in eta-pt """
        if s.sliced_2d:
            assert (re.search(':x:',s.histo) or re.search(':y:',s.histo) if s.histo else True)
        return s.sliced_2d
        #return re.search('bin_',s.histo) if s.histo else False
    def is_ntuple_etabins(s):
        """ Returns 2 if we are doing a qcd fit in |eta| bins , 1 if in eta bins, and False otherwise """
        if s.use_ntuple() and 'etabins' in s.qcd and s.qcd['etabins']==True:
            return 1
        if s.use_ntuple() and 'absetabins' in s.qcd and s.qcd['absetabins']==True:
            return 2
        return False
    def is_histo_etabins(s):
        """ Returns 2 if we are doing a qcd fit in |eta| bins , 1 if in eta bins, and False otherwise """
        if (not s.use_ntuple()) and 'etabins' in s.qcd and s.qcd['etabins']==True:
            return 1
        if (not s.use_ntuple()) and 'absetabins' in s.qcd and s.qcd['absetabins']==True:
            return 2
        return False
    def clone(s,
              sysdir=None,sysdir_mc=None,sysdir_qcd=None,
              subdir=None,subdir_mc=None,basedir=None,
              qcderr=None,qcdadd=None,name=None,q=None,histo=None,sliced_1d=None,sliced_2d=None,
              unfold=None,unfdir=None,unfhisto=None,
              ntuple=None,path=None,var=None,bin=None,pre=None,weight=None,
              slice=None,bgsig=None):
        """ deep copy, also allowing to update some members on-the-fly (useful to spawn systematics) """
        import copy
        res =  copy.copy(s)
        # do not clone the actual histograms
        #res.h,res.stack = None,None
        # global over-rides for MC/data/QCD folder paths
        if sysdir!=None: res.sysdir = s.qlist(sysdir)
        if subdir!=None: res.subdir = s.qlist(subdir)
        if basedir!=None: res.basedir = s.qlist(basedir)
        # only replace in MC or in QCD anti-isolation
        if sysdir_mc!=None: res.sysdir = s.qlist([res.sysdir[0],sysdir_mc,res.sysdir[2]])
        if sysdir_qcd!=None: res.sysdir = s.qlist([res.sysdir[0],res.sysdir[1],sysdir_qcd])
        if subdir_mc!=None: res.subdir = s.qlist([res.subdir[0],subdir_mc,res.subdir[2]])
        # other replacements
        if qcderr!=None: res.qcderr = qcderr
        if qcdadd!=None: # addition to qcd map for fine-control of QCD fits
            qcd = copy.copy(res.qcd)
            qcd.update(qcdadd)
            res.qcd = qcd
        if name!=None: res.name = name
        if histo!=None: res.histo = histo
        if sliced_1d!=None: res.sliced_1d = sliced_1d
        if sliced_2d!=None: res.sliced_2d = sliced_2d
        if q!=None: res.charge = q
        if unfold!=None:
            res.unfold = copy.copy(unfold)
        else: # always make a copy of unfolding map
            res.unfold = copy.copy(s.unfold)
        if unfdir!=None: res.unfold['sysdir'] = unfdir
        if unfhisto!=None: res.unfold['histo'] = unfhisto
        # ntuple replacements
        if ntuple!=None: res.ntuple = ntuple
        if path!=None: res.path = path
        if var!=None: res.var = var
        if bin!=None: res.bin = bin
        if pre!=None: res.pre = s.qlist(pre)
        if weight!=None: res.weight = weight
        # other
        if slice!=None: res.slice = slice
        if bgsig!=None: res.bgsig = bgsig
        return res
    def stack_bg_frac(s,iback=1,overflow=True):
        """ Returns integral fraction of background NBG-iback (NBG corresponds to wmunu) """
        return s.stack_bg_events(iback,overflow)[0] / s.stack_total_events(overflow)[0]
    def stack_qcd_frac(s,overflow=True):
        return s.stack_qcd_events(overflow)[0] / s.stack_total_events(overflow)[0]
    def stack_ewknosig_frac(s,overflow=True):
        return s.stack_ewknosig_events(overflow)[0] / s.stack_total_events(overflow)[0]
    def stack_total_events(s,overflow=True):
        """ Returns integral of total stack """
        h = s.stack.GetStack().Last()
        err = ROOT.Double()
        totevts = h.IntegralAndError(0,h.GetNbinsX()+1,err) if overflow else h.IntegralAndError(1,h.GetNbinsX(),err)
        return totevts,float(err)
    def histo_total_events(s,overflow=True):
        """ Returns integral of histo """
        h = s.h
        err = ROOT.Double()
        totevts = h.IntegralAndError(0,h.GetNbinsX()+1,err) if overflow else h.IntegralAndError(1,h.GetNbinsX(),err)
        return totevts,float(err)
    def stack_bg_events(s,iback,overflow=True):
        """ Returns integral of background NBG-iback (NBG corresponds to wmunu) """
        NBG = s.stack.GetStack().GetLast()
        h = s.stack.GetHists().At(NBG - iback)   # TList::At(NBG) = signal; At(0) = qcd
        # include overflow? E.g., met>200
        err = ROOT.Double()
        qcdevts = h.IntegralAndError(0,h.GetNbinsX()+1,err) if overflow else h.IntegralAndError(1,h.GetNbinsX(),err)
        return qcdevts,float(err)
    def stack_qcd_events(s,overflow=True):
        NBG = s.stack.GetStack().GetLast()
        return s.stack_bg_events(NBG,overflow)
    def stack_sig_events(s,overflow=True):
        return s.stack_bg_events(0,overflow)
    def stack_ewknosig_events(s,overflow=True):
        """ number of electroweak background events, excluding signal """
        NBG = s.stack.GetStack().GetLast()
        bglist = range(1,NBG)
        pairs = [ s.stack_bg_events(ibg,overflow) for ibg in bglist ]
        # counts: add up each background
        evts = sum([x[0] for x in pairs])
        # error: quadrature sum of each background (assume stat error is independent)
        err = math.sqrt( sum([x[1]*x[1] for x in pairs]) )
        return evts,err
    def update_var(s,histo,bin=None):
        """ Updates histogram name and ntuple-expression (aka variable)
        In reality, only one of these applies for a given SuData instance.
        """
        if bin: #ntuple
            s.var = histo
            s.bin = bin
        else:   #histo
            s.histo = histo
        return True
    def Add(s,o,dd=1.0):
        """ Smart add function that can handle None in either self.h or o.h
        return s.h.Add(o.h,dd) if s.h and o.h else None
        """
        if s.h and not o.h:
            pass
        elif s.h and o.h:
            s.h.Add(o.h,dd)
        elif not s.h and o.h:
            s.h = o.h.Clone()
            if dd!=1.0:
                s.h.Scale(dd)
        elif not s.h and not o.h:
            print 'WARNING: attempting to add two Null histograms',s.name
            pass
        else:
            assert False
        return True
    def AddStack(s,o,opt='hist'):
        return s.stack.Add(o.h,opt) if s.stack and o.h else None
    def SetName(s,n):
        return s.h.SetName(n) if s.h else None
    def SetLineColor(s,c):
        return s.h.SetLineColor(c) if s.h else None
    def SetFillColor(s,c):
        return s.h.SetFillColor(c) if s.h else None
    def SetMarkerSize(s,c):
        return s.h.SetMarkerSize(c) if s.h else None
    def ScaleOne(s,*args,**kwargs):
        """ In SuSys, this is just an alias for Scale() """
        return s.Scale(*args,**kwargs)
    def Scale(s,c):
        """ special treatment: if c is a list, then the scales are to be applied per-bin """
        if isinstance(c,list) or isinstance(c,tuple):
            assert len(c)==s.h.GetNbinsX()+2,'Found scale array of size=%d, but need %d'%(len(c),s.h.GetNbinsX()+2)
            for i,v in enumerate(c):
                s.h.SetBinContent(i, s.h.GetBinContent(i)*v)
                s.h.SetBinError(i, s.h.GetBinError(i)*v)
            return True
        return s.h.Scale(c) if s.h else None
    def Unitize(s):
        return s.h.Scale(1.0/s.h.Integral()) if s.h else None
    def MakeStack(s,n):
        s.stack = ROOT.THStack(n,n)
        return s.stack
            
class SuPlot:
    """ A collection of SuSys objects - a histogram along with all of its systematics.
    Handles final error computations and printing of systematic percent error tables.
    Implements the TH1* interface for easier handling
    """
    def __init__(s):
        s.hsys = None
        s.htot = None
        s.status = 0
        s.groups = []   # flat list of groups - names of systematics
        s.sys = []      # nested list (groups->up/downs)
        s.flat = []     # flat list of all systematics
        s.enable = [0,] # flat list of enabled indices
        s.do_unfold = False
    def has_systematics(s):
        return len(s.enable)>1
    def eta_to_AC(s):
        return [ p1.eta_to_AC() for i,p1 in enumerate(s.flat) if i in s.enable]
    def Add(s,h,dd=1.0):
        return [ p1.Add(p2,dd) for i,(p1,p2) in enumerate(zip(s.flat,h.flat)) if i in s.enable]
    def AddStack(s,h):
        return [ p1.AddStack(p2) for i,(p1,p2) in enumerate(zip(s.flat,h.flat)) if i in s.enable ]
    def SetName(s,name):
        return [ p1.SetName(name+'_'+p1.name) for i,p1 in enumerate(s.flat) if i in s.enable ]
    def SetLineColor(s,c):
        return [ p1.SetLineColor(c) for i,p1 in enumerate(s.flat) if i in s.enable ]
    def SetFillColor(s,c):
        return [ p1.SetFillColor(c) for i,p1 in enumerate(s.flat) if i in s.enable ]
    def SetMarkerSize(s,c):
        return [ p1.SetMarkerSize(c) for i,p1 in enumerate(s.flat) if i in s.enable ]
    def ScaleOne(s,v):
        """ This version scales all systematics with the same scale (or scale array) """
        return [ p1.Scale(v) for i,p1 in enumerate(s.flat) if i in s.enable ]
    def Scale(s,v):
        """ This version has a separate scale (or scale array) for each systematics """
        assert isinstance(v,list) or isinstance(v,tuple)
        j=0
        for i,p1 in enumerate(s.flat):
            if not i in s.enable: continue
            p1.Scale(v[j]) # j is an index for *enabled* systematics only!
            j+=1
        return True
    def Unitize(s):
        return [ p1.Unitize() for i,p1 in enumerate(s.flat) if i in s.enable ]
    def MakeStacks(s,hname):
        return [ p1.MakeStack(hname+'_'+p1.name) for i,p1 in enumerate(s.flat) if i in s.enable ]
    def update_from_slices(s,ds,heta,imin,imax):
        """ Builds a histogram from a collection of 1D histograms (e.g., vs pT) in eta slices
        imin and imax give a range of bins in pT histogram that gets projected on final eta plot
        heta is a dummy abseta histogram that's used to determine binning for the final histogram
        """
        print '--------->', 'update_from_slices: started working on',s.flat[0].histo
        sys.stdout.flush()
        import SuCanvas
        # create final eta histogram for each systematic
        for i,dsys in enumerate(s.flat):
            if not i in s.enable: continue
            hpts = [d.flat[i].h for d in ds] # loop over slices
            assert all(hpts)
            dsys.h = SuCanvas.SuCanvas.from_slices(hpts,heta,imin,imax)
        return s
    def get(s,sysname):
        """ Gets a particular systematic """
        for sgroups in s.sys:
            for sinst in sgroups:
                if sinst.name==sysname:
                    return sinst
        print 'WARNING: cannot find systematic:',sysname,' - will return nominal'
        assert len(s.sys)>0,'Empty s.sys collection - even nominal is absent!'
        return s.sys[0][0]
    def nominal(s):
        return s.flat[0]
    def any_h(s):
        for h in s.flat:
            if h.h:
                return h
        return None
    def nominal_h(s,rebin=1.0):
        if rebin==1.0:
            return s.nominal().h
        else:
            h = s.nominal().h.Clone()
            h.Rebin(rebin)
            return h
    def nominal_h_name(s,exc='_Nominal'):
        """ Returns the name of nominal histogram """
        res = s.nominal_h().GetName()
        return re.sub('_'+s.flat[0].name,'',res)
    def enable_name(s,name):
        return s.enable_names([name,])
    def enable_names(s,names):
        idx = 0
        s.enable = []
        for sgroups in s.sys:
            for sinst in sgroups:
                if sinst.name in names:
                    s.enable.append(idx)
                idx+=1
        if len(s.enable)==0:
            print 'WARNING: could not enable any of these systematics:',names
        else:
            print 'Enabled',len(s.enable),'systematics out of',len(names)
            print names
    def enable_nominal(s):
        s.enable = [0,]
    def enable_some(s,v):
        s.enable = list(v)[:]
        assert max(s.enable) < len(s.flat)
    def enable_all(s):
        s.enable = []
        idx = 0
        for ig,sgroups in enumerate(s.sys):
            for iss,sinst in enumerate(sgroups):
                s.enable.append(idx)
                idx += 1
    def disable_max_control(s):
        """ Disable some of the systematics for the control plots, per Max.
        Thinking again about this, i think we also don't have the contribution
        from CT10 error set for each plot, similarly as for SF toys. So in the
        list i mentioned below the PDF rw is undefined....let's keep it out for
        the moment.

        Then the proposal is to draw a band with **shape uncertainty only**
        and systematics covering:

        all up/down exp systematics + MC gen + boson pT rw
        """
        assert len(s.flat) == len(s.enable), 'ERROR: disable_max_control requires that everything is enabled first'
        idx = 0
        newenable = s.enable[:]
        for ig,sgroups in enumerate(s.sys):
            name = s.groups[ig]
            for iss,sinst in enumerate(sgroups):
                if idx in s.enable and name in ('PDF',):
                    newenable.remove(idx)
                    print 'Disabling systematic:',name,sinst.name
                idx += 1
        print 'Removed a total of %d systematics'%(len(s.enable) - len(newenable))
        s.enable = newenable[:]
    def qcd_region(s):
        """ Puts all SuSys in qcd region based on qcd map """
        assert False, "This function is unused. Rather, we normally call qcd_region directly on individual SuSys's"
        res = s.clone()
        [o.qcd_region() for i,o in enumerate(res.flat) if i in res.enable]
        res.status = 1 # to prevent infinite recursion
        return res
    def bootstrap(s,charge=2,qcd = {},unfold={},do_unfold=False,qcderr='NOM',
                  ntuple=None,path=None,var=None,bin=None,pre='',weight="mcw*puw*effw*trigw",
                  histo=None,sliced_1d=None,sliced_2d=None,sysdir=None,subdir=None,basedir=None ):
        """ bootstraps a full collection of systematics from the nominal instance of SuSys """
        prep=''
        if sysdir and sysdir[0]=='tight_nominal':
            prep='tight_'
        # nominal first:
        nom = SuSys(charge=charge,qcd=qcd,unfold=unfold,qcderr=qcderr,
                    ntuple=ntuple,path=path,var=var,bin=bin,pre=pre,weight=weight,
                    histo=histo,sliced_1d=sliced_1d,sliced_2d=sliced_2d,sysdir=sysdir,subdir=subdir,basedir=basedir)
        s.sys = [ [nom,] ]
        s.flat = [ nom ]
        s.groups = [ 'Nominal' ]
        s.do_unfold = do_unfold
        # if this is using the ntuple, we don't have all systematics
        if nom.use_ntuple():
            print 'Created ntuple-based SuPlot'
            return
        # systematic variations
        res = []
        def add(n,ss,unfss=None,xadd=None):
            """ Clones sysdir """
            res.append(nom.clone(name=n,sysdir_mc=ss,qcdadd=xadd,unfdir=unfss if unfss!=None else ss))
            s.flat.append(res[-1])
        def add2(n,ss,unfss=None,xadd=None):
            """ Clones subdir (e.g., for efficiencies) """
            res.append(nom.clone(name=n,subdir_mc=ss,qcdadd=xadd,unfdir=unfss if unfss!=None else ss))
            s.flat.append(res[-1])
        def add3(n,ss,unfss,xadd=None):
            """ Clones qcd normalization """
            res.append(nom.clone(name=n,qcderr=ss,unfdir=unfss,qcdadd=xadd))
            s.flat.append(res[-1])
        def add4(n,ss,unfss,bgsig,xadd=None):
            """ Clones with different MC unfolding matrix """
            if ss in nom.unfold['mc']: return False
            res.append(nom.clone(name=n,unfdir=unfss,bgsig=bgsig,qcdadd=xadd))
            res[-1].unfold['mc'] = ss
            s.flat.append(res[-1])
            return True
        def next(nn):
            s.sys.append(res[:])
            s.groups.append(nn)
            del res[:]
        qcdadd = {'forcenominal':False}
        # MCP smearing UP
        add('MuonResMSUp','MuonResMSUp',xadd=qcdadd)
        add('MuonResMSDown','MuonResMSDown',xadd=qcdadd)
        next('MCP_MS_RES')
        # MCP smearing DOWN
        add('MuonResIDUp','MuonResIDUp',xadd=qcdadd)
        add('MuonResIDDown','MuonResIDDown',xadd=qcdadd)
        next('MCP_ID_RES')
        # MCP scale
        if False: # old MCP scale recommendation: on/off
            add('MuonNoScale','MuonNoScale',xadd=qcdadd)
            next('MCP_SCALE')
        else:  # using my C/K variations
            add('MuonKScaleUp','MuonKScaleUp',xadd=qcdadd)
            add('MuonKScaleDown','MuonKScaleDown',xadd=qcdadd)
            next('MCP_KSCALE')
            add('MuonCScaleUp','MuonCScaleUp',xadd=qcdadd)
            add('MuonCScaleDown','MuonCScaleDown',xadd=qcdadd)
            next('MCP_CSCALE')
        # MCP efficiency
        if True:
            add2('MuonRecoSFUp','MuonRecoSFUp','MuonRecoSFUp',xadd=qcdadd)
            add2('MuonRecoSFDown','MuonRecoSFDown','MuonRecoSFDown',xadd=qcdadd)
            next('MCP_EFF')
        # Pileup rescaling of mu=0.97 (or 1.03 when applied to data)
        if True:
            add2('PileupScale','PileupUp','PileupUp',xadd=qcdadd)
            next('PILEUP_SCALE')
        # trigger systematic
        if True:
            add2('MuonTriggerSFPhi','MuonTriggerSFPhi','MuonTriggerSFPhi',xadd=qcdadd)
            next('MCP_TRIG')
        else:
            add2('MuonTriggerSFUp','MuonTriggerSFUp','MuonTriggerSFUp',xadd=qcdadd)
            add2('MuonTriggerSFDown','MuonTriggerSFDown','MuonTriggerSFDown',xadd=qcdadd)
            next('MCP_TRIG')
        # ISO efficiency
        if True:
            add2('MuonIsoSFUp','MuonIsoSFUp','MuonIsoSFUp',xadd=qcdadd)
            add2('MuonIsoSFDown','MuonIsoSFDown','MuonIsoSFDown',xadd=qcdadd)
            next('MCP_ISO')
        # JET
        if True:
            add('JetResolUp','JetResolUp',xadd=qcdadd)
            add('JetResolDown','JetResolDown',xadd=qcdadd)
            next('JER')
            add('JetScaleUp','JetScaleUp',xadd=qcdadd)
            add('JetScaleDown','JetScaleDown',xadd=qcdadd)
            next('JES')
            add('JetNPVUp','JetNPVUp',xadd=qcdadd)
            add('JetNPVDown','JetNPVDown',xadd=qcdadd)
            next('JES_NPV')
            add('JetMUUp','JetMUUp',xadd=qcdadd)
            add('JetMUDown','JetMUDown',xadd=qcdadd)
            next('JES_MU')
        # MET
        if True:  # new recommended MET systematic
            add('ResoSoftTermsUp_ptHard','ResoSoftTermsUp_ptHard',xadd=qcdadd)
            add('ResoSoftTermsDown_ptHard','ResoSoftTermsDown_ptHard',xadd=qcdadd)
            next('MET_RESO')
            add('ScaleSoftTermsUp_ptHard','ScaleSoftTermsUp_ptHard',xadd=qcdadd)
            add('ScaleSoftTermsDown_ptHard','ScaleSoftTermsDown_ptHard',xadd=qcdadd)
            next('MET_SCALE')
        # using fully calibrated jets for MET?
        if False:
            add('NominalCalJet','NominalCalJet',xadd=qcdadd)
            next('MET_CALJET')
        # W pT targets
        if True:
            add2('WptSherpa','WptSherpa','WptSherpa',xadd=qcdadd)
            add2('WptPythia8','WptPythia8','WptPythia8',xadd=qcdadd) # new: as of 03/06/2013
            #add2('WptPythiaMC10','WptPythiaMC10','WptPythiaMC10',xadd=qcdadd)
            next('WPT_REWEIGHT')
        # PDF reweighting
        if False: # new: as of 03/17/2013
            #add2('PdfCT10nlo','PdfCT10nlo',xadd=qcdadd)
            add2('PdfMSTW','PdfMSTW','PdfMSTW',xadd=qcdadd)
            add2('PdfHERA','PdfHERA','PdfHERA',xadd=qcdadd)
            add2('PdfNNPDF','PdfNNPDF','PdfNNPDF',xadd=qcdadd)
            add2('PdfABM','PdfABM','PdfABM',xadd=qcdadd)
            next('PDF')
        # QCD normalization
        if True:
            add3('qcdup',1.04,'Nominal',xadd=qcdadd)
            add3('qcddown',0.96,'Nominal',xadd=qcdadd)
            next('QCD_FRAC')
        # Signal MC systematic (previously: unfolding systematic)
        if True and 'mc' in nom.unfold:
            if 'powheg_pythia' in nom.unfold['mc']:
                add4('unfoldMCNLO','mcnlo','Nominal',1,xadd=qcdadd)
                next('Generator ME')
                add4('unfoldPowhegJimmy','powheg_herwig','Nominal',4,xadd=qcdadd)
                next('Generator PS')
            else:
                add4('unfoldMCNLO','mcnlo','Nominal',1,xadd=qcdadd)
                add4('unfoldPowhegJimmy','powheg_herwig','Nominal',4,xadd=qcdadd)
                add4('unfoldPowhegPythia','powheg_pythia','Nominal',5,xadd=qcdadd)
                next('UNFOLDING')
        assert len(s.sys)==len(s.groups)
        print 'Created systematic variations: N =',len(s.sys)
    def update_errors(s,sysonly=False,force=False,rebin=1,scale=1,renorm=False,avg=True):
        """ folds systematic variations into total TH1 error.
        If renorm is true, scale holds the data normalization
        Possible upgrade: independent two-sided variations (non-symmetrized)
        if avg=True, it takes the mean up/down variation, as opposed to max
        """
        # list of systematics that should NOT be averaged no matter what
        NAVG = [ 'WPT_REWEIGHT' , 'PDF' ]
        if s.hsys and sysonly==True and not force: return s.hsys
        if s.htot and sysonly==False and not force: return s.htot
        stack_mode = False
        if not s.sys[0][0].h:
            assert s.sys[0][0].stack
            stack_mode = True
        s.htot = s.sys[0][0].stack.GetStack().Last().Clone() if stack_mode else s.sys[0][0].h.Clone()
        s.htot.Sumw2()
        if renorm:
            s.htot.Scale( scale / s.htot.Integral() )
        elif scale!=1:
            s.htot.Scale(scale)
        s.hsys = s.htot.Clone()
        [s.hsys.SetBinError(ii,0) for ii in xrange(0,s.hsys.GetNbinsX()+2)]
        i = len(s.sys[0])
        iss = 1
        for hss in s.sys[1:]:
            bdiffs = [[] for z in xrange(0,s.hsys.GetNbinsX()+2)]
            for hs in hss: # loop over systematics in this group
                if not i in s.enable:
                    i+=1
                    continue
                h = hs.stack.GetStack().Last().Clone() if stack_mode else hs.h.Clone()
                if renorm:
                    h.Scale( scale / h.Integral() )
                elif scale!=1:
                    h.Scale(scale)
                for ibin in xrange(0,s.hsys.GetNbinsX()+2):
                    bdiffs[ibin].append ( abs(s.hsys.GetBinContent(ibin)-h.GetBinContent(ibin)) )
                i+=1
            davg = (avg==True) and not (s.groups[iss] in NAVG)
            for ibin in xrange(0,s.hsys.GetNbinsX()+2):
                newerr = (  (bdiffs[ibin][0]+bdiffs[ibin][1])/2.0 if len(bdiffs[ibin])==2 and davg==True else max(bdiffs[ibin])  )   if len(bdiffs[ibin])>0 else 0
                if False and ibin==4: #DEBUG
                    print i,'%.1f'%s.sys[0][0].stack.GetStack().Last().GetBinError(ibin),['%.1f'%zz for zz in bdiffs[ibin]],'%.1f'%newerr,'%.1f'%s.htot.GetBinError(ibin),'%.1f'%s.hsys.GetBinError(ibin)
                # hsys
                olderr = s.hsys.GetBinError(ibin)
                s.hsys.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + 1.0*newerr*newerr))
                # htot
                olderr = s.htot.GetBinError(ibin)
                s.htot.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + 1.0*newerr*newerr))
            iss += 1
        if rebin!=1:
            s.hsys.Rebin(rebin)
            s.htot.Rebin(rebin)
        return s.hsys if sysonly else s.htot
    def summary_bin_txt(s,b):
        """ Prints relative deviation of various systematics in a given bin """
        nom = s.nominal_h()
        for ig,hss in enumerate(s.sys[1:]):
            print s.groups[ig+1],':'
            for hs in hss: # loop over systematics in this group
                print '%s \t\t:\t\t %.2f%%' % (hs.name, 100.0*(hs.h.GetBinContent(b) - nom.GetBinContent(b))/nom.GetBinContent(b) if nom.GetBinContent(b) else 0 )
        pass
    def individual_systematics(s,canvas_name='SYS',norm=False):
        """
        if norm==True, absolute relative deviations are plotted
        """
        assert 0 in s.enable,'Nominal histogram must always be enabled'
        nom = s.nominal_h()
        bins = range(1,nom.GetNbinsX()+1)
        # statistical errors (for reference)
        stat = nom.Clone("staterr")
        stat.Reset()
        stat.Sumw2()
        for b in bins:
            err  = 100.0*(nom.GetBinError(b))/nom.GetBinContent(b) if nom.GetBinContent(b) else 0
            stat.SetBinContent(b,err)
        Cc,Hh,Ll = None,[],None
        s.Hh = Hh
        colorlist = [2,3,4,5,6,20,28,41,46]
        colorlist += colorlist
        colorlist += colorlist
        markerlist = [20,21,22,23,29,33,34]
        markerlist+= markerlist
        markerlist+= markerlist
        import SuCanvas
        s.Ccan = SuCanvas.SuCanvas(canvas_name)
        s.Ccan.buildDefault(width=800,height=600)
        s.Cc = Cc = s.Ccan.cd_canvas()
        Cc.cd()
        for ig,hss in enumerate(s.flat[1:]):
            if ig+1 not in s.enable: continue
            hh = nom.Clone('hsys%d'%ig)
            hh.Reset()
            hh.Sumw2()
            hh.SetFillColor(0)
            Hh.append( hh )
            Hh[-1].SetLineColor(colorlist[ig])
            Hh[-1].SetMarkerColor(colorlist[ig])
            Hh[-1].SetMarkerStyle(markerlist[ig])
            Hh[-1].SetMarkerSize(1.5)
        s.Ll = Ll = ROOT.TLegend()
        Ll.SetFillColor(0)
        Ll.SetHeader('Systematics:')
        itot = 0
        for ig,hs in enumerate(s.flat[1:]):
            if ig+1 not in s.enable: continue
            name = hs.name
            for b in bins:
                diff = 100.0*(hs.h.GetBinContent(b) - nom.GetBinContent(b))/nom.GetBinContent(b) if nom.GetBinContent(b) else 0
                if norm==True:
                    diff = abs(diff)
                Hh[itot].SetBinContent(b,diff)
            Ll.AddEntry(Hh[itot],name,'LP')
            itot += 1
        Hh[0].Draw('C P0')
        s.Ccan.ConfigureAxis(Hh[0], None)
        vmax = max ( hh.GetMaximum() for hh in Hh )
        vmax = vmax*2.5 if vmax>0 else vmax
        vmin = min ( hh.GetMinimum() for hh in Hh )
        vmin = vmin*1.3 if vmin<0 else vmin
        Hh[0].GetYaxis().SetRangeUser(vmin,vmax)
        Hh[0].GetYaxis().SetTitle('Percentage deviation')
        Hh[0].GetXaxis().SetTitle('|#eta|')
        [ih.Draw('C P0 same') for ih in Hh[1:]]
        #stat.Draw('C P0 same')
        s.Ccan.ConfigureLegend(Ll)
        Ll.Draw('same')
        return s.Ccan
    def update_var(s,histo,bin=None):
        """ Updates histogram name and ntuple-expression (aka variable)
        In reality, only one of these applies for a given SuData instance.
        """
        for i,o in enumerate(s.flat):
            if i in s.enable:
                o.update_var(histo,bin)
    def clone(s,q=None,enable=None,histo=None,do_unfold=None,unfhisto=None,qcdadd=None,
              sysdir=None,sysdir_mc=None,sysdir_qcd=None,
              subdir=None,subdir_mc=None,
              basedir=None,
              sliced_1d=None,sliced_2d=None,slice=None,bgsig=None,
              ntuple=None,path=None,var=None,bin=None,pre=None,weight=None):
        """ Clones an entire SuPlot.
        Each SuSys is cloned individually to avoid soft pointer links
        """
        res = SuPlot()
        res.status = 0
        res.groups = s.groups
        res.sys = []
        res.flat = []
        res.enable = enable if enable!=None else s.enable
        res.do_unfold = do_unfold if do_unfold!=None else s.do_unfold
        for sgroups in s.sys:
            bla = []
            for sinst in sgroups:
                bla.append(sinst.clone(q=q,histo=histo,unfhisto=unfhisto,qcdadd=qcdadd,sysdir=sysdir,sysdir_mc=sysdir_mc,sysdir_qcd=sysdir_qcd,subdir=subdir,subdir_mc=subdir_mc,basedir=basedir,ntuple=ntuple,path=path,var=var,bin=bin,pre=pre,weight=weight,sliced_1d=sliced_1d,sliced_2d=sliced_2d,slice=slice,bgsig=bgsig))
                res.flat.append(bla[-1])
            res.sys.append( bla )
        return res

class SuSample:
    """
    Encapsulates a single TChain
    For example: jimmy_wmunu_np0
    """
    GLOBAL_CACHE = 'cache/%s.root' # if None, global cache is disabled
    nevts_histogram = None         # the name of the nevents histogram for QCD normalization (if None, use default)
    cache = None
    rootpath = None
    lumi = None
    lumifake = False
    xsecerr = 0
    debug = False
    def __init__(s,name,po=None,flags=None,table=None):
        """ constructor """
        s.name = name
        # data sources
        s.nt = {}
        s.nevt = {}
        s.files = []
        s.flags = []   #general flags
        s.table = {}   #general settings map
        s._evcounts = ("nevts","nevts_unw","nevts_mcw","nevts2_vtx","nevts3_wz")
        s.seen_samples = []
        # volatile
        s.path = None  # current ntuple path
        # fast histogram cache
        s.data = {}
        s.missing = {} # keep track of missing histo printouts
        # back-navigation to total stack and the parent StackElm
        s.po = po
        s.flags = flags
        s.table = table
    def addchain(s,path='st_w_final/ntuple'):
        """ add one of the TNtuples
        path excludes dg/
        """
        s.nt[path] = ROOT.TChain(path,path)
        s.nevt[path] = {}
        s.path = path
    def choose_evcount(s,cut=None):
        """ chooses the right nevents histogram depending on which scales were requested
        info: this should alwats be nevts - otherwise it would revert the scale correction
        """
        if SuSample.nevts_histogram!=None:
            return SuSample.nevts_histogram
        DEF1 = 'nevts_unw'  # (unweighted)
        DEF2 = 'nevts_mcw'  # (mc)
        DEF3 = 'nevts'      # (mc+pu)
        DEF4 = 'nevts2_vtx' # (mc+pu+vtx) - old default
        DEF5 = 'nevts3_wz'  # (mc+pu+vtx+wz)
        return DEF5
    def topdir(s,f):
        """ descend down the root file in case it's the output of single-file dgplot merge """
        topdir = 'dg'
        if not f.Get(topdir):
            topdir = ''
            #ldirs = [z.GetName() for z in f.GetListOfKeys()]
        else:
            topdir = topdir+'/'
        return topdir
    def auto(s):
        """ add all files satisfying a glob pattern - using rootpath"""
        assert s.rootpath, 'Please set rootpath variable to use autoloading'
        pattern = os.path.join(s.rootpath,s.name,'root_%s.root'%s.name)
        return s.addglob(pattern)
    def addglob(s,pattern):
        """ add all files satisfying a glob pattern
        WARNING: a missing file will cause a hanged process
        """
        return s.add(pattern) if True else s.add(glob.glob(pattern))
    def get_from_file(s,hpath,i=0):
        """ Returns TFile::Get(hpath) from i'th file """
        assert i<len(s.files)
        return s.files[i].Get( s.topdir(s.files[i])+hpath )
    def xrootdize(s,l):
        """ Prepends root:// prefix and server if the file is on XROOTD """
        if re.match('/atlas',l):
            l = 'root://uct3-xrd.mwt2.org/%s'%l
        elif re.match('/pnfs',l):
            #l = 'dcache://%s'%l
            l = 'root://xrddc.mwt2.org:1096%s'%l
        return l
    def add(s,file_raw):
        """ add a file or list of files to all declared TChain's """
        if type(file_raw)==type([]) or type(file_raw)==type(()):
            file = [s.xrootdize(ff) for ff in file_raw]
            [s.add(f) for f in file]
            return
        else:
            file = s.xrootdize(file_raw)
            s.files.append(ROOT.TFile.Open(file))
            f = s.files[-1]
            # add files to ntuple:
            for path,nt in s.nt.iteritems():
                nadd = nt.AddFile(file,ROOT.TChain.kBigNumber,s.topdir(f)+path)
                if not nadd>0:
                    print 'WARNING: failed to find chain %s in file %s'%(path,file)
                if SuSample.debug:
                    print 'Added %d file(s) to TChain [%s]: %s'%(nadd,nt.GetName(),file)
            # update total nevt counts for xsec normalization
            for ii,ev in enumerate(s._evcounts):
                hname = '%s%s'%(s.topdir(f),ev)
                hist = f.Get(hname)
                n = 0
                if hist:
                    #n = hist.GetEntries()  # unweighted
                    n = hist.GetBinContent(1) # weighted (needed for MC@NLO)
                    if ii==0 and SuSample.debug:
                        print 'Number of GRL events:',n
                    # fill nevt arrays
                    for path in s.nt.keys():
                        if ev in s.nevt[path]:
                            s.nevt[path][ev] += n
                        else:
                            s.nevt[path][ev] = n
                elif SuSample.debug:
                    print 'WARNING: failed to find object [%s] in file [%s]'%(hname,file)
            #f.Close()
    def nentries(s,path=None):
        """ compute number of entries in TNtuple """
        return s.nt[path].GetEntries() if path else s.nt[s.path].GetEntries()
    def get_preserving_lumi(s,evcnt):
        """ Special hack: returns fake s.lumi value that results in a lumi scale of 1.0 for this sample """
        assert re.match('mc',s.name)
        from MC import mc
        mrun = mc.match_sample(s.name)
        assert mrun
        err = mrun.errdown if SuSample.xsecerr<0 else mrun.errup
        xsec = mrun.xsec*mrun.filteff*(1.0 + SuSample.xsecerr*err)
        nevents = s.nevt[s.path][evcnt]
        sample = mrun.sample
        flumi = nevents*1.0 / xsec
        scale = 1.0/nevents*flumi*xsec
        assert abs(scale)-1.0 < 1e-5
        return flumi
    def scale(s,evcnt):
        """ normalize MC histograms to luminosity """
        if not re.match('mc',s.name):
            return 1.0
        from MC import mc
        mrun = mc.match_sample(s.name)
        if mrun:
            err = mrun.errdown if SuSample.xsecerr<0 else mrun.errup
            xsec = mrun.xsec*mrun.filteff*(1.0 + SuSample.xsecerr*err)
            # Choose the right evcnt - depending on which scale factors were used (effw/trigw)
            nevents = s.nevt[s.path][evcnt]
            sample = mrun.sample
            scale = 1.0/nevents*s.lumi*xsec
            if ((s.lumi,sample) not in s.seen_samples) or SuSample.debug:
                print 'MC %s: \t xsec=%.1f (%.1f*%.1f) nb \t nevts=%d scale=%.8f%s'%(sample,xsec,mrun.xsec,mrun.filteff,nevents,scale,' (fakelumi)' if SuSample.lumifake==True else '')
                s.seen_samples.append((s.lumi,sample))
            return scale
        print 'WARNING: unable to find scale for MC sample ',s.name
        return 1.0
    def GetHisto(s,hname,hpath):
        """ Retrieves a sum of histograms from a given file """
        import common
        h = None
        for i,f in enumerate(s.files):
            assert f.IsOpen()
            if not h:
                if SuSample.debug:
                    print 'GetHisto:: %s \t %s%s'%(os.path.basename(f.GetName()),s.topdir(f),hpath),
                try:
                    fgot = f.Get('%s%s'%(s.topdir(f),hpath))
                except:
                    print 'Failed to get: ','%s%s'%(s.topdir(f),hpath)
                    raise
                if not fgot:
                    if SuSample.debug: print ''
                    return None
                fpath = '%s%s'%(s.topdir(f),hpath)
                fname = re.sub(r'[^\w]', '_',s.name+'_'+hname+'_'+fpath+'_'+common.rand_name())
                h = f.Get(fpath).Clone(fname)
                h.Sumw2()
                if SuSample.debug:
                    print ' Int: %.1f  N: %d  x: %.3f'%(h.Integral(),h.GetEntries(),h.GetMean(1))
            else:
                h.Add( f.Get('%s%s'%(s.topdir(f),hpath)) )
        if h:
            h.SetTitle(os.path.basename(s.files[0].GetName())+' : '+hname+" "+hpath)
        return h
    @staticmethod
    def make_habseta(name='habseta_template'):
        """ makes an instance of abseta histogram with detector-motivated binning """
        import array
        import binning
        bins = binning.absetabins
        a = array.array('d',bins)
        return ROOT.TH1D(name,name,len(a)-1,a)
    @staticmethod
    def make_heta(name='heta_template'):
        """ makes an instance of abseta histogram with detector-motivated binning """
        import array
        import binning
        bins = binning.setabins
        a = array.array('d',bins)
        return ROOT.TH1D(name,name,len(a)-1,a)
    def histo(s,hname,dall,rebin=1.0,unitize=False):
        """ A wrapper around histogram-based and ntuple-based histo accessors """
        # ok this is tricky. sometimes, a particular systematic variation calls for a different signal MC
        # if that is the case, the actual histogram is NOT pulled from self, but from a dynamically substituted signal MC
        # HOWEVER: we do not do that for QCD bg subtraction (that's fixed to be Powheg+Pythia)
        sh = s
        def get_sh(d):
            if d.bgsig!= None and s.name in MCMAP and 'driven' not in s.flags:
                if d.bgsig not in MCMAP[s.name]:
                    print 'DEBUG:',s.name,d.bgsig
                    print 'DEBUG:',MCMAP
                assert d.bgsig in MCMAP[s.name]
                sh = s.po.find_sample( MCMAP[s.name][d.bgsig] , gname='sig_'+MAP_BGSIG[d.bgsig] )
                assert sh,'ERROR: unable to change signal sample to a different generator'
                if SuSample.debug:
                    print 'INFO: QCD  generator systematic (%s): switching'%d.name,s.name,'to',MCMAP[s.name][d.bgsig]
                    print '      flags: %s -> %s'%(s.flags,sh.flags)
                return sh
            else:
                return s
        if isinstance(dall,SuSys):  # used in SuFit to produce QCD and ewk templates
            d = dall
            sh = get_sh(d)
            if d.use_ntuple():
                d.h = sh.histo_nt(hname,d.var,d.bin,d.nt_prew(flags=sh.flags),path=d.path,rebin=rebin,hsource=d.histo)
            else:
                d.h = sh.histo_h(hname,d,rebin)
            return dall
        # This is where d splits into per-systematic loop
        # and calls histo_nt or histo_h on a particular SuSys
        for i,d in enumerate(dall.flat):
            if not i in dall.enable: continue
            sh = get_sh(d)
            if d.use_ntuple():
                d.h = sh.histo_nt(hname,d.var,d.bin,d.nt_prew(flags=sh.flags),path=d.path,rebin=rebin,hsource=d.histo)
            else:
                d.h = sh.histo_h(hname,d,rebin)
        return dall
    def histo_nt(s,hname,var,bin,cut,path=None,hsource=None,rebin=1.0):
        """ retrieve a particular histogram from ntuple (with cache) """
        _HSPECIAL =  []   # special values of var and hsource
        _HSPECIAL += [('fabs(l_eta)','lepton_absetav')]
        _HSPECIAL += [('fabs(lP_eta)','leptonP_absetav')]
        _HSPECIAL += [('fabs(lN_eta)','leptonN_absetav')]
        _HSPECIAL += [('l_eta','lepton_etav')]
        _HSPECIAL += [('lP_eta','leptonP_etav')]
        _HSPECIAL += [('lN_eta','leptonN_etav')]
        path = path if path else s.path
        key = None
        spair = (var,hsource)
        if spair in _HSPECIAL:
            key = (s.rootpath,s.name,path,var,bin,cut,hsource)
        else:
            key = (s.rootpath,s.name,path,var,bin,cut)
        assert key
        if False:
            key_str = re.sub(r'[^\w]', '_', '_'.join(key))
        else:
            key_str_all =  '_'.join([str(zz) for zz in key])
            key_str = md5(key_str_all).hexdigest()
        hname2 = hname + '_' + s.name + key_str
        needs_saving = s.GLOBAL_CACHE
        if s.GLOBAL_CACHE and os.path.exists(s.GLOBAL_CACHE%s.name):
            #if True:
            with FileLock(s.GLOBAL_CACHE%s.name):
                s.cache = ROOT.TFile.Open(s.GLOBAL_CACHE%s.name,'READ')
                assert s.cache and s.cache.IsOpen()
                if s.cache.Get(key_str):
                    if SuSample.debug==True:
                        print 'TTree::Cache :',os.path.basename(s.files[0].GetName()),var,'|',bin,'|',cut
                    s.data[key]=s.cache.Get(key_str).Clone(hname2)
                    s.data[key].SetDirectory(0)
                    needs_saving = False
                s.cache.Close()
                s.cache = None
        if not key in s.data:
            #hname = hname + '_' + s.name
            usebin,xtra = True,''
            # special handling to create variable-bin eta histograms:
            if spair in _HSPECIAL[:3]:
                s.habseta = s.make_habseta(hname2)
                usebin = False
                xtra = ' with special abseta binning'
            elif spair in _HSPECIAL[-3:]:
                s.heta = s.make_heta(hname2)
                usebin = False
                xtra = ' with special eta binning'
            print '--> creating sample %s: %s%s'%(s.name,hname,xtra)
            sys.stdout.flush()
            # build from TNtuple
            drawstring = '%s>>%s'%(var,hname2)
            if usebin:
                drawstring = '%s>>%s(%s)'%(var,hname2,bin)
            if SuSample.debug==True:
                print 'TTree::Draw :',drawstring,'|',cut
            s.nt[path].Draw(drawstring,cut,'goff')
            if not ROOT.gDirectory.Get(hname2):
                return None
            s.data[key] = ROOT.gDirectory.Get(hname2).Clone()
            if needs_saving:
                #print 'SAVING INTO CACHE:',key
                with FileLock(s.GLOBAL_CACHE%s.name):
                    s.cache = ROOT.TFile.Open(s.GLOBAL_CACHE%s.name,'UPDATE')
                    assert s.cache and s.cache.IsOpen()
                    s.cache.cd()
                    htmp = s.data[key].Clone()
                    htmp.Write(key_str,ROOT.TObject.kOverwrite)
                    s.cache.Close()
                    s.cache = None
        if not s.data[key]:
            return None
        res = s.data[key].Clone()
        if s.lumi:
            res.Scale(s.scale(evcnt = s.choose_evcount(cut)))
        if rebin!=1:
            res.Rebin(rebin)
        if True:
            res.Sumw2()
        return res
    def histo_h_2dproj(s,hname,d,rebin=1.0):
        """ retrieves a particular 2d histogram and performs a 1d projection
        based on instructions encoded in hname.
        hname format :  d2_abseta_lpt:y:0:8
        """
        hbase = d.histo
        assert len(hbase.split(':'))==4
        horig = hbase.split(':')[0]  # d2_abseta_lpt
        haxis = hbase.split(':')[1]  # y
        assert haxis in ('x','y')
        imin,imax = [int(cc) for cc in hbase.split(':')[2:2+2]]
        haxisP = [z for z in ['x','y'] if z!=haxis][0]
        d.histo = horig
        res2d = s.GetHisto(hname,d.h_path(flags=s.flags))
        if not res2d:
            if len(s.files)==0:
                print 'WARNING: no files found for sample:',s.name
            fname = os.path.basename(s.files[0].GetName())
            key = horig + fname + d.h_path(flags=s.flags)
            if key not in s.missing:
                print 'WARNING: -> Missing 2d-proj histo:',horig, fname, d.h_path(flags=s.flags)
                s.missing[key] = True
            d.histo = hbase
            return None   # be careful, returning None is a recipe for not noticing problems
        #print 'DEBUG: ',res2d.GetName(),hbase
        res = getattr(res2d,'Projection'+haxisP.upper())(res2d.GetName()+'_%s%d%d'%(haxis,imin,imax),imin,imax,'e')
        assert res,'Failed to perform 2d projection'
        if s.lumi:
            res.Scale(s.scale(evcnt = s.choose_evcount('')))
        if rebin!=1:
            res.Rebin(rebin)
        res.Sumw2()
        d.histo = hbase
        return res
    def histo_h_3dproj(s,hname,d,rebin=1.0):
        """ retrieves a particular 3d histogram and performs a 1d projection
        based on instructions encoded in hname.
        hname format :  d3_abseta_lpt_met:x:0:8:y:1:2
        """
        hbase = d.histo
        assert len(hbase.split(':'))==7
        horig = hbase.split(':')[0]  # d3_abseta_lpt_met
        haxisA = hbase.split(':')[1]
        haxisB = hbase.split(':')[4]
        assert haxisA!=haxisB,'3D projection: both projection axes are the same'
        haxisP = [z for z in ['x','y','z'] if z!=haxisA and z!=haxisB][0]
        assert haxisA in ('x','y','z')
        assert haxisB in ('x','y','z')
        iminA,imaxA = [int(cc) for cc in hbase.split(':')[2:2+2]]
        iminB,imaxB = [int(cc) for cc in hbase.split(':')[5:5+2]]
        imin1,imax1 = (iminA,imaxA) if haxisA<haxisB else (iminB,imaxB)
        imin2,imax2 = (iminA,imaxA) if haxisA>haxisB else (iminB,imaxB)
        haxis1 = haxisA if haxisA<haxisB else haxisB
        haxis2 = haxisA if haxisA>haxisB else haxisB
        d.histo = horig
        res3d = s.GetHisto(hname,d.h_path(flags=s.flags))
        if not res3d:
            if len(s.files)==0:
                print 'WARNING: no files found for sample:',s.name
            fname = os.path.basename(s.files[0].GetName())
            key = horig + fname + d.h_path(flags=s.flags)
            if key not in s.missing:
                print 'WARNING: -> Missing 3d-proj histo:',horig, fname, d.h_path(flags=s.flags)
                s.missing[key] = True
            d.histo = hbase
            return None   # be careful, returning None is a recipe for not noticing problems
        nname = res3d.GetName()+'_%s_%d_%d__%s_%d_%d'%(haxis1,imin1,imax1,haxis2,imin2,imax2)
        res = getattr(res3d,'Projection'+haxisP.upper())(nname,imin1,imax1,imin2,imax2,'e')
        assert res,'Failed to perform 3d projection'
        if s.lumi:
            res.Scale(s.scale(evcnt = s.choose_evcount('')))
        if rebin!=1:
            res.Rebin(rebin)
        res.Sumw2()
        d.histo = hbase
        return res
    def histo_h(s,hname,d,rebin=1.0):
        """ retrieve a particular histogram; path is re-built manually"""
        # in case we're asking for a projection from a 2d/3d histogram
        if len(d.histo.split(':'))==7:
            return s.histo_h_3dproj(hname,d,rebin)
        elif len(d.histo.split(':'))==4:
            return s.histo_h_2dproj(hname,d,rebin)
        # or else, it's a regular 1d or 2d histogram
        res = s.GetHisto(hname,d.h_path(flags=s.flags))
        if not res:
            if len(s.files)==0:
                print 'WARNING: no files found for sample:',s.name
            fname = os.path.basename(s.files[0].GetName())
            key = hname + fname + d.h_path(flags=s.flags)
            if key not in s.missing:
                print 'WARNING: -> Missing histo:',hname, fname, d.h_path(flags=s.flags)
                s.missing[key] = True
            return None   # be careful, returning None is a recipe for not noticing problems
        if s.lumi:
            res.Scale(s.scale(evcnt = s.choose_evcount('')))
        if rebin!=1:
            res.Rebin(rebin)
        res.Sumw2()
        return res
    
class SuStackElm:
    """
    Encapsulates a set of SuSamples (to be drawn with one color)
    For example: jimmy_wmunu_np{0..5}
    """
    new_scales = True
    qcdsource = None
    qcdscale = None
    qcdfile = None
    def __init__(s,name,label,samples,color=ROOT.kBlack,flags=[],table={},po=None,sample_weights=None):
        """ constructor """
        s.samples = [SuSample(a) for a in samples]
        assert sample_weights!=None,'Programming bug: sample_weights should be set to 1.0 if not used'
        s.sample_weights = sample_weights  # additional weight for each sample. E.g., -1.0 for qcd template bg subtraction
        s.name = name     # internal name to refer to this set of SuSamples
        s.label = label   # label used in printouts and on the legend
        s.color = color
        s.flags = list(set([a.lower() for a in flags]))
        s.table = table
        s.po = po # for back-navigation
        for a in s.samples: # propagate the flags to individual SuSample's
            a.flags = s.flags
            a.table = table
            a.po = po
    def addchain(s,path):
        """ add one of the TNtuples """
        return [e.addchain(path) for e in s.samples]
    def auto(s):
        """ add all files satisfying a glob pattern - using rootpath"""
        if SuSample.debug:
            print 'Auto-load:',s.label,s.name,':',','.join([e.name for e in s.samples])
        return [e.auto() for e in s.samples]
    def histo(s,hname,d,unitize=False,rebin=1.0):
        """ sum histogram of all subsamples """
        if len(s.samples)==0:
            return None
        assert len(s.samples) == len(s.sample_weights),'Unequal number of samples (%d) and sample weights (%d)'%(len(s.samples),len(s.sample_weights))
        # special case: qcd from a separate ROOT file
        if s.qcdsource!=None and 'qcd' in s.flags and (isinstance(d,SuPlot) and d.status==0):
            qs = s.qcdsource
            assert len(qs.split(':'))==2,'Format: file.root:histo'
            qs1 = qs.split(':')[0]
            qs2 = qs.split(':')[1]
            if not SuStackElm.qcdfile:
                SuStackElm.qcdfile = ROOT.TFile.Open(qs1,'READ')
            assert SuStackElm.qcdfile.IsOpen()
            hpath = SuSys.QMAP[d.nominal().charge][1] + '/' + qs2
            htmp = SuStackElm.qcdfile.Get(hpath)
            assert htmp,'ERROR: unable to retrieve QCD histogram %s from file %s'%(hpath,qs1)
            htmp = htmp.Clone('qcd_from_file')
            htmp.SetLineColor(ROOT.kBlack)
            htmp.SetFillColor(s.color)
            dout = d.clone()
            for dd in dout.flat:
                dd.h = htmp
            print 'INFO: retrieved QCD histogram from file : %s'%hpath
            return dout
        # otherwise retrieve the histogram from the main ntuples
        res = s.samples[0].histo(hname,d,rebin=rebin)
        assert s.sample_weights[0] == 1
        for ih,h in enumerate(s.samples[1:]):
            htmp = h.histo(hname,d.clone(),rebin=rebin)
            res.Add(htmp,s.sample_weights[ih+1])
        # special trick: merge mu+ and mu- data QCD templates (for better statistics)
        # for actual qcd contribution in signal region:
        if SuStack.QCD_MIX_CHARGE and 'qcd' in s.flags and 'driven' in s.flags and isinstance(d,SuPlot) and d.nominal().charge in (0,1):
            print 'INFO: combining charges for a data-driven QCD histogram (in signal region)'
            otherq = 0 if d.nominal().charge==1 else 1
            for ih,h in enumerate(s.samples[0:]):
                htmp = h.histo(hname,d.clone(q=otherq),rebin=rebin)
                res.Add(htmp,s.sample_weights[ih])
        # for qcd template in qcd fits
        if SuStack.QCD_MIX_CHARGE and 'qcd' in s.flags and 'driven' in s.flags and isinstance(d,SuSys) and d.charge in (0,1):
            print 'INFO: combining charges for a data-driven QCD histogram (in qcd fit control region)'
            otherq = 0 if d.charge==1 else 1
            for ih,h in enumerate(s.samples[0:]):
                htmp = h.histo(hname,d.clone(q=otherq),rebin=rebin)
                res.Add(htmp,s.sample_weights[ih])
        if res:
            res.SetLineColor(ROOT.kBlack)
            res.SetFillColor(s.color)
            #res.SetMarkerSize(0)
            if unitize:
                res.Unitize()
            # special scaling: for QCD
            elif SuStackElm.new_scales==True and 'qcd' in s.flags and (isinstance(d,SuPlot) and d.status==0):
                if s.qcdscale!=None:
                    res.ScaleOne(s.qcdscale)
                else:
                    print '--------->', 'qcd scaling start:',hname
                    sys.stdout.flush()
                    scales = s.po.get_scales(d)
                    print '--------->', 'qcd scaling applying (QCD_SYS_SCALES=%d):'%SuStack.QCD_SYS_SCALES,hname
                    sys.stdout.flush()
                    if SuStack.QCD_SYS_SCALES:
                        res.Scale(scales)
                    else:
                        res.ScaleOne(scales)
                    print '--------->', 'qcd scaling end:',hname
                    sys.stdout.flush()
        assert res,'Failed to create: ' + hname
        return res

class SuStack:
    """
    Provides top-level interface to retrieve stacked histograms for all data and BG
    Underlying caching is implemented in SuSample
    This class does not own the histograms it creates!
    """
    QCD_SYS_SCALES = False
    QCD_TF_FITTER = True
    QCD_STAT_HACK = 1
    QCD_EXC_ZERO_BINS = None
    QCD_USE_FITTER2 = False
    def __init__(s):
        """ constructor """
        # stack elements
        s.elm = []
        # QCD normalization fit results
        s.scales = {} # cache of QCD normalization scales
        s.scalekeys = []
        # garbage collector
        s.gbg = []
        s.fitnames = {}
        s.fits = {}
        # for unfolding:
        s.funfold = {}
        # stack composition flags
        s.flagsum = {}
    def addchain(s,path):
        """ add one of the TNtuples """
        return [e.addchain(path) for e in s.elm]
    def auto(s):
        """ add all files satisfying a glob pattern - using rootpath"""
        print 'Auto-loading ntuples...'
        return [e.auto() for e in s.elm]
    def set_path(s,path):
        """ Changes default ntuple path """
        for elm in s.elm:
            for sam in elm.samples:
                assert path in sam.nt.keys()
                sam.path = path
    @staticmethod
    def add_flag(m,v='no'):
        if not v in m.flags:
            m.flags+=[v]
    @staticmethod
    def rm_flag(m,v='no'):
        if v in m.flags:
            m.flags.remove(v)
    def get_flagsum(s):
        """ Returns the flag-summary string that summarizes the choice of MC samples and QCD normalizations """
        res = []
        keys = sorted(s.flagsum.keys())
        for key in keys:
            res.append('%s%s'%(key,s.flagsum[key]))
        return ''.join(res)
    def choose_flag(s,name,flag):
        """ Select a particular subsample with a given flag, turning all others off """
        loop = [e for e in s.elm if flag in e.flags]
        for elm in loop:
            if elm.name == name:
                SuStack.rm_flag(elm,'no')
            else:
                SuStack.add_flag(elm,'no')
        return True
    def choose_qcd(s,i):
        """ Select QCD sample, turning all others off """
        s.flagsum['Q']=i
        return s.choose_flag('qcd_'+MAP_BGQCD[i],'qcd')
    def which_sig(s):
        return s.flagsum['S']
    def choose_sig(s,i):
        """ Select signal sample, turning all others off """
        s.flagsum['S']=i
        return s.choose_flag('sig_'+MAP_BGSIG[i],'sig')
    def choose_tau(s,i):
        """ Select tau samples, turning all others off """
        s.choose_wtaunu(i)
        s.choose_ztautau(i)
    def choose_ewk(s,i,ntuple='w'):
        """ Select electroweak (w or z) samples, turning all others off """
        if ntuple=='w':
            s.choose_zmumu(i)
        elif ntuple=='z':
            s.choose_wmunu(i)
    def choose_wmunu(s,i):
        """ Select signal sample, turning all others off """
        s.flagsum['V']=i
        return s.choose_flag('wmunu_'+MAP_BGSIG[i],'wmunu')
    def choose_zmumu(s,i):
        """ Select signal sample, turning all others off """
        s.flagsum['X']=i
        return s.choose_flag('zmumu_'+MAP_BGSIG[i],'zmumu')
    def choose_wtaunu(s,i):
        """ Select signal sample, turning all others off """
        s.flagsum['Y']=i
        return s.choose_flag('wtaunu_'+MAP_BGSIG[i],'wtaunu')
    def choose_ztautau(s,i):
        """ Select signal sample, turning all others off """
        s.flagsum['Z']=i
        return s.choose_flag('ztautau_'+MAP_BGSIG[i],'ztautau')
    def all_samples(s):
        """ returns all samples """
        res = []
        for elm in s.elm:
            for sam in elm.samples:
                res.append(sam)
        return res
    def print_counts(s):
        """ print nevents for each sample """
        tot = 0
        for elm in s.elm:
            print '==',elm.name,elm.label,'=='
            for sam in elm.samples:
                print '--->',sam.name,sam.nentries()
                sys.stdout.flush()
                tot += sam.nentries()
        print 'Total:',tot
    def add(s,name,samples,color,flags=[],table={},label=None,sample_weights=None,sample_weights_bgsub=False):
        """ backward-compatible interface:
            po.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen)
            if sample_weights_bgsub is set to True, all samples that start with mc_ automatically get a weight of -1.0
        """
        if not isinstance(samples,list):
            samples = [samples,]
        if label==None: label = name
        if sample_weights==None:
            if sample_weights_bgsub==True:
                sample_weights = [-1.0 if re.match('mc_',sam) else 1.0 for sam in samples]
            else:
                sample_weights = [1.0]*len(samples)
        assert sample_weights[0]==1.0,'Attention: first weight HAS to be 1.0'
        s.elm.append( SuStackElm(name,label,samples,color,flags,table,po=s,sample_weights=sample_weights) )
    def adn(s,name,samples,color,flags=[],table={},label=None,sample_weights=None,sample_weights_bgsub=False):
        """ add with flags=['no',...] """
        return s.add(name,samples,color,flags+['no'],table,label,sample_weights,sample_weights_bgsub)
    def load_unfolding(s):
        """ Loads unfolding migration matrices """
        # make sure RooUnfold.so is loadable:
        from ROOT import RooUnfold
        hpythia = os.path.join(SuSample.rootpath,'unfold_pythia.root')
        hmcnlo  = os.path.join(SuSample.rootpath,'unfold_mcnlo.root')
        halpgen_herwig = os.path.join(SuSample.rootpath,'unfold_alpgen_herwig.root')
        halpgen_pythia = os.path.join(SuSample.rootpath,'unfold_alpgen_pythia.root')
        hpowheg_herwig = os.path.join(SuSample.rootpath,'unfold_powheg_herwig.root')
        hpowheg_pythia = os.path.join(SuSample.rootpath,'unfold_powheg_pythia.root')
        if os.path.exists(hpythia):
            s.funfold['pythia'] = ROOT.TFile.Open(hpythia)
            print 'Found unfolding matrices for:','pythia'
        else:
            print 'Cannot find unfolding matrices for:','pythia',hpythia
        if os.path.exists(hmcnlo):
            s.funfold['mcnlo'] = ROOT.TFile.Open(hmcnlo)
            print 'Found unfolding matrices for:','mcnlo'
        else:
            print 'Cannot find unfolding matrices for:','mcnlo',hmcnlo
        if os.path.exists(halpgen_herwig):
            s.funfold['alpgen_herwig'] = ROOT.TFile.Open(halpgen_herwig)
            print 'Found unfolding matrices for:','alpgen_herwig'
        else:
            print 'Cannot find unfolding matrices for:','alpgen_herwig',halpgen_herwig
        if os.path.exists(halpgen_pythia):
            s.funfold['alpgen_pythia'] = ROOT.TFile.Open(halpgen_pythia)
            print 'Found unfolding matrices for:','alpgen_pythia'
        else:
            print 'Cannot find unfolding matrices for:','alpgen_pythia',halpgen_pythia
        if os.path.exists(hpowheg_herwig):
            s.funfold['powheg_herwig'] = ROOT.TFile.Open(hpowheg_herwig)
            print 'Found unfolding matrices for:','powheg_herwig'
        else:
            print 'Cannot find unfolding matrices for:','powheg_herwig',hpowheg_herwig
        if os.path.exists(hpowheg_pythia):
            s.funfold['powheg_pythia'] = ROOT.TFile.Open(hpowheg_pythia)
            print 'Found unfolding matrices for:','powheg_pythia'
        else:
            print 'Cannot find unfolding matrices for:','powheg_pythia',hpowheg_pythia
    def run_unfolding(s,dall):
        """ Unfolds SuSys histograms to particle level """
        if isinstance(dall,SuSys) or not dall.do_unfold: return dall
        if not s.funfold: s.load_unfolding()
        out = dall.clone()
        for i,d in enumerate(out.flat):
            if i not in out.enable: continue
            response = s.funfold[d.unfold['mc']].Get(d.unfold_get_path())
            h = d.h if d.h else d.stack.GetStack().Last()
            assert response
            method = d.unfold['method']
            #print 'Unfolding method:',method,d.name,d.unfold['mc']
            par = d.unfold['par']
            unfold = None
            if method=='RooUnfoldBayes':
                unfold = ROOT.RooUnfoldBayes(response, h, par);
            elif method=='RooUnfoldBinByBin':
                unfold = ROOT.RooUnfoldBinByBin(response, h);
            elif method=='RooUnfoldSvd':
                unfold = ROOT.RooUnfoldSvd(response, h, par);
            assert unfold
            unfold.SetVerbose(0)
            # replace original histo
            d.h = unfold.Hreco()
        return out
    def get_scales(s,d):
        if not SuStack.QCD_SYS_SCALES:
            return s.get_scale_wrap(d.nominal())
        res = []
        for i,o in enumerate(d.flat):
            if i not in d.enable: continue
            # special treatment for systematics for which we choose NOT to redo QCD fits
            if 'forcenominal' in o.qcd and o.qcd['forcenominal']==True:
                if o.name != 'Nominal':
                    print 'INFO: forcing *nominal* QCD fit for sytematic %d (%s)'%(i,o.name)
                res.append(s.get_scale_wrap(d.nominal()))
            else:
                res.append(s.get_scale_wrap(o))
        return res
    def get_scale_wrap(s,d):
        """ A wrapper around get_scale() that knows how to deal with bin_%d/lpt sliced_1d histograms
        Note: d is an instance of SuSys, not SuPlot
        """
        if d.is_sliced_1d():
            # retrieve an instance of the sliced_1d histogram (vs lpt) to bootstrap the binning
            hpt = s.sig('tmp',d.clone(q=0)).h
            if not hpt:
                hpt = s.sig('tmp',d.clone(q=1)).h
            assert hpt,'Unable to retrieve original histogram: %s'%d.h_path()
            nb = hpt.GetNbinsX()
            tname=None
            res = []
            for ib in xrange(nb):
                tmin = int(hpt.GetBinLowEdge(ib+1))
                tmax = tmin + int(hpt.GetBinWidth(ib+1))
                tname = '%s_%d'%(os.path.basename(d.h_path()),ib) #lpt_0
                # modify basedir to become metfit/bin_%d/lpt_%d
                dtmp = d.clone()
                dtmp.basedir = dtmp.qlist( d.basedir[2] + '/bin_%d'%d.slice + '/' + tname ) # nominal/bin_%(ieta)/lpt_%(ipt)
                # mechanism to manually skip non-convergent fits  (SIGFLAG,Q,QCDVAR,ETA,PT)
                VETO = [(1,1,'met',7,2),(1,0,'wmt',6,5),(1,1,'wmt',0,5),(1,1,'wmt',2,5),(1,1,'wmt',3,5),(1,1,'wmt',4,3)]
                VETO = []
                if (s.flagsum['S'],d.charge,d.qcd['var'],d.slice,ib) in VETO:
                    res.append(0.0)
                else:
                    res.append(s.get_scale(dtmp))
            # duplicate first/last bin scales for underflow/overflow scale
            return [res[0],] + res + [res[-1],]
        elif d.is_sliced_2d():
            assert False,'Not implemented yet'
        elif d.is_ntuple_etabins():
            print 'INFO: creating QCD scales in eta bins using ntuple fits'
            import binning
            echoice = d.is_ntuple_etabins()
            assert echoice in (1,2),'Unknown ntuple_etabins choice'
            etabins = binning.absetabins if echoice==2 else binning.setabins
            res = []
            oldpre = [zz for zz in d.pre]
            oldpreqcd = [zz for zz in d.qcd['pre']]
            for ib in xrange(0,len(etabins)-1):
                ibb = ib
                x = ''
                if echoice==2:
                    x = ' && fabs(l_eta)>=%.2f && fabs(l_eta)<=%.2f'%(etabins[ibb],etabins[ibb+1])
                elif echoice==1:
                    x = ' && l_eta>=%.2f && l_eta<=%.2f'%(etabins[ibb],etabins[ibb+1])
                # modify pre string - but via s.qcd['pre'] because that's what will be used!
                assert len(d.pre)==3
                assert len(d.qcd['pre'])==3
                newpre = [zz+x for zz in oldpre]
                dtmp = d.clone(pre=newpre)
                newpre = [zz+x for zz in oldpreqcd]
                d.qcd['pre'] = newpre
                res.append(s.get_scale(dtmp,fitsfx='_eta%d'%ib))
            # duplicate first/last bin scales for underflow/overflow scale
            return [res[0],] + res + [res[-1],]
        elif d.is_histo_etabins():
            print 'INFO: creating QCD scales in eta bins using histo fits'
            assert False,'Update me - no longer using bin_ subfolders'
            import binning
            echoice = d.is_histo_etabins()
            assert echoice in (1,2),'Unknown ntuple_etabins choice'
            etabins = binning.absetabins if echoice==2 else binning.setabins
            res = []
            oldvar = d.qcd['var']
            for ib in xrange(0,len(etabins)-1):
                ibb = ib
                var = 'bin%s_%d/%s'%('' if echoice==2 else 'e',ibb,oldvar)
                # modify qcd fit var via s.qcd['var'] because that's what will be used!
                qcdadd = {'var':var}
                dtmp = d.clone(qcdadd=qcdadd)
                res.append(s.get_scale(dtmp))
            # duplicate first/last bin scales for underflow/overflow scale
            return [res[0],] + res + [res[-1],]
        return s.get_scale(d)  #same scale factor for all bins
    def scale_sys(s,key,d):
        """ s.scales[key] = (scale,scale_error).
        d.qcderr is either NOM/UP/DOWN, or a multiplicative scale factor.
        Note that this function also propagates the results of scale fit to SuSys object d"""
        res = None
        if d.qcderr=='NOM':
            res = s.scales[key][0]
        elif d.qcderr=='UP':
            res = s.scales[key][0]*(1.0+1.0*s.scales[key][1])
        elif d.qcderr=='DOWN':
            res = s.scales[key][0]*(1.0-1.0*s.scales[key][1])
        else:
            qcderr = float(d.qcderr)
            res =  s.scales[key][0]*qcderr
        assert res,'Unsupported QCD scale variation'
        # propagate fractions/scales and their errors to SuSys object
        #assert len(d.scales)==0  # not true in slices!
        d.scales += s.scales[key] # includes all scales, fractions, and errors
        d.fits   += [s.fits[key]]
        return res
    def get_scale(s,d,fitsfx=''):
        """ A lookup cache of QCD scale values
        Note: d is an instance of SuSys, not SuPlot
        """
        # massage the path to put us in the QCD region
        d2 = d.clone()
        d2.qcd_region()
        # key encodes stack composition + path to metfit histogram with histo name + range of fit
        key = None
        fitname = None
        if d2.use_ntuple():
            key = SuSys.QMAP[d2.charge][1]+'_'+s.get_flagsum()+'_'+d2.qcd['descr']+'_'+d2.qcd['var']+d2.pre[2]+'_'+str(d2.qcd['min'])+'to'+str(d2.qcd['max'])
            fitname = SuSys.QMAP[d2.charge][1]+'_'+d2.qcd['descr']+fitsfx+'_'+s.get_flagsum()+'_'+d2.qcd['var']+'_'+str(d2.qcd['min'])+'to'+str(d2.qcd['max'])
        else:
            key = s.get_flagsum()+'_'+d2.h_path_fname()+'_'+str(d2.qcd['min'])+'to'+str(d2.qcd['max'])
            if d2.bgsig != None:
                key = 'S%d_'%(d2.bgsig)+key
            fitname = key
        assert key
        if key in s.scales:
            return s.scale_sys(key,d)
        print 'COMPUTING NEW QCD WEIGHT:',key
        import SuFit
        f = SuFit.SuFit()
        f.addFitVar( d2.qcd['var'], d2.qcd['min'] , d2.qcd['max'] , '%s (GeV)'%(d2.qcd['var']) );
        # get histograms
        if SuSample.debug: print 'SuData::get_scale(SuFit): 0 making data'
        hdata   = s.data('data',d2.clone()).h
        assert hdata.Integral()>0,'data template is empty for systematic: %s'%d.name
        if SuSample.debug: print 'SuData::get_scale(SuFit): 0 making qcd'
        hfree = s.qcd('bgfree',d2.clone()).h    # using correct lumi for bg subtraction here
        assert hfree.Integral()>0,'qcd template is empty (%.3f) for systematic: %s'%(hfree.Integral(),d.name)
        # use fake lumi to make sure the signal statistics is not scaled before entering TFractionFitter
        oldlumi = SuSample.lumi
        if SuStack.QCD_STAT_HACK>0:
            thesig = None
            # normalize to signal statistics
            if SuStack.QCD_STAT_HACK==1:
                loop = [e for e in s.elm if 'sig' in e.flags and 'no' not in e.flags]
                assert len(loop)==1,'WARNING: QCD fitter assumes that your signal SuStackElm contains exactly one element'
                samples = loop[0].samples
                thesig = samples[0]
                if d2.charge==0:
                    for sample in samples:
                        if re.search('wplus',sample.name):
                            thesig = sample
                            break
                if d2.charge==1:
                    for sample in samples:
                        if re.search('wmin',sample.name):
                            thesig = sample
                            break
                # changing signal sample to a different generator (e.g., for certain systematics)
                if d2.bgsig != None:
                    signame = thesig.name
                    thesig = s.find_sample( MCMAP[signame][d.bgsig] )
            # normalize to lowest-statistics sample
            elif SuStack.QCD_STAT_HACK==2:
                loop = [e for e in s.elm if 'mc' in e.flags and 'no' not in e.flags and 'driven' not in e.flags]
                msample,mscale = None,-999
                for lp in loop:
                    for sample in lp.samples:
                        cscale = sample.scale(evcnt = sample.choose_evcount())
                        if cscale>=mscale:
                            mscale=cscale
                            msample = sample
                assert msample
                thesig = msample
            else:
                assert False,'Unknown values of SuStack.QCD_STAT_HACK: %d (possible values: 0,1,2)'%(SuStack.QCD_STAT_HACK)
            assert thesig,'Failed to find a montecarlo target for stat hack normalization'
            SuSample.lumi = thesig.get_preserving_lumi(evcnt = thesig.choose_evcount())
            SuSample.lumifake = True
            if SuSample.debug:
                print 'QCD ewk lumi: fake lumi = %.2f, based on this sample: %s'%(SuSample.lumi,thesig.name)
        if SuSample.debug: print 'SuData::get_scale(SuFit): 0 making ewk'
        hfixed = s.ewk('bgfixed',d2.clone()).h
        assert hfixed.Integral()>0,'ewk template is empty (%.2f) for systematic: %s'%(hfixed.Integral(),d.name)
        lumifrac =  oldlumi / SuSample.lumi
        SuSample.lumi = oldlumi
        SuSample.lumifake = False
        if not (hdata and hfree and hfixed):
            print 'WARNING: missing histogram for QCD normalization',hdata,hfree,hfixed
            return 1.0
        assert hdata,'Failed to find data'
        assert hfixed,'Failed to find fixed backgrounds'
        assert hfree,'Failed to find free backgrounds'
        # rebinning
        if 'rebin' in d2.qcd and d2.qcd['rebin']!=1:
            print 'Rebinning QCD fit histograms by a factor:',d2.qcd['rebin']
            hdata  = hdata.Rebin(d2.qcd['rebin'])
            hfixed = hfixed.Rebin(d2.qcd['rebin'])
            hfree  = hfree.Rebin(d2.qcd['rebin'])
        if SuSample.debug:
            print 'QCD: input integrals for data/ewk/qcd = %.1f / %.1f / %.1f'%(hdata.Integral(),hfixed.Integral(),hfree.Integral())
        # run SuFit
        hdata.getLegendName = lambda : 'DATA'
        hfixed.getLegendName = lambda : 'EWK backgrounds'
        hfree.getLegendName = lambda : 'QCD'
        f.setDataBackgrounds(hdata,hfixed,hfree)
        if False: # check the effect of model errors on the fit
            f.blowUpErrors(0,100,inc=True)
        tmp = None
        logscale = 'log' in d2.qcd and d2.qcd['log']==True
        f.doFitTF(d2.qcd['plotrange'],SuStack.QCD_EXC_ZERO_BINS,SuStack.QCD_USE_FITTER2)
        ftitle=SuSys.QMAP[d2.charge][4]
        if not d2.use_ntuple():
            pteta = d2.histo_pteta()
            assert type(pteta)==type([]) and len(pteta)==2
            ftitle = [ SuSys.QMAP[d2.charge][4]+' : '+pteta[0]   ,   pteta[1]   ]
        tmp = f.drawFitsTF(ftitle,logscale=logscale,modbins=SuStack.QCD_PLOT_MODIFIED_BINS,lumifrac=lumifrac)
        assert tmp
        s.fitnames[key] = fitname
        s.fits[key] = tmp[0]
        s.gbg.append((f,hdata,hfixed,hfree,tmp))
        s.scales[key] = (f.scales[0],f.scalesE[0], f.fractions[0],f.fractionsE[0], f.Wscales[0],f.WscalesE[0], f.Wfractions[0],f.WfractionsE[0], f.chi2[0],f.ndf[0] , f.nfits)
        s.scalekeys.append(key)
        return s.scale_sys(key,d)
    def find_sample(s,name,gname=None):
        for grp in s.elm:
            if gname!=None and grp.name!=gname: continue
            for sam in grp.samples:
                if sam.name==name:
                    return sam
        return None
    def histo(s,name,hname,d,norm=None):
        """ generic function to return histogram for a particular subsample """
        loop = [z for z in s.elm if z.name==name]
        return s.histosum(loop,hname,d,norm=norm)
    def asym_generic(s,method,hname,d,*args,**kwargs):
        """ Generic function that builds asymmetry for a given method """
        import SuCanvas
        print '--------->', 'asym_generic: preparing hPOS'
        sys.stdout.flush()
        hPOSs = method(hname+'_POS',d.clone(q=0),*args,**kwargs)
        print '--------->', 'asym_generic: preparing hNEG'
        sys.stdout.flush()
        hNEGs = method(hname+'_NEG',d.clone(q=1),*args,**kwargs)
        assert len(hPOSs.flat) == len(hNEGs.flat)
        for i,(hPOS,hNEG) in enumerate( zip(hPOSs.flat,hNEGs.flat) ):
            if not i in hPOSs.enable: continue
            if hPOS.h and hNEG.h:
                d.flat[i].h = SuCanvas.SuCanvas.WAsymmetry(hPOS.h,hNEG.h)
        return d
    def ratio_generic(s,method,hname,d,*args,**kwargs):
        """ Generic function that builds W+/W- ratio for a given method """
        import SuCanvas
        print '--------->', 'ratio_generic: preparing hPOS'
        sys.stdout.flush()
        hPOSs = method(hname+'_POS',d.clone(q=0),*args,**kwargs)
        print '--------->', 'ratio_generic: preparing hNEG'
        sys.stdout.flush()
        hNEGs = method(hname+'_NEG',d.clone(q=1),*args,**kwargs)
        assert len(hPOSs.flat) == len(hNEGs.flat)
        for i,(hPOS,hNEG) in enumerate( zip(hPOSs.flat,hNEGs.flat) ):
            if not i in hPOSs.enable: continue
            if hPOS.h and hNEG.h:
                d.flat[i].h = SuCanvas.SuCanvas.WRatio(hPOS.h,hNEG.h)
        return d
    def data(s,hname,d,leg=[],norm=None):
        """ data summed histogram """
        loop = [e for e in s.elm if 'data' in e.flags and 'no' not in e.flags]
        res = s.histosum(loop,hname,d,norm=norm)
        if type(leg)==type([]):
            leg.append( ['data','data 2011 (#sqrt{s} = 7 TeV)','LP'] )
        return res
    def asym_data(s,*args,**kwargs):
        return s.asym_generic(s.data,*args,**kwargs)
    def ratio_data(s,*args,**kwargs):
        return s.ratio_generic(s.data,*args,**kwargs)
    def data_sub(s,hname,d,norm=None):
        """ bg-subtracted data """
        loop1 = [e for e in s.elm if 'data' in e.flags and 'no' not in e.flags]
        weights = [1.0]*len(loop1)
        loop2 = [e for e in s.elm if 'mc' in e.flags and 'sig' not in e.flags and 'no' not in e.flags]
        weights+= [-1.0]*len(loop2)
        loop = loop1 + loop2
        res = s.histosum(loop,hname,d,weights=weights,norm=norm)
        return res
    def asym_data_sub(s,*args,**kwargs):
        return s.asym_generic(s.data_sub,*args,**kwargs)
    def ratio_data_sub(s,*args,**kwargs):
        return s.ratio_generic(s.data_sub,*args,**kwargs)
    def mc(s,hname,d,name=None,norm=None):
        """ MC summed histogram """
        if not name: # use flags to determine which MC to plot
            loop = [e for e in s.elm if 'mc' in e.flags and 'no' not in e.flags]
        else:         # manually specify what to plot
            loop = [e for e in s.elm if e.name==name]
        return s.histosum(loop,hname,d,norm=norm)
    def asym_mc(s,*args,**kwargs):
        return s.asym_generic(s.mc,*args,**kwargs)
    def ratio_mc(s,*args,**kwargs):
        return s.ratio_generic(s.mc,*args,**kwargs)
    def sig(s,hname,d,norm=None):
        """ signal MC summed histogram """
        loop = [e for e in s.elm if 'sig' in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d,norm=norm)
    def asym_sig(s,*args,**kwargs):
        return s.asym_generic(s.sig,*args,**kwargs)
    def ratio_sig(s,*args,**kwargs):
        return s.ratio_generic(s.sig,*args,**kwargs)
    def bg(s,hname,d,norm=None):
        """ background MC summed histogram """
        loop = [e for e in s.elm if 'mc' in e.flags and 'sig' not in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d,norm=norm)
    def ewk(s,hname,d,norm=None):
        """ EWK background summed histogram """
        loop = [e for e in s.elm if 'ewk' in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d,norm=norm)
    def qcd(s,hname,d,norm=None):
        """ QCD background summed histogram """
        loop = [e for e in s.elm if 'qcd' in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d,norm=norm)
    def SaveROOT(s,fname,d,flags1=['mc'],flags2=['data'],mode='UPDATE',dname=None):
        """ Saves a collection of plots (in d) into a ROOT file """
        ELES = """
        Attention: need to implement ResolDown. To think: decide how to implement bsgub with mc@nlo vs powhegHerwig.
        MuonResIDUp, MuonResIDDown, MuonResMSUp, MuonResMSDown, MuonScaleUp, MuonScaleDown, MuonTriggerSFUp, MuonTriggerSFDown, MuonRecoSFUp, MuonRecoSFDown, MuonIsoSFUp, MuonIsoSFDown
        JetResolUp, JetResolDown, JetScaleUp, JetScaleDown, ResoSoftTermsUp_ptHard, ResoSoftTermsDown_ptHard, ScaleSoftTermsUp_ptHard, ScaleSoftTermsDown_ptHard    
        """
        NMAP = {}
        NMAP['t#bar{t}'] = 'ttbar'
        NMAP['t#bar{t}+single-top'] = 'ttbar_stop'
        NMAP['single-top'] = 'stop'
        NMAP['wtaunu_powheg_pythia'] = 'wtaunu'
        NMAP['wtaunu_powheg_herwig'] = 'wtaunu'
        NMAP['wtaunu_mcnlo'] = 'wtaunu'
        NMAP['wtaunu_alpgen_herwig'] = 'wtaunu'
        NMAP['wtaunu_pythia'] = 'wtaunu'
        NMAP['zmumu_powheg_pythia'] = 'zmumu'
        NMAP['zmumu_powheg_herwig'] = 'zmumu'
        NMAP['zmumu_mcnlo'] = 'zmumu'
        NMAP['zmumu_alpgen_herwig'] = 'zmumu'
        NMAP['ztautau_powheg_pythia'] = 'ztautau'
        NMAP['ztautau_powheg_herwig'] = 'ztautau'
        NMAP['ztautau_mcnlo'] = 'ztautau'
        NMAP['ztautau_alpgen_herwig'] = 'ztautau'
        NMAP['ztautau_pythia'] = 'ztautau'
        NMAP['dyan_pythia'] = 'dyan'
        NMAP['dyan_mcnlo'] = 'dyan'
        NMAP['WW/WZ/ZZ'] = 'diboson'
        NMAP['qcd_driven'] = 'qcd'
        NMAP['qcd_driven_sub'] = 'qcd'
        NMAP['qcd_mc'] = 'qcd'
        NMAP['sig_pythia'] = 'wmunu'
        NMAP['sig_powheg_pythia'] = 'wmunu'
        NMAP['sig_powheg_herwig'] = 'wmunu'
        NMAP['sig_alpgen_herwig'] = 'wmunu'
        NMAP['sig_mcnlo'] = 'wmunu'
        NMAP['2011 data'] = 'data'
        # populate with data
        loop =  [e for e in s.elm if set(flags1) == (set(flags1) & set(e.flags)) and 'no' not in e.flags]
        loop += [e for e in s.elm if set(flags2) == (set(flags2) & set(e.flags)) and 'no' not in e.flags]
        hs = []
        for bg in loop:
            h = bg.histo('histo',d.clone())
            assert bg.name in NMAP, 'Cannog find %s in NMAP in SuData::SaveROOT'%(bg.name,)
            name = NMAP[bg.name]
            isys = 0
            for sgroups in h.sys:
                for sinst in sgroups:
                    isys += 1
                    assert sinst.name == sinst.name.strip(), 'FATAL: found a white space in systematic name: [%s]'%sinst.name
                    title = name+'_'+sinst.name
                    # skip rules
                    if name=='data':
                        if isys==1: title = name
                        else: continue
                    if name=='qcd':
                        if isys==1: title = name
                        else: continue
                    if sinst.name in ('qcdup','qcddown'): continue
                    hs.append( sinst.h )
                    assert hs[-1],'Failed to find systematic %s in sample %s'%(sinst.name,name)
                    hs[-1].SetName(title)
                    hs[-1].SetTitle(title)
        # save stuff
        fout = ROOT.TFile.Open(fname,mode)
        assert fout.IsOpen(),'Failed to open file: %s'%fname
        nom = d.nominal()
        aname = dname if dname else SuSys.QMAP[nom.charge][1]+'_'+MAP_BGSIG[s.flagsum['S']]
        adir = fout.Get(aname)
        if not adir:
            fout.mkdir(aname) # automatically recursive
            adir = fout.Get(aname)
        assert adir,'Failed to create subdirectory: %s'%aname
        adir.cd()
        [ h.Write(h.GetTitle(),ROOT.TObject.kOverwrite) for h in hs ]
        fout.Close()
    def stack(s,hname,d,flags=['mc'],leg=[]):
        """ MC histogram stack """
        # prepare containers for results
        res = d
        res.MakeStacks(hname)
        # populate with data
        loop = [e for e in s.elm if set(flags) == (set(flags) & set(e.flags)) and 'no' not in e.flags]
        for bg in loop:
            h = bg.histo(hname,d.clone())
            if h.any_h():
                res.AddStack(h)
                if type(leg)==type([]):
                    leg.append( [bg.name,bg.label,'F'] )
        return res
    def histosum(s,loop,hname,d,norm=None,weights=None):
        """ a wrapper around histosum_apply: allows reconstruction of histograms in eta slices """
        # regular histogram is easy, just return histosum_apply:
        if isinstance(d,SuSys) or not d.nominal().is_sliced():
            return s.histosum_apply(loop,hname,d,norm,weights)
        # histogram specified in several eta slices: we need to reconstruct & unfold in each eta slice, then re-build:
        if d.nominal().is_sliced_1d():
            assert False,'ERROR: need to update bin_ stuff to 2d/3d histos'
            hspec = d.nominal().histo    # bin_%d/lpt:imin:imax
            assert len(hspec.split(':'))==3
            horig = hspec.split(':')[0]  # bin_%d/lpt
            hname = horig.split('/')[1]  # lpt
            imin,imax = [int(cc) for cc in hspec.split(':')[1:1+2]]
            assert hname in ('lepton_pt',)
            # locate list of eta bins
            import binning
            bins = binning.setabins if re.search('bine_',horig) else binning.absetabins
            idxs = list(range(0,len(bins)-1))
            assert idxs[-1]==len(idxs)-1
            # locate heta [should always be able to find it]
            heta = None
            for ilp in loop:
                if heta: break
                for ismpl in ilp.samples:
                    if heta: break
                    hetaname = 'lepton_etav' if re.search('bine_',horig) else 'lepton_absetav'
                    heta = ismpl.get_from_file( d.nominal().h_path_folder() + '/' + hetaname )
                    if heta: break
            assert heta,'Cannot locate %s in any sample'%d.nominal().h_path_folder()
            heta = heta.Clone()
            heta.SetLineColor(ROOT.kBlack)
            heta.SetFillColor(ROOT.kBlack)
            heta.SetMarkerSize(0)
            # loop over each eta bin
            ds = []
            for idx in idxs:
                # update histo and unfhisto to bin_7/lpt
                hnew = horig%idx
                ds.append( s.histosum_apply(loop,hname,d.clone(histo=hnew,unfhisto=hnew,slice=idx),norm,weights) )
            # now we have final unfolded histograms (vs lpt) in each eta bin. Go back into eta space:
            d.update_from_slices(ds,heta,imin,imax)
            return d
        elif d.nominal().is_sliced_2d():
            assert False,'ERROR: need to update bin_ stuff to 2d/3d histos'
            hspec = d.nominal().histo    # d2_abseta_lpt:y:0:8
            assert len(hspec.split(':'))==4
            horig = hspec.split(':')[0]  # d2_abseta_lpt
            haxis = hspec.split(':')[1]  # y
            assert haxis in ('x','y')
            imin,imax = [int(cc) for cc in hspec.split(':')[2:2+2]]
            # locate list of eta bins
            import binning
            bins = binning.absetabins if re.search('abseta',horig) else binning.setabins
            idxs = list(range(0,len(bins)-1))
            # locate heta [should always be able to find it]
            heta = None
            for ilp in loop:
                if heta: break
                for ismpl in ilp.samples:
                    if heta: break
                    hetaname = 'lepton_absetav' if re.search('abseta',horig) else 'lepton_etav'
                    heta = ismpl.get_from_file( d.nominal().h_path_folder() + '/' + hetaname )
                    if heta: break
            assert heta,'Cannot locate %s in any sample'%d.nominal().h_path_folder()
            heta = heta.Clone()
            heta.SetLineColor(ROOT.kBlack)
            heta.SetFillColor(ROOT.kBlack)
            heta.SetMarkerSize(0)
            # loop over each eta bin
            ds = []
            for idx in idxs:
                # update histo and unfhisto to bin_7/lpt
                hnew = '%s:%s:%d:%d'%(horig,haxis,idx+1,idx+1)
                unfhisto = 'bin%s_%d/lepton_pt'%('' if re.search('abseta',horig) else 'e',idx)
                ds.append( s.histosum_apply(loop,hname,d.clone(histo=hnew,unfhisto=unfhisto,slice=idx),norm,weights) )
            # now we have final unfolded histograms (vs lpt) in each eta bin. Go back into eta space:
            d.update_from_slices(ds,heta,imin,imax)
            return d
    def histosum_apply(s,loop,hname,d,norm=None,weights=None):
        """ generic function to add up a subset of samples """
        if len(loop)==0:
            return None
        # weights for each sample being summed
        if not weights:
            weights = [1.0]*len(loop)
        assert len(weights)==len(loop)
        res = loop[0].histo(hname,d)
        for i,h in enumerate(loop[1:]) :
            htmp = h.histo(hname,d.clone())
            if res and not htmp:
                continue
            elif res and htmp:
                res.Add(htmp,weights[i+1])
            elif not res:
                res = htmp
        if res:
            res.SetName(hname)
            if norm:
                res.Unitize()
        return s.run_unfolding(res)
