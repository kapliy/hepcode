#!/usr/bin/env python

_PTMIN = 19.99

def filter_fct(t):
    trig = ('EF_mu18_MG_medium' in t.trig_ef) or ('EF_mu18_MG' in t.trig_ef)
    if not trig: return False
    nmu = [i for i in xrange(t.nmu) if t.mu_primauthor[i]==6 and t.mu_pt[i]>_PTMIN]
    if len(nmu)==0: return False
    return True
