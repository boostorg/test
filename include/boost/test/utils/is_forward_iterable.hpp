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
//  Description : defines a special purpose type trait
// ***************************************************************************

#ifndef BOOST_TEST_IS_FRWARD_ITERABLE_HPP_110612GER
#define BOOST_TEST_IS_FRWARD_ITERABLE_HPP_110612GER

// Boost

// STL
#include <list>
#include <vector>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {

// ************************************************************************** //
// **************             is_forward_iterable              ************** //
// ************************************************************************** //

template<typename T>
struct is_forward_iterable : std::false_type {};

template<typename T>
struct is_forward_iterable<T const> : is_forward_iterable<T> {};

template<typename T>
struct is_forward_iterable<T&> : is_forward_iterable<T> {};

template<typename T>
struct is_forward_iterable<std::vector<T>> : std::true_type {};

template<typename T>
struct is_forward_iterable<std::list<T>> : std::true_type {};

} // namespace unit_test
} // namespace boost

#endif // BOOST_TEST_IS_FRWARD_ITERABLE_HPP_110612GER

