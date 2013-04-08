#include "TrigFTKAna/AnaCommon.hpp"
#include <string>
#include "TrigFTKAna/AnaEvent.hpp"

const std::string AnaEvent::DEFAULT_SOURCE = "DEFAULT";

#include "TrigFTKAna/AnaVertex.hpp"
unsigned int AnaEvent::nprimary_vertices() const {
  const std::vector< boost::shared_ptr<const AnaVertex> >& vx = vertices();
  return std::count_if( vx.begin() , vx.end() , bind(&AnaVertex::type,_1) == ATLAS::PriVtx || bind(&AnaVertex::type,_1) == ATLAS::PileUp);
}

// DEFINE_DGHEAP(AnaEvent,"AnaEvent");
