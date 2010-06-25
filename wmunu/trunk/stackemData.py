#!/usr/bin/env python
try:
    import psyco
    psyco.full()
    print 'Using psyco'
except:
    pass

import math,sys,glob,re
import ROOT, PyCintex
PyCintex.Cintex.Enable()

ROOT.gROOT.SetBatch(1) #uncomment for interactive usage
filenames=['/home/melachri/output10/105861/out.root','/home/melachri/output10/105985/out.root','/home/melachri/output10/105986/out.root','/home/melachri/output10/105987/out.root','/home/melachri/output10/106280/out.root','/home/melachri/output10/106281/out.root','/home/melachri/output10/106282/out.root','/home/melachri/output10/106283/out.root','/home/melachri/output10/107650/out.root','/home/melachri/output10/107651/out.root','/home/melachri/output10/107652/out.root','/home/melachri/output10/107653/out.root','/home/melachri/output10/107654/out.root','/home/melachri/output10/107655/out.root','/home/melachri/output10/107660/out.root','/home/melachri/output10/107661/out.root','/home/melachri/output10/107662/out.root','/home/melachri/output10/107663/out.root','/home/melachri/output10/107664/out.root','/home/melachri/output10/107665/out.root','/home/melachri/output10/107670/out.root','/home/melachri/output10/107671/out.root','/home/melachri/output10/107672/out.root','/home/melachri/output10/107673/out.root','/home/melachri/output10/107674/out.root','/home/melachri/output10/107675/out.root','/home/melachri/output10/107680/out.root','/home/melachri/output10/107681/out.root','/home/melachri/output10/107682/out.root','/home/melachri/output10/107683/out.root','/home/melachri/output10/107684/out.root','/home/melachri/output10/107685/out.root','/home/melachri/output10/107690/out.root','/home/melachri/output10/107691/out.root','/home/melachri/output10/107692/out.root','/home/melachri/output10/107693/out.root','/home/melachri/output10/107694/out.root','/home/melachri/output10/107695/out.root','/home/melachri/output10/107700/out.root','/home/melachri/output10/107701/out.root','/home/melachri/output10/107702/out.root','/home/melachri/output10/107703/out.root','/home/melachri/output10/107704/out.root','/home/melachri/output10/107705/out.root','/home/melachri/output10/108340/out.root','/home/melachri/output10/108341/out.root','/home/melachri/output10/108342/out.root','/home/melachri/output10/108343/out.root','/home/melachri/output10/108344/out.root','/home/melachri/output10/108345/out.root','/home/melachri/output10/108346/out.root','/home/melachri/output10/108405/out.root']

colors=[3,2,2,2,5,5,5,5,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,8,8,8,8,8,8,8]

vars=["RMET","RW_mass","NRJETS","RW_massAfter","RMU0_pt","RJET0_pt"]
stack=[None]*(len(vars))
f=[None]*(len(vars))
h=[None]*(len(vars))
c=[None]*(len(vars))
f0=[None]*(len(vars))
h0=[None]*(len(vars))

# after GRL
filenames0=["/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152166.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152214.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152221.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152345.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152409.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152441.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152508.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152777.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152844.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152845.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152878.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152933.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00152994.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00153030.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00153134.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00153136.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00153159.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00153200.physics_MuonswBeam.merge.AOD.r1239_p134.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00153565.physics_MuonswBeam.merge.AOD.f251_m466.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00153599.physics_MuonswBeam.merge.AOD.f251_m466.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00154810.physics_MuonswBeam.merge.AOD.f255_m471.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00154813.physics_MuonswBeam.merge.AOD.f255_m471.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00154815.physics_MuonswBeam.merge.AOD.f255_m471.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00154817.physics_MuonswBeam.merge.AOD.f255_m471.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00154822.physics_MuonswBeam.merge.AOD.f255_m471.20100515_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00155073.physics_MuonswBeam.merge.AOD.f255_m471.20100521_v2_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00155112.physics_MuonswBeam.merge.AOD.f255_m471.20100521_v2_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00155116.physics_MuonswBeam.merge.AOD.f255_m471.20100521_v2_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00155118.physics_MuonswBeam.merge.AOD.f255_m471.20100521_v2_D2AODM_TOPMU/out.root","/home/melachri/outputDATA/grluser.Constantinos.group10.phys-top.D2PD_rel15.6.8.2.1_data10_7TeV.00155160.physics_MuonswBeam.merge.AOD.f255_m471.20100521_v2_D2AODM_TOPMU/out.root"]
    

for j in range(0,len(vars)):
    stack[j]=ROOT.THStack(vars[j],vars[j] + ' ')
    f[j]=[None]*(len(filenames))
    h[j]=[None]*(len(filenames))
    f0[j]=[None]*(len(filenames0))

    order=[4,5,6,7,1,2,3,44,45,46,47,48,49,50,0]
    for i in range(8,43):
        order.append(i)

    for i in order:
        f[j][i]=(ROOT.TFile(filenames[i]))
        h[j][i]=(ROOT.TH1F(f[j][i].Get(vars[j])))
        h[j][i].SetFillColor(colors[i])
        h[j][i].SetMarkerColor(colors[i])
        h[j][i].SetLineColor(colors[i])
        h[j][i].Scale(8929.58/10000000)
        stack[j].Add(h[j][i])

    for i in range(0,len(filenames0)):
        f0[j][i]=(ROOT.TFile(filenames0[i]))
        if i==0:
            h0[j]=(ROOT.TH1F(f0[j][i].Get(vars[j])))
        else:
            h0[j].Add(ROOT.TH1F(f0[j][i].Get(vars[j])))

    c[j] = ROOT.TCanvas('c'+str(j),vars[j] + 'hists',10,10,700,900)
    c[j].SetLogy(1)
    stack[j].SetMinimum(0.1)
    h0[j].SetFillColor(0)
    h0[j].SetMarkerColor(1)
    h0[j].SetMarkerStyle(4)
    #h0[j].Scale(10/1.058)
    h0[j].Draw("H")
    stack[j].Draw("H,same")
    leg = ROOT.TLegend(0.6,0.7,0.98,0.96)
    leg.AddEntry(h0[j],"topmixing","P")
    leg.AddEntry(h[j][26],"W+jets","F")
    leg.AddEntry(h[j][8],"Z+jets","F")
    leg.AddEntry(h[j][0],"ttbar","F")
    leg.AddEntry(h[j][44],"single top","F")
    leg.AddEntry(h[j][1],"WW/WZ/ZZ","F")
    leg.AddEntry(h[j][4],"Wbb+jets","F")
    leg.Draw("same")
    ROOT.gStyle.SetOptStat(0)
    c[j].SaveAs(vars[j] + '.png')

