#!/usr/bin/env python
from datetime import date
import sys,os

# all lumi numbers are in mubars
# Run, date, nevents, intlumi, LBGRL, intlumiGRL,

class LHCR:
    """ LHC run """
    def __init__(s,rnum,d,nevents,lumi,GRL,lumiGRL,tag):
        s.rnum = rnum
        s.date = d
        s.nevents = nevents
        s.lumi = lumi
        s.GRL = GRL
        s.lumiGRL = lumiGRL
        s.tag = tag
        # to be defined manually, if needed:
        s.dataset_pattern = None
        s.local_pattern = None
        s.ntupleDS = None
    def dq2_ls(s):
        print 'Trying to find',s.rnum
        os.system('dq2-ls -L ROAMING '+s.dataset_pattern%(s.rnum,s.tag))
    def pathena_submit(s):
        print './grid_submit '+s.dataset_pattern%(s.rnum,s.tag)+' --nEventsPerJob 100000'

class PLHC_runs:
    def __init__(s):
        s.runs = []
    def append(s,run):
        run.dataset_pattern='data10_7TeV.00%d.physics_MuonswBeam.merge.AOD.%s/'
        run.local_pattern='dcache:///pnfs/uct3/data/users/antonk/ANALYSIS/PLHC/%d/*root*'
        s.runs.append(run)
    def nruns(s):
        return len(s.runs)
    def nevents(s):
        return sum([r.nevents for r in s.runs])
    def lumi(s):
        return sum([r.lumi for r in s.runs])
    def lumiGRL(s):
        return sum([r.lumiGRL for r in s.runs])

class MCR:
    """ dataset """
    def __init__(s,rnum,sample,tag,nevents,filteff,xsec,ntupleDS=None):
        s.rnum = rnum
        s.sample = sample
        s.tag = tag
        s.nevents = nevents
        s.filteff = filteff
        s.xsec = xsec/1000.0  # convert nanobarns to microbarns
        s.ntupleDS=ntupleDS
    def dq2_ls(s):
        print 'Trying to find',s.rnum
        os.system('dq2-ls -L ROAMING '+s.dataset_pattern%(s.rnum,s.sample,s.tag))
    def pathena_submit(s):
        print './grid_submit '+s.dataset_pattern%(s.rnum,s.sample,s.tag)+' --nEventsPerJob 100000'

class MC09_samples:
    def __init__(s):
        s.runs = []
    def append(s,run):
        run.dataset_pattern='mc09_7TeV.%d.%s.merge.AOD.%s/'
        run.local_pattern='dcache:///pnfs/uct3/data/users/antonk/ANALYSIS/PLHC_MC/%d/*root*'
        run.ntupleDS='user10.Anton.Kapliy.'+run.dataset_pattern.strip('/')+'.ntuple.v1_7'
        s.runs.append(run)
    def nruns(s):
        return len(s.runs)
    def nevents(s):
        return sum([r.nevents for r in s.runs])

