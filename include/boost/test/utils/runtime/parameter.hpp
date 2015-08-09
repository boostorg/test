//  (C) Copyright Gennadiy Rozental 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : formal parameter definition
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_PARAMETER_HPP
#define BOOST_TEST_UTILS_RUNTIME_PARAMETER_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>
#include <boost/test/utils/runtime/fwd.hpp>
#include <boost/test/utils/runtime/modifier.hpp>
#include <boost/test/utils/runtime/argument.hpp>
#include <boost/test/utils/runtime/interpret_argument_value.hpp>

// Boost.Test
#include <boost/test/utils/class_properties.hpp>

// STL
#include <algorithm>

namespace boost {
namespace runtime {

// ************************************************************************** //
// **************           runtime::parameter_cla_id          ************** //
// ************************************************************************** //
// set of attributes identifying the parameter in the command line

struct parameter_cla_id {
    parameter_cla_id( basic_param const& owner, cstring prefix, cstring full_name, cstring value_separator )
    : m_owner( owner )
    , m_prefix( prefix.begin(), prefix.size() )
    , m_full_name( full_name.begin(), full_name.size() )
    , m_value_separator( value_separator.begin(), value_separator.size() )
    {
        
        if( !std::all_of( m_prefix.begin(), m_prefix.end(), valid_prefix_char ) )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << m_full_name << " has invalid characters in prefix." );
        if( !std::all_of( m_full_name.begin(), m_full_name.end(), valid_name_char ) )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << m_full_name << " has invalid characters in name." );
        if( !std::all_of( m_value_separator.begin(), m_value_separator.end(), valid_separator_char ) )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << m_full_name << " has invalid characters in value separator." );
    }

    static bool             valid_prefix_char( char c )
    {
        return c == '-' || c == '/' ;
    }
    static bool             valid_separator_char( char c )
    {
        return c == '=' || c == ':' || c == ' ' || c == '\0';
    }
    static bool             valid_name_char( char c )
    {
        return std::isalnum( c ) || c == '+' || c == '_' || c == '?';
    }

    basic_param const&      m_owner;
    std::string             m_prefix;
    std::string             m_full_name;
    std::string             m_value_separator;
};

typedef std::vector<parameter_cla_id> parameter_cla_ids;

// ************************************************************************** //
// **************             runtime::basic_param             ************** //
// ************************************************************************** //

class basic_param {
    typedef unit_test::readwrite_property<std::string>  string_property;
    typedef unit_test::readwrite_property<bool>         bool_property;
protected:
    /// Constructor with modifiers
    template<typename Modifiers>
    basic_param( cstring name, bool is_optional, Modifiers const& m )
    : p_name( std::string(name.begin(), name.end()) )
    , p_optional( is_optional )
    , p_repeatable( false )
    , p_has_optional_value( false )
    {
        nfp::optionally_assign( p_description.value, m, description );
        nfp::optionally_assign( p_env_var.value, m, env_var );

        if( m.has( optional_value ) )
            p_has_optional_value.value = true;
    }

public:
    virtual                 ~basic_param() {}

    /// interface for cloning typed parameters
    virtual basic_param_ptr clone() const = 0;

    // Pubic properties
    string_property         p_name;
    string_property         p_description;
    string_property         p_env_var;
    bool_property           p_optional;
    bool_property           p_repeatable;
    bool_property           p_has_optional_value;

    // Access methods
    parameter_cla_ids const& cla_ids() const { return m_cla_ids; }
    void                    add_cla_id( cstring prefix, cstring full_name, cstring value_separator )
    {
        if( full_name.is_empty() )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter can't have an empty name." );
        if( prefix.is_empty() )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << full_name << " can't have an empty prefix." );
        if( value_separator.is_empty() )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << full_name << " can't have an empty value separator." );

        value_separator.trim();
        if( value_separator.is_empty() && !!p_has_optional_value )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << full_name << " with optional value attribute can't use space as value separator." );

        // We trim value separaotr from all the spaces, so token end will indicate separator
        m_cla_ids.push_back( parameter_cla_id( *this, prefix, full_name, value_separator ) );
    }

    /// interface for producing argument values for this parameter
    virtual void produce_argument( cstring token, arguments_store& store ) const = 0;

