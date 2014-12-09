//[example_code
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>

namespace data = boost::unit_test::data;

int samples1[] = {1,2};
char const* samples2[] = {"qwerty", "asdfg"};

__BOOST_DATA_TEST_CASE__( 
      test_name, 
      data::make(samples1)^samples2, 
      integer_values, 
      string_value)
{
  std::cout << integer_values << ", " << string_value << std::endl;
}
//]

