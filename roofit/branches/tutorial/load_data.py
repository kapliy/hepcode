#!/usr/bin/env python

"""
A few modules to load data from TGraphs or TNtuples into python array.array's
"""

import array,math,random,string

def rand_name(ln=10):
    return ''.join(random.choice(string.ascii_uppercase + string.digits) for x in range(ln))

def graph_to_array1(hz,maxdata):
    N = hz.GetN()
    print 'Reading graph with',N,'entries'
    v1 = hz.GetX()
    nmax = min(N,maxdata);
    res = [v1[i] for i in range(0,nmax)]
    return len(res),array.array('f',res)
def graph_to_array2(hz,maxdata):
    N = hz.GetN()
    print 'Reading graph with',N,'entries'
    v1 = hz.GetX()
    v2 = hz.GetY()
    nmax = min(N,maxdata);
    res1 = [v1[i] for i in range(0,nmax)]
    res2 = [v2[i] for i in range(0,nmax)]
    assert len(res1) == len(res2)
    return len(res1),array.array('f',res1),array.array('f',res2)

def erange(let):
    if let=='B':
        return (-1.05,1.05)
    if let=='C':
        return (-10.0,-1.05)
    if let=='A':
        return (1.05,10.0)
def islen2(name):
    """ True if string is like AA or BB0 """
    if len(name)==2:
        return True
    try:
        int(name[2:])
        return True
    except:
        return False
def eranges(name):
    """ Converts a string like /BB/ into a range of etas """
    if islen2(name):
        return erange(name[0]),erange(name[1])
    elif name[:3] == 'FWC':
        return ((-10.0,-2.0),(-10.0,-2.0))
    elif name[:3] == 'FWA':
        return ((2.0,10.0),(2.0,10.0))
    elif name[:3] == 'MWC':
        return ((-2.0,-1.05),(-2.0,-1.05))
    elif name[:3] == 'MWA':
        return ((1.05,2.0),(1.05,2.0))
    elif name[:5] == 'TRFWC':
        return ((-2.0,-1.7),(-2.0,-1.7))
    elif name[:5] == 'TRFWA':
        return ((1.7,2.0),(1.7,2.0))
    elif name[:5] == 'TRMWC':
        return ((-1.7,-1.05),(-1.7,-1.05))
    elif name[:5] == 'TRMWA':
        return ((1.05,1.7),(1.05,1.7))
    elif name[:3] == 'Bcc':
        return ((-1.05,0.0),(-1.05,0.0))
    elif name[:3] == 'Baa':
        return ((0.0,1.05),(0.0,1.05))
    else:
        assert False,'Error: wrong --region: %s'%name

def get_eranges(name):
    rp,rn=eranges(name);
    print 'Eta ranges for positive and negative muons:', rp,rn
    return rp,rn
def get_prange(name):
    rp = (-100,100)
    pi = math.pi
    try:
        ir = int(name[-1])
        if ir == 0:
            rp = (-pi,-pi/2.0)
        elif ir==1:
            rp = (-pi/2.0,0.0)
        elif ir==2:
            rp = (0.0,pi/2.0)
        elif ir==3:
            rp = (pi/2.0,pi)
        elif ir==4:
            rp = (-pi/2.0,pi/2.0)
    except:
        print 'Range not constrained'
        pass
    print 'Phi range for muons:', rp
    return rp
    
def ntuple_to_array1(t,name,xmin,xmax,maxdata=1000000):
    """ Load TNtuple from a file: only load mZ """
    ep_name,en_name = 'lP_eta','lN_eta'
    fp_name,fn_name = 'lP_phi','lN_phi'
    mz_name = 'Z_m'
    rp,rn = get_eranges(name)
    rf = get_prange(name)
    N = t.GetEntries()
    print 'Reading tree with',N,'entries'
    res = []
    nl=0
    for i in range(N):
        t.GetEntry(i)
        ep = getattr(t,ep_name)
        en = getattr(t,en_name)
        fp = getattr(t,fp_name)
        fn = getattr(t,fn_name)
        mz = getattr(t,mz_name)
        good = rp[0] < ep < rp[1] and rn[0] < en < rn[1] and xmin < mz < xmax
        if not good: continue
        res.append(mz)
        nl+=1
        if nl>=maxdata: break
    return len(res),array.array('f',res)

