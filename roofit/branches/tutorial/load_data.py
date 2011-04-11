#!/usr/bin/env python

"""
A few modules to load data from TGraphs or TNtuples into python array.array's
"""

import array

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
def eranges(name):
    """ Converts a string like /BB/ into a range of etas """
    if len(name)==2:
        return erange(name[0]),erange(name[1])
    elif name == 'Bcc':
        return ((-1.05,0.0),(-1.05,0.0))
    elif name == 'Baa':
        return ((0.0,1.05),(0.0,1.05))
    else:
        assert False,'Error: wrong --region: %s'%name

def get_eranges(name):
    rp,rn=eranges(name);
    print 'Ranges for positive and negative muons:', rp,rn
    return rp,rn

def ntuple_to_array1(t,name,xmin,xmax,maxdata=1000000):
    """ Load TNtuple from a file: only load mZ """
    ep_name,en_name = 'lP_eta','lN_eta'
    mz_name = 'Z_m'
    rp,rn = get_eranges(name)
    N = t.GetEntries()
    print 'Reading tree with',N,'entries'
    res = []
    nl=0
    for i in range(N):
        t.GetEntry(i)
        ep = getattr(t,ep_name)
        en = getattr(t,en_name)
        mz = getattr(t,mz_name)
        good = rp[0] < ep < rp[1] and rn[0] < en < rn[1] and xmin < mz < xmax
        if not good: continue
        res.append(mz)
        nl+=1
        if nl>maxdata: break
    return len(res),array.array('f',res)
def ntuple_to_array2(t,name,xmin,xmax,maxdata=1000000):
    """ Load TNtuple from a file """
    ep_name,en_name = 'lP_eta','lN_eta'
    pp_name,pn_name = 'lP_pt','lN_pt'
    mz_name = 'Z_m'
    rp,rn = get_eranges(name)
    N = t.GetEntries()
    print 'Reading tree with',N,'entries'
    resp = []
    resn = []
    nl=0
    for i in range(N):
        t.GetEntry(i)
        ep = getattr(t,ep_name)
        en = getattr(t,en_name)
        pp = getattr(t,pp_name)
        pn = getattr(t,pn_name)
        mz = getattr(t,mz_name)
        good = rp[0] < ep < rp[1] and rn[0] < en < rn[1] and xmin < mz < xmax
        if not good: continue
        resp.append(pp)
        resn.append(pn)
        nl+=1
        if nl>maxdata: break
    assert len(resp)==len(resn)
    return len(resp),array.array('f',resp),array.array('f',resn)
