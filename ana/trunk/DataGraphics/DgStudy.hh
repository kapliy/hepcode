#ifndef WRAP_DGSTUDY_HH
#define WRAP_DGSTUDY_HH

// base, abstract class used to encapsulate a "study" of a collection of some type of objects.
// for example, one might want to make some standard plots of all the parameters of a track fit.
// the code to do that can go into a derived class.
//

#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <functional>
#include <boost/foreach.hpp>
#include <boost/static_assert.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/any.hpp>
#include "DataGraphics/DgCollection.hh"

namespace
DataGraphics
{

  template<typename objectT> // object type, defined by derived class
  class
  DgStudy
  {

  private:

    bool _finished;
    // study name and description (TDirectory or DgCollection name and title/description)
    std::string _name;
    std::string _description;
    // subdivide the sample into these categories and make subdirectories of plots for each.
    typedef std::pair< std::string , boost::function<bool (objectT const&) > > CategoryPairUnary;
    std::vector< CategoryPairUnary > _category_visitor_interfaces;
    std::vector< boost::any > _category_visitor_copies;
    // optional selector(s) to choose single "best" (e.g. closest to Z mass) object and plot separately
    //   selector should take two objectT and return the better one.
    typedef std::pair< std::string , boost::function< objectT const& ( objectT const& , objectT const& ) > > GroupSelectorUnary;
    std::vector< GroupSelectorUnary > _group_selectors;
    std::vector< boost::any > _group_selector_copies;
    // category functors to handle cases when the compiler discovers the user's category criteria are always true or false
    struct ConstantTruePredicate : public boost::function<bool (objectT const&) > {
      const bool operator()( objectT const& ) { return true; }
    };
    struct ConstantFalsePredicate : public boost::function<bool (objectT const&) > {
      const bool operator()( objectT const& ) { return false; }
    };
    ConstantTruePredicate _constant_true_predicate;
    ConstantFalsePredicate _constant_false_predicate;
    // set to make plots in the top folder (uncategorized)
    bool _study_uncategorized;
    // set to make plots in subfolders (categories)
    bool _study_categorized; 
    // set to reflect the status of current study
    bool _now_uncategorized;
    // switch: plot number of objects in each for_each block.
    bool _study_n_objects;

  private:

    void _begin_iteration() {
      DgCollection::down( _name , _description );
      // finished set to false on first for_each* iteration
    }
    void _end_iteration() { 
      DgCollection::up(); 
      _finish();
    }

    const bool _satisfies_category( const CategoryPairUnary& catpair , objectT const& obj ) const { return( catpair.second(obj) ); }
    objectT const& _selected_object( const GroupSelectorUnary& sel , objectT const& objA , objectT const& objB ) const { return( sel.second(objA,objB) ); }

    // initialize data structure to keep track of the selection of
    // each group selector for uncategorized and each category. first element in the vector is the
    // uncategorized best object and the remainder are for each category.
    typedef typename std::vector< std::vector<const objectT*> > BlockSelected;
    const BlockSelected _initialize_block_selected() const {
      if( _group_selectors.empty() ) { static BlockSelected null; return null; }
      BlockSelected selected;
      std::vector<const objectT*> empty_vec( _category_visitor_interfaces.size()+1 , 0 );
      BOOST_FOREACH( const GroupSelectorUnary& sel , _group_selectors ) {
        selected.push_back( empty_vec );
      }
      return selected;
    }
    void _update_block_selected( BlockSelected& selected , objectT const& obj ) const {
      if( _group_selectors.empty() ) { return; }
      assert( selected.size() == _group_selectors.size() );
      typename std::vector< GroupSelectorUnary >::const_iterator isel = _group_selectors.begin();
      for( typename BlockSelected::iterator icurvec=selected.begin(), fcurvec=selected.end(); icurvec!=fcurvec; ++icurvec, ++isel ) {
        std::vector<const objectT*>& vec( *icurvec );
        assert( vec.size() == _category_visitor_interfaces.size()+1 );
        typename std::vector<const objectT*>::iterator icur = vec.begin();
        // update first "uncategorized" entry.
        assert( !vec.empty() );
        // if no best uncategorized object yet, just set it to the
        // first one we see else compare current best object with this
        // one, and update.
        if( _study_uncategorized ) { 
          if( !(*icur) ) { 
            (*icur) = &obj; 
          } else {
            (*icur) = & _selected_object( *isel , **icur , obj );
          }
        }
        ++icur;
        // do the same for each subcategory.
        for( typename std::vector< CategoryPairUnary >::const_iterator icat=_category_visitor_interfaces.begin(), fcat=_category_visitor_interfaces.end(); icat!=fcat; ++icat,++icur ) {
          if( !_satisfies_category( *icat , obj ) ) { continue; }
          if( !(*icur) ) { 
            (*icur) = &obj; 
          } else {
            (*icur) = & _selected_object( *isel , **icur , obj );
          }
        }
      }
    }
    // for each selector, do study for uncat. best object and best object in each category, if it/they exist.
    void _end_block_selected( const BlockSelected& selected ) {
      if( _group_selectors.empty() ) { return; }
      assert( selected.size() == _group_selectors.size() );
      typename std::vector< GroupSelectorUnary >::const_iterator isel = _group_selectors.begin();
      for( typename BlockSelected::const_iterator icurvec=selected.begin(), fcurvec=selected.end(); icurvec!=fcurvec; ++icurvec, ++isel ) {
        typename std::vector<const objectT*> const & vec( *icurvec );
        assert( vec.size() == _category_visitor_interfaces.size()+1 );
        if( vec.empty() ) { continue; }
        typename std::vector<const objectT*>::const_iterator icur = vec.begin();
        // if no uncategorized object, there will be no subcategory objects either.
        if( !(*icur) ) { continue; }
        dg::down( isel->first , "selector study" );
        if( _study_uncategorized && (*icur) ) {
          _now_uncategorized = true;
          _study( **icur );
          _now_uncategorized = false;
        }
        ++icur;
        // do the same for each subcategory.
        for( typename std::vector< CategoryPairUnary >::const_iterator icat=_category_visitor_interfaces.begin(), fcat=_category_visitor_interfaces.end(); icat!=fcat; ++icat,++icur ) {
          if( !(*icur) ) { continue; }
          dg::down( icat->first , "category and selector study" );
          _study( **icur );
          dg::up();
        }
        dg::up(); // end selector study
      }
    }

  protected:

    // do the study, whatever it is.
    virtual void _study( objectT const& obj ) = 0;
    // finish/reset the persistent part of the study.
    void _finish() { _finished = true; }  

  public:

    DgStudy( const std::string& name , const std::string& description ) 
      : _name( name )
      , _description( description )
      , _finished( true )
      , _study_uncategorized( true ) 
      , _study_categorized( true ) 
      , _now_uncategorized( false)
      , _study_n_objects( false )
    {}
    virtual ~DgStudy() { 
      if( !_finished ) { std::cout << _name << ": you must call DgStudy::finish when done." << std::endl; }
      assert( _finished ); 
    }

    const std::string& name() const { return _name; }
    const std::string& description() const { return _description; }  

    void based_on ( const DgStudy& s ) {
      std::copy( s._category_visitor_interfaces.begin() , s._category_visitor_interfaces.end() , std::back_inserter(_category_visitor_interfaces) );
      std::copy( s._category_visitor_copies.begin() , s._category_visitor_copies.end() , std::back_inserter(_category_visitor_copies) );
      std::copy( s._group_selectors.begin() , s._group_selectors.end() , std::back_inserter(_group_selectors) );
      std::copy( s._group_selector_copies.begin() , s._group_selector_copies.end() , std::back_inserter(_group_selector_copies) );
      _study_uncategorized = s._study_uncategorized;
      _study_categorized = s._study_categorized; 
      _study_n_objects = s._now_uncategorized;
    }
    void repurpose( const std::string& name , const std::string& description ) {
      if( !_finished ) { _finish(); }; // finish off the last part of the study before starting anew. crucial.
      _name = name;
      _description = description;
    }
    template<typename bindT>
    void add_category( const std::string& category_description , bindT const& pred ) {
      // pred could be a temporary, generated by boost::bind. we need
      // to keep it until the study destructor is called, so that
      // calls to _satisfies_category can find it. so, save it to a
      // vector<boost::any> and let it die with the DgStudy
      // destructor.
      _category_visitor_copies.push_back( pred );
      _category_visitor_interfaces.push_back( CategoryPairUnary(category_description,boost::any_cast<bindT>(_category_visitor_copies.back())) );
    }
    void add_constant_category( const std::string& category_description , const bool& constant_pred ) {
      add_category( category_description , boost::bind( std::equal_to<bool>() , true , constant_pred ) );
    }
    // this is equivalent add_category, where the predicate is AND'ed with an event-wide constant_pred condition
    template<typename bindT>
    void add_category_with_constant( const std::string& category_description , bindT const& pred , const bool& constant_pred) {
      add_category( category_description , pred && boost::bind( std::equal_to<bool>() , true , constant_pred ) );
    }
    template<typename bindT>
    void add_selector( const std::string& selector_description , bindT const& selector ) {
      // add a "group selector" which picks out a single object for
      // separate study.  useful e.g. for making separate set of plots
      // for the highest pT lepton in the event, or the AnaZ with dimuon
      // mass closest to the Z pole mass, etc.
      _group_selector_copies.push_back( selector );
      _group_selectors.push_back( GroupSelectorUnary(selector_description,boost::any_cast<bindT>(_group_selector_copies.back())) );
    }
    void study_uncategorized( const bool& yes ) { _study_uncategorized = yes; }
    bool uncategorized() const { return _now_uncategorized; }
    bool categorized() const { return !uncategorized(); }
    void study_categorized( const bool& yes ) { _study_categorized = yes; }
    void study_n_objects( const bool& yes ) { _study_n_objects = yes; }

    // routines for processing input objects for the study.
    std::vector<bool> for_only( objectT const& obj ) { // a single input object
      assert( _finished );
      _begin_iteration();
      if( _study_uncategorized ) { 
        _now_uncategorized = true;
        _study( obj );
        _now_uncategorized = false;
      }
      std::vector<bool> nobj_cat( _category_visitor_interfaces.size() , false );
      unsigned int icat = 0u;
      BOOST_FOREACH( const CategoryPairUnary& catpair , _category_visitor_interfaces ) {
	if( _satisfies_category(catpair,obj) ) {
	  nobj_cat[icat] = true;
	  if( _study_categorized ) { 
	    dg::down( catpair.first , "category study" );
	    _study( obj );
	    dg::up();
	  }
	}
	icat++;
      }
      _end_iteration();
      return nobj_cat;
    }
    template<typename iteratorT>
    std::vector<unsigned int> for_each( iteratorT beg , iteratorT end ) { // a range of input objects in a collection
      assert( _finished );
      _begin_iteration();
      BlockSelected selected( _initialize_block_selected() );
      // study each object
      unsigned int nobj = 0u;
      std::vector<unsigned int> nobj_cat( _category_visitor_interfaces.size() , 0u );
      for( ; beg!=end; ++beg, ++nobj ) {
        if( _study_uncategorized ) {
          _now_uncategorized = true;
          _study( *beg ); 
          _now_uncategorized = false;
        }
        unsigned int icat = 0u;
        BOOST_FOREACH( const CategoryPairUnary& catpair , _category_visitor_interfaces ) {
          if( _satisfies_category(catpair,*beg) ) {
            ++(nobj_cat[icat]);
            if( _study_categorized ) { 
              dg::down( catpair.first , "category study" );
              _study( *beg );
              dg::up();
            }
          }
          ++icat;
        }
        _finished = false;
        _update_block_selected( selected , *beg );
      }
      // study selected objects separately.
      _end_block_selected( selected );
      // study n objects
      if( _study_n_objects ) {
        dg::fillh( "number_of" , 100 , 0 , 100 , nobj , "n" );
        unsigned int icat = 0u;
        BOOST_FOREACH( const CategoryPairUnary& catpair , _category_visitor_interfaces ) {
          dg::down( catpair.first , "category study" );
          dg::fillh( "number_of" , 100 , 0 , 100 , nobj_cat[icat] , "n" );
          dg::up();
          ++icat;
        }
      } // end study n objects
      // end this iteration
      _end_iteration();
      return nobj_cat;
    }
    // conditional for_each. do study only if predicate is true for the object.
    template<typename iteratorT,typename predicateT>
    std::vector<unsigned int> for_each_if( iteratorT beg , iteratorT end , predicateT predicate ) {
      assert( _finished );
      _begin_iteration();
      BlockSelected selected( _initialize_block_selected() );
      // study each object
      unsigned int nobj = 0u;
      std::vector<unsigned int> nobj_cat( _category_visitor_interfaces.size() , 0u );
      for( ; beg!=end; ++beg ) {
        if( !predicate(*beg) ) { continue; }
        ++nobj;
        if( _study_uncategorized ) { 
          _now_uncategorized = true;
          _study( *beg ); 
          _now_uncategorized = false;
        }
        unsigned int icat = 0u;
        BOOST_FOREACH( const CategoryPairUnary& catpair , _category_visitor_interfaces ) {
          if( _satisfies_category(catpair,*beg) ) {
            ++(nobj_cat[icat]);
	    if( _study_categorized ) { 
	      dg::down( catpair.first , "category study" );
	      _study( *beg );
	      dg::up();
	    }
          }
          ++icat;
        }
        _finished = false;
        _update_block_selected( selected , *beg );
      }
      // study selected objects separately.
      _end_block_selected( selected );
      // study n objects
      if( _study_n_objects ) {
        dg::fillh( "number_of" , 100 , 0 , 100 , nobj , "n" );
        unsigned int icat = 0u;
        BOOST_FOREACH( const CategoryPairUnary& catpair , _category_visitor_interfaces ) {
          dg::down( catpair.first , "category study" );
          dg::fillh( "number_of" , 100 , 0 , 100 , nobj_cat[icat] , "n" );
          dg::up();
          ++icat;
        }
      } // end study n objects
      _end_iteration();
      return nobj_cat;
    }
  };
  
};

#endif // WRAP_DGSTUDY_HH
