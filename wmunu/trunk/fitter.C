#include "RooRealVar.h"
#include "RooDataHist.h"
#include "RooPlot.h"
#include "RooAddPdf.h"
#include "RooHistPdf.h"
#include "TH1.h"
#include "TF1.h"
#include "THStack.h"
#include "RooFitResult.h"
#include <RooArgSet.h>
#include <RooArgList.h>
#include <RooGlobalFunc.h>
#include <RooDataSet.h>
#include <RooArgSet.h>
#include <RooSimultaneous.h>
#include <RooCategory.h>
#include <RooExtendPdf.h>
#include <RooGenericPdf.h>
#include <RooFitResult.h>
#include <RooSimPdfBuilder.h>
#include <TCanvas.h>

TH1*
get_topmix80()
{
  TH1 *RMETbef = new TH1F("RMETbef","RMETbef",100,0,200);
   RMETbef->SetBinContent(1,120.376);
   RMETbef->SetBinContent(2,296.1687);
   RMETbef->SetBinContent(3,445.6302);
   RMETbef->SetBinContent(4,475.2542);
   RMETbef->SetBinContent(5,468.9129);
   RMETbef->SetBinContent(6,456.4373);
   RMETbef->SetBinContent(7,347.2969);
   RMETbef->SetBinContent(8,293.8455);
   RMETbef->SetBinContent(9,234.6077);
   RMETbef->SetBinContent(10,180.4756);
   RMETbef->SetBinContent(11,137.372);
   RMETbef->SetBinContent(12,92.80691);
   RMETbef->SetBinContent(13,61.04675);
   RMETbef->SetBinContent(14,66.74796);
   RMETbef->SetBinContent(15,40.4878);
   RMETbef->SetBinContent(16,21.5752);
   RMETbef->SetBinContent(17,14.22764);
   RMETbef->SetBinContent(18,16.52846);
   RMETbef->SetBinContent(19,12.93089);
   RMETbef->SetBinContent(20,4.065041);
   RMETbef->SetBinContent(21,6.097561);
   RMETbef->SetBinContent(22,5.315041);
   RMETbef->SetBinContent(24,1.01626);
   RMETbef->SetBinContent(25,2.03252);
   RMETbef->SetBinContent(26,1.01626);
   RMETbef->SetBinContent(27,1.25);
   RMETbef->SetBinContent(28,1.050813);
   RMETbef->SetBinContent(29,1.01626);
   RMETbef->SetBinContent(30,3.083333);
   RMETbef->SetBinContent(31,3.04878);
   RMETbef->SetBinContent(32,0.06910568);
   RMETbef->SetBinContent(33,2.03252);
   RMETbef->SetBinContent(39,0.03455284);
   RMETbef->SetBinContent(43,0.03455284);
   RMETbef->SetBinContent(45,0.03455284);
   RMETbef->SetBinContent(54,0.03455284);
   RMETbef->SetBinContent(56,0.03455284);
   RMETbef->SetBinContent(70,0.06910568);
   RMETbef->SetBinContent(81,0.03455284);
   RMETbef->SetBinContent(89,0.03455284);
   RMETbef->SetBinContent(92,0.03455284);
   RMETbef->SetBinContent(101,0.03455284);
   RMETbef->SetBinError(1,11.25682);
   RMETbef->SetBinError(2,17.69078);
   RMETbef->SetBinError(3,21.6214);
   RMETbef->SetBinError(4,22.33946);
   RMETbef->SetBinError(5,22.27359);
   RMETbef->SetBinError(6,22.0063);
   RMETbef->SetBinError(7,19.08);
   RMETbef->SetBinError(8,17.7551);
   RMETbef->SetBinError(9,15.75825);
   RMETbef->SetBinError(10,13.7969);
   RMETbef->SetBinError(11,12.03457);
   RMETbef->SetBinError(12,9.91487);
   RMETbef->SetBinError(13,8.041701);
   RMETbef->SetBinError(14,8.521582);
   RMETbef->SetBinError(15,6.594205);
   RMETbef->SetBinError(16,4.713618);
   RMETbef->SetBinError(17,3.802497);
   RMETbef->SetBinError(18,4.129826);
   RMETbef->SetBinError(19,3.739486);
   RMETbef->SetBinError(20,2.03252);
   RMETbef->SetBinError(21,2.489319);
   RMETbef->SetBinError(22,2.386135);
   RMETbef->SetBinError(24,1.01626);
   RMETbef->SetBinError(25,1.437209);
   RMETbef->SetBinError(26,1.01626);
   RMETbef->SetBinError(27,1.25);
   RMETbef->SetBinError(28,1.016847);
   RMETbef->SetBinError(29,1.01626);
   RMETbef->SetBinError(30,1.760553);
   RMETbef->SetBinError(31,1.760214);
   RMETbef->SetBinError(32,0.0488651);
   RMETbef->SetBinError(33,1.437209);
   RMETbef->SetBinError(39,0.03455284);
   RMETbef->SetBinError(43,0.03455284);
   RMETbef->SetBinError(45,0.03455284);
   RMETbef->SetBinError(54,0.03455284);
   RMETbef->SetBinError(56,0.03455284);
   RMETbef->SetBinError(70,0.0488651);
   RMETbef->SetBinError(81,0.03455284);
   RMETbef->SetBinError(89,0.03455284);
   RMETbef->SetBinError(92,0.03455284);
   RMETbef->SetBinError(101,0.03455284);
   RMETbef->SetEntries(3658);
   return RMETbef;
}

