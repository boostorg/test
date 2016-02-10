//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief This is the timer policy based on chrono::high_resolution_clock.
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_TIMER_HIGH_RESOLUTION_HPP
#define BOOST_TEST_BENCHMARK_TIMER_HIGH_RESOLUTION_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

// Boost.Chrono
#include <boost/chrono/system_clocks.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {
namespace timer {

class high_resolution {
    using clock_t = chrono::high_resolution_clock;
public:
    using duration = chrono::nanoseconds;

    high_resolution() : m_start( clock_t::now() ) {}

    chrono::nanoseconds elapsed() const
    {
        clock_t::time_point now = clock_t::now();
        return chrono::duration_cast<chrono::nanoseconds>(now - m_start);
    }

private:
    // Data members
    clock_t::time_point m_start;
};

} // namespace timer
} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_TIMER_HIGH_RESOLUTION_HPP
