//  (C) Copyright Raffi Enficiaud 2018.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief Unit test for benchmark::test results and logging
// ***************************************************************************

// Boost.Test Benchmark
#include <boost/test/benchmark/time.hpp>
#include <boost/test/benchmark/sampling/average.hpp>
#include <boost/test/benchmark/sampling/skip_first.hpp>
#include <boost/test/benchmark/sampling/skip_outliers.hpp>
#include <boost/test/benchmark/sampling/stop_on_convergence.hpp>
#include <boost/test/benchmark/sampling/chain.hpp>



BOOST_AUTO_TEST_CASE( hrf_logger_with_baseline )
{
    using namespace bm::sampling;
    using units = chrono::duration<double,boost::micro>;

    const boost::int_least64_t MAX = 1000;

    std::vector<boost::int_least64_t> v;
    v.reserve( MAX );

    auto res_baseline = bm::time((
        bm::func = [&v, MAX]() {
            for( boost::int_least64_t i = 0; i<MAX; ++i )
                v.push_back( i );
        }, 
        bm::setup = [&v]() {
            v.clear();
        },
        bm::num_iterations = 10,
        bm::timer_policy = bm::timer::high_resolution(),
        bm::sampling_policy = make_chain<units, average, skip_outliers, stop_on_convergence>( units(100), 2, 10000, 10 )
    ));

    // here we have something that we want to measure, given the previous baseline
    auto res_action = bm::time((
        bm::func = [&v, MAX]() {
            for( boost::int_least64_t i = 0; i<MAX; ++i )
                v.push_back( i * i );
        }, 
        bm::setup = [&v]() {
            v.clear();
        },
        bm::num_iterations = 10,
        bm::timer_policy = bm::timer::high_resolution(),
        bm::sampling_policy = make_chain<units, average, skip_outliers, stop_on_convergence>( units(100), 2, 10000, 10 )
    ));

    hrf_logger logger;

    logger.set_baseline(res_baseline);

    res.report_time( std::cout );
    res.report_time<double_sec>( std::cout );
}
