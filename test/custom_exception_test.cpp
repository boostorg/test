//  (C) Copyright Gennadiy Rozental 2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

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
using namespace boost::unit_test_framework;

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
	BOOST_MESSAGE( "Caught my_exception1" );
}

//____________________________________________________________________________//

void throw_my_exception2()
{
	throw my_exception1( 89 );
}

void my_exception2_translator( my_exception2 )
{
	BOOST_MESSAGE( "Caught my_exception2" );
}

//____________________________________________________________________________//

test_suite*
init_unit_test_suite( int /*argc*/, char* /*argv*/[] ) {
	test_suite* test = BOOST_TEST_SUITE("custom_exception_test");

	register_exception_translator<my_exception1>( &my_exception1_translator );
	register_exception_translator<my_exception2>( &my_exception2_translator );

	test->add( BOOST_TEST_CASE( &throw_my_exception1 ) );
	test->add( BOOST_TEST_CASE( &throw_my_exception2 ) );

	return test;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.2  2003/11/06 07:31:42  rogeeff
//  Licence update
//
//  Revision 1.1  2003/11/02 06:13:27  rogeeff
//  2 new tests added: multiple module auto unit testing and custom exception translator regitry unit test
//
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
