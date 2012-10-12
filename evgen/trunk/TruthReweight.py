#!/usr/bin/env python

"""
A simple mechanism to create reweighting histograms between different truth Monte-Carlos
"""

import os,sys,re,glob
if not 'libPyRoot' in sys.modules: #hack to get rid of TenvRec warnings
    from ROOT import gROOT
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorIgnoreLevel').GetAddress().__setitem__(0,3000)
    import ROOT
    gErrorIgnoreLevel=ROOT.kInfo
    sys.modules['libPyROOT'].gROOT.GetListOfGlobals().FindObject('gErrorAbortLevel').GetAddress().__setitem__(0,5001)
    ROOT.gROOT.ProcessLine('.L loader.C+')
    ROOT.gROOT.ProcessLine(
        "struct CandStruct {\
        Int_t event;\
        Int_t w_q;\
        Float_t w_pt;\
        Float_t w_eta;\
        Float_t w_phi;\
        Float_t w_e;\
        Int_t mu_q;\
        Float_t mu_pt;\
        Float_t mu_eta;\
        Float_t mu_phi;\
        Float_t mu_e;\
        Float_t nu_pt;\
        Float_t nu_eta;\
        Float_t nu_phi;\
        Float_t nu_e;\
        };" );

GEV=1000.0
FLAT,D3PD = range(2)
PYTHIA,HERWIG = range(2)

