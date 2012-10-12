#!/usr/bin/env python

import sys,os,re,urllib2
import BeautifulSoup

""" Example of extracting metadata from job page """

def get_inout(s,link):
    link = 'http://panda.cern.ch/server/pandamon/query?job=1452626250'
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
                return inp,out
    return None