private:
    // Data members
    parameter_cla_ids       m_cla_ids;
};

// ************************************************************************** //
// **************             runtime::typed_param             ************** //
// ************************************************************************** //

template<typename ValueType, typename Derived>
class typed_param : public basic_param {
protected:
    /// Constructor with modifiers
    template<typename Modifiers>
    typed_param( cstring name, bool is_optional, Modifiers const& m )
    : basic_param( name, is_optional, m )
    {
        if( m.has( optional_value ) )
            m_optional_value = (ValueType)(m[optional_value]);
    }

    virtual basic_param_ptr clone() const
    {
        return basic_param_ptr( new Derived( *static_cast<Derived const*>(this) ) );
    }

    // Data members
    ValueType   m_optional_value;    
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************              runtime::parameter              ************** //
// ************************************************************************** //

enum args_amount { 
    REQUIRED,   // exactly 1
    OPTIONAL,   // 0-1
    REPEATABLE  // 0-N
};

//____________________________________________________________________________//

template<typename ValueType, args_amount a = runtime::OPTIONAL>
class parameter : public typed_param<ValueType, parameter<ValueType, a>> {
    typedef typed_param<ValueType, parameter<ValueType, a>> base;
public:
    /// Constructor with modifiers
    template<typename Modifiers=nfp::no_params_type>
    parameter( cstring name, Modifiers const& m = nfp::no_params )
    : base( name, a != runtime::REQUIRED, m )
    {
    }

    virtual void produce_argument( cstring token, arguments_store& store ) const
    {
        if( token.empty() && p_has_optional_value ) {
            store.set( p_name.get(), m_optional_value );
        }
        else {
            ValueType value;
            interpret_argument_value( token, value );

            store.set( p_name.get(), value );
        }
    }
};

//____________________________________________________________________________//

template<typename ValueType>
class parameter<ValueType, runtime::REPEATABLE> : public typed_param<ValueType, parameter<ValueType, runtime::REPEATABLE>> {
    typedef typed_param<ValueType, parameter<ValueType, runtime::REPEATABLE>> base;

public:
    /// Constructor with modifiers
    template<typename Modifiers=nfp::no_params_type>
    parameter( cstring name, Modifiers const& m = nfp::no_params )
    : base( name, true, m )
    {
        p_repeatable.value = true;
    }

    virtual void produce_argument( cstring token, arguments_store& store ) const
    {
        ValueType value;
        interpret_argument_value( token, value );

        if( store.has( p_name.get() ) ) {
            std::vector<ValueType>& values = store.get<std::vector<ValueType> >( p_name.get() );
            values.push_back( value );
        }
        else {
            std::vector<ValueType> values( 1, value );
            
            store.set( p_name.get(), values );
        }
    }
};

// ************************************************************************** //
// **************           runtime::parameters_store          ************** //
// ************************************************************************** //

class parameters_store {
public:
    typedef std::map<cstring, basic_param_ptr> storage_type;

    /// Adds parameter into the persistent store
    void                    add( basic_param const& in )
    {
        basic_param_ptr p = in.clone();

        if( !m_parameters.insert( std::make_pair( cstring(p->p_name), p ) ).second )
            BOOST_TEST_IMPL_THROW( duplicate_param() << "Parameter " << p->p_name << " is duplicate." );
    }

    /// Returns true if there is no parameters registered
    bool                    is_empty() const    { return m_parameters.empty(); }
    /// Returns map of all the registered parameter
    storage_type const&     all() const         { return m_parameters; }
    /// Returns map of all the registered parameter
    basic_param_ptr         get( cstring name ) const
    {
        auto found = m_parameters.find( name );
        if( found == m_parameters.end() )
            BOOST_TEST_IMPL_THROW( unknown_param() << "Parameter " << name << " is unknown." );

        return found->second;
    }

private:
    // Data members
    storage_type            m_parameters;
};

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_PARAMETER_HPP
