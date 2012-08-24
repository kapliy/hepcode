#!/bin/bash
for nn in 'nmu' 'njet' 'nph' 'nele' 'nvx' 'nmc' 'ntrk' 'njpsi'; do echo -n "$nn  "; cat VARMAX | grep "VAR $nn" | grep SAMPLE  | cut -d ' ' -f 6 | sort -n | tail -n1; done
