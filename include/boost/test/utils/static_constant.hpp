//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : helper template to prevent ODR violations
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_STATIC_CONSTANT_HPP
#define BOOST_TEST_UTILS_STATIC_CONSTANT_HPP

// C Runtime
#include <cstddef>

namespace boost {
namespace unit_test {

// ************************************************************************** //
// **************                  static_constant             ************** //
// ************************************************************************** //

template<class T>
struct static_constant {
    static T value;
};

template<class T>
T static_constant<T>::value;

} // namespace unit_test
} // namespace boost

#endif // BOOST_TEST_UTILS_STATIC_CONSTANT_HPP
