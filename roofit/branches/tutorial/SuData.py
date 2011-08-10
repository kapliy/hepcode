#/usr/bin/env python

"""
A generalized class to handle all data access matters
"""

import sys,os,math,re,glob
import ROOT

class SuSample:
    """
    Encapsulates a single TChain
    For example: jimmy_wmunu_np0
    """
    _evcounts = ("nevts","nevts_eff","nevts_trig","nevts_efftrig","nevts_unw")
    seen_samples = []
    rootpath = None
    lumi = None
    rebin = 1
    hsource = None  # source path to the histograms
    def __init__(s,name):
        """ constructor """
        s.name = name
        # data sources
        s.nt = {}
        s.nevt = {}
        s.files = []
        # volatile
        s.path = None  # current ntuple path
        # fast histogram cache
        s.data = {}
    def addchain(s,path='st_w_final/ntuple'):
        """ add one of the TNtuples """
        s.nt[path] = ROOT.TChain(path,path)
        s.path = path # excluding dg/
    def topdir(s,f):
        """ descend down the root file in case it's the output of single-file dgplot merge """
        topdir = 'dg'
        if not f.Get(topdir):
            ldirs = [z.GetName() for z in f.GetListOfKeys()]
            assert len(ldirs)==1, 'Expecting one folder here: '+' '.join(ldirs)
            topdir=ldirs[0]+'/dg'
            print 'Rewriting topdir from [/dg] to [%s]'%topdir
        return topdir
    def auto(s):
        """ add all files satisfying a glob pattern - using rootpath"""
        assert s.rootpath, 'Please set rootpath variable to use autoloading'
        pattern = os.path.join(s.rootpath,s.name,'root_%s*.root'%s.name)
        return s.addglob(pattern)
    def addglob(s,pattern):
        """ add all files satisfying a glob pattern """
        return s.add(glob.glob(pattern))
    def add(s,file):
        """ add a file or list of files to all declared TChain's """
        if type(file)==type([]) or type(file)==type(()):
            [s.add(f) for f in file]
            return
        else:
            s.files.append(ROOT.TFile.Open(file))
            f = s.files[-1]
            # add files to ntuple:
            for path,nt in s.nt.iteritems():
                nadd = nt.AddFile(file,ROOT.TChain.kBigNumber,s.topdir(f)+'/'+path)
                if not nadd>0:
                    print 'WARNING: failed to find chain %s in file %s'%(path,file)
                print 'Added %d file(s) to TChain: %s'%(nadd,file)
            # update total nevt counts for xsec normalization
            for ii,ev in enumerate(s._evcounts):
                hname = '%s/%s'%(s.topdir(f),ev)
                hist = f.Get(hname)
                n = 0
                if hist:
                    n = hist.GetEntries()
                    if ii==0:
                        print 'Number of GRL events:',n
                else:
                    print 'WARNING: failed to find object [%s] in file [%s]'%(hname,file)
                if ev in s.nevt:
                    s.nevt[ev] += n
                else:
                    s.nevt[ev] = n
            #f.Close()
    def nentries(s,path=None):
        """ compute number of entries in TNtuple """
        return s.nt[path].GetEntries() if path else s.nt[s.path].GetEntries()
    def scale(s,evcnt='nevts'):
        """ normalize MC histograms to luminosity """
        if not re.match('mc',s.name):
            return 1.0
        from MC import mc
        mrun = mc.match_sample(s.name)
        qcdscale = 1
        if False: # blow up QCD for debugging
            if re.search('mu15x',s.name):
                qcdscale=10
                assert False
        if mrun:
            xsec = mrun.xsec*mrun.filteff
            # TODO - choose the right evcnt depending on cut (effw/trigw)
            nevents = s.nevt[evcnt]
            sample = mrun.sample
            if sample not in s.seen_samples:
                print 'MC %s: \t xsec=%.1f (%.1f*%.1f) nb \t nevts=%d/%d'%(sample,xsec,mrun.xsec,mrun.filteff,nevents,mrun.nevents)
                s.seen_samples.append(sample)
            return 1.0/nevents*s.lumi*xsec*qcdscale
        print 'WARNING: unable to find scale for MC sample ',s.name
        return 1.0
    def GetHisto(s,hname,hpath):
        """ Retrieves a sum of histograms from a given file """
        h = None
        for i,f in enumerate(s.files):
            assert f.IsOpen()
            if not h:
                h = f.Get('%s/%s'%(s.topdir(f),hpath)).Clone(hname)
                h.Sumw2()
            else:
                h.Add( f.Get('%s/%s'%(s.topdir(),hpath)) )
        return h
    def histo(s,hname,var,bin,cut,path=None):
        """ retrieve a particular histogram from ntuple """
        path = path if path else s.path
        key = (path,var,bin,cut)
        if not key in s.data:
            hname = hname + '_' + s.name
            print '--> creating %s'%hname
            if s.hsource:
                # build from TH1
                def QAPP(path,iq):
                    POS,NEG,ALL=range(3)
                    QMAP = {}
                    QMAP[POS] = (0,'POS','(l_q>0)','mu+ only')
                    QMAP[NEG] = (1,'NEG','(l_q<0)','mu- only')
                    QMAP[ALL] = (2,'ALL','(l_q!=0)','mu+ and mu-')
                    htmp = path.split('/');
                    htmp.insert(-1,QMAP[iq][1])
                    return '/'.join(htmp) if iq in (0,1) else path
                hpath = QAPP(s.hsource,s.hcharge)
                s.data[key]=s.GetHisto(hname,hpath)
            else:
                # build from TNtuple
                s.nt[path].Draw('%s>>%s(%s)'%(var,hname,bin),cut,'goff')
                s.data[key] = ROOT.gDirectory.Get(hname).Clone()
        res = s.data[key].Clone()
        if s.lumi:
            res.Scale(s.scale())
        if s.rebin!=1:
            res.Rebin(s.rebin)
        if True:
            res.Sumw2()
        return res

