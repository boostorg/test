//  (C) Copyright Gennadiy Rozental 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_UNIT_TEST_CONFIG_HPP
#define BOOST_UNIT_TEST_CONFIG_HPP

// BOOST
#include <boost/config.hpp>

// STL
#include <string>

namespace boost {

namespace unit_test_framework {

typedef unsigned long unit_test_counter;

// framework parameters and there corresponding command-line arguments
char const* const LOGLEVEL          = "BOOST_TEST_LOG_LEVEL";        // --log_level
char const* const NO_RESULT_CODE    = "BOOST_TEST_RESULT_CODE";      // --result_code
char const* const RESULT_REPORT     = "BOOST_TEST_REPORT_LEVEL";     // --result_report
char const* const TESTS_TO_RUN      = "BOOST_TESTS_TO_RUN";          // --run_test
char const* const SAVE_TEST_PATTERN = "BOOST_TEST_SAVE_PATTERN";     // --save_pattern


enum result_report_type                 { CONFIRMATION_REPORT, SHORT_REPORT, DETAILED_REPORT, NO_REPORT };
char const* const report_type_names[] = { "confirm"          , "short"     , "detailed"     , "no"      };

std::string retrieve_framework_parameter( char const* parameter_name, int* argc = NULL, char ** argv = NULL );

namespace detail {

#ifdef BOOST_NO_STD_DISTANCE
template <class T>
std::ptrdiff_t distance(const T& x, const T& y)
{ 
    std::ptrdiff_t res = 0;

    std::distance( x, y, res );

    return res;
}
#else
using std::distance;
#endif

} // namespace detail

} // namespace unit_test_framework

} // namespace boost

//  Revision History
//   5 Oct 01  Initial version (Gennadiy Rozental)

#endif // BOOST_UNIT_TEST_CONFIG_HPP