class TreeReader:
    """ Interface to either a D3PD or FlatNtupler tree """
    def __init__(s,fname='output.root'):
        """ Dummy constructor """
        s.ntuple = None
        s.gen = None
        s.tree = None
        s.files = []
        s.evt = 0
        s.nskip = 0
        # current-event variables
        s.ok = False
        s.mu  = ROOT.TLorentzVector()
        s.w  = ROOT.TLorentzVector()
        s.nu  = ROOT.TLorentzVector()
        s.met = ROOT.TLorentzVector()
        # output: trees
        if os.path.exists(fname):
            print 'WARNING: overwriting output file',fname
        else:
            print 'INFO: saving output tree under:',fname
        s.fout = ROOT.TFile.Open(fname,'RECREATE')
        s.fout.SetCompressionLevel(3)
        s.fout.cd()
        s.tout = ROOT.TTree('tree','Tree with truth-level mu and nu 4vectors')
        s.cand = ROOT.CandStruct()
        s.tout.Branch("tree",s.cand,'event/I:w_q/I:w_pt/F:w_eta/F:w_phi/F:w_e/F:mu_q/I:mu_pt/F:mu_eta/F:mu_phi/F:mu_e/F:nu_pt/F:nu_eta/F:nu_phi/F:nu_e/F')
        # output: histograms
        s.hmupt = ROOT.TH1F('hmupt','hmupt',80,0,200) #2.5 GeV bin width
        s.hnupt = ROOT.TH1F('hnupt','hnupt',80,0,200) #2.5 GeV bin width
        s.hwpt = ROOT.TH1F('hwpt','hwpt',80,0,200) #2.5 GeV bin width
    def add_files(s,rawfiles):
        """ Add a list of files to TChain """
        def wrap(x):
            new = 'root://xrddc.mwt2.org:1096//atlas/dq2/'
            old='/pnfs/uchicago.edu/atlaslocalgroupdisk/'
            if re.search(old,x):
                return re.sub(old,new,x)
            return x
        files = [wrap(ff) for ff in rawfiles]
        assert len(files)>0,'Add at least one file in add_files'
        print 'First file:',files[0]
        s.files = files[:]
        file = files[0]
        s.ntuple = FLAT if re.search('flatntuple',file) else D3PD
        s.gen = PYTHIA if re.search('ythia',file) else HERWIG
        tname = 'truth' if s.ntuple == D3PD else 'tree'
        s.tree = ROOT.TChain(tname,tname)
        [s.tree.AddFile(ff) for ff in files]
        # enable selected branches
        s.tree.SetBranchStatus('*',0)
        br = []
        if s.ntuple==D3PD:
            br.append('EventNumber')
            #br.append('mc_event_weight')
            br.append('mc_n')
            br.append('mc_pt')
            br.append('mc_eta')
            br.append('mc_phi')
            br.append('mc_E')
            br.append('mc_status')
            br.append('mc_pdgId')
            #br.append('mc_vx_z')
            br.append('mc_charge')
            br.append('mc_parent_index')
            #br.append('MET_Truth_NonInt_etx')
            #br.append('MET_Truth_NonInt_ety')
            #br.append('MET_Truth_Int_etx')
            #br.append('MET_Truth_Int_ety')
            #br+= ['mu_staco_truth_n','mu_staco_truth_pt','mu_staco_truth_eta','mu_staco_truth_phi','mu_staco_truth_E']
        else:
            br += ['nmc','mc_status','mc_pdgid','mc_e','mc_pt','mc_eta','mc_phi','mc_parent']
            #br += ['met_truth','met_truth_phi']
        [s.tree.SetBranchStatus(v,1) for v in br]
        # set up TTreeCache
        if True:
            cachesize = 100000000
            s.tree.SetCacheSize(cachesize);
            for sub in s.tree.GetListOfBranches():
                if s.tree.GetBranchStatus(sub.GetName()):
                    s.tree.AddBranchToCache(sub.GetName(),ROOT.kTRUE)
                else:
                    continue
            s.tree.StopCacheLearningPhase()
    def nevents(s):
        """ Total number of events in the initialized TChain """
        return s.tree.GetEntries()
    def get_event(s,ievt=None):
        """ Gets event ievt from input ntuple. If ievt is not given, gets the next event """
        if s.evt%100 == 0:
            print 'Event:',s.evt
            sys.stdout.flush()
        s.evt = ievt if ievt!=None else s.evt+1
        s.ok = False
        evt = s.evt - 1  # temporarily revert the premature increment
        s.tree.LoadTree(evt); 
        return s.tree.GetEntry(evt)
    def construct_objects(s):
        """ Reconstructs muon and neutrino objects for this event """
        t = s.tree
        mustats = (123,124) if s.gen==HERWIG else (3,)
        wpdg = (-24,24) if s.gen==HERWIG else (-24,24)
        wstats = 155 if s.gen==HERWIG else 3
        imus,inus,iws = [],[],[]
        if s.ntuple==D3PD:
            iws = [ i for i in xrange(0,t.mc_n) if abs(t.mc_pdgId[i])==24 and t.mc_status[i]==wstats ]
            imus = [ i for i in xrange(0,t.mc_n) if abs(t.mc_pdgId[i])==13 and t.mc_status[i] in mustats and t.mc_parent_index[i].size()>0 and t.mc_pdgId[t.mc_parent_index[i][0]] in wpdg]
            inus = [ i for i in xrange(0,t.mc_n) if abs(t.mc_pdgId[i])==14 and t.mc_status[i] in mustats and t.mc_parent_index[i].size()>0 and t.mc_pdgId[t.mc_parent_index[i][0]] in wpdg]
        else:
            iws = [ i for i in xrange(0,t.nmc) if abs(t.mc_pdgid[i])==24 and t.mc_status[i]==wstats ]
            imus = [ i for i in xrange(0,t.nmc) if abs(t.mc_pdgid[i])==13 and t.mc_status[i] in mustats and t.mc_parent[i]>=0 and t.mc_pdgid[t.mc_parent[i]] in wpdg]
            inus = [ i for i in xrange(0,t.nmc) if abs(t.mc_pdgid[i])==14 and t.mc_status[i] in mustats and t.mc_parent[i]>=0 and t.mc_pdgid[t.mc_parent[i]] in wpdg]
        if len(imus)==1 and len(inus)==1 and len(iws)==1:
            mc_pt = t.mc_pt
            mc_eta = t.mc_eta
            mc_phi = t.mc_phi
            mc_e = t.mc_E if s.ntuple==D3PD else t.mc_e
            mc_pdgid = t.mc_pdgId if s.ntuple==D3PD else t.mc_pdgid
            i = iws[0]
            s.w.SetPtEtaPhiE(mc_pt[i]/GEV if s.ntuple==D3PD else mc_pt[i],mc_eta[i],mc_phi[i],mc_e[i]/GEV if s.ntuple==D3PD else mc_e[i])
            s.w.q = -1 if mc_pdgid[i]==-24 else 1
            i = imus[0]
            s.mu.SetPtEtaPhiE(mc_pt[i]/GEV if s.ntuple==D3PD else mc_pt[i],mc_eta[i],mc_phi[i],mc_e[i]/GEV if s.ntuple==D3PD else mc_e[i])
            s.mu.q = -1 if mc_pdgid[i]==13 else 1
            i = inus[0]
            s.nu.SetPtEtaPhiE(mc_pt[i]/GEV if s.ntuple==D3PD else mc_pt[i],mc_eta[i],mc_phi[i],mc_e[i]/GEV if s.ntuple==D3PD else mc_e[i])
            s.ok = True
            # save output
            s.cand.event = t.EventNumber if s.ntuple==D3PD else t.event
            s.cand.w_q = s.w.q
            s.cand.w_pt = s.w.Pt()
            s.cand.w_eta = s.w.Eta()
            s.cand.w_phi = s.w.Phi()
            s.cand.w_e = s.w.E()
            s.cand.mu_q = s.mu.q
            s.cand.mu_pt = s.mu.Pt()
            s.cand.mu_eta = s.mu.Eta()
            s.cand.mu_phi = s.mu.Phi()
            s.cand.mu_e = s.mu.E()
            s.cand.nu_pt = s.nu.Pt()
            s.cand.nu_eta = s.nu.Eta()
            s.cand.nu_phi = s.nu.Phi()
            s.cand.nu_e = s.nu.E()
            s.tout.Fill()
            s.hwpt.Fill(s.cand.w_pt)
            s.hmupt.Fill(s.cand.mu_pt)
            s.hnupt.Fill(s.cand.nu_pt)
        else:
            print 'Bad event:',s.evt,'nmu =',len(imus),'nnu =',len(inus),'nw =',len(iws)
            sys.stdout.flush()
            s.nskip += 1
            s.ok = False
    def next(s):
        """ Load and process the next event """
        s.get_event()
        s.construct_objects()
    def finalize(s):
        """ Save output """
        print 'TOTAL EVENTS:',s.evt,' ERRORS:',s.nskip
        _file = s.tree.GetCurrentFile()
        print "INFO: File stats for",_file.GetName(), ":"
        print _file.GetBytesRead(),"bytes and",_file.GetReadCalls(),"calls"
        sys.stdout.flush()
        s.fout.cd()
        s.tout.Write()
        s.hwpt.Write()
        s.hmupt.Write()
        s.hnupt.Write()
        s.fout.Close()

