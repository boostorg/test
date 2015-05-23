//  (C) Copyright Gennadiy Rozental 2011-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

//[example_code
#include<boost/test/included/unit_test.hpp>

using namespace boost::unit_test; 

class test_class
{
public:
  void test_method()
  {
    BOOST_TEST( true /* test assertion */ );
  }
};

void free_test_function() 
{ 
  test_class inst;
  inst.test_method();
}

test_suite* init_unit_test_suite( int argc, char* argv[] )
{ 
  framework::master_test_suite().add( BOOST_TEST_CASE( &free_test_function ) );
  return 0; 
} 
//]
