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
// returns true if source is used false otherwise

// std::string case
inline bool
interpret_argument_value( cstring source, boost::optional<std::string>& res )
{
    res = std::string();
    assign_op( *res, source, 0 );

    return true;
}

//____________________________________________________________________________//

// cstring overload
inline bool
interpret_argument_value( cstring source, boost::optional<cstring>& res )
{
    res = source;

    return true;
}

//____________________________________________________________________________//

// bool overload
inline bool
interpret_argument_value( cstring source, boost::optional<bool>& res )
{
    static literal_cstring YES( "YES" );
    static literal_cstring Y( "Y" );
    static literal_cstring NO( "NO" );
    static literal_cstring N( "N" );
    static literal_cstring one( "1" );
    static literal_cstring zero( "0" );

    source.trim();

    if( case_ins_eq( source, YES ) || case_ins_eq( source, Y ) || case_ins_eq( source, one ) ) {
        res = true;
        return true;
    }
    else if( case_ins_eq( source, NO ) || case_ins_eq( source, N ) || case_ins_eq( source, zero ) ) {
        res = false;
        return true;
    }
    else {
        res = true;
        return source.is_empty();
    }
}

//____________________________________________________________________________//

// overload for list of values
template<typename T>
inline bool
interpret_argument_value( cstring source, boost::optional<std::vector<T> >& res )
{
    res = std::vector<T>();

    while( !source.is_empty() ) {
        cstring::iterator single_value_end = std::find( source.begin(), source.end(), ',' );

        boost::optional<T> value;
        interpret_argument_value( cstring( source.begin(), single_value_end ), value, 0 );

        res->push_back( *value );

        source.trim_left( single_value_end + 1 );
    }

    return true;
}

//____________________________________________________________________________//

// generic overload
template<typename T>
inline bool
interpret_argument_value( cstring source, boost::optional<T>& res )
{
    res = lexical_cast<T>( source );
    return true;
}

//____________________________________________________________________________//

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_INTERPRET_ARGUMENT_VALUE_HPP