class SuStackElm:
    """
    Encapsulates a set of SuSamples (to be drawn with one color)
    For example: jimmy_wmunu_np{0..5}
    """
    def __init__(s,label,samples,color=ROOT.kBlack,flags=[]):
        """ constructor """
        s.samples = [SuSample(a) for a in samples]
        s.label = label
        s.color = color
        s.flags = [a.lower() for a in flags]
    def addchain(s,path):
        """ add one of the TNtuples """
        return [e.addchain(path) for e in s.samples]
    def auto(s):
        """ add all files satisfying a glob pattern - using rootpath"""
        return [e.auto() for e in s.samples]
    def histo(s,hname,var,bin,cut,path=None):
        """ sum histogram of all subsamples """
        if len(s.samples)==0:
            return None
        res = s.samples[0].histo(hname,var,bin,cut,path)
        for h in s.samples[1:]:
            htmp = h.histo(hname,var,bin,cut,path)
            if not htmp:
                continue
            res.Add(htmp)
        res.SetLineColor(ROOT.kBlack)
        res.SetFillColor(s.color)
        res.SetMarkerSize(0)
        return res

class SuStack:
    """
    Provides top-level interface to retrieve stacked histograms for all data and BG
    Underlying caching is implemented in SuSample
    This class does not own the histograms it creates!
    """
    def __init__(s):
        """ constructor """
        # stack elements
        s.elm = []
    def addchain(s,path):
        """ add one of the TNtuples """
        return [e.addchain(path) for e in s.elm]
    def auto(s):
        """ add all files satisfying a glob pattern - using rootpath"""
        return [e.auto() for e in s.elm]
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
            print '==',elm.label,'=='
            for sam in elm.samples:
                print '--->',sam.name,sam.nentries()
                tot += sam.nentries()
        print 'Total:',tot
    def add(s,label,samples,color,flags=[]):
        """ backward-compatible interface:
            po.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen)
        """
        if not isinstance(samples,list):
            samples = [samples,]
        s.elm.append( SuStackElm(label,samples,color,flags) )
    def histosum(s,loop,hname,var,bin,cut,path=None):
        """ generic function to add up a subset of samples """
        if len(loop)==0:
            return None
        res = loop[0].histo(hname,var,bin,cut,path)
        for h in loop[1:] :
            htmp = h.histo(hname,var,bin,cut,path)
            if not htmp:
                continue
            res.Add(htmp)
        res.SetName(hname)
        res.SetMarkerSize(0)
        return res
    def data(s,hname,var,bin,cut,path=None,leg=None):
        """ data summed histogram """
        loop = [e for e in s.elm if 'data' in e.flags]
        res = s.histosum(loop,hname,var,bin,cut,path)
        if leg:
            leg.AddEntry(res,'Data(#int L dt = %.1f pb^{-1})'%(SuSample.lumi/1000.0),'LP')
        return res
    def data_sub(s,hname,var,bin,cut,path=None):
        """ bg-subtracted data """
        hdata = s.data(hname,var,bin,cut,path)
        hbg = s.bg(hname,var,bin,cut,path)
        if hdata and hbg:
            hdata.Add(hbg,-1.0)
        return hdata
    def mc(s,hname,var,bin,cut,path=None):
        """ MC summed histogram """
        loop = [e for e in s.elm if 'mc' in e.flags]
        return s.histosum(loop,hname,var,bin,cut,path)
    def sig(s,hname,var,bin,cut,path=None):
        """ signal MC summed histogram """
        loop = [e for e in s.elm if 'sig' in e.flags]
        return s.histosum(loop,hname,var,bin,cut,path)
    def bg(s,hname,var,bin,cut,path=None):
        """ background MC summed histogram """
        loop = [e for e in s.elm if 'mc' in e.flags and 'sig' not in e.flags]
        return s.histosum(loop,hname,var,bin,cut,path)
    def ewk(s,hname,var,bin,cut,path=None):
        """ EWK background summed histogram """
        loop = [e for e in s.elm if 'ewk' in e.flags]
        return s.histosum(loop,hname,var,bin,cut,path)
    def qcd(s,hname,var,bin,cut,path=None):
        """ QCD background summed histogram """
        loop = [e for e in s.elm if 'qcd' in e.flags]
        return s.histosum(loop,hname,var,bin,cut,path)
    def stack_ewk(s,hname,var,bin,cut,path=None):
        """ MC histogram stack for EWK backgrounds """
        return s.stack(hname,var,bin,cut,path,flags=['mc','ewk'])
    def stack(s,hname,var,bin,cut,path=None,flags=['mc'],leg=None):
        """ MC histogram stack """
        # prepare containers for results
        res = ROOT.THStack(var,var)
        if leg:
            leg.SetFillStyle(1001)
            leg.SetFillColor(10)
        # populate with data
        loop = [e for e in s.elm if set(flags) == (set(flags) & set(e.flags))]
        #loop = [e for e in s.elm if 'mc' in e.flags]
        for bg in loop:
            h = bg.histo(hname,var,bin,cut,path)
            res.Add(h)
            if leg:
                leg.AddEntry(h,bg.label,'F')
        return res
