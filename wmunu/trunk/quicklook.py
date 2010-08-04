#!/usr/bin/env python
from helpers import *;
import glob;
fp=glob.glob("/home/antonk/outputABC_check/user*period*/out.root");
print fp
f=[AnaFile(fpp) for fpp in fp];
[ff.Load() for ff in f]
print [ff.cands.GetEntries() for ff in f]
t = sum(f)

