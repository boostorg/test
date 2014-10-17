//  (C) Copyright Jamie Allsop 2012.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  Description : timer and elapsed types
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_TIMER_HPP
#define BOOST_TEST_UTILS_TIMER_HPP

#ifdef BOOST_TEST_USE_DEPRECATED_TIMER
#include <boost/timer.hpp>
#else
#include <boost/timer/timer.hpp>
#endif
#include <sstream>

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************          opaque timer and elapsed types      ************** //
// ************************************************************************** //

#ifdef BOOST_TEST_USE_DEPRECATED_TIMER

typedef boost::timer timer_t;
typedef unsigned long elapsed_t;

inline std::string to_string( elapsed_t elapsed )
{
    std::ostringstream output;
    if( elapsed % 1000 == 0 )
    {
        output << elapsed/1000 << "ms";
    }
    else
    {
        output << elapsed << "mks";
    }
    return output.str();
}

inline std::string to_xml( elapsed_t elapsed )
{
    std::ostringstream output;
    output << "<TestingTime>" << elapsed << "</TestingTime>";
    return output.str();
}

inline bool has_time( const elapsed_t& elapsed )
{
    return elapsed;
}

#else

typedef boost::timer::cpu_timer timer_t;
typedef boost::timer::cpu_times elapsed_t;

inline std::string to_string( elapsed_t elapsed )
{
    return boost::timer::format( elapsed, 9, "%ws wall, %us user + %ss system = %ts CPU (%p%)" );
}

inline std::string to_xml( elapsed_t elapsed )
{
    std::ostringstream output;
    output << "<TestingTime>" << ( elapsed.user + elapsed.system ) << "</TestingTime>"
           << "<WallTime>"    << elapsed.wall   << "</WallTime>"
           << "<UserTime>"    << elapsed.user   << "</UserTime>"
           << "<SystemTime>"  << elapsed.system << "</SystemTime>";
    return output.str();
}

inline bool has_time( const elapsed_t& elapsed )
{
    return elapsed.wall != 0 || elapsed.user != 0 || elapsed.system != 0;
}

#endif

//____________________________________________________________________________//

} // namespace unit_test

} // namespace boost

#endif // BOOST_TEST_UTILS_TIMER_HPP

