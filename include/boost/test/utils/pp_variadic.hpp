//  (C) Copyright Gennadiy Rozental 2005-2012.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 81012 $
//
//  Description : simple helpers for creating cusom output manipulators
// ***************************************************************************

#ifndef BOOST_TEST_PP_VARIADIC_HPP_102412GER
#define BOOST_TEST_PP_VARIADIC_HPP_102412GER

// Boost
#include <boost/config.hpp>
#include <boost/preprocessor/arithmetic/mul.hpp>
#include <boost/preprocessor/arithmetic/sub.hpp>
#include <boost/preprocessor/punctuation/comma.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/logical/bool.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {

// ************************************************************************** //
// **************          BOOST_TEST_PP_VARIADIC_SIZE         ************** //
// ************************************************************************** //

#define BOOST_TEST_PP_VARIADIC_SIZE(...) BOOST_PP_SUB( BOOST_PP_MUL( BOOST_PP_VARIADIC_SIZE(__VA_ARGS__), 2 ),      \
                                                       BOOST_PP_VARIADIC_SIZE( BOOST_PP_COMMA __VA_ARGS__ () ) )    \
/**/

// ************************************************************************** //
// **************            BOOST_TEST_PP_NON_EMPTY           ************** //
// ************************************************************************** //

#define BOOST_TEST_PP_NON_EMPTY(...) BOOST_PP_BOOL(BOOST_TEST_PP_VARIADIC_SIZE(__VA_ARGS__))

} // namespace unit_test
} // namespace boost


#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_PP_VARIADIC_HPP_102412GER
