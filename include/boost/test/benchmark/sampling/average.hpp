//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief This policy collects N samples (N is runtime argument). Reported value is
//!        mean of collected samples. 
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_SAMPLING_AVERAGE_HPP
#define BOOST_TEST_BENCHMARK_SAMPLING_AVERAGE_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {
namespace sampling {

// !! Add validations to all methods to avoid misuse
template<typename SamplingUnits = chrono::duration<double>>
class average {
public:
    using units_t = SamplingUnits;

    average(num_samples_t total_samples) 
    : m_curr_num_samples( 0 )
    , m_total_samples( total_samples )
    , m_min_sample( SamplingUnits::max() )
    , m_max_sample( SamplingUnits::min() )
    , m_samples_mean( 0 )
    , m_second_moment( 0 )
    {}

    // Access methods
    bool            is_done() const         { return m_curr_num_samples == m_total_samples; }
    SamplingUnits   value() const            { return m_samples_mean; }
    SamplingUnits   best() const             { return m_min_sample; }
    SamplingUnits   worst() const            { return m_max_sample; }
    SamplingUnits   variance() const         { return m_curr_num_samples < 2 ? SamplingUnits{0} : m_second_moment / (m_curr_num_samples-1); }
    num_samples_t   total_samples() const   { return m_curr_num_samples; }

    // Samples collection
    void            add_sample( SamplingUnits sample )
    {
        if( is_done() )
            return;

        if( sample < m_min_sample )
            m_min_sample = sample;

        if( sample > m_max_sample )
            m_max_sample = sample;

        ++m_curr_num_samples;

        // Per Knuth iterative algorithm
        SamplingUnits delta = sample - m_samples_mean;
        m_samples_mean += delta/m_curr_num_samples;
        m_second_moment += delta.count() * (sample - m_samples_mean);
    }

private:
    // Data members
    num_samples_t   m_curr_num_samples;
    num_samples_t   m_total_samples;
    SamplingUnits   m_min_sample;
    SamplingUnits   m_max_sample;
    SamplingUnits   m_samples_mean;
    SamplingUnits   m_second_moment;
};

} // namespace sampling
} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_SAMPLING_AVERAGE_HPP
