#include "TrigFTKAna/AnaCommon.hpp"
bool RUNZ = false;                     // run Z selection and histograms? [was:true 05/13/2013]
bool RUNZ_NT = true;                   // save Z ntuples? This is set automatically for MCP studies [was:true 05/13/2013]
bool RUN_QCD_SLICES = true;            // save QCD normalization histograms?
bool NOMONLY = false;                  // skip systematics and only do NOMINAL case?
const bool D0CUT = false;              // optional cut on d0 signficance?
const bool NUMET = true;               // use neutrinos as a proxy for truth-MET?
const bool MULTIJET_JES = false;       // utilize MultijetJESProvider instead of JetJESProvider? (close-by jet + flavor effects)
const bool STUDY_CALJET = false;       // study differences between CALJETs and RAWJETs in MET propagation?
const int TRIGSCALE_CHOICE = 20;       // 2=EtaPt binning; 20=Oct25 version from MCP+Max; 21=20+eta/phi correction
const bool UNFOLD_TREE        = false; // besides histograms, save ufolding tree?
const bool UNFOLD_TREE_REDUCE = true;  // remove all optional branches from unfolding tree?
const int PDF_REWEIGHTING = 1;         // 0=None, 1=CT10(members), 2=CT10(members)+5(families)
const bool ZMUMU_FIX = true;           // Fix ZMUMU sample overlap with DYAN?
unsigned int NREPLICASF = 100;         // statistical error replicas (propagated into response matrices)
const unsigned int NREPLICASP = 53;    // PDF uncertainty replicas
const unsigned int NREPLICASB = 1000;  // bootstrap replicas (data)
bool RUN_BOOTSTRAP = true;             // generate bootstrap histograms? For now, auto-disabled for MC
// Control application of certain weights to histograms. All should be "true"
const bool HIST_VXZ_WEIGHT = true;     // vertex z0 weight - needs V1_29i ntuple
const bool HIST_WZPT_WEIGHT = true;    // W/Z pt weight
const bool HIST_ZNLO_WEIGHT = true;    // observed MC@NLO generator deficit in "Z samples" in ~60-65 GeV mass region
const bool HIST_ALPY_WEIGHT = true;    // alpgen reweighting
const bool WPOL_PHYS_CONSTRAIN = false;
const bool HIST_LS_WEIGHT = true;      // W/Z lineshape weight

#define WZPT_WEIGHT_TARGET "PowhegPythia8MC11_ZData2011" // nominal
#define WZPT_WEIGHT_TARGET2 "Sherpa14MC11"
#define WZPT_WEIGHT_TARGET3 "PowhegPythia8MC11"

#include <set>
#include <string>
#include <vector>
#include <list>
#include <bitset>
#include <algorithm>
#include <functional>
#include <iterator>
#include <cassert>
#include <cmath>
#include <ctime>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/scope_exit.hpp>
#include <boost/foreach.hpp>
#include <boost/function.hpp>
#include <boost/format.hpp>
#include <boost/any.hpp>
#include <boost/ref.hpp>
#include <boost/unordered_map.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <iostream>
#include <iomanip>
#include "TrigFTKAna/AnaConfiguration.hpp"
#include "TrigFTKAna/AnaEventMgr.hpp"
#include "TrigFTKAna/AnaTrack.hpp"
#include "TrigFTKAna/AnaTruthParticle.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"
#include "TrigFTKAna/AnaTrigger.hpp"
#include "TrigFTKAna/AnaWCrossSectionCandidate.hpp"
#include "TrigFTKAna/AnaZCrossSectionCandidate.hpp"
#include "TrigFTKAna/MuonEtaPhiBinning.h"
#include "TrigFTKAna/mcp/MuonAllAuthor.hpp"
#include "TrigFTKAna/StudyParticle.hpp"
#include "TrigFTKAna/StudyTruthParticle.hpp"
#include "TrigFTKAna/StudyWAsymmetry.hpp"
#include "TrigFTKAna/StudyZAsymmetry.hpp"
#include "TrigFTKAna/EventObjectColl.hpp"
#include "TrigFTKAna/CompositeGenerator.hpp"
#include "TrigFTKAna/Conference.hpp"
#include "DataGraphics/DataGraphics.hh"
#include "DataGraphics/DgCutSet.hh"
#include "DataGraphics/DgSTL.hh"
#include <DataGraphics/DgTools.hh>
#include <DataGraphics/DgBin.hh>
#include <TTree.h>
#include <TCanvas.h>

// special classes
#include "PileupReweighting/TPileupReweighting.h"
#include "GoodRunsLists/TGoodRunsListReader.h"
#include "GoodRunsLists/TGoodRunsList.h"
#include "MissingETUtility/METUtility.h"
#include "egammaAnalysisUtils/BosonPtReweightingTool.h"
#include "egammaAnalysisUtils/VertexPositionReweightingTool.h"
#include "LineShapeTool/LineShapeTool.h"
#include "LineShapeTool/GeneratorSettings.h"
#include "ScaleFactorProvider/SFProvider.h"
#include "ScaleFactorProvider/WeightProvider.h"

// pdf reweighting (only in 64 bits: needs 3 GB of VMEM)
#include "TrigFTKAna/misc/PDFReweightTool.hpp"

using namespace std;
using namespace DataGraphics;
using namespace boost;
namespace mb = MuonEtaPhiBinning;
#define boost_foreach BOOST_FOREACH

// shortcuts for cuts, collections, and types
typedef EventObjectColl< boost::shared_ptr<const AnaTruthParticle> > TPCOL;
typedef DgCutSet< boost::shared_ptr<const AnaTruthParticle> > TPCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaMuon> > MUCOL;
typedef DgCutSet< boost::shared_ptr<const AnaMuon> > MUCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaElectron> > ELCOL;
typedef DgCutSet< boost::shared_ptr<const AnaElectron> > ELCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaPhoton> > PHCOL;
typedef DgCutSet< boost::shared_ptr<const AnaPhoton> > PHCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaJet> > JETCOL;
typedef DgCutSet< boost::shared_ptr<const AnaJet> > JETCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaMET> > METCOL;
typedef DgCutSet< boost::shared_ptr<const AnaMET> > METCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaVertex> > VXCOL;
typedef DgCutSet< boost::shared_ptr<const AnaVertex> > VXCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaW> > WCOL;
typedef EventObjectColl< boost::shared_ptr<const AnaWCrossSectionCandidate> > WXCOL;
typedef DgCutSet< boost::shared_ptr<const AnaWCrossSectionCandidate> > WXCUT;
typedef EventObjectColl< boost::shared_ptr<const AnaZ> > ZCOL;
typedef EventObjectColl< boost::shared_ptr<const AnaZCrossSectionCandidate> > ZXCOL;
typedef DgCutSet< boost::shared_ptr<const AnaZCrossSectionCandidate> > ZXCUT;

// Monte-Carlo tools
enum MC_TYPE { MC_PYTHIA , MC_ALPGEN_HERWIG , MC_ALPGEN_PYTHIA , MC_MCNLO , MC_POWHEG_HERWIG , MC_POWHEG_PYTHIA , MC_SHERPA , MC_NONE};
enum TRUTH_STATUS { STABLE, POSTFSR, BORN };
bool is_heavy_flavor(unsigned int chan);
MC_TYPE get_mc_type(unsigned int chan);
int get_wz_status(const MC_TYPE& mctype , const TRUTH_STATUS& mcstatus);
void get_munu_status_parent(bool is_w , const MC_TYPE& mctype , const TRUTH_STATUS& mcstatus, int& status, int& mu_parent, int& nu_parent);
const std::string& get_wz_reweight_source(const MC_TYPE& mctype);
const VertexPositionReweightingTool::eDataMCBeamspot get_mc_beamspot(const std::string& fname);
void pdfinfo_lineshape_quarks(int& qid1, int& qid2, const int wz_pdgid);

// declare functions
double L(double x) { return x>120.0 ? 119.9999 : x; }

bool jet_mu_dr_compare( const boost::shared_ptr<const AnaMuon>& mu , boost::shared_ptr<const AnaJet> a , boost::shared_ptr<const AnaJet> b );
void jet_mu_overlap_removal( JETCOL& jetcoll , MUCOL& mucoll , const float& dr , std::vector< boost::shared_ptr<const AnaJet> >& output_col );
bool jet_tp_dr_compare( const boost::shared_ptr<const AnaParticle>& mu , boost::shared_ptr<const AnaJet> a , boost::shared_ptr<const AnaJet> b );
void jet_tp_overlap_removal( JETCOL& jetcoll , TPCOL& mucoll , const float& dr , std::vector< boost::shared_ptr<const AnaJet> >& output_col );
template< typename jetIterT > const bool jet_larhole_veto( jetIterT jet_begin , jetIterT jet_end );
const double jet_drmin( const boost::shared_ptr<const AnaJet>& jet , JETCOL& jetcoll );
void mu_eff_scale( MUCOL& mucoll , double *eff_weights,
		   double& eff_weight , double& eff_stat_error , double& eff_sys_error , int choice=0);
void mu_trig_scale( MUCOL& mucoll , double& trig_weight , double& trig_stat_error , int choice , int replica=-1 );
void mu_isol_scale( MUCOL& mucoll , double& trig_weight , double& trig_stat_error , int choice , int replica=-1 );

void study_wz(std::string label, bool do_ntuples, bool do_eff, int do_unf,
	      MUCOL& mu_type_col, MUCOL& mu_ntuple_col, MUCOL& mu_qual_col,
	      MUCOL& mu_pt_col, MUCOL& mu_eta_col, MUCOL& mu_iso_col,
	      VXCOL& vtx_base_col, VXCOL& vtx_col,
	      JETCOL& badjet_in_col, JETCOL& jet_in_col,
	      METCOL& met_col,
	      StudyWAsymmetry& st_w,
	      StudyZAsymmetry& st_z);

// Max-style search through the truth record (derived from AnalysisStore/TruthStore.cxx)
bool getBosonBornLeptons(const std::vector< boost::shared_ptr<const AnaTruthParticle> >& col, long lepPdgCode,
			 boost::shared_ptr<const AnaTruthParticle>& v,
			 boost::shared_ptr<const AnaTruthParticle>& mu,
			 boost::shared_ptr<const AnaTruthParticle>& nu,
			 bool is_z);

