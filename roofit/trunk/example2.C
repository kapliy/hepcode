
double XFULLMIN = 0.;
double XFULLMAX = 200000.;
double XFITMIN = 60000.;
double XFITMAX = 120000.;

TFile* f = new TFile("file.root");
TTree* imassTree = (TTree*)f->Get("imassTree");

// --- imassTree has a branch called "imass"
// double m_imass;
// imassTree->SetBranchAdress("imass",&m_imass);
   
RooRealVar nsig("nsig","#signal events",1300,1,2000);
RooRealVar nbkg("nbkg","#background events",100,1,2000);
   
// --- Observable (same name as the tree branch)
RooRealVar imass("imass", "#hat{m}_{#mu#mu}", XFULLMIN, XFULLMAX, "MeV");   
imass.setRange("fitRange",XFITMIN,XFITMAX);
   
// --- Signal Parameters
RooRealVar gaussSigma("gaussSigma", "Resolution", 3000., 100., 10000.);
RooRealVar breitWignerMean("breitWignerMean", "m_{Z^{0}}", 91000., XFITMIN, XFITMAX);
RooRealVar breitWignerGamma("breitWignerGamma", "#Gamma", 2495.2);
// --- fix the BW width parameter to the known value (PDG)
breitWignerGamma.setConstant(kTRUE);
// --- Build the convolution of the Gauss and Breit-Wigner PDFs ---
RooVoigtian BreitGaussSignal("BreitGauss", "Breit-Wigner #otimes Gauss PDF", imass, breitWignerMean, breitWignerGamma, gaussSigma);

   
// --- Background Parameters
RooRealVar expMeasure("expMeasure", "Exponent measure", -1.e-6, -1.e-4, -1.e-8);
// --- Build the background exponential PDFs ---
RooExponential ExponentBG("ExponentBG", "Exponential BG", imass, expMeasure);
   
   
// --- redefine the generic PDFs in the fit range
RooExtendPdf sigE("sigE", "signalExtended",     BreitGaussSignal, nsig, "fitRange");
RooExtendPdf bkgE("bkgE", "backgroundExtended", ExponentBG,  nbkg, "fitRange");   
RooAddPdf model("model", "BreitGaussSignal #oplus ExponentBG", RooArgList(sigE,bkgE));
   
// --- for un-extended ML fit (not relevant here)
//RooRealVar fsig("fsig","signal fraction",0.9,0.,1.);
//RooAddPdf model("model", "BreitGaussSignal #oplus ExponentBG", RooArgList(BreitGaussSignal,ExponentBG), fsig);
   
// --- get the data set ---
RooDataSet* data = new RooDataSet("data", "data", imassTree, imass);
   
// --- Perform extended ML fit of composite PDF to data ---
model.fitTo(*data, Range("fitRange"), Extended(kTRUE));
   
   
// --- Perform a regular ML fit of composite PFD to data (not relevant here)
//model.fitTo(*data, Range("fitRange"));
      

// --- Plot toy data and composite PDF overlaid ---
TCanvas* canv_imass_roofit = new TCanvas("imass_roofit","imass_roofit",602,400);
canv_imass_roofit->Draw();
canv_imass_roofit->cd();
RooPlot* frame = imass.frame();
data->plotOn(frame, XErrorSize(0));
model.plotOn(frame);
model.plotOn(frame,Components(BreitGaussSignal),LineColor(kRed),LineWidth(1));
model.plotOn(frame,Components(ExponentBG),LineStyle(kDashed),LineWidth(1));
frame->SetMinimum(1.e-5);   
frame->Draw();
canv_imass_roofit->Update();
canv_imass_roofit->Write();
   
RooArgSet* params = model.getVariables();
params->Print("v");
