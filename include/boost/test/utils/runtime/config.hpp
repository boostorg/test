//  (C) Copyright Gennadiy Rozental 2005-2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : Runtime.Param library configuration
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_CONFIG_HPP
#define BOOST_TEST_UTILS_RUNTIME_CONFIG_HPP

// Boost
#include <boost/config.hpp>
#ifdef BOOST_MSVC
# pragma warning(disable: 4511) // copy constructor could not be generated
# pragma warning(disable: 4512) // assignment operator could not be generated
# pragma warning(disable: 4181) // qualifier applied to reference type; ignored
# pragma warning(disable: 4675) // resolved overload was found by argument-dependent lookup
#endif

// Boost.Test
#include <boost/test/detail/config.hpp>
#include <boost/test/utils/basic_cstring/basic_cstring.hpp>
#include <boost/test/utils/wrap_stringstream.hpp>
#include <boost/test/utils/basic_cstring/io.hpp> // operator<<(boost::runtime::cstring)

// STL
#include <string>
#include <cstdlib>

#ifdef __SUNPRO_CC
  #include <stdlib.h>
#endif

//____________________________________________________________________________//

namespace boost {
namespace runtime {

#ifndef BOOST_TEST_UTILS_RUNTIME_PARAM_CUSTOM_STRING

typedef unit_test::const_string     cstring;
typedef unit_test::literal_string   literal_cstring;
typedef wrap_stringstream           format_stream;

#ifdef BOOST_MSVC
#pragma warning(push)
#pragma warning(disable:4996) // putenv
#endif

#if defined(__MINGW32__)
extern "C" int putenv( const char * );
#endif

#ifndef UNDER_CE
#if defined(__COMO__) && 0
inline void
putenv_impl( cstring name, cstring value )
{
    using namespace std;
    // !! this may actually fail. What should we do?
    setenv( name.begin(), value.begin(), 1 );
}
#else
inline void
putenv_impl( cstring name, cstring value )
{
    format_stream fs;

    fs << name << '=' << value;

    // !! this may actually fail. What should we do?
    // const_cast is used to satisfy putenv interface
    using namespace std;
    putenv( const_cast<char*>( fs.str().c_str() ) );
}
#endif
#endif

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

//____________________________________________________________________________//

#endif

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_CONFIG_HPP
