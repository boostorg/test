//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief benchmark::result contains the result of the benchmark::time invocation
//!        and facilitates human readable report output.
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_RESULT_HPP
#define BOOST_TEST_BENCHMARK_RESULT_HPP

// Boost.Test Benchmark
#include <boost/test/benchmark/config.hpp>

// Boost
#include <boost/chrono/duration.hpp>
#include <boost/chrono/io/duration_io.hpp>
#include <boost/io/ios_state.hpp>

// STL
#include <cmath>
#include <iosfwd>
#include <iomanip>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {

//!! different report formats?

class result {
public:
    template<typename SamplingPolicy>
    result( SamplingPolicy const& sampling_policy, iterations_t num_iterations )
    : m_estimated_value( _get_nano( sampling_policy.value() ) )
    , m_best_value( _get_nano( sampling_policy.best() ) )
    , m_worst_value( _get_nano( sampling_policy.worst() ) )
    , m_total_measured_calls( sampling_policy.total_samples() * num_iterations )
    , m_num_iterations( num_iterations )
    {
        // If SU is sample units , NI is number of iterations,
        // V count of sample units in sample variance
        //
        // Sample Variance: Vs = V in SU^2 = V * (SU/1ns)^2 in ns^2
        // Call Variance: Vc = Vs / NI = V * (SU/1ns)^2 / NI in ns^2
        // Call StdDev:   SDc = SQRT( Vc ) = SQRT( V / NI ) * SU/1ns = 
        // = (SQRT( V * NI ) * SU/1ns) / NI
        // Since we devide all values by NI in the end we store measument error as
        // measument_error = SQRT( V * NI ) * SU/1ns = SQRT( V * NI ) in SU
        using sample_units = SamplingPolicy::units_t;

        m_measument_error = _get_nano( sample_units{
            static_cast<sample_units::rep>( std::sqrt(sampling_policy.variance().count() * m_num_iterations )) } );
    }

    // Access methods
    template<typename Units=chrono::nanoseconds>
    Units           estimated() const    { return _as<Units>( m_estimated_value ); }
    template<typename Units=chrono::nanoseconds>
    Units           best() const        { return _as<Units>( m_best_value ); }
    template<typename Units=chrono::nanoseconds>
    Units           worst() const       { return _as<Units>( m_worst_value ); }
    template<typename Units=chrono::nanoseconds>
    Units           error() const       { return _as<Units>( m_measument_error ); }

    // I/O
    template<typename ReportUnits>
    void            report_time( std::ostream& ostr, std::streamsize precision = 3, bool abbreviate_units = true )
    {
        io::ios_precision_saver precision_saver( ostr );

        chrono::duration_style_io_saver style_saver( ostr, abbreviate_units
                                                            ? chrono::duration_style::symbol
                                                            : chrono::duration_style::prefix );

        ostr << std::setprecision( precision );

        ostr << "Estimated time: " << estimated<ReportUnits>() << " (";
        if( m_measument_error != 0LL )
            ostr << "+/-" << error<ReportUnits>() << " ";

        ostr << "based on " << m_total_measured_calls << " measured call"
             << (m_total_measured_calls > 1 ? "s" : "" ) << ")" ;

        ostr << "\nBest time:      " << best<ReportUnits>();
        if( m_estimated_value != m_best_value )
            ostr << " (" << _percent_diff( m_estimated_value, m_best_value ) << "%)";

        ostr << "\nWorst time:     " << worst<ReportUnits>();
        if( m_estimated_value != m_worst_value )
            ostr << " (+" << _percent_diff( m_estimated_value, m_worst_value ) << "%)";

        ostr << "\n";
    }
    void            report_time( std::ostream& ostr, std::streamsize precision = 3, bool abbreviate_units = true )
    {
        nanosecond_t value = m_estimated_value / m_num_iterations;
        const nanosecond_t thou = 1000LL;

        if( value < thou )
            report_time<chrono::duration<double, nano>>( ostr, precision, abbreviate_units );
        else if( (value /= thou) < thou )
            report_time<chrono::duration<double, micro>>( ostr, precision, abbreviate_units );
        else if( (value /= thou) < thou )
            report_time<chrono::duration<double, milli>>( ostr, precision, abbreviate_units );
        else if( (value /= thou) < thou )
            report_time<chrono::duration<double>>( ostr, precision, abbreviate_units );
        else
            report_time<chrono::duration<double, ratio<60>>>( ostr, precision, abbreviate_units );
    }
    template<typename ReportUnits>
    void            report_performance( std::ostream& ostr, std::streamsize precision = 3, bool abbreviate_units = true )
    {
        io::ios_precision_saver precision_saver( ostr );

        chrono::duration_style_io_saver style_saver( ostr, abbreviate_units
                                                            ? chrono::duration_style::symbol
                                                            : chrono::duration_style::prefix );

        ostr << std::setprecision( precision );

        ReportUnits per_unit{1};

        ostr << "Estimated performance: " << _perf<ReportUnits>( m_estimated_value ) << " (";
        if( m_measument_error != 0LL )
            ostr << "+/-" << error<ReportUnits>() << " ";

        ostr << "based on " << m_total_measured_calls << " measured call"
             << (m_total_measured_calls > 1 ? "s" : "" ) << ")" ;

        ostr << "\nBest time:      " << per_unit / best<ReportUnits>();
        if( m_estimated_value != m_best_value )
            ostr << " (+" << _percent_diff( m_estimated_value, m_best_value ) << "%)";

        ostr << "\nWorst time:     " << per_unit / worst<ReportUnits>();
        if( m_estimated_value != m_worst_value )
            ostr << " (" << _percent_diff( m_estimated_value, m_worst_value ) << "%)";

        ostr << "\n";
    }

private:
    template<typename SamplingUnits>
    static nanosecond_t _get_nano( SamplingUnits const& sample )
    {
        return chrono::duration_cast<chrono::nanoseconds>(sample).count();        
    }
    template<typename Units>
    Units           _as( nanosecond_t val ) const
    {
        return chrono::duration_cast<Units>( chrono::nanoseconds( val ) ) / m_num_iterations;
    }
    template<typename Units>
    typename Units::rep _perf( nanosecond_t val ) const
    {
        return Units{1} / chrono::nanoseconds( val ) * m_num_iteration;
    }
    template<typename Units>
    static constexpr double _std_dev_mult()
    {
        return std::sqrt( Units{1} / chrono::nanoseconds{1} );
    }

    double          _percent_diff( nanosecond_t base, nanosecond_t some_val )
    {
        nanosecond_t delta = 100*(some_val-base);

        return static_cast<double>(delta) / base;
    }

    // Data members
    nanosecond_t    m_estimated_value;
    nanosecond_t    m_best_value;
    nanosecond_t    m_worst_value;
    nanosecond_t    m_measument_error;
    num_samples_t   m_total_measured_calls;
    iterations_t    m_num_iterations;
};

} // namespace benchmark
} // namespace boost

#endif // BOOST_TEST_BENCHMARK_RESULT_HPP
