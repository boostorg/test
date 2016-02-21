//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief stop_on_convergence models policy aspect, which produces the policy 
//!        which stops, when sample variance is below some value.
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_SAMPLING_STOP_ON_CONVERGENCE_HPP
#define BOOST_TEST_BENCHMARK_SAMPLING_STOP_ON_CONVERGENCE_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

// Boost
#include <boost/chrono/duration.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {
namespace sampling {

template<typename Next>
class stop_on_convergence : public Next {
public:
    using units_t = typename Next::units_t;
    using threshold_t = units_t;

    stop_on_convergence() : Next(), m_variance_threshold( 0 ), m_done( false ) {}
    stop_on_convergence( Next&& next, threshold_t stdev_threshold ) 
    : Next( std::move( next ) )
    , m_variance_threshold( stdev_threshold * stdev_threshold.count() )
    , m_done( false ) {}
    template<typename ...Args>
    stop_on_convergence( threshold_t stdev_threshold, Args&&... args )
    : Next( std::forward<Args>( args )... )
    , m_variance_threshold( stdev_threshold * stdev_threshold.count() )
    , m_done( false ) {}

    // Access methods
    bool            is_done() const
    {
        if( !m_done )
            m_done = Next::is_done() || (Next::variance() != units_t{0} && Next::variance() < m_variance_threshold);

        return m_done;
    }

    // Samples collection
    void            add_sample( units_t sample ) { if( !m_done ) Next::add_sample( sample ); }

private:
    // Data members
    threshold_t     m_variance_threshold;
    mutable bool    m_done;
};

} // namespace sampling
} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_SAMPLING_STOP_ON_CONVERGENCE_HPP