#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <boost/test/data/monomorphic.hpp>

#include <boost/test/data/monomorphic/generators/xrange.hpp>
#include <boost/test/data/monomorphic/zip.hpp>

// generation of a sequence/range
namespace data=boost::unit_test::data;


//[snippet_dataset1_1
BOOST_DATA_TEST_CASE( test_case_1, data::xrange(5) )
{
  BOOST_TEST_MESSAGE(sample);
  BOOST_CHECK(sample <= 4 && sample >= 0);
}
//]

//[snippet_dataset1_2
BOOST_DATA_TEST_CASE( test_case_1_1, data::xrange(5), my_var )
{
  BOOST_TEST_MESSAGE(my_var);
  BOOST_CHECK(my_var <= 4 && my_var >= 0);
}
//]

//[snippet_dataset1_3
BOOST_DATA_TEST_CASE( test_case_2, data::xrange(5) ^ data::xrange(5), var1, var2)
{
  BOOST_TEST_MESSAGE(var1 << " - " << var2);
  BOOST_CHECK(true);
}
//]