def ntuple_to_array_etalim(t,name,xmin,xmax,maxdata=1000000,nskip=0):
    """ Load TNtuple from a file; both Z muons in the same eta range, but phi cut is applied on integrated spectra"""
    ep_name,en_name = 'lP_eta','lN_eta'
    pp_name,pn_name = 'lP_pt','lN_pt'
    fp_name,fn_name = 'lP_phi','lN_phi'
    mz_name = 'Z_m'
    rp,rn = get_eranges(name)
    rf = get_prange(name)
    N = t.GetEntries()
    print 'Reading tree with',N,'entries'
    resp = []
    resn = []
    for i in range(N):
        t.GetEntry(i)
        ep = getattr(t,ep_name)
        en = getattr(t,en_name)
        pp = getattr(t,pp_name)
        pn = getattr(t,pn_name)
        fp = getattr(t,fp_name)
        fn = getattr(t,fn_name)
        mz = getattr(t,mz_name)
        if not xmin < mz < xmax:
            continue
        if not (rp[0] < ep < rp[1] and rn[0] < en < rn[1]):
            continue
        goodP = rf[0] < fp < rf[1]
        goodN = rf[0] < fn < rf[1]
        if goodP:
            resp.append(pp)
        if goodN:
            resn.append(pn)
        nl=min(len(resp),len(resn))
        if nl>=maxdata: break
    # truncate to the min(mu+,mu-)
    Np,Nn = len(resp),len(resn)
    nl=min(len(resp),len(resn))
    print 'Read out %d mu+ and %d mu- - truncating to %d'%(len(resp),len(resn),nl)
    resp,resn = resp[0:nl],resn[0:nl]
    assert len(resp)==len(resn)
    return Np,Nn,len(resp),array.array('f',resp),array.array('f',resn)

def ntuple_to_array_kluit(t,name,xmin,xmax,maxdata=1000000,nskip=0):
    """ Load TNtuple from a file; never require that muons are in fiducial eta/phi in the same Z event (more statistics) """
    ep_name,en_name = 'lP_eta','lN_eta'
    pp_name,pn_name = 'lP_pt','lN_pt'
    fp_name,fn_name = 'lP_phi','lN_phi'
    mz_name = 'Z_m'
    rp,rn = get_eranges(name)
    rf = get_prange(name)
    N = t.GetEntries()
    print 'Reading tree with',N,'entries via Kluit method'
    resp = []
    resn = []
    for i in range(N):
        t.GetEntry(i)
        ep = getattr(t,ep_name)
        en = getattr(t,en_name)
        pp = getattr(t,pp_name)
        pn = getattr(t,pn_name)
        fp = getattr(t,fp_name)
        fn = getattr(t,fn_name)
        mz = getattr(t,mz_name)
        if not xmin < mz < xmax:
            continue
        goodP = rp[0] < ep < rp[1] and rf[0] < fp < rf[1]
        goodN = rn[0] < en < rn[1] and rf[0] < fn < rf[1]
        if goodP:
            resp.append(pp)
        if goodN:
            resn.append(pn)
        nl=min(len(resp),len(resn))
        if nl>=maxdata: break
    # truncate to the min(mu+,mu-)
    Np,Nn = len(resp),len(resn)
    nl=min(len(resp),len(resn))
    print 'Read out %d mu+ and %d mu- - truncating to %d'%(len(resp),len(resn),nl)
    resp,resn = resp[0:nl],resn[0:nl]
    assert len(resp)==len(resn)
    return Np,Nn,len(resp),array.array('f',resp),array.array('f',resn)

def ntuple_to_array_akluit(t,name,xmin,xmax,maxdata=1000000,nskip=0):
    """ Load TNtuple from a file; require that muons are NOT in the same fiducial eta in the same Z event (exclusive sample) """
    ep_name,en_name = 'lP_eta','lN_eta'
    pp_name,pn_name = 'lP_pt','lN_pt'
    fp_name,fn_name = 'lP_phi','lN_phi'
    mz_name = 'Z_m'
    rp,rn = get_eranges(name)
    rf = get_prange(name)
    N = t.GetEntries()
    print 'Reading tree with',N,'entries via Anti-Kluit method'
    resp = []
    resn = []
    for i in range(N):
        t.GetEntry(i)
        ep = getattr(t,ep_name)
        en = getattr(t,en_name)
        pp = getattr(t,pp_name)
        pn = getattr(t,pn_name)
        fp = getattr(t,fp_name)
        fn = getattr(t,fn_name)
        mz = getattr(t,mz_name)
        if not xmin < mz < xmax:
            continue
        # apply
        goodP_e = rp[0] < ep < rp[1]
        goodN_e = rn[0] < en < rn[1]
        goodP_f = rf[0] < fp < rf[1]
        goodN_f = rf[0] < fn < rf[1]
        if goodP_e and goodP_f and not goodN_e:
            resp.append(pp)
        if goodN_e and goodN_f and not goodP_e:
            resn.append(pn)
        nl=min(len(resp),len(resn))
        if nl>=maxdata: break
    # truncate to the min(mu+,mu-)
    Np,Nn = len(resp),len(resn)
    nl=min(len(resp),len(resn))
    print 'Read out %d mu+ and %d mu- - truncating to %d'%(len(resp),len(resn),nl)
    resp,resn = resp[0:nl],resn[0:nl]
    assert len(resp)==len(resn)
    return Np,Nn,len(resp),array.array('f',resp),array.array('f',resn)
