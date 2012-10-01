#!/usr/bin/env python

# a bunch of global binning constants

ptbins = [20,25,30,35,40,45,50,120]

absetabins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]

setabins    = [-zz for zz in absetabins[1:]]
setabins.reverse()
setabins += (absetabins)

if __name__ == '__main__':
    print len(absetabins)
    print absetabins
    print len(setabins)
    print setabins
