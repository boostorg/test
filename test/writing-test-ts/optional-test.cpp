//  (C) Copyright Marek Kurdej 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//
//  Test for GH 152 https://github.com/boostorg/test/issues/152
// *****************************************************************************

#include <boost/test/unit_test.hpp>
#include <optional>

BOOST_TEST_DONT_PRINT_LOG_VALUE( ::std::nullopt_t     )
BOOST_TEST_DONT_PRINT_LOG_VALUE( ::std::optional<int> )

BOOST_AUTO_TEST_CASE(y) {
    BOOST_TEST( ::std::make_optional( 5 ) == ::std::nullopt );
}