if __name__=='__main__':
    files = []
    E1 = '/pnfs/uchicago.edu/atlaslocalgroupdisk/user/kapliy/UCNTUP/user.kapliy.UCNTUP.mc11_7TeV.108301.PowHegWminmunuPythia.merge.AOD.e825_s1372_s1370_r3043_r2993.j.mc11c.v1_29i.120811072540/user.kapliy.010569.flatntuple._00003.root.1'
    E2 = '/pnfs/uchicago.edu/atlaslocalgroupdisk/user/kapliy/UCNTUP/user.kapliy.UCNTUP.mc11_7TeV.113185.PowHegWminmunuJimmy.merge.AOD.e995_s1372_s1370_r3043_r2993.j.mc11c.v1_29i.120811074222/user.kapliy.010593.flatntuple._00004.root.1'
    E3 = '/pnfs/uchicago.edu/atlaslocalgroupdisk/user/mbellomo/mc11_7TeV/user.mbellomo.mc11_7TeV.113185.PowHegWminmunuJimmy.evgen.EVNT.e995_D3PD.120913100857/user.mbellomo.011296.EXT0._03451.pool.root'
    if len(sys.argv)>=2:
        x = sys.argv[1]
        if x=='1':
            print 'Running over: FLAT PowhegPythia (AOD reco)'
            files=[E1]
        elif x=='2':
            print 'Running over: FLAT PowhegHerwig (AOD reco)'
            files=[E2]
        elif x=='3':
            print 'Running over: D3PD PowhegHerwig (EVNT full stat)'
            files=[E3]
        else:
            files = [z for z in glob.glob(sys.argv[1])]
    else:
        files = [E1]
    outname = 'output.root'
    if len(sys.argv)>=3:
        outname = sys.argv[2]
    a = TreeReader(outname)
    a.add_files(files)
    [a.next() for z in xrange(a.nevents())]
    a.finalize()
    print 'DONE'
    sys.stdout.flush()
