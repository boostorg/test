//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief This header includes helper class, which models the notion os a 
//!        single run consisting of multiple iterations/calls to the code 
//!        being measured.
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_DETAIL_SINGLE_RUN_HPP
#define BOOST_TEST_BENCHMARK_DETAIL_SINGLE_RUN_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {
namespace detail {

template<typename Func, typename Setup, typename Timer>
class single_run {
public:
    using iterations_t = iterations_t;

    single_run( Func&& f, Setup&& s, iterations_t num_iterations = 0 )
    : m_func( std::forward<Func>( f ) )
    , m_setup( std::forward<Setup>( s ) )
    , m_num_iterations( num_iterations )
    {
        if( m_num_iterations == 0 ) {
            // !! compute number of iterations
            m_num_iterations = 1;
        }
    }

    iterations_t num_iterations() const { return m_num_iterations; }

    template<typename SamplingPolicy>
    void        run( SamplingPolicy& sampling_policy )
    {
        run_impl( sampling_policy, m_setup );
    }

private:
    template<typename SamplingPolicy, typename SetupT>
    void        run_impl( SamplingPolicy& sampling_policy, SetupT const& )
    {
        iterations_t num_iter = m_num_iterations;

        Timer::duration_t total_elapsed{0};

        // !! Loop unrolling
        // !! Should we measure setup time and deduct it at the end instead?
        while( num_iter-- > 0 ) {
            m_setup();

            Timer t;
            m_func();
            total_elapsed += t.elapsed();
        }

        sampling_policy.add_sample( total_elapsed );    
    }

    template<typename SamplingPolicy>
    void        run_impl( SamplingPolicy& sampling_policy, nullptr_t )
    {
        iterations_t num_iter = m_num_iterations;

        Timer t;

        // !! Loop unrolling
        while( num_iter-- > 0 )
            m_func();

        sampling_policy.add_sample( chrono::duration_cast<typename SamplingPolicy::units_t>( t.elapsed() ) );    
    }

    // Data members
    Func        m_func;
    Setup       m_setup;
    iterations_t m_num_iterations;
};

} // namespace detail
} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_DETAIL_SINGLE_RUN_HPP
