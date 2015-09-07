//  (C) Copyright Ben Wiederhake 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : test_observer inheritors regression test
// *****************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE test_observer inheritors regression test
#include <boost/test/progress_monitor.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/unit_test.hpp>

namespace ut = boost::unit_test;

//____________________________________________________________________________//

/* The following is a crude hack in order to test for successful compilation,
 * and *only* for compilation. This has two issues:
 * 1. The behaviour isn't being checked.
 * 2. I'm not sure whether the compiler might notice the missing call and
 *    "optimize it away", thus ignoring any compilation errors.
 *    On my machine, at least, it fails-as-expected without the
 *    overloaded-methods patch. */

void test_overloads( const ut::test_unit& ut_ )
{
    ut::progress_monitor.test_unit_skipped( ut_ );
    ut::progress_monitor.test_unit_skipped( ut_, "Dummy reason" );
    ut::results_collector.test_unit_skipped( ut_ );
    ut::results_collector.test_unit_skipped( ut_, "Dummy reason" );
    ut::unit_test_log.test_unit_skipped( ut_ );
    ut::unit_test_log.test_unit_skipped( ut_, "Dummy reason" );
}

//____________________________________________________________________________//

/* Would fail if no tests are defined. */
BOOST_AUTO_TEST_CASE( test_observer_dummy )
{
}

//____________________________________________________________________________//

// EOF
