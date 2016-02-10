//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief This policy collects single sample.
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_SAMPLING_SINGLE_SAMPLE_HPP
#define BOOST_TEST_BENCHMARK_SAMPLING_SINGLE_SAMPLE_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

// Boost
#include <boost/chrono/duration.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {
namespace sampling {

// !! Add validations to all methods to avoid misuse
template<typename SamplingUnits = chrono::nanoseconds>
class single_sample {
public:
    using units_t = SamplingUnits;

    single_sample() : m_sample( SamplingUnits::min() ) {}

    // Access methods
    bool            is_done() const         { return m_sample != SamplingUnits::min(); }
    SamplingUnits   value() const           { return m_sample; }
    SamplingUnits   best() const            { return m_sample; }
    SamplingUnits   worst() const           { return m_sample; }
    SamplingUnits   variance() const        { return SamplingUnits( 0 ); }
    num_samples_t   total_samples() const   { return is_done() ? 1 : 0; }

    // Samples collection
    void            add_sample( SamplingUnits sample )
    {
        if( !is_done() )
            m_sample = sample;
    }

private:
    // Data members
    SamplingUnits  m_sample;
};

} // namespace sampling
} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_SAMPLING_SINGLE_SAMPLE_HPP
