from ROOT import *
hh=TH1F("hh", "test histo", 100, -5, 5)
hh.FillRandom("gaus", 10000)

x=RooRealVar("x", "x", -5, 5)

data=RooDataHist("data", "dataset with x", RooArgList(x), hh)

frame=x.frame()
RooAbsData.plotOn( data, frame )
frame.Draw()