// declare macros
#define CUTFLOW(v,id,cut0,cut1) if(cut0) {				\
    dg::down(#v "cut",#v " cutflow");					\
    dg::filleffw( "u" #id "_passes_" #cut1 , cut1 , truth_weight[0]);	\
    if(is_mc) dg::filleffw( "v" #id "_passes_" #cut1 , cut1 , event_weight[0]); \
    if(is_mc) dg::filleffw( "w" #id "_passes_" #cut1 , cut1 , dg::global_weight() ); \
    dg::up();								\
  }									\
  do {} while(false)

// weights
const int NWPU = 2; // number of pileup variations
const int NWPT = 2; // number of wzpt variations
const int NWPO = 2; // number of w polarization variations (z unaffected)
const int NWDA = 1; // number of MC-to-data or other truth-level reweightings
const int NWEIGHTS = PDF_REWEIGHTING ? 1+NWPU+NWPT+NWPO+NWDA + NREPLICASP + (PDF_REWEIGHTING>1 ? 5 : 0) : 1+NWPU+NWPT+NWPO+NWDA;
const int pdf_offset = (PDF_REWEIGHTING==2) ? 5 : 0;
// nominal,  pu2,pu3,  wzpt2,wzpt3,  pdf[1..5],  pdf_member[53]
double event_weight[NWEIGHTS];
double truth_weight[NWEIGHTS];
double mc_weight = 1.0;
double pu_weight[] = { 1.0 , 1.0 , 1.0 , 1.0 , 1.0};
double vx_weight = 1.0;
// unitize, multiply or copy all weights
#define WS1(x) for(int iii=0; iii<NWEIGHTS; iii++) x[iii]=1.0
#define WSX(x,v) for(int iii=0; iii<NWEIGHTS; iii++) x[iii] *= v
#define WSE(tr,re) for(int iii=0; iii<NWEIGHTS; iii++) tr[iii] = re[iii]

// flags
bool is_mc = false;
bool is_data = false;
bool is_unfold = false;
bool is_wmunu = false;
bool is_wmunu_powheg_pythia = false;
bool is_wmunu_powheg_herwig = false;
bool is_wmunu_mcnlo = false;
bool is_wminmunu = false;
bool is_wplusmunu = false;
bool is_wgamma = false;
bool is_zmumu = false;
bool is_zmumu_mcnlo = false;
bool is_wtaunu = false;
bool is_ztautau = false;
bool is_alpgen = false;
bool is_pdfrw = false;
// event metadata
unsigned long evnum = 0;
unsigned long rnum = 0;        // for MC, this becomes the pseudo-run number from TPileupReweighting
//unsigned long rnum_DtoM = 0;   // for MC, this becomes the pseudo-run number from TPileupReweighting
//unsigned long rnum_DtoK = 0;   // for MC, this becomes the pseudo-run number from TPileupReweighting
//unsigned long rnum_LtoM = 0;   // for MC, this becomes the pseudo-run number from TPileupReweighting
unsigned long raw_rnum = 0;    // for MC, this stores the actual, original run number
double avgmu = 0;
double actmu = 0;
float truth_met_pt = -1.0;
// various globals
std::vector<double> int_lumi;
//std::vector<double> int_lumi_DtoM;
//std::vector<double> int_lumi_DtoK;
//std::vector<double> int_lumi_LtoM;
std::vector<std::string> run_periods;
DATARANGE::DataRange drange;
detector::MCP_TYPE mu_type = detector::MCP_STACO_COMBINED;
CompositeGenerator<const AnaParticle,const AnaW> wgen;
CompositeGenerator<const AnaParticle,const AnaZ> zgen;
// constants
const unsigned long pB[] = {177986,178109};
const unsigned long pD[] = {179710,180481};
const unsigned long pE[] = {180614,180776};
const unsigned long pF[] = {182013,182519};
const unsigned long pG[] = {182726,183462};
const unsigned long pH[] = {183544,184169};
const unsigned long pI[] = {185353,186493};
const unsigned long pJ[] = {186516,186755};
const unsigned long pK[] = {186873,187815}; // DtoK
const unsigned long pL[] = {188902,190343}; // 188921..190343
const unsigned long pM[] = {190503,191933}; // LtoM
const int STATUS_123_124 = 123124;
const int PDG_MU = 13;
const int PDG_NU = 14;
const int PDG_W = 24;
const int PDG_Z = 23;

// UNFOLDING [binning must be compatible with histograms in StudyWAsymmetry]
#include "TrigFTKAna/misc/UnfoldingHistogramTool.hpp"
#include "TH2D.h"
#include "TFile.h"
typedef struct {
  // VOLATILE PARTS
  // TTree variables
  Int_t Channel;
  Bool_t Truth_Is_Fiducial;
  Double_t Truth_Weight;  // only used for TTree
  std::vector<Double_t> Truth_Weight_Replicas; // used for UnfoldingHistogramTool
  Double_t Truth_PileUp_Weight;
  Double_t Truth_PtW;  // rebuilt from mu+nu
  Double_t Truth_MtW;  // rebuilt from mu+nu
  Double_t Truth_PtNu; // added for acceptance studies
  Double_t Truth_PtLep;
  Double_t Truth_EtaLep;
  Bool_t Reco_isReconstructed;
  Double_t Reco_Weight;  // only used for TTree
  std::vector<Double_t> Reco_Weight_Replicas; // used for UnfoldingHistogramTool
  Double_t Reco_PtW;
  Double_t Reco_PtLep;
  Double_t Reco_EtaLep;
  // 4vector pointers
  boost::shared_ptr<const AnaWCrossSectionCandidate> unftru;
  boost::shared_ptr<const AnaWCrossSectionCandidate> unfrec;
  // NON-VOLATILE PARTS
  // migration matrices (different kinematic variables & bins)
  std::vector<UnfoldingHistogramTool> response;
  // migration trees
  TTree *tree;
} UNFDATA; // one for each systematic
// list of keys [initialized later] - systematic variations
std::vector<std::string> unf_keys;     // all variations
std::vector<std::string> unf_keys_nom; // all variations within Nominal (e.g., scale factors)
std::vector<std::string> unf_keys_tru; // truth-level scale factors ( wpt, pdf, pu) within Nominal. Excludes pdf variations
std::vector<std::string> unf_keys_rec; // reco-level scale factors within Nominal
std::vector<std::string> unf_keys_rep; // stat. replicas scale factors within Nominal
std::vector<std::string> unf_keys_pdf; // pdf member sets within Nominal
// list of unfolding sets [POS,NEG] * [uint,abseta,eta,abseta-pt,eta-pt] - for each systematic variation
const int unf_nh = 4; // was:5. Now dropped eta-pt
const int unf_nq = 2;
double *EFFVEC = 0;
double *TRIVEC = 0;
double *ISOVEC = 0;
// unfolding data structure (indexed by unf_keys)
typedef std::map<std::string,UNFDATA>unfdata_type;
unfdata_type unfdata;

void create_response(const std::string& key , bool hastree, unsigned int nreplicas, TFile* fout) {
  static bool histos_inited = false;
  // update global histogram pointers
  if(!histos_inited) {
    histos_inited = true;
  }
  // Unfolding data and tree
  unfdata[key] = UNFDATA();
  TTree *t = 0;
  if(hastree) {
    t = new TTree( (std::string("physics_")+key).c_str() , "Unfolding tree" );
    unfdata[key].tree = t;
    t->Branch("Channel",&unfdata[key].Channel,"Channel/I");
    t->Branch("Truth_Is_Fiducial",&unfdata[key].Truth_Is_Fiducial,"Truth_Is_Fiducial/O");
    t->Branch("Truth_Mc_Weight",&unfdata[key].Truth_Weight,"Truth_Mc_Weight/D");
    if(!UNFOLD_TREE_REDUCE) {
      t->Branch("Truth_PileUp_Weight",&unfdata[key].Truth_PileUp_Weight,"Truth_PileUp_Weight/D");
      t->Branch("Truth_PtW",&unfdata[key].Truth_PtW,"Truth_PtW/D");
    }
    t->Branch("Truth_MtW",&unfdata[key].Truth_MtW,"Truth_MtW/D");
    t->Branch("Truth_PtNu",&unfdata[key].Truth_PtNu,"Truth_PtNu/D");
    t->Branch("Truth_PtLep",&unfdata[key].Truth_PtLep,"Truth_PtLep/D");
    t->Branch("Truth_EtaLep",&unfdata[key].Truth_EtaLep,"Truth_EtaLep/D");
    t->Branch("Reco_isReconstructed",&unfdata[key].Reco_isReconstructed,"Reco_isReconstructed/O");
    t->Branch("Reco_Weight",&unfdata[key].Reco_Weight,"Reco_Weight/D");
    if(!UNFOLD_TREE_REDUCE) {
      t->Branch("Reco_PtW",&unfdata[key].Reco_PtW,"Reco_PtW/D");
    }
    t->Branch("Reco_PtLep",&unfdata[key].Reco_PtLep,"Reco_PtLep/D");
    t->Branch("Reco_EtaLep",&unfdata[key].Reco_EtaLep,"Reco_EtaLep/D");
  }
  // vector of weights
  if(nreplicas>0) {
    unfdata[key].Truth_Weight_Replicas.resize(nreplicas);
    unfdata[key].Reco_Weight_Replicas.resize(nreplicas);
  }
  // Unfolding matrices
  unfdata[key].response = std::vector<UnfoldingHistogramTool>();
  std::vector<UnfoldingHistogramTool>& tmp = unfdata[key].response;
  // careful: the order is hard-coded
  for(int iq=0;iq<unf_nq;iq++) {
    std::string sdir = iq==0 ? "POS/" : (iq==1 ? "NEG/" : "ALL/");
    int itot=0;
    // integrated vs abseta
    tmp.push_back(  UnfoldingHistogramTool(fout,sdir+key+"/uint",dg::bin().D_uintabseta.size()-1,&(dg::bin().D_uintabseta[0]) , nreplicas )  ); itot++;
    // abseta and eta
    tmp.push_back(  UnfoldingHistogramTool(fout,sdir+key+"/abseta",dg::bin().D_abseta.size()-1,&(dg::bin().D_abseta[0]) , nreplicas )  ); itot++;
    tmp.push_back(  UnfoldingHistogramTool(fout,sdir+key+"/eta",dg::bin().D_eta.size()-1,&(dg::bin().D_eta[0]),nreplicas ) ); itot++;
    // abseta and eta vs pT
    tmp.push_back(  UnfoldingHistogramTool(fout,sdir+key+"/abseta_lpt",
					   dg::bin().D_abseta.size()-1,&(dg::bin().D_abseta[0]),
					   dg::bin().D_lpt.size()-1,&(dg::bin().D_lpt[0]),
					   true , nreplicas )); itot++;
    if(false){
      tmp.push_back(  UnfoldingHistogramTool(fout,sdir+key+"/eta_lpt",
					     dg::bin().D_eta.size()-1,&(dg::bin().D_eta[0]),
					     dg::bin().D_lpt.size()-1,&(dg::bin().D_lpt[0]),
					     true , nreplicas )); itot++;
    }
    assert(unf_nh==itot);
  }
}

void UNF_RESET() {
  for(unfdata_type::iterator it = unfdata.begin(); it!=unfdata.end(); it++) { // loop over systematics
    UNFDATA &d = it->second;
    d.Channel = 2;
    d.Truth_Is_Fiducial = false;
    d.Truth_Weight=-999;
    std::fill( d.Truth_Weight_Replicas.begin(), d.Truth_Weight_Replicas.end(), -999 );
    d.Truth_PileUp_Weight=-999;
    d.Truth_PtW=-999;
    d.Truth_MtW=-999;
    d.Truth_PtNu=-999;
    d.Truth_PtLep=-999;
    d.Truth_EtaLep=-999;
    d.Reco_isReconstructed = false;
    d.Reco_Weight = -999;
    std::fill( d.Reco_Weight_Replicas.begin(), d.Reco_Weight_Replicas.end(), -999 );
    d.Reco_PtW = -999;
    d.Reco_PtLep = -999;
    d.Reco_EtaLep = -999;
    d.unftru.reset();
    d.unfrec.reset();
  }
}

void UNF_MISS() {
  if(!is_unfold) return;
  // this function is called if RECO has not been found
  // NOTE ON ACCEPTANCE:
  // In principle, if TRUTH hasn't been found either, we could skip the event altogether
  // BUT: if we keep it, we will preserve the ability to also calculate the acceptance!
  const double w = dg::global_weight();
  for(unfdata_type::iterator it = unfdata.begin(); it!=unfdata.end(); it++) { // loop over systematics
    UNFDATA &d = it->second;
    if(UNFOLD_TREE && d.tree) d.tree->Fill();
    if(d.Reco_isReconstructed || d.Truth_Is_Fiducial) {
      boost::shared_ptr<const AnaWCrossSectionCandidate>& unftru = d.unftru;
      const int q = unftru->scharge()>0 ? 0 : 1;
//       for(unsigned int replica=0; replica<d.Reco_Weight_Replicas.size(); replica++) {
// 	assert( std::abs(-999.0 - d.Truth_Weight_Replicas[replica]) > 0.000001 );
// 	if(d.Reco_isReconstructed) assert( std::abs(-999.0 - d.Reco_Weight_Replicas[replica]) > 0.000001 );
//       }
      const unsigned int nreplicas = d.Reco_Weight_Replicas.size();
      if(nreplicas>0) {
	d.response[ q*unf_nh + 0 ].fill(d.Reco_isReconstructed,
					d.Reco_EtaLep==-999 ? d.Reco_EtaLep : (std::abs(d.Reco_EtaLep)<2.4 ? 0.5 : 1.1),
					d.Reco_Weight_Replicas,
					d.Truth_Is_Fiducial,
					d.Truth_EtaLep==-999 ? d.Truth_EtaLep : (std::abs(d.Truth_EtaLep)<2.4 ? 0.5 : 1.1),
					d.Truth_Weight_Replicas,
					nreplicas);
	d.response[ q*unf_nh + 1 ].fill(d.Reco_isReconstructed,
					d.Reco_EtaLep==-999 ? d.Reco_EtaLep : std::abs(d.Reco_EtaLep),
					d.Reco_Weight_Replicas,
					d.Truth_Is_Fiducial,
					d.Truth_EtaLep==-999 ? d.Truth_EtaLep : std::abs(d.Truth_EtaLep),
					d.Truth_Weight_Replicas,
					nreplicas);
	d.response[ q*unf_nh + 2 ].fill(d.Reco_isReconstructed,
					d.Reco_EtaLep,
					d.Reco_Weight_Replicas,
					d.Truth_Is_Fiducial,
					d.Truth_EtaLep,
					d.Truth_Weight_Replicas,
					nreplicas);
	d.response[ q*unf_nh + 3 ].fill(d.Reco_isReconstructed,
					d.Reco_EtaLep==-999 ? d.Reco_EtaLep : std::abs(d.Reco_EtaLep),
					d.Reco_PtLep,
					d.Reco_Weight_Replicas,
					d.Truth_Is_Fiducial,
					d.Truth_EtaLep==-999 ? d.Truth_EtaLep : std::abs(d.Truth_EtaLep),
					d.Truth_PtLep,
					d.Truth_Weight_Replicas,
					nreplicas);
	if(false) {
	  d.response[ q*unf_nh + 4 ].fill(d.Reco_isReconstructed,
					  d.Reco_EtaLep,
					  d.Reco_PtLep,
					  d.Reco_Weight_Replicas,
					  d.Truth_Is_Fiducial,
					  d.Truth_EtaLep,
					  d.Truth_PtLep,
					  d.Truth_Weight_Replicas,
					  nreplicas);
	}
      }
    }
  } // end loop over systematics
  return;
}

void UNF_FILL(const std::string& key ) {
  if(!is_unfold) return;
  unfdata_type::iterator it = unfdata.find(key);
  if( it == unfdata.end() ) {
    std::cerr << "Missing declaration for the unfolding block: " << key << std::endl;
  }
  assert( it != unfdata.end() ); //sanity check
  UNFDATA &d = it->second;
  if(UNFOLD_TREE && d.tree) d.tree->Fill();
  if(d.Reco_isReconstructed || d.Truth_Is_Fiducial) {
    boost::shared_ptr<const AnaWCrossSectionCandidate>& unfrec = d.unfrec;
    boost::shared_ptr<const AnaWCrossSectionCandidate>& unftru = d.unftru;
    // charge determination: prefer from truth, but use reco for fakes
    const int q = unftru ? (unftru->scharge()>0 ? 0 : 1) : (unfrec->scharge()>0 ? 0 : 1);
//     for(unsigned int replica=0; replica<d.Reco_Weight_Replicas.size(); replica++) {
//       assert( std::abs(-999.0 - d.Truth_Weight_Replicas[replica]) > 0.000001 );
//       if(d.Reco_isReconstructed) assert( std::abs(-999.0 - d.Reco_Weight_Replicas[replica]) > 0.000001 );
//     }
    const unsigned int nreplicas = d.Reco_Weight_Replicas.size();
    if(nreplicas>0) {
      d.response[ q*unf_nh + 0 ].fill(d.Reco_isReconstructed,
				      d.Reco_EtaLep==-999 ? d.Reco_EtaLep : (std::abs(d.Reco_EtaLep)<2.4 ? 0.5 : 1.1),
				      d.Reco_Weight_Replicas,
				      d.Truth_Is_Fiducial,
				      d.Truth_EtaLep==-999 ? d.Truth_EtaLep : (std::abs(d.Truth_EtaLep)<2.4 ? 0.5 : 1.1),
				      d.Truth_Weight_Replicas,
				      nreplicas);
      d.response[ q*unf_nh + 1 ].fill(d.Reco_isReconstructed,
				      d.Reco_EtaLep==-999 ? d.Reco_EtaLep : std::abs(d.Reco_EtaLep),
				      d.Reco_Weight_Replicas,
				      d.Truth_Is_Fiducial,
				      d.Truth_EtaLep==-999 ? d.Truth_EtaLep : std::abs(d.Truth_EtaLep),
				      d.Truth_Weight_Replicas,
				      nreplicas);
      d.response[ q*unf_nh + 2 ].fill(d.Reco_isReconstructed,
				      d.Reco_EtaLep,
				      d.Reco_Weight_Replicas,
				      d.Truth_Is_Fiducial,
				      d.Truth_EtaLep,
				      d.Truth_Weight_Replicas,
				      nreplicas);
      d.response[ q*unf_nh + 3 ].fill(d.Reco_isReconstructed,
				      d.Reco_EtaLep==-999 ? d.Reco_EtaLep : std::abs(d.Reco_EtaLep),
				      d.Reco_PtLep,
				      d.Reco_Weight_Replicas,
				      d.Truth_Is_Fiducial,
				      d.Truth_EtaLep==-999 ? d.Truth_EtaLep : std::abs(d.Truth_EtaLep),
				      d.Truth_PtLep,
				      d.Truth_Weight_Replicas,
				      nreplicas);
      if(false) {
	d.response[ q*unf_nh + 4 ].fill(d.Reco_isReconstructed,
					d.Reco_EtaLep,
					d.Reco_PtLep,
					d.Reco_Weight_Replicas,
					d.Truth_Is_Fiducial,
					d.Truth_EtaLep,
					d.Truth_PtLep,
					d.Truth_Weight_Replicas,
					nreplicas);
      }
    }
  }
  return;
}

////////////////////////////////////////////////////////////////

int main( int argc , char* argv[] )
{

  AnaConfiguration::configure("ana_wasym",argc,argv);
  if( !AnaConfiguration::configured() ) { exit(0); }
  TH1::AddDirectory(kTRUE);

  // determine athena release version
  const int REL = AnaConfiguration::release();
  assert(REL==17);
  const std::string PROSTRING = "pro10";
  const CONF::ConfType conf = AnaConfiguration::conf();
  drange = AnaConfiguration::data_range();
  const std::string DRANGESTRING = DATARANGE::datarange_string(AnaConfiguration::data_range());
  std::cout << "Athena release = " << REL << std::endl;
  std::cout << "Data Range = " << AnaConfiguration::data_range() << std::endl;
  std::cout << "Conference code = " << conf << std::endl;
  // set muon pt cut
  double MU_PT_CUT = AnaConfiguration::pt_cut();
  assert(MU_PT_CUT>19.99 && MU_PT_CUT < 25.01);
  std::cout << "Minimum PT cut (only applies to W selection; Z is fixed at 20) is: " << MU_PT_CUT << std::endl;
  // see if we are running wmunu/zmumu or some other special sample [hardcoded!]
  const std::string sample_name = AnaConfiguration::sample_name();
  std::cout << "Sample name = " << sample_name << std::endl;
  is_data = boost::algorithm::icontains(sample_name,"data_period");
  is_unfold = boost::algorithm::icontains(sample_name,"_wminmunu") || boost::algorithm::icontains(sample_name,"_wplusmunu") || boost::algorithm::icontains(sample_name,"max_wplus") || boost::algorithm::icontains(sample_name,"max_wminus");
  if(is_unfold) {
    std::cout << "Detected MC that needs to be unfolded" << std::endl;
  }
  is_wmunu = boost::algorithm::icontains(sample_name,"wmu") || boost::algorithm::icontains(sample_name,"wminmunu") || boost::algorithm::icontains(sample_name,"wplusmunu") || boost::algorithm::icontains(sample_name,"max_wplus") || boost::algorithm::icontains(sample_name,"max_wminus");
  is_wmunu_powheg_pythia = boost::algorithm::icontains(sample_name,"powheg_pythia_wminmunu") || boost::algorithm::icontains(sample_name,"powheg_pythia_wplusmunu") || boost::algorithm::icontains(sample_name,"max_wplus") || boost::algorithm::icontains(sample_name,"max_wminus");
  is_wmunu_powheg_herwig = boost::algorithm::icontains(sample_name,"powheg_herwig_wminmunu") || boost::algorithm::icontains(sample_name,"powheg_herwig_wplusmunu");
  is_wmunu_mcnlo = boost::algorithm::icontains(sample_name,"mcnlo_wminmunu") || boost::algorithm::icontains(sample_name,"mcnlo_wplusmunu");
  is_wminmunu = boost::algorithm::icontains(sample_name,"wminmunu");
  is_wplusmunu = boost::algorithm::icontains(sample_name,"wplusmunu");
  if(is_wmunu) {
    std::cout << "Detected wmunu MC";
    if(is_wmunu_powheg_pythia) std::cout << " (PowhegPythia)";
    if(is_wmunu_powheg_herwig) std::cout << " (PowhegHerwig)";
    if(is_wmunu_mcnlo) std::cout << " (McAtNlo)";
    std::cout << std::endl;
  }
  is_wgamma = boost::algorithm::icontains(sample_name,"wgamma");
  if(is_wgamma) {
    std::cout << "Detected wgamma MC" << std::endl;
  }
  is_wtaunu = boost::algorithm::icontains(sample_name,"wtau") || boost::algorithm::icontains(sample_name,"wmintaunu") || boost::algorithm::icontains(sample_name,"wplustaunu");
  if(is_wtaunu) {
    std::cout << "Detected wtaunu MC" << std::endl;
  }
  is_zmumu = boost::algorithm::icontains(sample_name,"zmu");
  if(is_zmumu) {
    std::cout << "Detected zmumu MC" << std::endl;
  }
  is_zmumu_mcnlo = boost::algorithm::icontains(sample_name,"mc_mcnlo_zmumu");
  if(is_zmumu_mcnlo) {
    std::cout << "Detected zmumu MC@NLO MC to be used with LineShapeFix_Z_McAtNlo" << std::endl;
  }
  is_ztautau = boost::algorithm::icontains(sample_name,"ztau");
  if(is_ztautau) {
    std::cout << "Detected ztautau MC" << std::endl;
  }
  is_alpgen = boost::algorithm::icontains(sample_name,"alpgen");
  if(is_alpgen) {
    std::cout << "Detected alpgen MC" << std::endl;
  }
  // PDF reweighting should only be done on NLO samples
  const bool is_pythia_lo = boost::algorithm::icontains(sample_name,"mc_pythia_");
  if(is_pythia_lo) {
    std::cout << "Detected LO pythia MC" << std::endl;
  }
  is_pdfrw = (is_wmunu || is_wtaunu || is_zmumu || is_ztautau) && (!is_alpgen) && (!is_pythia_lo);
  if(is_pdfrw) {
    std::cout << "Detected a sample that can be PDF-reweighted" << std::endl;
  }

  // experimental: enable pythia_to_herwig reweighting
  bool PYTHIA_TO_HERWIG = boost::algorithm::icontains(sample_name,"powheg_pythia_wminmunu") && boost::algorithm::icontains(sample_name,"toherwig");
  if(PYTHIA_TO_HERWIG) {
    std::cout << "ATTENTION: will attempt to reweight powheg_pythia_wminmunu to powheg_herwig_wminmunu" << std::endl;
  }
  
  const bool do_pileup = AnaConfiguration::apply_pileup_weights();
  NOMONLY = AnaConfiguration::skip_systematics();
  int save_ntuples = AnaConfiguration::save_ntuples();

  // enable AnaMuon replicas if running over wmunu MC
  NREPLICASF = is_wmunu_powheg_pythia ? AnaConfiguration::replicas() : 1; // should be 0, but may have a bug below
  std::cout << "SF replicas = " << NREPLICASF << std::endl;
  AnaMuon::NREPLICASF = NREPLICASF;
  if(true) {
    EFFVEC = new double[NREPLICASF];
    TRIVEC = new double[NREPLICASF];
    ISOVEC = new double[NREPLICASF];
  }

  // bootstrap histograms. for now, only apply to data
  if(!is_data) RUN_BOOTSTRAP = false;
  if(RUN_BOOTSTRAP) dg::initialize_bootstrap("boot",NREPLICASB);

  // UNFOLDING (order of definitions matters!)
#define UNA(x) unf_keys.push_back(x)
#define UNT(x) unf_keys.push_back(x); unf_keys_nom.push_back(x); unf_keys_tru.push_back(x)
#define UNR(x) unf_keys.push_back(x); unf_keys_nom.push_back(x); unf_keys_rec.push_back(x)
#define UNREP(x) unf_keys.push_back(x); unf_keys_nom.push_back(x); unf_keys_rep.push_back(x)
#define UNPDF(x) unf_keys.push_back(x); unf_keys_nom.push_back(x); unf_keys_pdf.push_back(x)
  UNA("Nominal");
  if(!NOMONLY) {
    UNA("Rawmet");
    // pileup reweighting variations
    UNT("PileupDown");
    UNT("PileupUp");
    // wzpt reweighting variations
    UNT("WptSherpa");
    UNT("WptPythia8");
    // w polarization reweighting variations
    UNT("WpolPowhegHerwig"); // only mcnlo is reweighted
    UNT("WpolPowhegPythia"); // both mcnlo and powhegHerwig reweighted
    // generator-based pT reweighting to equalize reco-level distributions
    UNT("MCModeling"); // PowhegPythia_to_Data
    // other PDFs
    if(PDF_REWEIGHTING) {
      // pdf families
      if(PDF_REWEIGHTING>1) {
	UNT("PdfCT10");
	UNT("PdfMSTW");
	UNT("PdfHERA");
	UNT("PdfNNPDF");
	UNT("PdfABM");
      }
      // members inside CT10
      UNPDF("PDFMEM");
    }
    // scale factors - replicas
    UNREP("SFUncorrReco");
    UNREP("SFUncorrTrig");
    UNREP("SFUncorrIsol");
    if(false) { // combined toy, deprecated May 13 2013
      UNREP("SFUncorr");
    }
    // scale factors
    // -reco
    UNR("MuonRecoSFUp");
    UNR("MuonRecoSFDown");
    UNR("MuonRecoSFPhi"); // CaloTag-probe systematic. OR: experimental MCP reco eta x phi corrections
    UNR("MuonRecoStatSFUp"); //[3]
    UNR("MuonRecoStatSFDown");
    // -trig
    UNR("MuonTriggerSFPhi"); // additional corrections on top (systematic)
    UNR("MuonTriggerStatSFUp");
    UNR("MuonTriggerStatSFDown"); //[7]
    //UNR("MuonTriggerRecoSFPhi"); // both reco and trigger phi corrections
    // -iso
    UNR("MuonIsoSFUp");
    UNR("MuonIsoSFDown"); //[9]
    // advanced met
    UNA("ResoSoftTermsUp_ptHard");
    UNA("ResoSoftTermsDown_ptHard");
    UNA("ResoSoftTermsUpDown_ptHard");
    UNA("ResoSoftTermsDownUp_ptHard");
    UNA("ScaleSoftTermsUp_ptHard");
    UNA("ScaleSoftTermsDown_ptHard");
    // simplified met
    if(false) {
      UNA("ResoSoftTermsUp");
      UNA("ResoSoftTermsDown");
      UNA("ScaleSoftTermsUp");
      UNA("ScaleSoftTermsDown");
    }
    // jes/jer
    UNA("JetResol");
    UNA("JetResolUp");
    UNA("JetResolDown");
    UNA("JetScaleUp");
    UNA("JetScaleDown");
    UNA("JetNPVUp");
    UNA("JetNPVDown");
    UNA("JetMUUp");
    UNA("JetMUDown");
    if(STUDY_CALJET) {
      UNA("NominalCalJet");
      UNA("JetResolCalJet");
      UNA("JetResolUpCalJet");
      UNA("JetResolDownCalJet");
      UNA("JetScaleUpCalJet");
      UNA("JetScaleDownCalJet");
      UNA("JetNPVUpCalJet");
      UNA("JetNPVDownCalJet");
      UNA("JetMUUpCalJet");
      UNA("JetMUDownCalJet");
    }
    // mcp
    UNA("MuonResMSUp");
    UNA("MuonResMSDown");
    UNA("MuonResIDUp");
    UNA("MuonResIDDown");
    UNA("MuonNoScale");
    UNA("MuonKScaleUp");
    UNA("MuonKScaleDown");
    UNA("MuonCScaleUp");
    UNA("MuonCScaleDown");
  }
#undef UNA
#undef UNT
#undef UNR
#undef UNREP
#undef UNPDF

  // Output file and TTree's
  TFile *funfold = 0;
  if(true) {
    std::string unf_outpath = DataGraphics::Tools::expand_filename_to_valid( AnaConfiguration::full_output_path() + ".unfold.root" );
    std::cout << "Unfolding output: " << unf_outpath << std::endl;
    funfold = new TFile(unf_outpath.c_str(),"RECREATE");
    funfold->cd();
  }
  // response matrices
  if(true) {
    Bool_t oldstat= TH1::AddDirectoryStatus();
    TH1::AddDirectory (kFALSE);
    for(std::vector<std::string>::const_iterator it=unf_keys.begin(); it!=unf_keys.end(); it++) {
      std::cout << "Creating response histograms for: " << *it << std::endl;
      bool isreplicas = false;
      bool ispdfs = false;
      for(unsigned int i=0; i<unf_keys_rep.size(); i++) {
	if(*it == unf_keys_rep[i]) {
	  isreplicas=true;
	  std::cout << "     -> with " << NREPLICASF << " replicas" << std::endl;
	  break;
	}
      }
      for(unsigned int i=0; i<unf_keys_pdf.size(); i++) {
	if(*it == unf_keys_pdf[i]) {
	  ispdfs=true;
	  std::cout << "     -> with " << NREPLICASP << " pdf members" << std::endl;
	  break;
	}
      }
      if(ispdfs) {
	create_response(*it , true, NREPLICASP, funfold);
      }
      else if(isreplicas) {
	create_response(*it , true, NREPLICASF, funfold);
      } else {
	create_response(*it , true, 1, funfold);
      }
    }
    TH1::AddDirectory (oldstat);
  }

  // PILEUP MC CONFIGURATION
  std::string pileup_config_file =  "";
  if(!AnaConfiguration::pileup_config_map().empty()) {
    std::cout << "WARNING: adding custom pileup configuration file: " << AnaConfiguration::pileup_config_map() << std::endl;    
    pileup_config_file = AnaConfiguration::pileup_config_map();
  }

  // PILEUP DISTRIBUTION HISTOGRAMS IN DATA
  std::string pileup_data_file =  "./pileup_histos/asym_data11_7TeV."+PROSTRING+"."+DRANGESTRING+".root";
  const std::string pileup_data_fileDtoK =  "./pileup_histos/asym_data11_7TeV."+PROSTRING+"."+"DtoK"+".root";
  const std::string pileup_data_fileLtoM =  "./pileup_histos/asym_data11_7TeV."+PROSTRING+"."+"LtoM"+".root";
  if(!AnaConfiguration::pileup_weights_map().empty()) {
    std::cout << "WARNING: adding custom mu distributions file: " << AnaConfiguration::pileup_weights_map() << std::endl;    
    pileup_data_file = AnaConfiguration::pileup_weights_map();
  }

  // GOOD RUN LIST
  Root::TGoodRunsListReader grlR;
  Root::TGoodRunsList grl;
  std::string grlname("UNDEFINED");
  if(AnaConfiguration::grl_xml_files().empty()) {
    grlname = "./good_run_lists/asym_data11_7TeV."+PROSTRING+"."+DRANGESTRING+".xml";
    std::cout << "INFO: adding default GRL xml map: " << grlname << std::endl;
  } else {
    grlname = AnaConfiguration::grl_xml_files();
    std::cout << "WARNING: adding custom GRL xml map: " << grlname << std::endl;
  }
  grlR.SetXMLFile(grlname.c_str());
  if(!grlR.Interpret()) {
    assert(0&&"Failed to add a GRL file");
  }
  grl = grlR.GetMergedGoodRunsList();
  //grl.Summary(false);
  
  // define lists of analysis objects for each event. insert each
  // object collection into this list. each EventObjectColl will
  // automatically get cleared at the beginning of each event.

  // TRUTH
  TPCOL truth_all_col;
  TPCUT truth_all_cut("truth_all");
  truth_all_cut.add_u("pdg_pre",bind(&AnaTruthParticle::is_lepton_or_heavy_boson,_1));
  TPCUT truth_stable_cut("truth_stable");
  truth_stable_cut.add_u("stable",bind(&AnaTruthParticle::status,_1) == 1);
  // truth muons
  TPCOL truth_mu_col;
  TPCUT truth_mu_cut("truth_mu");
  truth_mu_cut.add_uu("pdg",bind(dgSTL::absI,bind(&AnaTruthParticle::pdg_id,_1)) == 13, bind(&dg::fillh,_1,100,-50,50,bind(&AnaTruthParticle::pdg_id,_2)) );
  truth_mu_cut.add_uu("pt",bind(&AnaTruthParticle::pt,_1)>15.0, bind(&dg::fillh,_1,200,0,200,bind(&AnaTruthParticle::pt,_2)) );
  truth_mu_cut.add_uu("eta",bind(dgSTL::absD,bind(&AnaTruthParticle::eta,_1))<=2.5, bind(&dg::fillh,_1,100,0,3.0,bind(&AnaTruthParticle::eta,_2)) );
  // truth w's (from MC truth record)
  TPCOL truth_w_col;
  // truth w's (from truth-met and truth muon)
  WCOL truth_anaw_reco_all_col;
  WXCOL truth_w_reco_all_col;
  WXCOL truth_w_reco_col;
  WXCUT truth_w_reco_cut( "truth_w_reco" );
  truth_w_reco_cut.add_u( "met" , bind(&AnaWCrossSectionCandidate::met_pt,_1)>25 );
  truth_w_reco_cut.add_u( "mt" , bind(&AnaWCrossSectionCandidate::mt,_1)>40 );
  // truth z's (from MC truth record)
  TPCOL truth_z_col;
  // truth z's (inferred from two truth muons)
  ZCOL  truth_anaz_reco_all_col;
  ZXCOL truth_z_reco_all_col;
  ZXCOL truth_z_reco_col;
  ZXCUT truth_z_reco_cut("truth_z_reco");
  truth_z_reco_cut.add_u( "opsign" , bind(&AnaZCrossSectionCandidate::opposite_leptons,_1)==true );
  truth_z_reco_cut.add_u( "m" , bind(&AnaZCrossSectionCandidate::asym_zwindow,_1)==true );
  // truth met (actual cut applied in truth-w)
  METCOL truth_met_col;
  METCUT truth_met_cut("truth_met");
  truth_met_cut.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::TRUTH);
  // truth neutrinos [alternative to truth-MET]
  TPCOL truth_nu_col;
  TPCUT truth_nu_cut("truth_nu");
  truth_nu_cut.add_uu("pdg",bind(dgSTL::absI,bind(&AnaTruthParticle::pdg_id,_1)) == 14, bind(&dg::fillh,_1,100,-50,50,bind(&AnaTruthParticle::pdg_id,_2)) );
  truth_nu_cut.add_uu("pt",bind(&AnaTruthParticle::pt,_1)>25.0, bind(&dg::fillh,_1,200,0,200,bind(&AnaTruthParticle::pt,_2)) ); // apply full MET selection
  // truth jets - used to build w candidates
  JETCOL truth_jet_all_col;
  JETCUT truth_jet_cut("truth_jet");
  truth_jet_cut.add_uu( "pt" , bind(&AnaJet::pt,_1) > 30. , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );
  truth_jet_cut.add_uu( "rapidity", bind(dgSTL::absD,bind(&AnaJet::rapidity,_1)) < 4.4 ,
			bind(&dg::fillh,_1,100,-4.5,4.5,bind(&AnaJet::rapidity,_2)) );

  // VERTEX
  VXCOL vtx_base_col;
  VXCUT vtx_base_cut("vtx_base");
  vtx_base_cut.add_uu( "good", bind(&AnaVertex::type,_1) == ATLAS::PriVtx || bind(&AnaVertex::type,_1) == ATLAS::PileUp,
		       bind( &dg::filleff , _1 , _2 ) );
  // analysis vertices
  VXCOL vtx_col;
  VXCUT vtx_cut("vtx");
  vtx_cut.add_uu( "ntracks", bind(&AnaVertex::ntracks,_1)>=3,bind( &dg::fillh , _1 , 15 , 0 , 15 , bind(&AnaVertex::ntracks,_2)) );
  if(false) { // disabled as of Feb 2012 per W/Z recommendations
    vtx_cut.add_uu( "z", bind(dgSTL::absD, bind(&AnaVertex::z,_1))<=200.0,bind( &dg::fillh , _1 , 100 , -300 , 300 , bind(&AnaVertex::z,_2)) );
  }
  // vertices for jet calibration and JES uncertainty (>=2 tracks)
  VXCOL vtx_calib_col;
  VXCUT vtx_calib_cut( "vtx_calib" );
  vtx_calib_cut.add_u( "ntracks",bind(&AnaVertex::ntracks,_1)>=2 );
  // METUtil requirement on having a good primary vertex
  VXCOL vtx_softmet_good_col;
  VXCUT vtx_softmet_good_cut("vtx_softmet_good");
  vtx_softmet_good_cut.add_u( "primary", bind(&AnaVertex::type,_1) == ATLAS::PriVtx );
  vtx_softmet_good_cut.add_u( "ntracks",bind(&AnaVertex::ntracks,_1)>2 );
  vtx_softmet_good_cut.add_u( "z", bind(dgSTL::absD, bind(&AnaVertex::z,_1))<=200.0 );
  // vertices for SoftTerms MET corrections (>=3 tracks)
  VXCOL vtx_softmet_col;
  VXCUT vtx_softmet_cut( "vtx_softmet" );
  vtx_softmet_cut.add_u( "ntracks",bind(&AnaVertex::ntracks,_1)>2 );

  // MUONS
  // Choose muon family (aka class)
  MUCOL mu_class_col;
  MUCUT mu_class_cut("mu_class");
  switch( AnaConfiguration::muon_type() ) {
  case 0:
  case 1:
  case 2:
    mu_class_cut.add_u( "mu_class" ,bind(&AnaMuon::aod_class,_1)==AnaMuon::AOD_STACO );
    break;
  case 100:
  case 101:
  case 102:
    mu_class_cut.add_u( "mu_class" ,bind(&AnaMuon::aod_class,_1)==AnaMuon::AOD_MUID );
   break;
  default:
    assert(0&&"Unknown muon-type");
  }
  // cutflow stages
#define MU(x) MUCOL mu_ ##x## _mcp_col; MUCOL mu_ ##x## _type_col; MUCOL mu_ ##x## _ntuple_col; MUCOL mu_ ##x## _qual_col; \
  MUCOL mu_ ##x## _pt_col; MUCOL mu_ ##x## _eta_col; MUCOL mu_ ##x## _iso_col; MUCOL mu_ ##x## _isoloose_col; \
  MUCOL mu_ ##x## _qcd40_col; MUCOL mu_ ##x## _qcd20_col; MUCOL mu_ ##x## _qcdwind20_col; MUCOL mu_ ##x## _qcdwind40_col; \
  MUCOL mu_ ##x## _qcdwind20m_col; MUCOL mu_ ##x## _qcdd0_col;
  MU(raw);
  MU(datascaled);
  MU(smeared);
  MU(unscaled);
  MU(msup);
  MU(msdown);
  MU(idup);
  MU(iddown);
  MU(kup);
  MU(kdown);
  MU(cup);
  MU(cdown);
#undef MU
  // mu_type is shared among all systematics
  MUCUT mu_type_cut("mu_type");
  switch( AnaConfiguration::muon_type() ) {
  case 0:
    // STACO combined only
    mu_type = detector::MCP_STACO_COMBINED;
    mu_type_cut.add_uu( "primary_author" ,bind(&AnaMuon::primary_author,_1)==MuonPrimAuthor::STACO,bind( &dg::filleff , _1 , _2 ) );
    mu_type_cut.add_uu( "type" , bind(&AnaMuon::is_staco_combined,_1) == true , bind( &dg::filleff , _1 , _2 ) );
    break;
  case 1:
    // STACO loose
    mu_type = detector::MCP_STACO_LOOSE;
    mu_type_cut.add_uu( "type" ,
			((bind(&AnaMuon::primary_author,_1)==MuonPrimAuthor::STACO) && (bind(&AnaMuon::is_staco_combined,_1) == true)) ||
			((bind(&AnaMuon::primary_author,_1)==MuonPrimAuthor::MuTag) && (bind(&AnaMuon::quality_loose,_1)==true)),
			bind( &dg::filleff , _1 , _2 ) );
    break;
  case 2:
    // STACO tight
    mu_type = detector::MCP_STACO_TIGHT;
    mu_type_cut.add_uu( "type" ,
			((bind(&AnaMuon::primary_author,_1)==MuonPrimAuthor::STACO) && (bind(&AnaMuon::is_staco_combined,_1) == true)) ||
			((bind(&AnaMuon::primary_author,_1)==MuonPrimAuthor::MuTag) && (bind(&AnaMuon::quality_tight,_1)==true)),
			bind( &dg::filleff , _1 , _2 ) );
    break;
  case 100:
    // Muid combined only
    mu_type = detector::MCP_MUID_COMBINED;
    mu_type_cut.add_uu( "primary_author" ,bind(&AnaMuon::primary_author,_1)==MuonPrimAuthor::MuidCo,bind( &dg::filleff , _1 , _2 ) );
    mu_type_cut.add_uu( "type" , bind(&AnaMuon::is_muid_combined,_1) == true , bind( &dg::filleff , _1 , _2 ) );
   break;
  default:
    assert(0&&"Unknown muon-type");
  }
  // A complete sequence of cuts per W/Z common recommendations
  MUCUT mu_qual_cut("mu_qual");
  if(D0CUT) { // Max variation including a d0 cut
    mu_qual_cut.add_uu( "d0sig" , bind(dgSTL::absD,bind( &AnaMuon::exPV_d0_signif , _1 )) < 3.5 ,
			bind( &dg::fillh ,_1,200,-5,5,bind( &AnaMuon::exPV_d0_signif,_2)));
  }
  if(false) { // d0 and MSID cuts are disabled as of Feb 2012
    mu_qual_cut.add_uu( "d0sig" , bind(dgSTL::absD,bind( &AnaMuon::exPV_d0_signif , _1 )) < 10. ,
			bind( &dg::fillh ,_1,200,-5,5,bind( &AnaMuon::exPV_d0_signif,_2)));
    mu_qual_cut.add_uu( "pt_MSID" , (bind(&AnaMuon::primary_author,_1)==MuonPrimAuthor::MuTag) || (bind(&AnaMuon::pt_diff_ratio,_1) < 0.5),
			bind( &dg::fillh , _1 , 200 , -2 , 2 , bind( &AnaMuon::pt_diff_ratio , _2 )));
  }
  mu_qual_cut.add_uu( "idhits" , bind(&AnaMuon::passes_hits_mcp,_1,conf)==true, bind(&dg::fillh,_1,50,0,50,bind(&AnaMuon::nhits_silicon,_2)) );
  MUCUT mu_pt_cut("mu_pt");
  mu_pt_cut.add_uu( "pt" , bind(&AnaMuon::pt,_1) > MU_PT_CUT , bind(&dg::fillh,_1,200,0,200,bind(&AnaMuon::pt,_2)) );
  MUCUT mu_eta_cut("mu_eta");
  mu_eta_cut.add_uu( "eta2p4" , bind(dgSTL::absD, bind(&AnaMuon::eta,_1)) <= 2.4, bind(&dg::fillh,_1,200,-2.5,2.5,bind(&AnaMuon::eta,_2)) );
  MUCUT mu_iso_cut("mu_iso");
  mu_iso_cut.add_uu( "ptcone40" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE40) < 0.1 , bind(&dg::fillh,_1,100,0,1,bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE40)) );
  // qcd
  MUCUT mu_qcd40_cut("mu_qcd40");
  mu_qcd40_cut.add_u( "ptcone40" , !(bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE40) < 0.1 ) );
  MUCUT mu_qcd20_cut("mu_qcd20");
  mu_qcd20_cut.add_u( "ptcone20r" , !(bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 ) );
  MUCUT mu_qcdwind20_cut("mu_qcdwind20");
  mu_qcdwind20_cut.add_u( "ptcone20r" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) >= 0.1 && bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) <= 0.2 );
  MUCUT mu_qcdwind20m_cut("mu_qcdwind20m");
  mu_qcdwind20m_cut.add_u( "ptcone20mr" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) >= 0.12 && bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) <= 0.25 );
  MUCUT mu_qcdwind40_cut("mu_qcdwind40");
  mu_qcdwind40_cut.add_u( "ptcone40r" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE40) >= 0.1 && bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE40) <= 0.2 );
  MUCUT mu_qcdd0_cut("mu_qcdd0"); // normal isolation cut PLUS d0sig>2.5
  mu_qcdd0_cut.add_uu( "ptcone40" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE40) < 0.1 , bind(&dg::fillh,_1,100,0,1,bind(&AnaMuon::pt_cone_ratio,_2,AnaMuon::CONE40)) );
  mu_qcdd0_cut.add_u( "d0sig" , bind(dgSTL::absD,bind( &AnaMuon::exPV_d0_signif , _1 )) > 2.5 );
  // alternative isolation cuts
  MUCUT mu_isoloose_cut("mu_iso_loose");
  mu_isoloose_cut.add_u( "ptcone20r" , bind(&AnaMuon::pt_cone_ratio,_1,AnaMuon::CONE20) < 0.1 );
  // Cuts for ntupling: these muons are used to broadly form all W/Z candidates for unbinned ntuple
  MUCUT mu_ntuple_cut("mu_ntuple");
  mu_ntuple_cut.add_u( "pt15" , bind(&AnaMuon::pt,_1) > 15.0 );
  
  // MET selections (actual MET cut is applied in Ws)
  // These MET collections are duplicated for raw and recalibrated jets
