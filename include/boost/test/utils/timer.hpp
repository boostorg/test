//  (C) Copyright Jamie Allsop 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  Description : timer and elapsed types
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_TIMER_HPP
#define BOOST_TEST_UTILS_TIMER_HPP

#include <boost/test/unit_test_parameters.hpp>
#include <boost/timer/timer.hpp>
#include <sstream>

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************          opaque timer and elapsed types      ************** //
// ************************************************************************** //

typedef boost::timer::cpu_timer timer_t;
typedef boost::timer::cpu_times elapsed_t;
typedef boost::timer::nanosecond_type nanoseconds_t;

inline long microsecond_cpu_time( elapsed_t elapsed )
{
    return ( elapsed.user + elapsed.system )/1000;
}

inline std::string deprecated_hrf_format( elapsed_t elapsed )
{
    std::ostringstream output;
    long duration = microsecond_cpu_time( elapsed );
    if( duration % 1000 == 0 )
    {
        output << duration/1000 << "ms";
    }
    else
    {
        output << duration << "mks";
    }
    return output.str();
}

inline std::string to_string( elapsed_t elapsed )
{
    if( runtime_config::deprecated_timer_format() )
    {
        return deprecated_hrf_format( elapsed );
    }
    return boost::timer::format( elapsed, 9, "%ws wall, %us user + %ss system = %ts CPU (%p%)" );
}

inline std::string to_xml( elapsed_t elapsed )
{
    std::ostringstream output;
    output << "<TestingTime>" << microsecond_cpu_time( elapsed )   << "</TestingTime>"
           << "<CpuTime>"     << ( elapsed.user + elapsed.system ) << "</CpuTime>"
           << "<WallTime>"    << elapsed.wall                      << "</WallTime>"
           << "<UserTime>"    << elapsed.user                      << "</UserTime>"
           << "<SystemTime>"  << elapsed.system                    << "</SystemTime>";
    return output.str();
}

inline bool has_time( const elapsed_t& elapsed )
{
    return      !runtime_config::suppress_timer_output()
            &&  ( elapsed.wall != 0 || elapsed.user != 0 || elapsed.system != 0 );
}

//____________________________________________________________________________//

} // namespace unit_test

} // namespace boost

#endif // BOOST_TEST_UTILS_TIMER_HPP

