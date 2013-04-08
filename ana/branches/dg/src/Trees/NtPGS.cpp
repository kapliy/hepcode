
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <list>
#include <map>
#include <set>
#include <bitset>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/foreach.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/filesystem.hpp>
#include "TrigFTKAna/ana_streams.hpp"
#include "TrigFTKAna/Trees/NtPGS.hpp"
#include "TrigFTKAna/Detector.hpp"
#include "TrigFTKAna/AnaElectron.hpp"
#include "TrigFTKAna/AnaMuon.hpp"
#include "TrigFTKAna/AnaJet.hpp"
#include "TrigFTKAna/AnaMET.hpp"

using namespace std;
using namespace boost;

const unsigned long
NtPGS::count_events( const std::string& filename )
{
  cout << " warning: NtPGS::count_events not implemented" << endl;
  return 0;
}


// //bool NtPGS::pTsort() 
// struct pTsort
// {
//   inline bool operator() (AnaJet& jet1, AnaJet& jet2){
//     return (jet1.pt() > jet2.pt());
//   }
  
// };


// struct pTsort
// {
//   bool operator()(std::shared_ptr<AnaJet>& jet1, std::shared_ptr<Test>& jet2)
//   {
//     return(jet1->pt() > jet2->pt());
//   }
  
// };



const bool
NtPGS::begin_file( const std::string& filename )
{
  // lock the file TrigFTKAna/.file_open_lock. this is a signal to all
  // other parallel jobs that we are about to attempt a file open, and
  // they should chill. the motivation for this (and the included
  // random delay) is to reduce the peak load on dcache servers that
  // might otherwise get hit with many file open requests
  // simultaneously.
  boost::shared_ptr<ana_streams::istream> tmp;
  try { 
    boost::interprocess::file_lock lock( "./.file_open_lock" );
    boost::interprocess::scoped_lock<boost::interprocess::file_lock> sc_lock( lock );
    tmp = ana_streams::open_for_read( filename );
  } catch( ... ) {
    // do nothing and hope for the best.
    tmp = ana_streams::open_for_read( filename );
  }
  if( !tmp || !(tmp->good()) ) { return false; }
  _inf.swap( tmp );
  _last_event = optional<unsigned int>();
  _open_filename = filename;
  return true;
}

const bool
NtPGS::end_file()
{
  _inf.reset();
  _open_filename = string();
  _last_event = optional<unsigned int>();
}

