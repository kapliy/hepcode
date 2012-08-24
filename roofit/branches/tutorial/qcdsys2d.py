#!/usr/bin/env python

"""
A small script that summarizes QCD systematic deviations
due to different signal MC or different fit variables.
THIS VERSION OPERATES ON 2D GRID: |eta| x pT
"""

memo = """
<pre>
<U>In each cell</U>:<BR>
The <B>first number</B> is the <B>CHI2/NDF</B> for a given fit.<BR>
The <B>second number</B> is either the <B>absolute QCD fraction</B> (for the first, <B>nominal</B>, row), or the <B>relative deviation</B> wrt nominal (all other rows).<BR>
<U>Order of rows</U>:
<ul>
<li>Nom: Powheg+Pythia (MET) </li>
<li>Sys: Powheg+Herwig (MET) </li>
<li>Sys: MC@NLO (MET) </li>
<li>Sys: Powheg+Pythia (WMT) </li>
<li>Sys: Powheg+Herwig (WMT) </li>
<li>Sys: MC@NLO (WMT) </li>
<li></B>Maximum deviation (total QCD uncertainty)<B> </li>
</ul>
</pre>
"""

import sys,pickle,os

etabins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]
ptbins = [20,25,30,35,40,45,50,120]
QMAP = {0:'mu+',1:'mu-',2:'both charges'}
QMAPN = {0:'POS',1:'NEG',2:'ALL'}
SIGMAP = { 1 : "MC@NLO", 4 : "PowHeg+Her", 5 : "PowHeg+Pyt" }
S = '&nbsp;'

import ROOT
fupd_name = 'CRAP_QCD_W_PTCONE4.root'

if True:
    fin = open('save.pickle','rb')
    R = pickle.load(fin)
    f = open('index2.html','w')
    fupd = None
    if os.path.exists(fupd_name):
        fupd = ROOT.TFile.Open(fupd_name,"UPDATE")
    print >>f,'<HTML><BODY>'
    print >>f,memo
    #RES[iq][ieta][ipt][ivar][bgsig]
    for iq in (0,1,):
        if iq!=0:
            print >>f,'<HR size="20" color="red">'
        # make a separate table for each pT bin
        for ipt in xrange(0 , len(ptbins)-1):
            print 'Working on: ',QMAP[iq],'in pt bin',ipt
            print >>f,'<HR>'
            print >>f,QMAP[iq],'%d&lt;pT&lt;%d'%(ptbins[ipt],ptbins[ipt+1])
            print >>f,'<HR>'
            print >>f,'<TABLE border="1" CELLPADDING="0" CELLSPACING="1" width="1200">'
            print >>f,'<TR>'
            for ieta in xrange(0,len(etabins)-1):
                print >>f, '<TD width="80">','%.2f&lt;|&eta;|&lt;%.2f'%(etabins[ieta],etabins[ieta+1]),"</TD>"
            print >>f,'</TR>'
            # actual table contents
            print >>f,'<TR>'
            for ieta in xrange(0,len(etabins)-1):
                nom = R[iq][ieta][ipt]['met'][5][0]
                scales = R[iq][ieta][ipt]['met'][5][1]
                #sys = [('wmt',5), ('met',4),('wmt',4), ('met',1),('wmt',1)]
                sys = [('met',4),('met',1),('wmt',5),('wmt',4),('wmt',1)]
                fname = 'TEST/TEST_Q3S%dX5Y5Z5_loose_isofail__nominal_st_w_final_metfit_%s_bin_%d_lpt_%d_met_0to80.png'%(5,QMAPN[iq],ieta,ipt)
                print >>f,'<TD>','%.1f | <B><a href="%s">%.2f%%</a></B>'%(scales[-2]/scales[-1],fname,nom*100.0),'<BR>'
                pers = []
                for isys in sys:
                    ivar,bgsig = isys
                    fname = 'TEST/TEST_Q3S%dX5Y5Z5_loose_isofail__nominal_st_w_final_metfit_%s_bin_%d_lpt_%d_met_0to80.png'%(bgsig,QMAPN[iq],ieta,ipt)
                    if ivar=='wmt':
                        fname = 'TEST/TEST_Q3S%dX5Y5Z5_loose_isofail__nominal_st_w_final_baseline_%s_bin_%d_lpt_%d_wmt_40to90.png'%(bgsig,QMAPN[iq],ieta,ipt)
                    v=-1
                    try:
                        v = R[iq][ieta][ipt][ivar][bgsig][0]
                        scales = R[iq][ieta][ipt][ivar][bgsig][1]
                    except:
                        v=-1
                        scales = None
                    if v<0 or nom<0:
                        print >>f,'NA','<BR>'
                        pers.append(0)
                    else:
                        per = (v - nom)/nom*100.0
                        pers.append(per)
                        print >>f,'%.1f | <a href="%s">%s%.1f%%</a>'%(scales[-2]/scales[-1],fname,'' if per<0 else '+',per),'<BR>'
                pers.sort(key = lambda x: abs(x))
                print >>f,'<B>%s%.1f%% (%.1f%%)</B>'%('' if pers[-1]<0 else '+',pers[-1],abs(pers[-1])*nom)
                print >>f,'</TD>'
            print >>f,'</TR>'
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