TH1*
get_topmix79()
{
   TH1 *RMETbef = new TH1F("RMETbef","RMETbef",100,0,200);
   RMETbef->SetBinContent(1,104.1718);
   RMETbef->SetBinContent(2,279.9387);
   RMETbef->SetBinContent(3,392.2699);
   RMETbef->SetBinContent(4,451.411);
   RMETbef->SetBinContent(5,483.4969);
   RMETbef->SetBinContent(6,528.4049);
   RMETbef->SetBinContent(7,590.4294);
   RMETbef->SetBinContent(8,698.1595);
   RMETbef->SetBinContent(9,845.3987);
   RMETbef->SetBinContent(10,1038.221);
   RMETbef->SetBinContent(11,1279.325);
   RMETbef->SetBinContent(12,1569.939);
   RMETbef->SetBinContent(13,1875.46);
   RMETbef->SetBinContent(14,2165.46);
   RMETbef->SetBinContent(15,2454.785);
   RMETbef->SetBinContent(16,2705.46);
   RMETbef->SetBinContent(17,2948.773);
   RMETbef->SetBinContent(18,3144.417);
   RMETbef->SetBinContent(19,3265.215);
   RMETbef->SetBinContent(20,3340.429);
   RMETbef->SetBinContent(21,3167.914);
   RMETbef->SetBinContent(22,2793.374);
   RMETbef->SetBinContent(23,2294.724);
   RMETbef->SetBinContent(24,1769.08);
   RMETbef->SetBinContent(25,1346.871);
   RMETbef->SetBinContent(26,995.2147);
   RMETbef->SetBinContent(27,739.5092);
   RMETbef->SetBinContent(28,576.1349);
   RMETbef->SetBinContent(29,448.2209);
   RMETbef->SetBinContent(30,346.7485);
   RMETbef->SetBinContent(31,274.4172);
   RMETbef->SetBinContent(32,228.5276);
   RMETbef->SetBinContent(33,192.5153);
   RMETbef->SetBinContent(34,155.3988);
   RMETbef->SetBinContent(35,131.9018);
   RMETbef->SetBinContent(36,118.4663);
   RMETbef->SetBinContent(37,96.13497);
   RMETbef->SetBinContent(38,76.38037);
   RMETbef->SetBinContent(39,68.95705);
   RMETbef->SetBinContent(40,63.92638);
   RMETbef->SetBinContent(41,50.98159);
   RMETbef->SetBinContent(42,47.91411);
   RMETbef->SetBinContent(43,41.41104);
   RMETbef->SetBinContent(44,36.74847);
   RMETbef->SetBinContent(45,31.16564);
   RMETbef->SetBinContent(46,29.32515);
   RMETbef->SetBinContent(47,24.96933);
   RMETbef->SetBinContent(48,23.12883);
   RMETbef->SetBinContent(49,20.2454);
   RMETbef->SetBinContent(50,16.68712);
   RMETbef->SetBinContent(51,16.01227);
   RMETbef->SetBinContent(52,13.00613);
   RMETbef->SetBinContent(53,12.94479);
   RMETbef->SetBinContent(54,11.04294);
   RMETbef->SetBinContent(55,11.71779);
   RMETbef->SetBinContent(56,11.04294);
   RMETbef->SetBinContent(57,9.8773);
   RMETbef->SetBinContent(58,8.650307);
   RMETbef->SetBinContent(59,7.055215);
   RMETbef->SetBinContent(60,6.871166);
   RMETbef->SetBinContent(61,7.607362);
   RMETbef->SetBinContent(62,5.521472);
   RMETbef->SetBinContent(63,6.809816);
   RMETbef->SetBinContent(64,4.846626);
   RMETbef->SetBinContent(65,4.662577);
   RMETbef->SetBinContent(66,4.846626);
   RMETbef->SetBinContent(67,4.478528);
   RMETbef->SetBinContent(68,3.742331);
   RMETbef->SetBinContent(69,4.417178);
   RMETbef->SetBinContent(70,2.883435);
   RMETbef->SetBinContent(71,3.312883);
   RMETbef->SetBinContent(72,3.190184);
   RMETbef->SetBinContent(73,2.883435);
   RMETbef->SetBinContent(74,2.576687);
   RMETbef->SetBinContent(75,2.760736);
   RMETbef->SetBinContent(76,2.392638);
   RMETbef->SetBinContent(77,2.453988);
   RMETbef->SetBinContent(78,1.779141);
   RMETbef->SetBinContent(79,1.656442);
   RMETbef->SetBinContent(80,1.840491);
   RMETbef->SetBinContent(81,1.779141);
   RMETbef->SetBinContent(82,1.717791);
   RMETbef->SetBinContent(83,2.331288);
   RMETbef->SetBinContent(84,1.288344);
   RMETbef->SetBinContent(85,1.472393);
   RMETbef->SetBinContent(86,1.349693);
   RMETbef->SetBinContent(87,1.226994);
   RMETbef->SetBinContent(88,1.165644);
   RMETbef->SetBinContent(89,0.8588957);
   RMETbef->SetBinContent(90,0.6748466);
   RMETbef->SetBinContent(91,1.042945);
   RMETbef->SetBinContent(92,0.797546);
   RMETbef->SetBinContent(93,0.613497);
   RMETbef->SetBinContent(94,1.104294);
   RMETbef->SetBinContent(95,0.9815951);
   RMETbef->SetBinContent(96,1.165644);
   RMETbef->SetBinContent(97,0.5521472);
   RMETbef->SetBinContent(98,0.9815951);
   RMETbef->SetBinContent(99,0.3067485);
   RMETbef->SetBinContent(100,0.4294479);
   RMETbef->SetBinContent(101,15.76687);
   RMETbef->SetBinError(1,2.528024);
   RMETbef->SetBinError(2,4.145079);
   RMETbef->SetBinError(3,4.907209);
   RMETbef->SetBinError(4,5.263932);
   RMETbef->SetBinError(5,5.447701);
   RMETbef->SetBinError(6,5.694957);
   RMETbef->SetBinError(7,6.019777);
   RMETbef->SetBinError(8,6.546906);
   RMETbef->SetBinError(9,7.202778);
   RMETbef->SetBinError(10,7.982772);
   RMETbef->SetBinError(11,8.859244);
   RMETbef->SetBinError(12,9.816334);
   RMETbef->SetBinError(13,10.72831);
   RMETbef->SetBinError(14,11.52836);
   RMETbef->SetBinError(15,12.27347);
   RMETbef->SetBinError(16,12.88475);
   RMETbef->SetBinError(17,13.45014);
   RMETbef->SetBinError(18,13.8908);
   RMETbef->SetBinError(19,14.15477);
   RMETbef->SetBinError(20,14.31684);
   RMETbef->SetBinError(21,13.94178);
   RMETbef->SetBinError(22,13.09295);
   RMETbef->SetBinError(23,11.86605);
   RMETbef->SetBinError(24,10.42078);
   RMETbef->SetBinError(25,9.092182);
   RMETbef->SetBinError(26,7.817212);
   RMETbef->SetBinError(27,6.739537);
   RMETbef->SetBinError(28,5.94839);
   RMETbef->SetBinError(29,5.247461);
   RMETbef->SetBinError(30,4.614704);
   RMETbef->SetBinError(31,4.109514);
   RMETbef->SetBinError(32,3.749365);
   RMETbef->SetBinError(33,3.438868);
   RMETbef->SetBinError(34,3.091318);
   RMETbef->SetBinError(35,2.848635);
   RMETbef->SetBinError(36,2.697294);
   RMETbef->SetBinError(37,2.433194);
   RMETbef->SetBinError(38,2.169907);
   RMETbef->SetBinError(39,2.058646);
   RMETbef->SetBinError(40,1.987957);
   RMETbef->SetBinError(41,1.768532);
   RMETbef->SetBinError(42,1.716696);
   RMETbef->SetBinError(43,1.60098);
   RMETbef->SetBinError(44,1.501502);
   RMETbef->SetBinError(45,1.388185);
   RMETbef->SetBinError(46,1.341301);
   RMETbef->SetBinError(47,1.240721);
   RMETbef->SetBinError(48,1.191196);
   RMETbef->SetBinError(49,1.117845);
   RMETbef->SetBinError(50,1.011805);
   RMETbef->SetBinError(51,0.9911346);
   RMETbef->SetBinError(52,0.893265);
   RMETbef->SetBinError(53,0.8995631);
   RMETbef->SetBinError(54,0.8276526);
   RMETbef->SetBinError(55,0.8478696);
   RMETbef->SetBinError(56,0.8230925);
   RMETbef->SetBinError(57,0.7784403);
   RMETbef->SetBinError(58,0.7336356);
   RMETbef->SetBinError(59,0.6579022);
   RMETbef->SetBinError(60,0.6492641);
   RMETbef->SetBinError(61,0.6831613);
   RMETbef->SetBinError(62,0.5820143);
   RMETbef->SetBinError(63,0.6463591);
   RMETbef->SetBinError(64,0.545288);
   RMETbef->SetBinError(65,0.5348342);
   RMETbef->SetBinError(66,0.5521472);
   RMETbef->SetBinError(67,0.524172);
   RMETbef->SetBinError(68,0.4791564);
   RMETbef->SetBinError(69,0.5205694);
   RMETbef->SetBinError(70,0.4205923);
   RMETbef->SetBinError(71,0.4508263);
   RMETbef->SetBinError(72,0.4423989);
   RMETbef->SetBinError(73,0.4205923);
   RMETbef->SetBinError(74,0.3975915);
   RMETbef->SetBinError(75,0.4115463);
   RMETbef->SetBinError(76,0.3831287);
   RMETbef->SetBinError(77,0.3880095);
   RMETbef->SetBinError(78,0.3303782);
   RMETbef->SetBinError(79,0.3187824);
   RMETbef->SetBinError(80,0.3360261);
   RMETbef->SetBinError(81,0.3303782);
   RMETbef->SetBinError(82,0.3246321);
   RMETbef->SetBinError(83,0.3781849);
   RMETbef->SetBinError(84,0.2811396);
   RMETbef->SetBinError(85,0.3005509);
   RMETbef->SetBinError(86,0.2877556);
   RMETbef->SetBinError(87,0.2743642);
   RMETbef->SetBinError(88,0.2674171);
   RMETbef->SetBinError(89,0.2295495);
   RMETbef->SetBinError(90,0.2034739);
   RMETbef->SetBinError(91,0.2529513);
   RMETbef->SetBinError(92,0.2211995);
   RMETbef->SetBinError(93,0.1940048);
   RMETbef->SetBinError(94,0.2602847);
   RMETbef->SetBinError(95,0.2453988);
   RMETbef->SetBinError(96,0.2674171);
   RMETbef->SetBinError(97,0.1840491);
   RMETbef->SetBinError(98,0.2453988);
   RMETbef->SetBinError(99,0.1371821);
   RMETbef->SetBinError(100,0.162316);
   RMETbef->SetBinError(101,0.9835104);
   RMETbef->SetEntries(759721);
   return RMETbef;
}

