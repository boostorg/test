//  (C) Copyright Gennadiy Rozental 2001-2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Id$
//
//  Description : tests an extentions to Unit Test Framework
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test_suite_ex.hpp>
#include <boost/test/test_tools.hpp>

using boost::unit_test_framework::test_suite;

#if !defined(BOOST_MSVC) && !defined(__SUNPRO_CC)
// BOOST
#include <boost/bind.hpp>
#endif

#include <boost/function.hpp>
using namespace boost;

// STL
#include <list>

//____________________________________________________________________________//

void test0()
{
    BOOST_MESSAGE( "Hello there" );
}

//____________________________________________________________________________//

void test1( int arg )
{
    BOOST_CHECK( (arg & 0x80) == 0 );
}

//____________________________________________________________________________//

void test2( int arg, int mask )
{
    BOOST_CHECK( (arg & mask) != 0 );
}

//____________________________________________________________________________//

struct sub_test_suite : public test_suite {
    sub_test_suite()
    {
        parameters_list.push_back( 1 );
        parameters_list.push_back( 5 );
        parameters_list.push_back( 6 );
        parameters_list.push_back( 7 );
        parameters_list.push_back( 15 );

        masks_list.push_back( 0x01 );
        masks_list.push_back( 0x04 );
        masks_list.push_back( 0x80 );

#if !defined(BOOST_MSVC) && !defined(__SUNPRO_CC)
        for( std::list<int>::iterator it = masks_list.begin(); it != masks_list.end(); ++it ) {
            function1<void,int> fct = bind( &test2, _1, *it );
            add( BOOST_PARAM_TEST_CASE( fct, parameters_list.begin(), parameters_list.end() ) );
        }
#endif

        function1<void,int> fct = &test1;
        add( BOOST_PARAM_TEST_CASE( fct, parameters_list.begin(), parameters_list.end() ) );
    }

    std::list<int> parameters_list;
    std::list<int> masks_list;
};

//____________________________________________________________________________//

test_suite*
init_unit_test_suite( int argc, char* argv[] ) {
    test_suite* test = BOOST_TEST_SUITE("ostream_test_stream test");

    function0<void> fct1 = &test0;
    test->add( BOOST_TEST_CASE( fct1 ) );

#if !defined(BOOST_MSVC) && !defined(__SUNPRO_CC)
    function0<void> fct2 = bind( &test2, 12345, 0xcdf );
    test->add( BOOST_TEST_CASE( fct2 ) );
#endif

    test->add( new sub_test_suite
#if !defined(BOOST_MSVC) && !defined(__SUNPRO_CC)
    , 7 
#endif
    );

    return test;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.6  2002/08/26 09:08:06  rogeeff
//  cvs kw added
//
//   6 Nov 01  Initial  version (Gennadiy Rozental)

// ***************************************************************************

// EOF
