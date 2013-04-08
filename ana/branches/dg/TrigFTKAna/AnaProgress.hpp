#ifndef WRAP_ANAPROGRESS_HH
#define WRAP_ANAPROGRESS_HH

#include <string>
#include <numeric>
#include <iostream>
#include <boost/format.hpp>
#include <boost/timer.hpp>
#include <boost/optional.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/math/special_functions/fpclassify.hpp>

template<typename ostreamT>
class
AnaProgress
{
private:
  std::string _name;
  ostreamT& _os;
  double _min_silent_time;
  boost::optional<unsigned long> _total_laps;
  double _total_timer;
  boost::timer _display_timer;
  // running statistics
  unsigned long _nlaps; // total number of events processed
  unsigned long _nbytes; // total number of bytes read
  unsigned long _nlaps_sample; // total number of events processed since last sample
  unsigned long _nbytes_sample; // total number of bytes read since last sample
  boost::optional<double> _lap_rate; // event processing rate
  boost::optional<double> _byte_rate; // byte bandwidth
  boost::circular_buffer<double> _lap_rate_history; // previous n laps
  boost::circular_buffer<double> _lap_byte_history; // previous n laps
private:
  const double mean_lap_rate_rolling() const {
    double result = 0.;
    if( _lap_rate_history.empty() ) { return result; }
//     try {
      result = std::accumulate( _lap_rate_history.begin() , _lap_rate_history.end() , 0. ) / _lap_rate_history.size();
//     } catch(...) {}
    return result;
  }

  const double mean_byte_rate_rolling() const {
    double result = 0.;
    if( _lap_byte_history.empty() ) { return result; }
//     try {
      result = std::accumulate( _lap_byte_history.begin() , _lap_byte_history.end() , 0. ) / _lap_byte_history.size();
//     } catch(...) {}
    return result;
  }
public:
  AnaProgress( const std::string& name , ostreamT& os )
    : _name(name)
    , _os(os)
    , _min_silent_time(30.)
    , _total_laps()
    , _total_timer(0.)
    , _display_timer()
    , _nlaps(0ul)
    , _nbytes(0ul)
    , _nlaps_sample(0ul)
    , _nbytes_sample(0ul)
    , _lap_rate()
    , _byte_rate()
    , _lap_rate_history(5)
    , _lap_byte_history(5)
  {
    _display_timer.restart();
  }
  AnaProgress( const std::string& name , ostreamT& os , const unsigned long& total_laps )
    : _name(name)
    , _os(os)
    , _min_silent_time(30.)
    , _total_laps(total_laps)
    , _total_timer(0.)
    , _display_timer()
    , _nlaps(0ul)
    , _nbytes(0ul)
    , _nlaps_sample(0ul)
    , _nbytes_sample(0ul)
    , _lap_rate()
    , _byte_rate()
    , _lap_rate_history(5)
    , _lap_byte_history(5)
  {
    _display_timer.restart();
  }
  virtual ~AnaProgress() {
    progress(_display_timer.elapsed());
  }
  void set_total_events( const unsigned long& nlaps ) { _total_laps.reset(nlaps); }
  void set_minimum_silent_time( const double & duration ) { _min_silent_time = duration; }
  void reset() {
    _total_timer = 0.;
    _display_timer.restart();
    _nlaps = 0ul;
    _nbytes = 0ul;
    _nlaps_sample = 0ul;
    _nbytes_sample = 0ul;
    _lap_rate_history.clear();
    _lap_byte_history.clear();
  }
  void progress( const double& elapsed ) {
    _total_timer += elapsed;
    _nlaps += _nlaps_sample;
    _nbytes += _nbytes_sample;
    _lap_rate.reset();
    try {
      const double rate = _nlaps_sample/elapsed;
      _lap_rate.reset( rate );
      _lap_rate_history.push_back( rate );
    } catch(...) {}
    _byte_rate.reset();
    try {
      const double rate = _nbytes_sample/elapsed;
      _byte_rate.reset( rate );
      _lap_byte_history.push_back( rate );
    } catch(...) {}
    _os << "[ " << _name;
    if( _total_laps ) {
      double lap_fraction = 0.; 
      try { 
        lap_fraction = 100. * static_cast<double>(_nlaps) / (*_total_laps);
      } catch(...) {}
      _os << boost::format("%|10t| %|6.2f| %%") % lap_fraction;
    } else {
      _os << boost::format("%|10t| %|5d|---%|8d|--- events") % _nlaps_sample % _nlaps;
    }
    _os << boost::format(" %|6.2f| sec") % _display_timer.elapsed();
    _os << boost::format(" %|5t| rate: %|7.2f| <%|0.0f|> Hz") % (*_lap_rate) % mean_lap_rate_rolling();
    _os << boost::format(" %|5t| read: %|7.2f| <%|0.2f|> MB/sec") % ((*_byte_rate)/1048576.) % (mean_byte_rate_rolling()/1048576.);
    _os << " ]" << std::endl;
    _display_timer.restart();
  }
  void update( const unsigned long& nlaps , const unsigned long& nbytes ) {
    const double elapsed = _display_timer.elapsed();
    _nlaps_sample += nlaps;
    _nbytes_sample += nbytes;
    if( elapsed >= _min_silent_time ) {
      progress(elapsed);
      _nlaps_sample = 0ul;
      _nbytes_sample = 0ul;
    }
  }
  
  void display() {
//     if( _total_laps && (_nlaps!=_total_laps) ) {
//       _os << boost::format("[ %1% warning: incomplete run of %2% of %3% events ]") % _name % _nlaps % _total_laps << std::endl;
//     }

    // _os << boost::format("[ %1% complete: %|30t| %|#5f| events %|#5f| sec %|#5f| Hz read %|#4f| MB/sec ]") 
    //   % _name % _nlaps % _total_timer % mean_lap_rate() % mean_byte_rate() 
    //     << std::endl;
  }

  const double mean_lap_rate() const {
    double result = 0.;
    try {
      result = _nlaps / _total_timer;
    } catch(...) {}
    return boost::math::isnan(result) ? 0 : result;
  }

  const double mean_byte_rate() const {
    double result = 0.;
    try {
      result = _nbytes / _total_timer;
    } catch(...) {}
    return boost::math::isnan(result) ? 0 : result;
  }

  const unsigned long total_events() const { return _total_laps; }
  const double total_time() const { return _total_timer; } // sec

  // boost::serialization
  template<class serializationArchiveT>
  void serialize(serializationArchiveT& ar, const unsigned int version) {
    ar & _name;
    ar & _total_laps;
    ar & _total_timer;
    ar & _nlaps;
    ar & _nbytes;
  }

};

namespace boost { 
  namespace serialization {
    template<class T>
    struct version< AnaProgress<T> >
    {
      BOOST_STATIC_CONSTANT(unsigned int, value = 1); // BOOST_CLASS VERSION == value
    };
  } // namespace serialization
} // namespace boost

#endif // WRAP_ANAPROGRESS_HH