TH1*
get_data()
{
  TH1 *RMETbef = new TH1F("RMETbef","RMETbef",100,0,200);
   RMETbef->SetBinContent(1,5);
   RMETbef->SetBinContent(2,10);
   RMETbef->SetBinContent(3,16);
   RMETbef->SetBinContent(4,26);
   RMETbef->SetBinContent(5,15);
   RMETbef->SetBinContent(6,14);
   RMETbef->SetBinContent(7,10);
   RMETbef->SetBinContent(8,7);
   RMETbef->SetBinContent(9,6);
   RMETbef->SetBinContent(10,19);
   RMETbef->SetBinContent(11,14);
   RMETbef->SetBinContent(12,10);
   RMETbef->SetBinContent(13,7);
   RMETbef->SetBinContent(14,13);
   RMETbef->SetBinContent(15,16);
   RMETbef->SetBinContent(16,8);
   RMETbef->SetBinContent(17,11);
   RMETbef->SetBinContent(18,20);
   RMETbef->SetBinContent(19,16);
   RMETbef->SetBinContent(20,12);
   RMETbef->SetBinContent(21,17);
   RMETbef->SetBinContent(22,21);
   RMETbef->SetBinContent(23,11);
   RMETbef->SetBinContent(24,9);
   RMETbef->SetBinContent(25,6);
   RMETbef->SetBinContent(26,3);
   RMETbef->SetBinContent(27,5);
   RMETbef->SetBinContent(28,4);
   RMETbef->SetBinContent(29,3);
   RMETbef->SetBinContent(31,3);
   RMETbef->SetBinContent(32,1);
   RMETbef->SetBinContent(33,2);
   RMETbef->SetBinContent(35,2);
   RMETbef->SetBinContent(36,2);
   RMETbef->SetBinContent(39,1);
   RMETbef->SetBinContent(40,1);
   RMETbef->SetBinContent(64,1);
   RMETbef->SetBinContent(101,1);
   RMETbef->SetBinError(1,2.236068);
   RMETbef->SetBinError(2,3.162278);
   RMETbef->SetBinError(3,4);
   RMETbef->SetBinError(4,5.09902);
   RMETbef->SetBinError(5,3.872983);
   RMETbef->SetBinError(6,3.741657);
   RMETbef->SetBinError(7,3.162278);
   RMETbef->SetBinError(8,2.645751);
   RMETbef->SetBinError(9,2.44949);
   RMETbef->SetBinError(10,4.358899);
   RMETbef->SetBinError(11,3.741657);
   RMETbef->SetBinError(12,3.162278);
   RMETbef->SetBinError(13,2.645751);
   RMETbef->SetBinError(14,3.605551);
   RMETbef->SetBinError(15,4);
   RMETbef->SetBinError(16,2.828427);
   RMETbef->SetBinError(17,3.316625);
   RMETbef->SetBinError(18,4.472136);
   RMETbef->SetBinError(19,4);
   RMETbef->SetBinError(20,3.464102);
   RMETbef->SetBinError(21,4.123106);
   RMETbef->SetBinError(22,4.582576);
   RMETbef->SetBinError(23,3.316625);
   RMETbef->SetBinError(24,3);
   RMETbef->SetBinError(25,2.44949);
   RMETbef->SetBinError(26,1.732051);
   RMETbef->SetBinError(27,2.236068);
   RMETbef->SetBinError(28,2);
   RMETbef->SetBinError(29,1.732051);
   RMETbef->SetBinError(31,1.732051);
   RMETbef->SetBinError(32,1);
   RMETbef->SetBinError(33,1.414214);
   RMETbef->SetBinError(35,1.414214);
   RMETbef->SetBinError(36,1.414214);
   RMETbef->SetBinError(39,1);
   RMETbef->SetBinError(40,1);
   RMETbef->SetBinError(64,1);
   RMETbef->SetBinError(101,1);
   RMETbef->SetEntries(348);
   return RMETbef;
}

