//  (C) Copyright Gennadiy Rozental 2001-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : storage for unit test framework parameters information
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_PARAMETERS_HPP
#define BOOST_UNIT_TEST_PARAMETERS_HPP

// STL
#include <string>   // std::string

#include <boost/test/detail/unit_test_config.hpp>

namespace boost {

namespace unit_test_framework {

// framework parameters and there corresponding command-line arguments
c_string_literal const LOG_LEVEL         = "BOOST_TEST_LOG_LEVEL";              // --log_level
c_string_literal const NO_RESULT_CODE    = "BOOST_TEST_RESULT_CODE";            // --result_code
c_string_literal const REPORT_LEVEL      = "BOOST_TEST_REPORT_LEVEL";           // --report_level
c_string_literal const TESTS_TO_RUN      = "BOOST_TESTS_TO_RUN";                // --run_test
c_string_literal const SAVE_TEST_PATTERN = "BOOST_TEST_SAVE_PATTERN";           // --save_pattern
c_string_literal const BUILD_INFO        = "BOOST_TEST_BUILD_INFO";             // --build_info
c_string_literal const CATCH_SYS_ERRORS  = "BOOST_TEST_CATCH_SYSTEM_ERRORS";    // --catch_system_errors
c_string_literal const REPORT_FORMAT     = "BOOST_TEST_REPORT_FORMAT";          // --report_format
c_string_literal const LOG_FORMAT        = "BOOST_TEST_LOG_FORMAT";             // --log_format
c_string_literal const OUTPUT_FORMAT     = "BOOST_TEST_OUTPUT_FORMAT";          // --output_format

enum report_level                             { CONFIRMATION_REPORT, SHORT_REPORT, DETAILED_REPORT, NO_REPORT, UNDEF_REPORT };
c_string_literal const report_level_names[] = { "confirm"          , "short"     , "detailed"     , "no"     };

enum output_format { HRF /* human readable format */, XML /* XML */ };

std::string retrieve_framework_parameter( c_string_literal parameter_name_, int* argc_, char** argv_ );

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.10  2003/10/27 07:13:13  rogeeff
//  licence update
//
//  Revision 1.9  2003/06/10 03:36:51  rogeeff
//  UNDEF report level introduced
//
//  Revision 1.8  2003/06/09 08:39:28  rogeeff
//  1.30.beta1
//

// ***************************************************************************

#endif // BOOST_UNIT_TEST_CONFIG_HPP