plhc = PLHC_runs()
tag='r1297_p157'
plhc.append(LHCR(152844,date(2010,04,11),7512,7.14,(195,234),3.3,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00152844.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'
plhc.append(LHCR(152845,date(2010,04,12),14675,24.11,(111,349),24.0,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00152845.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'
plhc.append(LHCR(152878,date(2010,04,13),22992,22.64,(99,214),22.8,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00152878.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'  # fail to dq2-get ANALY_GRIF-IRFU
#plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00152878.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7.retry2' # fail to dq2-get
#plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00152878.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7.retry3'
plhc.append(LHCR(152933,date(2010,04,14),9413,18.34,(46,173),18.4,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00152933.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'
plhc.append(LHCR(152994,date(2010,04,14),21936,4.74,(293,353),4.8,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00152994.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'
plhc.append(LHCR(153030,date(2010,04,16),11641,20.08,(120,203),20.1,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153030.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'
plhc.append(LHCR(153134,date(2010,04,16),20610,27.33,(354,573),23.3,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153134.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7' #FREIBURG
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153134.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7.retry' #FREIBURG
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153134.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7.retry2'
plhc.append(LHCR(153136,date(2010,04,17),8866,1.82,(249,253),0.6,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153136.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7' #infinite delay on start
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153136.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7.retry'
plhc.append(LHCR(153159,date(2010,04,18),6683,10.08,(87,177),10.2,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153159.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'
plhc.append(LHCR(153200,date(2010,04,18),5696,6.97,(145,178),6.6,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153200.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'
plhc.append(LHCR(153565,date(2010,04,23),114207,661.83,(258,953),575.6,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153565.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'
plhc.append(LHCR(153599,date(2010,04,23),18047,45.64,(399,440),27.9,tag))
plhc.runs[-1].ntupleDS='user10.AntonKapliy.data10_7TeV.00153599.physics_MuonswBeam.merge.AOD.r1297_p157.ntuple.v1_7'

if False:
    print 'Registered',plhc.nruns(),'runs:'
    print 'nevents   =',plhc.nevents()
    print 'lumi      =',plhc.lumi()
    print 'lumi(GRL) =',plhc.lumiGRL()
    #[r.pathena_submit() for r in plhc.runs]
    for r in plhc.runs:
        if r.rnum!=153134 and r.rnum!=153136 and r.rnum!=152878:   #not finished yet / failed / failed
            print r.ntupleDS
            cmd =  ['cd /scratch/antonk/']
            cmd += ['dq2-get -L ROAMING '+ r.ntupleDS]
            cmd += ['mkdir -p /pnfs/uct3/data/users/antonk/ANALYSIS/PLHC/'+str(r.rnum)]
            cmd += ['for f in '+r.ntupleDS+'/*root*; do dccp $f /pnfs/uct3/data/users/antonk/ANALYSIS/PLHC/'+str(r.rnum)+'/ ; done']
            os.system(';'.join(cmd))

mc09 = MC09_samples()
mc09.append(MCR(105001,'pythia_minbias','e517_s764_s767_r1204_r1210',48445000.000000,1.000000,1))
mc09.append(MCR(106047,'PythiaZmumu_no_filter','e468_s765_s767_r1207_r1210',0.851011,1.000000,1))
mc09.append(MCR(106088,'McAtNloZmumu_no_filter','e521_s765_s767_r1207_r1210',0.952526,1.000000,1))
mc09.append(MCR(106044,'PythiaWmunu_no_filter','e468_s765_s767_r1207_r1210',8.894060,1.000000,1))
mc09.append(MCR(106083,'McAtNloWplusmunu_no_filter','e521_s765_s767_r1207_r1210',5.868290,1.000000,1))
mc09.append(MCR(106084,'McAtNloWminmunu_no_filter','e521_s765_s767_r1207_r1210',3.996130,1.000000,1))
mc09.append(MCR(108319,'PythiaDrellYan_mumu','e518_s765_s767_r1207_r1210',1.249550,1.000000,1))
mc09.append(MCR(108321,'PythiaDrellYanLowM_mu3','e518_s765_s767_r1207_r1210 ',4.415710,0.511700,0))
mc09.append(MCR(106022,'PythiaWtaunu_1Lepton','e468_s765_s767_r1207_r1210',8.916330,0.876600,0))
mc09.append(MCR(106052,'PythiaZtautau','e468_s765_s767_r1205_r1210',0.854173,1.000000,1))
mc09.append(MCR(106062,'McAtNloZtautau','e521_s765_s767_r1205_r1210',0.952427,1.000000,1))
mc09.append(MCR(105861,'TTbar_PowHeg_Pythia','e521_s765_s767_r1207_r1210',0.145642,0.538200,0))
mc09.append(MCR(108405,'PythiaB_bbmu15X','e521_s765_s767_r1207_r1210',73.900000,1.000000,1))
mc09.append(MCR(106059,'PythiaB_ccmu15X','e521_s765_s767_r1207_r1210',28.400000,1.000000,1))
mc09.append(MCR(105009,'J0_pythia_jetjet','e468_s766_s767_r1206_r1210',9752970.000000,1.000000,1))
mc09.append(MCR(105010,'J1_pythia_jetjet','e468_s766_s767_r1206_r1210',673020.000000,1.000000,1))
mc09.append(MCR(105011,'J2_pythia_jetjet','e468_s766_s767_r1206_r1210',41194.700000,1.000000,1))
mc09.append(MCR(105012,'J3_pythia_jetjet','e468_s766_s767_r1206_r1210',2193.250000,1.000000,1))
mc09.append(MCR(105013,'J4_pythia_jetjet','e468_s766_s767_r1206_r1210',87.848700,1.000000,1))
mc09.append(MCR(105014,'J5_pythia_jetjet','e468_s766_s767_r1206_r1210',2.328560,1.000000,1))
mc09.append(MCR(105015,'J6_pythia_jetjet','e468_s766_s767_r1206_r1210',0.033846,1.000000,1))
mc09.append(MCR(109276,'J0_pythia_jetjet_1muon','e534_s765_s767_r1250_r1260',9752970.000000,0.000079,0))
mc09.append(MCR(109277,'J1_pythia_jetjet_1muon','e534_s765_s767_r1250_r1260',673020.000000,0.001233,0))
mc09.append(MCR(109278,'J2_pythia_jetjet_1muon','e534_s765_s767_r1250_r1260',41194.700000,0.005443,0))
mc09.append(MCR(109279,'J3_pythia_jetjet_1muon','e534_s765_s767_r1250_r1260',2193.250000,0.012949,0))
mc09.append(MCR(109280,'J4_pythia_jetjet_1muon','e534_s765_s767_r1250_r1260',87.848700,0.022156,0))
mc09.append(MCR(109281,'J5_pythia_jetjet_1muon','e534_s765_s767_r1250_r1260',2.328560,0.029753,0))

if True:
    print 'Registered',mc09.nruns(),'runs:'
    print 'nevents   =',mc09.nevents()
    [r.pathena_submit() for r in mc09.runs]
