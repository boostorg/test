//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief skip_outliers models policy aspect, which produces the policy 
//!        which skips outlier samples, which are distanced from sample 
//!        population mean by more then N sigmas. Note that this aspect does
//!        not attempt to "fix the past". Only new samples are skipped.
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_SAMPLING_SKIP_OUTLIERS_HPP
#define BOOST_TEST_BENCHMARK_SAMPLING_SKIP_OUTLIERS_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

// Boost
#include <boost/chrono/duration.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {
namespace sampling {

template<typename Next>
class skip_outliers : public Next {
public:
    using units_t = typename Next::units_t;

    skip_outliers()
    : Next()
    , m_var_multiplier( 0 )
    {}
    skip_outliers( Next&& next, unsigned num_sigmas )
    : Next( std::move( next ) )
    , m_var_multiplier( num_sigmas * num_sigmas )
    {}
    template<typename ...Args>
    skip_outliers( unsigned num_sigmas, Args&&... args )
    : Next( std::forward<Args>( args )... )
    , m_var_multiplier( num_sigmas * num_sigmas )
    {}

    // Samples collection
    void            add_sample( units_t sample )
    {
        auto distance = sample - this->value();

        if( this->variance().count() == 0 || distance.count() * distance.count() < m_var_multiplier * this->variance().count() )
            Next::add_sample( sample );
    }

private:
    // Data members
    unsigned        m_var_multiplier;
};

} // namespace sampling
} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_SAMPLING_SKIP_OUTLIERS_HPP
