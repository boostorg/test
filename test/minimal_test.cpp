//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : minimal testing unit test
// ***************************************************************************

// Boost.Test
#include <boost/test/minimal.hpp>

//____________________________________________________________________________//

struct bool_convertible1 {
    bool_convertible1( bool v ) : v_( v ) {}
    operator bool() { return v_; }

    bool v_;
};

//____________________________________________________________________________//

struct bool_convertible2 {
    bool_convertible2( int v ) : v_( v ) {}
    operator int() { return v_; }

    int v_;
};

//____________________________________________________________________________//

struct bool_convertible3 {
    bool_convertible3( void* v ) : v_( v ) {}

    struct Tester {};
    operator Tester*() { return (Tester*)v_; }

    void* v_;
};

//____________________________________________________________________________//

int 
test_main( int /*argc*/, char* /*argv*/[] ) 
{
    int i = 1;

    BOOST_CHECK( i == 1 );
    BOOST_CHECK( i == 2 );

    BOOST_CHECK( bool_convertible1( true  ) );
    BOOST_CHECK( bool_convertible1( false ) );

    BOOST_CHECK( bool_convertible2( 1 ) );
    BOOST_CHECK( bool_convertible2( 0 ) );

    BOOST_CHECK( bool_convertible3( (void*)1 ) );
    BOOST_CHECK( bool_convertible3( NULL ) );

    BOOST_ERROR( "Some error" );

    BOOST_REQUIRE( i == 4 );

    return 0;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.7  2003/11/06 07:31:42  rogeeff
//  Licence update
//
//  Revision 1.6  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.5  2003/06/09 09:25:24  rogeeff
//  1.30.beta1
//
//  Revision 1.4  2003/02/15 21:51:56  rogeeff
//  borland warnings fix
//
//  Revision 1.3  2002/11/02 20:04:43  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
