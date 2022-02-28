//  (C) Copyright Raffi Enficiaud 2022.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  from https://github.com/boostorg/test/issues/316
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE string_view_with_custom_traits
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>

using namespace boost::unit_test;
using boost::test_tools::output_test_stream;

struct custom_char_traits : std::char_traits<char> {};

void custom_char_traits_compare()
{
    using sv = std::basic_string_view<char, custom_char_traits>;
    sv sv1 = "sv1";
    sv sv2 = "sv2";
    BOOST_TEST(sv1 == sv2);
}

void std_char_traits_compare()
{
    using sv = std::basic_string_view<char, std::char_traits<char>>;
    sv sv1 = "sv1";
    sv sv2 = "sv2";
    BOOST_TEST(sv1 == sv2);
}

// ********************************** boilerplate
void check( output_test_stream& output,
            output_format log_format,
            test_unit_id id,
            log_level ll = log_successful_tests )
{
    class reset_status : public test_tree_visitor {
    private:
        virtual bool    visit( test_unit const& tu )
        {
            const_cast<test_unit&>(tu).p_default_status.value = test_case::RS_INHERIT;
            const_cast<test_unit&>(tu).p_run_status.value = test_case::RS_INVALID;
            return true;
        }
    } rstatus;

    {
      log_setup_teardown holder(output, log_format, ll);

      // reinit the default/run status otherwise we cannot apply the decorators
      // after the first run
      traverse_test_tree( id, rstatus, true );
      framework::get<test_suite>(id).p_default_status.value = test_unit::RS_ENABLED;

      output << "* " << log_format << "-format  *******************************************************************";
      output << std::endl;
  
      framework::finalize_setup_phase( id );
      framework::run( id, false ); // do not continue the test tree to have the test_log_start/end
      output << std::endl;
    }

    BOOST_TEST( output.match_pattern(true) ); // flushes the stream at the end of the comparison.
}

void check( output_test_stream& output, test_suite* ts )
{
    check( output, OF_CLF, ts->p_id );
}
// ********************************** boilerplate end

BOOST_AUTO_TEST_CASE( test_stringview_honors_chartraits )
{
#define PATTERN_FILE_NAME "stringview-test.pattern"

    std::string pattern_file_name(
        framework::master_test_suite().argc <= 1
            ? (runtime_config::save_pattern() ? PATTERN_FILE_NAME : "./baseline-outputs/" PATTERN_FILE_NAME )
            : framework::master_test_suite().argv[1] );

    output_test_stream_for_loggers test_output( pattern_file_name,
                                                !runtime_config::save_pattern(),
                                                true,
                                                __FILE__ );

    test_case* tc1 = BOOST_TEST_CASE(custom_char_traits_compare);
    test_case* tc2 = BOOST_TEST_CASE(std_char_traits_compare);
    
    test_suite* ts_main = BOOST_TEST_SUITE( "fake master test suite" );
        ts_main->add( tc1 );
        ts_main->add( tc2 );

    check( test_output, ts_main );
}
