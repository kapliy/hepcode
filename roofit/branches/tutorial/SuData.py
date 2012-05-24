#/usr/bin/env python

"""
A generalized class for histogram/ntuple data access
"""

import sys,os,math,re,glob
from hashlib import md5
import ROOT
from FileLock import FileLock

class SuSys:
    """ Generic class that describes where to get the data for one histogram (systematic).
    Basically, it generalizes ntuple-based and histo-based data access.
    def stack(s,hname,var,bin,cut,path=None,flags=['mc'],leg=None):
    def stack2(s,sysdir,subdir,basedir,charge,hname,qcd={},flags=['mc'],leg=None,unfold=None):
    """
    QMAP = { 0 : ('/POS','POS','(l_q>0)','mu+ only'), 1 : ('/NEG','NEG','(l_q<0)','mu- only'), 2 : ('','ALL','(l_q!=0)','mu+ and mu-') }
    def qlist(s,v):
        """ Assign a triplet of directory names: data,MC,QCD """
        if isinstance(v,list) or isinstance(v,tuple):
            assert len(v)==3
            return v
        else:
            return [v]*3
    def __init__(s,charge=2,qcd = {}, unfold=None,qcderr=0,
                 ntuple='w',path=None,var=None,bin=None,pre='',weight="mcw*puw*effw*trigw",
                 histo=None,sysdir=None,subdir=None,basedir=None ):
        # actual histograms
        s.h = None
        s.stack = None
        # Common resources:
        s.name = ''
        s.charge = charge
        s.unfold = unfold
        s.qcd = qcd       # special map to place ourselves into a QCD-region (e.g, anti-isolation)
        s.qcderr = qcderr # Controls QCD scale uncertainty: 1 = sigma_up, -1 = sigma_down, 0 = nominal 
        # Ntuple-based resources:
        s.ntuple = ntuple # w or z
        s.path = path
        s.var = var
        s.bin = bin
        s.pre = pre
        s.weight = weight
        # Histogram-based resources
        s.histo = histo
        # these path elements are organized into lists with: [ /in/data , /in/mc , /in/isofail ]
        s.sysdir = s.qlist(sysdir)
        s.subdir = s.qlist(subdir)
        s.basedir = s.qlist(basedir)
    def use_ntuple(s):
        """ True or False """
        return s.path and s.bin and s.pre
    def nt_prew(s):
        """ Returns a TTree::Draw cut string for w ntuple """
        assert s.var
        assert s.bin
        assert s.pre
        return '(%s)*(%s)*(%s)' % (SuSys.QMAP[s.charge][2] , s.pre, s.weight)
    def nt_prez(s):
        assert False,'Not implemented'
        return '(%s)*(%s)' % ( s.pre, s.weight )
    def h_path(s,i=0,flags=None):
        """ Returns the path to the histogram. Index i has the following meaning:
        i=0 - data
        i=1 - MC
        i=2 - data-driven QCD template
        """
        assert s.histo
        assert s.sysdir
        assert s.subdir
        assert s.basedir
        # bootstrap "i" from MC sample flags
        if flags:
            if 'data' in flags:
                i=0
            elif 'driven' in flags:
                i=2
            else:
                i=1
        hpath = os.path.join(s.sysdir[i],s.subdir[i],s.basedir[i]) + SuSys.QMAP[s.charge][0] + '/' + s.histo
        return hpath
    def h_path_fname(s,i=1):
        return s.sysdir[2]+'__'+s.sysdir[i] + '_' + s.subdir[i] + '_' + s.basedir[i] + '_' + SuSys.QMAP[s.charge][1] + '_' + s.histo
    def qcd_region(s):
        """ Puts this SuSys in qcd region based on qcd map """
        s.basedir = [s.qcd['metfit']]*3 # disable MET>25 cut
        s.histo = 'met'
    def clone(s,sysdir=None,sysdir_mc=None,subdir=None,subdir_mc=None,basedir=None,qcderr=None,name=None,q=None,histo=None):
        """ deep copy, also allowing to update some members on-the-fly (useful to spawn systematics) """
        import copy
        res =  copy.copy(s)
        # do not clone the actual histograms
        #res.h,res.stack = None,None
        # only replace in MC
        if sysdir_mc!=None: res.sysdir = s.qlist([res.sysdir[0],sysdir_mc,res.sysdir[2]])
        if subdir_mc!=None: res.subdir = s.qlist([res.subdir[0],subdir_mc,res.subdir[2]])
        # other replacements
        if sysdir!=None: res.sysdir = s.qlist(sysdir)
        if subdir!=None: res.subdir = s.qlist(subdir)
        if basedir!=None: res.basedir = s.qlist(basedir)
        if qcderr!=None: res.qcderr = qcderr
        if name!=None: res.name = name
        if histo!=None: res.histo = histo
        if q!=None: res.charge = q
        return res
    def Add(s,o,dd=1.0):
        return s.h.Add(o.h,dd) if s.h and o.h else None
    def AddStack(s,o):
        return s.stack.Add(o.h) if s.stack and o.h else None
    def SetName(s,n):
        return s.h.SetName(n) if s.h else None
    def SetLineColor(s,c):
        return s.h.SetLineColor(c) if s.h else None
    def SetFillColor(s,c):
        return s.h.SetFillColor(c) if s.h else None
    def SetMarkerSize(s,c):
        return s.h.SetMarkerSize(c) if s.h else None
    def Scale(s,c):
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
        s.status = 0
        s.groups = []   # flat list of groups - names of systematics
        s.sys = []      # nested list (groups->up/downs)
        s.flat = []     # flat list of all systematics
        s.enable = [0,] # flat list of enabled indices
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
    def Scale(s,v):
        """ either one constant, or a per-systematic tuple """
        if isinstance(v,list) or isinstance(v,tuple):
            return [ p1.Scale(v[i]) for i,p1 in enumerate(s.flat) if i in s.enable ]
        else:
            return [ p1.Scale(v) for i,p1 in enumerate(s.flat) if i in s.enable ]
    def Unitize(s):
        return [ p1.Unitize() for i,p1 in enumerate(s.flat) if i in s.enable ]
    def MakeStacks(s,hname):
        return [ p1.MakeStack(hname+'_'+p1.name) for i,p1 in enumerate(s.flat) if i in s.enable ]
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
    def nominal_h(s):
        return s.nominal().h
    def enable_name(s,name):
        idx = 0 
        for sgroups in s.sys:
            for sinst in sgroups:
                if sinst.name==sysname:
                    s.enable = [idx]
                    return
                idx+=1
        print 'WARNING: could not enable systematic:',name
    def enable_nominal(s):
        s.enable = [0,]
    def enable_all(s):
        s.enable = []
        idx = 0
        for ig,sgroups in enumerate(s.sys):
            for iss,sinst in enumerate(sgroups):
                s.enable.append(idx)
                idx += 1
    def qcd_region(s):
        """ Puts all SuSys in qcd region based on qcd map """
        res = s.clone()
        [o.qcd_region() for o in res.flat]
        res.status = 1 # to prevent infinite recursion
        return res
    def bootstrap(s,charge=2,qcd = {},unfold=None,qcderr=0,
                  ntuple='w',path=None,var=None,bin=None,pre='',weight="mcw*puw*effw*trigw",
                  histo=None,sysdir=None,subdir=None,basedir=None ):
        """ bootstraps a full collection of systematics from the nominal instance of SuSys """
        pre=''
        if sysdir and sysdir[0]=='tight_nominal':
            pre='tight_'
        # nominal first:
        nom = SuSys(charge=charge,qcd=qcd,unfold=unfold,qcderr=qcderr,
                    ntuple=ntuple,path=path,var=var,bin=bin,pre=pre,weight=weight,
                    histo=histo,sysdir=sysdir,subdir=subdir,basedir=basedir)
        s.sys = [ [nom,] ]
        s.flat = [ nom ]
        s.groups = [ 'nominal' ]
        # systematic variations
        print 'Creating systematic variations...'
        res = []
        def add(n,ss):
            """ Clones sysdir """
            res.append(nom.clone(name=n,sysdir_mc=ss))
            s.flat.append(res[-1])
        def add2(n,ss):
            """ Clones subdir (e.g., for efficiencies) """
            res.append(nom.clone(name=n,subdir_mc=ss))
            s.flat.append(res[-1])
        def add3(n,ss):
            """ Clones qcd normalization """
            res.append(nom.clone(name=n,qcderr=ss))
            s.flat.append(res[-1])
        def next(nn):
            s.sys.append(res[:])
            s.groups.append(nn)
            del res[:]
        # MCP smearing UP
        add('mcp_msup',pre+'mcp_msup')
        add('mcp_msdown',pre+'mcp_msdown')
        next('MCP_MS_RES')
        # MCP smearing DOWN
        add('mcp_idup',pre+'mcp_idup')
        add('mcp_iddown',pre+'mcp_iddown')
        next('MCP_ID_RES')
        # MCP scale
        if False: # old MCP scale recommendation: on/off
            add('mcp_unscaled',pre+'mcp_unscaled')
            next('MCP_SCALE')
        else:  # using my C/K variations
            add('mcp_kup',pre+'mcp_kup')
            add('mcp_kdown',pre+'mcp_kdown')
            next('MCP_KSCALE')
            add('mcp_cup',pre+'mcp_cup')
            add('mcp_cdown',pre+'mcp_cdown')
            next('MCP_CSCALE')
        # MCP efficiency
        add2('nominal_effstatup',pre+'st_w_effstatup')
        add2('nominal_effstatdown',pre+'st_w_effstatdown')
        next('MCP_EFFSTAT')
        add2('nominal_effsysup',pre+'st_w_effsysup')
        add2('nominal_effsysdown',pre+'st_w_effsysdown')
        next('MCP_EFFSYS')
        add2('nominal_trigstatup',pre+'st_w_trigstatup')
        add2('nominal_trigstatdown',pre+'st_w_trigstatdown')
        next('MCP_TRIG')
        # JET
        add('jet_jer',pre+'jet_jer')
        next('JER')
        add('jet_jesup',pre+'jet_jesup')
        add('jet_jesdown',pre+'jet_jesdown')
        next('JES')
        # MET
        add('met_allcluup',pre+'met_allcluup')
        add('met_allcludown',pre+'met_allcludown')
        next('MET')
        # QCD normalization
        add3('qcdup',1)
        add3('qcddown',-1)
        next('QCD_FRAC')
        assert len(s.sys)==len(s.groups)
        print 'Created systematic variations: N =',len(s.sys)
    def update_errors(s):
        """ folds systematic variations into total TH1 error  """
        stack_mode = False
        if not s.sys[0][0].h:
            assert s.sys[0][0].stack
            stack_mode = True
        s.hsys = s.sys[0][0].stack.GetStack().Last().Clone() if stack_mode else s.sys[0][0].h.Clone()
        i = 0
        for hss in s.sys[1:]:
            bdiffs = [[] for z in xrange(0,s.hsys.GetNbinsX()+2)]
            for hs in hss: # loop over systematics in this group
                for ibin in xrange(0,s.hsys.GetNbinsX()+2):
                    h = hs.stack.GetStack().Last() if stack_mode else hs.h
                    bdiffs[ibin].append ( abs(s.hsys.GetBinContent(ibin)-h.GetBinContent(ibin)) )
                i+=1
            for ibin in xrange(0,s.hsys.GetNbinsX()+2):
                olderr = s.hsys.GetBinError(ibin)
                newerr = max(bdiffs[ibin])
                #print 'SETTING ERROR:',ibin,olderr,newerr
                s.hsys.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + newerr*newerr))
        return s.hsys
    def summary_bin(s,b):
        """ Prints relative deviation of various systematics in a given bin """
        nom = s.nominal_h()
        for ig,hss in enumerate(s.sys[1:]):
            print s.groups[ig],':'
            for hs in hss: # loop over systematics in this group
                print '%s \t:\t %.2f' % (hs.name, 100.0*(hs.h.GetBinContent(b) - nom.GetBinContent(b))/nom.GetBinContent(b) if nom.GetBinContent(b) else 0 )
        pass
    def update_histo(s,histo):
        for o in s.flat:
            o.histo = histo
    def clone(s,q=None,enable=None,histo=None):
        """ Clones an entire SuPlot.
        Each SuSys is cloned individually to avoid soft pointer links
        """
        res = SuPlot()
        res.status = 0
        res.groups = s.groups
        res.sys = []
        res.flat = []
        res.enable = enable if enable else s.enable
        for sgroups in s.sys:
            bla = []
            for sinst in sgroups:
                bla.append(sinst.clone(q=q,histo=histo))
                res.flat.append(bla[-1])
            res.sys.append( bla )
        return res
        
