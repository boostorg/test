#define BOOST_TEST_MODULE example
#include <boost/test/unit_test.hpp>

namespace utf = boost::unit_test;

BOOST_TEST_DECORATOR(
  + utf::timeout(2)
)
BOOST_AUTO_TEST_SUITE( suite1 )

  BOOST_AUTO_TEST_CASE( test_case1 )
  {
    for(;;) {}
    BOOST_TEST(true);
  }

  BOOST_AUTO_TEST_CASE( test_case2 )
  {
    while(true) {}
    BOOST_TEST(true);
  }

BOOST_AUTO_TEST_SUITE_END()
