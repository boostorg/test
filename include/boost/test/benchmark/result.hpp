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
#include <boost/chrono/io/duration_units.hpp>
#include <boost/io/ios_state.hpp>

// STL
#include <cmath>
#include <iosfwd>
#include <iomanip>

//____________________________________________________________________________//

namespace boost {
namespace benchmark {

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
        // V is sample variance in sample units
        //
        // Sample Variance: V(sample) = V in SU^2 = V * (SU/1ns)^2 in ns^2
        // Call Variance: V(call) = V(sample) / NI = V * (SU/1ns)^2 / NI in ns^2
        // Call StdDev:   SD(call) = SQRT( V(call) ) in ns = SQRT( V / NI ) * SU/1ns in ns = 
        // = (SQRT( V * NI ) * SU/1ns) / NI in ns
        // Since we divide all values by NI in the end we store measument error as
        // measument_error = SQRT( V * NI ) * SU/1ns in ns = SQRT( V * NI ) in SU
        using sample_units = SamplingPolicy::units_t;

        m_measument_error = _get_nano( sample_units{
            static_cast<sample_units::rep>( std::sqrt( sampling_policy.variance().count() * m_num_iterations ) ) } );
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
        ostr << std::setprecision( precision );

        auto style = abbreviate_units ? chrono::duration_style::symbol : chrono::duration_style::prefix;

        auto nanosec_per_unit = ReportUnits{1} / chrono::nanoseconds{1};
        decltype(nanosec_per_unit) estimated_performance{};

        chrono::duration_units_default<char> facet;
        
        auto units_name = " calls/" + facet.get_unit( style, ReportUnits{} );

        if( m_estimated_value != 0LL ) {
            estimated_performance = nanosec_per_unit * m_num_iterations / m_estimated_value;
            ostr << "Estimated performance: " << estimated_performance << units_name << " (";

            if( m_measument_error != 0LL ) {
                // If RU is report units and given 
                //      SD(call) = measument_error / NI in ns
                //  and 
                //      MEAN(call) = estimated_value / NI in ns
                // We can get an approximation for SD(RU/call).
                // SD(RU/call) = SD(ns/call) * RU/1ns
                // SD(ns/call) ~= SD(call) / MEAN(call)^2 in "calls per ns" = 
                // = (measument_error / NI ) / (estimated_value / NI) ^ 2  in "calls per ns" = 
                // = measument_error * NI / estimated_value^2 in "calls per ns" = 
                // = measument_error * NI * RU/1ns / estimated_value^2 in "calls per RU"

                // auto perf_error = nanosec_per_unit * m_num_iterations * m_measument_error / 
                //                   (m_estimated_value * m_estimated_value);
                auto perf_error = estimated_performance * m_measument_error / m_estimated_value;

                if( perf_error != 0 )
                    ostr << "+/-" << perf_error << units_name << " ";
            }

            ostr << "based on " << m_total_measured_calls << " measured call"
                 << (m_total_measured_calls > 1 ? "s" : "" ) << ")" ;
        }

        if( m_best_value != 0 ) {
            auto best_performance = nanosec_per_unit * m_num_iterations / m_best_value;
            ostr << "\nBest time:      " << best_performance << units_name;
            if( m_estimated_value != m_best_value )
                ostr << " (+" << _percent_diff( estimated_performance, best_performance ) << "%)";
        }

        if( m_worst_value != 0 ) {
            auto worst_perfomance = nanosec_per_unit * m_num_iterations / m_worst_value;
            ostr << "\nWorst time:     " << worst_perfomance << units_name;
            if( m_estimated_value != m_worst_value )
                ostr << " (" << _percent_diff( estimated_performance, worst_perfomance ) << "%)";

            ostr << "\n";
        }
    }
    void            report_performance( std::ostream& ostr, std::streamsize precision = 3, bool abbreviate_units = true )
    {
        if( m_estimated_value == 0LL ) 
            return;

        nanosecond_t nanosec_per_unit = chrono::seconds{1} / chrono::nanoseconds{1};
        const nanosecond_t thou = 1000LL;

        if( (60 * nanosec_per_unit * m_num_iterations / m_estimated_value) == 0 )
            report_performance<chrono::duration<double, ratio<60>>>( ostr, precision, abbreviate_units );
        else if( (nanosec_per_unit * m_num_iterations / m_estimated_value) == 0 )
            report_performance<chrono::minutes>( ostr, precision, abbreviate_units );
        else if( (nanosec_per_unit * m_num_iterations / m_estimated_value) < thou )
            report_performance<chrono::seconds>( ostr, precision, abbreviate_units );
        else if( ((nanosec_per_unit /= thou) * m_num_iterations / m_estimated_value) < thou )
            report_performance<chrono::milliseconds>( ostr, precision, abbreviate_units );
        else if( ((nanosec_per_unit /= thou) * m_num_iterations / m_estimated_value) < thou )
            report_performance<chrono::microseconds>( ostr, precision, abbreviate_units );
        else if( (nanosec_per_unit * m_num_iterations / m_estimated_value) != 0LL )
            report_performance<chrono::nanoseconds>( ostr, precision, abbreviate_units );
        else
            report_performance<chrono::duration<double, boost::nano>>( ostr, precision, abbreviate_units );
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
    double          _percent_diff( Units base, Units some_val )
    {
        if( base == Units{0} )
            return 0.;

        Units delta = 100*(some_val-base);

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