void
fitter()
{
  // define toy histograms. override later with the real data, if desired.
  TH1* hqcd = new TH1F( "hqcd" , "" , 100 , 0 , 200 );
  TH1* hw = new TH1F( "hw" , "" , 100 , 0 , 200 );
  TH1* hdata = new TH1F("hdata","", 100 , 0 , 200 );

  TF1* metqcd = new TF1( "metqcd" , "gaus" );
  metqcd->SetParameter( 0 , 1 );
  metqcd->SetParameter( 1 , 0 );
  metqcd->SetParameter( 2 , 10. );
  TF1* metw = new TF1( "metw" , "gaus" );
  metw->SetParameter( 0 , 1 );
  metw->SetParameter( 1 , 0 );
  metw->SetParameter( 2 , 50. );
  hqcd->FillRandom( "metqcd" , 100000 );
  hw->FillRandom( "metw" , 50000 );
  for( int i=1; i!=101; ++i ) {
    hdata->SetBinContent(i,hqcd->GetBinContent(i)+hw->GetBinContent(i) );
  }
  hqcd->FillRandom( "metqcd" , 5*100000 );
  hw->FillRandom( "metw" , 5*50000 );
  hqcd->Scale(1./6.);
  hw->Scale(1./6.);

  // override toy histograms with real histograms.
  hqcd = get_topmix80();
  hw = get_topmix79();
  hdata = get_data();

  hqcd->SetFillColor(kRed);
  hw->SetFillColor(kBlue);
  THStack* stack = new THStack( "stack" , "" );
  stack->Add( hqcd );
  stack->Add( hw );
  TCanvas* c2 = new TCanvas( "c2" , "" );
  // hqcd->Draw();
  // hw->Draw("same");
  stack->Draw();
  hdata->Draw("e same");

  RooRealVar* met = new RooRealVar( "met" , "met" , 0 , 100 , "GeV" );
  RooDataHist* dsdata = new RooDataHist( "data" , "data" , *met , hdata );
  RooDataHist* dsqcd = new RooDataHist( "qcd" , "qcd" , *met , hqcd );
  RooDataHist* dsw = new RooDataHist( "w" , "w" , *met , hw );

  RooHistPdf* pdfqcd = new RooHistPdf( "pdfqcd" , "pdfqcd" , *met , *dsqcd );
  RooHistPdf* pdfw = new RooHistPdf( "pdfw" , "pdfw" , *met , *dsw );
  RooRealVar* nqcd = new RooRealVar( "nqcd" , "nqcd" , 1 , 0 , 1E6 );
  RooRealVar* nw = new RooRealVar( "nw" , "nw" , 0.1 , 0 , 1E6 );
  RooAddPdf* pdf = new RooAddPdf( "pdf" , "pdf" , RooArgList(*pdfqcd,*pdfw) , RooArgList(*nqcd,*nw) );

  RooFitResult* sfit = pdf->fitTo( *dsdata , RooFit::Save(true) , RooFit::Extended(true) );

  TCanvas* c1 = new TCanvas( "c1" , "" );
  RooPlot* frame = met->frame();
  pdfw->plotOn( frame , RooFit::Normalization(nw->getVal(),RooAbsReal::NumEvent) , RooFit::LineColor(kBlue) );
  pdfqcd->plotOn( frame , RooFit::Normalization(nqcd->getVal(),RooAbsReal::NumEvent) , RooFit::LineColor(kGreen) );
  pdf->plotOn( frame , RooFit::Normalization(nw->getVal()+nqcd->getVal(),RooAbsReal::NumEvent) , RooFit::LineColor(kRed) );
  dsdata->plotOn( frame );
  frame->Draw();

}
