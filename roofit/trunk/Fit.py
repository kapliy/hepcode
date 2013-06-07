import ROOT
from ROOT import RooWorkspace,RooArgSet,RooArgList,RooDataHist,RooAbsData,RooDataSet,RooFormulaVar
from ROOT import RooGaussModel,RooAddModel,RooRealVar,RooAbsReal,RooRealSumPdf
from ROOT import kTRUE,kFALSE,kDashed,gStyle,gPad
from ROOT import TFile,TH1,TH1F,TH1D
from ROOT import RooFit as RF
mz0 = 91.1876

def cmd_none():
    return ['']
def cmd_gaus():
    """ Standard Gaussian resolution shape """
    cmds = []
    cmds.append("r_m[0.0,-1,1]")
    cmds.append("r_s[2.5,0,10]")
    cmds.append('Gaussian::res(x,r_m,r_s)')
    return cmds
def cmd_dgaus():
    """ Double Gaussian resolution shape """
    cmds = []
    cmds.append("r_m[0.0,-1,1]")
    cmds.append("r_s[1.5,0,10]")
    cmds.append('Gaussian::gaus_c(x,r_m,r_s)')
    cmds.append("rt_m[0.0,-1,1]")
    cmds.append("rt_s[3,0,10]")
    cmds.append('Gaussian::gaus_t(x,rt_m,rt_s)')
    cmds.append("f[0.85]")  # fraction of core
    cmds.append("SUM::res(f*gaus_c,gaus_t)")
    return cmds
def cmd_cb():
    """ Crystal Ball resolution shape """
    cmds = []
    #cmds.append("r_m[0.0,-1,1]")
    cmds.append("r_m[0.0]")
    cmds.append('r_a[1.7,0,10]')
    cmds.append('r_n[1.5,0,10]')
    cmds.append("r_s[2.5,0,10]")
    cmds.append('RooCBShape::res(x,r_m,r_s,r_a,r_n)')
    return cmds
resolutions = (cmd_none,cmd_gaus,cmd_dgaus,cmd_cb)

gam={}
gam['BB'] = -0.37
gam['CC'] = -1.16
gam['AA'] = 1.10

# Voigtian with exponential
def make_voigbg(w,minZ,maxZ,m=mz0,fixw=False):
    """ Returns PDF and YES/NO whether it is an extended likelihood fit"""
    cmds = []
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('width[2.49,0,5.0]')
    cmds.append('sigma[1,0,5.0]')
    cmds.append('expar[-0.1,-1,0]')
    cmds.append("RooVoigtian::voig(x,m,width,sigma)")
    cmds.append("RooExponential::exp(x,expar)")
    cmds.append('nvoig[1,0,1000000]')
    cmds.append('nexp[1,0,1000000]')
    cmds.append("SUM::voigbg(nvoig*voig,nexp*exp)")
    [w.factory(cmd) for cmd in cmds]
    if fixw:
        w.var('width').setConstant(kTRUE) if w.var('width') else None
    return w.pdf('voigbg'), kTRUE

# Simple voigtian
def make_voig(w,minZ,maxZ,m=mz0,fixw=False):
    """ Returns PDF and YES/NO whether it is an extended likelihood fit"""
    cmds = []
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('width[2.49,0,5.0]')
    cmds.append('sigma[1,0,5.0]')
    cmds.append("RooVoigtian::voig(x,m,width,sigma)")
    [w.factory(cmd) for cmd in cmds]
    if fixw:
        w.var('width').setConstant(kTRUE) if w.var('width') else None
    return w.pdf('voig'), kFALSE

# Simple gaussian
def make_gaus(w,minZ,maxZ,m=mz0):
    cmds = []
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('s[1.0,0,5.0]')
    cmds.append("RooGaussian::gaus(x,m,s)")
    [w.factory(cmd) for cmd in cmds]
    return w.pdf('gaus'), kFALSE

