//  (C) Copyright Marek Kurdej 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : BOOST_TEST_DONT_PRINT_LOG_VALUE unit test
// *****************************************************************************

// Boost.Test
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

// STL
#include <vector>

struct MyClass
{
  bool operator==(MyClass const&) const
  {
    return true;
  }
    
  bool operator!=(MyClass const&) const
  {
    return false;
  }    
};

typedef ::std::vector<MyClass> MyClassVec;

BOOST_TEST_DONT_PRINT_LOG_VALUE(MyClass)

BOOST_AUTO_TEST_CASE(single_object)
{
    MyClass actual, expected;
    BOOST_CHECK_EQUAL(actual, expected);
}

BOOST_AUTO_TEST_CASE(collection_of_objects)
{
    MyClassVec actual, expected;
    BOOST_CHECK_EQUAL_COLLECTIONS(actual.begin(), actual.end(), expected.begin(), expected.end());
}
