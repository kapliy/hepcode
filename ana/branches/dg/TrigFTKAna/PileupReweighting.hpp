b#ifndef WRAP_PILEUPREWEIGHTING_HPP
#define WRAP_PILEUPREWEIGHTING_HPP

#include <string>
#include <utility>
#include <iostream>
#include "DataGraphics/DgXMLMap.hh"

class
PileupReweighting
{

public:

  typedef float ftype;

private:

  typedef DgXMLIntMap<ftype> map_type;
  map_type _map;
  unsigned int _max_nvert;

  void _recompute_max_nvert() {
    // recompute maximum number of vertices in map
    std::pair<int,int> mmk = _map.min_max_key();
    assert( mmk->first == 0 || mmk->first == 1 ); 
    _max_nvert = mmk->second;
  }

public:

  PileupReweighting( const std::string& filename ) : _max_nvert(0) {
    if( filename.empty() ) { return; }
    const bool ok = _map.load( filename );
    if( !ok ) { 
      std::cerr << " error: could not load pileup reweighting map " << filename << "." << std::endl;
      throw;
    }
    _recompute_max_nvert();
  }
  virtual ~PileupReweighting() {}

  const ftype for_nvtx( const unsigned int& number_of_good_primary_vertices ) const {
    const unsigned int clip_nvert = std::min( number_of_good_primary_vertices , _max_nvert );
    return( _data.value[clip_nvert] ); // default value if clip_nvert not present in map
  }

  // event weighting numbers for various applications
  // ================================================================
  // H->ww dilepton vertex reweighting function for full 2010 H->WW
  // dataset, provided by "Aaron" at
  // https://twiki.cern.ch/twiki/pub/AtlasProtected/HiggsWWdilConfNote2011Winter/primVtxWeight.C
  void set_hwwdil_2010() {
    _data[0] = 1.;
    _data[1] = 1.66781;
    _data[2] = 1.11649;
    _data[3] = 0.846559;
    _data[4] = 0.695447;
    _data[5] = 0.678215;
    _data[6] = 0.672109;
    _recompute_max_nvert();
  }

  // SM->ww dilepton vertex reweighting function for full 2010 SM->WW dataset
  // provided at 
  // https://svnweb.cern.ch/trac/atlasgrp/browser/Physics/StandardModel/ElectroWeak/WWObservationPaper/SupportNote/WWtools/VtxReweighting.h
  // for pileup MC
  void set_smwwdil_2010() {
    _data[0] = 1.;
    _data[1] = 1.6169;
    _data[2] = 1.1216;
    _data[3] = 0.8561;
    _data[4] = 0.7070;
    _data[5] = 0.6859;
    _data[6] = 0.6218;
    _data[7] = 0.6168;
    _recompute_max_nvert();
  }


  void save( const std::string& filename ) const { _data.save( filename ); }
 
};

#endif // WRAP_PILEUPREWEIGHTING_HPP

