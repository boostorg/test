//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// Boost.Test
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

typedef boost::mpl::list<char,int,float,double> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE( test, T, test_types )
{
    T t = 9;

    BOOST_CHECK_EQUAL( (t*t+t)/10, 9 );
}

//____________________________________________________________________________//

// EOF
