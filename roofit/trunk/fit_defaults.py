#!/usr/bin/env python


def set_egge_defaults(w,reg,N):
    """
    1) 0xb7b7e40 RooRealVar::   A = 0.76039 +/- 4777.93  L(0 - 1e+06)  "A"
    2) 0xb9891c8 RooRealVar::   B = 0.154139 +/- 3667.29  L(0 - 1e+06)  "B"
    3) 0xbaac250 RooRealVar::   C = 12272 +/- 570378  L(0 - 1e+06)  "C"
    4) 0xba8d5c8 RooRealVar::   g = 2.64938 +/- 0.137425  L(0 - 100)  "g"
    5) 0xba8d198 RooRealVar::   m = 90.8434 +/- 0.044614  L(70 - 110)  "m"
    6) 0xb988fa0 RooRealVar:: r_a = 1.6684 +/- 0.0974905  L(0 - 10)  "r_a"
    7) 0xba92d08 RooRealVar:: r_m = 0 C  L(-INF - +INF)  "r_m"
    8) 0xbaadc68 RooRealVar:: r_n = 1.20966 +/- 0.174213  L(0 - 10)  "r_n"
    9) 0xba811a8 RooRealVar:: r_s = 2.05936 +/- 0.0822396  L(0 - 10)  "r_s"
    10) 0xbb79508 RooRealVar::   x = 86.4938  L(70 - 110)  "x"
    """
    def set(key,val):
        return w.var(key).setVal(val) if w.var(key) else None
    def set(key,val,N,NTOT):
        return w.var(key).setVal(val*N/NTOT) if w.var(key) else None
    if reg=='ALL':
        NTOT = 11448.0
        set('A',0.76039,N,NTOT)
        set('A',0.76039,N,NTOT)
        set('A',0.76039,N,NTOT)
        
