//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief 
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_CONFIG_HPP
#define BOOST_TEST_BENCHMARK_CONFIG_HPP

// Boost
#include <boost/cstdint.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {

/// type representing number of iterations in a single run
using iterations_t = uint_least32_t;

/// type representing measured time values
using nanosecond_t = int_least64_t;

/// type representing number of samples
using num_samples_t = uint_least32_t;

// TODO:
// benchmark::time_performance directly sample number of calls per unit of time
//   common result structure with benchmark::time?
// results comparison? with tolerance?
// BOOST_TEST_BENCHMARK_TIME, BOOST_TEST_BENCHMARK_GOLDEN, --benchmark_tolerance, --benchmark_golden
//    test tool to store and compare against golden benchmark measurements to validate against changes in performance.
// benchmark::memory - measure memory utilization
// benchmark::record/log/profile - records time and/or memory utilization in a scope. Builds hierarchical tree of records to 
// allow profiling resource utilization in the program

} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_CONFIG_HPP
