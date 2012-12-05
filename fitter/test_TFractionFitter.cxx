/**

   Testing ROOT's TFractionFitter

   Sebastian Schmitt
   sebastian.schmitt@cern.ch

   July 2011

   g++ -g test_TFractionFitter.cxx `root-config --libs --ldflags --cflags` -o test_TFractionFitter

*/
#include <iostream>
#include <utility>
#include <cstdlib>
#include <vector>
#include <map>

#include <TMath.h>
#include <TH1D.h>
#include <THStack.h>
#include <TLegend.h>
#include <TRandom3.h>
#include <TFractionFitter.h>
#include <TFile.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TSystem.h>
#include <TStyle.h>

// magic number indication that the fit failed
const Double_t FIT_FAILED = -1;

// flag to set verbose mode
Bool_t verbose = kFALSE;

// contribution to data from mc0 distribution
UInt_t ndata_mc0;
// contribution to data from mc1 distribution
UInt_t ndata_mc1;
// corresponding fraction of mc1 distribution in data
Double_t f;

// number of entries in mc0 histogram
UInt_t nmc0;
// number of entries in mc1 histogram
UInt_t nmc1;

// mc0/1 distributions are gaussian with a width of one and the given means
// mean of mc0 distribution
const Double_t mean_mc0 = 4;
// mean of mc1 distribution
const Double_t mean_mc1 = 7;

// settings for all histograms
const Double_t xmin = -5;
const Double_t xmax = 20;
const Double_t nbins = 100;

// histograms
TH1D* data;
TH1D* mc0;
TH1D* mc1;

TH1D* pull = new TH1D("pull","pull", 200, -10, 10);

// number of fits to perform
UInt_t nfits;

TFractionFitter* fit = 0;

// resets and fills histograms
void fillHistos(Bool_t fData=kTRUE, Bool_t fmc0=kTRUE, Bool_t fmc1=kTRUE) {

    if(fData) data->Reset();
    if(fmc0) mc0->Reset();
    if(fmc1) mc1->Reset();

    if(fData) {
        for(UInt_t n=0; n != ndata_mc0; ++n) {
            data->Fill(gRandom->Gaus(mean_mc0,1));
        }

        for(UInt_t n=0; n != ndata_mc1; ++n) {
            data->Fill(gRandom->Gaus(mean_mc1,1));
        }
    }

    if(fmc0) {
        for(UInt_t n=0; n != nmc0; ++n) {
            mc0->Fill(gRandom->Gaus(mean_mc0,1));
        }
    }

    if(fmc1) {
        for(UInt_t n=0; n != nmc1; ++n) {
            mc1->Fill(gRandom->Gaus(mean_mc1,1));
        }
    }

}

std::map<int,bool> _excluded;

void include_all_bins() {
  int nbins = data->GetNbinsX();
  for(int ibin=0; ibin<=nbins; ibin++) {
    if(_excluded.find(ibin)!=_excluded.end()) fit->IncludeBin(ibin);
  }
  _excluded.clear();
}
int exclude_some_bins(double limit=0) {
  include_all_bins();
  int nbins = data->GetNbinsX();
  int nexcl=0;
  for(int ibin=0; ibin<=nbins; ibin++) {
    //    if( data->GetBinContent(ibin)<=limit || mc0->GetBinContent(ibin)<=limit || mc1->GetBinContent(ibin)<=limit ) {
    if( data->GetBinContent(ibin)<=limit) {
    //    if( mc0->GetBinContent(ibin)<=limit ) {
      fit->ExcludeBin(ibin);
      _excluded[ibin] = true;
      nexcl++;
    }
  }
  if(verbose) std::cout << "Excluded: " << nexcl << " bins" << std::endl;
  return nexcl;
}

// performs the fraction fit with TFractionFitters,
// returns fraction and error of mc1 contribution
std::pair<Double_t, Double_t> tfractionfitter() {
  
    fillHistos();

    exclude_some_bins(0);

    fit->Constrain(0,0.0,1.0);
    fit->Constrain(1,0.0,1.0);
    //fit->SetRangeX(0,100);

    UInt_t status = fit->Fit();

    const Bool_t first_fit_failed = (status != 0);

    if(first_fit_failed){

        if(verbose) std::cout << "first fit failed, try again - " << std::endl;

        status = fit->Fit();

        if(status != 0 && verbose ) {
            std::cout << "failed again" << std::endl;
        }

    } else {

        if(verbose) std::cout << "worked" << std::endl;

    }

    if(status == 0) {

        Double_t v0;
        Double_t e0;
        fit->GetResult(0, v0, e0);

        Double_t v1;
        Double_t e1;
        fit->GetResult(1, v1, e1);

        if(verbose) std::cout << "tfractionfitter " << f << " " << v1 << " " << e1 << " " << v0 << " " << e0 << std::endl;
        // return std::make_pair(v1, TMath::Sqrt(e1*e1+e0*e0));
        return std::make_pair(v1, e1);

    } else {

        return std::make_pair(FIT_FAILED, FIT_FAILED);

    }
}

