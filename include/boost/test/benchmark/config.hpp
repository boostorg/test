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

} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_CONFIG_HPP
