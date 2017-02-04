//  (C) Copyright Raffi Enficiaud 2017.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! Customization point for printing user defined types
// *****************************************************************************

#define BOOST_TEST_MODULE user type logger customization points
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>


struct user_defined_type {
    int value;

    user_defined_type(int value_) : value(value_)
    {}

    bool operator==(int right) const {
        return right == value;
    }
};

std::ostream& boost_test_print_type(std::ostream& ostr, user_defined_type const& right) {
    ostr << "value of my type is " << right.value;
    return ostr;
}

BOOST_AUTO_TEST_CASE(test1)
{
    using ::boost_test_print_type;
    user_defined_type t(10);
    BOOST_CHECK_EQUAL(t, 10);
    BOOST_TEST(t == 10);
}
