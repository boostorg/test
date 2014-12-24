//  (C) Copyright Gennadiy Rozental 2006-2012.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines portable debug interfaces
// ***************************************************************************

#ifndef BOOST_TEST_DEBUG_API_HPP_112006GER
#define BOOST_TEST_DEBUG_API_HPP_112006GER

// Boost.Test
#include <boost/test/detail/config.hpp>
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>

// Boost
#include <boost/function/function1.hpp>

// STL
#include <string>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace debug {

#ifdef BOOST_TEST_HAS_DEBUG_SUPPORT

// ************************************************************************** //
// **************  check if program is running under debugger  ************** //
// ************************************************************************** //

bool BOOST_TEST_DECL under_debugger();

// ************************************************************************** //
// **************       cause program to break execution       ************** //
// **************           in debugger at call point          ************** //
// ************************************************************************** //

void BOOST_TEST_DECL debugger_break();

// ************************************************************************** //
// **************              gui debugger setup              ************** //
// ************************************************************************** //

struct dbg_startup_info {
    long                    pid;
    bool                    break_or_continue;
    unit_test::const_string binary_path;
    unit_test::const_string display;
    unit_test::const_string init_done_lock;
};

typedef boost::function<void (dbg_startup_info const&)> dbg_starter;

// ************************************************************************** //
// **************                debugger setup                ************** //
// ************************************************************************** //

std::string BOOST_TEST_DECL set_debugger( unit_test::const_string dbg_id, dbg_starter s = dbg_starter() );

// ************************************************************************** //
// **************    attach debugger to the current process    ************** //
// ************************************************************************** //

bool BOOST_TEST_DECL attach_debugger( bool break_or_continue = true );

#endif

// ************************************************************************** //
// **************   switch on/off detect memory leaks feature  ************** //
// ************************************************************************** //

void BOOST_TEST_DECL detect_memory_leaks( bool on_off, unit_test::const_string report_file = unit_test::const_string() );

// ************************************************************************** //
// **************      cause program to break execution in     ************** //
// **************     debugger at specific allocation point    ************** //
// ************************************************************************** //

void BOOST_TEST_DECL break_memory_alloc( long mem_alloc_order_num );

} // namespace debug
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif
