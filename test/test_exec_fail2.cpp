//  test failures reported by different Test Tools

//  Copyright Gennadiy Rozental 2001-2003.
//  Copyright Beman Dawes 2000.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org/libs/test for documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : test failures reported by different Test Tools.
//  Should fail during run.
// ***************************************************************************

// Boost.Test
#include <boost/test/test_tools.hpp>

int test_main( int, char *[] )  // note the name
{
    int v = 1;

    if( v < 10 ) { // to eliminate unreachable return statement warning
        BOOST_CHECK( v == 2 );
        BOOST_ERROR( "sample BOOST_ERROR call" );
        BOOST_REQUIRE( 2 == v );

        throw "Opps! should never reach this point";
    }

    return 1;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.8  2003/09/14 12:42:22  beman_dawes
//  Change to new license (with Gennadiy's permission)
//
//  Revision 1.7  2003/06/09 09:25:24  rogeeff
//  1.30.beta1
//
//  Revision 1.6  2003/02/15 21:58:32  rogeeff
//  borland warning fix
//
//  Revision 1.5  2002/11/02 20:04:43  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
