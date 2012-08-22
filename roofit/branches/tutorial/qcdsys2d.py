#!/usr/bin/env python

"""
A small script that summarizes QCD systematic deviations
due to different signal MC or different fit variables.
"""

memo = """
<pre>
In each cell, the first number is the QCD fraction (multiplied by 100%) in the region MET=25..200 GeV.
The second number is the relative deviation of QCD fraction with respect to the nominal case (Powheg+Pythia MET fit)
</pre>
"""

import sys,pickle,os

etabins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]

import ROOT
fupd_name = 'QCD_W_PTCONE4.root'

if True:
    fin = open('save.pickle','r')
    R = pickle.load(fin)
    f = open('index2.html','w')
    #RES[ebin][lvpair][bgsig] = hfrac
    fupd = None
    if os.path.exists(fupd_name):
        fupd = ROOT.TFile.Open(fupd_name,"UPDATE")
    print >>f,'<HTML><BODY>'
    print >>f,memo,'<BR>'
    for iq in (0,1,2):
        QMAP = {0:'mu+',1:'mu-',2:'both charges'}
        QMAPN = {0:'POS',1:'NEG',2:'ALL'}
        print 'Working on: ',QMAP[iq]
        print >>f,'<HR>'
        print >>f,QMAP[iq]
        print >>f,'<HR>'
        print >>f,'<TABLE border="1" CELLPADDING="0" CELLSPACING="1" width="1200">'
        print >>f,'<TR>'
        print >>f, '<TD width="120">|&eta;| bin</TD>'
        for ieta in xrange(0,10+1):
            print >>f, '<TD width="80">','%.2f&lt;|&eta;|&lt;%.2f'%(etabins[ieta],etabins[ieta+1]),"</TD>"
        print >>f,'</TR>'
        maxv = [0.0]*len(etabins)
        for lvar in ('met','w_mt','l_pt'):
            SIGMAP = { 1 : "MC@NLO", 4 : "PowHeg+Her", 5 : "PowHeg+Pyt" }
            print >>f, '<TR><TD colspan="%d"></TR>'%(len(etabins)+1)
            for bgsig in (5,4,1):
                print >>f,'<TR>'
                print >>f, '<TD width="120">',lvar,'('+SIGMAP[bgsig]+')'+'</TD>'
                for ieta in xrange(0,10+1):
                    nom = R[iq][ieta]['met'][5]
                    v =   R[iq][ieta][lvar][bgsig]
                    if v<0 or nom<0:
                        print >>f,'<TD width="80">N/A</TD>'
                    else:
                        per = (v - nom)/nom*100.0
                        if abs(per)>abs(maxv[ieta]): maxv[ieta]=per
                        print >>f,'<TD width="80">','%.1f | %s%.1f%%'%(v*100.0,'' if per<0 else '+',per),'</TD>'
                print >>f,'</TR>'
        print >>f, '<TR><TD colspan="%d"></TR>'%(len(etabins)+1)
        print >>f, '<TR>'
        print >>f, '<TD>Max. deviation</TD>'
        for ieta in xrange(0,10+1):
            print >>f, '<TD>%.1f%%</TD>'%maxv[ieta]
        print >>f, '</TR>'
        print >>f,'</TABLE>'
        if fupd and fupd.IsOpen():
            STATMAP = [vv/100.0 for vv in maxv][:-1]
            hh = fupd.Get(QMAPN[iq]+'/bg_QCD')
            fupd.cd(QMAPN[iq])
            hup = hh.Clone('bg_QCD_sysup')
            hup.SetTitle('bg_QCD_sysup')
            hdown = hh.Clone('bg_QCD_sysdown')
            hdown.SetTitle('bg_QCD_sysdown')
            assert (hh.GetNbinsX() == len(STATMAP))
            for ieta in xrange(0,hh.GetNbinsX()):
                assert abs(STATMAP[ieta])>=0
                hup.SetBinContent(ieta+1 , hup.GetBinContent(ieta+1) * (1.0+abs(STATMAP[ieta])))
                hup.SetBinError(ieta+1 , hup.GetBinError(ieta+1) * (1.0+abs(STATMAP[ieta])))
                if hup.GetBinContent(ieta+1)<0:
                    hup.SetBinContent(ieta+1 , 0)
                    hup.SetBinError(ieta+1 , 0)
                hdown.SetBinContent(ieta+1 , hdown.GetBinContent(ieta+1) * (1.0-abs(STATMAP[ieta])))
                hdown.SetBinError(ieta+1 , hdown.GetBinError(ieta+1) * (1.0-abs(STATMAP[ieta])))
                if hdown.GetBinContent(ieta+1)<0:
                    hdown.SetBinContent(ieta+1 , 0)
                    hdown.SetBinError(ieta+1 , 0)
            hup.Write(hup.GetTitle(),ROOT.TObject.kOverwrite)
            hdown.Write(hdown.GetTitle(),ROOT.TObject.kOverwrite)
            pass
    if fupd and fupd.IsOpen():
        fupd.Close()
    print >>f,'</BODY></HTML>'
