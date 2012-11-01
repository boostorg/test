//  (C) Copyright Gennadiy Rozental 2012.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : forward declares monomorphic datasets interfaces
// ***************************************************************************

#ifndef BOOST_TEST_DATA_MONOMORPHIC_FWD_HPP_102212GER
#define BOOST_TEST_DATA_MONOMORPHIC_FWD_HPP_102212GER

// Boost.Test
#include <boost/test/data/config.hpp>
#include <boost/test/data/size.hpp>

// Boost
#include <boost/utility/declval.hpp>

// STL
#include <list>
#include <vector>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {
namespace data {

// ************************************************************************** //
// **************              is_std_collection               ************** //
// ************************************************************************** //

namespace ds_detail {

template<typename T>
struct is_std_collection : std::false_type {};

template<typename T>
struct is_std_collection<T const> : is_std_collection<T> {};

template<typename T>
struct is_std_collection<T&> : is_std_collection<T> {};

template<typename T>
struct is_std_collection<std::vector<T>> : std::true_type {};

template<typename T>
struct is_std_collection<std::list<T>> : std::true_type {};

} // namespace ds_detail

namespace monomorphic {

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

// ************************************************************************** //
// **************            monomorphic::is_dataset           ************** //
// ************************************************************************** //

template<typename DS>
struct is_dataset : std::false_type {};

//____________________________________________________________________________//

template<typename DS>
struct is_dataset<DS&> : is_dataset<DS> {};

//____________________________________________________________________________//

template<typename DS>
struct is_dataset<DS const> : is_dataset<DS> {};

//____________________________________________________________________________//

} // namespace monomorphic

// ************************************************************************** //
// **************                  data::make                  ************** //
// ************************************************************************** //

template<typename DS>
inline typename std::enable_if<monomorphic::is_dataset<DS>::value,DS>::type
make(DS&& ds)
{
    return std::forward<DS>( ds );
}

//____________________________________________________________________________//

template<typename T>
inline typename std::enable_if<!ds_detail::is_std_collection<T>::value &&
                               !monomorphic::is_dataset<T>::value,
                               monomorphic::singleton<T> >::type
make( T&& v );

//____________________________________________________________________________//

template<typename C>
inline monomorphic::collection<typename std::enable_if<ds_detail::is_std_collection<C>::value,C>::type>
make( C&& c );

//____________________________________________________________________________//

template<typename T, std::size_t size>
inline monomorphic::array<T>
make( T (&a)[size] );

//____________________________________________________________________________//

inline monomorphic::singleton<char*>
make( char* str );

//____________________________________________________________________________//

inline monomorphic::singleton<char const*>
make( char const* str );

//____________________________________________________________________________//

namespace result_of {

template<typename DS>
struct make
{
    typedef decltype(data::make(boost::declval<DS>())) type;
};

} // namespace result_of

//____________________________________________________________________________//

} // namespace data
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_DATA_MONOMORPHIC_FWD_HPP_102212GER

