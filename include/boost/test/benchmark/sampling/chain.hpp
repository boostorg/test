//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief This header contains definition of sampling::chain, which allows
//!        chain terminal policy and arbitrary number of aspects to produce 
//!        new policies.
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_SAMPLING_CHAIN_HPP
#define BOOST_TEST_BENCHMARK_SAMPLING_CHAIN_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {
namespace sampling {

template<typename TerminalPolicy, template<typename> class ...Aspects>
struct chain;

template<typename TerminalPolicy>
struct chain<TerminalPolicy> {
    using type = TerminalPolicy;
};

template<typename TerminalPolicy, template<typename> class Aspect, template<typename> class ...Aspects>
struct chain<TerminalPolicy, Aspect, Aspects...> {
	using type = typename chain<Aspect<TerminalPolicy>, Aspects...>::type;
};

template<typename TerminalPolicy, template<typename> class ...Aspects, typename ...Args>
typename chain<TerminalPolicy, Aspects...>::type
make_chain( Args... args )
{
    using chain_type = typename chain<TerminalPolicy, Aspects...>::type;

    return chain_type( args... );
}

template<typename TerminalPolicy, template<typename> class ...Aspects>
typename chain<TerminalPolicy, Aspects...>::type
make_chain()
{
    using chain_type = typename chain<TerminalPolicy, Aspects...>::type;

    return chain_type();
}

} // namespace sampling
} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_SAMPLING_CHAIN_HPP
