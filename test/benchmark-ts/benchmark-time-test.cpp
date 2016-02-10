//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief Unit test for benchmark::test interface
// ***************************************************************************

// Boost.Test Benchmark
#include <boost/test/benchmark/time.hpp>
#include <boost/test/benchmark/sampling/average.hpp>
#include <boost/test/benchmark/sampling/skip_first.hpp>
#include <boost/test/benchmark/sampling/stop_on_convergence.hpp>
#include <boost/test/benchmark/sampling/chain.hpp>

// Boost.Test
#define BOOST_TEST_MODULE benchmark::time test
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/test/unit_test_parameters.hpp>
#include <boost/test/data/test_case.hpp>

// STL
#include <vector>

namespace ut = boost::unit_test;
namespace tt = boost::test_tools;
namespace bm = boost::benchmark;
namespace data=boost::unit_test::data;
namespace chrono=boost::chrono;

//____________________________________________________________________________//

using double_sec = chrono::duration<double>;

chrono::nanoseconds operator "" _ns( unsigned long long val )
{
    return chrono::nanoseconds{ val };
}

chrono::milliseconds operator "" _ms( unsigned long long val )
{
    return chrono::milliseconds{ val };
}

double_sec operator "" _sec( long double val )
{
    return double_sec{ static_cast<double>(val) };
}

//____________________________________________________________________________//

#if 0
BOOST_AUTO_TEST_CASE( benchmark_time_api_test )
{
    // Just func
    bm::time( [](){} );

    // func+setup
    bm::time( [](){}, [](){} );

    // 8 combinations of named params apis; adding num_iterations only in 1 case
    bm::time( bm::func = [](){} );
    bm::time(( bm::func = [](){}, bm::setup = [](){}, bm::num_iterations = 1 ));
    bm::time(( bm::func = [](){}, bm::sampling_policy = bm::sampling::single_sample<>() ));
    bm::time(( bm::func = [](){}, bm::setup = [](){}, 
                                  bm::sampling_policy = bm::sampling::single_sample<>() ));
    bm::time(( bm::func = [](){}, bm::timer_policy = bm::timer::high_resolution() ));
    bm::time(( bm::func = [](){}, bm::setup = [](){}, 
                                  bm::timer_policy = bm::timer::high_resolution() ));
    bm::time(( bm::func = [](){}, bm::sampling_policy = bm::sampling::single_sample<>(), 
                                  bm::timer_policy = bm::timer::high_resolution() ));
    bm::time(( bm::func = [](){}, bm::setup = [](){}, 
                                  bm::sampling_policy = bm::sampling::single_sample<>(), 
                                  bm::timer_policy = bm::timer::high_resolution() ));

    // trying different params order
    bm::time(( bm::timer_policy = bm::timer::high_resolution(),
               bm::num_iterations = 3,
               bm::sampling_policy = bm::sampling::single_sample<>(), 
               bm::setup = [](){},
               bm::func = [](){} ));

    // trying different sampling units
    bm::time(( bm::sampling_policy = bm::sampling::single_sample<chrono::seconds>(), 
               bm::func = [](){} ));
}

#endif
//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE( sampling_policy_ts )