# fancy shape with interference term, but no BG - via AmplitudeFit
#https://kyoko.web.cern.ch/KYOKO/DiffZ/KyokoYamamoto_DiffZ_20101217.pdf
def make_egge(w,minZ,maxZ,ires=1,m=mz0):
    """ Dimuon mass spectrum before FSR - includes photon and interference terms """
    cmds = []
    # coefficients for the amplitudes
    cmds.append("A[1,0,1000000]")
    cmds.append("B[1,0,1000000]")
    cmds.append("C[10000.0,0,1000000]")
    # amplitudes
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('g[8,0,100]')
    denom = '((x^2-m^2)^2+g^2*m^2)'
    cmds.append("expr::z_rbw('x^2/%s',x,m,g)"%denom)
    cmds.append("expr::z_int('(x^2-m^2)/%s',x,m,g)"%denom)
    cmds.append("expr::z_rad('1/(x^2+1)',x)")
    # resolution model
    cmds += resolutions[ires]()
    [w.factory(cmd) for cmd in cmds]
    # sum-of-amplitudes pdf
    lshape = RooRealSumPdf('lshape','lshape',RooArgList(w.function('z_rad'),w.function('z_int'),w.function('z_rbw')),RooArgList(w.var('A'),w.var('B'),w.var('C')))
    getattr(w,'import')(lshape)
    # convolution
    pdf = w.pdf('lshape')
    if w.pdf('res'):
        w.var('x').setBins(10000,'cache')
        cmd = 'FCONV::sum(x,lshape,res)'
        w.factory(cmd)
        pdf = w.pdf('sum')
    return pdf, kFALSE

# fancy shape with interference term, but no BG - via AmplitudeFit
def make_bwfull(w,minZ,maxZ,ires=1,fixw=False,m=mz0):
    """ isDouble enabled a double-gaussian resolution model """
    cmds = []
    # coefficients for the amplitudes
    cmds.append("A[1,0,1000000]")
    cmds.append("B[1,0,1000000]")
    cmds.append("C[10000.0,0,1000000]")
    # amplitudes
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('g[2.49,0,10]')
    denom = '((x^2-m^2)^2+x^4*g^2/m^2)'
    cmds.append("expr::z_rbw('x^2/%s',x,m,g)"%denom)
    cmds.append("expr::z_int('(x^2-m^2)/%s',x,m,g)"%denom)
    cmds.append("expr::z_rad('1/(x^2)',x)")
    # resolution model
    cmds += resolutions[ires]()
    [w.factory(cmd) for cmd in cmds]
    # any parameter adjustments
    if True:
        w.var('r_m').setConstant(kTRUE) if w.var('r_m') else None
        w.var('rt_m').setConstant(kTRUE) if w.var('rt_m') else None
        w.var('g').setConstant(kTRUE) if w.var('g') and fixw else None
    # sum-of-amplitudes pdf
    lshape = RooRealSumPdf('lshape','lshape',RooArgList(w.function('z_rad'),w.function('z_int'),w.function('z_rbw')),RooArgList(w.var('A'),w.var('B'),w.var('C')))
    getattr(w,'import')(lshape)
    # convolution
    pdf = w.pdf('lshape')
    if w.pdf('res'):
        w.var('x').setBins(10000,'cache')
        cmd = 'FCONV::sum(x,lshape,res)'
        w.factory(cmd)
        pdf = w.pdf('sum')
    return pdf, kFALSE

# Just clear bw with gaussian
def make_bw(w,minZ,maxZ,ires=0,m=mz0):
    cmds = []
    # amplitudes
    cmds.append('m[%s,%s,%s]'%(m,minZ,maxZ))
    cmds.append('g[2.495,0,100]')
    cmds.append("EXPR::z_rbw('x^2/((x^2-m^2)^2+x^2*g^2/m^2)',x,m,g)")
    # resolution model
    cmds += resolutions[ires]()
    [w.factory(cmd) for cmd in cmds]
    # any parameter adjustments
    w.var('r_m').setConstant(kTRUE) if w.var('r_m') else None
    pdf = w.pdf('z_rbw')
    if w.pdf('res'):
        w.var('x').setBins(10000,'cache')
        cmd = 'FCONV::sum(x,z_rbw,res)'
        w.factory(cmd)
        pdf = w.pdf('sum')
    return pdf, kFALSE

# Parse command line option "--func" and create appropriate fit function
def make_fit_function(w,funcin,bins):
    func,res,isExt = funcin,None,False
    mymin = float(bins.split(',')[1])
    mymax = float(bins.split(',')[2])
    if funcin[-1].isdigit():
        func = funcin[:-1]
        res = int(funcin[-1])
    if func=='gaus':
        model,isExt = make_gaus(w,mymin,mymax)
    elif func=='bw':
        model,isExt = make_bw(w,mymin,mymax,res)
    elif func=='egge':
        model,isExt = make_egge(w,mymin,mymax,res)
    elif func=='bwfull':
        model,isExt = make_bwfull(w,mymin,mymax,res)
    elif func=='voig':
        model,isExt = make_voig(w,mymin,mymax)
    elif func=='voigbg':
        model,isExt = make_voigbg(w,mymin,mymax)
    else:
        print 'Wrong --func (%s). Exiting...'%funcin
        sys.exit(0)
    model.isExt = isExt
    w.func = func
    w.model = model
    return model

