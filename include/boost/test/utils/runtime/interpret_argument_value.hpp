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
//  Description : algorithms for string to specific type convertions
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_INTERPRET_ARGUMENT_VALUE_HPP
#define BOOST_TEST_UTILS_RUNTIME_INTERPRET_ARGUMENT_VALUE_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>
#include <boost/test/utils/runtime/errors.hpp>

// Boost.Test
#include <boost/test/utils/basic_cstring/io.hpp>
#include <boost/test/utils/basic_cstring/compare.hpp>

// Boost
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <vector>

namespace boost {
namespace runtime {

// ************************************************************************** //
// **************       runtime::interpret_argument_value      ************** //
// ************************************************************************** //

// std::string case
inline void
interpret_argument_value( cstring source, std::string& res )
{
    res.assign( source.begin(), source.size() );
}

//____________________________________________________________________________//

// cstring overload
inline void
interpret_argument_value( cstring source, cstring& res )
{
    res = source;
}

//____________________________________________________________________________//

// bool overload
inline void
interpret_argument_value( cstring source, bool& res )
{
    static literal_cstring YES( "YES" );
    static literal_cstring Y( "Y" );
    static literal_cstring NO( "NO" );
    static literal_cstring N( "N" );
    static literal_cstring one( "1" );
    static literal_cstring zero( "0" );

    source.trim();

    if( source.is_empty() || case_ins_eq( source, YES ) || case_ins_eq( source, Y ) || case_ins_eq( source, one ) )
        res = true;
    else if( case_ins_eq( source, NO ) || case_ins_eq( source, N ) || case_ins_eq( source, zero ) )
        res = false;
    else
        BOOST_TEST_IMPL_THROW( format_error() << source << " can't be interpreted as bool value." );
}

//____________________________________________________________________________//

// overload for list of values
template<typename T>
inline void
interpret_argument_value( cstring source, std::vector<T>& res )
{
    while( !source.is_empty() ) {
        auto single_value_end = std::find( source.begin(), source.end(), ',' );

        T value;
        interpret_argument_value( cstring( source.begin(), single_value_end ), value );

        res.push_back( std::move( value ) );

        source.trim_left( single_value_end + 1 );
    }
}

//____________________________________________________________________________//

// generic overload
template<typename T>
inline void
interpret_argument_value( cstring source, T& res )
{
    res = lexical_cast<T>( source );
}

//____________________________________________________________________________//

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_INTERPRET_ARGUMENT_VALUE_HPP
