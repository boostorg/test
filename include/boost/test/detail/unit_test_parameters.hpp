//  (C) Copyright Gennadiy Rozental 2001-2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Id$
//
//  Description : storage for unit test framework parameters information
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_PARAMETERS_HPP
#define BOOST_UNIT_TEST_PARAMETERS_HPP

// STL
#include <string>

#include <boost/test/detail/unit_test_config.hpp>

namespace boost {

namespace unit_test_framework {

// framework parameters and there corresponding command-line arguments
c_string_literal const LOGLEVEL          = "BOOST_TEST_LOG_LEVEL";              // --log_level
c_string_literal const NO_RESULT_CODE    = "BOOST_TEST_RESULT_CODE";            // --result_code
c_string_literal const RESULT_REPORT     = "BOOST_TEST_REPORT_LEVEL";           // --result_report
c_string_literal const TESTS_TO_RUN      = "BOOST_TESTS_TO_RUN";                // --run_test
c_string_literal const SAVE_TEST_PATTERN = "BOOST_TEST_SAVE_PATTERN";           // --save_pattern
c_string_literal const BUILD_INFO        = "BOOST_TEST_BUILD_INFO";             // --build_info
c_string_literal const CATCH_SYS_ERRORS  = "BOOST_TEST_CATCH_SYSTEM_ERRORS";    // --catch_system_errors

enum result_report_level                      { CONFIRMATION_REPORT, SHORT_REPORT, DETAILED_REPORT, NO_REPORT };
c_string_literal const report_level_names[] = { "confirm"          , "short"     , "detailed"     , "no"      };

std::string retrieve_framework_parameter( c_string_literal parameter_name_, int* argc_, char** argv_ );

} // namespace unit_test_framework

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.6  2002/12/08 17:38:44  rogeeff
//  catch_system_error framework cla parameter and envronment variable introduced
//  switch to use c_string_literal
//
//  Revision 1.5  2002/11/02 19:31:05  rogeeff
//  merged into the main trank
//

// ***************************************************************************

#endif // BOOST_UNIT_TEST_CONFIG_HPP
