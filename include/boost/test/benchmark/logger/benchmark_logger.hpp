//  (C) Copyright Raffi Enficiaud 2019.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief Contains the benchmark logger interface
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_LOGGER_HPP__
#define BOOST_TEST_BENCHMARK_LOGGER_HPP__

#include <boost/test/benchmark/config.hpp>
#include <boost/test/benchmark/result.hpp>

namespace boost {
namespace benchmark {
namespace logger {

struct benchmark_logger {

    //! Sets the baseline of the results
    //!
    //! @param baseline_result the result to use as a baseline
    virtual void set_baseline(result const& baseline_result) {
        baseline = baseline_result;
    }

    //! Sets the time unit for the reports
    virtual void set_benchmark_time_units(e_time_units unit) {
        t_unit = unit;
    }

    //! Outputs the logs to the selected stream
    virtual void produce_logs(std::ostream& ost) const = 0;

    virtual void start_benchmark() = 0;
    virtual void stop_benchmark() = 0;

    virtual void enter_benchmark_scope(std::string const &scope_name) = 0;
    virtual void exit_benchmark_scope() = 0;

    virtual void add_result(result const& measurements);
    virtual void add_comparative_result(result const& measurements, result const& baseline);

};

}}}

#endif /* benchmark_logger_h */
