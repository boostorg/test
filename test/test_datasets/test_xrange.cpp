//  (C) Copyright Gennadiy Rozental 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests singleton dataset
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/monomorphic/join.hpp>
namespace data=boost::unit_test::data;
namespace mono=boost::unit_test::data::monomorphic;

#include "test_datasets.hpp"

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_single_range )
{
    BOOST_CHECKA( data::xrange( 5 ).size() == 5 );
    BOOST_CHECKA( data::xrange( 3. ).size() == 3 );
    BOOST_CHECK_THROW( data::xrange( -5 ), std::logic_error );
    BOOST_CHECK_THROW( data::xrange( 0 ), std::logic_error );

    BOOST_CHECKA( data::xrange( 1, 5 ).size() == 4 );
    BOOST_CHECKA( data::xrange( -5, 0 ).size() == 5 );
    BOOST_CHECKA( data::xrange( 1., 7.5 ).size() == 7 );
    BOOST_CHECK_THROW( data::xrange( 0, 0 ), std::logic_error );
    BOOST_CHECK_THROW( data::xrange( 3, 1 ), std::logic_error );

    BOOST_CHECKA( data::xrange( 3, 9, 2 ).size() == 3 );
    BOOST_CHECKA( data::xrange( 5, 0, -1 ).size() == 5 );
    BOOST_CHECKA( data::xrange( 1, 10, 2 ).size() == 5 );
    BOOST_CHECKA( data::xrange( 1, 10, 3 ).size() == 3 );
    BOOST_CHECKA( data::xrange( 1, 10, 8 ).size() == 2 );
    BOOST_CHECKA( data::xrange( 0., 3., 0.4 ).size() == 8 );
    BOOST_CHECKA( data::xrange( 1e-6, 2.e-6, 1e-9 ).size() == 1000 );

    BOOST_CHECKA( data::xrange<int>(( mono::begin = 9, mono::end = 15 )).size() == 6 );
    BOOST_CHECKA( data::xrange<double>(( mono::step = 0.5, mono::end = 3 )).size() == 6 );
    
    int c = 0;

    data::for_each_sample( data::xrange( 3 ), [&c](int a) {
        BOOST_CHECKA( a == c++ );
    });

    c = 1;
    data::for_each_sample( data::xrange( 1, 10, 2 ), [&c](int a) {
        BOOST_CHECKA( a == c );
        c += 2;
    });
}

BOOST_AUTO_TEST_CASE( test_range_join )
{
    auto ds = data::xrange( 1, 4 ) + data::xrange( 7, 11 );

    BOOST_CHECKA( ds.size() == 7 );

    invocation_count ic;
    ic.m_value = 0;
    data::for_each_sample( ds, ic );
    BOOST_CHECKA( ic.m_value == 7 );

    int arr[] = {1,2,3,7,8,9,10};
    int* exp = arr;
    int c = 0;

    data::for_each_sample( ds, [&c,exp](int a) {
        BOOST_CHECKA( a == exp[c++] );
    });
}

//____________________________________________________________________________//

// EOF
