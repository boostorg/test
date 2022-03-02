//  (C) Copyright Raffi Enficiaud 2022.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// from https://github.com/boostorg/test/issues/296

#define BOOST_TEST_MODULE Tests
#define BOOST_TEST_NO_MAIN 1
#define BOOST_TEST_ALTERNATIVE_INIT_API 1

#include <boost/test/included/unit_test.hpp>

int main( int argc, char** argv )
{
    boost::unit_test::unit_test_main( init_unit_test, argc, argv );
}

BOOST_AUTO_TEST_CASE( SpuriousDoubleFree )
{
    std::exit( 0 );
}
