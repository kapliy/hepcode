#!/usr/bin/env python

"""
A small script that summarizes QCD systematic deviations
due to different signal MC or different fit variables.
"""

memo = """
<pre>
In each cell, the first number is the QCD fraction (multiplied by 100%) in the region MET=25..120 GeV.
The second number is the relative deviation of QCD fraction with respect to the nominal case (Powheg+Pythia MET fit)
</pre>
"""

import sys,pickle

etabins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]

if True:
    fin = open('save.pickle','r')
    R = pickle.load(fin)
    f = open('index2.html','w')
    #RES[ebin][lvpair][bgsig] = hfrac
    print >>f,'<HTML><BODY>'
    print >>f,memo,'<BR>'
    for iq in ('POS',):
        QMAP = {'POS':'mu+','NEG':'mu-'}
        print >>f,'<HR>'
        print >>f,QMAP[iq]
        print >>f,'<HR>'
        print >>f,'<TABLE border="1" CELLPADDING="0" CELLSPACING="1" width="1300">'
        print >>f,'<TR>'
        print >>f, '<TD width="130">|eta| bin</TD>'
        for ieta in xrange(0,10+1):
            print >>f, '<TD width="80">','%.2f&lt;|eta|&lt;%.2f'%(etabins[ieta],etabins[ieta+1]),"</TD>"
        print >>f,'</TR>'
        maxv = [-1]*len(etabins)
        for lvar in ('met','w_mt','l_pt'):
            SIGMAP = { 1 : "MC@NLO", 4 : "PowHeg+Herwig", 5 : "PowHeg+Pythia" }
            print >>f, '<TR><TD colspan="%d"></TR>'%(len(etabins)+1)
            for bgsig in (5,4,1):
                print >>f,'<TR>'
                print >>f, '<TD width="130">',lvar,'('+SIGMAP[bgsig]+')'+'</TD>'
                for ieta in xrange(0,10+1):
                    nom = R[ieta]['met'][5]
                    v =   R[ieta][lvar][bgsig]
                    per = (v - nom)/nom*100.0
                    if per>maxv[ieta]: maxv[ieta]=per
                    if v<0 or nom<0:
                        v = -0.01
                        per = 0
                    print >>f,'<TD width="80">','%.1f | %s%.1f%%'%(v*100.0,'' if per<0 else '+',per),'</TD>'
                print >>f,'</TR>'
        print >>f, '<TR><TD colspan="%d"></TR>'%(len(etabins)+1)
        print >>f, '<TR>'
        print >>f, '<TD>Max. deviation</TD>'
        for ieta in xrange(0,10+1):
            print >>f, '<TD>%.1f%%</TD>'%maxv[ieta]
        print >>f, '</TR>'
        print >>f,'</TABLE>'
    print >>f,'</BODY></HTML>'