class SuSample:
    """
    Encapsulates a single TChain
    For example: jimmy_wmunu_np0
    """
    GLOBAL_CACHE = 'cache.root' # if None, global cache is disabled
    cache = None
    rootpath = None
    lumi = None
    debug = False
    # for unfolding:
    funfold = {}
    def __init__(s,name):
        """ constructor """
        s.name = name
        # data sources
        s.nt = {}
        s.nevt = {}
        s.files = []
        s.flags = []   #general flags
        s.table = {}   #general settings map
        s._evcounts = ("nevts","nevts_eff","nevts_trig","nevts_efftrig","nevts_unw","nevts_mcw")
        s.seen_samples = []
        # volatile
        s.path = None  # current ntuple path
        # fast histogram cache
        s.data = {}
    @staticmethod
    def load_unfolding():
        """ Static function to load unfolding migration matrices """
        # make sure RooUnfold.so is loadable:
        from ROOT import RooUnfold
        hpythia = os.path.join(SuSample.rootpath,'unfold_pythia.root')
        halpgen = os.path.join(SuSample.rootpath,'unfold_alpgen.root')
        hpowheg = os.path.join(SuSample.rootpath,'unfold_powheg.root')
        hmcnlo  = os.path.join(SuSample.rootpath,'unfold_mcnlo.root')
        if os.path.exists(hpythia):
            SuSample.funfold['pythia'] = ROOT.TFile.Open(hpythia)
            print 'Found unfolding matrices for:','pythia'
        else:
            print 'Cannot find unfolding matrices for:','pythia',hpythia
        if os.path.exists(halpgen):
            SuSample.funfold['alpgen'] = ROOT.TFile.Open(halpgen)
            print 'Found unfolding matrices for:','alpgen'
        else:
            print 'Cannot find unfolding matrices for:','alpgen',halpgen
        if os.path.exists(hpowheg):
            SuSample.funfold['powheg'] = ROOT.TFile.Open(hpowheg)
            print 'Found unfolding matrices for:','powheg'
        else:
            print 'Cannot find unfolding matrices for:','powheg',hpowheg
        if os.path.exists(hmcnlo):
            SuSample.funfold['mcnlo'] = ROOT.TFile.Open(hmcnlo)
            print 'Found unfolding matrices for:','mcnlo'
        else:
            print 'Cannot find unfolding matrices for:','mcnlo',hmcnlo
    def addchain(s,path='st_w_final/ntuple'):
        """ add one of the TNtuples
        path excludes dg/
        """
        s.nt[path] = ROOT.TChain(path,path)
        s.nevt[path] = {}
        s.path = path
    def choose_evcount(s,cut):
        """ chooses the right nevents histogram depending on which scales were requested
        UPDATE: this should alwats be nevts - otherwise it would revert the scale correction
        """
        return 'nevts'
        return 'nevts_mcw' if 'nevts_mcw' in s.nevt[s.path].keys() else 'nevts_unw'
        effw = re.search('effw',cut)
        trigw = re.search('trigw',cut)
        if effw and not trigw:
            return 'nevts_eff'
        elif trigw and not effw:
            return 'nevts_trig'
        elif trigw and effw:
            return 'nevts_efftrig'
        return 'nevts'
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
                if SuSample.debug:
                    print 'Added %d file(s) to TChain [%s]: %s'%(nadd,nt.GetName(),file)
            # update total nevt counts for xsec normalization
            for ii,ev in enumerate(s._evcounts):
                hname = '%s/%s'%(s.topdir(f),ev)
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
    def scale(s,evcnt='nevts'):
        """ normalize MC histograms to luminosity """
        if not re.match('mc',s.name):
            return 1.0
        from MC import mc
        mrun = mc.match_sample(s.name)
        # This is legacy QCD scaling ( for ntuple-based analysis via renormalize() )
        # New histogram-based scaling is applied to the stack element (i.e., after summing SuSample's)
        qcdscale = 1.0
        if re.search('mu15x',s.name):
            qcdscale=s.qcdscale
            if qcdscale!=1.0:
                print 'MC QCD: extra scaling by %.3f'%qcdscale
        if mrun:
            xsec = mrun.xsec*mrun.filteff
            # Choose the right evcnt - depending on which scale factors were used (effw/trigw)
            nevents = s.nevt[s.path][evcnt]
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
                if SuSample.debug:
                    print 'GetHisto:: %s \t\t %s/%s'%(os.path.basename(f.GetName()),s.topdir(f),hpath)
                if not  f.Get('%s/%s'%(s.topdir(f),hpath)):
                    return None
                h = f.Get('%s/%s'%(s.topdir(f),hpath)).Clone(hname)
                h.Sumw2()
            else:
                h.Add( f.Get('%s/%s'%(s.topdir(f),hpath)) )
        return h
    def histo(s,hname,dall,rebin=1.0,unitize=False):
        """ A wrapper around histogram-based and ntuple-based histo accessors """
        if isinstance(dall,SuSys):
            dall.h = s.histo_h(hname,dall,rebin)
            return dall
        # This is where d splits into per-systematic loop
        # and calls histo_nt or histo_h on a particular SuSys
        for i,d in enumerate(dall.flat):
            if not i in dall.enable: continue
            if d.use_ntuple():
                d.h = s.histo_nt( hname,d.var,d.bin,d.weight,d.path,rebin )
            else:
                d.h = s.histo_h(hname,d,rebin)
        return dall
    def histo_nt(s,hname,var,bin,cut,path=None,rebin=1.0):
        """ retrieve a particular histogram from ntuple (with cache) """
        path = path if path else s.path
        key = (s.rootpath,s.name,path,var,bin,cut)
        if False:
            key_str = re.sub(r'[^\w]', '_', '_'.join(key))
        else:
            key_str_all =  '_'.join(key)
            key_str = md5(key_str_all).hexdigest()
        needs_saving = s.GLOBAL_CACHE
        if s.GLOBAL_CACHE and os.path.exists(s.GLOBAL_CACHE):
            #with FileLock(s.GLOBAL_CACHE):
            if True:
                s.cache = ROOT.TFile.Open(s.GLOBAL_CACHE,'READ')
                assert s.cache and s.cache.IsOpen()
                if s.cache.Get(key_str):
                    s.data[key]=s.cache.Get(key_str).Clone(hname)
                    s.data[key].SetDirectory(0)
                    needs_saving = False
                s.cache.Close()
                s.cache = None
        if not key in s.data:
            hname = hname + '_' + s.name
            print '--> creating %s'%hname
            # build from TNtuple
            s.nt[path].Draw('%s>>%s(%s)'%(var,hname,bin),cut,'goff')
            if not ROOT.gDirectory.Get(hname):
                return None
            s.data[key] = ROOT.gDirectory.Get(hname).Clone()
            if needs_saving:
                #print 'SAVING INTO CACHE:',key
                with FileLock(s.GLOBAL_CACHE):
                    s.cache = ROOT.TFile.Open(s.GLOBAL_CACHE,'UPDATE')
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
    def histo_h(s,hname,d,rebin=1.0):
        """ retrieve a particular histogram; path is re-built manually"""
        res = s.GetHisto(hname,d.h_path(flags=s.flags))
        if not res:
            print '->Missing histo:',hname, d.h_path(flags=s.flags)
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
    def __init__(s,label,samples,color=ROOT.kBlack,flags=[],table={},po=None):
        """ constructor """
        s.samples = [SuSample(a) for a in samples]
        s.label = label
        s.color = color
        s.flags = [a.lower() for a in flags]
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
        return [e.auto() for e in s.samples]
    def histo(s,hname,d,unitize=False,rebin=1.0):
        """ sum histogram of all subsamples (legacy version for ntuples) """
        if len(s.samples)==0:
            return None
        res = s.samples[0].histo(hname,d,rebin=rebin)
        for h in s.samples[1:]:
            htmp = h.histo(hname,d.clone(),rebin=rebin)
            if res and not htmp:
                continue
            elif res and htmp:
                res.Add(htmp)
            elif not res:
                res = htmp
        if res:
            res.SetLineColor(ROOT.kBlack)
            res.SetFillColor(s.color)
            res.SetMarkerSize(0)
            if unitize:
                res.Unitize()
            elif 'qcd' in s.flags and (isinstance(d,SuPlot) and d.status==0):
                scales = s.po.get_scales(d)
                res.Scale(scales)
        assert res,'Failed to create: ' + hname
        return res

