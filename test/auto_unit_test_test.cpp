//  (C) Copyright Gennadiy Rozental 2001-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : unit test for auto unit test facility
// ***************************************************************************

// Boost.Test
#include <boost/test/auto_unit_test.hpp>

BOOST_AUTO_UNIT_TEST( test1 )
{
    BOOST_CHECK( true );
}

BOOST_AUTO_UNIT_TEST( test2 )
{
    BOOST_CHECK( true );
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.3  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.2  2003/06/09 09:23:03  rogeeff
//  1.30.beta1
//
//  Revision 1.1  2002/12/09 05:13:31  rogeeff
//  Initial commit
//

// ***************************************************************************

// EOF