BOOST_AUTO_TEST_CASE( single_sample_test )
{
    bm::sampling::single_sample<chrono::milliseconds> policy;
    chrono::milliseconds sample( 123LL );

    BOOST_TEST( !policy.is_done() );
    BOOST_TEST( policy.total_samples() == 0U );

    policy.add_sample( sample );

    BOOST_TEST( policy.is_done() );
    BOOST_TEST( policy.total_samples() == 1U );
    BOOST_TEST( policy.value() == sample );
    BOOST_TEST( policy.best() == sample );
    BOOST_TEST( policy.worst() == sample );
    BOOST_TEST( policy.variance().count() == 0LL );

    policy.add_sample( chrono::milliseconds( 321LL ) );
    BOOST_TEST( policy.value() == sample );

    BOOST_TEST( policy.total_samples() == 1U );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( average_test )
{
    bm::sampling::average<chrono::nanoseconds> policy( 5 );

    BOOST_TEST( !policy.is_done() );
    BOOST_TEST( policy.total_samples() == 0U );

    policy.add_sample( 95023_ns );

    BOOST_TEST( !policy.is_done() );
    BOOST_TEST( policy.total_samples() == 1U );

    policy.add_sample( 95893_ns );

    BOOST_TEST( !policy.is_done() );
    BOOST_TEST( policy.total_samples() == 2U );

    policy.add_sample( 96354_ns );
    policy.add_sample( 94801_ns );
    policy.add_sample( 95502_ns );

    BOOST_TEST( policy.is_done() );
    BOOST_TEST( policy.total_samples() == 5U );

    // actual mean = 95514.6
    BOOST_TEST( policy.value() == 95515_ns );
    BOOST_TEST( policy.best() == 94801_ns );
    BOOST_TEST( policy.worst() == 96354_ns );
    // actual variane = 399708.3, but we are close.
    // standard deviation difference is negligeble
    BOOST_TEST( policy.variance() == 399800_ns );

    policy.add_sample( 103_ns );

    BOOST_TEST( policy.is_done() );
    BOOST_TEST( policy.total_samples() == 5U );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( average_double_seconds_test )
{
    bm::sampling::average<double_sec> policy( 3 );

    policy.add_sample( 1.2031_sec );
    policy.add_sample( 1.1984_sec );
    policy.add_sample( 1.197_sec );

    BOOST_TEST( policy.is_done() );
    BOOST_TEST( policy.total_samples() == 3U );

    tt::local_fpc_tolerance<double> tolerance( 1e-7 );

    BOOST_TEST( policy.value().count() == 1.1995 );
    BOOST_TEST( policy.best().count() == 1.197 );
    BOOST_TEST( policy.worst().count() == 1.2031 );
    BOOST_TEST( policy.variance().count() == 1.021e-05 );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( skip_first_test )
{
    using namespace bm::sampling;

    skip_first<single_sample<>> policy;

    BOOST_TEST( !policy.is_done() );
    BOOST_TEST( policy.total_samples() == 0U );

    policy.add_sample( 123_ns );

    BOOST_TEST( !policy.is_done() );
    BOOST_TEST( policy.total_samples() == 0U );

    policy.add_sample( 123_ns );

    BOOST_TEST( policy.is_done() );
    BOOST_TEST( policy.total_samples() == 1U );

    BOOST_TEST( policy.value() == 123_ns );

    skip_first<average<chrono::milliseconds>> policy2{ average<chrono::milliseconds>{ 3 } };

    policy2.add_sample( 123_ms );

    BOOST_TEST( !policy2.is_done() );
    BOOST_TEST( policy2.total_samples() == 0U );

    policy2.add_sample( 345_ms );

    BOOST_TEST( !policy2.is_done() );
    BOOST_TEST( policy2.total_samples() == 1U );

    policy2.add_sample( 348_ms );
    policy2.add_sample( 342_ms );

    BOOST_TEST( policy2.is_done() );
    BOOST_TEST( policy2.total_samples() == 3U );

    BOOST_TEST( policy2.value() == 345_ms );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( stop_on_convergence_test )
{
    using namespace bm::sampling;

    stop_on_convergence<average<double_sec>> policy{ average<double_sec>{ 100 }, 0.5_sec };

    policy.add_sample( 5.12_sec );
    policy.add_sample( 6.37_sec );
    policy.add_sample( 4.66_sec );

    BOOST_TEST( !policy.is_done() );
    BOOST_TEST( policy.total_samples() == 3U );

    policy.add_sample( 5.31_sec );
    policy.add_sample( 4.87_sec );
    policy.add_sample( 5.55_sec );
    policy.add_sample( 4.98_sec );
    policy.add_sample( 5.76_sec );
    policy.add_sample( 5.09_sec );
    policy.add_sample( 5.42_sec );

    BOOST_TEST( policy.is_done() );
    BOOST_TEST( policy.total_samples() == 10U );

    policy.add_sample( 6.01_sec );

    BOOST_TEST( policy.total_samples() == 10U );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( policy_chains_test )
{
    using namespace bm::sampling;

    auto policy = make_chain<single_sample<>, skip_first, skip_first>();

    policy.add_sample( 123_ns );
    policy.add_sample( 123_ns );

    BOOST_TEST( !policy.is_done() );
    BOOST_TEST( policy.total_samples() == 0U );

    policy.add_sample( 123_ns );

    BOOST_TEST( policy.is_done() );
    BOOST_TEST( policy.total_samples() == 1U );

    auto policy2 = make_chain<average<chrono::nanoseconds>, skip_first, skip_first>( 3 );

    policy2.add_sample( 123_ns );
    policy2.add_sample( 123_ns );

    BOOST_TEST( policy2.total_samples() == 0U );

    policy2.add_sample( 123_ns );
    policy2.add_sample( 321_ns );
    policy2.add_sample( 543_ns );

    BOOST_TEST( policy2.is_done() );
    BOOST_TEST( policy2.total_samples() == 3U );

    BOOST_TEST( policy2.value() == 329_ns );
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_CASE( result_test )
{
    bm::sampling::average<double_sec> policy( 3 );

    policy.add_sample( 2.5_sec );
    policy.add_sample( 2.61_sec );
    policy.add_sample( 2.55_sec );

    bm::result res( policy, 150 );

    BOOST_TEST( res.estimated() == 17022222_ns );
    BOOST_TEST( res.best() == 16666666_ns );
    BOOST_TEST( res.worst() == 17400000_ns );
    BOOST_TEST( res.error() == 4496912_ns );

    BOOST_TEST( res.estimated<double_sec>().count() == 0.0170222, 1.% tt::tolerance() );
    BOOST_TEST( res.estimated<chrono::milliseconds>().count() == 17 );
}

//____________________________________________________________________________//

BOOST_DATA_TEST_CASE( benchmark_time_report_test, 
                      data::make({ 123_ns, 84607_ns, 2359011_ns, 346234678_ns, 400933842101_ns }) *
                      data::make({ 1000, 10, 1 }),
                      sample, num_iter )
{
    static boost::shared_ptr<tt::output_test_stream> test_stream;

    if( !test_stream ) {
        std::string golden = "benchmark_time_report_test.golden";
        
        if( ut::framework::master_test_suite().argc > 1 )
            golden = ut::framework::master_test_suite().argv[1];
        else if( !ut::runtime_config::save_pattern() )
            golden = "./baseline-outputs/" + golden;

        test_stream.reset( new tt::output_test_stream( golden, !ut::runtime_config::save_pattern() ) );
    }

    *test_stream << "==========================================================\n";
    *test_stream << "sample: " << sample << "; num_iter: " << num_iter << "\n";

    BOOST_TEST( test_stream->match_pattern() );
    
    using namespace boost::chrono;
    bm::sampling::single_sample<> simple_sample_policy;
    simple_sample_policy.add_sample( sample );    
    bm::result res( simple_sample_policy, num_iter );

#if 0
    res.report_time<nanoseconds>( *test_stream );
    BOOST_TEST( test_stream->match_pattern() );

    res.report_time<microseconds>( *test_stream );
    BOOST_TEST( test_stream->match_pattern() );

    res.report_time<milliseconds>( *test_stream, 8, false );
    BOOST_TEST( test_stream->match_pattern() );

    res.report_time<seconds>( *test_stream );
    BOOST_TEST( test_stream->match_pattern() );

    res.report_time<minutes>( *test_stream );
    BOOST_TEST( test_stream->match_pattern() );

    res.report_time<duration<double, boost::micro>>( *test_stream );
    BOOST_TEST( test_stream->match_pattern() );

    res.report_time<duration<double, boost::milli>>( *test_stream );
    BOOST_TEST( test_stream->match_pattern() );

    res.report_time<duration<double>>( *test_stream, 5 );
    BOOST_TEST( test_stream->match_pattern() );

    res.report_time<duration<double, boost::ratio<60>>>( *test_stream, 2 );
    BOOST_TEST( test_stream->match_pattern() );
#endif

    res.report_time( *test_stream );
    BOOST_TEST( test_stream->match_pattern() );

    bm::sampling::average<chrono::duration<double,boost::micro>> average_policy( 7 );
    average_policy.add_sample( sample * 0.96 );    
    average_policy.add_sample( sample * 0.95 );    
    average_policy.add_sample( sample * 1.04 );    
    average_policy.add_sample( sample * 1.02 );    
    average_policy.add_sample( sample * 1.07 );    
    average_policy.add_sample( sample * 0.99 );    
    average_policy.add_sample( sample * 0.955 );    
 
    bm::result res2( average_policy, num_iter );

    res2.report_time( *test_stream );
    BOOST_TEST( test_stream->match_pattern() );

//    res2.report_performance<chrono::milliseconds>( *test_stream, 4 );
//    BOOST_TEST( test_stream->match_pattern() );
}

BOOST_AUTO_TEST_CASE( practical_example_test )
{
    const boost::int_least64_t MAX = 100;

    std::vector<boost::int_least64_t> v;
    v.reserve( MAX );

    auto res = bm::time(( 
        bm::func = [&v, MAX]() {
            for( boost::int_least64_t i = 0; i<MAX; ++i )
                v.push_back( i );
        }, 
        bm::setup = [&v]() {
            v.clear();
        },
        bm::num_iterations = 10,
        bm::sampling_policy = bm::sampling::average<>( 10000 ) ));

    res.report_time( std::cout );
    res.report_time<double_sec>( std::cout );
}

// EOF
