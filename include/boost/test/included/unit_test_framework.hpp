//  (C) Copyright Gennadiy Rozental 2001-2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : included (vs. linked ) version of Unit Test Framework
// ***************************************************************************

#ifndef BOOST_INCLUDED_UNIT_TEST_FRAMEWORK_HPP_071894GER
#define BOOST_INCLUDED_UNIT_TEST_FRAMEWORK_HPP_071894GER

#include <boost/test/impl/execution_monitor.ipp>

#include <boost/test/impl/test_tools.ipp>

#include <boost/test/impl/unit_test_result.ipp>
#include <boost/test/impl/unit_test_suite.ipp>
#include <boost/test/impl/unit_test_parameters.ipp>
#include <boost/test/impl/unit_test_log.ipp>
#include <boost/test/impl/unit_test_monitor.ipp>
#include <boost/test/impl/supplied_log_formatters.ipp>

#include <boost/test/impl/unit_test_main.ipp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.10  2005/01/22 19:22:13  rogeeff
//  implementation moved into headers section to eliminate dependency of included/minimal component on src directory
//
//  Revision 1.9  2004/07/19 12:30:49  rogeeff
//  guard rename
//
//  Revision 1.8  2004/05/21 06:19:35  rogeeff
//  licence update
//
//  Revision 1.7  2004/05/11 11:00:55  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//
//  Revision 1.6  2003/12/01 00:41:56  rogeeff
//  prerelease cleaning
//
// ***************************************************************************

#endif // BOOST_INCLUDED_UNIT_TEST_FRAMEWORK_HPP_071894GER