#define MET(x) METCOL met_rawjet_ ##x## _col; METCOL met_caljet_ ##x## _col
  MET(nominal);
  // new met
  MET(resosofttermsup);
  MET(resosofttermsdown);
  MET(resosofttermsupdown);
  MET(resosofttermsdownup);
  MET(scalesofttermsup);
  MET(scalesofttermsdown);
  // old met
  MET(resosoftup);
  MET(resosoftdown);
  MET(scalesoftup);
  MET(scalesoftdown);
  // mcp
  MET(mcp_smeared);
  MET(mcp_unscaled);
  MET(mcp_msup);
  MET(mcp_msdown);
  MET(mcp_idup);
  MET(mcp_iddown);
  MET(mcp_kup);
  MET(mcp_kdown);
  MET(mcp_cup);
  MET(mcp_cdown);
  // jets
  MET(jer);
  MET(jerup);
  MET(jerdown);
  MET(jesup);
  MET(jesdown);
  MET(npvup);
  MET(npvdown);
  MET(muup);
  MET(mudown);
#undef MET

  METCOL met_ref_col; // original out-of-the-box MET
  METCUT met_ref_cut("met_ref");
  met_ref_cut.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::REF_FINAL);

  METCOL met_truth_nonint_col;
  METCUT met_truth_nonint_cut("met_truth_nonint");
  met_truth_nonint_cut.add_u( "met_type" , bind(&AnaMET::met_type,_1) == AnaMET::TRUTH_NONINT);

  // JETS
#define JET(x,t) JETCOL rawjet_ ##t## _ ##x## _col; JETCOL caljet_ ##t## _ ##x## _col; \
  JETCOL rawjet_ ##t## _ ##x## _final_col; JETCOL caljet_ ##t## _ ##x## _final_col; \
  JETCOL rawjet_ ##t## _ ##x## _bad_col; JETCOL caljet_ ##t## _ ##x## _bad_col
  // EM jets
  JET(nominal,em);
  JET(jer,em);
  JET(jerup,em);
  JET(jerdown,em);
  JET(jesup,em);
  JET(jesdown,em);
  JET(npvup,em);
  JET(npvdown,em);
  JET(muup,em);
  JET(mudown,em);
  // LC jets
  JET(nominal,lc);
  JET(jer,lc);
  JET(jerup,lc);
  JET(jerdown,lc);
  JET(jesup,lc);
  JET(jesdown,lc);
  JET(npvup,lc);
  JET(npvdown,lc);
  JET(muup,lc);
  JET(mudown,lc);
  // MultiJES jet collections
  JETCOL rawjet_em_7gev_col; JETCOL caljet_em_7gev_col;
  JETCOL rawjet_lc_7gev_col; JETCOL caljet_lc_7gev_col; 