// writes outfile.root with all histograms used and displays the pull distribution
int main(int argc, char* argv[]) {

    TApplication app("app", 0, 0);
    TH1::SetDefaultSumw2();

    if( argc < 6 || argc > 7 ) {

        std::cout << "Usage: " << argv[0] << " nfits ndata_mc0 ndata_mc1 nmc0 nmc1 (verbose)" << std::endl;
        gSystem->Exit(1);

    }

    nfits = atoi(argv[1]);
    ndata_mc0 = atoi(argv[2]);
    ndata_mc1 = atoi(argv[3]);
    nmc0 = atoi(argv[4]);
    nmc1 = atoi(argv[5]);

    if( argc > 6 ) {

        verbose = atoi(argv[6]);

    }

    f = ((Double_t)(ndata_mc1))/(ndata_mc0+ndata_mc1);

    std::cout << argv[0] << " running - exit with ctrl-c" << std::endl;
    std::cout << "number of fits: " << nfits << std::endl;
    std::cout << "number of entries in data histogram: " << ndata_mc0 + ndata_mc1 << std::endl;
    std::cout << "contribution to data from mc0 distribution: " << ndata_mc0 << std::endl;
    std::cout << "contribution to data from mc1 distribution: " << ndata_mc1 << std::endl;
    std::cout << "corresponding fraction of mc1 distribution in data: " << f << std::endl;
    std::cout << "number of entries in mc0 histogram: " << nmc0 << std::endl;
    std::cout << "number of entries in mc1 histogram: " << nmc1 << std::endl;
    std::cout << "verbose mode: " << std::boolalpha << verbose << std::endl;
    std::cout << std::endl;

    gRandom = new TRandom3(1);

    data = new TH1D("data", "data", nbins, xmin, xmax);
    mc0 = (TH1D*)(data->Clone("mc0"));
    mc1 = (TH1D*)(data->Clone("mc1"));

    data->SetLineColor(kBlack);
    data->SetMarkerStyle(kFullDotLarge);

    mc0->SetTitle("mc0");
    mc0->SetLineColor(kBlue);
    mc0->SetLineWidth(2);
    mc0->SetMarkerSize(0);

    mc1->SetTitle("mc1");
    mc1->SetLineColor(kGreen);
    mc1->SetLineWidth(2);
    mc1->SetMarkerSize(0);

    TObjArray* mc = new TObjArray(2);
    mc->Add(mc0);
    mc->Add(mc1);

    fillHistos(); // histos will be filled again in tfractionfitter()

    fit = new TFractionFitter(data, mc, "Q");

    std::vector<std::pair<Double_t, Double_t> > fits;

    for(UInt_t n=0; n != nfits; ++n) {

        if(n % 100 == 0 || verbose) {

            std::cout << "#fit: " << n << std::endl;

        }

        fits.push_back(tfractionfitter());

    }

    UInt_t inside_error = 0;
    UInt_t nfailed = 0;

    for(UInt_t n=0; n!= fits.size(); ++n) {

        const Double_t fitf = fits[n].first;
        const Double_t fite = fits[n].second;

        if(fite != FIT_FAILED) {

            if( TMath::Abs(f-fitf) < fite) {

                ++inside_error;

            }

            pull->Fill((f-fitf)/fite);

        } else {

            ++nfailed;

        }

    }

    std::cout << nfailed << " of " << nfits << " fits failed" << std::endl;
    std::cout << "inside error: " << ((Double_t)inside_error)/fits.size() << std::endl;

    TFile* outfile = new TFile("outfile.root", "RECREATE");

    data->Write();
    mc0->Write();
    mc1->Write();
    pull->Write();

    outfile->Write();
    outfile->Close();

    TCanvas* c1 = new TCanvas("c1", "c1");
    gStyle->SetOptFit(1);

    c1->Divide(1,2);
    c1->cd(1);

    pull->Draw();
    pull->Fit("gaus");

    c1->cd(2);

    mc0->Scale(1/mc0->Integral());
    mc0->Scale((1-f)*data->Integral());

    mc1->Scale(1/mc1->Integral());
    mc1->Scale(f*data->Integral());

    THStack* stack = new THStack("stack", "stack");
    stack->Add(mc0);
    stack->Add(mc1);

    stack->Draw("histe");
    data->Draw("pesame");

    TLegend* legend = new TLegend(0.2, 0.6, 0.4, 0.8);
    legend->SetFillColor(kWhite);
    legend->SetFillStyle(0);
    legend->SetBorderSize(0);

    legend->AddEntry(data, "data");
    legend->AddEntry(mc0);
    legend->AddEntry(mc1);
    legend->Draw();

    app.Run();

}
