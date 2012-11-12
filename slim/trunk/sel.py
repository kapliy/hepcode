#!/usr/bin/env python

_PTMIN = 13

def filter_fct(t):
    trig = ('EF_mu18_MG_medium' in t.trig_ef) or ('EF_mu18_MG' in t.trig_ef)
    if not trig: return False
    return True