#undef JET
  JETCUT jet_7gev_cut("jet_7gev");
  jet_7gev_cut.add_u( "pt" , bind(&AnaJet::pt,_1) > 7.);
  JETCUT jet_final_cut("jet_final");
  jet_final_cut.add_uu( "pt" , bind(&AnaJet::pt,_1) > 30. , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );
  jet_final_cut.add_uu( "JVF", bind(dgSTL::absD,bind(&AnaJet::vtx_fraction,_1)) >= 0.75 || bind(&AnaJet::ntrkjetalg,_1) == 0,
			bind(&dg::fillh,_1,100,-1,1, bind(&AnaJet::vtx_fraction,_2)));
  jet_final_cut.add_uu( "rapidity", bind(dgSTL::absD,bind(&AnaJet::rapidity,_1)) < 4.4 , 
			bind(&dg::fillh,_1,100,-4.5,4.5,bind(&AnaJet::rapidity,_2)) );
  jet_final_cut.add_uu( "bad_looser", bind(&AnaJet::isBadJet_looser_v17,_1)==false,bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );
  // met cleaning jets
  JETCUT jet_bad_cut("jet_bad");
  jet_bad_cut.add_u( "pt" , bind(&AnaJet::pt,_1) >= 0.0);
  if(false) { // disable cuts here: will apply on-the-spot
    jet_bad_cut.add_u( "pt" , bind(&AnaJet::pt,_1) >= 20.0);
    jet_bad_cut.add_u( "eta", bind(dgSTL::absD,bind(&AnaJet::em_scale_eta,_1)) < 4.5 );
  }
  // note: actual isBadJet cut is applied later via count_if
  //jet_bad_cut.add_uu( "bad_looser" , bind(&AnaJet::isBadJet_looser_v17,_1)==true , bind(&dg::fillh,_1,200,0,200,bind(&AnaJet::pt,_2)) );

  /* DgStudy classes for various types of objects */
  
  // Truth
  StudyTruthParticle st_truth( "st_truth" , "Truth" );
  st_truth.study_n_objects(false);
  st_truth.add_category("POS",bind(&AnaTruthParticle::charge,_1)==1);
  st_truth.add_category("NEG",bind(&AnaTruthParticle::charge,_1)==-1);

  // Z candidates
  StudyZAsymmetry st_z( "st_z_final" , "Z event candidates" );
  st_z.study_n_objects(false);
  st_z.add_category("baseline",bind(&AnaZCrossSectionCandidate::asym_baseline_quick,_1));
  //st_z.add_category("samesign",bind(&AnaZCrossSectionCandidate::asym_ss_quick,_1));

  StudyZAsymmetry st_z_truth( "st_truth_reco_z" , "Z event candidates (truth level)" );
  st_z_truth.add_category("baseline",bind(&AnaZCrossSectionCandidate::asym_baseline_truth,_1));
  st_z_truth.study_n_objects(false);
  st_z_truth.do_save_ntuple(false);
  st_z_truth.do_save_histo(false);

  // W candidates (truth)
  StudyWAsymmetry st_w_truth( "st_w_final" , "W event candidates (truth level)" );
  st_w_truth.study_n_objects(false);
  st_w_truth.do_charge_separation(true);
  st_w_truth.do_save_ntuple(false);
  st_w_truth.do_save_histo(false);
  st_w_truth.run_qcd_slices(false);
  // baseline only
  if(MU_PT_CUT>19.99 && MU_PT_CUT<20.01) {
    st_w_truth.add_category("baseline",bind(&AnaWCrossSectionCandidate::asym_baseline_truth,_1)==true);
  } else {
    st_w_truth.add_category("baseline",bind(&AnaWCrossSectionCandidate::asym_baseline_truth25,_1)==true);
  }
  
  // W candidates
  StudyWAsymmetry st_w( "st_w_final" , "W event candidates" );
  st_w.do_bootstrap(false);
  st_w.study_n_objects(false);
  st_w.do_charge_separation(true);
  // the following assumes that the pt/eta cuts have already been applied at DgCut level
  // we do this so that it's easy
  st_w.add_category("baseline",bind(&AnaWCrossSectionCandidate::asym_baseline_quick,_1)==true);
  st_w.add_category("metfit",bind(&AnaWCrossSectionCandidate::asym_metfit_quick,_1)==true);
  st_w.add_category("wmtfit",bind(&AnaWCrossSectionCandidate::asym_wmtfit_quick,_1)==true);
  st_w.add_category("anyfit",bind(&AnaWCrossSectionCandidate::asym_anyfit_quick,_1)==true);

  // special trick when running in MCP scale determination mode: disable ALL histograms
  if(AnaConfiguration::muon_scale()==999) {
    RUNZ = true;
    RUNZ_NT = true;
    RUN_QCD_SLICES = false;
    st_w.do_save_histo(false);
    st_w_truth.do_save_histo(false);
    st_z.do_save_histo(false);
    st_z_truth.do_save_histo(false);
  }
  // special trick when running in unfolding mode: only generate response matrix
  if(AnaConfiguration::muon_scale()==1000) {
    std::cout << "INFO: turning on a response-matrix mode. All histograms and ntuples will be disabled" << std::endl;
    RUNZ = false;
    RUNZ_NT = false;
    RUN_QCD_SLICES = false;
    save_ntuples = false;
    st_w.do_save_histo(false);
    st_w.do_save_ntuple(false);
    st_w_truth.do_save_histo(false);
    st_w_truth.do_save_ntuple(false);
    st_z.do_save_histo(false);
    st_z_truth.do_save_histo(false);
    st_z_truth.do_save_ntuple(false);
  }

  // Additional classes and variables
  METUtility *metutil = new METUtility();
  // Old configuration
  //metutil->setCellFix( false );
  //metutil->doForwardEtaCut( false );
  //metutil->setPileUpUncertainty( 0.1 );

  // Turn on (off) the relevant MET terms
  // Standard MET_RefFinal has:
  // RefEle, RefGamma, RefTau, RefJet, SoftJets, RefMuon, MuonTotal, (CellOut), CellOut_Eflow
  metutil->defineMissingET(true, true, true, true, true, true, true, false, true);
  // The threshold below which jets enter the SoftJets term (JES is not applied)
  metutil->setSoftJetCut(20e3);
  // Whether to use MUID muons (otherwise STACO).
  metutil->setIsMuid(false);
  // Whether METUtility should scream at you over every little thing
  metutil->setVerbosity(false);

  // W boson pt and polarization reweighting: 1 nominal vpt;  2 variations; 1 alpgen y reweighting tool;  2 new polarization rw tools
  BosonPtReweightingTool* wzptutil[] = {0, 0,0, 0,  0,0};
  // Data-to-MC reweighting (for PowhegPythia signal ONLY)
  WeightProvider* wProv = 0;
  if(is_wmunu_powheg_pythia ) {
    wProv = new WeightProvider("CommonAnalysis/RootCore/data/ScaleFactorProvider/DataWeightsWmn.root",
			       is_wminmunu ? "WeightNEG" : "WeightPOS",
			       WeightProvider::EtaPt, 1.0 );
  }
  // Vertex reweighting
  VertexPositionReweightingTool *vtxutil = 0;
  // W and Z lineshape reweighting
  LineShapeTool rwtz;
  LineShapeTool rwtw(80.399, 2.085, false);

  // PDF reweighting
  PDFReweightTool *rwpdf = 0; 

  // this lists all the run periods included in your pileup reweighting histogram... need it for muon efficiency calculation
  run_periods.push_back( "B" ); //0
  run_periods.push_back( "D" ); //1
  run_periods.push_back( "E" ); //2
  run_periods.push_back( "F" ); //3
  run_periods.push_back( "G" ); //4
  run_periods.push_back( "H" ); //5
  run_periods.push_back( "I" ); //6
  run_periods.push_back( "J" ); //7
  run_periods.push_back( "K" ); //8
  run_periods.push_back( "L" ); //9
  run_periods.push_back( "M" ); //10

  // ================================================================

  // event loop
  unsigned long n_events = 0ul;
  static enum { PU_NONE , PU_MC11B } pileup_reweighting_mode = PU_NONE;
  AnaEventMgr* evtmgr = AnaEventMgr::instance(); assert(evtmgr);
  for( AnaEventMgr::iterator i=evtmgr->begin(), f=evtmgr->end(AnaConfiguration::user_max_nevents()); i!=f; ++i, ++n_events ) {
    dg::event_info().reset();
    dg::set_global_weight( 1.0 );
    WS1(event_weight); WS1(truth_weight);
    UNF_RESET();
    truth_met_pt = -1.0;

    // the following line allocates about 1 GB of VMEM
    const boost::shared_ptr<const AnaEvent>& evt( *i );
    if( !evt ) { break; }
    
    // get the full pathname of the current file
    const std::set<std::string> filenames = evt->provenance();
    assert(!filenames.empty());
    const std::string current_filename = *(filenames.begin());
    // check sample type and decide whether or not we need to make
    // data-only or mc-only preselection cuts.
    AnaEvent::tribool is_mc_tri = evt->check_flag( "mc" );
    is_mc = is_mc_tri;
    assert(is_data == (!is_mc));
    // retrieve metadata
    avgmu = evt->average_mu();
    actmu = evt->actual_mu();
    const unsigned long lbnum = evt->lumi_block();
    evnum = evt->event_number();
    raw_rnum = evt->run_number();
    mc_weight = is_mc ? evt->mc_weight() : 1.0;
    WSX(event_weight,mc_weight);
    // MC pileup reweighting
    static boost::shared_ptr<Root::TPileupReweighting> pw( new Root::TPileupReweighting( "pw" ) );
    static boost::shared_ptr<Root::TPileupReweighting> pw_up( new Root::TPileupReweighting( "pw_up" ) );
    static boost::shared_ptr<Root::TPileupReweighting> pw_down( new Root::TPileupReweighting( "pw_down" ) );
    // special period-separated files
    //static boost::shared_ptr<Root::TPileupReweighting> pw_DtoK( new Root::TPileupReweighting( "pw_DtoK" ) );
    //static boost::shared_ptr<Root::TPileupReweighting> pw_LtoM( new Root::TPileupReweighting( "pw_LtoM" ) );
#define PINIT(p,s,df) p->SetDefaultChannel(0);				\
    p->SetUnrepresentedDataAction(2);					\
    p->DisableWarnings(true);						\
    p->SetDataScaleFactors( s );					\
    if(pileup_config_file.empty()) {					\
      if(evt->check_tag("mc11c")) {					\
	p->AddConfigFile("pileup_histos/MC11c.prw.root");		\
	p->AddConfigFile("CommonAnalysis/RootCore/data/PileupReweighting/mc11b_defaults.prw.root"); \
	std::cout << "Initializing TPileupReweighting for MC11C" << std::endl; \
      }									\
      else {								\
	assert(0&&"Bad MC type");					\
      }									\
    } else {								\
      p->AddConfigFile(pileup_config_file.c_str());			\
    }									\
    p->AddLumiCalcFile(df);						\
    p->Initialize();							\
    std::cout << "   ---> total lumi = " << p->GetIntegratedLumi() << " from " << df << std::endl
    if( (evt->check_tag("mc11b") || evt->check_tag("mc11c")) && pileup_reweighting_mode!=PU_MC11B ) {
      PINIT(pw,AnaConfiguration::pileup_data_scale(),pileup_data_file);
      PINIT(pw_down,0.97,pileup_data_file);
      PINIT(pw_up,1.03,pileup_data_file);
//       PINIT(pw_DtoK,AnaConfiguration::pileup_data_scale(),pileup_data_fileDtoK);
//       PINIT(pw_LtoM,AnaConfiguration::pileup_data_scale(),pileup_data_fileLtoM);
//       int_lumi_DtoM = pw->getIntegratedLumiVector();
//       int_lumi_DtoK = pw_DtoK->getIntegratedLumiVector();
//       int_lumi_LtoM = pw_LtoM->getIntegratedLumiVector();
      int_lumi = pw->getIntegratedLumiVector();
      const double lumi_tot = pw->GetIntegratedLumi();
      std::cout << "Total lumi = " << lumi_tot << std::endl;
      pileup_reweighting_mode=PU_MC11B;
    } else if( !evt->check_tag( "mc11b" ) && !evt->check_tag( "mc11c" ) && pileup_reweighting_mode!=PU_NONE ) {
      pw.reset(); pw_up.reset(); pw_down.reset();
      pileup_reweighting_mode=PU_NONE;
    }
    if(is_mc) {
      pw->SetRandomSeed(314159 + evt->mc_channel()*2718 + evnum);
      pw_up->SetRandomSeed(314159 + evt->mc_channel()*2718 + evnum);
      pw_down->SetRandomSeed(314159 + evt->mc_channel()*2718 + evnum);
//       pw_DtoK->SetRandomSeed(314159 + evt->mc_channel()*2718 + evnum);
//       pw_LtoM->SetRandomSeed(314159 + evt->mc_channel()*2718 + evnum);
    }
#undef PINIT
    // for MC, calculate pileup weight
    if(is_mc && do_pileup) {
      pu_weight[0] = pw ? pw->GetCombinedWeight( raw_rnum , evt->mc_channel() , avgmu ) : 1. ;
      pu_weight[1] = pw_down ? pw_down->GetCombinedWeight( raw_rnum , evt->mc_channel() , avgmu ) : 1. ;
      pu_weight[2] = pw_up ? pw_up->GetCombinedWeight( raw_rnum , evt->mc_channel() , avgmu ) : 1. ;
//       pu_weight[3] = pw_DtoK ? pw_DtoK->GetCombinedWeight( raw_rnum , evt->mc_channel() , avgmu ) : 1. ;
//       pu_weight[4] = pw_LtoM ? pw_LtoM->GetCombinedWeight( raw_rnum , evt->mc_channel() , avgmu ) : 1. ;
      // skip event; negative pileup weight means we don't know the weight (rare event)
      for(int ii=1; ii<5; ii++) {
	if(pu_weight[ii]<0) pu_weight[ii]=0.0;
      }
      if(pu_weight[0]<0) {
	pu_weight[0]=0.0;
	std::cerr << "WARNING: found event with negative pileup weight: " << pu_weight[0] << std::endl;
      }
      dg::fillh( "pu_weight" , 100 , -2 , 2 , pu_weight[0] );
      dg::fillh( "mc_weight" , 100 , -2 , 2 , mc_weight );
      event_weight[0] *= pu_weight[0]; // nominal
      event_weight[1] *= pu_weight[1]; // down
      event_weight[2] *= pu_weight[2]; // up
      for(int iii=1+NWPU;iii<NWEIGHTS;iii++) event_weight[iii] *= pu_weight[0];
      dg::set_global_weight( event_weight[0] );
      dg::event_info().set_mc_weight(mc_weight);
      dg::event_info().set_pu_weight(pu_weight[0]);
    }

    // NUMBER OF EVENTS. Note that weights below this are NOT included in nevts normalization!
    dg::fillh("nevts",1,1,2,1);  // pileup and MC
    dg::fillh("nevts_eff",1,1,2,1);  // will subtract off efficiency corrections
    dg::fillh("nevts_trig",1,1,2,1); // will subtract off trigger corrections
    dg::fillh("nevts_efftrig",1,1,2,1); // will subtract off both efficiency and trigger corrections
    dg::fillhw("nevts_unw",1,1,2,1,1.0); // unweighted
    dg::fillhw("nevts_mcw",1,1,2,1,dg::event_info().get_mc_weight()); // only mc-weight

    // Vertex reweighting
    if (is_mc) {
      if(!vtxutil) {
	const std::string vtxwpath = "CommonAnalysis/RootCore/data/egammaAnalysisUtils/zvtx_weights_2011_2012.root";
	vtxutil = new VertexPositionReweightingTool( get_mc_beamspot(current_filename), vtxwpath );
      }
      const bool has_vxz = evt->truth_particles().size()>2 && evt->truth_particles()[2]->vx_z()>-9e5;
      vx_weight = has_vxz ? vtxutil->GetWeight(evt->truth_particles()[2]->vx_z()) : 1.0;
      dg::event_info().set_vx_weight(vx_weight);
      if( HIST_VXZ_WEIGHT) {
	WSX(event_weight,vx_weight);
	dg::set_global_weight( event_weight[0] );
      }
      if(has_vxz) {
	dg::fillh( "tr_vxz0" , 100 , -200 , 200 , evt->truth_particles()[2]->vx_z() , "truth vertex z0" );
	dg::fillhw( "tr_vxz0_unw" , 100,-200,200, evt->truth_particles()[2]->vx_z() , 1.0 );
      }
      else {
	static long nvxwarn = 0;
	nvxwarn++;
	if(nvxwarn<100) {
	  std::cerr << "WARNING: unable to find a truth vertex. Total truth particles = " << evt->truth_particles().size() << std::endl;
	}
      }
    }
    dg::fillh("nevts2_vtx",1,1,2,1);  // pileup and MC and vertex

    // retrieve more metadata
//     rnum_DtoM = is_data ? raw_rnum : pw->GetRandomRunNumber(raw_rnum);
//     rnum_DtoK = is_data ? raw_rnum : pw_DtoK->GetRandomRunNumber(raw_rnum);
//     rnum_LtoM = is_data ? raw_rnum : pw_LtoM->GetRandomRunNumber(raw_rnum);
    rnum = is_data ? raw_rnum : pw->GetRandomRunNumber(raw_rnum);
    /* Comment on rnum in MC:
       Pseudo-run numbers in aggregate give the right fraction of events in each run.
       However, they don't directly maintain the correlations to <nmu> for a given MC event.
       In other words, usign rnum to split into DtoK and LtoM does not put events with the right
       nmu, so DtoK and LtoM MET distributions won't be updated correctly.
     */
    dg::event_info().set_run_number(rnum);
    dg::event_info().set_event_number(evnum);
    std::string srnum = detector::to_string(raw_rnum);

    // TRUTH (CONSISTENT WITH TruthStore.cxx) + W/Z TRUTH REWEIGHTING
    bool skip_event = false;
    if ( is_mc && (is_wmunu || is_wtaunu || is_zmumu || is_ztautau)) {
      dg::down( "truth" , "Some truth-based quantities" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
      const long lpdg = (is_wmunu || is_zmumu) ? 13 : 15; // muon or electrons?
      boost::shared_ptr<const AnaTruthParticle> l1;
      boost::shared_ptr<const AnaTruthParticle> l2;
      boost::shared_ptr<const AnaTruthParticle> v;
      // truth jets [pt and y]
      dgSTL::copy_if( evt->atlas_cone_4_truth_jets().begin() , evt->atlas_cone_4_truth_jets().end() ,back_inserter(truth_jet_all_col),CUT_ALL(AnaJet,truth_jet_cut) );
      // truth MET [presence]
      dgSTL::copy_if( evt->missing_et().begin(),evt->missing_et().end(),back_inserter(truth_met_col),CUT_ALL(AnaMET,truth_met_cut) );
      truth_met_pt = truth_met_col.front()->pt();
      // general MC metadata
      const MC_TYPE mc_type = get_mc_type(evt->mc_channel());
      assert(mc_type != MC_NONE);
      const int wz_status = get_wz_status(mc_type,BORN);
      // initialize W pt reweighting (only done once)
      const std::string rw_path = "CommonAnalysis/RootCore/data/egammaAnalysisUtils/WZBosonKinematicsReweight.root";
      if(!wzptutil[0]) {
	const std::string& wzpt_source = get_wz_reweight_source(mc_type);
	// set up for vpt & pol. reweighting
	if(wzpt_source!="") {
	  std::string wzpt_destination;
	  wzpt_destination = WZPT_WEIGHT_TARGET;
	  assert(std::string("PowhegPythia8MC11_ZData2011") == wzpt_destination);
	  BosonPtReweightingTool::ePtWeightType vtype = (is_wmunu || is_wtaunu) ? BosonPtReweightingTool::PeakWeight : BosonPtReweightingTool::MassBinnedWeight;
	  wzptutil[0] = new BosonPtReweightingTool(wzpt_source,wzpt_destination,vtype,rw_path);
	  wzptutil[0]->SetImposePhysicalPolConstraint(WPOL_PHYS_CONSTRAIN);
	  wzpt_destination = WZPT_WEIGHT_TARGET2;
	  assert(std::string("Sherpa14MC11") == wzpt_destination);
	  wzptutil[1] = new BosonPtReweightingTool(wzpt_source,wzpt_destination,vtype,rw_path);
	  wzptutil[1]->SetImposePhysicalPolConstraint(WPOL_PHYS_CONSTRAIN);
	  wzpt_destination = WZPT_WEIGHT_TARGET3;
	  assert(std::string("PowhegPythia8MC11") == wzpt_destination);
	  wzptutil[2] = new BosonPtReweightingTool(wzpt_source,wzpt_destination,BosonPtReweightingTool::PeakWeight,rw_path);
	  wzptutil[2]->SetImposePhysicalPolConstraint(WPOL_PHYS_CONSTRAIN);
	}
      }
      if(!wzptutil[3] && is_alpgen && !is_wgamma) {
	// set up for alpgen reweighting
	std::string alpgen_source = BosonPtReweightingTool::MapMCName(current_filename);
	if(alpgen_source != current_filename) {
	  std::string alpgen_target = alpgen_source+"_CT10";
	  wzptutil[3] = new BosonPtReweightingTool(alpgen_source,alpgen_target,
						   BosonPtReweightingTool::PeakWeight,rw_path);
	}
      }
      if(!wzptutil[4] && is_wmunu_mcnlo) {
	// set up McAtNLOMC11 -> PowhegHerwigMC11 reweighting
	std::string rw_source = BosonPtReweightingTool::MapMCName(current_filename);
	assert(rw_source == "McAtNloMC11");
	std::string rw_target = "PowhegHerwigMC11";
	wzptutil[4] = new BosonPtReweightingTool(rw_source,rw_target,
						 BosonPtReweightingTool::PeakWeight,rw_path);
      }
      if(!wzptutil[5] && (is_wmunu_mcnlo || is_wmunu_powheg_herwig) ) {
	// set up PowhegHerwigMC11,McAtNloMC11 -> PowhegPythiaMC11 reweighting
	std::string rw_source = BosonPtReweightingTool::MapMCName(current_filename);
	if(is_wmunu_mcnlo) assert(rw_source == "McAtNloMC11");
	if(is_wmunu_powheg_herwig) assert(rw_source == "PowhegHerwigMC11");
	std::string rw_target = "PowhegPythiaMC11";
	wzptutil[5] = new BosonPtReweightingTool(rw_source,rw_target,
						 BosonPtReweightingTool::PeakWeight,rw_path);
      }
      // W->lnu or Z->ll selection
      bool lepFound = false;
      lepFound = getBosonBornLeptons(evt->truth_particles(),lpdg,v,l1,l2,is_zmumu || is_ztautau);
      if(!lepFound) lepFound = getBosonBornLeptons(evt->truth_particles(),-lpdg,v,l1,l2,is_zmumu || is_ztautau);
      if(lepFound) { assert(l1); assert(l2); assert(v); }
      // W->lnu case:
      if(lepFound && (is_wmunu || is_wtaunu)) {
	boost::shared_ptr<const AnaMET> met;
	if(NUMET) {
	  boost::shared_ptr<AnaMET> tmet (new AnaMET());
	  tmet->setup_met_for_truth( l2->pt() , l2->phi() , l2->eta() , l2->m());
	  met = tmet;
	} else {
	  met = truth_met_col.front();
	}
	boost::shared_ptr<AnaW> w(new AnaW(l1,met));
	boost::shared_ptr<AnaWCrossSectionCandidate> wc(new AnaWCrossSectionCandidate(w));
	wc->add_event_lepton(l1);
	wc->add_event_jets(truth_jet_all_col.begin() , truth_jet_all_col.end());
	// REWEIGHTING
	// w pt reweighting
	if( wzptutil[0] && !is_alpgen ) {
	  const double fwpt = v->pt()*1000.0;
	  double wpt_weight = v->charge()>0 ? wzptutil[0]->GetPtWeightWplus(fwpt) : wzptutil[0]->GetPtWeightWminus(fwpt);
	  dg::event_info().set_wzpt_weight(wpt_weight);   // main signal weight
	  // apply to histograms, too?
	  if(HIST_WZPT_WEIGHT) {
	    for(int iii=0;iii<1+NWPU;iii++) event_weight[iii] *= wpt_weight;
	    for(int iii=1+NWPU+NWPT;iii<NWEIGHTS;iii++) event_weight[iii] *= wpt_weight;
	    dg::set_global_weight( event_weight[0] );
	    // additional weights
	    wpt_weight = v->charge()>0 ? wzptutil[1]->GetPtWeightWplus(fwpt) : wzptutil[1]->GetPtWeightWminus(fwpt);
	    event_weight[1+NWPU+0] *= wpt_weight;
	    dg::event_info().set_wzpt_weight2(wpt_weight);
	    wpt_weight = v->charge()>0 ? wzptutil[2]->GetPtWeightWplus(fwpt) : wzptutil[2]->GetPtWeightWminus(fwpt);
	    event_weight[1+NWPU+1] *= wpt_weight;
	    dg::event_info().set_wzpt_weight3(wpt_weight);
	  }
	}
	// w polarization (mcnlo -> powheg_herwig)
	if( wzptutil[4] && is_wmunu_mcnlo ) {
	  const double wpol_weight = l1->charge()>0 ? wzptutil[4]->GetPolarisationWeightWplusGeV(l1->four_vector(),l2->four_vector()) : wzptutil[4]->GetPolarisationWeightWminusGeV(l1->four_vector(),l2->four_vector());
	  event_weight[1+NWPU+NWPT+0] *= wpol_weight;
	  // since this is NOT applied to central value,
	  // we do not need to multiply all other weights by wpol_weight!
	}
	// w polarization (mcnlo,powheg_herwig -> powheg_pythia)
	if( wzptutil[5] && (is_wmunu_mcnlo||is_wmunu_powheg_herwig) ) {
	  const double wpol_weight = l1->charge()>0 ? wzptutil[5]->GetPolarisationWeightWplusGeV(l1->four_vector(),l2->four_vector()) : wzptutil[5]->GetPolarisationWeightWminusGeV(l1->four_vector(),l2->four_vector());
	  dg::event_info().set_wpol_weight(wpol_weight);
	  event_weight[1+NWPU+NWPT+1] *= wpol_weight;
	  // since this is NOT applied to central value,
	  // we do not need to multiply all other weights by wpol_weight!
	}
	// Max's Data-to-MC signal reweighting (MCModeling). Make sure the found truth boson has the right charge.
	if(wProv && ( (v->charge()>0 && is_wplusmunu) || (v->charge()<0 && is_wminmunu) ) ) {
	  double dmc_weight = wProv->getWeight( const_cast<TLorentzVector&> (l1->four_vector()) );
	  dg::event_info().set_ph_weight(dmc_weight);
	  event_weight[1+NWPU+NWPT+NWPO+0] *= dmc_weight;
	  // since this is NOT applied to central value,
	  // we do not need to multiply all other weights by wpol_weight!
	}
	// Alpgen rapidity reweighting
	if( wzptutil[3] && is_alpgen ) {
	  const double alpy_weight = v->charge()>0 ? wzptutil[3]->GetRapidityWeightWplus( v->rapidity() ) : wzptutil[3]->GetRapidityWeightWminus( v->rapidity() );
	  dg::event_info().set_alpy_weight(alpy_weight);
	  // apply to histograms, too?
	  if(HIST_ALPY_WEIGHT) {
	    WSX(event_weight,alpy_weight);
	    dg::set_global_weight( event_weight[0] );
	  }
	}
	// perform w lineshape reweighting
	if( true ) {
	  const double m = v->m();
	  int id1 = evt->mcevt_id1();
	  int id2 = evt->mcevt_id2();
	  int wz_pdgid = v->pdg_id();
	  assert(wz_pdgid==24 || wz_pdgid==-24);
	  pdfinfo_lineshape_quarks(id1,id2,wz_pdgid);
	  double ls1_weight = 1.0;
	  double ls2_weight = 1.0;
	  if( mc_type == MC_POWHEG_HERWIG || mc_type == MC_POWHEG_PYTHIA ) {
	    ls1_weight = rwtw.reweightPowhegToImprovedBorn( id1, id2, m );
	  }
	  else if( mc_type == MC_PYTHIA ) {
	    ls1_weight = rwtw.reweightPythiaToImprovedBorn( id1, id2, m );
	  }
	  else if( mc_type == MC_MCNLO ) {
	    ls1_weight = rwtw.reweightMCANLOToImprovedBorn( id1, id2, m );
	  }
	  dg::event_info().set_ls1_weight(ls1_weight);
	  dg::event_info().set_ls2_weight(ls2_weight);
	  // apply to histograms, too?
	  if(HIST_LS_WEIGHT) {
	    WSX(event_weight,ls1_weight*ls2_weight);
	    dg::set_global_weight( event_weight[0] );
	  }
	}
	// Experimental Powheg+Pythia -> Powheg+Herwig reweighting
	if(false && mc_type == MC_POWHEG_PYTHIA && PYTHIA_TO_HERWIG) {
	  static SFProvider tc("data/pythia_to_herwig_rw.root","scale","pythia","herwig",SFProvider::PtEta,1.0);
	  double ph_weight = tc.getSF(l1->pt() , l2->pt());
	  dg::event_info().set_ph_weight(ph_weight);
	  // std::cout << "DEBUG: " << unftru->lepton()->pt() << " " << unftru->met()->pt() << " : " << ph_weight << std::endl;
	  // WSX(event_weight,ph_weight);
	}
	// SAVE RESULTS
	// direct truth w's
	if(false) {
	  st_truth.repurpose( "st_truth_w" , "Truth W bosons" );
	  st_truth.for_only( v );
	}
	// inferred (aka reconstructed) truth w's
	st_w_truth.repurpose( "st_w_final" , "Inferred truth W bosons" );
	std::vector<bool> res = st_w_truth.for_only( wc );
	assert( res.size()==1 );
	dg::event_info().set_is_fiducial( res[0] );
	// Map unfolding structures
	if(is_mc || is_unfold) {
	  for(unfdata_type::iterator it = unfdata.begin(); it!=unfdata.end(); it++) { // loop over systematics
	    it->second.unftru = wc;
	    it->second.Truth_Is_Fiducial = res[0];
	    it->second.Truth_PtW = v->pt();
	    it->second.Truth_MtW = wc->transverse_mass();
	    it->second.Truth_PtNu = l2->pt();
	    it->second.Truth_PtLep = L(l1->pt()); // IMPORTANT: we expand the last bin to be beyond 120 GeV
	    it->second.Truth_EtaLep = l1->eta();
	  }
	}
      } // end W->lnu case
      
      // Z->ll case:
      if(lepFound && (is_zmumu || is_ztautau)) {
	// AnaZ has to be pt-sorted
	boost::shared_ptr<AnaZ> z(new AnaZ( l1->pt()>=l2->pt() ? l1 : l2  ,  l1->pt()>=l2->pt() ? l2 : l1 ));
	boost::shared_ptr<AnaZCrossSectionCandidate> zc(new AnaZCrossSectionCandidate(z));
	zc->add_event_lepton(l1);
	zc->add_event_lepton(l2);
	zc->add_event_jets(truth_jet_all_col.begin() , truth_jet_all_col.end());
	// Filter Zmumu samples that are already covered by Drell-Yan (optional)
	if( evt->mc_channel()==108304 || evt->mc_channel()==126007 ) { // powheg
	  if(v->m()<53.8) skip_event = true;
	}
	if( evt->mc_channel()==106088 ) { // mc@nlo
	  if(v->m()<60.0) skip_event = true;
	}
	// REWEIGHTING
	// perform z pt reweighting
	if( wzptutil[0] && !is_alpgen) {
	  const double fzpt = v->pt()*1000.0;
	  const double fzm = v->m()*1000.0;
	  double zpt_weight = wzptutil[0]->GetPtWeightZ(fzpt,fzm);
	  dg::event_info().set_wzpt_weight(zpt_weight);
	  // apply to histograms, too?
	  if(HIST_WZPT_WEIGHT) {
	    for(int iii=0;iii<1+NWPU;iii++) event_weight[iii] *= zpt_weight;
	    for(int iii=1+NWPU+NWPT;iii<NWEIGHTS;iii++) event_weight[iii] *= zpt_weight;
	    dg::set_global_weight( event_weight[0] );
	    // additional weights
	    zpt_weight = wzptutil[1]->GetPtWeightZ(fzpt,fzm);
	    event_weight[1+NWPU+0] *= zpt_weight;
	    dg::event_info().set_wzpt_weight2(zpt_weight);
	    zpt_weight = wzptutil[2]->GetPtWeightZ(fzpt,fzm);
	    event_weight[1+NWPU+1] *= zpt_weight;
	    dg::event_info().set_wzpt_weight3(zpt_weight);
	  }
	}
	// Z mc@nlo deficit around 60 GeV
	if( wzptutil[0] && is_zmumu_mcnlo ) {
	  const double znlo_weight = wzptutil[0]->LineShapeFix_Z_McAtNloGeV(v->m());
	  dg::event_info().set_znlo_weight(znlo_weight);
	  // apply to histograms, too?
	  if(HIST_ZNLO_WEIGHT) {
	    WSX(event_weight,znlo_weight);
	    dg::set_global_weight( event_weight[0] );
	  }
	}
	// Alpgen rapidity reweighting
	if(wzptutil[3] && is_alpgen) {
	  const double alpy_weight = wzptutil[3]->GetRapidityWeightZ( v->rapidity() );
	  dg::event_info().set_alpy_weight(alpy_weight);
	  // apply to histograms, too?
	  if(HIST_ALPY_WEIGHT) {
	    WSX(event_weight,alpy_weight);
	    dg::set_global_weight( event_weight[0] );
	  }
	}
	// perform z lineshape reweighting
	if( true ) {
	  const double m = v->m();
	  int id1 = evt->mcevt_id1();
	  int id2 = evt->mcevt_id2();
	  int wz_pdgid = v->pdg_id();
	  assert(wz_pdgid==23);
	  pdfinfo_lineshape_quarks(id1,id2,wz_pdgid);
	  double ls1_weight = 1.0;
	  double ls2_weight = 1.0;
	  if( mc_type == MC_POWHEG_HERWIG || mc_type == MC_POWHEG_PYTHIA ) {
	    ls1_weight = rwtz.reweightPowhegToImprovedBorn( id1, id2, m );
	  }
	  else if( mc_type == MC_PYTHIA ) {
	    ls1_weight = rwtz.reweightPythiaToImprovedBorn( id1, id2, m );
	  }
	  else if( mc_type == MC_MCNLO ) {
	    ls1_weight = rwtz.reweightMCANLOToImprovedBorn( id1, id2, m );
	  }
	  dg::event_info().set_ls1_weight(ls1_weight);
	  dg::event_info().set_ls2_weight(ls2_weight);
	  // apply to histograms, too?
	  if(HIST_LS_WEIGHT) {
	    WSX(event_weight,ls1_weight*ls2_weight);
	    dg::set_global_weight( event_weight[0] );
	  }
	}
	// SAVE RESULTS
	// direct truth z's
	if(false) {
	  st_truth.repurpose( "st_truth_z" , "Truth Z bosons" );
	  st_truth.for_only( v );
	}
	// inferred (aka reconstructed) truth z's
	st_z_truth.repurpose( "st_z_final" , "Inferred truth Z bosons" );
	std::vector<bool> res = st_z_truth.for_only( zc );
	dg::event_info().set_is_fiducial( res[0] );
      }
    }
    if(ZMUMU_FIX && skip_event) {
      dg::fillhw("nevts_zskip",1,1,2,1,1.0); // unweighted
      continue;
    }
    dg::fillh("nevts3_wz",1,1,2,1);  // pileup and MC and vertex and w/z pt+lineshape

    // PDF reweighting [ has to go last ]
    if(is_mc && PDF_REWEIGHTING && is_pdfrw && !NOMONLY) {
      if( !rwpdf ) {
	const std::string LHAPATH = "/share/ftkdata/software/pdfsets";
	rwpdf = new PDFReweightTool();
	if(PDF_REWEIGHTING>1) {
	  rwpdf->SetLHAPATH(LHAPATH);  // need for NNPDF and some other latest-version PDFs
	  rwpdf->AddDefaultPDFSets();
	} else {
	  assert(PDF_REWEIGHTING==1);
	  rwpdf->AddCT10PDFSet();
	}
	rwpdf->Initialize();
	assert( rwpdf->numberPDF(1) + 1 == NREPLICASP );
      }
      const double mcevt_pdf_scale = evt->mcevt_pdf_scale();
      const int mcevt_id1 = evt->mcevt_id1();
      const int mcevt_id2 = evt->mcevt_id2();
      const double x1 = evt->mcevt_pdf_x1();
      const double x2 = evt->mcevt_pdf_x2();
      const double pdf1 = evt->mcevt_pdf1();
      const double pdf2 = evt->mcevt_pdf2();
      if (PDF_REWEIGHTING==2) {
	// warning: hardcoded order of PDFs (from PDFReweightTool.hpp)
	assert(pdf_offset==5);
	const bool debug_nominal_pdf = false;
	std::vector<double> pdf_weight;
	std::vector<float> ntpdf;
	pdf_weight.push_back( rwpdf->GetEventWeight(1,0,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,debug_nominal_pdf) );
	pdf_weight.push_back( rwpdf->GetEventWeight(2,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,false) );
	pdf_weight.push_back( rwpdf->GetEventWeight(3,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,false) );
	pdf_weight.push_back( rwpdf->GetEventWeight(4,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,false) );
	pdf_weight.push_back( rwpdf->GetEventWeight(5,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,false) );
	// 5 PDF variations
	for(int ii=1+NWPU+NWPT+NWPO+NWDA; ii<1+NWPU+NWPT+NWPO+NWDA+pdf_offset; ii++) {
	  const int ipdf = ii-(1+NWPU+NWPT+NWPO+NWDA);
	  assert(  ipdf >= 0 && ipdf<pdf_weight.size()  );
	  event_weight[ii] *= pdf_weight[ipdf];
	  ntpdf.push_back(pdf_weight[ipdf]);
	}
	// save to ntuple, too [ needs recompiled LHAPDF that supports multiple families ]
	dg::event_info().set_pdf_weights( ntpdf );
      } else {
	assert(pdf_offset==0);
      }
      // reweight to each member of CT10 family
      for(int ii=1+NWPU+NWPT+NWPO+NWDA+pdf_offset; ii<NWEIGHTS; ii++) {
	const int member = ii - (1+NWPU+NWPT+NWPO+NWDA) - pdf_offset;
	assert(member>=0 && member<= 52);
	event_weight[ii] *= rwpdf->GetEventWeight(1,member,mcevt_pdf_scale,mcevt_id1,mcevt_id2,x1,x2,pdf1,pdf2,false);
      }
    }
    dg::fillh("nevts4_pdf",1,1,2,1);  // pileup and MC and vertex and w/z pt+lineshape and pdf [which shouldn't be used per Kristin]

    // END OF TRUTH-LEVEL WEIGHTS
    WSE(truth_weight,event_weight); // copy over event_weight to truth_weight

    // map unfolding structures. Use all MC weights up to now
#define FT(ix,x) it->second.Truth_Weight_Replicas[ix]=x; it->second.Truth_Weight=x
    if(is_mc || is_unfold) {
      for(unfdata_type::iterator it = unfdata.begin(); it!=unfdata.end(); it++) { // loop over systematics
	it->second.Truth_PileUp_Weight=pu_weight[0];
	bool found_key_tru = false;
	// check if this is one of the truth-level SF systematics [ excluding PDF ]
	for(int ii=0; ii<unf_keys_tru.size(); ii++) {
	  if( unf_keys_tru[ii] == it->first ) {
	    FT(0,event_weight[ii+1]);
	    found_key_tru = true;
	    break;
	  }
	}
	if(found_key_tru) continue;
	// PDF member variations
	if(it->first == "PDFMEM") {
	  assert( it->second.Truth_Weight_Replicas.size() == NREPLICASP );
	  for(int member = 0; member < NREPLICASP; member++) { // [0..52]
	    it->second.Truth_Weight_Replicas[member]=event_weight[1+NWPU+NWPT+NWPO+NWDA + pdf_offset + member];
	  }
	  it->second.Truth_Weight=event_weight[1+NWPU+NWPT+NWPO+NWDA+pdf_offset];
	}
	// everything else: fall back on nominal
	else {
	  it->second.Truth_Weight=event_weight[0];
	  for(int ii=0; ii<it->second.Truth_Weight_Replicas.size(); ii++) { it->second.Truth_Weight_Replicas[ii]=event_weight[0]; }
	}
      }
    }
#undef FT
    
    dg::fillh( "avgmu" , 30 , 0 , 30 , avgmu , "average_mu" );
    dg::fillhw( "avgmu_unw" , 30,0,30, avgmu , 1.0 );
    dg::fillh( "actmu" , 30 , 0 , 30 , actmu , "actual_mu" );
    dg::fillhw( "actmu_unw" , 30,0,30, actmu , 1.0 );

    // dump verbose event info
    if(false) {
      std::cout << "WEIGHTS: " << evnum << " " << mc_weight << " " << event_weight[0] << " " << avgmu << " " << pu_weight[0] << std::endl;
    }
    if( false ) {
      cout << boost::format( "%|d| - run %|d| event %|d| lumi block %|d|") % n_events % rnum % evnum % lbnum << endl;
      BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , evt->muons() ) { p->print(cout); }
      BOOST_FOREACH( const boost::shared_ptr<const AnaMuon>& p , evt->muons() ) { p->print(cout); }
      BOOST_FOREACH( const boost::shared_ptr<const AnaVertex>& p , evt->vertices() ) { p->print(cout); }
      BOOST_FOREACH( const boost::shared_ptr<const AnaParticle>& p , evt->missing_et() ) { p->print(cout); }
    }

    // GRL
    const bool e_grl = is_mc ? true : grl.HasRunLumiBlock(rnum,lbnum);

    CUTFLOW(w,01,true,e_grl);
    CUTFLOW(z,01,true,e_grl);
    if( !e_grl ) {
      UNF_MISS();
      continue;
    } 
    
    // HEAVY-FLAVOR MC HFOR CLEANING (hfor=4 means "kill")
    const bool e_hfor = is_data ? true : (is_heavy_flavor(evt->mc_channel()) ? evt->mc_hfor()!=4 : true);
    CUTFLOW(w,02,true,e_hfor);
    CUTFLOW(z,02,true,e_hfor);
    if ( !e_hfor ) {
      UNF_MISS();
      continue;
    }
    
    // LAR CLEANING
    bool e_lar_cleaning = is_mc ? true : evt->trigger()->error_lar()<=1;
    CUTFLOW(w,03,true,e_lar_cleaning);
    CUTFLOW(z,03,true,e_lar_cleaning);
    if ( !e_lar_cleaning ) {
      UNF_MISS();
      continue;
    }

    // prepare the Study classes for reco-level plots
    
    st_w.do_yield( pB[0],pM[1],float(rnum),float( pw->GetIntegratedLumi(rnum,rnum)) );
    st_z.do_yield( pB[0],pM[1],float(rnum),float( pw->GetIntegratedLumi(rnum,rnum)) );
    st_w.for_run(rnum);
    st_z.for_run(rnum);

    st_w.for_average_mu(avgmu);
    st_z.for_average_mu(avgmu);

    st_w.for_mc(is_mc);
    st_z.for_mc(is_mc);

    // VERTEX
    {
      dgSTL::copy_if( evt->vertices().begin(),evt->vertices().end(),back_inserter(vtx_base_col),CUT_ALL(AnaVertex,vtx_base_cut) );
      dgSTL::copy_if( evt->vertices().begin(),evt->vertices().end(),back_inserter(vtx_col),CUT_ALL(AnaVertex,vtx_cut) );
      dgSTL::copy_if( evt->vertices().begin(),evt->vertices().end(),back_inserter(vtx_softmet_good_col),CUT_ALL(AnaVertex,vtx_softmet_good_cut) );
      dgSTL::copy_if( evt->vertices().begin(),evt->vertices().end(),back_inserter(vtx_softmet_col),CUT_ALL(AnaVertex,vtx_softmet_cut) );
      dgSTL::copy_if( evt->vertices().begin(),evt->vertices().end(),back_inserter(vtx_calib_col),CUT_ALL(AnaVertex,vtx_calib_cut) );
    }
    int nvtx_sec = vtx_col.size();
    dg::fillh( "nvtx_grltrig" , 15,0,15, nvtx_sec );
    dg::fillhw( "nvtx_grltrig_unw" , 15,0,15, nvtx_sec,1.0 );
    int nvtxsoftmet = 0;
    if(!vtx_softmet_good_col.empty()) {
      nvtxsoftmet = vtx_softmet_col.size();
    }
    // only accept first vertex, which must have >=3 tracks [per W/Z twiki]
    const bool e_primary_vertex = !vtx_col.empty() && (vtx_col[0]==evt->vertices()[0]);
    CUTFLOW(w,04,true,e_primary_vertex);
    CUTFLOW(z,04,true,e_primary_vertex);
    if ( !e_primary_vertex ) {
      UNF_MISS();
      continue;
    }
    dg::fillh( "vxz0" , 100 , -200 , 200 , vtx_col[0]->z() , "truth vertex z0" );
    dg::fillhw( "vxz0_unw" , 100,-200,200, vtx_col[0]->z() , 1.0 );

    // TRIGGER
    // retrieve trigger
    const boost::shared_ptr<const AnaTrigger>& trigger = evt->trigger();
    assert(trigger);
    // apply trigger
    bool e_trigger = trigger->wz_asym_muon_trigger_2012_nomg(rnum); // switched to mu18 muid chains
    CUTFLOW(w,05,true,e_trigger);
    CUTFLOW(z,05,true,e_trigger);
    if ( !e_trigger && !AnaConfiguration::notrigger() ) {
      UNF_MISS();
      continue;
    }

    // THIS WAS THE LAST EVENT-LEVEL CUT THAT DOESN'T DEPEND ON SPECIFIC LEPTON/JET COLLECTIONS //

    // JETS
    size_t ijet=0;
#define FILLJET(col,calib,smear) col.push_back( jet->jetetmiss_calibrate( conf , jet , evnum , ijet , avgmu , vtx_calib_col.size() , is_mc , calib, smear ) );
#define FILLJET2(col,calib,smear) col.push_back( jet->jetetmiss_calibrate2( conf , jet , evnum , ijet , avgmu , vtx_calib_col.size() , is_mc , calib, smear ) );
    ijet = 0;
    // EM jets are used in analysis, so apply jet calibration + JER smearing (caljet)
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_topo_jets() ) {
      rawjet_em_nominal_col.push_back(jet);
      FILLJET2(caljet_em_nominal_col,true,0);
      if(is_mc) {
	// old, one-sided jer smearing
	FILLJET(rawjet_em_jer_col,false,true);
	FILLJET(caljet_em_jer_col,true,true);
	// new, two-sided jer smearing
	FILLJET2(rawjet_em_jerup_col,false,1);
	FILLJET2(caljet_em_jerup_col,true,1);
	FILLJET2(rawjet_em_jerdown_col,false,-1);
	FILLJET2(caljet_em_jerdown_col,true,-1);
      }
      ijet++;
    }
    // LC jets are used for MET recomputation, so apply only JER smearing (rawjet)
    ijet=0;
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , evt->atlas_anti_kt_4_h1_lc_topo_jets() ) {
      rawjet_lc_nominal_col.push_back(jet);
      FILLJET2(caljet_lc_nominal_col,true,0);
      if(is_mc) {
	// old, one-sided jer smearing
	FILLJET(rawjet_lc_jer_col,false,true);
	FILLJET(caljet_lc_jer_col,true,true);
	// new, two-sided jer smearing
	FILLJET2(rawjet_lc_jerup_col,false,1);
	FILLJET2(caljet_lc_jerup_col,true,1);
	FILLJET2(rawjet_lc_jerdown_col,false,-1);
	FILLJET2(caljet_lc_jerdown_col,true,-1);
      }
      ijet++;
    }
