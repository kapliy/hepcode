//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jan 21 15:24:12 2010 by ROOT version 5.22/00d
// from TTree vtuple/JetTagNtuple
// found on file: user10.AntonioBoveia.v14WHbb.set02_RECO0E34.rset1.20100121.EXT1._00001.btag.pool.root
//////////////////////////////////////////////////////////

#ifndef TagTree_h
#define TagTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class TagTree {
public:
  // Constants from JetTagNtDimArrays.h in the
  // PhysicsAnalysis/JetTagging/JetTagValidation/JetTagNtuple
  // package.
  enum { s_nmax_JTNt_JetColl = 5 }; // number of jet collections
  enum { s_nmax_JTNt_Jets = 50 };
  enum { s_nmax_JTNt_PV = 30 };    // number of primary vertices
  enum { s_nmax_JTNt_Tracks = 1500 };
  enum { s_nmax_JTNt_TracksInJets = 100 };
  enum { s_nmax_JTNt_PixClusters = 5000 };
  enum { s_nmax_JTNt_HitsOnTrack = 22500 }; // s_nmax_JTNt_Tracks*15
  enum { s_nmax_JTNt_Truth = 2000 };
  enum { s_nmax_JTNt_TruthLinks = 200 };
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          run;
   UInt_t          evt;
   UInt_t          lumiBlock;
   UInt_t          timeStamp;
   UInt_t          lhcBCID;
   Float_t         MBTStimeDiff;
   Float_t         LArECtimeDiff;
   UInt_t          syncevt;
   UInt_t          uidmc;
   UInt_t          uidtp;
   UInt_t          nbpvtrue;
   UInt_t          nbpv;
   UInt_t          ntrueparticles;
   UInt_t          ntruetracks;
   UInt_t          ntracks;
   UInt_t          nhits;
   UInt_t          npixclus;
   Float_t         pvxtrue[s_nmax_JTNt_PV];   //[nbpvtrue]
   Float_t         pvytrue[s_nmax_JTNt_PV];   //[nbpvtrue]
   Float_t         pvztrue[s_nmax_JTNt_PV];   //[nbpvtrue]
   UInt_t          pvtype[s_nmax_JTNt_PV];   //[nbpv]
   UInt_t          pvnbtk[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         pvsumpt[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         pvx[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         pvy[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         pvz[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         errpvx[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         errpvy[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         errpvz[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         covpvxpvy[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         covpvypvz[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         covpvzpvx[s_nmax_JTNt_PV];   //[nbpv]
   Float_t         trueparticlepx[1];   //[ntrueparticles]
   Float_t         trueparticlepy[1];   //[ntrueparticles]
   Float_t         trueparticlepz[1];   //[ntrueparticles]
   Float_t         trueparticlemass[1];   //[ntrueparticles]
   Int_t           trueparticlepdg[1];   //[ntrueparticles]
   Int_t           trueparticlebarcode[1];   //[ntrueparticles]
   Int_t           trueparticlestatus[1];   //[ntrueparticles]
   UInt_t          trueparticlendaughters[1];   //[ntrueparticles]
   Int_t           trueparticledaughteridx[1][s_nmax_JTNt_TruthLinks];   //[ntrueparticles]
   UInt_t          trueparticlenmothers[1];   //[ntrueparticles]
   Int_t           trueparticlemotheridx[1][s_nmax_JTNt_TruthLinks];   //[ntrueparticles]
   Int_t           tperigeeok[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         td0[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         tz0[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         tphi[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         ttheta[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         tqop[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         tpt[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         teta[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Int_t           pdg[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Int_t           barcode[s_nmax_JTNt_Tracks];   //[ntruetracks]
   ULong64_t       mybarcode[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Int_t           pileupevtidx[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Int_t           begvtxok[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         xbegvtx[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         ybegvtx[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         zbegvtx[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Int_t           endvtxok[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         xendvtx[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         yendvtx[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Float_t         zendvtx[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Int_t           reco_index[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Int_t           matched2r[s_nmax_JTNt_Tracks];   //[ntruetracks]
   Int_t           pvidx[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         d0[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         z0[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         phi[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         theta[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         qop[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         pt[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         eta[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errd0[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errz0[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errphi[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errtheta[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errqop[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covd0z0[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covd0phi[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covd0theta[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covd0qop[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covz0phi[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covz0theta[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covz0qop[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covphitheta[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covphiqop[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         covthetaqop[s_nmax_JTNt_Tracks];   //[ntracks]
   Long64_t        trackPattInfo[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           trackPropInfo[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           trackFitInfo[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         chi2[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           ndof[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           hitpattern[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           nbla[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           nblaout[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           npix[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           nsct[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           ntrt[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           npixdead[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           xpectbla[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           nblash[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           npixsh[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           nsctsh[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           nambig[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           nganged[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           npixho[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           nsctho[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         blaresx[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         blaresy[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         blapulx[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         blapuly[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           true_index[s_nmax_JTNt_Tracks];   //[ntracks]
   ULong64_t       true_barcode[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           true_pileupevtidx[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         true_proba[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           matched2t[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         d0wrtPVunb[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         z0wrtPVunb[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errd0wrtPVunb[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errz0wrtPVunb[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         d0wrtPVbias[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         z0wrtPVbias[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errd0wrtPVbias[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errz0wrtPVbias[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         d0wrtBSunb[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         z0wrtBSunb[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errd0wrtBSunb[s_nmax_JTNt_Tracks];   //[ntracks]
   Float_t         errz0wrtBSunb[s_nmax_JTNt_Tracks];   //[ntracks]
   Int_t           hdtyp[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Float_t         hxg[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Float_t         hyg[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Float_t         hzg[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Float_t         heta[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Float_t         hphi[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Float_t         hxres[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Float_t         hyres[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Int_t           hreco_index[s_nmax_JTNt_HitsOnTrack];   //[nhits]
   Int_t           pcdtyp[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcxglob[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcyglob[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pczglob[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pceta[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcphi[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcomegax[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcomegay[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcxloc[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcyloc[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcxlocerr[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pcylocerr[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcgroupsize[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pclayer[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcetamodule[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcphimodule[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcrow[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pccol[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcecbarrel[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcdeltarow[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcdeltacol[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pctot[s_nmax_JTNt_PixClusters];   //[npixclus]
   Float_t         pccharge[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pclvl1[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcganged[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcfake[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcambig[s_nmax_JTNt_PixClusters];   //[npixclus]
   Int_t           pcrecoindex[s_nmax_JTNt_PixClusters];   //[npixclus]
   UInt_t          njetsC4H1TJ;
   Float_t         phiC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         etaC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         ptC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         eC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           ntrakC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           trakassoc_trkindexC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Int_t           nmuonC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           muonassoc_trkindexC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Int_t           nelecC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           elecassoc_trkindexC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Int_t           nphotC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           photassoc_trkindexC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         trakassoc_trkd0valC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         trakassoc_trkd0sigC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         trakassoc_trkz0valC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         trakassoc_trkz0sigC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Int_t           labelC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         drminbC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         drmincC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         drmintC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           truebpdgC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         truebvtxC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         truebvtyC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         truebvtzC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           ntagsC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         ip2d_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         ip3d_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv1_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv2_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         cmb_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jetp_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jetpneg_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         trkc_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         lf2d_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         svbu_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         lhsig_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         softm_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         softe_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jfit_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jfitNN_wC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           ipplus_ntrkC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           svplus_ntrkC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv0plus_ntrkC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         ip2d_pbC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         ip2d_puC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           ip2d_ntrkC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         ip2d_trkwC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         ip3d_pbC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         ip3d_puC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           ip3d_ntrkC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         ip3d_trkwC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         jetp_trkwC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         jetpneg_trkwC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Int_t           ip_trkindexC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Int_t           ip_trkgradeC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Int_t           ip_trkv0intC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         ip_trkd0valC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         ip_trkd0sigC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         ip_trkz0valC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         ip_trkz0sigC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         ip_trkptC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         ip_trkphiC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         ip_trketaC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         sv1_pbC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv1_puC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv2_pbC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv2_puC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv_okC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv_ntrkvC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv_ntrkjC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_massC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_efrcC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv_nv2tC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_xC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_yC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_zC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_err_xC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_err_yC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_err_zC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_cov_xyC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_cov_xzC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_cov_yzC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv_chi2C4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv_ndofC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv_trkindexC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         sv_trkptC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         sv_trkphiC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         sv_trketaC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Int_t           sv0_okC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv0_ntrkvC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv0_ntrkjC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_massC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_efrcC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv0_nv2tC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_xC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_yC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_zC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_err_xC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_err_yC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_err_zC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_cov_xyC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_cov_xzC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_cov_yzC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         sv0_chi2C4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv0_ndofC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           sv0_trkindexC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         sv0_trkptC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         sv0_trkphiC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         sv0_trketaC4H1TJ[s_nmax_JTNt_Jets][s_nmax_JTNt_TracksInJets];   //[njetsC4H1TJ]
   Float_t         jfit_pbC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jfit_puC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           jfit_nvtxntC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           jfit_nvtx1tC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Int_t           jfit_ntrkC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jfit_efrcC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jfit_massC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jfit_sig3dC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jfit_dphiC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]
   Float_t         jfit_detaC4H1TJ[s_nmax_JTNt_Jets];   //[njetsC4H1TJ]

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_lumiBlock;   //!
   TBranch        *b_timeStamp;   //!
   TBranch        *b_lhcBCID;   //!
   TBranch        *b_MBTStimeDiff;   //!
   TBranch        *b_LArECtimeDiff;   //!
   TBranch        *b_syncevt;   //!
   TBranch        *b_uidmc;   //!
   TBranch        *b_uidtp;   //!
   TBranch        *b_nbpvtrue;   //!
   TBranch        *b_nbpv;   //!
   TBranch        *b_ntrueparticles;   //!
   TBranch        *b_ntruetracks;   //!
   TBranch        *b_ntracks;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_npixclus;   //!
   TBranch        *b_pvxtrue;   //!
   TBranch        *b_pvytrue;   //!
   TBranch        *b_pvztrue;   //!
   TBranch        *b_pvtype;   //!
   TBranch        *b_pvnbtk;   //!
   TBranch        *b_pvsumpt;   //!
   TBranch        *b_pvx;   //!
   TBranch        *b_pvy;   //!
   TBranch        *b_pvz;   //!
   TBranch        *b_errpvx;   //!
   TBranch        *b_errpvy;   //!
   TBranch        *b_errpvz;   //!
   TBranch        *b_covpvxpvy;   //!
   TBranch        *b_covpvypvz;   //!
   TBranch        *b_covpvzpvx;   //!
   TBranch        *b_trueparticlepx;   //!
   TBranch        *b_trueparticlepy;   //!
   TBranch        *b_trueparticlepz;   //!
   TBranch        *b_trueparticlemass;   //!
   TBranch        *b_trueparticlepdg;   //!
   TBranch        *b_trueparticlebarcode;   //!
   TBranch        *b_trueparticlestatus;   //!
   TBranch        *b_trueparticlendaughters;   //!
   TBranch        *b_trueparticledaughteridx;   //!
   TBranch        *b_trueparticlenmothers;   //!
   TBranch        *b_trueparticlemotheridx;   //!
   TBranch        *b_tperigeeok;   //!
   TBranch        *b_td0;   //!
   TBranch        *b_tz0;   //!
   TBranch        *b_tphi;   //!
   TBranch        *b_ttheta;   //!
   TBranch        *b_tqop;   //!
   TBranch        *b_tpt;   //!
   TBranch        *b_teta;   //!
   TBranch        *b_pdg;   //!
   TBranch        *b_barcode;   //!
   TBranch        *b_mybarcode;   //!
   TBranch        *b_pileupevtidx;   //!
   TBranch        *b_begvtxok;   //!
   TBranch        *b_xbegvtx;   //!
   TBranch        *b_ybegvtx;   //!
   TBranch        *b_zbegvtx;   //!
   TBranch        *b_endvtxok;   //!
   TBranch        *b_xendvtx;   //!
   TBranch        *b_yendvtx;   //!
   TBranch        *b_zendvtx;   //!
   TBranch        *b_reco_index;   //!
   TBranch        *b_matched2r;   //!
   TBranch        *b_pvidx;   //!
   TBranch        *b_d0;   //!
   TBranch        *b_z0;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_theta;   //!
   TBranch        *b_qop;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_eta;   //!
   TBranch        *b_errd0;   //!
   TBranch        *b_errz0;   //!
   TBranch        *b_errphi;   //!
   TBranch        *b_errtheta;   //!
   TBranch        *b_errqop;   //!
   TBranch        *b_covd0z0;   //!
   TBranch        *b_covd0phi;   //!
   TBranch        *b_covd0theta;   //!
   TBranch        *b_covd0qop;   //!
   TBranch        *b_covz0phi;   //!
   TBranch        *b_covz0theta;   //!
   TBranch        *b_covz0qop;   //!
   TBranch        *b_covphitheta;   //!
   TBranch        *b_covphiqop;   //!
   TBranch        *b_covthetaqop;   //!
   TBranch        *b_trackPattInfo;   //!
   TBranch        *b_trackPropInfo;   //!
   TBranch        *b_trackFitInfo;   //!
   TBranch        *b_chi2;   //!
   TBranch        *b_ndof;   //!
   TBranch        *b_hitpattern;   //!
   TBranch        *b_nbla;   //!
   TBranch        *b_nblaout;   //!
   TBranch        *b_npix;   //!
   TBranch        *b_nsct;   //!
   TBranch        *b_ntrt;   //!
   TBranch        *b_npixdead;   //!
   TBranch        *b_xpectbla;   //!
   TBranch        *b_nblash;   //!
   TBranch        *b_npixsh;   //!
   TBranch        *b_nsctsh;   //!
   TBranch        *b_nambig;   //!
   TBranch        *b_nganged;   //!
   TBranch        *b_npixho;   //!
   TBranch        *b_nsctho;   //!
   TBranch        *b_blaresx;   //!
   TBranch        *b_blaresy;   //!
   TBranch        *b_blapulx;   //!
   TBranch        *b_blapuly;   //!
   TBranch        *b_true_index;   //!
   TBranch        *b_true_barcode;   //!
   TBranch        *b_true_pileupevtidx;   //!
   TBranch        *b_true_proba;   //!
   TBranch        *b_matched2t;   //!
   TBranch        *b_d0wrtPVunb;   //!
   TBranch        *b_z0wrtPVunb;   //!
   TBranch        *b_errd0wrtPVunb;   //!
   TBranch        *b_errz0wrtPVunb;   //!
   TBranch        *b_d0wrtPVbias;   //!
   TBranch        *b_z0wrtPVbias;   //!
   TBranch        *b_errd0wrtPVbias;   //!
   TBranch        *b_errz0wrtPVbias;   //!
   TBranch        *b_d0wrtBSunb;   //!
   TBranch        *b_z0wrtBSunb;   //!
   TBranch        *b_errd0wrtBSunb;   //!
   TBranch        *b_errz0wrtBSunb;   //!
   TBranch        *b_hdtyp;   //!
   TBranch        *b_hxg;   //!
   TBranch        *b_hyg;   //!
   TBranch        *b_hzg;   //!
   TBranch        *b_heta;   //!
   TBranch        *b_hphi;   //!
   TBranch        *b_hxres;   //!
   TBranch        *b_hyres;   //!
   TBranch        *b_hreco_index;   //!
   TBranch        *b_pcdtyp;   //!
   TBranch        *b_pcxglob;   //!
   TBranch        *b_pcyglob;   //!
   TBranch        *b_pczglob;   //!
   TBranch        *b_pceta;   //!
   TBranch        *b_pcphi;   //!
   TBranch        *b_pcomegax;   //!
   TBranch        *b_pcomegay;   //!
   TBranch        *b_pcxloc;   //!
   TBranch        *b_pcyloc;   //!
   TBranch        *b_pcxlocerr;   //!
   TBranch        *b_pcylocerr;   //!
   TBranch        *b_pcgroupsize;   //!
   TBranch        *b_pclayer;   //!
   TBranch        *b_pcetamodule;   //!
   TBranch        *b_pcphimodule;   //!
   TBranch        *b_pcrow;   //!
   TBranch        *b_pccol;   //!
   TBranch        *b_pcecbarrel;   //!
   TBranch        *b_pcdeltarow;   //!
   TBranch        *b_pcdeltacol;   //!
   TBranch        *b_pctot;   //!
   TBranch        *b_pccharge;   //!
   TBranch        *b_pclvl1;   //!
   TBranch        *b_pcganged;   //!
   TBranch        *b_pcfake;   //!
   TBranch        *b_pcambig;   //!
   TBranch        *b_pcrecoindex;   //!
   TBranch        *b_njetsC4H1TJ;   //!
   TBranch        *b_phiC4H1TJ;   //!
   TBranch        *b_etaC4H1TJ;   //!
   TBranch        *b_ptC4H1TJ;   //!
   TBranch        *b_eC4H1TJ;   //!
   TBranch        *b_ntrakC4H1TJ;   //!
   TBranch        *b_trakassoc_trkindexC4H1TJ;   //!
   TBranch        *b_nmuonC4H1TJ;   //!
   TBranch        *b_muonassoc_trkindexC4H1TJ;   //!
   TBranch        *b_nelecC4H1TJ;   //!
   TBranch        *b_elecassoc_trkindexC4H1TJ;   //!
   TBranch        *b_nphotC4H1TJ;   //!
   TBranch        *b_photassoc_trkindexC4H1TJ;   //!
   TBranch        *b_trakassoc_trkd0valC4H1TJ;   //!
   TBranch        *b_trakassoc_trkd0sigC4H1TJ;   //!
   TBranch        *b_trakassoc_trkz0valC4H1TJ;   //!
   TBranch        *b_trakassoc_trkz0sigC4H1TJ;   //!
   TBranch        *b_labelC4H1TJ;   //!
   TBranch        *b_drminbC4H1TJ;   //!
   TBranch        *b_drmincC4H1TJ;   //!
   TBranch        *b_drmintC4H1TJ;   //!
   TBranch        *b_truebpdgC4H1TJ;   //!
   TBranch        *b_truebvtxC4H1TJ;   //!
   TBranch        *b_truebvtyC4H1TJ;   //!
   TBranch        *b_truebvtzC4H1TJ;   //!
   TBranch        *b_ntagsC4H1TJ;   //!
   TBranch        *b_ip2d_wC4H1TJ;   //!
   TBranch        *b_ip3d_wC4H1TJ;   //!
   TBranch        *b_sv0_wC4H1TJ;   //!
   TBranch        *b_sv1_wC4H1TJ;   //!
   TBranch        *b_sv2_wC4H1TJ;   //!
   TBranch        *b_cmb_wC4H1TJ;   //!
   TBranch        *b_jetp_wC4H1TJ;   //!
   TBranch        *b_jetpneg_wC4H1TJ;   //!
   TBranch        *b_trkc_wC4H1TJ;   //!
   TBranch        *b_lf2d_wC4H1TJ;   //!
   TBranch        *b_svbu_wC4H1TJ;   //!
   TBranch        *b_lhsig_wC4H1TJ;   //!
   TBranch        *b_softm_wC4H1TJ;   //!
   TBranch        *b_softe_wC4H1TJ;   //!
   TBranch        *b_jfit_wC4H1TJ;   //!
   TBranch        *b_jfitNN_wC4H1TJ;   //!
   TBranch        *b_ipplus_ntrkC4H1TJ;   //!
   TBranch        *b_svplus_ntrkC4H1TJ;   //!
   TBranch        *b_sv0plus_ntrkC4H1TJ;   //!
   TBranch        *b_ip2d_pbC4H1TJ;   //!
   TBranch        *b_ip2d_puC4H1TJ;   //!
   TBranch        *b_ip2d_ntrkC4H1TJ;   //!
   TBranch        *b_ip2d_trkwC4H1TJ;   //!
   TBranch        *b_ip3d_pbC4H1TJ;   //!
   TBranch        *b_ip3d_puC4H1TJ;   //!
   TBranch        *b_ip3d_ntrkC4H1TJ;   //!
   TBranch        *b_ip3d_trkwC4H1TJ;   //!
   TBranch        *b_jetp_trkwC4H1TJ;   //!
   TBranch        *b_jetpneg_trkwC4H1TJ;   //!
   TBranch        *b_ip_trkindexC4H1TJ;   //!
   TBranch        *b_ip_trkgradeC4H1TJ;   //!
   TBranch        *b_ip_trkv0intC4H1TJ;   //!
   TBranch        *b_ip_trkd0valC4H1TJ;   //!
   TBranch        *b_ip_trkd0sigC4H1TJ;   //!
   TBranch        *b_ip_trkz0valC4H1TJ;   //!
   TBranch        *b_ip_trkz0sigC4H1TJ;   //!
   TBranch        *b_ip_trkptC4H1TJ;   //!
   TBranch        *b_ip_trkphiC4H1TJ;   //!
   TBranch        *b_ip_trketaC4H1TJ;   //!
   TBranch        *b_sv1_pbC4H1TJ;   //!
   TBranch        *b_sv1_puC4H1TJ;   //!
   TBranch        *b_sv2_pbC4H1TJ;   //!
   TBranch        *b_sv2_puC4H1TJ;   //!
   TBranch        *b_sv_okC4H1TJ;   //!
   TBranch        *b_sv_ntrkvC4H1TJ;   //!
   TBranch        *b_sv_ntrkjC4H1TJ;   //!
   TBranch        *b_sv_massC4H1TJ;   //!
   TBranch        *b_sv_efrcC4H1TJ;   //!
   TBranch        *b_sv_nv2tC4H1TJ;   //!
   TBranch        *b_sv_xC4H1TJ;   //!
   TBranch        *b_sv_yC4H1TJ;   //!
   TBranch        *b_sv_zC4H1TJ;   //!
   TBranch        *b_sv_err_xC4H1TJ;   //!
   TBranch        *b_sv_err_yC4H1TJ;   //!
   TBranch        *b_sv_err_zC4H1TJ;   //!
   TBranch        *b_sv_cov_xyC4H1TJ;   //!
   TBranch        *b_sv_cov_xzC4H1TJ;   //!
   TBranch        *b_sv_cov_yzC4H1TJ;   //!
   TBranch        *b_sv_chi2C4H1TJ;   //!
   TBranch        *b_sv_ndofC4H1TJ;   //!
   TBranch        *b_sv_trkindexC4H1TJ;   //!
   TBranch        *b_sv_trkptC4H1TJ;   //!
   TBranch        *b_sv_trkphiC4H1TJ;   //!
   TBranch        *b_sv_trketaC4H1TJ;   //!
   TBranch        *b_sv0_okC4H1TJ;   //!
   TBranch        *b_sv0_ntrkvC4H1TJ;   //!
   TBranch        *b_sv0_ntrkjC4H1TJ;   //!
   TBranch        *b_sv0_massC4H1TJ;   //!
   TBranch        *b_sv0_efrcC4H1TJ;   //!
   TBranch        *b_sv0_nv2tC4H1TJ;   //!
   TBranch        *b_sv0_xC4H1TJ;   //!
   TBranch        *b_sv0_yC4H1TJ;   //!
   TBranch        *b_sv0_zC4H1TJ;   //!
   TBranch        *b_sv0_err_xC4H1TJ;   //!
   TBranch        *b_sv0_err_yC4H1TJ;   //!
   TBranch        *b_sv0_err_zC4H1TJ;   //!
   TBranch        *b_sv0_cov_xyC4H1TJ;   //!
   TBranch        *b_sv0_cov_xzC4H1TJ;   //!
   TBranch        *b_sv0_cov_yzC4H1TJ;   //!
   TBranch        *b_sv0_chi2C4H1TJ;   //!
   TBranch        *b_sv0_ndofC4H1TJ;   //!
   TBranch        *b_sv0_trkindexC4H1TJ;   //!
   TBranch        *b_sv0_trkptC4H1TJ;   //!
   TBranch        *b_sv0_trkphiC4H1TJ;   //!
   TBranch        *b_sv0_trketaC4H1TJ;   //!
   TBranch        *b_jfit_pbC4H1TJ;   //!
   TBranch        *b_jfit_puC4H1TJ;   //!
   TBranch        *b_jfit_nvtxntC4H1TJ;   //!
   TBranch        *b_jfit_nvtx1tC4H1TJ;   //!
   TBranch        *b_jfit_ntrkC4H1TJ;   //!
   TBranch        *b_jfit_efrcC4H1TJ;   //!
   TBranch        *b_jfit_massC4H1TJ;   //!
   TBranch        *b_jfit_sig3dC4H1TJ;   //!
   TBranch        *b_jfit_dphiC4H1TJ;   //!
   TBranch        *b_jfit_detaC4H1TJ;   //!

   TagTree(TTree *tree=0);
   virtual ~TagTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef TagTree_cxx
TagTree::TagTree(TTree *tree)
{
   Init(tree);
}

TagTree::~TagTree()
{
  if( !fChain ) { return; }
  //if( fChain->GetCurrentFile() ) { delete fChain->GetCurrentFile(); }
}

Int_t TagTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TagTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void TagTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fChain = 0;

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchStatus("*",1);

   if( fChain->GetBranchStatus("run") ) { fChain->SetBranchAddress("run", &run, &b_run); }
   if( fChain->GetBranchStatus("evt") ) { fChain->SetBranchAddress("evt", &evt, &b_evt); }
   if( fChain->GetBranchStatus("lumiBlock") ) { fChain->SetBranchAddress("lumiBlock", &lumiBlock, &b_lumiBlock); }
   if( fChain->GetBranchStatus("timeStamp") ) { fChain->SetBranchAddress("timeStamp", &timeStamp, &b_timeStamp); }
   if( fChain->GetBranchStatus("lhcBCID") ) { fChain->SetBranchAddress("lhcBCID", &lhcBCID, &b_lhcBCID); }
   if( fChain->GetBranchStatus("MBTStimeDiff") ) { fChain->SetBranchAddress("MBTStimeDiff", &MBTStimeDiff, &b_MBTStimeDiff); }
   if( fChain->GetBranchStatus("LArECtimeDiff") ) { fChain->SetBranchAddress("LArECtimeDiff", &LArECtimeDiff, &b_LArECtimeDiff); }
   if( fChain->GetBranchStatus("syncevt") ) { fChain->SetBranchAddress("syncevt", &syncevt, &b_syncevt); }
   if( fChain->GetBranchStatus("uidmc") ) { fChain->SetBranchAddress("uidmc", &uidmc, &b_uidmc); }
   if( fChain->GetBranchStatus("uidtp") ) { fChain->SetBranchAddress("uidtp", &uidtp, &b_uidtp); }
   if( fChain->GetBranchStatus("nbpvtrue") ) { fChain->SetBranchAddress("nbpvtrue", &nbpvtrue, &b_nbpvtrue); }
   if( fChain->GetBranchStatus("nbpv") ) { fChain->SetBranchAddress("nbpv", &nbpv, &b_nbpv); }
   if( fChain->GetBranchStatus("ntrueparticles") ) { fChain->SetBranchAddress("ntrueparticles", &ntrueparticles, &b_ntrueparticles); }
   if( fChain->GetBranchStatus("ntruetracks") ) { fChain->SetBranchAddress("ntruetracks", &ntruetracks, &b_ntruetracks); }
   if( fChain->GetBranchStatus("ntracks") ) { fChain->SetBranchAddress("ntracks", &ntracks, &b_ntracks); }
   if( fChain->GetBranchStatus("nhits") ) { fChain->SetBranchAddress("nhits", &nhits, &b_nhits); }
   if( fChain->GetBranchStatus("npixclus") ) { fChain->SetBranchAddress("npixclus", &npixclus, &b_npixclus); }
   if( fChain->GetBranchStatus("pvxtrue") ) { fChain->SetBranchAddress("pvxtrue", pvxtrue, &b_pvxtrue); }
   if( fChain->GetBranchStatus("pvytrue") ) { fChain->SetBranchAddress("pvytrue", pvytrue, &b_pvytrue); }
   if( fChain->GetBranchStatus("pvztrue") ) { fChain->SetBranchAddress("pvztrue", pvztrue, &b_pvztrue); }
   if( fChain->GetBranchStatus("pvtype") ) { fChain->SetBranchAddress("pvtype", pvtype, &b_pvtype); }
   if( fChain->GetBranchStatus("pvnbtk") ) { fChain->SetBranchAddress("pvnbtk", pvnbtk, &b_pvnbtk); }
   if( fChain->GetBranchStatus("pvsumpt") ) { fChain->SetBranchAddress("pvsumpt", pvsumpt, &b_pvsumpt); }
   if( fChain->GetBranchStatus("pvx") ) { fChain->SetBranchAddress("pvx", pvx, &b_pvx); }
   if( fChain->GetBranchStatus("pvy") ) { fChain->SetBranchAddress("pvy", pvy, &b_pvy); }
   if( fChain->GetBranchStatus("pvz") ) { fChain->SetBranchAddress("pvz", pvz, &b_pvz); }
   if( fChain->GetBranchStatus("errpvx") ) { fChain->SetBranchAddress("errpvx", errpvx, &b_errpvx); }
   if( fChain->GetBranchStatus("errpvy") ) { fChain->SetBranchAddress("errpvy", errpvy, &b_errpvy); }
   if( fChain->GetBranchStatus("errpvz") ) { fChain->SetBranchAddress("errpvz", errpvz, &b_errpvz); }
   if( fChain->GetBranchStatus("covpvxpvy") ) { fChain->SetBranchAddress("covpvxpvy", covpvxpvy, &b_covpvxpvy); }
   if( fChain->GetBranchStatus("covpvypvz") ) { fChain->SetBranchAddress("covpvypvz", covpvypvz, &b_covpvypvz); }
   if( fChain->GetBranchStatus("covpvzpvx") ) { fChain->SetBranchAddress("covpvzpvx", covpvzpvx, &b_covpvzpvx); }
   if( fChain->GetBranchStatus("trueparticlepx") ) { fChain->SetBranchAddress("trueparticlepx", &trueparticlepx, &b_trueparticlepx); }
   if( fChain->GetBranchStatus("trueparticlepy") ) { fChain->SetBranchAddress("trueparticlepy", &trueparticlepy, &b_trueparticlepy); }
   if( fChain->GetBranchStatus("trueparticlepz") ) { fChain->SetBranchAddress("trueparticlepz", &trueparticlepz, &b_trueparticlepz); }
   if( fChain->GetBranchStatus("trueparticlemass") ) { fChain->SetBranchAddress("trueparticlemass", &trueparticlemass, &b_trueparticlemass); }
   if( fChain->GetBranchStatus("trueparticlepdg") ) { fChain->SetBranchAddress("trueparticlepdg", &trueparticlepdg, &b_trueparticlepdg); }
   if( fChain->GetBranchStatus("trueparticlebarcode") ) { fChain->SetBranchAddress("trueparticlebarcode", &trueparticlebarcode, &b_trueparticlebarcode); }
   if( fChain->GetBranchStatus("trueparticlestatus") ) { fChain->SetBranchAddress("trueparticlestatus", &trueparticlestatus, &b_trueparticlestatus); }
   if( fChain->GetBranchStatus("trueparticlendaughters") ) { fChain->SetBranchAddress("trueparticlendaughters", &trueparticlendaughters, &b_trueparticlendaughters); }
   if( fChain->GetBranchStatus("trueparticledaughteridx") ) { fChain->SetBranchAddress("trueparticledaughteridx", &trueparticledaughteridx, &b_trueparticledaughteridx); }
   if( fChain->GetBranchStatus("trueparticlenmothers") ) { fChain->SetBranchAddress("trueparticlenmothers", &trueparticlenmothers, &b_trueparticlenmothers); }
   if( fChain->GetBranchStatus("trueparticlemotheridx") ) { fChain->SetBranchAddress("trueparticlemotheridx", &trueparticlemotheridx, &b_trueparticlemotheridx); }
   if( fChain->GetBranchStatus("tperigeeok") ) { fChain->SetBranchAddress("tperigeeok", tperigeeok, &b_tperigeeok); }
   if( fChain->GetBranchStatus("td0") ) { fChain->SetBranchAddress("td0", td0, &b_td0); }
   if( fChain->GetBranchStatus("tz0") ) { fChain->SetBranchAddress("tz0", tz0, &b_tz0); }
   if( fChain->GetBranchStatus("tphi") ) { fChain->SetBranchAddress("tphi", tphi, &b_tphi); }
   if( fChain->GetBranchStatus("ttheta") ) { fChain->SetBranchAddress("ttheta", ttheta, &b_ttheta); }
   if( fChain->GetBranchStatus("tqop") ) { fChain->SetBranchAddress("tqop", tqop, &b_tqop); }
   if( fChain->GetBranchStatus("tpt") ) { fChain->SetBranchAddress("tpt", tpt, &b_tpt); }
   if( fChain->GetBranchStatus("teta") ) { fChain->SetBranchAddress("teta", teta, &b_teta); }
   if( fChain->GetBranchStatus("pdg") ) { fChain->SetBranchAddress("pdg", pdg, &b_pdg); }
   if( fChain->GetBranchStatus("barcode") ) { fChain->SetBranchAddress("barcode", barcode, &b_barcode); }
   if( fChain->GetBranchStatus("mybarcode") ) { fChain->SetBranchAddress("mybarcode", mybarcode, &b_mybarcode); }
   if( fChain->GetBranchStatus("pileupevtidx") ) { fChain->SetBranchAddress("pileupevtidx", pileupevtidx, &b_pileupevtidx); }
   if( fChain->GetBranchStatus("begvtxok") ) { fChain->SetBranchAddress("begvtxok", begvtxok, &b_begvtxok); }
   if( fChain->GetBranchStatus("xbegvtx") ) { fChain->SetBranchAddress("xbegvtx", xbegvtx, &b_xbegvtx); }
   if( fChain->GetBranchStatus("ybegvtx") ) { fChain->SetBranchAddress("ybegvtx", ybegvtx, &b_ybegvtx); }
   if( fChain->GetBranchStatus("zbegvtx") ) { fChain->SetBranchAddress("zbegvtx", zbegvtx, &b_zbegvtx); }
   if( fChain->GetBranchStatus("endvtxok") ) { fChain->SetBranchAddress("endvtxok", endvtxok, &b_endvtxok); }
   if( fChain->GetBranchStatus("xendvtx") ) { fChain->SetBranchAddress("xendvtx", xendvtx, &b_xendvtx); }
   if( fChain->GetBranchStatus("yendvtx") ) { fChain->SetBranchAddress("yendvtx", yendvtx, &b_yendvtx); }
   if( fChain->GetBranchStatus("zendvtx") ) { fChain->SetBranchAddress("zendvtx", zendvtx, &b_zendvtx); }
   if( fChain->GetBranchStatus("reco_index") ) { fChain->SetBranchAddress("reco_index", reco_index, &b_reco_index); }
   if( fChain->GetBranchStatus("matched2r") ) { fChain->SetBranchAddress("matched2r", matched2r, &b_matched2r); }
   if( fChain->GetBranchStatus("pvidx") ) { fChain->SetBranchAddress("pvidx", pvidx, &b_pvidx); }
   if( fChain->GetBranchStatus("d0") ) { fChain->SetBranchAddress("d0", d0, &b_d0); }
   if( fChain->GetBranchStatus("z0") ) { fChain->SetBranchAddress("z0", z0, &b_z0); }
   if( fChain->GetBranchStatus("phi") ) { fChain->SetBranchAddress("phi", phi, &b_phi); }
   if( fChain->GetBranchStatus("theta") ) { fChain->SetBranchAddress("theta", theta, &b_theta); }
   if( fChain->GetBranchStatus("qop") ) { fChain->SetBranchAddress("qop", qop, &b_qop); }
   if( fChain->GetBranchStatus("pt") ) { fChain->SetBranchAddress("pt", pt, &b_pt); }
   if( fChain->GetBranchStatus("eta") ) { fChain->SetBranchAddress("eta", eta, &b_eta); }
   if( fChain->GetBranchStatus("errd0") ) { fChain->SetBranchAddress("errd0", errd0, &b_errd0); }
   if( fChain->GetBranchStatus("errz0") ) { fChain->SetBranchAddress("errz0", errz0, &b_errz0); }
   if( fChain->GetBranchStatus("errphi") ) { fChain->SetBranchAddress("errphi", errphi, &b_errphi); }
   if( fChain->GetBranchStatus("errtheta") ) { fChain->SetBranchAddress("errtheta", errtheta, &b_errtheta); }
   if( fChain->GetBranchStatus("errqop") ) { fChain->SetBranchAddress("errqop", errqop, &b_errqop); }
   if( fChain->GetBranchStatus("covd0z0") ) { fChain->SetBranchAddress("covd0z0", covd0z0, &b_covd0z0); }
   if( fChain->GetBranchStatus("covd0phi") ) { fChain->SetBranchAddress("covd0phi", covd0phi, &b_covd0phi); }
   if( fChain->GetBranchStatus("covd0theta") ) { fChain->SetBranchAddress("covd0theta", covd0theta, &b_covd0theta); }
   if( fChain->GetBranchStatus("covd0qop") ) { fChain->SetBranchAddress("covd0qop", covd0qop, &b_covd0qop); }
   if( fChain->GetBranchStatus("covz0phi") ) { fChain->SetBranchAddress("covz0phi", covz0phi, &b_covz0phi); }
   if( fChain->GetBranchStatus("covz0theta") ) { fChain->SetBranchAddress("covz0theta", covz0theta, &b_covz0theta); }
   if( fChain->GetBranchStatus("covz0qop") ) { fChain->SetBranchAddress("covz0qop", covz0qop, &b_covz0qop); }
   if( fChain->GetBranchStatus("covphitheta") ) { fChain->SetBranchAddress("covphitheta", covphitheta, &b_covphitheta); }
   if( fChain->GetBranchStatus("covphiqop") ) { fChain->SetBranchAddress("covphiqop", covphiqop, &b_covphiqop); }
   if( fChain->GetBranchStatus("covthetaqop") ) { fChain->SetBranchAddress("covthetaqop", covthetaqop, &b_covthetaqop); }
   if( fChain->GetBranchStatus("trackPattInfo") ) { fChain->SetBranchAddress("trackPattInfo", trackPattInfo, &b_trackPattInfo); }
   if( fChain->GetBranchStatus("trackPropInfo") ) { fChain->SetBranchAddress("trackPropInfo", trackPropInfo, &b_trackPropInfo); }
   if( fChain->GetBranchStatus("trackFitInfo") ) { fChain->SetBranchAddress("trackFitInfo", trackFitInfo, &b_trackFitInfo); }
   if( fChain->GetBranchStatus("chi2") ) { fChain->SetBranchAddress("chi2", chi2, &b_chi2); }
   if( fChain->GetBranchStatus("ndof") ) { fChain->SetBranchAddress("ndof", ndof, &b_ndof); }
   if( fChain->GetBranchStatus("hitpattern") ) { fChain->SetBranchAddress("hitpattern", hitpattern, &b_hitpattern); }
   if( fChain->GetBranchStatus("nbla") ) { fChain->SetBranchAddress("nbla", nbla, &b_nbla); }
   if( fChain->GetBranchStatus("nblaout") ) { fChain->SetBranchAddress("nblaout", nblaout, &b_nblaout); }
   if( fChain->GetBranchStatus("npix") ) { fChain->SetBranchAddress("npix", npix, &b_npix); }
   if( fChain->GetBranchStatus("nsct") ) { fChain->SetBranchAddress("nsct", nsct, &b_nsct); }
   if( fChain->GetBranchStatus("ntrt") ) { fChain->SetBranchAddress("ntrt", ntrt, &b_ntrt); }
   if( fChain->GetBranchStatus("npixdead") ) { fChain->SetBranchAddress("npixdead", npixdead, &b_npixdead); }
   if( fChain->GetBranchStatus("xpectbla") ) { fChain->SetBranchAddress("xpectbla", xpectbla, &b_xpectbla); }
   if( fChain->GetBranchStatus("nblash") ) { fChain->SetBranchAddress("nblash", nblash, &b_nblash); }
   if( fChain->GetBranchStatus("npixsh") ) { fChain->SetBranchAddress("npixsh", npixsh, &b_npixsh); }
   if( fChain->GetBranchStatus("nsctsh") ) { fChain->SetBranchAddress("nsctsh", nsctsh, &b_nsctsh); }
   if( fChain->GetBranchStatus("nambig") ) { fChain->SetBranchAddress("nambig", nambig, &b_nambig); }
   if( fChain->GetBranchStatus("nganged") ) { fChain->SetBranchAddress("nganged", nganged, &b_nganged); }
   if( fChain->GetBranchStatus("npixho") ) { fChain->SetBranchAddress("npixho", npixho, &b_npixho); }
   if( fChain->GetBranchStatus("nsctho") ) { fChain->SetBranchAddress("nsctho", nsctho, &b_nsctho); }
   if( fChain->GetBranchStatus("blaresx") ) { fChain->SetBranchAddress("blaresx", blaresx, &b_blaresx); }
   if( fChain->GetBranchStatus("blaresy") ) { fChain->SetBranchAddress("blaresy", blaresy, &b_blaresy); }
   if( fChain->GetBranchStatus("blapulx") ) { fChain->SetBranchAddress("blapulx", blapulx, &b_blapulx); }
   if( fChain->GetBranchStatus("blapuly") ) { fChain->SetBranchAddress("blapuly", blapuly, &b_blapuly); }
   if( fChain->GetBranchStatus("true_index") ) { fChain->SetBranchAddress("true_index", true_index, &b_true_index); }
   if( fChain->GetBranchStatus("true_barcode") ) { fChain->SetBranchAddress("true_barcode", true_barcode, &b_true_barcode); }
   if( fChain->GetBranchStatus("true_pileupevtidx") ) { fChain->SetBranchAddress("true_pileupevtidx", true_pileupevtidx, &b_true_pileupevtidx); }
   if( fChain->GetBranchStatus("true_proba") ) { fChain->SetBranchAddress("true_proba", true_proba, &b_true_proba); }
   if( fChain->GetBranchStatus("matched2t") ) { fChain->SetBranchAddress("matched2t", matched2t, &b_matched2t); }
   if( fChain->GetBranchStatus("d0wrtPVunb") ) { fChain->SetBranchAddress("d0wrtPVunb", d0wrtPVunb, &b_d0wrtPVunb); }
   if( fChain->GetBranchStatus("z0wrtPVunb") ) { fChain->SetBranchAddress("z0wrtPVunb", z0wrtPVunb, &b_z0wrtPVunb); }
   if( fChain->GetBranchStatus("errd0wrtPVunb") ) { fChain->SetBranchAddress("errd0wrtPVunb", errd0wrtPVunb, &b_errd0wrtPVunb); }
   if( fChain->GetBranchStatus("errz0wrtPVunb") ) { fChain->SetBranchAddress("errz0wrtPVunb", errz0wrtPVunb, &b_errz0wrtPVunb); }
   if( fChain->GetBranchStatus("d0wrtPVbias") ) { fChain->SetBranchAddress("d0wrtPVbias", d0wrtPVbias, &b_d0wrtPVbias); }
   if( fChain->GetBranchStatus("z0wrtPVbias") ) { fChain->SetBranchAddress("z0wrtPVbias", z0wrtPVbias, &b_z0wrtPVbias); }
   if( fChain->GetBranchStatus("errd0wrtPVbias") ) { fChain->SetBranchAddress("errd0wrtPVbias", errd0wrtPVbias, &b_errd0wrtPVbias); }
   if( fChain->GetBranchStatus("errz0wrtPVbias") ) { fChain->SetBranchAddress("errz0wrtPVbias", errz0wrtPVbias, &b_errz0wrtPVbias); }
   if( fChain->GetBranchStatus("d0wrtBSunb") ) { fChain->SetBranchAddress("d0wrtBSunb", d0wrtBSunb, &b_d0wrtBSunb); }
   if( fChain->GetBranchStatus("z0wrtBSunb") ) { fChain->SetBranchAddress("z0wrtBSunb", z0wrtBSunb, &b_z0wrtBSunb); }
   if( fChain->GetBranchStatus("errd0wrtBSunb") ) { fChain->SetBranchAddress("errd0wrtBSunb", errd0wrtBSunb, &b_errd0wrtBSunb); }
   if( fChain->GetBranchStatus("errz0wrtBSunb") ) { fChain->SetBranchAddress("errz0wrtBSunb", errz0wrtBSunb, &b_errz0wrtBSunb); }
   if( fChain->GetBranchStatus("hdtyp") ) { fChain->SetBranchAddress("hdtyp", hdtyp, &b_hdtyp); }
   if( fChain->GetBranchStatus("hxg") ) { fChain->SetBranchAddress("hxg", hxg, &b_hxg); }
   if( fChain->GetBranchStatus("hyg") ) { fChain->SetBranchAddress("hyg", hyg, &b_hyg); }
   if( fChain->GetBranchStatus("hzg") ) { fChain->SetBranchAddress("hzg", hzg, &b_hzg); }
   if( fChain->GetBranchStatus("heta") ) { fChain->SetBranchAddress("heta", heta, &b_heta); }
   if( fChain->GetBranchStatus("hphi") ) { fChain->SetBranchAddress("hphi", hphi, &b_hphi); }
   if( fChain->GetBranchStatus("hxres") ) { fChain->SetBranchAddress("hxres", hxres, &b_hxres); }
   if( fChain->GetBranchStatus("hyres") ) { fChain->SetBranchAddress("hyres", hyres, &b_hyres); }
   if( fChain->GetBranchStatus("hreco_index") ) { fChain->SetBranchAddress("hreco_index", hreco_index, &b_hreco_index); }
   if( fChain->GetBranchStatus("pcdtyp") ) { fChain->SetBranchAddress("pcdtyp", pcdtyp, &b_pcdtyp); }
   if( fChain->GetBranchStatus("pcxglob") ) { fChain->SetBranchAddress("pcxglob", pcxglob, &b_pcxglob); }
   if( fChain->GetBranchStatus("pcyglob") ) { fChain->SetBranchAddress("pcyglob", pcyglob, &b_pcyglob); }
   if( fChain->GetBranchStatus("pczglob") ) { fChain->SetBranchAddress("pczglob", pczglob, &b_pczglob); }
   if( fChain->GetBranchStatus("pceta") ) { fChain->SetBranchAddress("pceta", pceta, &b_pceta); }
   if( fChain->GetBranchStatus("pcphi") ) { fChain->SetBranchAddress("pcphi", pcphi, &b_pcphi); }
   if( fChain->GetBranchStatus("pcomegax") ) { fChain->SetBranchAddress("pcomegax", pcomegax, &b_pcomegax); }
   if( fChain->GetBranchStatus("pcomegay") ) { fChain->SetBranchAddress("pcomegay", pcomegay, &b_pcomegay); }
   if( fChain->GetBranchStatus("pcxloc") ) { fChain->SetBranchAddress("pcxloc", pcxloc, &b_pcxloc); }
   if( fChain->GetBranchStatus("pcyloc") ) { fChain->SetBranchAddress("pcyloc", pcyloc, &b_pcyloc); }
   if( fChain->GetBranchStatus("pcxlocerr") ) { fChain->SetBranchAddress("pcxlocerr", pcxlocerr, &b_pcxlocerr); }
   if( fChain->GetBranchStatus("pcylocerr") ) { fChain->SetBranchAddress("pcylocerr", pcylocerr, &b_pcylocerr); }
   if( fChain->GetBranchStatus("pcgroupsize") ) { fChain->SetBranchAddress("pcgroupsize", pcgroupsize, &b_pcgroupsize); }
   if( fChain->GetBranchStatus("pclayer") ) { fChain->SetBranchAddress("pclayer", pclayer, &b_pclayer); }
   if( fChain->GetBranchStatus("pcetamodule") ) { fChain->SetBranchAddress("pcetamodule", pcetamodule, &b_pcetamodule); }
   if( fChain->GetBranchStatus("pcphimodule") ) { fChain->SetBranchAddress("pcphimodule", pcphimodule, &b_pcphimodule); }
   if( fChain->GetBranchStatus("pcrow") ) { fChain->SetBranchAddress("pcrow", pcrow, &b_pcrow); }
   if( fChain->GetBranchStatus("pccol") ) { fChain->SetBranchAddress("pccol", pccol, &b_pccol); }
   if( fChain->GetBranchStatus("pcecbarrel") ) { fChain->SetBranchAddress("pcecbarrel", pcecbarrel, &b_pcecbarrel); }
   if( fChain->GetBranchStatus("pcdeltarow") ) { fChain->SetBranchAddress("pcdeltarow", pcdeltarow, &b_pcdeltarow); }
   if( fChain->GetBranchStatus("pcdeltacol") ) { fChain->SetBranchAddress("pcdeltacol", pcdeltacol, &b_pcdeltacol); }
   if( fChain->GetBranchStatus("pctot") ) { fChain->SetBranchAddress("pctot", pctot, &b_pctot); }
   if( fChain->GetBranchStatus("pccharge") ) { fChain->SetBranchAddress("pccharge", pccharge, &b_pccharge); }
   if( fChain->GetBranchStatus("pclvl1") ) { fChain->SetBranchAddress("pclvl1", pclvl1, &b_pclvl1); }
   if( fChain->GetBranchStatus("pcganged") ) { fChain->SetBranchAddress("pcganged", pcganged, &b_pcganged); }
   if( fChain->GetBranchStatus("pcfake") ) { fChain->SetBranchAddress("pcfake", pcfake, &b_pcfake); }
   if( fChain->GetBranchStatus("pcambig") ) { fChain->SetBranchAddress("pcambig", pcambig, &b_pcambig); }
   if( fChain->GetBranchStatus("pcrecoindex") ) { fChain->SetBranchAddress("pcrecoindex", pcrecoindex, &b_pcrecoindex); }
   if( fChain->GetBranchStatus("njetsC4H1TJ") ) { fChain->SetBranchAddress("njetsC4H1TJ", &njetsC4H1TJ, &b_njetsC4H1TJ); }
   if( fChain->GetBranchStatus("phiC4H1TJ") ) { fChain->SetBranchAddress("phiC4H1TJ", phiC4H1TJ, &b_phiC4H1TJ); }
   if( fChain->GetBranchStatus("etaC4H1TJ") ) { fChain->SetBranchAddress("etaC4H1TJ", etaC4H1TJ, &b_etaC4H1TJ); }
   if( fChain->GetBranchStatus("ptC4H1TJ") ) { fChain->SetBranchAddress("ptC4H1TJ", ptC4H1TJ, &b_ptC4H1TJ); }
   if( fChain->GetBranchStatus("eC4H1TJ") ) { fChain->SetBranchAddress("eC4H1TJ", eC4H1TJ, &b_eC4H1TJ); }
   if( fChain->GetBranchStatus("ntrakC4H1TJ") ) { fChain->SetBranchAddress("ntrakC4H1TJ", ntrakC4H1TJ, &b_ntrakC4H1TJ); }
   if( fChain->GetBranchStatus("trakassoc_trkindexC4H1TJ") ) { fChain->SetBranchAddress("trakassoc_trkindexC4H1TJ", trakassoc_trkindexC4H1TJ, &b_trakassoc_trkindexC4H1TJ); }
   if( fChain->GetBranchStatus("nmuonC4H1TJ") ) { fChain->SetBranchAddress("nmuonC4H1TJ", nmuonC4H1TJ, &b_nmuonC4H1TJ); }
   if( fChain->GetBranchStatus("muonassoc_trkindexC4H1TJ") ) { fChain->SetBranchAddress("muonassoc_trkindexC4H1TJ", muonassoc_trkindexC4H1TJ, &b_muonassoc_trkindexC4H1TJ); }
   if( fChain->GetBranchStatus("nelecC4H1TJ") ) { fChain->SetBranchAddress("nelecC4H1TJ", nelecC4H1TJ, &b_nelecC4H1TJ); }
   if( fChain->GetBranchStatus("elecassoc_trkindexC4H1TJ") ) { fChain->SetBranchAddress("elecassoc_trkindexC4H1TJ", elecassoc_trkindexC4H1TJ, &b_elecassoc_trkindexC4H1TJ); }
   if( fChain->GetBranchStatus("nphotC4H1TJ") ) { fChain->SetBranchAddress("nphotC4H1TJ", nphotC4H1TJ, &b_nphotC4H1TJ); }
   if( fChain->GetBranchStatus("photassoc_trkindexC4H1TJ") ) { fChain->SetBranchAddress("photassoc_trkindexC4H1TJ", photassoc_trkindexC4H1TJ, &b_photassoc_trkindexC4H1TJ); }
   if( fChain->GetBranchStatus("trakassoc_trkd0valC4H1TJ") ) { fChain->SetBranchAddress("trakassoc_trkd0valC4H1TJ", trakassoc_trkd0valC4H1TJ, &b_trakassoc_trkd0valC4H1TJ); }
   if( fChain->GetBranchStatus("trakassoc_trkd0sigC4H1TJ") ) { fChain->SetBranchAddress("trakassoc_trkd0sigC4H1TJ", trakassoc_trkd0sigC4H1TJ, &b_trakassoc_trkd0sigC4H1TJ); }
   if( fChain->GetBranchStatus("trakassoc_trkz0valC4H1TJ") ) { fChain->SetBranchAddress("trakassoc_trkz0valC4H1TJ", trakassoc_trkz0valC4H1TJ, &b_trakassoc_trkz0valC4H1TJ); }
   if( fChain->GetBranchStatus("trakassoc_trkz0sigC4H1TJ") ) { fChain->SetBranchAddress("trakassoc_trkz0sigC4H1TJ", trakassoc_trkz0sigC4H1TJ, &b_trakassoc_trkz0sigC4H1TJ); }
   if( fChain->GetBranchStatus("labelC4H1TJ") ) { fChain->SetBranchAddress("labelC4H1TJ", labelC4H1TJ, &b_labelC4H1TJ); }
   if( fChain->GetBranchStatus("drminbC4H1TJ") ) { fChain->SetBranchAddress("drminbC4H1TJ", drminbC4H1TJ, &b_drminbC4H1TJ); }
   if( fChain->GetBranchStatus("drmincC4H1TJ") ) { fChain->SetBranchAddress("drmincC4H1TJ", drmincC4H1TJ, &b_drmincC4H1TJ); }
   if( fChain->GetBranchStatus("drmintC4H1TJ") ) { fChain->SetBranchAddress("drmintC4H1TJ", drmintC4H1TJ, &b_drmintC4H1TJ); }
   if( fChain->GetBranchStatus("truebpdgC4H1TJ") ) { fChain->SetBranchAddress("truebpdgC4H1TJ", truebpdgC4H1TJ, &b_truebpdgC4H1TJ); }
   if( fChain->GetBranchStatus("truebvtxC4H1TJ") ) { fChain->SetBranchAddress("truebvtxC4H1TJ", truebvtxC4H1TJ, &b_truebvtxC4H1TJ); }
   if( fChain->GetBranchStatus("truebvtyC4H1TJ") ) { fChain->SetBranchAddress("truebvtyC4H1TJ", truebvtyC4H1TJ, &b_truebvtyC4H1TJ); }
   if( fChain->GetBranchStatus("truebvtzC4H1TJ") ) { fChain->SetBranchAddress("truebvtzC4H1TJ", truebvtzC4H1TJ, &b_truebvtzC4H1TJ); }
   if( fChain->GetBranchStatus("ntagsC4H1TJ") ) { fChain->SetBranchAddress("ntagsC4H1TJ", ntagsC4H1TJ, &b_ntagsC4H1TJ); }
   if( fChain->GetBranchStatus("ip2d_wC4H1TJ") ) { fChain->SetBranchAddress("ip2d_wC4H1TJ", ip2d_wC4H1TJ, &b_ip2d_wC4H1TJ); }
   if( fChain->GetBranchStatus("ip3d_wC4H1TJ") ) { fChain->SetBranchAddress("ip3d_wC4H1TJ", ip3d_wC4H1TJ, &b_ip3d_wC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_wC4H1TJ") ) { fChain->SetBranchAddress("sv0_wC4H1TJ", sv0_wC4H1TJ, &b_sv0_wC4H1TJ); }
   if( fChain->GetBranchStatus("sv1_wC4H1TJ") ) { fChain->SetBranchAddress("sv1_wC4H1TJ", sv1_wC4H1TJ, &b_sv1_wC4H1TJ); }
   if( fChain->GetBranchStatus("sv2_wC4H1TJ") ) { fChain->SetBranchAddress("sv2_wC4H1TJ", sv2_wC4H1TJ, &b_sv2_wC4H1TJ); }
   if( fChain->GetBranchStatus("cmb_wC4H1TJ") ) { fChain->SetBranchAddress("cmb_wC4H1TJ", cmb_wC4H1TJ, &b_cmb_wC4H1TJ); }
   if( fChain->GetBranchStatus("jetp_wC4H1TJ") ) { fChain->SetBranchAddress("jetp_wC4H1TJ", jetp_wC4H1TJ, &b_jetp_wC4H1TJ); }
   if( fChain->GetBranchStatus("jetpneg_wC4H1TJ") ) { fChain->SetBranchAddress("jetpneg_wC4H1TJ", jetpneg_wC4H1TJ, &b_jetpneg_wC4H1TJ); }
   if( fChain->GetBranchStatus("trkc_wC4H1TJ") ) { fChain->SetBranchAddress("trkc_wC4H1TJ", trkc_wC4H1TJ, &b_trkc_wC4H1TJ); }
   if( fChain->GetBranchStatus("lf2d_wC4H1TJ") ) { fChain->SetBranchAddress("lf2d_wC4H1TJ", lf2d_wC4H1TJ, &b_lf2d_wC4H1TJ); }
   if( fChain->GetBranchStatus("svbu_wC4H1TJ") ) { fChain->SetBranchAddress("svbu_wC4H1TJ", svbu_wC4H1TJ, &b_svbu_wC4H1TJ); }
   if( fChain->GetBranchStatus("lhsig_wC4H1TJ") ) { fChain->SetBranchAddress("lhsig_wC4H1TJ", lhsig_wC4H1TJ, &b_lhsig_wC4H1TJ); }
   if( fChain->GetBranchStatus("softm_wC4H1TJ") ) { fChain->SetBranchAddress("softm_wC4H1TJ", softm_wC4H1TJ, &b_softm_wC4H1TJ); }
   if( fChain->GetBranchStatus("softe_wC4H1TJ") ) { fChain->SetBranchAddress("softe_wC4H1TJ", softe_wC4H1TJ, &b_softe_wC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_wC4H1TJ") ) { fChain->SetBranchAddress("jfit_wC4H1TJ", jfit_wC4H1TJ, &b_jfit_wC4H1TJ); }
   if( fChain->GetBranchStatus("jfitNN_wC4H1TJ") ) { fChain->SetBranchAddress("jfitNN_wC4H1TJ", jfitNN_wC4H1TJ, &b_jfitNN_wC4H1TJ); }
   if( fChain->GetBranchStatus("ipplus_ntrkC4H1TJ") ) { fChain->SetBranchAddress("ipplus_ntrkC4H1TJ", ipplus_ntrkC4H1TJ, &b_ipplus_ntrkC4H1TJ); }
   if( fChain->GetBranchStatus("svplus_ntrkC4H1TJ") ) { fChain->SetBranchAddress("svplus_ntrkC4H1TJ", svplus_ntrkC4H1TJ, &b_svplus_ntrkC4H1TJ); }
   if( fChain->GetBranchStatus("sv0plus_ntrkC4H1TJ") ) { fChain->SetBranchAddress("sv0plus_ntrkC4H1TJ", sv0plus_ntrkC4H1TJ, &b_sv0plus_ntrkC4H1TJ); }
   if( fChain->GetBranchStatus("ip2d_pbC4H1TJ") ) { fChain->SetBranchAddress("ip2d_pbC4H1TJ", ip2d_pbC4H1TJ, &b_ip2d_pbC4H1TJ); }
   if( fChain->GetBranchStatus("ip2d_puC4H1TJ") ) { fChain->SetBranchAddress("ip2d_puC4H1TJ", ip2d_puC4H1TJ, &b_ip2d_puC4H1TJ); }
   if( fChain->GetBranchStatus("ip2d_ntrkC4H1TJ") ) { fChain->SetBranchAddress("ip2d_ntrkC4H1TJ", ip2d_ntrkC4H1TJ, &b_ip2d_ntrkC4H1TJ); }
   if( fChain->GetBranchStatus("ip2d_trkwC4H1TJ") ) { fChain->SetBranchAddress("ip2d_trkwC4H1TJ", ip2d_trkwC4H1TJ, &b_ip2d_trkwC4H1TJ); }
   if( fChain->GetBranchStatus("ip3d_pbC4H1TJ") ) { fChain->SetBranchAddress("ip3d_pbC4H1TJ", ip3d_pbC4H1TJ, &b_ip3d_pbC4H1TJ); }
   if( fChain->GetBranchStatus("ip3d_puC4H1TJ") ) { fChain->SetBranchAddress("ip3d_puC4H1TJ", ip3d_puC4H1TJ, &b_ip3d_puC4H1TJ); }
   if( fChain->GetBranchStatus("ip3d_ntrkC4H1TJ") ) { fChain->SetBranchAddress("ip3d_ntrkC4H1TJ", ip3d_ntrkC4H1TJ, &b_ip3d_ntrkC4H1TJ); }
   if( fChain->GetBranchStatus("ip3d_trkwC4H1TJ") ) { fChain->SetBranchAddress("ip3d_trkwC4H1TJ", ip3d_trkwC4H1TJ, &b_ip3d_trkwC4H1TJ); }
   if( fChain->GetBranchStatus("jetp_trkwC4H1TJ") ) { fChain->SetBranchAddress("jetp_trkwC4H1TJ", jetp_trkwC4H1TJ, &b_jetp_trkwC4H1TJ); }
   if( fChain->GetBranchStatus("jetpneg_trkwC4H1TJ") ) { fChain->SetBranchAddress("jetpneg_trkwC4H1TJ", jetpneg_trkwC4H1TJ, &b_jetpneg_trkwC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkindexC4H1TJ") ) { fChain->SetBranchAddress("ip_trkindexC4H1TJ", ip_trkindexC4H1TJ, &b_ip_trkindexC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkgradeC4H1TJ") ) { fChain->SetBranchAddress("ip_trkgradeC4H1TJ", ip_trkgradeC4H1TJ, &b_ip_trkgradeC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkv0intC4H1TJ") ) { fChain->SetBranchAddress("ip_trkv0intC4H1TJ", ip_trkv0intC4H1TJ, &b_ip_trkv0intC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkd0valC4H1TJ") ) { fChain->SetBranchAddress("ip_trkd0valC4H1TJ", ip_trkd0valC4H1TJ, &b_ip_trkd0valC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkd0sigC4H1TJ") ) { fChain->SetBranchAddress("ip_trkd0sigC4H1TJ", ip_trkd0sigC4H1TJ, &b_ip_trkd0sigC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkz0valC4H1TJ") ) { fChain->SetBranchAddress("ip_trkz0valC4H1TJ", ip_trkz0valC4H1TJ, &b_ip_trkz0valC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkz0sigC4H1TJ") ) { fChain->SetBranchAddress("ip_trkz0sigC4H1TJ", ip_trkz0sigC4H1TJ, &b_ip_trkz0sigC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkptC4H1TJ") ) { fChain->SetBranchAddress("ip_trkptC4H1TJ", ip_trkptC4H1TJ, &b_ip_trkptC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trkphiC4H1TJ") ) { fChain->SetBranchAddress("ip_trkphiC4H1TJ", ip_trkphiC4H1TJ, &b_ip_trkphiC4H1TJ); }
   if( fChain->GetBranchStatus("ip_trketaC4H1TJ") ) { fChain->SetBranchAddress("ip_trketaC4H1TJ", ip_trketaC4H1TJ, &b_ip_trketaC4H1TJ); }
   if( fChain->GetBranchStatus("sv1_pbC4H1TJ") ) { fChain->SetBranchAddress("sv1_pbC4H1TJ", sv1_pbC4H1TJ, &b_sv1_pbC4H1TJ); }
   if( fChain->GetBranchStatus("sv1_puC4H1TJ") ) { fChain->SetBranchAddress("sv1_puC4H1TJ", sv1_puC4H1TJ, &b_sv1_puC4H1TJ); }
   if( fChain->GetBranchStatus("sv2_pbC4H1TJ") ) { fChain->SetBranchAddress("sv2_pbC4H1TJ", sv2_pbC4H1TJ, &b_sv2_pbC4H1TJ); }
   if( fChain->GetBranchStatus("sv2_puC4H1TJ") ) { fChain->SetBranchAddress("sv2_puC4H1TJ", sv2_puC4H1TJ, &b_sv2_puC4H1TJ); }
   if( fChain->GetBranchStatus("sv_okC4H1TJ") ) { fChain->SetBranchAddress("sv_okC4H1TJ", sv_okC4H1TJ, &b_sv_okC4H1TJ); }
   if( fChain->GetBranchStatus("sv_ntrkvC4H1TJ") ) { fChain->SetBranchAddress("sv_ntrkvC4H1TJ", sv_ntrkvC4H1TJ, &b_sv_ntrkvC4H1TJ); }
   if( fChain->GetBranchStatus("sv_ntrkjC4H1TJ") ) { fChain->SetBranchAddress("sv_ntrkjC4H1TJ", sv_ntrkjC4H1TJ, &b_sv_ntrkjC4H1TJ); }
   if( fChain->GetBranchStatus("sv_massC4H1TJ") ) { fChain->SetBranchAddress("sv_massC4H1TJ", sv_massC4H1TJ, &b_sv_massC4H1TJ); }
   if( fChain->GetBranchStatus("sv_efrcC4H1TJ") ) { fChain->SetBranchAddress("sv_efrcC4H1TJ", sv_efrcC4H1TJ, &b_sv_efrcC4H1TJ); }
   if( fChain->GetBranchStatus("sv_nv2tC4H1TJ") ) { fChain->SetBranchAddress("sv_nv2tC4H1TJ", sv_nv2tC4H1TJ, &b_sv_nv2tC4H1TJ); }
   if( fChain->GetBranchStatus("sv_xC4H1TJ") ) { fChain->SetBranchAddress("sv_xC4H1TJ", sv_xC4H1TJ, &b_sv_xC4H1TJ); }
   if( fChain->GetBranchStatus("sv_yC4H1TJ") ) { fChain->SetBranchAddress("sv_yC4H1TJ", sv_yC4H1TJ, &b_sv_yC4H1TJ); }
   if( fChain->GetBranchStatus("sv_zC4H1TJ") ) { fChain->SetBranchAddress("sv_zC4H1TJ", sv_zC4H1TJ, &b_sv_zC4H1TJ); }
   if( fChain->GetBranchStatus("sv_err_xC4H1TJ") ) { fChain->SetBranchAddress("sv_err_xC4H1TJ", sv_err_xC4H1TJ, &b_sv_err_xC4H1TJ); }
   if( fChain->GetBranchStatus("sv_err_yC4H1TJ") ) { fChain->SetBranchAddress("sv_err_yC4H1TJ", sv_err_yC4H1TJ, &b_sv_err_yC4H1TJ); }
   if( fChain->GetBranchStatus("sv_err_zC4H1TJ") ) { fChain->SetBranchAddress("sv_err_zC4H1TJ", sv_err_zC4H1TJ, &b_sv_err_zC4H1TJ); }
   if( fChain->GetBranchStatus("sv_cov_xyC4H1TJ") ) { fChain->SetBranchAddress("sv_cov_xyC4H1TJ", sv_cov_xyC4H1TJ, &b_sv_cov_xyC4H1TJ); }
   if( fChain->GetBranchStatus("sv_cov_xzC4H1TJ") ) { fChain->SetBranchAddress("sv_cov_xzC4H1TJ", sv_cov_xzC4H1TJ, &b_sv_cov_xzC4H1TJ); }
   if( fChain->GetBranchStatus("sv_cov_yzC4H1TJ") ) { fChain->SetBranchAddress("sv_cov_yzC4H1TJ", sv_cov_yzC4H1TJ, &b_sv_cov_yzC4H1TJ); }
   if( fChain->GetBranchStatus("sv_chi2C4H1TJ") ) { fChain->SetBranchAddress("sv_chi2C4H1TJ", sv_chi2C4H1TJ, &b_sv_chi2C4H1TJ); }
   if( fChain->GetBranchStatus("sv_ndofC4H1TJ") ) { fChain->SetBranchAddress("sv_ndofC4H1TJ", sv_ndofC4H1TJ, &b_sv_ndofC4H1TJ); }
   if( fChain->GetBranchStatus("sv_trkindexC4H1TJ") ) { fChain->SetBranchAddress("sv_trkindexC4H1TJ", sv_trkindexC4H1TJ, &b_sv_trkindexC4H1TJ); }
   if( fChain->GetBranchStatus("sv_trkptC4H1TJ") ) { fChain->SetBranchAddress("sv_trkptC4H1TJ", sv_trkptC4H1TJ, &b_sv_trkptC4H1TJ); }
   if( fChain->GetBranchStatus("sv_trkphiC4H1TJ") ) { fChain->SetBranchAddress("sv_trkphiC4H1TJ", sv_trkphiC4H1TJ, &b_sv_trkphiC4H1TJ); }
   if( fChain->GetBranchStatus("sv_trketaC4H1TJ") ) { fChain->SetBranchAddress("sv_trketaC4H1TJ", sv_trketaC4H1TJ, &b_sv_trketaC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_okC4H1TJ") ) { fChain->SetBranchAddress("sv0_okC4H1TJ", sv0_okC4H1TJ, &b_sv0_okC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_ntrkvC4H1TJ") ) { fChain->SetBranchAddress("sv0_ntrkvC4H1TJ", sv0_ntrkvC4H1TJ, &b_sv0_ntrkvC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_ntrkjC4H1TJ") ) { fChain->SetBranchAddress("sv0_ntrkjC4H1TJ", sv0_ntrkjC4H1TJ, &b_sv0_ntrkjC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_massC4H1TJ") ) { fChain->SetBranchAddress("sv0_massC4H1TJ", sv0_massC4H1TJ, &b_sv0_massC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_efrcC4H1TJ") ) { fChain->SetBranchAddress("sv0_efrcC4H1TJ", sv0_efrcC4H1TJ, &b_sv0_efrcC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_nv2tC4H1TJ") ) { fChain->SetBranchAddress("sv0_nv2tC4H1TJ", sv0_nv2tC4H1TJ, &b_sv0_nv2tC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_xC4H1TJ") ) { fChain->SetBranchAddress("sv0_xC4H1TJ", sv0_xC4H1TJ, &b_sv0_xC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_yC4H1TJ") ) { fChain->SetBranchAddress("sv0_yC4H1TJ", sv0_yC4H1TJ, &b_sv0_yC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_zC4H1TJ") ) { fChain->SetBranchAddress("sv0_zC4H1TJ", sv0_zC4H1TJ, &b_sv0_zC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_err_xC4H1TJ") ) { fChain->SetBranchAddress("sv0_err_xC4H1TJ", sv0_err_xC4H1TJ, &b_sv0_err_xC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_err_yC4H1TJ") ) { fChain->SetBranchAddress("sv0_err_yC4H1TJ", sv0_err_yC4H1TJ, &b_sv0_err_yC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_err_zC4H1TJ") ) { fChain->SetBranchAddress("sv0_err_zC4H1TJ", sv0_err_zC4H1TJ, &b_sv0_err_zC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_cov_xyC4H1TJ") ) { fChain->SetBranchAddress("sv0_cov_xyC4H1TJ", sv0_cov_xyC4H1TJ, &b_sv0_cov_xyC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_cov_xzC4H1TJ") ) { fChain->SetBranchAddress("sv0_cov_xzC4H1TJ", sv0_cov_xzC4H1TJ, &b_sv0_cov_xzC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_cov_yzC4H1TJ") ) { fChain->SetBranchAddress("sv0_cov_yzC4H1TJ", sv0_cov_yzC4H1TJ, &b_sv0_cov_yzC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_chi2C4H1TJ") ) { fChain->SetBranchAddress("sv0_chi2C4H1TJ", sv0_chi2C4H1TJ, &b_sv0_chi2C4H1TJ); }
   if( fChain->GetBranchStatus("sv0_ndofC4H1TJ") ) { fChain->SetBranchAddress("sv0_ndofC4H1TJ", sv0_ndofC4H1TJ, &b_sv0_ndofC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_trkindexC4H1TJ") ) { fChain->SetBranchAddress("sv0_trkindexC4H1TJ", sv0_trkindexC4H1TJ, &b_sv0_trkindexC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_trkptC4H1TJ") ) { fChain->SetBranchAddress("sv0_trkptC4H1TJ", sv0_trkptC4H1TJ, &b_sv0_trkptC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_trkphiC4H1TJ") ) { fChain->SetBranchAddress("sv0_trkphiC4H1TJ", sv0_trkphiC4H1TJ, &b_sv0_trkphiC4H1TJ); }
   if( fChain->GetBranchStatus("sv0_trketaC4H1TJ") ) { fChain->SetBranchAddress("sv0_trketaC4H1TJ", sv0_trketaC4H1TJ, &b_sv0_trketaC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_pbC4H1TJ") ) { fChain->SetBranchAddress("jfit_pbC4H1TJ", jfit_pbC4H1TJ, &b_jfit_pbC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_puC4H1TJ") ) { fChain->SetBranchAddress("jfit_puC4H1TJ", jfit_puC4H1TJ, &b_jfit_puC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_nvtxntC4H1TJ") ) { fChain->SetBranchAddress("jfit_nvtxntC4H1TJ", jfit_nvtxntC4H1TJ, &b_jfit_nvtxntC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_nvtx1tC4H1TJ") ) { fChain->SetBranchAddress("jfit_nvtx1tC4H1TJ", jfit_nvtx1tC4H1TJ, &b_jfit_nvtx1tC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_ntrkC4H1TJ") ) { fChain->SetBranchAddress("jfit_ntrkC4H1TJ", jfit_ntrkC4H1TJ, &b_jfit_ntrkC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_efrcC4H1TJ") ) { fChain->SetBranchAddress("jfit_efrcC4H1TJ", jfit_efrcC4H1TJ, &b_jfit_efrcC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_massC4H1TJ") ) { fChain->SetBranchAddress("jfit_massC4H1TJ", jfit_massC4H1TJ, &b_jfit_massC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_sig3dC4H1TJ") ) { fChain->SetBranchAddress("jfit_sig3dC4H1TJ", jfit_sig3dC4H1TJ, &b_jfit_sig3dC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_dphiC4H1TJ") ) { fChain->SetBranchAddress("jfit_dphiC4H1TJ", jfit_dphiC4H1TJ, &b_jfit_dphiC4H1TJ); }
   if( fChain->GetBranchStatus("jfit_detaC4H1TJ") ) { fChain->SetBranchAddress("jfit_detaC4H1TJ", jfit_detaC4H1TJ, &b_jfit_detaC4H1TJ); }

   Notify();
}

Bool_t TagTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TagTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TagTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef TagTree_cxx
