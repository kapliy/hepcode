#!/usr/bin/env python

from BeautifulSoup import BeautifulSoup

f=open("twiki.html")
lines = f.readlines()
soup = BeautifulSoup(''.join(lines))

rows = soup.table.findAll('tr')

for row in rows:
    cols = row.findAll('td')
    try:
        dataset = str(cols[0].contents[0])
        parts = dataset.split('.')
        rnum=int(parts[1])
        dname=parts[2]
        tag=parts[5].strip().strip('/')
        xsec = float(cols[1].contents[0])
        eff = float(cols[2].contents[0])
        nevents = float(cols[2].contents[0])
        print "mc09.append(MCR(%d,'%s','%s',%f,%f,%d))"%(rnum,dname,tag,xsec,eff,nevents)
    except:
        pass
