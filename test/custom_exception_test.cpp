//  (C) Copyright Gennadiy Rozental 2003-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : unit test for custon user exception translator registry
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_monitor.hpp>
using namespace boost::unit_test;

//____________________________________________________________________________//

struct my_exception1
{
    explicit    my_exception1( int res_code ) : m_res_code( res_code ) {}

    int         m_res_code;
};

struct my_exception2
{
    explicit    my_exception2( int res_code ) : m_res_code( res_code ) {}

    int         m_res_code;
};

//____________________________________________________________________________//

void throw_my_exception1()
{
    throw my_exception1( 12 );
}

void my_exception1_translator( my_exception1 )
{
    BOOST_TEST_MESSAGE( "Caught my_exception1" );
}

//____________________________________________________________________________//

void throw_my_exception2()
{
    throw my_exception2( 89 );
}

void my_exception2_translator( my_exception2 )
{
    BOOST_TEST_MESSAGE( "Caught my_exception2" );
}

//____________________________________________________________________________//

test_suite*
init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
    test_suite* test = BOOST_TEST_SUITE("custom_exception_test");

    unit_test_monitor.register_exception_translator<my_exception1>( &my_exception1_translator );
    unit_test_monitor.register_exception_translator<my_exception2>( &my_exception2_translator );

    test->add( BOOST_TEST_CASE( &throw_my_exception1 ) );
    test->add( BOOST_TEST_CASE( &throw_my_exception2 ) );

    return test;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.8  2005/12/14 06:01:02  rogeeff
//  *** empty log message ***
//
//  Revision 1.7  2005/05/11 05:07:56  rogeeff
//  licence update
//
//  Revision 1.6  2005/02/20 08:28:34  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
//  Revision 1.5  2005/05/21 06:26:10  rogeeff
//  licence update
//
//  Revision 1.4  2005/05/11 11:05:05  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//  namespace names shortened
//
//  Revision 1.3  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

// EOF
