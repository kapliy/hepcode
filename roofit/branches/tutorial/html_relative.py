#!/usr/bin/env python

import sys,os

klus = ['','KLU_','AKLU_']
dets = ['cmb','id','exms']
regs = ["BB","Baa","Bcc","AA","MWA","FWA","CC","MWC","FWC"]  #["FWC0","FWC1","FWC2","FWC3","FWA0","FWA1","FWA2","FWA3"]
pattern = '%sroot_all_0428.root_data_%s_data_%s_%s'

dets_map = {'cmb' : 'Combined muons', 'id' : 'Inner Detector muons', 'exms' : 'Muon Spectrometer muons'}
regs_map = {'AA' : 'Endcap A-side', 'BB' : 'Barrel', 'CC' : 'Endcap C-side', 'Bcc' : 'Barrel C-side', 'Baa' : 'Barrel A-side', 'FWC' : 'Endcap C-side (-2.4 < eta < -2.0)', 'FWA' : 'Endcap A-side (2.0 < eta < 2.4)', 'MWC' : 'Endcap C-side (-2.0 < eta < -1.05)', 'MWA' : 'Endcap A-side (1.05 < eta < 2.0)'}
klus_map = { '' : 'Both Z legs in range', 'KLU_' : 'At least one Z leg in range', 'AKLU_' : 'One Z leg in range, one not' }

#['0', 'peak', '=', '1.0118400000', '+/-', '0.0065396304', ';', 'chi2', '=', '8.0096263055', ';', 'fitted_mean', '=', '1.0115227371', ';', 'avg_scale', '=', '1.0170994745', ';', 'avg_shift', '=', '0.7892375202', ';', 'nmax', '=', '615', ';', 'npos', '=', '615', ';', 'nneg', '=', '615']
cols = ['#events','stat. error', 'KS peak', 'KS fit', 'CHI peak', 'CHI fit', 'Simple average']
def get_means(lines):
    """ Returns an array of means computed in different ways """
    ks = lines[0]
    chi = lines[1]
    er=chi[5]
    N=int(ks[25])
    return (N,er,ks[3],ks[13],chi[3],chi[13],ks[17])

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
                print '<td>%d</td>'%(int(v[0]))
                print '<td>%.2f%%</td>'%(float(v[1])*100.0)
                for z in range(2,NCOLS):
                    fname = pattern%(klu,det,reg,'fit.png')
                    print '<td><a href="%s">%.2f%%</a></td>'%(fname,float(v[z])*100.0)
                print '</tr>'
    print '</table>'