#undef FILLJET
#undef FILLJET2
    
    // For MultiJESUncertainty: select a subset of raw jets with pt>7GeV
#define J7G(x) dgSTL::copy_if( x## _nominal_col.begin(), x## _nominal_col.end(), back_inserter(x## _7gev_col),CUT_ALL(AnaJet,jet_7gev_cut) );
    J7G(rawjet_em);
    J7G(rawjet_lc);
    J7G(caljet_em);
    J7G(caljet_lc);
#undef J7G
    
    // apply JES uncertainty
    const int njets_jes = MULTIJET_JES ? 2 : 1;
#define FILLJET(x) for(unsigned ijet=0; ijet < x## _nominal_col.size(); ijet++) { \
      const boost::shared_ptr<const AnaJet>& jet = x## _nominal_col[ijet]; \
      const double drmin = jet_drmin( jet , x## _7gev_col );		\
      x## _jesup_col.push_back( jet->jetetmiss_scale( conf , jet , evnum , njets_jes , drmin, ijet , avgmu , vtx_calib_col.size() , "JESUP" ) ); \
      x## _jesdown_col.push_back( jet->jetetmiss_scale( conf , jet , evnum , njets_jes , drmin, ijet , avgmu , vtx_calib_col.size() , "JESDOWN" ) ); \
      x## _npvup_col.push_back( jet->jetetmiss_scale( conf , jet , evnum , njets_jes , drmin, ijet , avgmu , vtx_calib_col.size() , "NPVUP" ) ); \
      x## _npvdown_col.push_back( jet->jetetmiss_scale( conf , jet , evnum , njets_jes , drmin, ijet , avgmu , vtx_calib_col.size() , "NPVDOWN" ) ); \
      x## _muup_col.push_back( jet->jetetmiss_scale( conf , jet , evnum , njets_jes , drmin, ijet , avgmu , vtx_calib_col.size() , "MUUP" ) ); \
      x## _mudown_col.push_back( jet->jetetmiss_scale( conf , jet , evnum , njets_jes , drmin, ijet , avgmu , vtx_calib_col.size() , "MUDOWN" ) ); \
    }
    if(is_mc) {
      FILLJET(rawjet_em);
      FILLJET(rawjet_lc);
      FILLJET(caljet_em);
      FILLJET(caljet_lc);
    }
#undef FILLJET

    // Apply "bad" and "final" jet cuts - only for EM jets, since LC jets have to be used pre-cuts
    // This is all repeated twice: using original, and re-calibrated jets
#define SELJET(x,y) dgSTL::copy_if( rawjet_em_ ##x## _col.begin(),rawjet_em_ ##x## _col.end(),back_inserter(rawjet_em_ ##x## _ ##y## _col),CUT_ALL(AnaJet,jet_ ##y## _cut) ); \
    dgSTL::copy_if( caljet_em_ ##x## _col.begin(),caljet_em_ ##x## _col.end(),back_inserter(caljet_em_ ##x## _ ##y## _col),CUT_ALL(AnaJet,jet_ ##y## _cut) )
    // final jet cuts:
    SELJET(nominal,final);
    SELJET(jer,final);
    SELJET(jerup,final);
    SELJET(jerdown,final);
    SELJET(jesup,final);
    SELJET(jesdown,final);
    SELJET(npvup,final);
    SELJET(npvdown,final);
    SELJET(muup,final);
    SELJET(mudown,final);
    // bad jet cuts - for met cleaning / lar hole veto:
    SELJET(nominal,bad);
    SELJET(jer,bad);
    SELJET(jerup,bad);
    SELJET(jerdown,bad);
    SELJET(jesup,bad);
    SELJET(jesdown,bad);
    SELJET(npvup,bad);
    SELJET(npvdown,bad);
    SELJET(muup,bad);
    SELJET(mudown,bad);
#undef SELJET

    // MUONS
    // STACO vs MuID
    dgSTL::copy_if( evt->muons().begin(),evt->muons().end(),back_inserter(mu_class_col),CUT_ALL(AnaMuon,mu_class_cut) );
    // muon smearing/scaling
    size_t imuon = 0;
#define FILLMU(col,scale,fun) mu_ ##col## _mcp_col.push_back( AnaMuon::mcp_smeared(conf,AnaConfiguration::data_range(), \
								  muon,AnaConfiguration::muon_type(), \
								  evnum,imuon,scale,fun) )
    BOOST_FOREACH( boost::shared_ptr<const AnaMuon>& muon , mu_class_col ) {
      if(is_data) {
	FILLMU(raw,0,"NONE");
	FILLMU(datascaled,-3,"NOMINAL");
      }
      if(is_mc) {
	FILLMU(smeared,3,"NOMINAL");
	FILLMU(raw,0,"NONE");
	FILLMU(unscaled,0,"NOMINAL"); //smeared but unscaled
	FILLMU(msup,3,"MSUP");
	FILLMU(msdown,3,"MSLOW");
	FILLMU(idup,3,"IDUP");
	FILLMU(iddown,3,"IDLOW");
	FILLMU(kup,3,"SCALEKUP");
	FILLMU(kdown,3,"SCALEKDOWN");
	FILLMU(cup,3,"SCALECUP");
	FILLMU(cdown,3,"SCALECDOWN");
      }
      imuon++;
    }
#undef FILLMU
    
    // muon preselection and final full selection