def PrintVariables(model):
    #model.Print('t')
    vars = model.getVariables()
    vars.Print('v')

def Fit(w,data,fullbins,fitbins,ncpus=4,extras=False,gaus=False):
    # plot the data first
    x = w.var('x'); xtra = None
    assert x
    frame = x.frame(RF.Title('Invariant mass fit using'))
    if data.InheritsFrom('TH1'):
        print 'INFO: automatically converting %s to RooDataHist'%data.ClassName()
        #x.setRange(fullbins[1],fullbins[2])
        data = import_histo(w,data,fullbins[1],fullbins[2])
    if data.ClassName()=='RooDataSet':
        RooAbsData.plotOn(data,frame,RF.Name('dataZ'),RF.Binning(fullbins[0]))
    else:
        RooAbsData.plotOn(data,frame,RF.Name('dataZ'))
    x.setRange('it1',fitbins[1],fitbins[2])
    RF.Hesse(kTRUE)
    r = w.model.fitTo(data,RF.PrintLevel(1),RF.Extended(w.model.isExt),RF.NumCPU(ncpus),RF.Save(),RF.Range('it1'))
    w.model.plotOn(frame,RF.Name('mit1'),RF.Range('it1'),RF.NormRange('it1'))
    #return (r,frame,None,None,None,None) # TODO - problem fitting in a reduced range!
    res1st = []
    if gaus:
        nsc = 1.0  #1.25
        mean=w.var('m').getVal()
        sigma=w.var('s').getVal()
        xtra = 'mz(1st) = %.3f +/- %.3f'%(w.var('m').getVal(),w.var('m').getError())
        ndf = r.floatParsFinal().getSize()
        chi2ndf = frame.chiSquare(ndf)
        res1st += [mean,sigma,chi2ndf,ndf]
        print 'SECOND ITERATION: mean =',mean,'sigma =',sigma,'chi2dof =',chi2ndf
        x.setRange('it2',mean-sigma*nsc,mean+sigma*nsc)
        r = w.model.fitTo(data,RF.LineColor(ROOT.kRed),RF.PrintLevel(1),RF.Extended(model.isExt),RF.NumCPU(ncpus),RF.Save(),RF.Range('it2'))
        w.model.plotOn(frame,RF.Name('mit2'),RF.Range('it2'),RF.NormRange('it2'),RF.LineColor(ROOT.kRed))
    ndf = r.floatParsFinal().getSize()
    chi2ndf = frame.chiSquare(ndf)
    # wildcards or comma-separated components are allowed:
    if w.model.isExt:
        w.model.plotOn(frame,RF.Components('exp*'),RF.LineStyle(kDashed))
    if extras:
        w.model.plotOn(frame,RF.VisualizeError(r))
        w.model.paramOn(frame,data)
    return (r,frame,chi2ndf,ndf,xtra,res1st)

def import_unbinned(w,hz,tt,pre,reg,xmin,xmax,ndata,scale=1.0):
    """ Load TTree from a file. Note that all selection cuts must be pre-filled in the 'pre' cut """
    if hz.ClassName() in ('TNtuple','TTree','TChain'):
        N,v1 = ntuple_to_array1(hz,tt,reg,xmin,xmax,ndata,pre=pre)
    else:
        print 'Problem loading class',hz.ClassName()
        sys.exit(0)
    # TODO - code the stuff below (for now, simply copy-pasted)
    print 'Loaded raw unbinned data with',N,'entries'
    ds1 = RooDataSet('ds1','ds1',RooArgSet(w.var('x')))
    ds2 = RooDataSet('ds2','ds2',RooArgSet(w.var('x')))
    nmax = min(N,ndata); nmaxp10 = nmax if nmax>10 else 10
    bar = SimpleProgressBar.SimpleProgressBar(10,nmax)
    x = w.var('x')
    nf=0
    for i in range(0,nmax):
        if i%(int(nmaxp10/10))==0 and i>0:
            print bar.show(i)
        xv = func_SCALE(v1[i],scale)
        if xmin < xv < xmax:
            x.setVal(xv)
            ds1.add(RooArgSet(x))
            nf+=1
    print 'Final count of unbinned data:',nf
    #getattr(w,'import')(ds1,RF.Rename('ds1'))
    #getattr(w,'import')(ds2,RF.Rename('ds2'))
    return ds1,ds2

def import_histo(w,hz,xmin,xmax):
    """ Load TH1 histogram from a file """
    print 'Loading histogram with',hz.GetEntries(),'entries'
    data = RooDataHist('data','Zmumu MC',RooArgList(w.var('x')),hz)
    return data
