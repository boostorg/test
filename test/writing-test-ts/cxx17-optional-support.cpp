// Copyright 2025 Matt Borland
// Distributed under the Boost Software License, Version 1.0.
// https://www.boost.org/LICENSE_1_0.txt

#define BOOST_TEST_MODULE cxx_std_optional_support
#include <optional>
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test1)
{
    std::optional<int> a = 1, b = 2;
    BOOST_TEST(a != b);
}

BOOST_AUTO_TEST_CASE(test2)
{
    std::optional<int> o;
    BOOST_CHECK_EQUAL(o, std::nullopt);
}
