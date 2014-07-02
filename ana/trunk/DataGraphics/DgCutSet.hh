#ifndef WRAP_DGCUTSET_HH
#define WRAP_DGCUTSET_HH

// classes to apply a set of user-defined cuts, measure total and n-1
// efficiencies, and histogram cut variables at various points during
// the selection. The user interface is through the DgCutSet class.
// 

// DgCutResult stores the result of the set of cuts; the individual
// cut results can be retreived by name (or by cut number).

// The DgDefinition* classes are for internal use only.

#include <cmath>
#include <string>
#include <utility>
#include <functional>
#include <vector>
#include <list>
#include <iterator>
#include <cassert>
#include <iostream>
#include <boost/any.hpp>
#include <boost/optional.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/scope_exit.hpp>
#include <boost/function.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/format.hpp>
#include <boost/foreach.hpp>
#include <boost/optional.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include "DataGraphics/DgCollection.hh"
#include "DataGraphics/DgStudy.hh"

namespace
DataGraphics
{

  class
  DgCutResult
  {
  public:
    // class to represent the result of a series of cuts. this result
    // is returned by the DgCutSet class which is defined farther
    // below. Bits are all zero if all cuts pass. if any cuts fail,
    // their corresponding bits are set.
    // 
    typedef boost::dynamic_bitset<> Bits;
    typedef std::string Label;
    typedef std::vector<std::string> NameColl;
  private:
    Bits _result;
    boost::shared_ptr< NameColl > _cutnames;
  public:
    DgCutResult( const boost::shared_ptr<NameColl>& cutnames )
      : _result( cutnames->size() )
      , _cutnames( cutnames )
    {}
    
    // retrieve the number of cuts tested
    const Bits::size_type n_cuts() const { return _result.size(); }

    // retrieve cut bit from the provided cut name
    const Bits::size_type bit_number( const Label& cutname ) const {
      NameColl::iterator i = std::find(_cutnames->begin(),_cutnames->end(),cutname);
      assert( i != _cutnames->end() );
      return std::distance(_cutnames->begin(),i);
    }

    // retrieve cut name from the provided cut bit number
    const Label& bit_name( const Bits::size_type& cutbit ) const {
      assert( cutbit<_cutnames->size() );
      return (*_cutnames)[cutbit];
    }

    // retrieve cut result from the provided cut bit
    const bool passes( const Bits::size_type& cutbit ) const {
      assert( cutbit < _result.size() );
      return( _result[cutbit]==0 );
    }
    const bool fails( const Bits::size_type& cutbit ) const {
      return( !passes(cutbit) );
    }

    // did cut pass/fail for the named cut?
    const bool passes( const Label& cutname ) const {
      const Bits::size_type n = bit_number(cutname);
      return passes( n );
    }
    const bool fails( const Label& cutname ) const {
      return !passes( cutname );
    }

    // did all cuts pass, up to (and including) a given cut in a sequence of cuts?
    const bool passes_upto( const Bits::size_type& n) const {
      for(Bits::size_type i=0; i<=n; i++) if(!passes(i)) return false;
      return true;
    }
    const bool passes_upto( const Label& cutname) const {
      const Bits::size_type n = bit_number(cutname);
      return passes_upto(n);
    }
    
    // did all or any cuts pass/fail?
    const bool all_pass() const { return _result.none(); }
    const bool all_fail() const { return (~_result).none(); }
    const bool any_pass() const { return !all_fail(); }
    const bool any_fail() const { return _result.any(); }
    
    // did all, excluding a given cut, pass?
    const bool all_others_pass( const Bits::size_type& cutbit ) const {
      if( _result.none() ) { return true; }
      if( _result.count()==1 && _result.test(cutbit) ) { return true; }
      return false;
    }
    const bool all_others_pass( const Label& cutname ) const {
      Bits::size_type n = bit_number(cutname);
      return( all_others_pass(n) );
    }

    // setting functions

    void set_all_fail() { _result.set(); }
    void set_all_pass() { _result.reset(); }
    void set_pass( const Bits::size_type& n ) { _result.set(n,false); }
    void set_fail( const Bits::size_type& n ) { _result.set(n,true); }

  };

  template<typename objectT>
  class DgCutDefinition { 
  public:
    typedef boost::function<bool (void) > Nonary;
    typedef boost::function<bool (objectT const&) > Unary;
    typedef boost::function<void (const std::string&) > FillNonary;
    typedef boost::function<void (const std::string&,objectT const&) > FillUnary;
  private:
    std::string _name; 
  public:
    DgCutDefinition( const std::string& name ) : _name(name) {}
    virtual ~DgCutDefinition() {}
    const std::string& cutname() const { return _name; }
    virtual const bool test( objectT const& obj ) const { return false; }
    virtual const void fill( const std::string& fillname , objectT const& obj ) const { return; }
  };

  template<typename objectT>
  class DgCutDefinitionUU : public DgCutDefinition<objectT> { 
  public: 
    typedef typename DgCutDefinition<objectT>::Unary PredicateType;
    typedef typename DgCutDefinition<objectT>::FillUnary FillType;
  private:
    PredicateType _predicate;
    FillType _fill;
  public:
    DgCutDefinitionUU( const std::string& name , const PredicateType& pred , const FillType& fill ) 
      : _predicate(pred) , _fill(fill) , DgCutDefinition<objectT>(name) {}
    virtual ~DgCutDefinitionUU() {}
    virtual const bool test( objectT const& obj ) const { return _predicate(obj); }
    virtual const void fill( const std::string& fillname , objectT const& obj ) const { return _fill(fillname,obj); }
  };

  template<typename objectT>
  class DgCutDefinitionUG : public DgCutDefinition<objectT>{ 
  public:
    typedef typename DgCutDefinition<objectT>::Unary PredicateType;
    typedef typename DgCutDefinition<objectT>::FillNonary FillType;
  private:
    PredicateType _predicate;
    FillType _fill;
  public:
    DgCutDefinitionUG( const std::string& name , const PredicateType& pred , const FillType& fill ) 
      : _predicate(pred) , _fill(fill) , DgCutDefinition<objectT>(name) {}
    virtual ~DgCutDefinitionUG() {}
    virtual const bool test( objectT const& obj ) const { return _predicate(obj); }
    virtual const void fill( const std::string& fillname , objectT const& obj ) const { return _fill(fillname); }
  };

  template<typename objectT>
  class DgCutDefinitionU : public DgCutDefinition<objectT> { 
  public:
    typedef typename DgCutDefinition<objectT>::Unary PredicateType;
  private:
    PredicateType _predicate;
  public:
    DgCutDefinitionU( const std::string& name , const PredicateType& pred ) 
      : _predicate(pred) , DgCutDefinition<objectT>(name) {}
    virtual ~DgCutDefinitionU() {}
    virtual const bool test( objectT const& obj ) const { return _predicate(obj); }
    virtual const void fill( const std::string& fillname , objectT const& obj ) const { return; }
  };

  template<typename objectT>
  class DgCutDefinitionGU : public DgCutDefinition<objectT> {
  public:
    typedef typename DgCutDefinition<objectT>::Nonary PredicateType;
    typedef typename DgCutDefinition<objectT>::FillUnary FillType;
  private:
    PredicateType _predicate;
    FillType _fill;
  public:
    DgCutDefinitionGU( const std::string& name , const PredicateType& pred , const FillType& fill )
      : _predicate(pred) , _fill(fill) , DgCutDefinition<objectT>(name) {}
    virtual ~DgCutDefinitionGU() {}
    virtual const bool test( objectT const& obj ) const { return _predicate(); }
    virtual const void fill( const std::string& fillname , objectT const& obj ) const { return _fill(fillname,obj); }
  };

  template<typename objectT>
  class DgCutDefinitionGG : public DgCutDefinition<objectT>{ 
  public:
    typedef typename DgCutDefinition<objectT>::Nonary PredicateType;
    typedef typename DgCutDefinition<objectT>::FillNonary FillType;
  private:
    PredicateType _predicate;
    FillType _fill;
  public:
    DgCutDefinitionGG( const std::string& name , const PredicateType& pred , const FillType& fill ) 
      : _predicate(pred) , _fill(fill) , DgCutDefinition<objectT>(name) {}
    virtual ~DgCutDefinitionGG() {}
    virtual const bool test( objectT const& obj ) const { return _predicate(); }
    virtual const void fill( const std::string& fillname , objectT const& obj ) const { return _fill(); }
  };

  template<typename objectT>
  class DgCutDefinitionG : public DgCutDefinition<objectT>{ 
  public:
    typedef typename DgCutDefinition<objectT>::Nonary PredicateType;
  private:
    PredicateType _predicate;
  public:
    DgCutDefinitionG( const std::string& name , const PredicateType& pred ) 
      : _predicate(pred) , DgCutDefinition<objectT>(name) {}
    virtual ~DgCutDefinitionG() {}
    virtual const bool test( objectT const& obj ) const { return _predicate(); }
    virtual const void fill( const std::string& fillname , objectT const& obj ) const { return; }
  };

  template<typename objectT>
  class
  DgCutSetAttachment
  {
  public:
    typedef boost::function< void ( objectT const& ) > Callback;
  protected:
    bool _init;
    Callback _callback;
  public:
    DgCutSetAttachment() : _init(false) , _callback() {}
    DgCutSetAttachment( const Callback callback ) : _init(true) , _callback(callback) {}
    void insert( objectT const& obj ) { 
      if( _init ) { _callback(obj); }
    }
  };

  template< typename objectT >
  class
  DgCutSet
  {
  public:
    // class to encode a series of cuts, apply the cuts and return a
    // DgCutResult, and produce a series of histograms of the involved
    // quantities (before cuts, after n-1 cuts, and after all cuts).
    //
    // the std::string provides a way to attach an external label to
    // each cut.
  public:
    typedef objectT input_type;
    typedef DgCutDefinition<objectT> DgCutDefinitionType;
    typedef std::vector< boost::shared_ptr<DgCutDefinitionType> > CutColl;
    typedef typename std::vector< boost::shared_ptr< DgStudy<objectT> > > StudyColl;
    typedef DgCutResult::NameColl NameColl;
    typedef std::vector< boost::any > GarbageColl;
  private:
    std::string _name;
    CutColl _cuts;
    StudyColl _extra_studies;
    GarbageColl _garbage;
    boost::shared_ptr< NameColl > _cutnames;
    bool _do_sequential_cutflow;
    bool _do_sequential_cutflow_histograms;
    boost::optional< std::vector<unsigned int> > _cutflow_order;
    DgCutSetAttachment<objectT> _attachment;
    mutable bool _process_attachment;
  private:
    void _do_extra_studies( objectT const& obj ) {
      BOOST_FOREACH( typename StudyColl::value_type& st , _extra_studies ) {
        st->for_only( obj );
      }
    }
  public:

    DgCutSet( const std::string& name )
      : _name( name )
      , _cuts()
      , _extra_studies()
      , _garbage()
      , _cutnames( new NameColl )
      , _do_sequential_cutflow( true )
      , _do_sequential_cutflow_histograms( true )
      , _cutflow_order()
      , _attachment()
      , _process_attachment( true )
    {}
    DgCutSet( const std::string& name , const DgCutSetAttachment<objectT> attachment )
      : _name( name )
      , _cuts()
      , _extra_studies()
      , _garbage()
      , _cutnames( new NameColl )
      , _do_sequential_cutflow( true )
      , _do_sequential_cutflow_histograms( true )
      , _cutflow_order()
      , _attachment(attachment)
      , _process_attachment( true )
    {}
    
    void based_on( const DgCutSet& base ) {
      std::copy( base._cuts.begin() , base._cuts.end() , std::back_inserter(_cuts) );
      std::copy( base._garbage.begin() , base._garbage.end() , std::back_inserter(_garbage) );
      boost::shared_ptr<NameColl> tmp( new NameColl(*_cutnames) );
      std::copy( base._cutnames->begin() , base._cutnames->end() , std::back_inserter(*tmp) );
      _cutnames.swap( tmp );
      // _cuts = base._cuts;
      // _garbage = base._garbage;
      // _cutnames.reset( new NameColl(*base._cutnames) );
      // explicitly do not copy attachment, since this cutset will select a
      // different list of objects than the base.
    }
    void do_sequential_cutflow( const bool& yes ) { _do_sequential_cutflow = yes; reset_cutflow_order(); }
    void do_sequential_cutflow_histograms( const bool& yes ) { _do_sequential_cutflow_histograms = yes; }
    void process_attachment( const bool& yes ) const { _process_attachment = yes; }
    const bool process_attachment() const { return _process_attachment; }

    void reset_cutflow_order() {
      std::vector<unsigned int> default_order;
      for( int i=0, f=_cuts.size(); i!=f; ++i ) { default_order.push_back( i ); }
      _cutflow_order.reset( default_order );
    }
    //std::vector<unsigned int>& set_cutflow_order() { _cutflow_order.reset( std::vector<unsigned int>() ); }
    void set_cutflow_order( const std::vector<std::string>& tmp ) {};
    // template< typename collT>
    // void set_cutflow_order( typename const collT<std::string>& cutflow_order ) {
    //   std::vector<unsigned int> new_order;
    //   for( typename collT<std::string>::const_iterator i=cutflow_order.begin(), f=cutflow_order.end(); i!=f; ++i,++n ) {
    //     const std::string& cutname( *i );
    //     typename CutColl::const_iterator icut = std::find( _cuts.begin() , _cuts.end() , boost::bind( &DgCutDefinitionType::cutname , _1 ) == cutname );
    //     assert( icut != _cuts.end() && "passed unrecognized cut name in cutflow order" );
    //     const unsigned int n = std::distance( _cuts.begin() , icut );
    //     new_order.push_back( n );
    //   }
    //   _cutflow_order.reset( new_order );
    // };

    // add extra one or more DgStudy to do with various cut combinations applied
    template<typename studyT>
    void do_extra_study( studyT& st ) { 
      typedef typename studyT::type study_type;
      // if this fails, the study is intended for a different type of
      // object than the cutset. for example, the study is a DgStudy<
      // shared_ptr<AnaJet> > study of jets but the cutset is a
      // DgCutSet< shared_ptr<AnaMuon> > to be applied to muons.
      BOOST_STATIC_ASSERT(( boost::is_convertible<study_type,objectT>::value ));
      // add the study to the stack.
      _extra_studies.push_back( boost::dynamic_pointer_cast< DgStudy<objectT> >( boost::make_shared<studyT>(st) ) ); 
    }

    // add the cut defined by predicate with the specified
    // name. only cut efficiency histograms will be produced; no cut
    // variable histograms will be made.
    //
    // predicates / fill functorscould be temporaries, e.g. generated by
    // boost::bind. we need to keep these until the cut set destructor
    // is called.

    // note: until I figure out how to provide generic "add( cutname ,
    // predicate )" and "add( cutname , predicate , histogrammer )"
    // functions, the user has to choose from the (add_ug, add_gg,
    // add_gu, ...) functions based on the type of cut definition
    // predicate and the type of the optional histogram-filling
    // functor. the naming scheme is as follows:
    //
    //  g) the functor does not depend on the object being selected
    //     (i.e. it is not a function of an objectT)
    //
    //  u) the functor takes the object being selected as an argument
    //     (i.e. it is a function of an objectT)
    //
    // For example, if both the predicate and histogram functors
    // depend on the properties of the objectT, use "add_uu". If the
    // call does not provide a histogram functor, use "add_u".
    //
    template<typename bindT>
    void add_g( const std::string& cutname , bindT const& predicate ) {
      _garbage.push_back( predicate );
      _cuts.push_back( boost::make_shared< DgCutDefinitionG<objectT> >( cutname ,
                                                                        typename DgCutDefinitionG<objectT>::PredicateType(boost::any_cast< bindT >( _garbage.back())) ) );
      _cutnames->push_back( cutname );
      assert( _cuts.size() == _cutnames->size() );
      assert( _cuts.back()->cutname() == _cutnames->back() );
    }
    template<typename bindT>
    void add_u( const std::string& cutname , bindT const& predicate ) {
      _garbage.push_back( predicate );
      _cuts.push_back( boost::make_shared< DgCutDefinitionU<objectT> >( cutname ,
                                                                        typename DgCutDefinitionU<objectT>::PredicateType(boost::any_cast< bindT >( _garbage.back())) ) );
      _cutnames->push_back( cutname );
      assert( _cuts.size() == _cutnames->size() );
      assert( _cuts.back()->cutname() == _cutnames->back() );
    }
    template<typename bindT,typename fillT>
    void add_gg( const std::string& cutname , bindT const& predicate , fillT const& fillfunc ) {
      _garbage.push_back( predicate );
      _garbage.push_back( fillfunc );
      _cuts.push_back( boost::make_shared< DgCutDefinitionGG<objectT> >( cutname ,
                                                                         typename DgCutDefinitionGG<objectT>::PredicateType(boost::any_cast< bindT >( *(++_garbage.rbegin()) )) ,
                                                                         typename DgCutDefinitionGG<objectT>::FillType(boost::any_cast< fillT >( _garbage.back())) ) );
      _cutnames->push_back( cutname );
      assert( _cuts.size() == _cutnames->size() );
      assert( _cuts.back()->cutname() == _cutnames->back() );
    }
    template<typename bindT,typename fillT>
    void add_gu( const std::string& cutname , bindT const& predicate , fillT const& fillfunc ) {
      _garbage.push_back( predicate );
      _garbage.push_back( fillfunc );
      _cuts.push_back( boost::make_shared< DgCutDefinitionGU<objectT> >( cutname ,
                                                                         typename DgCutDefinitionGU<objectT>::PredicateType(boost::any_cast< bindT >( *(++_garbage.rbegin()) )) ,
                                                                         typename DgCutDefinitionGU<objectT>::FillType(boost::any_cast< fillT >( _garbage.back())) ) );
      _cutnames->push_back( cutname );
      assert( _cuts.size() == _cutnames->size() );
      assert( _cuts.back()->cutname() == _cutnames->back() );
    }
    template<typename bindT,typename fillT>
    void add_ug( const std::string& cutname , bindT const& predicate , fillT const& fillfunc ) {
      _garbage.push_back( predicate );
      _garbage.push_back( fillfunc );
      _cuts.push_back( boost::make_shared< DgCutDefinitionUG<objectT> >( cutname ,
                                                                         typename DgCutDefinitionUG<objectT>::PredicateType(boost::any_cast< bindT >( *(++_garbage.rbegin()) )) ,
                                                                         typename DgCutDefinitionUG<objectT>::FillType(boost::any_cast< fillT >( _garbage.back())) ) );
      _cutnames->push_back( cutname );
      assert( _cuts.size() == _cutnames->size() );
      assert( _cuts.back()->cutname() == _cutnames->back() );
    }
    template<typename bindT,typename fillT>
    void add_uu( const std::string& cutname , bindT const& predicate , fillT const& fillfunc ) {
      _garbage.push_back( predicate );
      _garbage.push_back( fillfunc );
      _cuts.push_back( boost::make_shared< DgCutDefinitionUU<objectT> >( cutname, 
                                                                         typename DgCutDefinitionUU<objectT>::PredicateType(boost::any_cast< bindT >( *(++_garbage.rbegin()) )) ,
                                                                         typename DgCutDefinitionUU<objectT>::FillType(boost::any_cast< fillT >( _garbage.back())) ) );
      _cutnames->push_back( cutname );
      assert( _cuts.size() == _cutnames->size() );
      assert( _cuts.back()->cutname() == _cutnames->back() );
    }

    // routines for applying cuts and retrieving results.

    // applies a given (named) cut to the entire collection of objects
    // returns true it at least one object passes the cut
    template<typename iteratorT>
    const bool cut_all_objects(iteratorT beg , iteratorT end, std::string cutname ) const {
      const unsigned int n = std::distance( _cutnames->begin() , std::find(_cutnames->begin(),_cutnames->end(),cutname) );
      boost::shared_ptr<DgCutDefinitionType> dgcut = _cuts[n];
      for( ; beg!=end; ++beg ) if (dgcut->test(*beg)) return true;
      return false;
    }

    // applies a set of given (named) cuts to the entire collection of objects
    // returns true it at least one object passes ALL cuts simultaneously
    template<typename iteratorT>
    const bool cut_all_objects(iteratorT beg , iteratorT end, const std::list<std::string>& cutnames) const {
      std::list<boost::shared_ptr<DgCutDefinitionType> > dgcuts;
      for(std::list<std::string>::const_iterator cutname=cutnames.begin(); cutname!=cutnames.end(); cutname++) {
        const unsigned int n = std::distance( _cutnames->begin() , std::find(_cutnames->begin(),_cutnames->end(),*cutname) );
        dgcuts.push_back(_cuts[n]);
      }
      // loop over objects
      for( ; beg!=end; ++beg ) {
        bool passed=true;
        // loop over requested cuts and require that all are passed
        for(typename std::list<boost::shared_ptr<DgCutDefinitionType> >::const_iterator dgcut=dgcuts.begin(); dgcut!=dgcuts.end(); dgcut++) {
          if (!(*dgcut)->test(*beg)) {
            passed=false;
            break;
          }
        }
        if(passed) return true;
      }
      return false;
    }
    
    // evaluate cuts and produce a DgCutResult encoding the result for
    // each cut.
    const DataGraphics::DgCutResult cut( objectT const& obj ) const {
      // loop over cuts and apply each.
      DgCutResult result( _cutnames );
      result.set_all_fail();
      unsigned int n = 0u;
      for( typename CutColl::const_iterator i=_cuts.begin(), f=_cuts.end(); i!=f; ++i,++n ) {
        assert( std::find(_cutnames->begin(),_cutnames->end(),(*i)->cutname()) != _cutnames->end() );
        assert( std::distance( _cutnames->begin() , std::find(_cutnames->begin(),_cutnames->end(),(*i)->cutname()) ) == n );
        boost::shared_ptr<DgCutDefinitionType> dgcut( *i );
        // evaluate cut
        if( dgcut->test(obj) ) {
          result.set_pass(n);
        }
      }
      return result;
    }
    // check whether any cuts fail. do not record individual cut results.
    const bool passes_all( objectT const& obj ) const {
      // loop over cuts and apply each. if any fail, return false.
      for( typename CutColl::const_iterator i=_cuts.begin(), f=_cuts.end(); i!=f; ++i ) {
        const boost::shared_ptr<DgCutDefinitionType> dgcut( *i );
        // evaluate cut
        if( !(dgcut->test(obj)) ) { return false; }
      }
      return true;
    }
    // simply check whether all cuts pass or any cuts fail, as a function object.
    const bool operator()( objectT const& obj ) const { return passes_all(obj); }
    

    // routines for processing input objects for the cut study.
    template<typename iteratorT,typename predicateT>
    void for_each_if( iteratorT beg , iteratorT end , predicateT predicate ) {
      bool firstpass = true;
      dg::down( _name , "results of applying a set of cuts" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
      for( ; beg!=end; ++beg ) {
        assert( *beg );
        typename iteratorT::value_type v( *beg );
        if( !predicate(v) ) { continue; }
        const DgCutResult cutresult( cut(v) );
        // plot uncut variables, n-1 variables, and n-1
        // efficiencies. do uncut and n-1 separately to avoid the
        // dg::down lookup penalty. this is probably non-optimal for
        // large sets of cuts because of cache misses.
        if( true ) { 
          dg::down( "uncut" , "before any cuts applied" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
          BOOST_FOREACH( boost::shared_ptr<DgCutDefinitionType> dgcut , _cuts ) {
            assert( dgcut );
            dgcut->fill( dgcut->cutname() , v );
          } // end fill uncut histograms for each cut variable
          _do_extra_studies( v );
        } // end do uncut histograms
        if( true ) { 
          dg::down( "nm1" , "n-1 cut histograms" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
          unsigned int ncut=0u;
          for( typename CutColl::const_iterator i=_cuts.begin(), f=_cuts.end(); i!=f; ++i,++ncut ) {
            boost::shared_ptr<DgCutDefinitionType> dgcut( *i );
            assert( dgcut );
            const bool nm1ok = cutresult.all_others_pass(ncut);
            //dg::filleff( dgcut->cutname()+"_allbut_eff" , nm1ok );
            if( !nm1ok ) { continue; }
            // nm1==ok
            dgcut->fill( dgcut->cutname() , v );
            dg::filleff( dgcut->cutname()+"_eff" , cutresult.passes(ncut) );
            if( !_extra_studies.empty() ) {
              dg::down( dgcut->cutname() , "studies after n-1 cut" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
              _do_extra_studies( v );
            }
          } // end fill n-1 histograms for each cut variable
        } // end do n-1 histograms and n-1 efficiencies
        if( _do_sequential_cutflow ) {
          dg::down( "cutflow" , "cutflow" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
          if( _do_sequential_cutflow_histograms ) { 
            dg::down( "s0_uncut" , "all variables after prior stage cuts" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
            for( typename CutColl::const_iterator i=_cuts.begin(), f=_cuts.end(); i!=f; ++i ) {
              boost::shared_ptr<DgCutDefinitionType> dgcut( *i );
              dgcut->fill( dgcut->cutname() , v );
            }
          }
          bool cut_accumulator = true;
          unsigned int ncut=0u;
          // for( typename CutColl::const_iterator i=_cuts.begin(), f=_cuts.end(); i!=f; ++i,++ncut ) {
          //   boost::shared_ptr<DgCutDefinitionType> dgcut( *i );
          // assert( _cutflow_order );
          // unsigned int norder = 1u;
          // for( std::vector<unsigned int>::const_iterator iorder=_cutflow_order->begin() , forder=_cutflow_order->end(); iorder!=forder; ++iorder,++norder ) {
          //   const unsigned int ncut( *iorder );
          //   const boost::shared_ptr<DgCutDefinitionType>& dgcut( _cuts[ncut] );
	  //
          unsigned int norder = 1u;
          for( typename CutColl::const_iterator i=_cuts.begin(), f=_cuts.end(); i!=f; ++i,++ncut,++norder ) {
            boost::shared_ptr<DgCutDefinitionType> dgcut( *i );
            const bool cutok = cutresult.passes( ncut );
            // cumulative efficiency
            dg::filleff( (boost::format( "s%|d|_%|s|_cumeff" ) % norder % dgcut->cutname()).str() , cut_accumulator && cutok );
            // step efficiency
            if( cut_accumulator ) { dg::filleff( (boost::format( "s%|d|_%|s|_steff" ) % norder % dgcut->cutname()).str() , cutok ); }
	    // event cumulative efficiency
	    if( firstpass ) {
	      if( cut_accumulator && cutok ) { 
		dg::filleff( (boost::format( "s%|d|_%|s|_ge1eff" ) % norder % dgcut->cutname()).str() , true ); 
		firstpass = false; 
	      } else if( ncut == _cuts.size()-1 ) {
		dg::filleff( (boost::format( "s%|d|_%|s|_ge1eff" ) % norder % dgcut->cutname()).str() , false ); 
	      }
	    }
	    // histogram of cutflow, one bin per cut
	    if( cut_accumulator && cutok ) {
	      dg::fillh( "cumeff_hist" , _cuts.size() , 0 , _cuts.size() , ncut );
	    }
            cut_accumulator = cut_accumulator && cutok;
            if( _do_sequential_cutflow_histograms && cut_accumulator ) { 
              dg::down( (boost::format( "s%|d|_%|s|" ) % norder % dgcut->cutname()).str() , "all variables after prior stage cuts" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
              for( typename CutColl::const_iterator j=_cuts.begin(), jf=_cuts.end(); j!=jf; ++j ) {
                boost::shared_ptr<DgCutDefinitionType> tmpdgcut( *j );
                tmpdgcut->fill( tmpdgcut->cutname() , v );
              }
              _do_extra_studies( v );
            }
          } // end fill n-1 histograms for each cut variable
        } // do cutflow efficiencies and histograms
        if( true ) { 
          dg::down( "cut" , "after all cuts applied" ); BOOST_SCOPE_EXIT(void) { dg::up(); } BOOST_SCOPE_EXIT_END;
          dg::filleff( "eff_all" , cutresult.all_pass() );
          if( cutresult.all_pass() ) { 
            if( _process_attachment ) { _attachment.insert( v ); }
            BOOST_FOREACH( boost::shared_ptr<DgCutDefinitionType> dgcut , _cuts ) {
              dgcut->fill( dgcut->cutname() , v );
            } // end fill uncut histograms for each cut variable
            _do_extra_studies( v );
          }
        } // end if do plots of efficiency and cut variables after all cuts
      } // end for each object in range
    } // end for_each_if

    class constTruePred {
    public:
      const bool operator()( objectT const& obj ) const { return true; }
    };

    template<typename iteratorT>
    void for_each( iteratorT beg , iteratorT end ) {
      static constTruePred true_predicate;
      for_each_if( beg , end , true_predicate );
    }

    void for_only( objectT const& obj ) {
      std::vector<objectT> tmp;
      tmp.push_back( obj );
      for_each( tmp.begin() , tmp.end() );
    }

  };

  // some helper functions/classes
  template< typename objectT >
  class abs_for {
  public:
    typedef const typename std::pointer_to_unary_function<const typename objectT::ftype,const typename objectT::ftype> result_type;
    result_type operator()() {
      return std::ptr_fun<const typename objectT::ftype,const typename objectT::ftype>(std::abs);
    }
  };

};

 


// A few simplifying preprocessor macros (to be used in dgSTL::copy_if / std::count_if)
#define CUT_ONE(ANATYPE,CUTSET,CUT)                                     \
  bind(static_cast< const bool (DgCutResult::*) (const DgCutResult::Label&) const >(&DgCutResult::passes), \
       bind(&DgCutSet< boost::shared_ptr<const ANATYPE> >::cut,CUTSET,_1), CUT)==true
#define CUT_UPTO(ANATYPE,CUTSET,CUT)                                    \
  bind(static_cast< const bool (DgCutResult::*) (const DgCutResult::Label&) const >(&DgCutResult::passes_upto), \
       bind(&DgCutSet< boost::shared_ptr<const ANATYPE> >::cut,CUTSET,_1), CUT)==true
#define CUT_ALL(ANATYPE,CUTSET)                                         \
  bind(&DgCutResult::all_pass,bind(&DgCutSet< boost::shared_ptr<const ANATYPE> >::cut,CUTSET,_1))==true

#endif // WRAP_DGCUTSET_HH
