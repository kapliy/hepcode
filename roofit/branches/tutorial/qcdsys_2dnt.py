#!/usr/bin/env python

""" Systamatics on QCD in eta bins """

import pickle,os,sys

def make(bgsig,ivar):
    fitvars = ['met','wmt']
    fitranges = [(0,80),(40,90)]
    fitvar = fitvars[ivar]
    fitrange = fitranges[ivar]
    fitreg = 'metfit' if fitvar=='met' else 'baseline'
    assert len(fitrange)==2
    #TEST_Q3S2X5Y5Z5_isofail__tight_nominal_st_w_final_metfit_bin_8_lpt_4_NEG_met_0to100.png
    #TEST_Q3S5X5Y5Z5_loose_isofail__nominal_st_w_final_metfit_bin_0_lpt_4_POS_met_0to100.png
    n = 'Q3S%dX5Y5Z5_'%bgsig+'loose_isofail__nominal_st_w_final_' + fitreg + '_bin_%d_lpt_%d_%s_' + fitvar + '_' + str(fitrange[0]) + 'to' + str(fitrange[1])
    f = open('qcd.html','w')
    etabins = [0.0,0.21,0.42,0.63,0.84,1.05,1.37,1.52,1.74,1.95,2.18,2.4]
    ptbins = [20,25,30,35,40,45,50,120]
    print >>f,'<HTML><BODY>'
    for iq in ('POS','NEG'):
        print >>f,'<HR>'
        print >>f,iq
        print >>f,'<HR>'
        #print table
        print >>f,'<TABLE border="1" width="900">'
        print >>f,'<TR>'
        print >>f, '<TD width="100">pT/eta</TD>'
        for ipt in xrange(0,len(ptbins)-1):
            print >>f,'<TD width="50">','%d&lt;pT&lt;%d'%(ptbins[ipt],ptbins[ipt+1]),'</TD>'
        print >>f,'</TR>'
        for ieta in xrange(0,len(etabins)-1):
            print >>f,'<TR>'
            print >>f, '<TD width="100">','%.2f&lt;|eta|&lt;%.2f'%(etabins[ieta],etabins[ieta+1]),"</TD>"
            for ipt in xrange(0,len(ptbins)-1):
                key = n%(ieta,ipt,iq)
                v = -1.0
                if key in p:
                    v = p[key][0]  #scale factor
                    #v = p[key][2]*100.0  #fraction
                print >>f,'<TD width="50">','%.3f'%(v),'</TD>'
            print >>f,'</TR>'
        print >>f,'</TABLE>'
    print >>f,'</BODY></HTML>'
    f.close()


pf = open('data.pkl', 'rb')
p = pickle.load(pf)

make(5,1)