class SuStack:
    """
    Provides top-level interface to retrieve stacked histograms for all data and BG
    Underlying caching is implemented in SuSample
    This class does not own the histograms it creates!
    """
    QCD_SYS_SCALES = False
    def __init__(s):
        """ constructor """
        # stack elements
        s.elm = []
        # garbage collector
        s.scales = {} # cache of QCD normalization scales
        s.gbg = []
        s.fits = {}
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
    def add(s,label,samples,color,flags=[],table={}):
        """ backward-compatible interface:
            po.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen)
        """
        if not isinstance(samples,list):
            samples = [samples,]
        s.elm.append( SuStackElm(label,samples,color,flags,table,po=s) )
    def get_scales(s,d):
        if not SuStack.QCD_SYS_SCALES:
            return s.get_scale(d.sys[0][0])
        res = []
        for ig,o in enumerate(d.flat):
            res.append(s.get_scale(o))
        return res
    def get_scale(s,d):
        """ A lookup cache of QCD scale values """
        # massage the path to put us in the QCD region
        d2 = d.clone()
        d2.qcd_region()
        key = d2.h_path_fname()
        if key in s.scales:
            return s.scales[key][0]*(1.0+d.qcderr*s.scales[key][1])
        import SuFit
        f = SuFit.SuFit()
        f.addFitVar( 'met', 0 , 100 , 'MET (GeV)' );
        # get histograms
        hdata   = s.data('data',d2).h
        hfixed = s.ewk('bgfixed',d2).h
        hfree = s.qcd('bgfree',d2).h
        assert hdata,'Failed to find data'
        assert hfixed,'Failed to find fixed backgrounds'
        assert hfree,'Failed to find free backgrounds'
        # run SuFit
        hdata.getLegendName = lambda : 'DATA'
        hfixed.getLegendName = lambda : 'EWK backgrounds'
        hfree.getLegendName = lambda : 'QCD'
        f.setDataBackgrounds(hdata,hfixed,hfree)
        f.doFit()
        tmp = f.drawFits(key)
        s.fits[key] = tmp[0]
        s.gbg.append((f,hdata,hfixed,hfree,tmp))
        s.scales[key] = (f.scales[0],f.scalesE[0])
        return s.scales[key][0]*(1.0+d.qcderr*s.scales[key][1])
    def run_unfolding(s,d):
        """ Unfolds SuSys histograms to particle level """
        # TODO FIXME
        pass
    def histo(s,label,hname,d,norm=None):
        """ generic function to return histogram for a particular subsample """
        loop = [z for z in s.elm if z.label==label]
        return s.histosum(loop,hname,d,norm)
    def histosum(s,loop,hname,d,norm=None):
        """ generic function to add up a subset of samples """
        if len(loop)==0:
            return None
        res = loop[0].histo(hname,d)
        for h in loop[1:] :
            htmp = h.histo(hname,d.clone())
            if res and not htmp:
                continue
            elif res and htmp:
                res.Add(htmp)
            elif not res:
                res = htmp
        if res:
            res.SetName(hname)
            res.SetMarkerSize(0)
            if norm:
                res.Unitize()
                #res.Scale(1/res.GetSumOfWeights())
        return res
    def asym_generic(s,method,hname,d,*args,**kwargs):
        """ Generic function that builds asymmetry for a given method """
        import SuCanvas
        hPOSs = method(hname+'_POS',d.clone(q=0),*args,**kwargs)
        hNEGs = method(hname+'_NEG',d.clone(q=1),*args,**kwargs)
        assert len(hPOSs.flat) == len(hNEGs.flat)
        for i,(hPOS,hNEG) in enumerate( zip(hPOSs.flat,hNEGs.flat) ):
            if hPOS.h and hNEG.h:
                d.flat[i].h = SuCanvas.SuCanvas.WAsymmetry(hPOS.h,hNEG.h)
        return d
    def data(s,hname,d,leg=None):
        """ data summed histogram """
        loop = [e for e in s.elm if 'data' in e.flags and 'no' not in e.flags]
        res = s.histosum(loop,hname,d)
        if leg:
            leg.AddEntry(res.nominal_h(),'Data(#int L dt = %.1f pb^{-1})'%(SuSample.lumi/1000.0),'LP')
        return res
    def asym_data(s,*args,**kwargs):
        return s.asym_generic(s.data,*args,**kwargs)
    def data_sub(s,hname,d):
        """ bg-subtracted data """
        hdata = s.data(hname,d.clone())
        hbg = s.bg(hname,d.clone())
        if hdata and hbg:
            hdata.Add(hbg,-1.0)
        return hdata
    def asym_data_sub(s,*args,**kwargs):
        return s.asym_generic(s.data_sub,*args,**kwargs)
    def mc(s,hname,d,label=None):
        """ MC summed histogram """
        if not label: # use flags to determine which MC to plot
            loop = [e for e in s.elm if 'mc' in e.flags and 'no' not in e.flags]
        else:         # manually specify what to plot
            loop = [e for e in s.elm if e.label==label]
        return s.histosum(loop,hname,d)
    def asym_mc(s,*args,**kwargs):
        return s.asym_generic(s.mc,*args,**kwargs)
    def sig(s,hname,d):
        """ signal MC summed histogram """
        loop = [e for e in s.elm if 'sig' in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d)
    def asym_sig(s,*args,**kwargs):
        return s.asym_generic(s.sig,*args,**kwargs)
    def bg(s,hname,d):
        """ background MC summed histogram """
        loop = [e for e in s.elm if 'mc' in e.flags and 'sig' not in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d)
    def ewk(s,hname,d):
        """ EWK background summed histogram """
        loop = [e for e in s.elm if 'ewk' in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d)
    def qcd(s,hname,d):
        """ QCD background summed histogram """
        loop = [e for e in s.elm if 'qcd' in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d)
    def stack_ewk(s,hname,d,leg=None):
        """ MC histogram stack for EWK backgrounds """
        return s.stack(hname,d,flags=['mc','ewk'],leg=leg)
    def stack(s,hname,d,flags=['mc'],leg=None):
        """ MC histogram stack (legacy, ntuple-based)"""
        # prepare containers for results
        res = d
        res.MakeStacks(hname)
        if leg:
            leg.SetFillStyle(1001)
            leg.SetFillColor(10)
        # populate with data
        loop = [e for e in s.elm if set(flags) == (set(flags) & set(e.flags)) and 'no' not in e.flags]
        #loop = [e for e in s.elm if 'mc' in e.flags]
        for bg in loop:
            h = bg.histo(hname,d.clone())
            if h:
                res.AddStack(h)
                if leg:
                    leg.AddEntry(h.nominal_h(),bg.label,'F')
        return res
