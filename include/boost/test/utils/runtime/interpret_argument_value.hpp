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
//  Description : argument factories for different kinds of parameters
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_INTERPRET_ARGUMENT_VALUE_HPP
#define BOOST_TEST_UTILS_RUNTIME_INTERPRET_ARGUMENT_VALUE_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/errors.hpp>
#include <boost/test/utils/runtime/argument.hpp>

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
// **************          runtime::value_interpreter          ************** //
// ************************************************************************** //

template<typename ValueType, bool is_enum>
struct value_interpreter;

//____________________________________________________________________________//

template<typename ValueType>
struct value_interpreter<ValueType, false> {
    ValueType interpret( cstring source ) const
    {
        BOOST_TEST_IMPL_TRY{
            return lexical_cast<ValueType>(source);
        } BOOST_TEST_IMPL_CATCH0( bad_lexical_cast ) {
            BOOST_TEST_IMPL_THROW( format_error() << source << " can't be interpreted as parameter type value." );
        }
        return ValueType{};
    }
};

//____________________________________________________________________________//

template<>
struct value_interpreter<std::string, false> {
    std::string interpret( cstring source ) const
    {
        return std::string( source.begin(), source.size() );
    }
};

//____________________________________________________________________________//

template<>
struct value_interpreter<cstring, false> {
    cstring interpret( cstring source ) const
    {
        return source;
    }
};

//____________________________________________________________________________//

template<>
struct value_interpreter<bool, false> {
    bool    interpret( cstring source ) const
    {
        static cstring const YES( "YES" );
        static cstring const Y( "Y" );
        static cstring const NO( "NO" );
        static cstring const N( "N" );
        static cstring const TRUE( "TRUE" );
        static cstring const FALSE( "FALSE" );
        static cstring const one( "1" );
        static cstring const zero( "0" );

        source.trim();

        if( source.is_empty() || case_ins_eq( source, YES ) || case_ins_eq( source, Y ) || case_ins_eq( source, one ) || case_ins_eq( source, TRUE ) )
            return true;

        if( case_ins_eq( source, NO ) || case_ins_eq( source, N ) || case_ins_eq( source, zero ) || case_ins_eq( source, FALSE ) )
            return false;

        BOOST_TEST_IMPL_THROW( format_error() << source << " can't be interpreted as bool value." );
        return false;
    }
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************           runtime::argument_factory          ************** //
// ************************************************************************** //

template<typename ValueType, bool is_enum, bool repeatable>
class argument_factory;

//____________________________________________________________________________//

template<typename ValueType, bool is_enum>
class argument_factory<ValueType, is_enum, false> {
public:
    template<typename Modifiers>
    explicit    argument_factory( Modifiers const& m )
    : m_optional_value()
    , m_default_value()
    {
        nfp::optionally_assign( m_optional_value, m, optional_value );
        nfp::optionally_assign( m_default_value, m, default_value );
    }

    void        produce_argument( cstring source, cstring param_name, arguments_store& store ) const
    {
        store.set( param_name, source.empty() ? m_optional_value : m_interpreter.interpret( source ) );
    }

    void        produce_default( cstring param_name, arguments_store& store ) const
    {
        store.set( param_name, m_default_value );
    }

private:
    // Data members
    typedef value_interpreter<ValueType, is_enum> interp_t;
    interp_t    m_interpreter;
    ValueType   m_optional_value;
    ValueType   m_default_value;
};

//____________________________________________________________________________//

template<typename ValueType, bool is_enum>
class argument_factory<ValueType, is_enum, true> {
public:
    template<typename Modifiers>
    explicit    argument_factory( Modifiers const& )
    {
    }

    void        produce_argument( cstring source, cstring param_name, arguments_store& store ) const
    {
        ValueType value = m_interpreter.interpret( source );

        if( store.has( param_name ) ) {
            std::vector<ValueType>& values = store.get<std::vector<ValueType>>( param_name );
            values.push_back( value );
        }
        else {
            std::vector<ValueType> values( 1, value );

            store.set( param_name, values );
        }

    }
    void        produce_default( cstring param_name, arguments_store& store ) const
    {
        store.set( param_name, std::vector<ValueType>{} );
    }

private:
    value_interpreter<ValueType, is_enum> m_interpreter;
};

//____________________________________________________________________________//

} // namespace runtime
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UTILS_RUNTIME_INTERPRET_ARGUMENT_VALUE_HPP
