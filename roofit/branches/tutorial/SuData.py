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
    def __init__(s,name='nominal',charge=2,qcd = {}, unfold={},qcderr=0,
                 ntuple='w',path=None,var=None,bin=None,pre='',weight="mcw*puw*effw*trigw",
                 histo=None,sysdir=None,subdir=None,basedir=None ):
        # actual histograms
        s.h = None
        s.stack = None
        # Common resources:
        s.name = name
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
        hpath = os.path.join(s.sysdir[i],s.subdir[i],s.basedir[i]) + SuSys.QMAP[s.charge][0]
        return hpath
    def h_path(s,i=0,flags=None):
        """ Returns the path to the histogram. Index i has the following meaning:
        i=0 - data
        i=1 - MC
        i=2 - data-driven QCD template
        """
        hpath = s.h_path_folder(i,flags)+ '/' + s.histo
        return hpath
    def h_path_fname(s,i=1):
        out = s.sysdir[2]+'__'+s.sysdir[i] + '_' + s.subdir[i] + '_' + s.basedir[i] + '_' + SuSys.QMAP[s.charge][1] + '_' + s.histo
        return re.sub(r'[^\w]', '_', out)
    def unfold_get_path(s,ibin=None,ivar=None):
        path = s.unfold['sysdir'] + SuSys.QMAP[s.charge][0] + '/'
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
        basedir=''
        if len(s.basedir[2].split('/'))>1:  # nominal/lpt2025/tight
            basedir = '/' + '/'.join( s.basedir[2].split('/')[1:]) # lpt2025/tight
        basedir = s.qcd['metfit']+basedir   # metfit/lpt2025/tight [/POS/absetav_histo]
        s.basedir = [basedir]*3 # disable MET>25 cut
        s.histo = 'met'
    def is_sliced(s):
        """ Returns True if s.histo represents a folder of histograms in eta-slices """
        return re.search('bin_',s.histo)
    def clone(s,sysdir=None,sysdir_mc=None,subdir=None,subdir_mc=None,basedir=None,
              qcderr=None,name=None,q=None,histo=None,unfold=None,unfdir=None):
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
        if unfold!=None:
            res.unfold = copy.copy(unfold)
        else:
            res.unfold = copy.copy(s.unfold)
        if unfdir!=None: res.unfold['sysdir'] = unfdir
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
    def ScaleOne(s,*args,**kwargs):
        """ In SuSys, this is just an alias for Scale() """
        return s.Scale(*args,**kwargs)
    def Scale(s,c):
        """ special treatment: if c is a list, then the scales are to be applied per-bin """
        if isinstance(c,list) or isinstance(c,tuple):
            assert len(c)==s.h.GetNbinsX()+2
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
        print '--------->', 'update_from_slices: starting working on',s.flat[0].histo #FIXME
        import SuCanvas
        # create final eta histogram for each systematic
        for i,dsys in enumerate(s.flat):
            if not i in s.enable: continue
            hpts = [d.flat[i].h for d in ds] # loop over slices
            assert all(hpts)
            print '--------->', 'update_from_slices: looping',dsys.histo,i #FIXME
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
        # This function is unused. Rather, we normally call qcd_region directly on individual SuSys's
        res = s.clone()
        [o.qcd_region() for i,o in enumerate(res.flat) if i in res.enable]
        res.status = 1 # to prevent infinite recursion
        return res
    def bootstrap(s,charge=2,qcd = {},unfold={},do_unfold=False,qcderr=0,
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
        s.do_unfold = do_unfold
        # systematic variations
        print 'Creating systematic variations...'
        res = []
        def add(n,ss,unfss=None):
            """ Clones sysdir """
            res.append(nom.clone(name=n,sysdir_mc=ss,unfdir=unfss if unfss!=None else ss))
            s.flat.append(res[-1])
        def add2(n,ss,unfss=None):
            """ Clones subdir (e.g., for efficiencies) """
            res.append(nom.clone(name=n,subdir_mc=ss,unfdir=unfss if unfss!=None else ss))
            s.flat.append(res[-1])
        def add3(n,ss,unfss):
            """ Clones qcd normalization """
            res.append(nom.clone(name=n,qcderr=ss,unfdir=unfss))
            s.flat.append(res[-1])
        def add4(n,ss,unfss):
            """ Clones with different MC unfolding matrix """
            if ss in nom.unfold['mc']: return
            res.append(nom.clone(name=n,unfdir=unfss))
            res[-1].unfold['mc'] = ss
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
        add2('effstatup','st_w_effstatup',pre+'nominal_effstatup')
        add2('effstatdown','st_w_effstatdown',pre+'nominal_effstatdown')
        next('MCP_EFFSTAT')
        add2('effsysup','st_w_effsysup',pre+'nominal_effsysup')
        add2('effsysdown','st_w_effsysdown',pre+'nominal_effsysdown')
        next('MCP_EFFSYS')
        add2('trigstatup','st_w_trigstatup',pre+'nominal_trigstatup')
        add2('trigstatdown','st_w_trigstatdown',pre+'nominal_trigstatdown')
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
        add3('qcdup',1,pre+'nominal')
        add3('qcddown',-1,pre+'nominal')
        next('QCD_FRAC')
        # unfolding systematic
        if 'mc' in nom.unfold:
            add4('unfold_pythia','pythia',pre+'nominal')
            add4('unfold_alpgen','alpgen',pre+'nominal')
            add4('unfold_powheg','powheg',pre+'nominal')
            add4('unfold_mcnlo','mcnlo',pre+'nominal')
            next('UNFOLDING')
        assert len(s.sys)==len(s.groups)
        print 'Created systematic variations: N =',len(s.sys)
    def update_errors(s,sysonly=False):
        """ folds systematic variations into total TH1 error  """
        stack_mode = False
        if not s.sys[0][0].h:
            assert s.sys[0][0].stack
            stack_mode = True
        s.htot = s.sys[0][0].stack.GetStack().Last().Clone() if stack_mode else s.sys[0][0].h.Clone()
        s.hsys = s.htot.Clone()
        [s.hsys.SetBinError(ii,0) for ii in xrange(0,s.hsys.GetNbinsX()+2)]
        i = len(s.sys[0])
        for hss in s.sys[1:]:
            bdiffs = [[] for z in xrange(0,s.hsys.GetNbinsX()+2)]
            for hs in hss: # loop over systematics in this group
                if not i in s.enable:
                    i+=1
                    continue
                for ibin in xrange(0,s.hsys.GetNbinsX()+2):
                    h = hs.stack.GetStack().Last() if stack_mode else hs.h
                    bdiffs[ibin].append ( abs(s.hsys.GetBinContent(ibin)-h.GetBinContent(ibin)) )
                i+=1
            for ibin in xrange(0,s.hsys.GetNbinsX()+2):
                newerr = max(bdiffs[ibin]) if len(bdiffs[ibin])>0 else 0
                # hsys
                olderr = s.hsys.GetBinError(ibin)
                s.hsys.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + 1.0*newerr*newerr))
                # htot
                olderr = s.htot.GetBinError(ibin)
                s.htot.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + 1.0*newerr*newerr))
        return s.hsys if sysonly else s.htot
    def summary_bin(s,b):
        """ Prints relative deviation of various systematics in a given bin """
        nom = s.nominal_h()
        for ig,hss in enumerate(s.sys[1:]):
            print s.groups[ig+1],':'
            for hs in hss: # loop over systematics in this group
                print '%s \t\t:\t\t %.2f%%' % (hs.name, 100.0*(hs.h.GetBinContent(b) - nom.GetBinContent(b))/nom.GetBinContent(b) if nom.GetBinContent(b) else 0 )
        pass
    def update_histo(s,histo):
        for i,o in enumerate(s.flat):
            if i in s.enable:
                o.histo = histo
    def clone(s,q=None,enable=None,histo=None,do_unfold=None):
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
    def get_from_file(s,hpath,i=0):
        """ Returns TFile::Get(hpath) from i'th file """
        assert i<len(s.files)
        return s.files[i].Get( s.topdir(s.files[i])+'/'+hpath )
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
            fname = os.path.basename(s.files[0].GetName())
            print '->Missing histo:',hname, fname, d.h_path(flags=s.flags)
            #return None   # be careful, returning None is a recipe for not noticing problems
            assert False
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
                print '--------->', 'qcd scaling start:',hname #FIXME
                scales = s.po.get_scales(d)
                print '--------->', 'qcd scaling applying:',hname #FIXME
                if SuStack.QCD_SYS_SCALES:
                    res.Scale(scales)
                else:
                    res.ScaleOne(scales)
                print '--------->', 'qcd scaling end:',hname #FIXME
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
        # for unfolding:
        s.funfold = {}
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
    def load_unfolding(s):
        """ Loads unfolding migration matrices """
        # make sure RooUnfold.so is loadable:
        from ROOT import RooUnfold
        hpythia = os.path.join(SuSample.rootpath,'unfold_pythia.root')
        halpgen = os.path.join(SuSample.rootpath,'unfold_alpgen.root')
        hpowheg = os.path.join(SuSample.rootpath,'unfold_powheg.root')
        hmcnlo  = os.path.join(SuSample.rootpath,'unfold_mcnlo.root')
        if os.path.exists(hpythia):
            s.funfold['pythia'] = ROOT.TFile.Open(hpythia)
            print 'Found unfolding matrices for:','pythia'
        else:
            print 'Cannot find unfolding matrices for:','pythia',hpythia
        if os.path.exists(halpgen):
            s.funfold['alpgen'] = ROOT.TFile.Open(halpgen)
            print 'Found unfolding matrices for:','alpgen'
        else:
            print 'Cannot find unfolding matrices for:','alpgen',halpgen
        if os.path.exists(hpowheg):
            s.funfold['powheg'] = ROOT.TFile.Open(hpowheg)
            print 'Found unfolding matrices for:','powheg'
        else:
            print 'Cannot find unfolding matrices for:','powheg',hpowheg
        if os.path.exists(hmcnlo):
            s.funfold['mcnlo'] = ROOT.TFile.Open(hmcnlo)
            print 'Found unfolding matrices for:','mcnlo'
        else:
            print 'Cannot find unfolding matrices for:','mcnlo',hmcnlo
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
            print 'Unfolding method:',d.name,d.unfold['mc'] # FIXME AK
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
            return s.get_scale_wrap(d.sys[0][0])
        res = []
        for i,o in enumerate(d.flat):
            if i not in d.enable: continue
            res.append(s.get_scale_wrap(o))
        return res
    def get_scale_wrap(s,d):
        """ A wrapper around get_scale() that knows how to deal with bin_%d/lpt sliced histograms
        Note: d is an instance of SuSys, not SuPlot
        """
        if d.is_sliced():
            # retrieve an instance of the sliced histogram (vs lpt) to bootstrap the binning
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
                tname = '%s%s%s'%(os.path.basename(d.h_path()),tmin,tmax) #lpt2025
                # modify basedir to become metfit/lpt2025
                dtmp = d.clone()
                dtmp.basedir = dtmp.qlist( d.basedir[2] + '/' + tname )   # nominal/lpt2025
                res.append(s.get_scale(dtmp))
            # duplicate first/last bin scales for underflow/overflow scale
            return [res[0],] + res + [res[-1],]
        return s.get_scale(d)
    def get_scale(s,d):
        """ A lookup cache of QCD scale values
        Note: d is an instance of SuSys, not SuPlot
        """
        # massage the path to put us in the QCD region
        d2 = d.clone()
        d2.qcd_region()
        key = d2.h_path_fname()
        if key in s.scales:
            return s.scales[key][0]*(1.0+d.qcderr*s.scales[key][1])
        print 'COMPUTING NEW QCD WEIGHT:',key
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
    def histo(s,label,hname,d,norm=None):
        """ generic function to return histogram for a particular subsample """
        loop = [z for z in s.elm if z.label==label]
        return s.histosum(loop,hname,d,norm)
    def asym_generic(s,method,hname,d,*args,**kwargs):
        """ Generic function that builds asymmetry for a given method """
        import SuCanvas
        print '--------->', 'asym_generic: preparing hPOS' #FIXME
        hPOSs = method(hname+'_POS',d.clone(q=0),*args,**kwargs)
        print '--------->', 'asym_generic: preparing hNEG' #FIXME
        hNEGs = method(hname+'_NEG',d.clone(q=1),*args,**kwargs)
        assert len(hPOSs.flat) == len(hNEGs.flat)
        for i,(hPOS,hNEG) in enumerate( zip(hPOSs.flat,hNEGs.flat) ):
            if not i in hPOSs.enable: continue
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
        loop1 = [e for e in s.elm if 'data' in e.flags and 'no' not in e.flags]
        weights = [1.0]*len(loop1)
        loop2 = [e for e in s.elm if 'mc' in e.flags and 'sig' not in e.flags and 'no' not in e.flags]
        weights+= [-1.0]*len(loop2)
        loop = loop1 + loop2
        res = s.histosum(loop,hname,d,weights=weights)
        return res
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
    def histosum(s,loop,hname,d,norm=None,weights=None):
        """ a wrapper around histosum_apply: allows reconstruction of histograms in eta slices """
        # regular histogram is easy, just return histosum_apply:
        if isinstance(d,SuSys) or not d.nominal().is_sliced():
            return s.histosum_apply(loop,hname,d,norm,weights)
        # histogram specified in several eta slices: we need to reconstruct & unfold in each eta slice, then re-build:
        hspec = d.nominal().histo    # bin_%d/lpt:imin:imax
        assert len(hspec.split(':'))==3
        horig = hspec.split(':')[0]  # bin_%d/lpt
        hname = horig.split('/')[1]  # lpt
        imin,imax = [int(cc) for cc in hspec.split(':')[1:1+2]]
        assert hname in ('lpt','met','nj25','nj30','wmt','wpt')
        hdir = loop[0].samples[0].get_from_file( d.nominal().h_path_folder() )
        assert hdir
        allkeys = [z.GetName() for z in hdir.GetListOfKeys() if re.match('bin_',z.GetName())]
        idxs = sorted([ int(z.replace('bin_','')) for z in allkeys ])
        assert idxs[-1]==len(idxs)-1
        heta = loop[0].samples[0].get_from_file( d.nominal().h_path_folder() + '/' + 'lepton_absetav' )
        assert heta
        heta = heta.Clone()
        heta.SetLineColor(ROOT.kBlack)
        heta.SetFillColor(ROOT.kBlack)
        heta.SetMarkerSize(0)
        # loop over each eta bin
        ds = []
        for idx in idxs:
            ds.append( s.histosum_apply(loop,hname,d.clone(histo=(horig%idx)),norm,weights) )
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
            res.SetMarkerSize(0)
            if norm:
                res.Unitize()
                #res.Scale(1/res.GetSumOfWeights())
        return s.run_unfolding(res)
