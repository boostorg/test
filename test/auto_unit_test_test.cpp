//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : unit test for auto unit test facility
// ***************************************************************************

// Boost.Test
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_TEST_CASE( test1 )
{
    BOOST_CHECK( true );
}

BOOST_AUTO_TEST_CASE( test2 )
{
    BOOST_CHECK( true );
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.10  2005/05/11 05:07:56  rogeeff
//  licence update
//
//  Revision 1.9  2005/02/20 08:28:34  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
//  Revision 1.8  2005/05/27 06:30:48  rogeeff
//  no message
//
//  Revision 1.7  2005/05/21 06:26:10  rogeeff
//  licence update
//
//  Revision 1.6  2003/12/01 00:42:37  rogeeff
//  prerelease cleaning
//
// ***************************************************************************

// EOF
