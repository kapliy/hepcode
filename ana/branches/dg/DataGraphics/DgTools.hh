#ifndef WRAP_DGDATAGRAPHICS_TOOLS_HH
#define WRAP_DGDATAGRAPHICS_TOOLS_HH

#include <DataGraphics/DgBinomialErrors.hh>
#include <string>

namespace
DataGraphics
{
  namespace Tools
  {

    // functor which deletes any pointer passed to it.
    struct DeleteFunctor {
      typedef void result_type;
      template <typename T> 
      result_type operator()(const T* ptr) const { delete ptr; }
    };

    // binary_compose_binary:  h(x,y) = f( g(x) , g(y) )
    template<class AdaptableBinaryFunction,class AdaptableUnaryFunction>
    class binary_compose_binary: public std::binary_function<typename AdaptableUnaryFunction::argument_type ,
                                                             typename AdaptableUnaryFunction::argument_type ,
                                                             typename AdaptableBinaryFunction::result_type> {
    protected:
      AdaptableBinaryFunction _bfunc;
      AdaptableUnaryFunction  _ufunc;
    public:
      binary_compose_binary( const AdaptableBinaryFunction& bfunc , const AdaptableUnaryFunction& ufunc ) : _bfunc(bfunc) , _ufunc(ufunc) {}

      typename AdaptableBinaryFunction::result_type
      operator()( const typename AdaptableUnaryFunction::argument_type& x , const typename AdaptableUnaryFunction::argument_type& y ) {
        return _bfunc( _ufunc(x) , _ufunc(y) );
      }
    };
    // helper function for binary_compose_binary
    template<class AdaptableBinaryFunction,class AdaptableUnaryFunction>
    inline binary_compose_binary<AdaptableBinaryFunction,AdaptableUnaryFunction>
    compose2binary( const AdaptableBinaryFunction& bfunc , const AdaptableUnaryFunction& ufunc ) {
      return binary_compose_binary<AdaptableBinaryFunction,AdaptableUnaryFunction>(bfunc,ufunc);
    }
    
    const std::string expand_filename_to_valid_unique( std::string pattern );
    const std::string expand_filename_to_valid( std::string pattern );

  };
};

#endif // WRAP_DGDATAGRAPHICS_TOOLS_HH
