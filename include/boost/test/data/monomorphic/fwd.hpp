//  (C) Copyright Gennadiy Rozental 2012-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// Forward declares monomorphic datasets interfaces
// ***************************************************************************

#ifndef BOOST_TEST_DATA_MONOMORPHIC_FWD_HPP_102212GER
#define BOOST_TEST_DATA_MONOMORPHIC_FWD_HPP_102212GER

// Boost.Test
#include <boost/test/data/config.hpp>
#include <boost/test/data/size.hpp>

#include <boost/test/utils/is_forward_iterable.hpp>

// Boost
#ifdef BOOST_NO_CXX11_RVALUE_REFERENCES
#include <boost/utility/enable_if.hpp>
#else
#include <boost/utility/declval.hpp>
#endif
#include <boost/mpl/bool.hpp>
#include <boost/smart_ptr/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/type_traits/decay.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {
namespace data {

namespace monomorphic {


#if !defined(BOOST_TEST_DOXYGEN_DOC__)
template<typename T>
struct traits;

template<typename T>
class dataset;

template<typename T>
class singleton;

template<typename C>
class collection;

template<typename T>
class array;
#endif

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
#  define BOOST_TEST_ENABLE_IF std::enable_if
#else
#  define BOOST_TEST_ENABLE_IF boost::enable_if_c
#endif

// ************************************************************************** //
// **************            monomorphic::is_dataset           ************** //
// ************************************************************************** //

//! Helper metafunction indicating if the specified type is a dataset.
template<typename DS>
struct is_dataset : mpl::false_ {};

//! A reference to a dataset is a dataset
template<typename DS>
struct is_dataset<DS&> : is_dataset<DS> {};

//! A const dataset is a dataset
template<typename DS>
struct is_dataset<DS const> : is_dataset<DS> {};

//____________________________________________________________________________//

} // namespace monomorphic

// ************************************************************************** //
// **************                  data::make                  ************** //
// ************************************************************************** //

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES


//! @brief Creates a dataset from a value, a collection or an array
//!
//! This function has several overloads:
//! @code
//! // returns ds if ds is already a dataset
//! template <typename DS> DS make(DS&& ds);
//!
//! // creates a singleton dataset, for non forward iterable and non dataset type T
//! // (a C string is not considered as a sequence).
//! template <typename T> monomorphic::singleton<T> make(T&& v);
//! monomorphic::singleton<char*> make( char* str );
//! monomorphic::singleton<char const*> make( char const* str );
//!
//! // creates a collection dataset, for forward iterable and non dataset type C
//! template <typename C> monomorphic::collection<C> make(C && c);
//!
//! // creates an array dataset
//! template<typename T, std::size_t size> monomorphic::array<T> make( T (&a)[size] );
//! @endcode
template<typename DS>
inline typename BOOST_TEST_ENABLE_IF<monomorphic::is_dataset<DS>::value,DS>::type
make(DS&& ds)
{
    return std::forward<DS>( ds );
}


// warning: doxygen is apparently unable to handle @overload from different files, so if the overloads
// below are not declared with @overload in THIS file, they do not appear in the documentation.

// fwrd declaration for singletons
//! @overload boost::unit_test::data::make()
template<typename T>
inline typename BOOST_TEST_ENABLE_IF<!is_forward_iterable<T>::value &&
                                     !monomorphic::is_dataset<T>::value,
                                     monomorphic::singleton<T> >::type
make( T&& v );


//! @overload boost::unit_test::data::make()
template<typename C>
inline monomorphic::collection<typename BOOST_TEST_ENABLE_IF<unit_test::is_forward_iterable<C>::value,C>::type>
make( C&& c );


#else  // !BOOST_NO_CXX11_RVALUE_REFERENCES

//! @overload boost::unit_test:data::make()
template<typename DS>
inline typename BOOST_TEST_ENABLE_IF<monomorphic::is_dataset<DS>::value,DS const&>::type
make(DS const& ds)
{
    return ds;
}


// fwrd declaration for singletons
//! @overload boost::unit_test::data::make()
template<typename T>
inline typename BOOST_TEST_ENABLE_IF<!is_forward_iterable<T>::value &&
                                     !monomorphic::is_dataset<T>::value,
                                     monomorphic::singleton<T> >::type
make( T const& v );


// fwrd declaration for collections
//! @overload boost::unit_test::data::make()
template<typename C>
inline monomorphic::collection<typename BOOST_TEST_ENABLE_IF<unit_test::is_forward_iterable<C>::value,C>::type>
make( C const& c );

//____________________________________________________________________________//



#endif // !BOOST_NO_CXX11_RVALUE_REFERENCES




// fwrd declarations
//! @overload boost::unit_test::data::make()
template<typename T, std::size_t size>
inline monomorphic::array<T>
make( T (&a)[size] );

//! @overload boost::unit_test::data::make()
inline monomorphic::singleton<char*>
make( char* str );

//! @overload boost::unit_test::data::make()
inline monomorphic::singleton<char const*>
make( char const* str );



//____________________________________________________________________________//

#ifndef BOOST_NO_CXX11_DECLTYPE

namespace result_of {

//! Result of the make call.
template<typename DS>
struct make
{
    typedef decltype(data::make(boost::declval<DS>())) type;
};

} // namespace result_of

#endif // BOOST_NO_CXX11_DECLTYPE


//____________________________________________________________________________//

} // namespace data
} // namespace unit_test
} // namespace boost





#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_DATA_MONOMORPHIC_FWD_HPP_102212GER

