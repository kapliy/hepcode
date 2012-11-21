#!/usr/bin/env python

import re

POS,NEG,ALL=range(3)
QMAP = {}
QMAP[POS] = (0,'POS','(l_q>0)','mu+ only')
QMAP[NEG] = (1,'NEG','(l_q<0)','mu- only')
QMAP[ALL] = (2,'ALL','(l_q!=0)','mu+ and mu-')
QMAPZ = {}
QMAPZ[POS] = (0,'POS','lP','mu+')
QMAPZ[NEG] = (1,'NEG','lN','mu-')

def prunesub(pre,lvar,sub):
    """ substitutes all instances of 'lvar' in pre with 'sub' """
    res = []
    lvars=[]
    if type(lvar)==type([]) or type(lvar)==type(()):
        lvars = lvar
    else:
        lvars = (lvar,)
    already_subbed = False
    for elm in pre.split(' && '):
        keep = True
        for lvar in lvars:
            if re.match(lvar,elm):
                print '-> pruning away: ',elm,' --> ',sub
                keep = False
                break
        if keep:
            res.append(elm)
        elif sub and not already_subbed:
            res.append(sub)
            already_subbed = True
    return ' && '.join(res)

def prune(pre,lvar):
    """ removes all instances of 'lvar' from pre """
    return prunesub(pre,lvar,None)

w2zsub = []
w2zsub.append(('ptiso','lX_ptiso'))
w2zsub.append(('etiso','lX_etiso'))
w2zsub.append(('l_q','lX_q'))
w2zsub.append(('l_pt','lX_pt'))
w2zsub.append(('l_eta','lX_eta'))
w2zsub.append(('l_phi','lX_phi'))
w2zsub.append(('idhits','lX_idhits'))
w2zsub.append(('l_trigEF','lX_trigEF'))
w2zsub.append(('d0','lX_d0'))
w2zsub.append(('z0','lX_z0'))
def pre_w2z(pre):
    """ Converts a pre string for w ntuple into a corresponding one for z """
    pre = prune(pre,('met','w_mt','nmuons','l_trigEF'))
    xtra = "Z_m>70 && Z_m<110 && fabs(lP_z0-lN_z0)<3 && fabs(lP_d0-lN_d0)<2 && fabs(lP_phi-lN_phi)>0.0 && (lP_q*lN_q)<0"
    res = []
    for elm in pre.split(' && '):
        changed = False
        for isub in w2zsub:
            if re.search(isub[0],elm):
                elm = elm.replace(isub[0],isub[1]).replace('l_pt','lX_pt')
                res.append ( elm.replace('lX',QMAPZ[0][2]) + ' && ' + elm.replace('lX',QMAPZ[1][2]) )
                changed = True
                break
        if not changed:
            res.append(elm)
    return ' && '.join(res) + ' && ' + xtra

def fortruth(pre):
    """ removes pre variables that are not applicable to truth tracks """
    res = []
    for elm in pre.split(' && '):
        if re.search('iso',elm) or re.match('idhits',elm) or re.search('d0',elm) or re.search('z0',elm) or re.search('exms',elm):
            continue
        if re.search('trig',elm):
            continue
        res.append(elm)
    return ' && '.join(res)

def metfitreg_auto(pre):
    """ met fit region from w+jets 2010 analysis
    This is a version that automatically modifies the 'pre' string
    """
    res = []
    for elm in pre.split(' && '):
        if re.match('w_mt',elm):
            res.append('w_mt>35.0')
        elif re.match('met',elm):
            res.append('met>15.0')
        else:
            res.append(elm)
    result = ' && '.join(res)
    print 'INFO: QCD fit in MET variable with the following cuts:'
    print result
    return result

def metfitreg(pre):
    """ met fit region from w+jets 2010 analysis
    This is the version that hardcodes everything except for isolation.
    Useful to produce double-differential asymmetry plots """
    _default = 'l_pt>20.0 && fabs(l_eta)<2.4 && met>15.0 && w_mt>35.0 && idhits==1 && fabs(z0)<10.0 && fabs(d0sig)<10.0 && ptiso20/l_pt<0.1'
    res = []
    # first fill with _default values (minus isolation)
    for elm in _default.split(' && '):
        if re.match('ptiso',elm) or re.match('etiso',elm):
            continue
        res.append(elm)
    # next, take isolation cuts from the actual pre string
    for elm in pre.split(' && '):
        if re.match('ptiso',elm) or re.match('etiso',elm):
            res.append(elm)
    result = ' && '.join(res)
    print 'INFO: QCD fit in MET variable with the following cuts:'
    print result
    return result

def qcdreg(pre):
    """ qcd-enriched sample to get data-driven template """
    res = []
    for elm in pre.split(' && '):
        if re.match('fabs\(d0',elm) or re.match('d0',elm):
            res.append('fabs(d0sig)>3.0 && fabs(d0)>0.1 && fabs(d0)<0.4')
        else:
            res.append(elm)
    return ' && '.join(res)

def samesignreg(pre):
    """ removes the Z charge constraint and explicitly reverses it"""
    newpre = prunesub(pre,('lP_q\*lN_q','\(lP_q\*lN_q'),'(lP_q*lN_q)>0')
    return newpre

def revisoreg(pre):
    """ qcd-enriched sample to get data-driven template
    This version reverses isolation, which allegedly affects pt spectrum
    ptiso20/l_pt<0.1
    ptiso40<2.0 && etiso40<2.0
    """
    res = []
    for elm in pre.split(' && '):
        if re.match('ptiso',elm) or re.match('etiso',elm):
            ws = elm.split('<')
            assert len(ws)==2
            res.append( ws[0] + '>' + ws[1] )
        else:
            res.append(elm)
    return ' && '.join(res)

def looseisoreg(pre):
    """ Loose sample needed for matrix method QCD estimation """
    return prune(pre, ('ptiso','lP_ptiso','l_ptiso','etiso','lP_etiso','lN_etiso','nmuons','l_trigEF') )
