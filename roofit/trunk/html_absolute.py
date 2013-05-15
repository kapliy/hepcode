#!/usr/bin/env python

import sys,os,math

klus = ['','KLU_','AKLU_']
dets = ['cmb','exms','id']
regs = ["AA","BB","CC","Baa","Bcc","MWA","FWA","MWC","FWC"]  #["FWC0","FWC1","FWC2","FWC3","FWA0","FWA1","FWA2","FWA3"]
fname_pat='root_all_0428.root'
folder_pat='data'
if len(sys.argv)>=5:
    fname_pat=sys.argv[1]
    resmodel=sys.argv[2]
    folder_pat=sys.argv[3]
    mc_pat=sys.argv[4]
else:
    print >> sys.stderr, 'Not enough arguments!'
    sys.exit(0)
pattern = 'ZDATA_%s%s_%s_%s_%s_%s_%s_%s'
mc_pattern = 'ZMC_%s%s_%s_%s_%s_%s_%s_%s'

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
            fname = pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'results.rtxt')
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
                fname = pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'fit.png')
                mcfname = mc_pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'fit.png')
                print '<tr>'
                print '<td>%s</td>'%(klus_map[klu])
                print '<td><a href="%s">%.2f</a></td>'%(mcfname,v[0])
                print '<td><a href="%s">%.2f</a></td>'%(fname,v[1])
                print '<td>%.3f</td>'%(v[2])
                for z in range(3,NCOLS):
                    print '<td>%.2f%%</td>'%(float(v[z]))
                print '</tr>'
    print '</table>'

# create c++ array of scales
print '<BR>Default (low-stats) version<BR>'
klu=''
coarse,fine = [],[]
for det in dets:
    for reg in ('AA','BB','CC'):
        v = res[det][reg][klu]
        fname = pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'fit.png')
        if v:
            coarse.append('%.2f'%float(v[3]))
            coarse.append('%.2f'%float(v[5]))
        else:
            print >> sys.stderr, 'Missing:',fname
for det in dets:
    for reg in ('FWA','MWA','Baa','Bcc','MWC','FWC'):
        v = res[det][reg][klu]
        fname = pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'fit.png')
        if v:
            fine.append('%.2f'%float(v[3]))
            fine.append('%.2f'%float(v[5]))
        else:
            print >> sys.stderr, 'Missing:',fname
print '<BR>Coarse binning:<pre>'
for i,j in enumerate(coarse):
    print j,
    if i%2!=0:
        print ' , '
    else:
        print ' , ',
print '</pre>'

print '<BR>Fine binning:<pre>'
for i,j in enumerate(fine):
    print j,
    if i%2!=0:
        print ' , '
    else:
        print ' , ',
print '</pre>'

# create c++ array of scales
print '<BR>KLUIT (high-stats) version<BR>'
klu='KLU_'
coarse,fine = [],[]
for det in dets:
    for reg in ('AA','BB','CC'):
        v = res[det][reg][klu]
        fname = pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'fit.png')
        if v:
            coarse.append('%.2f'%float(v[3]))
            coarse.append('%.2f'%float(v[5]))
        else:
            print >> sys.stderr, 'Missing:',fname
for det in dets:
    for reg in ('FWA','MWA','Baa','Bcc','MWC','FWC'):
        v = res[det][reg][klu]
        fname = pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'fit.png')
        if v:
            fine.append('%.2f'%float(v[3]))
            fine.append('%.2f'%float(v[5]))
        else:
            print >> sys.stderr, 'Missing:',fname
print '<BR>Coarse binning:<pre>'
for i,j in enumerate(coarse):
    print j,
    if i%2!=0:
        print ' , '
    else:
        print ' , ',
print '</pre>'
print '<BR>Fine binning:<pre>'
for i,j in enumerate(fine):
    print j,
    if i%2!=0:
        print ' , '
    else:
        print ' , ',
print '</pre>'


# create c++ array of scales
print '<BR>Version without Z mass constraint (assuming k+ * k- = 1.0)<BR>'
klu=''
coarse,fine = [],[]
for det in dets:
    for reg in ('AA','BB','CC'):
        v = res[det][reg][klu]
        fname = pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'fit.png')
        if v:
            coarse.append('%.2f'%(100.0*math.sqrt(float(v[2]))))
            coarse.append('%.2f'%(100.0/math.sqrt(float(v[2]))))
        else:
            print >> sys.stderr, 'Missing:',fname
for det in dets:
    for reg in ('FWA','MWA','Baa','Bcc','MWC','FWC'):
        v = res[det][reg][klu]
        fname = pattern%(klu,fname_pat,folder_pat,mc_pat,det,reg,resmodel,'fit.png')
        if v:
            fine.append('%.2f'%(100.0*math.sqrt(float(v[2]))))
            fine.append('%.2f'%(100.0/math.sqrt(float(v[2]))))
        else:
            print >> sys.stderr, 'Missing:',fname
print '<BR>Coarse binning:<pre>'
for i,j in enumerate(coarse):
    print j,
    if i%2!=0:
        print ' , '
    else:
        print ' , ',
print '</pre>'
print '<BR>Fine binning:<pre>'
for i,j in enumerate(fine):
    print j,
    if i%2!=0:
        print ' , '
    else:
        print ' , ',
print '</pre>'
