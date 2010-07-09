#!/usr/bin/env python
import sys,os,re

class MCR:
    """ dataset """
    def __init__(s,rnum,sample,tag,xsec,filteff,nevents,ntupleDS=None):
        s.rnum = rnum
        s.sample = sample
        s.tag = tag
        s.nevents = nevents
        s.filteff = filteff
        s.xsec = xsec   # in nanobarns   # /1000.0 # convert nanobarns to microbarns
        s.ntupleDS=ntupleDS
    def dq2_ls(s):
        print 'Trying to find',s.rnum
        os.system('dq2-ls -L ROAMING '+s.dataset_pattern%(s.rnum,s.sample,s.tag))
    def pathena_submit(s):
        print './grid_submit '+s.dataset_pattern%(s.rnum,s.sample,s.tag)+' --nEventsPerJob 30000'
    def path(s):
        return '/pnfs/uct3/data/users/antonk/ANALYSIS/ICHEP_MC/'+s.ntupleDS+'/*root*'

class MC09_samples:
    def __init__(s):
        s.runs = []
    def append(s,run):
        run.dataset_pattern='mc09_7TeV.%d.%s.merge.AOD.%s/'
        run.ntupleDS='user.ponyisi.'+(run.dataset_pattern.strip('/'))%(run.rnum,run.sample,run.tag)+'.ntuple.v1_11'
        s.runs.append(run)
    def nruns(s):
        return len(s.runs)
    def nevents(s):
        return sum([r.nevents for r in s.runs])
    def match_run(s,path):
        runs = [a for a in s.runs if re.search('%s'%a.rnum,str(path))]
        if len(runs)==1:
            return runs[0]
        return None
    def get_xsec(s,path):
        return s.match_run(path).xsec
    def get_nevents(s,path):
        return s.match_run(path).nevents

mc09 = MC09_samples()
mc09.append(MCR(105001,'pythia_minbias','*',48445000.000000,1.000000,9996249))
mc09.append(MCR(106047,'PythiaZmumu_no_filter','*',0.851011,1.000000,4998410))
mc09.append(MCR(106088,'McAtNloZmumu_no_filter','*',0.952526,1.000000,4990000))
mc09.append(MCR(106044,'PythiaWmunu_no_filter','*',8.894060,1.000000,6993798))
mc09.append(MCR(106083,'McAtNloWplusmunu_no_filter','*',5.868290,1.000000,3996532))
mc09.append(MCR(106084,'McAtNloWminmunu_no_filter','*',3.996130,1.000000,2997334))
mc09.append(MCR(108319,'PythiaDrellYan_mumu','*',1.249550,1.000000,999503))
mc09.append(MCR(108321,'PythiaDrellYanLowM_mu3','*',4.415710,0.511700,499925))
mc09.append(MCR(106022,'PythiaWtaunu_1Lepton','*',8.916330,0.876600,999874))
mc09.append(MCR(106052,'PythiaZtautau','*',0.854173,1.000000,1998598))
mc09.append(MCR(106062,'McAtNloZtautau','*',0.952427,1.000000,1998040))
mc09.append(MCR(105861,'TTbar_PowHeg_Pythia','*',0.145642,0.538200,199838))
mc09.append(MCR(108405,'PythiaB_bbmu15X','*',73.900000,1.000000,4443898))
mc09.append(MCR(106059,'PythiaB_ccmu15X','*',28.400000,1.000000,1499257))
mc09.append(MCR(105009,'J0_pythia_jetjet','*',9752970.000000,1.000000,399993))
mc09.append(MCR(105010,'J1_pythia_jetjet','*',673020.000000,1.000000,396793))
mc09.append(MCR(105011,'J2_pythia_jetjet','*',41194.700000,1.000000,399691))
mc09.append(MCR(105012,'J3_pythia_jetjet','*',2193.250000,1.000000,399891))
mc09.append(MCR(105013,'J4_pythia_jetjet','*',87.848700,1.000000,398944))
mc09.append(MCR(105014,'J5_pythia_jetjet','*',2.328560,1.000000,399289))
mc09.append(MCR(105015,'J6_pythia_jetjet','*',0.033846,1.000000,393341))
mc09.append(MCR(109276,'J0_pythia_jetjet_1muon','*',9752970.000000,0.000079,500000))
mc09.append(MCR(109277,'J1_pythia_jetjet_1muon','*',673020.000000,0.001233,500000))
mc09.append(MCR(109278,'J2_pythia_jetjet_1muon','*',41194.700000,0.005443,500000))
mc09.append(MCR(109279,'J3_pythia_jetjet_1muon','*',2193.250000,0.012949,500000))
mc09.append(MCR(109280,'J4_pythia_jetjet_1muon','*',87.848700,0.022156,500000))
mc09.append(MCR(109281,'J5_pythia_jetjet_1muon','*',2.328560,0.029753,500000))
mc09.append(MCR(109282,'J6_pythia_jetjet_1muon','*',0.033846,0.03,500000))  # filter eff unknown!

if False:
    print 'Registered',mc09.nruns(),'runs:'
    print 'nevents   =',mc09.nevents()
    #[r.pathena_submit() for r in mc09.runs]
    #for r in mc09.runs: print r.path()