const bool
NtPGS::get_event( const unsigned long& ievent )
{
  // check that ievent is the next event to be processed.  there is
  // (at present) no mechanism for random, non-sequential access.
  if( !_last_event && ievent!=0ul ) {
    cout << " warning: NtPGS::get_event is intended for sequential access only; cannot get event " << ievent << endl;
    return false;
  }
  if( _last_event && ievent!=((*_last_event)+1) ) {
    cout << " warning: NtPGS::get_event is intended for sequential access only; cannot get event " << ievent << " after event " << (*_last_event) << endl;
    return false;
  }
  // parse the PGS file. format specified at
  // x-devonthink-item://7EFDA2A0-BC8F-40B9-AF70-179B0C6245B3 or on
  // the LHC Olympics webpages.
  clear();
  bool start_event = true;
  do { // event read loop
    // retrieve the next line
    string line;
    list<string> tokens;
    bool readok = false;
    if( !_cached_line || (*_cached_line)=="" ) {
      if( !_inf || !_inf->good() ) {
        // may or may not be premature end of usable file. let the event manager figure it out.
        return false;
      }
      readok = getline(*_inf,line)!=0;
    } else {
      line = *_cached_line;
      readok = true;
    }
    _cached_line.reset();
    boost::trim( line );
    if( istarts_with(line,"#") ) { continue; } // ignore comments.
    boost::split( tokens , line, boost::is_any_of("\t \n") , boost::token_compress_on );
    // first token on the line should be a number. 0 indicates the
    // line starts a new event record. otherwise, the number is a
    // unique label for each final state.
    int index = -1;
    if( !tokens.empty() ) { 
      convert( tokens.front() , index );
      tokens.pop_front();
      if( index<0 ) { 
        cout << " warning: NtPGS confused. stopping in event " << ievent << " in file " << _open_filename << " after line " << line << endl; 
        clear();
        return false;
      }
    }
    // end of event if the line begins with a zero (in which case we
    // begin a new event) or if we reach the end of the file or an empty line.
    if( (!start_event && index==0) || !readok || tokens.empty() ) {
      // done with the current event. save the line from the next
      // event we just read for when the next event is needed.
      _cached_line.reset( line );
      break;
    }
    if( tokens.empty() ) { continue; }
    // if this is the beginning of an event, record the run and event number
    if( index==0 ) { 
      convert( tokens.front() , _run_number ); tokens.pop_front();
      convert( tokens.front() , _event_number ); tokens.pop_front();
      _last_event.reset( ievent );
      start_event = false;
      continue;
    }
    // continue to fill in the current event.
    const int code_photon = 0;
    const int code_electron = 1;
    const int code_muon = 2;
    const int code_tau_hadronic = 3;
    const int code_jet = 4;
    const int code_met = 6;
    int this_code;
    convert( tokens.front() , this_code ); tokens.pop_front();
    double eta,phi,pt,mass,btag,hadem,dummy1,dummy2;
    double charge;
    int ntrk;
    switch( this_code ) {
    case code_electron:
      {
        convert( tokens.front() , eta ); tokens.pop_front();
        convert( tokens.front() , phi ); tokens.pop_front();
        convert( tokens.front() , pt ); tokens.pop_front();
        convert( tokens.front() , mass ); tokens.pop_front();
        convert( tokens.front() , charge ); tokens.pop_front();
        //convert( tokens.front() , dummy1 ); tokens.pop_front();
        //convert( tokens.front() , hadem ); tokens.pop_front();
        boost::shared_ptr<AnaElectron> ele( new AnaElectron );
        ele->_momentum.SetPtEtaPhiM( pt , eta , phi , mass );
        ele->_charge = charge;
        _electrons.push_back( ele );
        break;
      }
    case code_muon:
      {
        convert( tokens.front() , eta ); tokens.pop_front();
        convert( tokens.front() , phi ); tokens.pop_front();
        convert( tokens.front() , pt ); tokens.pop_front();
        convert( tokens.front() , mass ); tokens.pop_front();
        convert( tokens.front() , charge ); tokens.pop_front();
        boost::shared_ptr<AnaMuon> mu( new AnaMuon );
        mu->_momentum.SetPtEtaPhiM( pt , eta , phi , mass );
        mu->_charge = charge;
        _muons.push_back( mu );
        break;
      }
    case code_jet:
      {
        convert( tokens.front() , eta ); tokens.pop_front();
        convert( tokens.front() , phi ); tokens.pop_front();
        convert( tokens.front() , pt ); tokens.pop_front();
        convert( tokens.front() , mass ); tokens.pop_front();
        convert( tokens.front() , ntrk ); tokens.pop_front();
        convert( tokens.front() , btag ); tokens.pop_front();
        convert( tokens.front() , hadem ); tokens.pop_front();
        boost::shared_ptr<AnaJet> jet( new AnaJet );
        jet->_momentum.SetPtEtaPhiM( pt , eta , phi , mass );
        jet->_tag_pgs = btag;
        jet->_emf = 1/hadem;
        _jets.push_back( jet );
        break;
      }
    case code_met:
      {
        convert( tokens.front() , eta ); tokens.pop_front();
        convert( tokens.front() , phi ); tokens.pop_front();
        convert( tokens.front() , pt ); tokens.pop_front();
        boost::shared_ptr<AnaMET> met( new AnaMET );
        met->_momentum.SetPtEtaPhiM( pt , 0 , phi , 0 );
        //        met->_met_type = AnaMET::RAW;
        met->_met_type = AnaMET::LOCHADTOPO; //BP
        // met->_sum_et = pt;
        // met->_et_x = -1.*pt*std::cos(phi);
        // met->_et_y = -1.*pt*std::sin(phi);
        _mets.push_back( met );
        break;
      }
    default:
      {
        // ignore the rest for now.
      }
    }
  } while( true ); 

  if( false ) {
    cout << " NtPGS: processed event " << _run_number << " " << _event_number << " with " 
         << _electrons.size() << " electrons "
         << _muons.size() << " muons "
         << _jets.size() << " jets "
         << _mets.size() << " METs "
         << endl;
    BOOST_FOREACH( const boost::shared_ptr<const AnaElectron>& ele , _electrons ) { ele->print(); }
    BOOST_FOREACH( const boost::shared_ptr<const AnaMuon>& mu , _muons ) { mu->print(); }
    BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , _jets ) { jet->print(); }
    BOOST_FOREACH( const boost::shared_ptr<const AnaMET>& met , _mets ) { met->print(); }
  }


  //  BOOST_FOREACH( const boost::shared_ptr<const AnaJet>& jet , _jets ) { cout<<jet->pt()<<endl; }
  //  std::sort(_jets.begin(), _jets.end(), pTsort );
  
  // update last event accounting
  _last_event = ievent;
  return true;
}