#define SELMU(x) dgSTL::copy_if(mu_ ##x## _mcp_col.begin(),mu_ ##x## _mcp_col.end(),back_inserter(mu_ ##x## _type_col),CUT_ALL(AnaMuon,mu_type_cut)); \
    dgSTL::copy_if(mu_ ##x## _type_col.begin(),mu_ ##x## _type_col.end(),back_inserter(mu_ ##x## _qual_col),CUT_ALL(AnaMuon,mu_qual_cut)); \
    dgSTL::copy_if(mu_ ##x## _qual_col.begin(),mu_ ##x## _qual_col.end(),back_inserter(mu_ ##x## _ntuple_col),CUT_ALL(AnaMuon,mu_ntuple_cut)); \
    dgSTL::copy_if(mu_ ##x## _ntuple_col.begin(),mu_ ##x## _ntuple_col.end(),back_inserter(mu_ ##x## _pt_col),CUT_ALL(AnaMuon,mu_pt_cut)); \
    dgSTL::copy_if(mu_ ##x## _pt_col.begin(),mu_ ##x## _pt_col.end(),back_inserter(mu_ ##x## _eta_col),CUT_ALL(AnaMuon,mu_eta_cut)); \
    dgSTL::copy_if(mu_ ##x## _eta_col.begin(),mu_ ##x## _eta_col.end(),back_inserter(mu_ ##x## _qcd40_col),CUT_ALL(AnaMuon,mu_qcd40_cut)); \
    dgSTL::copy_if(mu_ ##x## _eta_col.begin(),mu_ ##x## _eta_col.end(),back_inserter(mu_ ##x## _qcd20_col),CUT_ALL(AnaMuon,mu_qcd20_cut)); \
    dgSTL::copy_if(mu_ ##x## _eta_col.begin(),mu_ ##x## _eta_col.end(),back_inserter(mu_ ##x## _qcdwind20_col),CUT_ALL(AnaMuon,mu_qcdwind20_cut)); \
    dgSTL::copy_if(mu_ ##x## _eta_col.begin(),mu_ ##x## _eta_col.end(),back_inserter(mu_ ##x## _qcdwind20m_col),CUT_ALL(AnaMuon,mu_qcdwind20m_cut)); \
    dgSTL::copy_if(mu_ ##x## _eta_col.begin(),mu_ ##x## _eta_col.end(),back_inserter(mu_ ##x## _qcdwind40_col),CUT_ALL(AnaMuon,mu_qcdwind40_cut)); \
    dgSTL::copy_if(mu_ ##x## _eta_col.begin(),mu_ ##x## _eta_col.end(),back_inserter(mu_ ##x## _qcdd0_col),CUT_ALL(AnaMuon,mu_qcdd0_cut)); \
    dgSTL::copy_if(mu_ ##x## _eta_col.begin(),mu_ ##x## _eta_col.end(),back_inserter(mu_ ##x## _iso_col),CUT_ALL(AnaMuon,mu_iso_cut)); \
    dgSTL::copy_if(mu_ ##x## _eta_col.begin(),mu_ ##x## _eta_col.end(),back_inserter(mu_ ##x## _isoloose_col),CUT_ALL(AnaMuon,mu_isoloose_cut));
    if(is_data) {
      SELMU(raw);
      SELMU(datascaled);
    }
    if(is_mc) {
      SELMU(smeared);
      SELMU(raw);
      SELMU(unscaled);
      SELMU(msup);
      SELMU(msdown);
      SELMU(idup);
      SELMU(iddown);
      SELMU(kup);
      SELMU(kdown);
      SELMU(cup);
      SELMU(cdown);
    }
#undef SELMU

    // MET recomputation for various systematics
    dgSTL::copy_if( evt->missing_et().begin(),evt->missing_et().end(),back_inserter(met_ref_col),CUT_ALL(AnaMET,met_ref_cut) );
    dgSTL::copy_if( evt->missing_et().begin(),evt->missing_et().end(),back_inserter(met_truth_nonint_col),CUT_ALL(AnaMET,met_truth_nonint_cut) );
    assert( met_ref_col.size()==1 );
    boost::shared_ptr<const AnaMET> met( met_ref_col[0] );
    const bool have_nonint = !met_truth_nonint_col.empty();
    metutil->reset();
    float MeV = 1000.;
    // set truth-level met terms (for new softmet systematics)
    metutil->setNvtx(nvtxsoftmet);
    // update 05/10/2013: use avgmu instead of nvtx
    metutil->setAverageIntPerXing(avgmu);
    if(have_nonint) {
      metutil->setMETTerm(METUtil::Truth, met->truth_nonint_etx()*MeV , met->truth_nonint_ety()*MeV , met->truth_nonint_sumet()*MeV );
    }
    // set reco-level met terms
    metutil->setMETTerm( METUtil::RefGamma , met->refgamma_etx()*MeV , met->refgamma_ety()*MeV , met->refgamma_sumet()*MeV );
    metutil->setMETTerm( METUtil::RefTau , met->reftau_etx()*MeV , met->reftau_ety()*MeV , met->reftau_sumet()*MeV );
    metutil->setMETTerm( METUtil::CellOutEflow , met->cellout_eflow_etx()*MeV , met->cellout_eflow_ety()*MeV , met->cellout_eflow_sumet()*MeV );
    metutil->setMETTerm( METUtil::MuonTotal , met->muonboy_etx()*MeV , met->muonboy_ety()*MeV , met->muonboy_sumet()*MeV );
    metutil->setMETTerm( METUtil::CellOut , met->cellout_etx()*MeV , met->cellout_ety()*MeV , met->cellout_sumet()*MeV );
    metutil->setMETTerm( METUtil::RefMuon , met->refmuon_etx()*MeV , met->refmuon_ety()*MeV , met->refmuon_sumet()*MeV );
    metutil->setMETTerm( METUtil::SoftJets , met->softjets_etx()*MeV , met->softjets_ety()*MeV , met->softjets_sumet()*MeV );

    /* re-compute MET terms. Note that the macro always forces recomputation of the jet component!
    jetetmiss_met( const boost::shared_ptr<const AnaMET>& met , METUTILCLASS *metutil ,
		 const elIterT& el_begin , const elIterT& el_end , const bool& reset_el ,
		 const muIterT& mu_begin , const muIterT& mu_end , const bool& reset_mu ,
		 const jetIterT& jet_begin , const jetIterT& jet_end , const bool& reset_jet ,
		 const METUTILTERM& term , const METUTILSYS& sys ) 
    */
#define FILLMET(x,ele,eleT,mu,muT,jet,jetT,sys) met_rawjet_ ##x## _col.push_back( AnaMET::jetetmiss_met(met,metutil,ele.begin() , ele.end() , eleT, mu.begin() , mu.end() , muT, rawjet_lc_ ##jet## _col.begin() , rawjet_lc_ ##jet## _col.end() , true , METUtil::RefFinal , sys ) ); \
    met_caljet_ ##x## _col.push_back( AnaMET::jetetmiss_met(met,metutil,ele.begin() , ele.end() , eleT, mu.begin() , mu.end() , muT, caljet_lc_ ##jet## _col.begin() , caljet_lc_ ##jet## _col.end() , true , METUtil::RefFinal , sys ) )
    
    FILLMET(nominal,evt->electrons(),true,evt->muons(),true,nominal,true,METUtil::None);
    if(is_mc) {
      if(false) { // only for debugging MET variations: disable propagation of all other objects.
	FILLMET(mcp_smeared,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::None); // nominal for MC
      } else {
	FILLMET(mcp_smeared,evt->electrons(),false,mu_smeared_mcp_col,true,nominal,false,METUtil::None); // nominal for MC
      }
      // new met uncertainties
      FILLMET(resosofttermsup,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ResoSoftTermsUp_ptHard);
      FILLMET(resosofttermsdown,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ResoSoftTermsDown_ptHard);
      FILLMET(resosofttermsupdown,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ResoSoftTermsUpDown_ptHard);
      FILLMET(resosofttermsdownup,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ResoSoftTermsDownUp_ptHard);
      FILLMET(scalesofttermsup,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ScaleSoftTermsUp_ptHard);
      FILLMET(scalesofttermsdown,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ScaleSoftTermsDown_ptHard);
      if(false){
	std::cout << "AKK: " << met_rawjet_nominal_col[0]->pt() << " " 
		  << met_rawjet_scalesofttermsup_col[0]->pt() << " "
		  << met_rawjet_scalesofttermsdown_col[0]->pt() << " " 
		  << endl;
      }
      // old met uncertainties
      if(false) {
	FILLMET(resosoftup,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ResoSoftTermsUp);
	FILLMET(resosoftdown,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ResoSoftTermsDown);
	FILLMET(scalesoftup,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ScaleSoftTermsUp);
	FILLMET(scalesoftdown,evt->electrons(),false,mu_smeared_mcp_col,false,nominal,false,METUtil::ScaleSoftTermsDown);
      }
      // mcp uncertainties
      FILLMET(mcp_unscaled,evt->electrons(),false,mu_unscaled_mcp_col,true,nominal,false,METUtil::None);
      FILLMET(mcp_msup,evt->electrons(),false,mu_msup_mcp_col,true,nominal,false,METUtil::None);
      FILLMET(mcp_msdown,evt->electrons(),false,mu_msdown_mcp_col,true,nominal,false,METUtil::None);
      FILLMET(mcp_idup,evt->electrons(),false,mu_idup_mcp_col,true,nominal,false,METUtil::None);
      FILLMET(mcp_iddown,evt->electrons(),false,mu_iddown_mcp_col,true,nominal,false,METUtil::None);
      FILLMET(mcp_kup,evt->electrons(),false,mu_kup_mcp_col,true,nominal,false,METUtil::None);
      FILLMET(mcp_kdown,evt->electrons(),false,mu_kdown_mcp_col,true,nominal,false,METUtil::None);
      FILLMET(mcp_cup,evt->electrons(),false,mu_cup_mcp_col,true,nominal,false,METUtil::None);
      FILLMET(mcp_cdown,evt->electrons(),false,mu_cdown_mcp_col,true,nominal,false,METUtil::None);
      // jet uncertainties
      FILLMET(jer,evt->electrons(),false,mu_smeared_mcp_col,true,jer,true,METUtil::None);
      FILLMET(jerup,evt->electrons(),false,mu_smeared_mcp_col,true,jerup,true,METUtil::None);
      FILLMET(jerdown,evt->electrons(),false,mu_smeared_mcp_col,true,jerdown,true,METUtil::None);
      FILLMET(jesup,evt->electrons(),false,mu_smeared_mcp_col,false,jesup,true,METUtil::None);
      FILLMET(jesdown,evt->electrons(),false,mu_smeared_mcp_col,false,jesdown,true,METUtil::None);
      FILLMET(npvup,evt->electrons(),false,mu_smeared_mcp_col,false,npvup,true,METUtil::None);
      FILLMET(npvdown,evt->electrons(),false,mu_smeared_mcp_col,false,npvdown,true,METUtil::None);
      FILLMET(muup,evt->electrons(),false,mu_smeared_mcp_col,false,muup,true,METUtil::None);
      FILLMET(mudown,evt->electrons(),false,mu_smeared_mcp_col,false,mudown,true,METUtil::None);
    }
#undef FILLMET    

    if( true ) {
      dg::down("METUtil","MET recalculation plots");
      const float ntuple_met_pt = met->pt();
      const float rawjet_met_pt = met_rawjet_nominal_col.front()->pt();
      const float caljet_met_pt = met_caljet_nominal_col.front()->pt();
      if(have_nonint) dg::fillh( "truth_nonint" , 200 , 0 , 100 , met->truth_nonint() );
      if(truth_met_pt>=0) { // met resolution (wrt. truth)
	dg::fillh( "truth_rawjet_res" , 200 , -1 , 1 , truth_met_pt - rawjet_met_pt);
	dg::fillh( "truth_caljet_res" , 200 , -20 , 20 , truth_met_pt - caljet_met_pt);
      }
      dg::fillh( "rawjet_caljet_res" , 200 , -20 , 20 , rawjet_met_pt - caljet_met_pt);
      dg::fillh( "ntuple_rawjet_res" , 200 , -1 , 1 , ntuple_met_pt - rawjet_met_pt); // should be identical
      dg::fillh( "ntuple_caljet_res" , 200 , -20 , 20 , ntuple_met_pt - caljet_met_pt);
      // later: first two may soon be removed, since we only care about differences with respect to true "nominal" (MCP smeared/scaled)
#define FILLRES(x) dg::fillh( "rawjet_" #x "_res" , 200 , -20 , 20 , rawjet_met_pt - met_rawjet_ ##x## _col.front()->pt() ); \
      dg::fillh( "caljet_" #x "_res" , 200 , -20 , 20 , caljet_met_pt - met_caljet_ ##x## _col.front()->pt() ); \
      dg::fillh( "mcp_rawjet_" #x "_res" , 200 , -20 , 20 , mcp_smeared_rawjet_met_pt - met_rawjet_ ##x## _col.front()->pt() ); \
      dg::fillh( "mcp_caljet_" #x "_res" , 200 , -20 , 20 , mcp_smeared_caljet_met_pt - met_caljet_ ##x## _col.front()->pt() )
      if(is_mc) {
	const float mcp_smeared_rawjet_met_pt = met_rawjet_mcp_smeared_col.front()->pt();
	const float mcp_smeared_caljet_met_pt = met_caljet_mcp_smeared_col.front()->pt();
	FILLRES(mcp_msup);
	FILLRES(mcp_msdown);
	FILLRES(mcp_idup);
	FILLRES(mcp_iddown);
	FILLRES(mcp_kup);
	FILLRES(mcp_kdown);
	FILLRES(mcp_cup);
	FILLRES(mcp_cdown);
	FILLRES(jer);
	FILLRES(jerup);
	FILLRES(jerdown);
	FILLRES(jesup);
	FILLRES(jesdown);
	FILLRES(npvup);
	FILLRES(npvdown);
	FILLRES(muup);
	FILLRES(mudown);
	//new met
	FILLRES(resosofttermsup);
	FILLRES(resosofttermsdown);
	FILLRES(resosofttermsupdown);
	FILLRES(resosofttermsdownup);
	FILLRES(scalesofttermsup);
	FILLRES(scalesofttermsdown);
	// old met
	if(false) {
	  FILLRES(resosoftup);
	  FILLRES(resosoftdown);
	  FILLRES(scalesoftup);
	  FILLRES(scalesoftdown);
	}
      }
#undef FILLRES
      dg::up();
    }

    // DEBUG: MET scale variations (with Adrian)
    bool runsel = evnum==104701 || evnum==104702 || evnum==104710 || evnum==104721 || evnum==104725 || evnum==104730 || evnum==104731 || evnum==104738 || evnum==104739 || evnum==104744 || evnum==104745 || evnum==104747 || evnum==104750 || evnum==104904;
    //runsel = evnum==104730;  // debugging the event where scaleUp/Down are NOT symmetric around nominal
    if(false && AnaConfiguration::verbose() && is_mc && runsel) {
      std::cout << "===============================================" << std::endl;
      std::cout << "RunNumber = " << raw_rnum << std::endl;
      std::cout << "EventNumber = " << evnum << std::endl;
      std::cout.precision(1);
      std::cout << std::fixed;
      std::cout << "Met in D3PD = " << met->pt()*1000.0 << std::endl;
      std::cout << "Nominal re-calculated Met = " << met_rawjet_nominal_col.front()->pt()*1000.0 << std::endl;
      std::cout << "Smeared Met = " << met_rawjet_mcp_smeared_col.front()->pt()*1000.0 << std::endl;
      std::cout << "SoftTerm pt-hard scale up  = " << met_rawjet_scalesofttermsup_col.front()->pt()*1000.0 << std::endl;
      std::cout << "SoftTerm pt-hard scale down  = " << met_rawjet_scalesofttermsdown_col.front()->pt()*1000.0 << std::endl;
      std::cout << "SoftTerm pt-hard reso up  = " << met_rawjet_resosofttermsup_col.front()->pt()*1000.0 << std::endl;
      std::cout << "SoftTerm pt-hard reso down  = " << met_rawjet_resosofttermsdown_col.front()->pt()*1000.0 << std::endl;
    }

    // DEBUG: stat scale factors with Max Bellomo (10 or 10k replicas only!)
    if(true && AnaConfiguration::verbose() && is_mc && n_events<2000 && mu_smeared_iso_col.size()==1 ) {
      std::cout << "======  EVENT: " << evnum << " " << rnum << " TYPE=" << mu_smeared_type_col.size() << " QUAL=" << mu_smeared_qual_col.size()
	 	<< " ETA=" << mu_smeared_eta_col.size() << " ISO=" << mu_smeared_iso_col.size() 
		<< " LPT=" << mu_smeared_iso_col[0]->pt() << std::endl;
      // this loop only includes stat. replicas
      for(unsigned int ik=0; ik<unf_keys_rep.size(); ik++) {
	unfdata_type::iterator it2 = unfdata.find(unf_keys_rep[ik]);
	assert( it2 != unfdata.end() );
	assert( it2->second.Reco_Weight_Replicas.size() == NREPLICASF );
	double _eff = 1.0;
	double _a=0 , _b=0;
	// default (replica-less)
	const bool SFRECO = (it2->first == "SFUncorrReco");
	const bool SFTRIG = (it2->first == "SFUncorrTrig");
	const bool SFISOL = (it2->first == "SFUncorrIsol");
	if(!SFRECO) continue;
	if(SFRECO) mu_eff_scale( mu_smeared_iso_col , EFFVEC , _eff, _a , _b );
	if(SFTRIG) mu_trig_scale( mu_smeared_iso_col , _eff , _a , 20);
	if(SFISOL) mu_isol_scale( mu_smeared_iso_col , _eff , _a , 0);
	std::cout << "SFDBG " << 0 << " : " << _eff << " +/- " << _a << " +/- " << _b << std::endl;
	std::string pl = TString::Format("pt=%.1f eta=%.2f",mu_smeared_iso_col[0]->pt(),mu_smeared_iso_col[0]->eta()).Data();
	TCanvas cc("cc","cc",1024,400);
	TH1D h1("histo",TString::Format("%s: %s",pl.c_str(),it2->first.c_str()).Data(),100,_eff-0.1,_eff+0.1);
	// compute efficiency scale factors on final selected muons, for each replica
	for(int replica=0; replica<it2->second.Reco_Weight_Replicas.size(); replica++) {
	  if( SFRECO ) {
	    h1.Fill(EFFVEC[replica]);
	  }
	  if (SFTRIG) {
	    mu_trig_scale( mu_smeared_iso_col , _eff , _a , 20 , replica);
	    h1.Fill(_eff);
	  }
	  if (SFISOL) {
	    mu_isol_scale( mu_smeared_iso_col , _eff , _a , 0, replica);
	    h1.Fill(_eff);
	  }
// 	  if(replica<10) {
// 	    std::cout << "R " << replica << " : " << _eff << std::endl;
// 	  }
	}
	if(n_events<100) {
	  cc.cd(); h1.Draw();
	  cc.SaveAs(TString::Format("results/EVENT_%d_%s.png",evnum,it2->first.c_str()).Data());
	}
      }
    }

    // default (MAX) isolation
#define STUDYDEF(TAG,nt,eff,unf,mu,jet,met) study_wz(TAG,nt,eff,unf,	\
						     mu_ ##mu## _type_col, mu_ ##mu## _ntuple_col, mu_ ##mu## _qual_col, \
						     mu_ ##mu## _pt_col, mu_ ##mu## _eta_col, mu_ ##mu## _iso_col, \
						     vtx_base_col, vtx_col, \
						     rawjet_em_nominal_bad_col, jet## _final_col, \
						     met_ ##met## _col,	\
						     st_w, st_z)
    // QCD templates: anti-isolated muons in data
    // {normal (cone40) anti-iso, loose (cone20) anti-iso, normal-window anti-iso}
#define STUDYQCD(TAG,mu,jet,met) study_wz("IsoFail40" TAG,0,0,0,	\
					  mu_ ##mu## _type_col, mu_ ##mu## _ntuple_col, mu_ ##mu## _qual_col, \
					  mu_ ##mu## _pt_col, mu_ ##mu## _eta_col, mu_ ##mu## _qcd40_col, \
					  vtx_base_col, vtx_col,	\
					  rawjet_em_nominal_bad_col, jet## _final_col, \
					  met_ ##met## _col,		\
					  st_w, st_z);			\
    study_wz("IsoWind20" TAG,0,0,0,					\
	     mu_ ##mu## _type_col, mu_ ##mu## _ntuple_col, mu_ ##mu## _qual_col, \
	     mu_ ##mu## _pt_col, mu_ ##mu## _eta_col, mu_ ##mu## _qcdwind20_col, \
	     vtx_base_col, vtx_col,					\
	     rawjet_em_nominal_bad_col, jet## _final_col,		\
	     met_ ##met## _col,						\
	     st_w, st_z);						\
    study_wz("IsoWind20m" TAG,0,0,0,					\
	     mu_ ##mu## _type_col, mu_ ##mu## _ntuple_col, mu_ ##mu## _qual_col, \
	     mu_ ##mu## _pt_col, mu_ ##mu## _eta_col, mu_ ##mu## _qcdwind20m_col, \
	     vtx_base_col, vtx_col,					\
	     rawjet_em_nominal_bad_col, jet## _final_col,		\
	     met_ ##met## _col,						\
	     st_w, st_z);						\
    study_wz("IsoWind40" TAG,0,0,0,					\
	     mu_ ##mu## _type_col, mu_ ##mu## _ntuple_col, mu_ ##mu## _qual_col, \
	     mu_ ##mu## _pt_col, mu_ ##mu## _eta_col, mu_ ##mu## _qcdwind40_col, \
	     vtx_base_col, vtx_col,					\
	     rawjet_em_nominal_bad_col, jet## _final_col,		\
	     met_ ##met## _col,						\
	     st_w, st_z)
//     study_wz("IsoFail20" TAG,0,0,0,					\
// 	     mu_ ##mu## _type_col, mu_ ##mu## _ntuple_col, mu_ ##mu## _qual_col, \
// 	     mu_ ##mu## _pt_col, mu_ ##mu## _eta_col, mu_ ##mu## _qcd20_col, \
// 	     vtx_base_col, vtx_col,					\
// 	     rawjet_em_nominal_bad_col, jet## _final_col,		\
// 	     met_ ##met## _col,						\
// 	     st_w, st_z);						\
//     study_wz("IsoD0" TAG,0,0,0,						\
// 	     mu_ ##mu## _type_col, mu_ ##mu## _ntuple_col, mu_ ##mu## _qual_col, \
// 	     mu_ ##mu## _pt_col, mu_ ##mu## _eta_col, mu_ ##mu## _qcdd0_col, \
// 	     vtx_base_col, vtx_col,					\
// 	     rawjet_em_nominal_bad_col, jet## _final_col,		\
// 	     met_ ##met## _col,						\
// 	     st_w, st_z)
// abbreviated version (only nominal)
#define STUDYQCS(TAG,mu,jet,met) study_wz("IsoWind20" TAG,0,0,0,	\
					  mu_ ##mu## _type_col, mu_ ##mu## _ntuple_col, mu_ ##mu## _qual_col, \
					  mu_ ##mu## _pt_col, mu_ ##mu## _eta_col, mu_ ##mu## _qcdwind20_col, \
					  vtx_base_col, vtx_col,	\
					  rawjet_em_nominal_bad_col, jet## _final_col, \
					  met_ ##met## _col,		\
					  st_w, st_z)

    // bootstrap replicas if requested
    if(RUN_BOOTSTRAP) dg::generate_bootstrap(raw_rnum,evnum);

    // regular histograms and ntuples
    if(is_data) {
      // nominal
      st_w.run_qcd_slices(RUN_QCD_SLICES);
      st_w.run_debug_studies(true);
      st_w.do_bootstrap(RUN_BOOTSTRAP);
      STUDYDEF("Nominal",save_ntuples&0x2,0,0,raw,caljet_em_nominal,rawjet_nominal);
      st_w.do_bootstrap(false);
      // anti-isolation qcd templates
      st_w.run_debug_studies(true);
      STUDYQCD("",raw,caljet_em_nominal,rawjet_nominal);
      st_w.run_debug_studies(false);
      // split into nmu subcategories
      if(!NOMONLY) {
	if(rnum<=pK[1]) {
	  STUDYDEF("NominalDtoK",0,0,0,raw,caljet_em_nominal,rawjet_nominal);
	  STUDYQCS("DtoK",             raw,caljet_em_nominal,rawjet_nominal);
	} else {
	  assert(rnum >= pL[0]);
	  STUDYDEF("NominalLtoM",0,0,0,raw,caljet_em_nominal,rawjet_nominal);
	  STUDYQCS("LtoM",             raw,caljet_em_nominal,rawjet_nominal);
	}
      }
      if(STUDY_CALJET && !NOMONLY) {
	STUDYDEF("NominalCalJet",0,0,0,raw,caljet_em_nominal,caljet_nominal);
      }
    }
    if(is_mc) {
      // nominal
      st_w.run_qcd_slices(RUN_QCD_SLICES);
      st_w.run_debug_studies(true);
      st_w.run_anyfit_studies(true);
      STUDYDEF("Nominal",save_ntuples&0x4,1,1,smeared,caljet_em_nominal,rawjet_mcp_smeared);
      st_w.run_anyfit_studies(false);
      st_w.run_debug_studies(true);
      STUDYQCD("",smeared,caljet_em_nominal,rawjet_mcp_smeared); // for background-subtraction from QCD template
      st_w.run_debug_studies(false);
      // completely raw (out of the box) muons for cutflow
      if(false) {
	STUDYDEF("Cutflow",0,0,0,raw,caljet_em_nominal,rawjet_nominal);
      }
      // systematic variations
      if(!NOMONLY) {
	// split into nmu subcategories
	if(rnum<=pK[1]) {
	  STUDYDEF("NominalDtoK",0,0,0,smeared,caljet_em_nominal,rawjet_mcp_smeared);
	  STUDYQCS("DtoK",             smeared,caljet_em_nominal,rawjet_mcp_smeared);
	} else {
	  assert(rnum >= pL[0]);
	  STUDYDEF("NominalLtoM",0,0,0,smeared,caljet_em_nominal,rawjet_mcp_smeared);
	  STUDYQCS("LtoM",             smeared,caljet_em_nominal,rawjet_mcp_smeared);
	}
	STUDYDEF("Rawmet",0,                0,1,smeared,caljet_em_nominal,rawjet_nominal); // without MET recalculation
	st_w.run_anyfit_studies(true);
	STUDYDEF("ResoSoftTermsUp_ptHard",0,0,1,smeared,caljet_em_nominal,rawjet_resosofttermsup);
	STUDYDEF("ResoSoftTermsDown_ptHard",0,0,1,smeared,caljet_em_nominal,rawjet_resosofttermsdown);
	//STUDYDEF("ResoSoftTermsUpDown_ptHard",0,0,1,smeared,caljet_em_nominal,rawjet_resosofttermsupdown);
	//STUDYDEF("ResoSoftTermsDownUp_ptHard",0,0,1,smeared,caljet_em_nominal,rawjet_resosofttermsdownup);
	STUDYDEF("ScaleSoftTermsUp_ptHard",0,0,1,smeared,caljet_em_nominal,rawjet_scalesofttermsup);
	STUDYDEF("ScaleSoftTermsDown_ptHard",0,0,1,smeared,caljet_em_nominal,rawjet_scalesofttermsdown);
	st_w.run_anyfit_studies(false);
	// old met
	if(false) {
	  STUDYDEF("ResoSoftTermsUp",0,0,1,smeared,caljet_em_nominal,rawjet_resosoftup);
	  STUDYDEF("ResoSoftTermsDown",0,0,1,smeared,caljet_em_nominal,rawjet_resosoftdown);
	  STUDYDEF("ScaleSoftTermsUp",0,0,1,smeared,caljet_em_nominal,rawjet_scalesoftup);
	  STUDYDEF("ScaleSoftTermsDown",0,0,1,smeared,caljet_em_nominal,rawjet_scalesoftdown);
	}
	// jer/jes
	STUDYDEF("JetResol",0,0,1,smeared,caljet_em_jer,rawjet_jer);
	STUDYDEF("JetResolUp",0,0,1,smeared,caljet_em_jerup,rawjet_jerup);
	STUDYDEF("JetResolDown",0,0,1,smeared,caljet_em_jerdown,rawjet_jerdown);
	STUDYDEF("JetScaleUp",0,0,1,smeared,caljet_em_jesup,rawjet_jesup);
	STUDYDEF("JetScaleDown",0,0,1,smeared,caljet_em_jesdown,rawjet_jesdown);
	// new mid-2012 jes variations (pileup uncertainty etc)
	STUDYDEF("JetNPVUp",0,0,1,smeared,caljet_em_npvup,rawjet_npvup);
	STUDYDEF("JetNPVDown",0,0,1,smeared,caljet_em_npvdown,rawjet_npvdown);
	STUDYDEF("JetMUUp",0,0,1,smeared,caljet_em_muup,rawjet_muup);
	STUDYDEF("JetMUDown",0,0,1,smeared,caljet_em_mudown,rawjet_mudown);
	if(STUDY_CALJET) {
	  STUDYDEF("NominalCalJet",0,0,1,smeared,caljet_em_nominal,caljet_mcp_smeared);
	  STUDYDEF("JetResolCalJet",0,0,1,smeared,caljet_em_jer,caljet_jer);
	  STUDYDEF("JetResolUpCalJet",0,0,1,smeared,caljet_em_jerup,caljet_jerup);
	  STUDYDEF("JetResolDownCalJet",0,0,1,smeared,caljet_em_jerdown,caljet_jerdown);
	  STUDYDEF("JetScaleUpCalJet",0,0,1,smeared,caljet_em_jesup,caljet_jesup);
	  STUDYDEF("JetScaleDownCalJet",0,0,1,smeared,caljet_em_jesdown,caljet_jesdown);
	  STUDYDEF("JetNPVUpCalJet",0,0,1,smeared,caljet_em_npvup,caljet_npvup);
	  STUDYDEF("JetNPVDownCalJet",0,0,1,smeared,caljet_em_npvdown,caljet_npvdown);
	  STUDYDEF("JetMUUpCalJet",0,0,1,smeared,caljet_em_muup,caljet_muup);
	  STUDYDEF("JetMUDownCalJet",0,0,1,smeared,caljet_em_mudown,caljet_mudown);
	}

	// mcp
	STUDYDEF("MuonNoScale",0,0,1,unscaled,caljet_em_nominal,rawjet_mcp_unscaled); // smeared/unscaled, but WITH met propagation
	STUDYDEF("MuonResMSUp",0,0,1,msup,caljet_em_nominal,rawjet_mcp_msup);
	STUDYDEF("MuonResMSDown",0,0,1,msdown,caljet_em_nominal,rawjet_mcp_msdown);
	STUDYDEF("MuonResIDUp",0,0,1,idup,caljet_em_nominal,rawjet_mcp_idup);
	STUDYDEF("MuonResIDDown",0,0,1,iddown,caljet_em_nominal,rawjet_mcp_iddown);
	STUDYDEF("MuonKScaleUp",0,0,1,kup,caljet_em_nominal,rawjet_mcp_kup);
	STUDYDEF("MuonKScaleDown",0,0,1,kdown,caljet_em_nominal,rawjet_mcp_kdown);
	STUDYDEF("MuonCScaleUp",0,0,1,cup,caljet_em_nominal,rawjet_mcp_cup);
	STUDYDEF("MuonCScaleDown",0,0,1,cdown,caljet_em_nominal,rawjet_mcp_cdown);
      } // end systematics
    } // end if_mc

    //special flag to control MCP scale studies
    const bool MCPNT = (AnaConfiguration::muon_scale()==999);
    if(is_data) STUDYDEF("datascaled",MCPNT,0,0,datascaled,rawjet_em_nominal,ref); // for Z scale: data-scaled muons; no MET propagation
    if(is_mc) STUDYDEF("smeared_unscaled",MCPNT,0,0,unscaled,rawjet_em_nominal,ref); // for Z scale: smeared but unscaled muons; no MET propagation

    dg::set_global_weight( 1. );
  } // end for each event

  // SAVE ROOT DATA
  AnaEventMgr::instance()->close_sample();
  dg::save( false && AnaConfiguration::verbose() );

  // SAVE UNFOLDING DATA
  if(is_unfold) {
    funfold->cd();
    for(unfdata_type::iterator it = unfdata.begin(); it!=unfdata.end(); it++) { // loop over systematics
      UNFDATA &d = it->second;
      if(d.tree) d.tree->FlushBaskets();
      for(unsigned int i=0; i<d.response.size(); i++) {
	d.response[i].write();
      }
    }
    funfold->Write();
//     funfold->Close();
//     delete funfold;
  }
  
  std::cout << "==================== FINISHED MAIN ====================" << std::endl;
  std::cout.flush();
  if( ! AnaConfiguration::noexit() ) std::_Exit(0);

  delete[] EFFVEC;
  delete[] TRIVEC;
  delete[] ISOVEC;

  return 0;
}

// complete study of w's and z's from a given muon and met collection
void study_wz(std::string label, bool do_ntuples, bool do_eff, int do_unf,
	      MUCOL& mu_type_col, MUCOL& mu_ntuple_col, MUCOL& mu_qual_col,
	      MUCOL& mu_pt_col, MUCOL& mu_eta_col, MUCOL& mu_iso_col,
	      VXCOL& vtx_base_col, VXCOL& vtx_col,
	      JETCOL& badjet_in_col, JETCOL& jet_in_col,
	      METCOL& met_col,
	      StudyWAsymmetry& st_w,
	      StudyZAsymmetry& st_z) {
  dg::down( label , "W study" ); BOOST_SCOPE_EXIT() { dg::up(); } BOOST_SCOPE_EXIT_END;
  const bool is_nominal = (label=="Nominal");
  
  // perform jet overlap removal and jet/met cleaning.
  // overlap removal uses em level phi/eta
  std::vector< boost::shared_ptr<const AnaJet> > jet_col , badjet_col;
  jet_mu_overlap_removal( jet_in_col , mu_iso_col , 0.3 , jet_col );
  jet_mu_overlap_removal( badjet_in_col , mu_iso_col , 0.3 , badjet_col );

  // MET cleaning wiki recommends using calibrated jets with a 20 GeV cut.
  const bool e_met_cleaning = std::count_if( badjet_col.begin(),
					     badjet_col.end(),
					     bind(&AnaJet::isBadJet_looser_v17,_1)==true && bind(&AnaJet::pt,_1)>=20 )==0;
  const bool e_lar_hole = !jet_larhole_veto( badjet_col.begin() , badjet_col.end() );

  // some useful initializations
  AnaZCrossSectionCandidate::Compose2<MUCOL::const_iterator,METCOL::const_iterator,JETCOL::const_iterator,VXCOL::const_iterator >
    z_compositor( mu_iso_col.begin() , mu_iso_col.end() ,
		  met_col.begin() , met_col.end() ,
		  jet_col.begin() , jet_col.end() ,
		  vtx_col.begin() , vtx_col.end() ,
		  vtx_base_col.begin() , vtx_base_col.end() 
		  ) ;
  
  AnaWCrossSectionCandidate::Compose2<MUCOL::const_iterator,METCOL::const_iterator,JETCOL::const_iterator,VXCOL::const_iterator >
    w_compositor( mu_iso_col.begin() , mu_iso_col.end() ,
		  met_col.begin() , met_col.end() ,
		  jet_col.begin() , jet_col.end() ,
		  vtx_col.begin() , vtx_col.end() ,
		  vtx_base_col.begin() , vtx_base_col.end() 
		  ) ;

#define MAKE_W(col)  std::vector<boost::shared_ptr<const AnaW> > anaw_all_col; \
  std::vector<boost::shared_ptr<const AnaWCrossSectionCandidate> > w_all_col; \
  wgen.generate(  mu_ ##col## _col.begin() , mu_ ##col## _col.end() , \
		  met_col.begin() , met_col.end() , \
		  AnaW::Compose() , anaw_all_col ); \
  std::transform( anaw_all_col.begin() , anaw_all_col.end() , \
		  back_inserter(w_all_col) , \
		  bind( w_compositor , _1 ) )

#define MAKE_Z(col) std::vector<boost::shared_ptr<const AnaZ> > anaz_all_col; \
  std::vector<boost::shared_ptr<const AnaZCrossSectionCandidate> > z_all_col; \
  zgen.generate(  mu_ ##col## _col.begin() , mu_ ##col## _col.end() , \
		  mu_ ##col## _col.begin() , mu_ ##col## _col.end() , \
		  AnaZ::Compose() , anaz_all_col ); \
  std::transform( anaz_all_col.begin() , anaz_all_col.end() , \
		  back_inserter(z_all_col) , \
		  bind( z_compositor , _1 ) )

  // compute muon efficiency
  double eff = 1.0;
  double effphi = 0.0;   // dual meaning: either phi-binned systematic, OR: calo probe systematic (which deprecates eff_errstat)
  double eff_errstat = 0.0;
  double eff_errsys = 0.0;
  double trig = 1.0;
  double trig_errstat = 0.0;
  double trigall = 1.0;  // multiplies trigger SF of each muon (i.e. ALL are required to fire, as opposed to an OR)
  double trigphi = 1.0;  // systematic: addition of phi on top
  double mcp = 1.0;      // MCP weights
  double mcpall = 1.0;   // multiplies trigger SF of each muon (i.e. ALL are required to fire, as opposed to an OR)
  double trig_dummy = 0.0;
  double siso = 1.0;
  double sisoup = 1.0;
  double sisodown = 1.0;
  double siso_err = 0.0;
  if(is_mc) {
    // compute efficiency scale factors on final selected muons
    // (which happens to be "iso" muons - whether isolated or anti-isolated
    // original version, before splitting ID vs CaloTag probes
    //mu_eff_scale( mu_iso_col , EFFVEC , effphi, eff_errstat , eff_errsys , 1); // eta x phi
    //mu_eff_scale( mu_iso_col , EFFVEC , eff, eff_errstat , eff_errsys , 0); // (nominal) eta
    mu_eff_scale( mu_iso_col , EFFVEC , effphi, eff_errstat , eff_errsys , 11); // eta x phi, calo probe (systematic)
    mu_eff_scale( mu_iso_col , EFFVEC , eff, eff_errstat , eff_errsys , 10); // (nominal) eta x phi, id probe
    mu_trig_scale( mu_iso_col , trig    , trig_errstat   , 20);
    mu_trig_scale( mu_iso_col , trigphi , trig_dummy , 21);
    mu_trig_scale( mu_iso_col , trigall , trig_dummy , 22);
    if(true) { // mcp
      mu_trig_scale( mu_iso_col , mcp    , trig_dummy   , 10);
      mu_trig_scale( mu_iso_col , mcpall , trig_dummy , 12);
    }
    mu_isol_scale( mu_iso_col , sisoup , siso_err , 2);
    mu_isol_scale( mu_iso_col , sisodown , siso_err , 1);
    mu_isol_scale( mu_iso_col , siso , siso_err , 0);
  }
  dg::event_info().set_eff_weight(eff,eff_errstat,eff_errsys);
  dg::event_info().set_effphi_weight(effphi);
  dg::event_info().set_trig_weight(trig,trig_errstat);
  dg::event_info().set_trigall_weight(trigall);
  dg::event_info().set_trigphi_weight(trigphi);
  dg::event_info().set_mcp_weight(mcp);
  dg::event_info().set_mcpall_weight(mcpall);
  dg::event_info().set_iso_weight(siso,siso_err);

  // define modified weights to account for systematic SF variations
  const int NRECKEYS = 10;
  assert( (unf_keys_rec.size() == NRECKEYS) || NOMONLY);
  double total_rec[NRECKEYS];
  total_rec[0] = event_weight[0]*(eff+eff_errsys)*trig*siso;
  total_rec[1] = event_weight[0]*(eff-eff_errsys)*trig*siso;
  total_rec[2] = event_weight[0]*(effphi)*trig*siso;
  total_rec[3] = event_weight[0]*(eff+eff_errstat)*trig*siso;
  total_rec[4] = event_weight[0]*(eff-eff_errstat)*trig*siso;
  total_rec[5] = event_weight[0]*eff*(trigphi)*siso;
  total_rec[6] = event_weight[0]*eff*(trig+trig_errstat)*siso;
  total_rec[7] = event_weight[0]*eff*(trig-trig_errstat)*siso;
  //total_rec[-1] = event_weight[0]*(effphi)*(trigphi)*siso;
  total_rec[8] = event_weight[0]*eff*trig*(sisoup);
  total_rec[9] = event_weight[0]*eff*trig*(sisodown);

  // prepare unfolding data structure
  unfdata_type::iterator it = unfdata.find(label);
  if( do_unf && (is_unfold||is_mc) ) {
    if( it == unfdata.end() ) {
      std::cerr << "Missing declaration for the unfolding block: " << label << std::endl;
    }
    assert( it != unfdata.end() ); //sanity check
    // reset all unfrec pointers (from last event)
    it->second.unfrec.reset();
    if(do_eff) {
      assert(is_nominal && "ERROR: efficiency variations are only possible for Nominal");
      for(unsigned int ik=0; ik<unf_keys_nom.size(); ik++) {
	unfdata_type::iterator it2 = unfdata.find(unf_keys_nom[ik]);
	assert(it2!=unfdata.end());
	it2->second.unfrec.reset();
      }
    }
  }

  // W cutflow
  bool w_reco_fid = false;
  dg::set_global_weight( event_weight[0] );
  const bool w_type = (mu_type_col.size()>=1);
  CUTFLOW(w,06,true,w_type);
  if(w_type) {
    const bool w_qual = (mu_qual_col.size()>=1);
    CUTFLOW(w,07,true,w_qual);
    if(w_qual) {
      const bool w_pt = (mu_pt_col.size()>=1);
      CUTFLOW(w,08,true,w_pt);
      if(w_pt) {
	const bool w_eta = (mu_eta_col.size()>=1);
	CUTFLOW(w,09,true,w_eta);
	if(w_eta) {
	  const bool w_iso = (mu_iso_col.size()>=1);
	  CUTFLOW(w,10,true,w_iso);
	  if(w_iso) {
	    // muon selections finished; apply event selections
	    const bool w_nmuons = (mu_iso_col.size()<2); // NOTE: used to be "mu_eta_col"
	    CUTFLOW(w,11,true,w_nmuons);
	    if(w_nmuons) {
	      if(false && label=="Nominal") {
		std::cout.precision(12);
		std::cout << "ANTON" << " " << evnum << " " << event_weight[0] << " " << event_weight[0]*eff*trig*siso 
			  << " " << eff << " " << trig << " " << siso << std::endl;
	      }
	      dg::set_global_weight( event_weight[0]*eff*trig*siso ); // apply efficiency correction after nmuons cut
	      const bool w_metclean = e_met_cleaning;
	      CUTFLOW(w,12,true,w_metclean);
	      if(w_metclean) {
		const bool w_larhole = e_lar_hole;
		CUTFLOW(w,13,true,w_larhole);
		if(w_larhole) {
		  // make w's (before MET and w_mt cuts)
		  MAKE_W(iso);
		  const bool w_met = std::count_if( w_all_col.begin(),w_all_col.end(),bind(&AnaWCrossSectionCandidate::asym_met,_1)==true);
		  CUTFLOW(w,14,true,w_met);
		  if(w_met) {
		    const bool w_mt = std::count_if( w_all_col.begin(),w_all_col.end(),
						     bind(&AnaWCrossSectionCandidate::asym_wmt,_1) && bind(&AnaWCrossSectionCandidate::asym_met,_1) == true);
		    CUTFLOW(w,15,true,w_mt);
		    if(w_mt) {
		      const bool w_eta_trig = std::count_if( w_all_col.begin(),w_all_col.end(),
							     bind(&AnaWCrossSectionCandidate::asym_wmt,_1)
							     && bind(&AnaWCrossSectionCandidate::asym_met,_1)
							     && bind(&AnaWCrossSectionCandidate::asym_eta_trig,_1) );
		      // CUTFLOW(w,16,true,w_eta_trig);
		      if(true) {
			w_reco_fid = true;
			const bool w_trig = std::count_if( w_all_col.begin(),w_all_col.end(),
							   bind(&AnaWCrossSectionCandidate::asym_wmt,_1)
							   && bind(&AnaWCrossSectionCandidate::asym_met,_1) 
							   && bind(&AnaWCrossSectionCandidate::asym_trigger,_1) );
			CUTFLOW(w,16,true,w_trig); // not used anymore in nominal cutflow!
			const bool w_pos = std::count_if( w_all_col.begin(),w_all_col.end(),
							  bind(&AnaWCrossSectionCandidate::asym_wmt,_1) 
							  && bind(&AnaWCrossSectionCandidate::asym_met,_1)
							  && bind(&AnaWCrossSectionCandidate::scharge,_1)>0 );
			const bool w_neg = std::count_if( w_all_col.begin(),w_all_col.end(),
							  bind(&AnaWCrossSectionCandidate::asym_wmt,_1)
							  && bind(&AnaWCrossSectionCandidate::asym_met,_1) 
							  && bind(&AnaWCrossSectionCandidate::scharge,_1)<0 );
			CUTFLOW(w,17,true,w_pos);
			CUTFLOW(w,18,true,w_neg);
		      }
		    }
		  }
		}
	      }
	    }
	  } // w_iso
	}
      }
    }
  }
  // total reconstruction efficiency with respect to fiducial truth volume.
  // Note: event-wide cuts (e.g., trigger) are not taken into account here! For that, you need to use unfolding ntuples
  if(do_unf && is_unfold) {
    CUTFLOW(w,19,it->second.Truth_Is_Fiducial,w_reco_fid);
  }
 
  // Z cutflow [ not needed ]
  if(RUNZ && false) {
    dg::set_global_weight( event_weight[0] );
    const bool z_type = (mu_type_col.size()>=2);
    CUTFLOW(z,06,true,z_type);
    if(z_type) {
      const bool z_qual = (mu_qual_col.size()>=2);
      CUTFLOW(z,07,true,z_qual);
      if(z_qual) {
	const bool z_pt = (mu_pt_col.size()>=2);
	CUTFLOW(z,08,true,z_pt);
	if(z_pt) {
	  const bool z_eta = (mu_eta_col.size()>=2);
	  CUTFLOW(z,09,true,z_eta);
	  if(z_eta) {
	    const bool z_iso = (mu_iso_col.size()>=2);
	    CUTFLOW(z,10,true,z_iso);
	    if(z_iso) {
	      // muon selections finished; apply event selections
	      const bool z_nmuons = (mu_iso_col.size()<3); // require exactly two isolated muons
	      CUTFLOW(z,11,true,z_nmuons);
	      if(z_nmuons) {
		dg::set_global_weight( event_weight[0]*eff*trig*siso ); // apply efficiency correction after nmuons cut
		const bool z_metclean = e_met_cleaning;
		CUTFLOW(z,12,true,z_metclean);
		if(z_metclean) {
		  const bool z_larhole = e_lar_hole;
		  CUTFLOW(z,13,true,z_larhole);
		  if(z_larhole) {
		    // make Z's (before opsign and z_m cuts)
		    MAKE_Z(iso);
		    const bool z_opsign = std::count_if( z_all_col.begin(),z_all_col.end(),bind(&AnaZCrossSectionCandidate::asym_opsign,_1)==true );
		    CUTFLOW(z,14,true,z_opsign);
		    if(z_opsign) {
		      const bool z_m = std::count_if( z_all_col.begin(),z_all_col.end(),
						      bind(&AnaZCrossSectionCandidate::asym_zwindow,_1) && bind(&AnaZCrossSectionCandidate::asym_opsign,_1) == true);
		      CUTFLOW(z,15,true,z_m);
		      if(z_m) {
			const bool z_trig = std::count_if( z_all_col.begin(),z_all_col.end(),
							   bind(&AnaZCrossSectionCandidate::asym_zwindow,_1) && bind(&AnaZCrossSectionCandidate::asym_opsign,_1)
							   && bind(&AnaZCrossSectionCandidate::asym_trigger,_1)==true);
			CUTFLOW(z,16,true,z_trig);
		      }
		    }
		  }
		}
	      }
	    } // z_iso
	  }
	}
      }
    }
  }

  // now we enter the histogram-making territory - apply all efficiency scale factors
  dg::set_global_weight( event_weight[0]*eff*trig*siso );

  // W ntuples
  if(e_met_cleaning && e_lar_hole && mu_ntuple_col.size()>=1 ) {
    MAKE_W(ntuple);
    // study w's
    st_w.do_save_ntuple(do_ntuples);
    st_w.study_categorized(false);
    st_w.repurpose( "st_w_final" , "W event candidates after full selection" );
    st_w.for_each( w_all_col.begin() , w_all_col.end() );
  }

  // W histograms and unfolding
  if(e_met_cleaning && e_lar_hole && mu_iso_col.size()>=1 ) {
    std::map< std::string , bool > wchk; // keeps track of weights that have been set
    MAKE_W(iso);
    // study w's
    st_w.do_save_ntuple(false);
    st_w.study_categorized(true);
    st_w.repurpose( "st_w_final" , "W event candidates after full selection" );
    st_w.for_each( w_all_col.begin() , w_all_col.end() );
    // disable debug studies for all subsequent plots
    st_w.run_debug_studies(false);

    // begin {unfolding exercise}
    if(do_unf && (is_unfold||is_mc)) {
      assert(is_mc && "ERROR: unfolding can only be done on signal MC");
      std::vector<boost::shared_ptr<const AnaWCrossSectionCandidate> > w_tmp_col;
      w_tmp_col.clear(); dgSTL::copy_if( w_all_col.begin() , w_all_col.end() , back_inserter(w_tmp_col) , bind(&AnaWCrossSectionCandidate::asym_baseline_quick,_1)==true );
      const bool good = (w_tmp_col.size()==1);
      if (true) {
	// nominal
	it->second.Reco_Weight = dg::get_global_weight();
	if(!it->second.Reco_Weight_Replicas.empty()) {
	  it->second.Reco_Weight_Replicas[0] = it->second.Reco_Weight;
	}
	if(good) {
	  it->second.Reco_isReconstructed = true;
	  it->second.unfrec = w_tmp_col[0];
	  it->second.Reco_PtW = w_tmp_col[0]->pt();
	  it->second.Reco_PtLep = L(w_tmp_col[0]->lepton_pt());
	  it->second.Reco_EtaLep = w_tmp_col[0]->lepton_eta();
	}
	// efficiency scale factor variations [NOMINAL only]
	if(do_eff) {
	  assert(is_nominal && "do_eff requires is_nominal");
	  // this loop includes all systematic sub-variations of Nominal
	  for(unsigned int ik=0; ik<unf_keys_nom.size(); ik++) {
	    unfdata_type::iterator it2 = unfdata.find(unf_keys_nom[ik]);
	    assert(it2 != unfdata.end());
	    assert( wchk.find(it2->first) == wchk.end()); wchk[ it2->first ] = true;
	    it2->second.Reco_Weight = dg::get_global_weight();              // to be over-ridden
	    if(!it2->second.Reco_Weight_Replicas.empty()) {
	      it2->second.Reco_Weight_Replicas[0] = it2->second.Reco_Weight;  // to be over-ridden
	    }
	    if(good) {
	      it2->second.Reco_isReconstructed = true;
	      it2->second.unfrec = w_tmp_col[0];
	      it2->second.Reco_PtW = w_tmp_col[0]->pt();
	      it2->second.Reco_PtLep = L(w_tmp_col[0]->lepton_pt());
	      it2->second.Reco_EtaLep = w_tmp_col[0]->lepton_eta();
	    }
	  }
	  // this loop only includes truth sf variations (pu(2), wpt(2), wpo(2), pp_to_data(1), PDF)
	  assert((unf_keys_tru.size()==7+pdf_offset) || NOMONLY);
	  for(unsigned int ik=0; ik<unf_keys_tru.size(); ik++) {
	    unfdata_type::iterator it2 = unfdata.find(unf_keys_tru[ik]);
	    assert(it2 != unfdata.end());
	    assert( it2->second.Reco_Weight_Replicas.size() == 1 );
	    // compute efficiency scale factors on final selected muons, for each replica
	    it2->second.Reco_Weight =  event_weight[1+ik]*eff*trig*siso;
	    it2->second.Reco_Weight_Replicas[0] =  event_weight[1+ik]*eff*trig*siso;
	  }
	  // this loop only includes stat. replicas
	  for(unsigned int ik=0; ik<unf_keys_rep.size(); ik++) {
	    unfdata_type::iterator it2 = unfdata.find(unf_keys_rep[ik]);
	    assert( it2 != unfdata.end() );
	    assert( it2->first.find("SFUncorr") != std::string::npos );
	    assert( it2->second.Reco_Weight_Replicas.size() == NREPLICASF );
	    const bool SFALL  = (it2->first == "SFUncorr");
	    const bool SFRECO = (it2->first == "SFUncorrReco");
	    const bool SFTRIG = (it2->first == "SFUncorrTrig");
	    const bool SFISOL = (it2->first == "SFUncorrIsol");
	    assert(SFALL || SFRECO || SFTRIG || SFISOL);
	    double _eff = 1.0;
	    double _trig = 1.0;
	    double _siso = 1.0;
	    double _a , _b;
	    // compute efficiency scale factors on final selected muons, for each replica
	    for(int replica=0; replica<it2->second.Reco_Weight_Replicas.size(); replica++) {
	      _eff = EFFVEC[replica];
	      mu_trig_scale( mu_iso_col , _trig , _a , 20 , replica);
	      mu_isol_scale( mu_iso_col , _siso , _a , 0, replica);
	      if(SFALL) it2->second.Reco_Weight_Replicas[replica] =        event_weight[0] *_eff *_trig *_siso ;
	      else if(SFRECO) it2->second.Reco_Weight_Replicas[replica] =  event_weight[0] *_eff * trig * siso ;
	      else if(SFTRIG) it2->second.Reco_Weight_Replicas[replica] =  event_weight[0] * eff *_trig * siso ;
	      else if(SFISOL) it2->second.Reco_Weight_Replicas[replica] =  event_weight[0] * eff * trig *_siso ;
	    }
	  }
	  // this loop only includes CT10 member sets
	  for(unsigned int ik=0; ik<unf_keys_pdf.size(); ik++) {
	    unfdata_type::iterator it2 = unfdata.find(unf_keys_pdf[ik]);
	    assert( it2 != unfdata.end() );
	    assert(it2->first == "PDFMEM");
	    assert( it2->second.Reco_Weight_Replicas.size() == NREPLICASP );
	    // compute efficiency scale factors on final selected muons, for each replica
	    for(int replica=0; replica<it2->second.Reco_Weight_Replicas.size(); replica++) {
	      it2->second.Reco_Weight_Replicas[replica] =  event_weight[1+NWPU+NWPT+NWPO+NWDA+pdf_offset+replica]*eff*trig*siso ;
	    }
	  }
	}
      }
    }    // end {unfolding exercise}

    // systematic variations of efficiency/trigger scale factors
    // (for nominal only)
#define UNF_TR(x) do {							\
      assert( unfdata.find(x) != unfdata.end() );			\
      UNFDATA &dcur = unfdata.find(x)->second;				\
      assert( wchk.find(x) != wchk.end());				\
      dcur.Reco_Weight = dg::get_global_weight();			\
      assert( dcur.Reco_Weight_Replicas.size()==1 && "Too many SF replicas"); \
      dcur.Reco_Weight_Replicas[0] = dg::get_global_weight();		\
    } while(false)
    if(do_eff && !NOMONLY) {
      assert(is_nominal);
      // optional section to save tru SF variations [ PDF families; Wpt; puw ]
      for(int ii=0; ii<unf_keys_tru.size(); ii++) {
	unfdata_type::iterator it2 = unfdata.find(unf_keys_tru[ii]);
	assert(it2!=unfdata.end());
	if( wchk.find(it2->first) == wchk.end() ) {
	  std::cerr << "ERROR: weight missing for: " << label << " - " << unf_keys_tru[ii] << std::endl;
	}
	assert( wchk.find(it2->first) != wchk.end());
	st_w.repurpose( unf_keys_tru[ii] , "W" );
	dg::set_global_weight( it2->second.Reco_Weight );
	st_w.for_each( w_all_col.begin() , w_all_col.end() );
      }
      // optional section to save PDF member variations (for MET)
      if(true) {
	for(int ii=0; ii<unf_keys_pdf.size(); ii++) {
	  unfdata_type::iterator it2 = unfdata.find(unf_keys_pdf[ii]);
	  assert( it2 != unfdata.end() );
	  assert( wchk.find(it2->first) != wchk.end());
	  for(int member=0; member<it2->second.Reco_Weight_Replicas.size(); member++) {
	    std::string tmpname = it2->first + std::string( TString::Format("%d",member).Data() );
	    st_w.repurpose( tmpname , "W" );
	    dg::set_global_weight( it2->second.Reco_Weight_Replicas[member] );
	    st_w.for_each( w_all_col.begin() , w_all_col.end() );
	  }
	}
      }
      // optional section to save reco SF variations
      for(int ii=0; ii<unf_keys_rec.size(); ii++) {
	st_w.repurpose( unf_keys_rec[ii] , "W" );
	dg::set_global_weight( total_rec[ii] );
	if(do_unf && is_unfold && !NOMONLY) UNF_TR(unf_keys_rec[ii]);
	st_w.for_each( w_all_col.begin() , w_all_col.end() );
      }
      // revert all weights to nominal:
      dg::set_global_weight( event_weight[0]*eff*trig*siso );
    }
#undef UNF_TR
  }
  // Fill ALL unfolding matrices (may be more than one if Nominal)
  if(do_unf && is_unfold) {
    UNF_FILL(label);
    if(do_eff && !NOMONLY) {
      // loop over all variations within Nominal
      for(unsigned int ik=0; ik<unf_keys_nom.size(); ik++) {
	UNF_FILL( unf_keys_nom[ik] );
      }
    }
  }

  // Z ntuples
  dg::set_global_weight( event_weight[0]*eff*trig*siso );
  if(RUNZ_NT && e_met_cleaning && e_lar_hole && mu_ntuple_col.size()>=2 ) {
    MAKE_Z(ntuple);
    // study z's
    st_z.do_save_ntuple(do_ntuples && RUNZ_NT);
    st_z.study_categorized(false);
    st_z.repurpose( "st_z_final" , "Z event candidates after full selection" );
    st_z.for_each( z_all_col.begin() , z_all_col.end() );
  }

  // Z histograms - only if do_ntuples is set (i.e., nominal case only)
  dg::set_global_weight( event_weight[0]*eff*trig*siso );
  if(RUNZ && e_met_cleaning && e_lar_hole && mu_iso_col.size()>=2 && do_ntuples) {
    MAKE_Z(iso);
    // study z's
    st_z.do_save_ntuple(false);
    st_z.study_categorized(true);
    st_z.repurpose( "st_z_final" , "Z event candidates after full selection" );
    st_z.for_each( z_all_col.begin() , z_all_col.end() );
    if(true && is_mc) { // add additional phi-correction plots
      // reco phi
      dg::set_global_weight( total_rec[2] );
      st_z.repurpose( "st_z_effphi" , "Z event candidates after full selection" );
      st_z.for_each( z_all_col.begin() , z_all_col.end() );
    }
    if(false && is_mc) { // add additional phi-correction plots
      // trig phi
      dg::set_global_weight( total_rec[3] );
      st_z.repurpose( "st_z_trigphi" , "Z event candidates after full selection" );
      st_z.for_each( z_all_col.begin() , z_all_col.end() );
      // reco+trig phi
//       dg::set_global_weight( total_rec[4] );
//       st_z.repurpose( "st_z_efftrigphi" , "Z event candidates after full selection" );
//       st_z.for_each( z_all_col.begin() , z_all_col.end() );
    }
  }

  dg::set_global_weight( event_weight[0]*eff*trig*siso );
  // Special event quantities
  dg::fillh( "avgmu" , 30 , 0 , 30 , avgmu , "average_mu" );
  dg::fillhw( "avgmu_unw" , 30,0,30, avgmu , 1.0 , "average_mu" );
  dg::fillh( "actmu" , 30 , 0 , 30 , actmu , "actual_mu" );
  dg::fillhw( "actmu_unw" , 30,0,30, actmu , 1.0 , "actual_mu" );

  return;
}

bool jet_mu_dr_compare( const boost::shared_ptr<const AnaMuon>& mu , boost::shared_ptr<const AnaJet> a , boost::shared_ptr<const AnaJet> b ) {
  return ( detector::delta_eta_phi( a->em_scale_eta() , a->em_scale_phi() , mu->eta() , mu->phi() ) >
	   detector::delta_eta_phi( b->em_scale_eta() , b->em_scale_phi() , mu->eta() , mu->phi() ) );
}

void jet_mu_overlap_removal( JETCOL& jetcoll , MUCOL& mucoll , const float& dr , std::vector< boost::shared_ptr<const AnaJet> >& output_col ) {
  std::copy( jetcoll.begin() , jetcoll.end() , back_inserter(output_col) );
  if( jetcoll.size()==0 ) return;
  if( mucoll.size()==0 ) return;
  BOOST_FOREACH( boost::shared_ptr<const AnaMuon>& mu , mucoll ) {
    // sort functor: true if first argument goes before. so here, largest-first sort order
    std::sort( output_col.begin() , output_col.end() , bind(&jet_mu_dr_compare,mu,_1,_2) );
    if( detector::delta_eta_phi( output_col.back()->em_scale_eta() , output_col.back()->em_scale_phi() , mu->eta() , mu->phi() ) < dr ) {
      output_col.pop_back();
      if( output_col.size()==0 ) return;
    }
  }
}

bool jet_tp_dr_compare( const boost::shared_ptr<const AnaParticle>& mu , boost::shared_ptr<const AnaJet> a , boost::shared_ptr<const AnaJet> b ) {
  return ( detector::delta_eta_phi( a->eta() , a->phi() , mu->eta() , mu->phi() ) >
	   detector::delta_eta_phi( b->eta() , b->phi() , mu->eta() , mu->phi() ) );
}

void jet_tp_overlap_removal( JETCOL& jetcoll , TPCOL& mucoll , const float& dr , std::vector< boost::shared_ptr<const AnaJet> >& output_col ) {
  std::copy( jetcoll.begin() , jetcoll.end() , back_inserter(output_col) );
  if( jetcoll.size()==0 ) return;
  if( mucoll.size()==0 ) return;
  BOOST_FOREACH( boost::shared_ptr<const AnaTruthParticle>& mu , mucoll ) {
    // sort functor: true if first argument goes before. so here, largest-first sort order
    std::sort( output_col.begin() , output_col.end() , bind(&jet_tp_dr_compare,mu,_1,_2) );
    if( detector::delta_eta_phi( output_col.back()->eta() , output_col.back()->phi() , mu->eta() , mu->phi() ) < dr ) {
      output_col.pop_back();
      if( output_col.size()==0 ) return;
    }
  }
}

/* MC channel numbers for heavy-flavor MC */
bool is_heavy_flavor(unsigned int chan) {
  if(chan>=117293 && chan<=117297) return true; // wc
  if(chan>=107280 && chan<=107283) return true; // wbb
  if(chan>=117284 && chan<=117287) return true; // wcc
  if(chan>=109305 && chan<=109308) return true; // zmumubb
  return false;
}

/* Returns type of W Monte-Carlo */
MC_TYPE get_mc_type(unsigned int chan) {
  if(chan==106044 || chan==107054 || chan==106047 || chan==106052 || chan==108319) return MC_PYTHIA;
  // AlpgenHerwig + AlpgenPythia
  if(chan>=107690 && chan<=107695) return MC_ALPGEN_HERWIG; //wmunu
  if(chan>=107700 && chan<=107705) return MC_ALPGEN_HERWIG; //wtaunu
  if(chan>=107660 && chan<=107665) return MC_ALPGEN_HERWIG; //zmumu
  if(chan>=107670 && chan<=107675) return MC_ALPGEN_HERWIG; //ztautau
  if(chan>=117690 && chan<=117695) return MC_ALPGEN_PYTHIA;
  // AlpgenHerwig heavy flavor
  if(chan>=107280 && chan<=107283) return MC_ALPGEN_HERWIG;
  if(chan>=117284 && chan<=117287) return MC_ALPGEN_HERWIG;
  if(chan>=117293 && chan<=117297) return MC_ALPGEN_HERWIG;
  // 
  if(chan==106083 || chan==106084 || chan==106088 || chan==108328 || chan==108329 || chan==106062) return MC_MCNLO;
  if(chan==113185 || chan==113187 || chan==126007 || chan==107394 || chan==107395) return MC_POWHEG_HERWIG;
  if(chan==108301 || chan==108298 || chan==108304 || chan==107390 || chan==107391 || chan==107392) return MC_POWHEG_PYTHIA;
  if(chan==114612) return MC_SHERPA;
  return MC_NONE;
}

/* Returns MC status for truth-level W/Z boson, which is different depending on Monte-Carlo
   This is primarily used to facilitate w/z pT reweighting at truth level
*/
int get_wz_status(const MC_TYPE& mctype , const TRUTH_STATUS& mcstatus) {
  switch( mctype ) {
  case MC_PYTHIA:
  case MC_ALPGEN_PYTHIA:
  case MC_POWHEG_PYTHIA:
    return 3;
    break;
  case MC_MCNLO:
  case MC_ALPGEN_HERWIG:
  case MC_POWHEG_HERWIG:
    return 155;
    break;
  default:
    return 1; //general stable particle status
    break;
  }
  return 1;
}

/* Sets MC status and parent PDG_ID for truth-level muon or neutrino
   status = 3 or 123/124
   parent = mu PDG or nu PDG or boson PDG
*/
void get_munu_status_parent(bool is_w , const MC_TYPE& mctype , const TRUTH_STATUS& mcstatus, int& status, int& mu_parent, int& nu_parent) {
  status = -1;
  mu_parent = is_w ? PDG_W : PDG_Z;
  nu_parent = is_w ? PDG_W : PDG_Z;
  switch( mctype ) {
  case MC_PYTHIA:
  case MC_ALPGEN_PYTHIA:
  case MC_POWHEG_PYTHIA:
    switch (mcstatus) {
    case STABLE: status=1; break;
    case BORN:   status=3; break;
    default:     assert(0); break;
    }
    break;
  case MC_ALPGEN_HERWIG:
  case MC_POWHEG_HERWIG:
  case MC_MCNLO:
    switch (mcstatus) {
    case STABLE: status=1; mu_parent=PDG_MU; nu_parent=PDG_NU; break; // tricky, but not often used (we always use BORN nowadays)
    case BORN:   status=STATUS_123_124; break;
    default:     assert(0); break;
    }
    break;
  default: assert(0); break;
  }
  assert(status!=-1);
  return;
}

const std::string& get_wz_reweight_source(const MC_TYPE& mctype) {
  static const std::string MC0("");
  static const std::string MC1("McAtNloMC11");
  static const std::string MC2("PowhegPythiaMC11");
  static const std::string MC3("PowhegHerwigMC11");
  static const std::string MC4("PythiaMC11");
  switch( mctype ) {
  case MC_PYTHIA:
    return MC4;
    break;
  case MC_MCNLO:
    return MC1;
    break;
  case MC_POWHEG_HERWIG:
    return MC3;
    break;
  case MC_POWHEG_PYTHIA:
    return MC2;
    break;
  default:
    return MC0; //general unknown case
    break;
  }
  return MC0;
}

const VertexPositionReweightingTool::eDataMCBeamspot get_mc_beamspot(const std::string& fname) {
  std::map<std::string,VertexPositionReweightingTool::eDataMCBeamspot> tagMC;
  // MC11 tags
  // primary sim tags
  tagMC["s1272"] = VertexPositionReweightingTool::MC11wide;
  tagMC["s1273"] = VertexPositionReweightingTool::MC11wide;
  tagMC["s1299"] = VertexPositionReweightingTool::MC11narrow;
  tagMC["s1310"] = VertexPositionReweightingTool::MC11narrow;
  tagMC["s1309"] = VertexPositionReweightingTool::MC11narrow;
  tagMC["s1372"] = VertexPositionReweightingTool::MC11narrow;
  tagMC["s1378"] = VertexPositionReweightingTool::MC11narrow;
  // merge tags are probably also clear
  tagMC["s1274"] = VertexPositionReweightingTool::MC11wide;
  tagMC["s1300"] = VertexPositionReweightingTool::MC11narrow;
  tagMC["s1353"] = VertexPositionReweightingTool::MC11narrow;
  tagMC["s1370"] = VertexPositionReweightingTool::MC11narrow;
  // AFII is always narrow?
  tagMC["a131"] = VertexPositionReweightingTool::MC11narrow;
  std::map<std::string, VertexPositionReweightingTool::eDataMCBeamspot>::iterator it;
  for (it = tagMC.begin() ; it != tagMC.end(); it++) {
    // std::cout << (*it).first << " => " << (*it).second << std::endl;
    if (fname.find((*it).first) != std::string::npos) {
      return (*it).second;
    }
  }
  std::cout << "WARNING: DataMCBeamspot returns unknown MC beam width. Defaulting to narrow" << std::endl;
  assert(0&&"Please include dataset name in the full path to FlatNtuple ntuples");
  return VertexPositionReweightingTool::MC11narrow;
}

// Remap gluons to quarks for LineShapeTool reweighting
// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZGroupInclusiveMeasurement2011#Event_weights_to_apply
// https://twiki.cern.ch/twiki/pub/AtlasProtected/WZGroupInclusiveMeasurement2011/pdfinfo_lineshape.txt
void pdfinfo_lineshape_quarks(int& qid1, int& qid2, const int wz_pdgid) {
  // double gluon -> just put u ubar/u dbar/ubar d (this will over/underestimate some corrections!)
  if (qid1 == 21 && qid2 == 21) {
    if (wz_pdgid == 23) {
      // Z/gamma*
      qid1 = 2;
      qid2 = -2;
    } else if (wz_pdgid == 24) {
      // W+
      qid1 = 2;
      qid2 = -1;
    } else if (wz_pdgid == -24) {
      // W-
      qid1 = -2;
      qid2 = 1;
    }
  } else if (qid1 == 21) {
    if (wz_pdgid == 23) {
      // Z/gamma*
      qid1 = -qid2;
    } else if (wz_pdgid == 24) {
      // W+
      qid1 = -(qid2-1);
    } else if (wz_pdgid == -24) {
      // W-
      qid1 = -(qid2+1);
    }
  } else if (qid2 == 21) {
    if (wz_pdgid == 23) {
      // Z/gamma*
      qid2 = -qid1;
    } else if (wz_pdgid == 24) {
      // W+
      qid2 = -(qid1-1);
    } else if (wz_pdgid == -24) {
      // W-
      qid2 = -(qid1+1);
    }
  }
}

// MC11 via https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/WZElectroweakCommonTopics2011
// https://twiki.cern.ch/twiki/bin/viewauth/AtlasProtected/HowToCleanJets2011#Definition
#include "TrigFTKAna/jetetmiss/LArHole_v2_00.hpp"
const int LARHOLE_MC_RNUM = 183003;
const int LARHOLE_DA_RNUM_MIN = 180614;
const int LARHOLE_DA_RNUM_MAX = 185352;
const float LARHOLE_PT_CUT = 20.0;
template< typename jetIterT >
const bool jet_larhole_veto( jetIterT jet_begin , jetIterT jet_end ) {
  if( is_mc && (raw_rnum != LARHOLE_MC_RNUM) ) { return false; }
  if( !is_mc && ( (raw_rnum < LARHOLE_DA_RNUM_MIN) || (raw_rnum >= LARHOLE_DA_RNUM_MAX) ) ) { return false; }
  for( std::vector< shared_ptr<const AnaJet> >::const_iterator ijet = jet_begin ; ijet != jet_end ; ++ijet ) {
    const shared_ptr<const AnaJet>& jet(*ijet);
    if( LArHole::simpleVeto( jet->pt() , jet->eta() , jet->phi() , jet->f_BCH_CORR_JET() , jet->f_BCH_CORR_CELL() , LARHOLE_PT_CUT ) ) { return true; }
  }
  return false;
}

const double jet_drmin( const boost::shared_ptr<const AnaJet>& jet , JETCOL& jetcoll )
{
  if( jetcoll.size()<2 ) return 0.;
  std::sort( jetcoll.begin() , jetcoll.end() , bind(&AnaParticle::dist_eta_phi,_1,jet) < bind(&AnaParticle::dist_eta_phi,_2,jet) );
  return double( jet->dist_eta_phi( jetcoll[1] ) );
}

void mu_eff_scale( MUCOL& mucoll , double *eff_weights,
		   double& eff_weight , double& eff_stat_error , double& eff_sys_error , int choice)
{
  std::fill(eff_weights, eff_weights+NREPLICASF, 1.0);
  eff_weight = 1.;
  eff_stat_error = 0.; // absolute errors
  eff_sys_error = 0.;
  for( std::vector< shared_ptr<const AnaMuon> >::const_iterator imu = mucoll.begin() ; imu != mucoll.end() ; ++imu ) {
    const shared_ptr<const AnaMuon>& mu(*imu);
    double wt , stat_err , sys_err;
    AnaMuon::mcp_effscale( AnaConfiguration::conf() , AnaConfiguration::data_range() , mu , mu_type , rnum , 0, int_lumi , run_periods , choice, eff_weights, wt , stat_err , sys_err );
    eff_weight *= wt;
    eff_stat_error += pow(stat_err/wt,2);
    eff_sys_error += pow(sys_err/wt,2);
  }
  eff_stat_error = eff_weight*sqrt(eff_stat_error);
  eff_sys_error = eff_weight*sqrt(eff_sys_error);
}

void mu_trig_scale( MUCOL& mucoll , double& trig_weight , double& trig_stat_error , int choice , int replica) {
  const detector::EGAMMA_TYPE el_type = detector::EGAMMA_MEDIUMPP;
  std::vector<TLorentzVector> velectrons; // should be empty
  std::vector<TLorentzVector> vmuons;
  std::vector<float> vmuons_charges;
  trig_weight = 1.0;
  trig_stat_error = 0.0;
  for( std::vector< shared_ptr<const AnaMuon> >::const_iterator imu = mucoll.begin() ; imu != mucoll.end() ; ++imu ) {
    vmuons.push_back( (*imu)->four_vector() );
    vmuons_charges.push_back( (*imu)->charge() );
  }
  if(choice<10) {  // using Max's custom scale factors
    AnaMuon::GetTriggerSF_v17_custom( AnaConfiguration::conf() , mu_type , el_type , vmuons , velectrons , vmuons_charges, 
				      rnum , 0 , trig_weight , trig_stat_error , choice);
  } else if (choice==10) { // official MCP scale factors
    AnaMuon::GetTriggerSF_v17( AnaConfiguration::conf() , mu_type , el_type , vmuons , velectrons ,
			       rnum , 0 , trig_weight , trig_stat_error );
  } else if (choice==12) { // same as choice==10 (MCP official), BUT taking a product of all per-muon weights
    for(int ii=0; ii<vmuons.size(); ii++) {
      double cweight = 1.0;
      double cerror = 0.0;
      std::vector<TLorentzVector> vmuon;
      std::vector<float> vmuon_charges;
      vmuon.push_back(vmuons[ii]);
      vmuon_charges.push_back(vmuons_charges[ii]);
      AnaMuon::GetTriggerSF_v17( AnaConfiguration::conf() , mu_type , el_type , vmuon , velectrons ,
					 rnum , 0 , cweight , cerror );
      trig_weight *= cweight;
    }
  } else if (choice==22) { // same as choice==20, BUT taking a product of all per-muon weights (for Z both-match)
    for(int ii=0; ii<vmuons.size(); ii++) {
      double cweight = 1.0;
      double cerror = 0.0;
      std::vector<TLorentzVector> vmuon;
      std::vector<float> vmuon_charges;
      vmuon.push_back(vmuons[ii]);
      vmuon_charges.push_back(vmuons_charges[ii]);
      AnaMuon::GetTriggerSF_v17_custom2( AnaConfiguration::conf() , mu_type , el_type , vmuon , velectrons , vmuon_charges, 
					 rnum , 0 , cweight , cerror , replica);
      trig_weight *= cweight;
    }
  } else if (choice==20 || choice==21) { // custom MCP scale factors from MCP + Max
    AnaMuon::GetTriggerSF_v17_custom2( AnaConfiguration::conf() , mu_type , el_type , vmuons , velectrons , vmuons_charges, 
				       rnum , 0 , trig_weight , trig_stat_error , replica);
    if(choice==21) { // apply eta-phi factors on top. As coded, this only works for single-mu events!!!
      double trig_weight_phi = 1.0;
      double trig_stat_error_phi = 0.0;
      AnaMuon::GetTriggerSF_v17_custom4( AnaConfiguration::conf() , mu_type , el_type , vmuons , velectrons , vmuons_charges, 
					 rnum , 0 , trig_weight_phi , trig_stat_error_phi);
      trig_weight *= trig_weight_phi;
    }
  }
}

void mu_isol_scale( MUCOL& mucoll , double& trig_weight , double& trig_stat_error , int choice, int replica) {
  std::vector<TLorentzVector> vmuons;
  trig_weight = 1.0;
  trig_stat_error = 0.0;
  for( std::vector< shared_ptr<const AnaMuon> >::const_iterator imu = mucoll.begin() ; imu != mucoll.end() ; ++imu ) {
    vmuons.push_back( (*imu)->four_vector() );
  }
  AnaMuon::GetIsolationSF_v17( AnaConfiguration::conf() , mu_type , vmuons , rnum , trig_weight , trig_stat_error , choice, replica );
}

bool getBosonBornLeptons(const std::vector< boost::shared_ptr<const AnaTruthParticle> >& col, long lepPdgCode,
			 boost::shared_ptr<const AnaTruthParticle>& v,
			 boost::shared_ptr<const AnaTruthParticle>& mu,
			 boost::shared_ptr<const AnaTruthParticle>& nu,
			 bool is_z) {
  v.reset(); mu.reset(); nu.reset();
  // pdg codes
  const int status_born = 3;
  long nuPdgCode=-1;
  long bosonPdgCode=-1;
  if(is_z) {
    nuPdgCode = -lepPdgCode;
    bosonPdgCode = 23;
  } else {
    nuPdgCode = abs(lepPdgCode)+1;
    nuPdgCode*=lepPdgCode>0 ? -1 : +1;
    bosonPdgCode = 24;
  }
  typedef std::vector< boost::shared_ptr<const AnaTruthParticle> > TCOL;
  for (unsigned i=0; i<col.size(); i++) {
    const boost::shared_ptr<const AnaTruthParticle>& p = col[i];
    if ( abs(p->pdg_id()) != bosonPdgCode ) continue;
    // derive Born-level status codes, which are nominally 3 for Pythia and 120/155 for Herwig
    Int_t lepStatusCodeHEP=0;
    Int_t lepStatusCodeHEP2=0;
    if(p->status()==3) { // For Pythia
      lepStatusCodeHEP=3;
    }
    else if(p->status()==62) { // For PowHeg-Pythia8 (MC12)
      lepStatusCodeHEP=3;
      lepStatusCodeHEP2=1;
    }
    else if(p->status()==120 || p->status()==155 // for MC@NLO single W,Z
	    || p->status()==195) { // For MC@NLO ZZ
      lepStatusCodeHEP=123;
      lepStatusCodeHEP2=124;
    } else {
      continue;
    }
    v = p;
    // find the children of this boson
    std::vector< boost::shared_ptr<const AnaTruthParticle> > cld;
    dgSTL::copy_if( col.begin(),col.end(),back_inserter(cld),
		    bind(&AnaTruthParticle::parent_index,_1)==i && (bind(&AnaTruthParticle::pdg_id,_1)==lepPdgCode || bind(&AnaTruthParticle::pdg_id,_1)==nuPdgCode) );
    // loop over children candidates
    for (unsigned j=0; j<cld.size(); j++) {
      boost::shared_ptr<const AnaTruthParticle> c = cld[j];
      // check for status code
      bool goodLepFound=false;
      if (c->status()==lepStatusCodeHEP) goodLepFound=true;
      else if(lepStatusCodeHEP2 && c->status()==lepStatusCodeHEP2) goodLepFound=true;
      // For Herwig, we need to loop over children-of-children of boson to check the status
      if(!goodLepFound) {
	std::vector< boost::shared_ptr<const AnaTruthParticle> > ccld;
	dgSTL::copy_if( col.begin(),col.end(),back_inserter(ccld),
			bind(&AnaTruthParticle::parent_index,_1)==j && (bind(&AnaTruthParticle::pdg_id,_1)==lepPdgCode || bind(&AnaTruthParticle::pdg_id,_1)==nuPdgCode) );
	for (unsigned int jj=0; jj<ccld.size(); jj++) {
	  const boost::shared_ptr<const AnaTruthParticle>& cc = ccld[jj];
	  if (cc->status()==lepStatusCodeHEP) {goodLepFound=true; c = cc; break;}
	  else if(lepStatusCodeHEP==123 && cc->status()==124) {goodLepFound=true; c = cc; break; }
	}
      }
      if(goodLepFound) {
	if (c->pdg_id()==lepPdgCode) mu = c;
	if (c->pdg_id()==nuPdgCode) nu = c;
	if(mu && nu) {
	  return true;
	}
      }
    }
  }
  return false;
}
