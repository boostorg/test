//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : storage for unit test framework parameters information
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_PARAMETERS_HPP_071894GER
#define BOOST_UNIT_TEST_PARAMETERS_HPP_071894GER

#include <boost/test/detail/unit_test_config.hpp>

namespace boost {

namespace unit_test {

// framework parameters and there corresponding command-line arguments
static literal_string LOG_LEVEL         = "BOOST_TEST_LOG_LEVEL";               // --log_level
static literal_string NO_RESULT_CODE    = "BOOST_TEST_RESULT_CODE";             // --result_code
static literal_string REPORT_LEVEL      = "BOOST_TEST_REPORT_LEVEL";            // --report_level
static literal_string TESTS_TO_RUN      = "BOOST_TESTS_TO_RUN";                 // --run_test
static literal_string SAVE_TEST_PATTERN = "BOOST_TEST_SAVE_PATTERN";            // --save_pattern
static literal_string BUILD_INFO        = "BOOST_TEST_BUILD_INFO";              // --build_info
static literal_string CATCH_SYS_ERRORS  = "BOOST_TEST_CATCH_SYSTEM_ERRORS";     // --catch_system_errors
static literal_string REPORT_FORMAT     = "BOOST_TEST_REPORT_FORMAT";           // --report_format
static literal_string LOG_FORMAT        = "BOOST_TEST_LOG_FORMAT";              // --log_format
static literal_string OUTPUT_FORMAT     = "BOOST_TEST_OUTPUT_FORMAT";           // --output_format
static literal_string DETECT_MEM_LEAKS  = "BOOST_TEST_DETECT_MEMORY_LEAKS";     // --detect_memory_leaks

enum report_level                            { CONFIRMATION_REPORT, SHORT_REPORT, DETAILED_REPORT, NO_REPORT, UNDEF_REPORT };
static literal_string report_level_names[] = { "confirm"          , "short"     , "detailed"     , "no"     };

enum output_format { HRF /* human readable format */, XML /* XML */ };

const_string retrieve_framework_parameter( const_string parameter_name_, int* argc_, char** argv_ );

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.18  2005/02/01 06:40:07  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.17  2005/01/31 05:59:36  rogeeff
//  detect_memory_leaks feature added
//
// ***************************************************************************

#endif // BOOST_UNIT_TEST_CONFIG_HPP_071894GER
