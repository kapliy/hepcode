#!/usr/bin/env python

import BeautifulSoup
import sys,re,os,urllib2

i=0
def get_inout(link):
    global i
    i+=1
    page = urllib2.urlopen(link)
    bs = BeautifulSoup.BeautifulSoup(page)
    tb = bs.find('table',id='jobSpecTableID0')
    for row in tb.findAll('tr'):
        td = row.find('td')
        if td and len(td.contents)==1 and str(td.contents[0])=='jobParameters':
            tds = row.findAll('td')
            if len(tds)==2 and tds[1] and len(tds[1].contents)==1:
                val = str(tds[1].contents[0])
                elms = val.split()
                inp = [elm for elm in elms if re.match('inputAODFile',elm)]
                out = [elm for elm in elms if re.match('outputNTUP_SMWZFile',elm)]
                assert len(inp)==len(out)==1
                inp = inp[0].split('=')[1]
                out = out[0].split('=')[1]
                print '- %03d'%i,link,out
                return str(inp),str(out)
    assert False,'Could not find anything here'

def aod_expand(fin):
    try:
        f1 = fin.split('[')[0]
        fl = fin.split('[')[1].split(',')[0]
        fr = fin.split(']')[0].split(',')[1]
        f3 = fin.split(']')[1]
        OL = []
        ol = []
        ol.append(f1+fl+f3)
        ol.append(f1+fr+f3)
        OL += ol
        OL += [oo+'.1' for oo in ol]
        OL += [oo+'.2' for oo in ol]
        OL += [oo+'.3' for oo in ol]
        OL += [oo+'.4' for oo in ol]
        return ','.join(OL)
    except:
        return fin

dataset,link,pattern = None,None,None
if len(sys.argv)>1:
    if sys.argv[1]=='0':
        dataset = 'mc11_7TeV.108298.PowHegWplusmunuPythia.merge.AOD.e825_s1372_s1370_r3043_r2993/'
        link = 'http://panda.cern.ch/server/pandamon/query?job=*&taskID=742962'
        pattern = 'NTUP_SMWZ.742962._000396.root.1'
    elif sys.argv[1]=='1':
        dataset = 'mc11_7TeV.108301.PowHegWminmunuPythia.merge.AOD.e825_s1372_s1370_r3043_r2993/'
        link = 'http://panda.cern.ch/server/pandamon/query?job=*&taskID=742959'
        pattern = 'NTUP_SMWZ.742959._000893.root.1'
    elif sys.argv[1]=='2':
        dataset = 'data11_7TeV.00189875.physics_Muons.merge.AOD.f406_m991'
        link = 'http://panda.cern.ch/server/pandamon/query?job=*&taskID=621992'
        pattern = 'NTUP_SMWZ.621992._000012.root.1'
    else:
        assert False
else:
    assert False,'Missing argument'
print 'Looking for %s at %s'%(pattern,link)

page = urllib2.urlopen(link)
bs = BeautifulSoup.BeautifulSoup(page)
tb = bs.findAll('table',attrs={'border':1,'cellpadding':'2'})
assert len(tb)==1
tb = tb[0]
rows = tb.findAll('td',attrs={'rowspan':'2'})
links = [ str(row.contents[0]['href']) for row in rows ]
for link in links:
    aod,dpd = get_inout(link)
    if dpd==pattern:
        print pattern,' -> ',aod
        print 'dq2-get -f "'+aod_expand(aod)+'" '+dataset
        sys.exit(0)
