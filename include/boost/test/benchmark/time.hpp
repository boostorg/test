//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief 
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_TIME_HPP
#define BOOST_TEST_BENCHMARK_TIME_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

#include <boost/test/benchmark/result.hpp>
#include <boost/test/benchmark/sampling/single_sample.hpp>
#include <boost/test/benchmark/timer/chrono_based.hpp>
#include <boost/test/benchmark/detail/single_run.hpp>

// Boost.Test utils
#include <boost/test/utils/named_params.hpp>
#include <boost/test/utils/static_constant.hpp>

// Boost
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/chrono/duration.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {

using default_sampling_policy = sampling::single_sample<>;
using default_timer_policy = timer::high_resolution;

template<typename Func, typename Setup, typename SamplingPolicy, typename TimerPolicy>
inline result
time( Func&& func, 
      Setup&& setup,
      SamplingPolicy sampling_policy,
      TimerPolicy const&,
      iterations_t forced_num_iterations = 0 )
{
    using single_run=benchmark::detail::single_run<Func, Setup, TimerPolicy>;
    // prepare wrapper ensuring measurable single run
    single_run sr_wrap( std::forward<Func>( func ), 
                        std::forward<Setup>( setup ),
                        forced_num_iterations );

    // keep sampling, till policy tells us we are done
    while( !sampling_policy.is_done() )
        sr_wrap.run( sampling_policy );

    // collect the results attributing for the "number of iteration in a single run"
    return result( sampling_policy, sr_wrap.num_iterations() );
}

//____________________________________________________________________________//

template<typename SamplingPolicy, typename Func, typename Setup>
inline result
time( Func&& func, Setup&& setup, SamplingPolicy&& sampling_policy = SamplingPolicy() )
{
    return benchmark::time( std::forward<Func>( func ), 
                            std::forward<Setup>( setup ),
                            std::forward<SamplingPolicy>( sampling_policy ),
                            default_timer_policy() );
}

//____________________________________________________________________________//

template<typename Func, typename Setup=nullptr_t>
inline typename enable_if_c<!nfp::is_named_param_pack<Func>::value,result>::type
time( Func&& func, Setup&& setup = Setup() )
{
    return benchmark::time( std::forward<Func>( func ),
                            std::forward<Setup>( setup ),
                            default_sampling_policy(),
                            default_timer_policy() );
}

//____________________________________________________________________________//

using func_kw               = nfp::keyword<struct func_t>;
using setup_kw              = nfp::keyword<struct setup_t>;
using sampling_policy_kw    = nfp::keyword<struct sampling_policy_t>;
using timer_policy_kw       = nfp::keyword<struct timer_policy_t>;
using num_iterations_kw     = nfp::typed_keyword<iterations_t, struct num_iterations_t>;

namespace {

auto const& func            = unit_test::static_constant<func_kw>::value;
auto const& setup           = unit_test::static_constant<setup_kw>::value;
auto const& sampling_policy = unit_test::static_constant<sampling_policy_kw>::value;
auto const& timer_policy    = unit_test::static_constant<timer_policy_kw>::value;
auto const& num_iterations  = unit_test::static_constant<num_iterations_kw>::value;

} // local namespace

//____________________________________________________________________________//

namespace detail {

template<typename Params, typename Keyword, typename Default>
inline typename nfp::param_type<Params,Keyword,Default>::type const&
get_param_impl( Params const&, Keyword const&, Default const& default_val, mpl::false_ )
{
    return default_val;
}

//____________________________________________________________________________//

template<typename Params, typename Keyword, typename Default>
inline typename nfp::param_type<Params,Keyword,Default>::type const&
get_param_impl( Params const& params, Keyword const& kw, Default const&, mpl::true_ )
{
    return params[kw];
}

//____________________________________________________________________________//

template<typename Params, typename Keyword, typename Default>
inline typename nfp::param_type<Params,Keyword,Default>::type const&
get_param( Params const& params, Keyword const& kw, Default const& default_val )
{
    return get_param_impl( params, kw, default_val, nfp::has_param<Params, Keyword>() );
}

} // namespace detail

//____________________________________________________________________________//

template<typename Params>
inline typename enable_if<nfp::is_named_param_pack<Params>,result>::type
time( Params const& params )
{
    return benchmark::time(
        params[benchmark::func],
        detail::get_param( params, benchmark::setup, nullptr ), 
        detail::get_param( params, benchmark::sampling_policy, default_sampling_policy() ), 
        detail::get_param( params, benchmark::timer_policy, default_timer_policy() ), 
        detail::get_param( params, benchmark::num_iterations, 0 ) );
}

} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_TIME_HPP
