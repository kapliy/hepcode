#include <TH2.h>
#include <TFile.h>

void test_max_2d() {
  TFile *f = new TFile("OUT_PLOTS_08242012.v1.root");
  f->ls();
  TH2 *data = f->Get("POS/data"); assert(data);
  TH2 *bg   = f->Get("POS/totalbg_nominal"); assert(bg);
  TH2 *sig =  f->Get("POS/wmunu_PowhegPythia_nominal"); assert(sig);
  
  TH2 *bgsub = data->Clone();
  bgsub->Add(bg,-1.0);
  
  TH1* bgsub1d = bgsub->ProjectionX();
  bgsub1d->SetLineColor(kBlack);
  TH1* sig1d = sig->ProjectionX();
  sig1d->SetLineColor(kRed);

  TCanvas *c = new TCanvas("c","c",800,600);
  c->cd();
  
  bgsub1d->Draw();
  sig1d->Draw("A SAME");
  bgsub1d->GetYaxis()->SetRangeUser(0.0,bgsub1d->GetMaximum()*1.5);

  c->SaveAs("plot.png");
 
  return;
}
