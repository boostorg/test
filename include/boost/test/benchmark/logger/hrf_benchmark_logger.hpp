//  (C) Copyright Raffi Enficiaud 2018.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief Contains the logger abstraction for storing results
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_LOGGER_HPP__
#define BOOST_TEST_BENCHMARK_LOGGER_HPP__

// Boost.Test Benchmark
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

protected:
    e_time_units t_unit;
    result baseline;
};

struct hrf_logger : public benchmark_logger {


    virtual void produce_logs(std::ostream& ost) const {


    }

};

}}} // namespace boost::benchmark::logger

#endif /* BOOST_TEST_BENCHMARK_LOGGER_HPP__ */
