//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief This is the timer policy based on chrono::high_resolution_clock.
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_TIMER_CHRONO_BASED_HPP
#define BOOST_TEST_BENCHMARK_TIMER_CHRONO_BASED_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

// Boost.Chrono
#include <boost/chrono/system_clocks.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {
namespace timer {

template<typename Clock>
class chrono_based {
public:
    using clock_t    = Clock;
    using duration_t = typename clock_t::duration;

    chrono_based() : m_start( clock_t::now() ) {}

    duration_t elapsed() const
    {
        clock_t::time_point now = clock_t::now();
        return chrono::duration_cast<duration_t>(now - m_start);
    }

private:
    // Data members
    typename clock_t::time_point m_start;
};

using high_resolution   = chrono_based<chrono::high_resolution_clock>;
using process_real_cpu  = chrono_based<chrono::process_real_cpu_clock>;
using process_user_cpu  = chrono_based<chrono::process_user_cpu_clock>;
using process_system_cpu= chrono_based<chrono::process_system_cpu_clock>;

} // namespace timer
} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_TIMER_CHRONO_BASED_HPP
