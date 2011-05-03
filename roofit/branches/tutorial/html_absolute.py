#!/usr/bin/env python

import sys,os

klus = ['','KLU_','AKLU_']
dets = ['cmb','id','exms']
regs = ["BB","Baa","Bcc","AA","MWA","FWA","CC","MWC","FWC"]  #["FWC0","FWC1","FWC2","FWC3","FWA0","FWA1","FWA2","FWA3"]
pattern = 'ZDATA_%sroot_all_0428.root_data_%s_%s_%s'
pattern = 'ZDATA_%sroot_all_0428.root_data_20110425_%s_%s_%s'

dets_map = {'cmb' : 'Combined muons', 'id' : 'Inner Detector muons', 'exms' : 'Muon Spectrometer muons'}
regs_map = {'AA' : 'Endcap A-side', 'BB' : 'Barrel', 'CC' : 'Endcap C-side', 'Bcc' : 'Barrel C-side', 'Baa' : 'Barrel A-side', 'FWC' : 'Endcap C-side (-2.4 < eta < -2.0)', 'FWA' : 'Endcap A-side (2.0 < eta < 2.4)', 'MWC' : 'Endcap C-side (-2.0 < eta < -1.05)', 'MWA' : 'Endcap A-side (1.05 < eta < 2.0)'}
klus_map = { '' : 'Both Z legs in range', 'KLU_' : 'At least one Z leg in range', 'AKLU_' : 'One Z leg in range, one not' }

cols = [ 'Z mass in MC', 'Z mass in data', 'Relative scale (k+/k-)','k+', 'stat. error on k+','k-','stat. error on k-']
def get_means(lines):
    """ Returns an array of means computed in different ways
    mz0 = 89.932
    mz = 87.566 +/- 0.999
    CHI2/NDF = 0.48, NDF = 8
    R = 0.974 +/- 0.008
    k+ = 96.071 +/- 1.195
    k- = 98.684 +/- 1.163
    """
    mz0 = float(lines[0][2])
    mz = float(lines[1][2])
    R = float(lines[3][2])
    kp = float(lines[4][2])
    ekp = float(lines[4][4])
    kn = float(lines[5][2])
    ekn = float(lines[5][4])
    return mz0,mz,R,kp,ekp,kn,ekn

res = {}
# load the data
for det in dets:
    res[det] = {}
    for reg in regs:
        res[det][reg] = {}
        for klu in klus:
            res[det][reg][klu] = None
            fname = pattern%(klu,det,reg,'results.rtxt')
            good = os.path.isfile(fname)
            if good:
                f = open(fname,'r')
                lines = []
                for l in f:
                    lines.append(l.split())
                res[det][reg][klu] = get_means(lines)

# format html table
NCOLS = len(cols)
for det in dets:
    print '<table border="5" width="70%" cellpadding="4" cellspacing="3">'
    print '<tr>'
    print '<TH COLSPAN="%d"><BR><H3>%s</H3></TH>'%(NCOLS+1,dets_map[det])
    print '</tr>'
    print '<tr>'
    print '<td>Variation</td>'
    for z in range(NCOLS):
        print '<td>%s</td>'%cols[z]
    print '</tr>'
    for reg in regs:
        print '<tr>'
        print '<TH COLSPAN="%d"><BR><H4>%s</H4></TH>'%(NCOLS+1,regs_map[reg])
        print '</tr>'
        for klu in klus:
            v = res[det][reg][klu]
            if v:
                print '<tr>'
                print '<td>%s</td>'%(klus_map[klu])
                print '<td>%.2f</td>'%(v[0])
                print '<td>%.2f</td>'%(v[1])
                print '<td>%.3f</td>'%(v[2])
                for z in range(3,NCOLS):
                    fname = pattern%(klu,det,reg,'fit.png')
                    print '<td><a href="%s">%.2f%%</a></td>'%(fname,float(v[z]))
                print '</tr>'
    print '</table>'
