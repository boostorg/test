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
//  Description : defines the is_forward_iterable collection type trait
// ***************************************************************************

#ifndef BOOST_TEST_IS_FRWARD_ITERABLE_HPP_110612GER
#define BOOST_TEST_IS_FRWARD_ITERABLE_HPP_110612GER

#if 1 // def BOOST_NO_CXX11_DECLTYPE
// Boost
#include <boost/mpl/bool.hpp>

// STL
#include <list>
#include <vector>

#else

// STL
#include <type_traits> 
#include <utility> 

#endif
//____________________________________________________________________________//

namespace boost {
namespace unit_test {

// ************************************************************************** //
// **************             is_forward_iterable              ************** //
// ************************************************************************** //

#if 1 // def BOOST_NO_CXX11_DECLTYPE
template<typename T>
struct is_forward_iterable : mpl::false_ {};

template<typename T>
struct is_forward_iterable<T const> : is_forward_iterable<T> {};

template<typename T>
struct is_forward_iterable<T&> : is_forward_iterable<T> {};

template<typename T>
struct is_forward_iterable<std::vector<T>> : mpl::true_ {};

template<typename T>
struct is_forward_iterable<std::list<T>> : mpl::true_ {};

#else

namespace detail {

struct is_forward_iterable_impl
{ 
    template<typename T, 
             typename = decltype(std::declval<const T&>().size()),
             typename = decltype(std::declval<T>().begin()), 
             typename = typename T::const_iterator,
             typename = typename T::value_type
    > 
    static std::true_type   test(int); 
    template<typename> 
    static std::false_type  test(...); 
}; 


} // namespace detail

template<typename T> 
struct is_forward_iterable 
: decltype(detail::is_forward_iterable_impl::test<typename std::remove_reference<T>::type>(0))
{ 
}; 

#endif

} // namespace unit_test
} // namespace boost

#endif // BOOST_TEST_IS_FRWARD_ITERABLE_HPP_110612GER

