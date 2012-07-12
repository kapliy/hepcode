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
MAP_BGQCD = {0:'mc',1:'bb',2:'JX',3:'driven'}

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
    def __init__(s,name='nominal',charge=2,qcd = {}, unfold={},qcderr='NOM',slice=None,
                 ntuple=None,path=None,var=None,bin=None,pre='',weight="mcw*puw*effw*trigw",
                 histo=None,sysdir=None,subdir=None,basedir=None ):
        # actual histograms
        s.h = None
        s.stack = None
        # Common resources:
        s.name = name
        s.charge = charge
        s.unfold = unfold
        s.qcd = qcd       # special map to place ourselves into a QCD-region (e.g, anti-isolation)
        s.qcderr = qcderr # controls QCD scale uncertainty: 'NOM','UP','DOWN',<float> - scale factor on nominal (e.g, 0.5 for 50%)
        s.slice = slice   # if not None, this says this SuSys refers to a particular |eta| slice
        # Ntuple-based resources:
        s.ntuple = ntuple # w or z
        s.path = path
        s.var = var
        s.bin = bin
        s.pre = s.qlist(pre)
        s.weight = weight
        # Histogram-based resources
        s.histo = histo
        # these path elements are organized into lists with: [ /in/data , /in/mc , /in/isofail ]
        s.sysdir = s.qlist(sysdir)
        s.subdir = s.qlist(subdir)
        s.basedir = s.qlist(basedir)
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
        return '(%s)*(%s)*(%s)' % (SuSys.QMAP[s.charge][2] , s.pre[i], s.weight)
    def nt_prez(s,i=0,flags=None):
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
        #isowind/st_w_final/metfit/bin_0/lpt_0/POS => #isowind/st_w_final/metfit/POS/bin_0/lpt_0
        hpath = os.path.join(s.sysdir[i],s.subdir[i],s.basedir[i]) + SuSys.QMAP[s.charge][0]
        if re.search('/bin_',hpath):
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
    def h_path_fname(s,i=1):
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
            return
        basedir=''
        if len(s.basedir[2].split('/'))>1:  # nominal/lpt2025/tight
            basedir = '/' + '/'.join( s.basedir[2].split('/')[1:]) # lpt2025/tight
        basedir = s.qcd['metfit']+basedir   # metfit/lpt2025/tight [/POS/absetav_histo]
        if s.qcd['var'][-3:]=='met': # only disable MET>25 cut if we are actually fitting in MET
            s.basedir = [basedir]*3
        s.histo = s.qcd['var']
        return
    def is_sliced(s):
        """ Returns True if s.histo represents a folder of histograms in eta-slices """
        return re.search('bin_',s.histo) if s.histo else False
    def clone(s,sysdir=None,sysdir_mc=None,subdir=None,subdir_mc=None,basedir=None,
              qcderr=None,qcdadd=None,name=None,q=None,histo=None,unfold=None,unfdir=None,unfhisto=None,
              ntuple=None,path=None,var=None,bin=None,pre=None,weight=None,
              slice=None):
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
        if qcdadd!=None: # addition to qcd map for fine-control of QCD fits
            qcd = copy.copy(res.qcd)
            qcd.update(qcdadd)
            res.qcd = qcd
        if name!=None: res.name = name
        if histo!=None: res.histo = histo
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
        return res
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
        print '--------->', 'update_from_slices: started working on',s.flat[0].histo
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
        assert False, "This function is unused. Rather, we normally call qcd_region directly on individual SuSys's"
        res = s.clone()
        [o.qcd_region() for i,o in enumerate(res.flat) if i in res.enable]
        res.status = 1 # to prevent infinite recursion
        return res
    def bootstrap(s,charge=2,qcd = {},unfold={},do_unfold=False,qcderr='NOM',
                  ntuple=None,path=None,var=None,bin=None,pre='',weight="mcw*puw*effw*trigw",
                  histo=None,sysdir=None,subdir=None,basedir=None ):
        """ bootstraps a full collection of systematics from the nominal instance of SuSys """
        prep=''
        if sysdir and sysdir[0]=='tight_nominal':
            prep='tight_'
        # nominal first:
        nom = SuSys(charge=charge,qcd=qcd,unfold=unfold,qcderr=qcderr,
                    ntuple=ntuple,path=path,var=var,bin=bin,pre=pre,weight=weight,
                    histo=histo,sysdir=sysdir,subdir=subdir,basedir=basedir)
        s.sys = [ [nom,] ]
        s.flat = [ nom ]
        s.groups = [ 'nominal' ]
        s.do_unfold = do_unfold
        # if this is using the ntuple, we don't have all systematics
        if nom.use_ntuple():
            print 'Created ntuple-based SuPlot'
            return
        # systematic variations
        res = []
        def add(n,ss,unfss=None,qcdadd=None):
            """ Clones sysdir """
            res.append(nom.clone(name=n,sysdir_mc=ss,qcdadd=qcdadd,unfdir=unfss if unfss!=None else ss))
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
        qcdadd = {'forcenominal':True}
        # MCP smearing UP
        add('mcp_msup',prep+'mcp_msup')
        add('mcp_msdown',prep+'mcp_msdown')
        next('MCP_MS_RES')
        # MCP smearing DOWN
        add('mcp_idup',prep+'mcp_idup')
        add('mcp_iddown',prep+'mcp_iddown')
        next('MCP_ID_RES')
        # MCP scale
        if False: # old MCP scale recommendation: on/off
            add('mcp_unscaled',prep+'mcp_unscaled')
            next('MCP_SCALE')
        else:  # using my C/K variations
            add('mcp_kup',prep+'mcp_kup')
            add('mcp_kdown',prep+'mcp_kdown')
            next('MCP_KSCALE')
            add('mcp_cup',prep+'mcp_cup')
            add('mcp_cdown',prep+'mcp_cdown')
            next('MCP_CSCALE')
        # MCP efficiency
        add2('effstatup','st_w_effstatup',prep+'nominal_effstatup')
        add2('effstatdown','st_w_effstatdown',prep+'nominal_effstatdown')
        next('MCP_EFFSTAT')
        add2('effsysup','st_w_effsysup',prep+'nominal_effsysup')
        add2('effsysdown','st_w_effsysdown',prep+'nominal_effsysdown')
        next('MCP_EFFSYS')
        add2('trigstatup','st_w_trigstatup',prep+'nominal_trigstatup')
        add2('trigstatdown','st_w_trigstatdown',prep+'nominal_trigstatdown')
        next('MCP_TRIG')
        # JET
        if True:
            #add('jet_jer',prep+'jet_jer',qcdadd=qcdadd)
            add('jet_jer',prep+'jet_jer')
            next('JER')
            add('jet_jesup',prep+'jet_jesup')
            add('jet_jesdown',prep+'jet_jesdown')
            next('JES')
        # MET
        if True:
            add('met_resosoftup',prep+'met_resosoftup')
            add('met_resosoftdown',prep+'met_resosoftdown')
            next('MET_RESO')
            add('met_scalesoftup',prep+'met_scalesoftup')
            add('met_scalesoftdown',prep+'met_scalesoftdown')
            next('MET_SCALE')
        else:  # new recommended MET systematic
            add('met_resosofttermsup',prep+'met_resosofttermsup')
            add('met_resosofttermsdown',prep+'met_resosofttermsdown')
            next('MET_RESO_COR')
            add('met_resosofttermsupdown',prep+'met_resosofttermsupdown')
            add('met_resosofttermsdownup',prep+'met_resosofttermsdownup')
            next('MET_RESO_ACOR')
            add('met_scalesofttermsup',prep+'met_scalesofttermsup')
            add('met_scalesofttermsdown',prep+'met_scalesofttermsdown')
            next('MET_SCALE')
        # QCD normalization
        if True:
            add3('qcdup',1.5,prep+'nominal')
            add3('qcddown',0.5,prep+'nominal')
            next('QCD_FRAC')
        # unfolding systematic
        if 'mc' in nom.unfold:
            add4('unfold_pythia','pythia',prep+'nominal')
            add4('unfold_mcnlo','mcnlo',prep+'nominal')
            add4('unfold_alpgen_herwig','alpgen_herwig',prep+'nominal')
            add4('unfold_powheg_pythia','powheg_pythia',prep+'nominal')
            next('UNFOLDING')
        assert len(s.sys)==len(s.groups)
        print 'Created systematic variations: N =',len(s.sys)
    def update_errors(s,sysonly=False,force=False):
        """ folds systematic variations into total TH1 error. TODO: independent two-sided variations (non-symmetrized)  """
        if s.hsys and sysonly==True and not force: return s.hsys
        if s.htot and sysonly==False and not force: return s.htot
        stack_mode = False
        if not s.sys[0][0].h:
            assert s.sys[0][0].stack
            stack_mode = True
        s.htot = s.sys[0][0].stack.GetStack().Last().Clone() if stack_mode else s.sys[0][0].h.Clone()
        s.htot.Sumw2()
        s.hsys = s.htot.Clone()
        [s.hsys.SetBinError(ii,0) for ii in xrange(0,s.hsys.GetNbinsX()+2)]
        i = len(s.sys[0])
        for hss in s.sys[1:]:
            bdiffs = [[] for z in xrange(0,s.hsys.GetNbinsX()+2)]
            for hs in hss: # loop over systematics in this group
                if not i in s.enable:
                    i+=1
                    continue
                h = hs.stack.GetStack().Last() if stack_mode else hs.h
                for ibin in xrange(0,s.hsys.GetNbinsX()+2):
                    bdiffs[ibin].append ( abs(s.hsys.GetBinContent(ibin)-h.GetBinContent(ibin)) )
                i+=1
            for ibin in xrange(0,s.hsys.GetNbinsX()+2):
                newerr = max(bdiffs[ibin]) if len(bdiffs[ibin])>0 else 0
                if False and ibin==4: #DEBUG
                    print i,'%.1f'%s.sys[0][0].stack.GetStack().Last().GetBinError(ibin),['%.1f'%zz for zz in bdiffs[ibin]],'%.1f'%newerr,'%.1f'%s.htot.GetBinError(ibin),'%.1f'%s.hsys.GetBinError(ibin)
                # hsys
                olderr = s.hsys.GetBinError(ibin)
                s.hsys.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + 1.0*newerr*newerr))
                # htot
                olderr = s.htot.GetBinError(ibin)
                s.htot.SetBinError(ibin,1.0*math.sqrt(olderr*olderr + 1.0*newerr*newerr))
        return s.hsys if sysonly else s.htot
    def summary_bin_txt(s,b):
        """ Prints relative deviation of various systematics in a given bin """
        nom = s.nominal_h()
        for ig,hss in enumerate(s.sys[1:]):
            print s.groups[ig+1],':'
            for hs in hss: # loop over systematics in this group
                print '%s \t\t:\t\t %.2f%%' % (hs.name, 100.0*(hs.h.GetBinContent(b) - nom.GetBinContent(b))/nom.GetBinContent(b) if nom.GetBinContent(b) else 0 )
        pass
    def summary_bin(s,b=None,fname=None):
        """ Prints relative deviation of various systematics in a given bin, compared with the statistical uncertainty """
        nom = s.nominal_h()
        bins = b if b else range(1,nom.GetNbinsX()+1)
        oldsys,f = sys.stdout,None
        if fname:
            f = open(fname,'w')
            sys.stdout = f
        print '<HTML><BODY>'
        print '<TABLE border="1">'
        print '<thead><tr>'
        print '<td width="100">|eta| bin</td>'
        for b in bins:
            tmin = nom.GetBinLowEdge(b)
            tmax = tmin + nom.GetBinWidth(b)
            print '<td width="50">%.1f..%.1f</td>'%(tmin,tmax)
        print '</tr></thead>'
        # statistical errors (for reference)
        print '<TR>'
        print '<TD>stat. err</TD>'
        for b in bins:
            err  = 100.0*(nom.GetBinError(b))/nom.GetBinContent(b) if nom.GetBinContent(b) else -999
            print '<TD>%.2f</TD>'%(err)
        print '</TR>'
        for ig,hss in enumerate(s.sys[1:]):
            print '<TR><TD colspan="%d">'%(len(bins)+1)
            print s.groups[ig+1]
            print '</TD></TR>'
            for hs in hss:
                name = hs.name
                print '<TR>'
                print '<TD>%s</TD>'%name
                for b in bins:
                    diff = 100.0*(hs.h.GetBinContent(b) - nom.GetBinContent(b))/nom.GetBinContent(b) if nom.GetBinContent(b) else -999
                    print '<TD>%.2f</TD>'%diff
                print '</TR>'
        print '</TABLE>'
        print '</BODY></HTML>'
        sys.stdout = oldsys
        if f: f.close()
        #print 'http://rendera.heroku.com/'
    def update_var(s,histo,bin=None):
        """ Updates histogram name and ntuple-expression (aka variable)
        In reality, only one of these applies for a given SuData instance.
        """
        for i,o in enumerate(s.flat):
            if i in s.enable:
                if bin:
                    o.var = histo
                    o.bin = bin
                else:
                    o.histo = histo
    def clone(s,q=None,enable=None,histo=None,do_unfold=None,unfhisto=None,qcdadd=None,
              sysdir=None,
              slice=None,
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
                bla.append(sinst.clone(q=q,histo=histo,unfhisto=unfhisto,qcdadd=qcdadd,sysdir=sysdir,ntuple=ntuple,path=path,var=var,bin=bin,pre=pre,weight=weight,slice=slice))
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
        s.missing = {} # keep track of missing histo printouts
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
        if h:
            h.SetTitle(os.path.basename(s.files[0].GetName())+' : '+hname+" "+hpath)
        return h
    @staticmethod
    def make_habseta(name='habseta_template'):
        """ makes an instance of abseta histogram with detector-motivated binning """
        import array
        bins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]
        a = array.array('f',bins)
        return ROOT.TH1F(name,name,len(a)-1,a)
    def histo(s,hname,dall,rebin=1.0,unitize=False):
        """ A wrapper around histogram-based and ntuple-based histo accessors """
        if isinstance(dall,SuSys):  # QCD fits
            d = dall
            if d.use_ntuple():
                d.h = s.histo_nt(hname,d.var,d.bin,d.nt_prew(flags=s.flags),path=d.path,rebin=rebin,hsource=d.histo)
            else:
                d.h = s.histo_h(hname,d,rebin)
            return dall
        # This is where d splits into per-systematic loop
        # and calls histo_nt or histo_h on a particular SuSys
        for i,d in enumerate(dall.flat):
            if not i in dall.enable: continue
            if d.use_ntuple():
                d.h = s.histo_nt(hname,d.var,d.bin,d.nt_prew(flags=s.flags),path=d.path,rebin=rebin,hsource=d.histo)
            else:
                d.h = s.histo_h(hname,d,rebin)
        return dall
    def histo_nt(s,hname,var,bin,cut,path=None,hsource=None,rebin=1.0):
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
                    if SuSample.debug==True:
                        print 'TTree::Cache :',os.path.basename(s.files[0].GetName()),var,'|',bin,'|',cut
                    s.data[key]=s.cache.Get(key_str).Clone(hname)
                    s.data[key].SetDirectory(0)
                    needs_saving = False
                s.cache.Close()
                s.cache = None
        if not key in s.data:
            hname = hname + '_' + s.name
            usebin,xtra = True,''
            # special handling to create variable-bin eta histograms:
            if hsource=='lepton_absetav':
                s.habseta = s.make_habseta(hname)
                usebin = False
                xtra = ' with special abseta binning'
            print '--> creating %s%s'%(hname,xtra)
            # build from TNtuple
            drawstring = '%s>>%s'%(var,hname)
            if usebin:
                drawstring = '%s>>%s(%s)'%(var,hname,bin)
            if SuSample.debug==True:
                print 'TTree::Draw :',drawstring,'|',cut
            s.nt[path].Draw(drawstring,cut,'goff')
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
    def __init__(s,name,label,samples,color=ROOT.kBlack,flags=[],table={},po=None):
        """ constructor """
        s.samples = [SuSample(a) for a in samples]
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
        return [e.auto() for e in s.samples]
    def histo(s,hname,d,unitize=False,rebin=1.0):
        """ sum histogram of all subsamples """
        if len(s.samples)==0:
            return None
        res = s.samples[0].histo(hname,d,rebin=rebin)
        for h in s.samples[1:]:
            htmp = h.histo(hname,d.clone(),rebin=rebin)
            res.Add(htmp)
        if res:
            res.SetLineColor(ROOT.kBlack)
            res.SetFillColor(s.color)
            res.SetMarkerSize(0)
            if unitize:
                res.Unitize()
            elif SuStackElm.new_scales==True and 'qcd' in s.flags and (isinstance(d,SuPlot) and d.status==0):
                print '--------->', 'qcd scaling start:',hname
                scales = s.po.get_scales(d)
                print '--------->', 'qcd scaling applying (QCD_SYS_SCALES=%d):'%SuStack.QCD_SYS_SCALES,hname
                if SuStack.QCD_SYS_SCALES:
                    res.Scale(scales)
                else:
                    res.ScaleOne(scales)
                print '--------->', 'qcd scaling end:',hname
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
    def choose_sig(s,i):
        """ Select signal sample, turning all others off """
        s.flagsum['S']=i
        return s.choose_flag('sig_'+MAP_BGSIG[i],'sig')
    def choose_ewk(s,i):
        """ Select all electroweak samples, turning all others off """
        s.choose_wtaunu(i)
        s.choose_zmumu(i)
        s.choose_ztautau(i)
    def choose_wtaunu(s,i):
        """ Select signal sample, turning all others off """
        s.flagsum['X']=i
        return s.choose_flag('wtaunu_'+MAP_BGSIG[i],'wtaunu')
    def choose_zmumu(s,i):
        """ Select signal sample, turning all others off """
        s.flagsum['Y']=i
        return s.choose_flag('zmumu_'+MAP_BGSIG[i],'zmumu')
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
                tot += sam.nentries()
        print 'Total:',tot
    def add(s,name,samples,color,flags=[],table={},label=None):
        """ backward-compatible interface:
            po.add(label='t#bar{t}',samples='mc_jimmy_ttbar',color=ROOT.kGreen)
        """
        if not isinstance(samples,list):
            samples = [samples,]
        if label==None: label = name
        s.elm.append( SuStackElm(name,label,samples,color,flags,table,po=s) )
    def adn(s,name,samples,color,flags=[],table={},label=None):
        """ add with flags=['no',...] """
        return s.add(name,samples,color,flags+['no'],table,label)
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
            print 'Unfolding method:',method,d.name,d.unfold['mc']
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
                print 'INFO: forcing *nominal* QCD fit for sytematic %d (%s)'%(i,o.name)
                res.append(s.get_scale_wrap(d.nominal()))
            else:
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
                tname = '%s_%d'%(os.path.basename(d.h_path()),ib) #lpt_0
                # modify basedir to become metfit/bin_%d/lpt_%d
                dtmp = d.clone()
                dtmp.basedir = dtmp.qlist( d.basedir[2] + '/bin_%d'%d.slice + '/' + tname ) # nominal/bin_%(ieta)/lpt_%(ipt)
                res.append(s.get_scale(dtmp))
            # duplicate first/last bin scales for underflow/overflow scale
            return [res[0],] + res + [res[-1],]
        return s.get_scale(d)  #same scale factor for all bins
    def scale_sys(s,key,qcderr):
        """ s.scales[key] = (scale,scale_error).
        qcderr is either NOM/UP/DOWN, or a multiplicative scale factor """
        #print 'NOM =',s.scales[key][0]
        return s.scales[key][0]
        if qcderr=='NOM':
            return s.scales[key][0]
        elif qcderr=='UP':
            return s.scales[key][0]*(1.0+1.0*s.scales[key][1])
        elif qcderr=='DOWN':
            return s.scales[key][0]*(1.0-1.0*s.scales[key][1])
        else:
            qcderr = float(qcderr)
            return s.scales[key][0]*qcderr
        assert False,'Unsupported QCD scale variation'
    def get_scale(s,d):
        """ A lookup cache of QCD scale values
        Note: d is an instance of SuSys, not SuPlot
        """
        # massage the path to put us in the QCD region
        d2 = d.clone()
        d2.qcd_region()
        # key encodes stack composition + path to metfit histogram with histo name + range of fit
        key = None
        if d2.use_ntuple(): # todo: add d2.path if we end up using unbinned ntuples for systematics
            key = s.get_flagsum()+'_'+d2.qcd['descr']+'_'+d2.qcd['var']+'_'+str(d2.qcd['min'])+'to'+str(d2.qcd['max'])
        else:
            key = s.get_flagsum()+'_'+d2.h_path_fname()+'_'+str(d2.qcd['min'])+'to'+str(d2.qcd['max'])
        assert key
        if key in s.scales:
            return s.scale_sys(key,d.qcderr)
        print 'COMPUTING NEW QCD WEIGHT:',key
        import SuFit
        f = SuFit.SuFit()
        f.addFitVar( d2.qcd['var'], d2.qcd['min'] , d2.qcd['max'] , '%s (GeV)'%(d2.qcd['var']) );
        # get histograms
        hdata   = s.data('data',d2).h
        hfixed = s.ewk('bgfixed',d2).h
        hfree = s.qcd('bgfree',d2).h
        if not (hdata and hfree and hfixed):
            print 'WARNING: missing histogram for QCD normalization',hdata,hfree,hfixed
            return 1.0
        assert hdata,'Failed to find data'
        assert hfixed,'Failed to find fixed backgrounds'
        assert hfree,'Failed to find free backgrounds'
        # run SuFit
        hdata.getLegendName = lambda : 'DATA'
        hfixed.getLegendName = lambda : 'EWK backgrounds'
        hfree.getLegendName = lambda : 'QCD'
        f.setDataBackgrounds(hdata,hfixed,hfree)
        if False: # check the effect of model errors on the fit
            f.blowUpErrors(0,100,inc=True)
        tmp = None
        if SuStack.QCD_TF_FITTER:
            f.doFitTF()
            tmp = f.drawFitsTF(key)
        else:
            f.doFit()
            tmp = f.drawFits(key)
        assert tmp
        s.fits[key] = tmp[0]
        s.gbg.append((f,hdata,hfixed,hfree,tmp))
        s.scales[key] = (f.scales[0],f.scalesE[0],f.fractions[0],f.fractionsE[0])
        return s.scale_sys(key,d.qcderr)
    def histo(s,name,hname,d,norm=None):
        """ generic function to return histogram for a particular subsample """
        loop = [z for z in s.elm if z.name==name]
        return s.histosum(loop,hname,d,norm)
    def asym_generic(s,method,hname,d,*args,**kwargs):
        """ Generic function that builds asymmetry for a given method """
        import SuCanvas
        print '--------->', 'asym_generic: preparing hPOS'
        hPOSs = method(hname+'_POS',d.clone(q=0),*args,**kwargs)
        print '--------->', 'asym_generic: preparing hNEG'
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
        hPOSs = method(hname+'_POS',d.clone(q=0),*args,**kwargs)
        print '--------->', 'ratio_generic: preparing hNEG'
        hNEGs = method(hname+'_NEG',d.clone(q=1),*args,**kwargs)
        assert len(hPOSs.flat) == len(hNEGs.flat)
        for i,(hPOS,hNEG) in enumerate( zip(hPOSs.flat,hNEGs.flat) ):
            if not i in hPOSs.enable: continue
            if hPOS.h and hNEG.h:
                d.flat[i].h = SuCanvas.SuCanvas.WRatio(hPOS.h,hNEG.h)
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
    def ratio_data(s,*args,**kwargs):
        return s.ratio_generic(s.data,*args,**kwargs)
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
    def ratio_data_sub(s,*args,**kwargs):
        return s.ratio_generic(s.data_sub,*args,**kwargs)
    def mc(s,hname,d,name=None):
        """ MC summed histogram """
        if not name: # use flags to determine which MC to plot
            loop = [e for e in s.elm if 'mc' in e.flags and 'no' not in e.flags]
        else:         # manually specify what to plot
            loop = [e for e in s.elm if e.name==name]
        return s.histosum(loop,hname,d)
    def asym_mc(s,*args,**kwargs):
        return s.asym_generic(s.mc,*args,**kwargs)
    def ratio_mc(s,*args,**kwargs):
        return s.ratio_generic(s.mc,*args,**kwargs)
    def sig(s,hname,d):
        """ signal MC summed histogram """
        loop = [e for e in s.elm if 'sig' in e.flags and 'no' not in e.flags]
        return s.histosum(loop,hname,d)
    def asym_sig(s,*args,**kwargs):
        return s.asym_generic(s.sig,*args,**kwargs)
    def ratio_sig(s,*args,**kwargs):
        return s.ratio_generic(s.sig,*args,**kwargs)
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
        assert hname in ('lpt','met','njets','wmt','wpt')
        # locate list of eta bins
        idxs = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10]  # temp. hack: just put it by hand!
        if False: #TODO: loop over multiple samples
            hdir = loop[0].samples[0].get_from_file( d.nominal().h_path_folder() )
            assert hdir
            allkeys = [z.GetName() for z in hdir.GetListOfKeys() if re.match('bin_',z.GetName())]
            idxs = sorted([ int(z.replace('bin_','')) for z in allkeys ])
            print idxs, d.nominal().h_path_folder()
        assert idxs[-1]==len(idxs)-1
        # locate heta [should always be able to find it]
        heta = None
        for ilp in loop:
            if heta: break
            for ismpl in ilp.samples:
                if heta: break
                heta = ismpl.get_from_file( d.nominal().h_path_folder() + '/' + 'lepton_absetav' )
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
