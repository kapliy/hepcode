#!/usr/bin/env python
from helpers import *;
import glob;
fp=glob.glob("/home/antonk/output/user*period*/out.root");
f=[AnaFile(fpp) for fpp in fp];
[ff.Load() for ff in f]
print [ff.cands.GetEntries() for ff in f]
t = sum(f)

