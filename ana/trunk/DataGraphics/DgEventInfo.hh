#ifndef WRAP_EVENTINFO_HH
#define WRAP_EVENTINFO_HH

#include <DataGraphics/DgTypes.hh>
#include <vector>
#include <string>
#include <cassert>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>

// This class is NOT serialized.
// It is designed to keep event-level info in memory for quick global access

namespace
DataGraphics
{

  // A class that will store current event information
  // (event number, run number, various efficiency weights etc)
  class DgEventInfo {
  protected:
    unsigned int _run_number;
    unsigned int _event_number;
    bool _is_fiducial;
    Value _mc_weight;
    std::vector<float> _pdf_weights;
    Value _pu_weight;
    Value _wzpt_weight;
    Value _wzpt_weight2;
    Value _wzpt_weight3;
    Value _wpol_weight;
    Value _znlo_weight; // reweight observed MC@NLO generator deficit in "Z samples" in ~60-65 GeV mass region
    Value _alpy_weight; // alpgen rapidity reweighting (similar to PDF reweighting, really)
    Value _vx_weight;
    Value _ls1_weight;  // line-shape weight: width parametrization
    Value _ls2_weight;  // line-shape weight: running coupling
    Value _ph_weight;   // pythia-to-herwig weight, OR: PowhegPythia->Data weight
    Value _eff_weight;
    Value _eff_weight_error;
    Value _eff_weight_error_sys;
    Value _effphi_weight; // reco eta x phi weight (applied instead)
    Value _trig_weight;
    Value _trigall_weight; // multiple of all trigger weights per event (e.g., Z with both-matched)
    Value _trigphi_weight; // trigger phi weight, applied *on top*
    Value _mcp_weight;  // mcp trigger
    Value _mcpall_weight; // mcp trigger (assuming ALL muons fire)
    Value _trig_weight_error;
    Value _iso_weight;
    Value _iso_weight_error;
    std::vector<float> _asym_extras;
  public:
    DgEventInfo() { reset(); }
    
    void reset() {
      _run_number = 0;
      _event_number = 0;
      _is_fiducial = false;
      _mc_weight = 1.0;
      _pu_weight = 1.0;
      _wzpt_weight = 1.0;
      _wzpt_weight2 = 1.0;
      _wzpt_weight3 = 1.0;
      _wpol_weight = 1.0;
      _znlo_weight = 1.0;
      _alpy_weight = 1.0;
      _vx_weight = 1.0;
      _ls1_weight = 1.0;
      _ls2_weight = 1.0;
      _ph_weight = 1.0;
      _eff_weight = 1.0;
      _eff_weight_error = 0.0;
      _eff_weight_error_sys = 0.0;
      _effphi_weight = 1.0;
      _trig_weight = 1.0;
      _trigall_weight = 1.0;
      _trigphi_weight = 1.0;
      _mcp_weight = 1.0;
      _mcpall_weight = 1.0;
      _trig_weight_error = 0.0;
      _iso_weight = 1.0;
      _iso_weight_error = 0.0;
      _pdf_weights.clear();
      _asym_extras.clear();
    }

    // vectors for DgNtuple
    const std::vector<std::string>& get_names() const {
      static std::vector<std::string> v = boost::assign::list_of("run")("event")("fid")("mcw")("puw")("wzptw")("wzptw2")("wzptw3")("wpolw")("znlow")("alpy")("vxw")("ls1w")("ls2w")("phw")("effw")("effphiw")("trigw")("trigallw")("trigphiw")("mcpw")("mcpallw")("isow")("lha_CT10")("lha_MSTW")("lha_HERA")("lha_NNPDF")("lha_ABM");
      return v;
    };
    const std::vector<float> get_values() const {
      std::vector<float> v = boost::assign::list_of(static_cast<float>(_run_number))(static_cast<float>(_event_number))(static_cast<float>(_is_fiducial))(_mc_weight)(_pu_weight)(_wzpt_weight)(_wzpt_weight2)(_wzpt_weight3)(_wpol_weight)(_znlo_weight)(_alpy_weight)(_vx_weight)(_ls1_weight)(_ls2_weight)(_ph_weight)(_eff_weight)(_effphi_weight)(_trig_weight)(_trigall_weight)(_trigphi_weight)(_mcp_weight)(_mcpall_weight)(_iso_weight);
      const std::vector<float>& w = _pdf_weights;
      static std::vector<float> wones = boost::assign::list_of(1)(1)(1)(1)(1);
      if (w.size()==5) {
	v.insert( v.end(), w.begin(), w.end() );
      }
      else {
	v.insert( v.end(), wones.begin(), wones.end() );
      }
      return v;
    };
    // additional values for wasymmetry DgNtuple [ deprecated ]
    const std::vector<std::string>& get_asym_extras_names() const {
      static std::vector<std::string> v = boost::assign::list_of("tr_ena")("tr_w_m")("tr_l_pt")("tr_l_eta")("tr_met")("tr_njet");
      return v;
    };
    const std::vector<float> get_asym_extras_values() const {
      std::vector<float> v;
      const std::vector<float>& w = _asym_extras;
      static std::vector<float> wzeros = boost::assign::list_of(0)(0)(0)(0)(0);
      if (w.size()==5) {
	v.push_back(1.0);
	v.insert( v.end(), w.begin(), w.end() );
      }
      else {
	v.push_back(0.0);
	v.insert( v.end(), wzeros.begin(), wzeros.end() );
      }
      return v;
    };

    void set_run_number(unsigned int val) { _run_number = val; }
    void set_event_number(unsigned int val) { _event_number = val; }
    void set_is_fiducial(bool val) { _is_fiducial = val; }
    void set_mc_weight(Value val) { _mc_weight = val; }
    void set_pdf_weights(const std::vector<float>& val) { _pdf_weights = val; }
    void set_asym_extras(const std::vector<float>& val) { _asym_extras = val; }
    void set_pu_weight(Value val) { _pu_weight = val; }
    void set_wzpt_weight(Value val) { _wzpt_weight = val; }
    void set_wzpt_weight2(Value val) { _wzpt_weight2 = val; }
    void set_wzpt_weight3(Value val) { _wzpt_weight3 = val; }
    void set_wpol_weight(Value val) { _wpol_weight = val; }
    void set_znlo_weight(Value val) { _znlo_weight = val; }
    void set_alpy_weight(Value val) { _alpy_weight = val; }
    void set_vx_weight(Value val) { _vx_weight = val; }
    void set_ls1_weight(Value val) { _ls1_weight = val; }
    void set_ls2_weight(Value val) { _ls2_weight = val; }
    void set_ph_weight(Value val) { _ph_weight = val; }
    void set_eff_weight(Value val, Value err=0.0, Value err_sys=0.0) {
      _eff_weight = val;
      _eff_weight_error = err;
      _eff_weight_error_sys = err_sys;
    }
    void set_effphi_weight(Value val, Value err=0.0) {
      _effphi_weight = val;
    }
    void set_trig_weight(Value val, Value err=0.0) {
      _trig_weight = val;
      _trig_weight_error = err;
    }
    void set_trigphi_weight(Value val, Value err=0.0) {
      _trigphi_weight = val;
    }
    void set_trigall_weight(Value val, Value err=0.0) {
      _trigall_weight = val;
    }
    void set_iso_weight(Value val, Value err=0.0) {
      _iso_weight = val;
      _iso_weight_error = err;
    }
    void set_mcp_weight(Value val, Value err=0.0) {
      _mcp_weight = val;
    }
    void set_mcpall_weight(Value val, Value err=0.0) {
      _mcpall_weight = val;
    }

    const unsigned int get_run_number() const { return _run_number; }
    const unsigned int get_event_number() const { return _event_number; }
    const bool get_is_fiducial() const { return _is_fiducial; }
    const Value get_mc_weight() const { return _mc_weight; }
    const Value get_pu_weight() const { return _pu_weight; }
    const Value get_wzpt_weight() const { return _wzpt_weight; }
    const Value get_wzpt_weight2() const { return _wzpt_weight2; }
    const Value get_wzpt_weight3() const { return _wzpt_weight3; }
    const Value get_wpol_weight() const { return _wpol_weight; }
    const Value get_znlo_weight() const { return _znlo_weight; }
    const Value get_alpy_weight() const { return _alpy_weight; }
    const Value get_vx_weight() const { return _vx_weight; }
    const Value get_ls1_weight() const { return _ls1_weight; }
    const Value get_ls2_weight() const { return _ls2_weight; }
    const Value get_ph_weight() const { return _ph_weight; }
    const Value get_eff_weight() const { return _eff_weight; }
    const Value get_trig_weight() const { return _trig_weight; }
    const Value get_trigall_weight() const { return _trigall_weight; }
    const Value get_trigphi_weight() const { return _trigphi_weight; }
    const Value get_effphi_weight() const { return _effphi_weight; }
    const Value get_mcp_weight() const { return _mcp_weight; }
    const Value get_mcpall_weight() const { return _mcpall_weight; }
    const Value get_iso_weight() const { return _iso_weight; }

  };

};

#endif //WRAP_EVENTINFO_HH
