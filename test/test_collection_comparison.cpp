//  (C) Copyright Gennadiy Rozental 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests collection comparison implementation
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE Test collection`s comparisons
#include <boost/test/unit_test.hpp>
namespace tt=boost::test_tools;

#define VALIDATE_OP( op )                           \
{                                                   \
    BOOST_TEST_INFO( "validating operator " #op );  \
    bool expected = (c1 op c2);                     \
    BOOST_TEST_BUILD_ASSERTION( c1 op c2 );         \
    auto const& res = E.evaluate();                 \
    BOOST_TEST( expected == !!res );                \
}                                                   \
/**/

template<typename Col>
void
validate_comparisons(Col const& c1, Col const& c2 )
{
    VALIDATE_OP( == )
    VALIDATE_OP( != )
    VALIDATE_OP( < )
    VALIDATE_OP( > )
    VALIDATE_OP( <= )
    VALIDATE_OP( >= )
}

BOOST_AUTO_TEST_CASE( test2 )
{
    std::vector<int> a{1, 2, 3};
    std::vector<int> b{1, 3, 2};
    std::vector<int> c{1, 2, 3};
    std::vector<int> d{1, 2, 3, 4};

    BOOST_TEST_CONTEXT( "validating comparisons of a and b" )
        validate_comparisons(a, b);
    BOOST_TEST_CONTEXT( "validating comparisons of a and c" )
        validate_comparisons(a, c);
    BOOST_TEST_CONTEXT( "validating comparisons of a and d" )
        validate_comparisons(a, d);
}

//____________________________________________________________________________//

// EOF
