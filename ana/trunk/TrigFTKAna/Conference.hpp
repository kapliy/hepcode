///////////////////////////////////////////////////////////////////
// Conference.hpp
///////////////////////////////////////////////////////////////////

#ifndef WRAP_CONFERENCE_HPP
#define WRAP_CONFERENCE_HPP

/** @enum ConfType
    This file defines the enum containing a list of major ATLAS processing campaigns and/or conferences
    As such, the value of ConfType is used internally in TrigFTKAna to select appropriate settings
    (scaling constants, efficiency corrections, etc)
    @enum DataPeriod
    Can be used to specify the range of ATLAS data periods that's being run over 
    (this is useful to emulate certain defects in MC, such as dead FEM modules).
*/

#include <string>
#include <boost/algorithm/string.hpp>

namespace CONF {

  enum ConfType         {
    LATEST = 7,          //!< Picks up the latest RootCore packages -- this now corresponds to 2011 tools
    SUSI = 8,
    NotSpecified = -99  //!< this is the default
  };

  // Deduce conference type from a user-supplied string.
  // If certain conferences share settings, feel free to add alias below.
  static const ConfType select_conf(const std::string& name) {
    const std::string upname = boost::to_upper_copy(name);
    try {
      if(name=="REL17MC11C" || name=="MC11C" || name=="LATEST") return LATEST;
      throw std::out_of_range("Conference name "+name+" not recognized. Look for examples in Conference.hpp");
    } catch( std::out_of_range& e ) {
      throw e;
    }
    return NotSpecified;
  }

}

namespace DATARANGE {
  
  // these ranges refer to 2011 data periods:
  enum DataRange         {
    BtoM  = 5,          //!< D through M
    DtoM  = 7,          //!< D through M
    DtoMnorpc  = 8,     //!< D through M (cutting out L3&L4 that have severe RPC timing issues)
    NotSpecified = -99  //!< this is the default
  };

  static const std::string& datarange_string(const DataRange& d) {
    static std::string sBtoM("BtoM");
    static std::string sDtoM("DtoM");
    static std::string sDtoMnorpc("DtoMnorpc");
    static std::string sNone("NotSpecified");
    switch (d) {
    case BtoM: return sBtoM; break;
    case DtoM: return sDtoM; break;
    case DtoMnorpc: return sDtoMnorpc; break;
    default: return sNone; break;
    }
    return sNone;
  }
  
  // Deduce data periods from a user-supplied string.
  // Feel free to add alias below.
  static const DataRange select_range(const std::string& name) {
    const std::string upname = boost::to_upper_copy(name);
    try {
      if(name=="BtoM") return BtoM;
      if(name=="DtoM") return DtoM;
      if(name=="DtoMnorpc") return DtoMnorpc;
      throw std::out_of_range("Data Range "+name+" not recognized. Look for examples in Conference.hpp");
    } catch( std::out_of_range& e ) {
      throw e;
    }
    return NotSpecified;
  }

}

#endif // WRAP_CONFERENCE_HPP
  
