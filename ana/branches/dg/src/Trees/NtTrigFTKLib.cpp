
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include "TrigFTKAna/ana_streams.hpp"
#include "TrigFTKAna/Trees/NtTrigFTKLib.hpp"
#include "TrigFTKAna/UniqueBarcode.hpp"
#include "TrigFTKAna/Detector.hpp"

using namespace std;
using namespace boost;

const bool
NtTrigFTKLib::begin_file( const std::string& filename )
{
  shared_ptr<ana_streams::istream> tmp( ana_streams::open_for_read( ana_streams::root_dcap_filename(filename) ) );
  if( !tmp || !(tmp->good()) ) { return false; }
  _inf.swap( tmp );
  _last_event = optional<unsigned int>();
  _open_filename = ana_streams::root_dcap_filename(filename);
  return true;
}

const bool
NtTrigFTKLib::end_file()
{
  _inf.reset();
  _open_filename = string();
  _last_event = optional<unsigned int>();
}

const bool
NtTrigFTKLib::get_event( const unsigned long& ievent )
{
  // check that ievent is the next event to be processed.  there is
  // (at present) no mechanism for random, non-sequential access.
  if( !_last_event && ievent!=0ul ) {
    cout << " warning: NtTrigFTKLib::get_event is intended for sequential access only; cannot get event " << ievent << endl;
    return false;
  }
  if( _last_event && ievent!=((*_last_event)+1) ) {
    cout << " warning: NtTrigFTKLib::get_event is intended for sequential access only; cannot get event " << ievent << " after event " << (*_last_event) << endl;
    return false;
  }
  // parse the file until the end-of-event character is reached or the file ends.
  // line type characters:
  const char run_token = 'R';
  const char event_token = 'F';
  const char end_of_event_token = 'L';
  const char ftk_track_token = 'E';
  const char cluster_on_track_token = 'C';
  // variables to read per event
  clear();
  unsigned int nhits = 0u;
  //
  bool is_valid = false;
  do { // event read loop
    // retrieve the next line
    string line;
    if( !_inf || !_inf->good() || !getline(*_inf,line) ) {
      // may or may not be premature end of usable file. let the event manager figure it out.
      // cout << " warning: NtTrigFTKLib::get_event out of file at event " << ievent << " after event " << (*_last_event) << endl;
      return false;
    }
    istringstream sline( line );
    char type_token;
    sline >> type_token;
    switch( type_token ) {
    case run_token:
      sline >> _run_number;
      break;
    case event_token:
      sline >> _event_number;
      break;
    case end_of_event_token:
      {
        unsigned long iev;
        sline >> iev;
        if( iev != _event_number ) {
          cout << " warning: NtTrigFTKLib::get_event event mismatch at " << ievent << " != " << iev << " after event " << (*_last_event) << endl;
          return false;
        } else {
          is_valid = true;
        }
      }
      break;
    case cluster_on_track_token:
      // format is: x , layer missing , barcode , frac
      // just increment the hit counter for now
      AnaTrack::ftype x, frac;
      unsigned int layer_missing;
      unsigned long barcode;
      sline >> x >> layer_missing >> barcode >> frac;
      if( layer_missing!=0 ) {
        ++nhits;
      }
      break;
    case ftk_track_token:
      // read ftk track
      {
        AnaTrack tmp;
        signed long barcode;
        AnaTrack::ftype pt;
        unsigned int sector;
        sline >> tmp._d0 >> tmp._z0 >> tmp._phi0 >> tmp._lambda >> pt >> tmp._chi2 >> tmp._ftk_bitmask >> sector >> barcode >> tmp._barcode_matching_fraction;
        tmp._alg = AnaTrack::Algorithm::FTK;
        tmp._curv = detector::safe_divide(static_cast<AnaTrack::ftype>(1.),pt)/1000.;
        tmp._phi0 = detector::canonical_phi( tmp._phi0 );
        tmp._ndof = std::max( nhits - 5 , 0u );
        tmp._nhits = nhits;
        tmp._nmisses = 14 - nhits;//detector::ftkNumLayersMissed(tmp._ftk_bitmask);
        //tmp._ftk_sector = sector;
        tmp._barcode = UniqueBarcode(barcode);
        _ftk_tracks.push_back( boost::make_shared<AnaTrack>(tmp) );
        //         cout << " AnaTrack (ftk):    " 
        //              << " pt: " << setw(10) << tmp.pt0()
        //              << " c0: " << setw(10) << tmp.c0()
        //              << " p0: " << setw(10) << tmp.phi0()
        //              << " d0: " << setw(10) << tmp.d0()
        //              << " z0: " << setw(10) << tmp.z0()
        //              << " ct: " << setw(10) << tmp.ct0()
        //              << endl;
        nhits = 0u;
      }
      break;
    default:
      cout << " warning: NtTrigFTKLib::get_event can't recognize line type " << type_token <<  " at event " << ievent << " after event " << (*_last_event) << endl;
    };
  } while( !is_valid );

  if( false ) {
    cout << " NtTrigFTKLib: processed event " << _run_number << " " << _event_number << " with " 
         << _ftk_tracks.size() << " reconstructed ftk tracks "
         << endl;
  }

  // update last event accounting
  _last_event = ievent;
  return true;
}

const unsigned long
NtTrigFTKLib::count_events( const std::string& filename )
{
  boost::shared_ptr<ana_streams::istream> inf( ana_streams::open_for_read( filename ) );
  string line;
  unsigned long n = 0ul;
  while( !(inf->eof()) ) {
    getline( *inf , line );
    if( line[0] == 'L' ) { ++n; }
  }
  return n;
}
