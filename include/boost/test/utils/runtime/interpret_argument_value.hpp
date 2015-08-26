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
#include <boost/test/utils/runtime/errors.hpp>

// Boost.Test
#include <boost/test/utils/basic_cstring/io.hpp>
#include <boost/test/utils/basic_cstring/compare.hpp>

// Boost
#include <boost/optional.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <vector>

#include <boost/test/detail/suppress_warnings.hpp>

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
    typedef unit_test::literal_string   literal_cstring;

    static literal_cstring YES( "YES" );
    static literal_cstring Y( "Y" );
    static literal_cstring NO( "NO" );
    static literal_cstring N( "N" );
    static literal_cstring TRUE( "TRUE" );
    static literal_cstring FALSE( "FALSE" );
    static literal_cstring one( "1" );
    static literal_cstring zero( "0" );

    source.trim();

    if( source.is_empty() || case_ins_eq( source, YES ) || case_ins_eq( source, Y ) || case_ins_eq( source, one ) || case_ins_eq( source, TRUE ) )
        res = true;
    else if( case_ins_eq( source, NO ) || case_ins_eq( source, N ) || case_ins_eq( source, zero ) || case_ins_eq( source, FALSE ) )
        res = false;
    else
        BOOST_TEST_IMPL_THROW( format_error() << source << " can't be interpreted as bool value." );
}

//____________________________________________________________________________//

// generic overload
template<typename T>
inline void
interpret_argument_value( cstring source, T& res )
{
    BOOST_TEST_IMPL_TRY {
        res = lexical_cast<T>( source );
    }
    BOOST_TEST_IMPL_CATCH0( bad_lexical_cast ) {
        BOOST_TEST_IMPL_THROW( format_error() << source << " can't be interpreted as parameter type value." );
    }
}

//____________________________________________________________________________//

} // namespace runtime
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UTILS_RUNTIME_INTERPRET_ARGUMENT_VALUE_HPP
