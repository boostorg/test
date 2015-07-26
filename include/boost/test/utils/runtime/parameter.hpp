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
#include <boost/test/utils/runtime/setup_error.hpp>

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
    parameter_cla_id( cstring prefix, cstring full_name, cstring value_separator )
    : m_prefix( prefix.begin(), prefix.size() )
    , m_full_name( full_name.begin(), full_name.size() )
    , m_value_separator( value_separator.begin(), value_separator.size() )
    {
        if( m_full_name.empty() )
            BOOST_TEST_IMPL_THROW( setup_error() << "Parameter can't have an empty prefix." );
        if( m_prefix.empty() )
            BOOST_TEST_IMPL_THROW( setup_error() << "Parameter " << m_full_name << " can't have an empty name." );
        if( m_value_separator.empty() )
            BOOST_TEST_IMPL_THROW( setup_error() << "Parameter " << m_full_name << " can't have an empty value separator." );

        if( !std::all_of( m_prefix.begin(), m_prefix.end(), valid_prefix_char ) )
            BOOST_TEST_IMPL_THROW( setup_error() << "Parameter " << m_full_name << " has invalid characters in prefix." );
        if( !std::all_of( m_full_name.begin(), m_full_name.end(), valid_name_char ) )
            BOOST_TEST_IMPL_THROW( setup_error() << "Parameter " << m_full_name << " has invalid characters in name." );
        if( !std::all_of( m_value_separator.begin(), m_value_separator.end(), valid_separator_char ) )
            BOOST_TEST_IMPL_THROW( setup_error() << "Parameter " << m_full_name << " has invalid characters in value separator." );
    }

    static bool             valid_prefix_char( char c )
    {
        return c == '-' || c == '/' ;
    }
    static bool             valid_separator_char( char c )
    {
        return c == '=' || c == ':' || c == ' ';
    }
    static bool             valid_name_char( char c )
    {
        return std::isalnum( c ) || c == '+' || c == '_' || c == '?';
    }

    std::string m_prefix;
    std::string m_full_name;
    std::string m_value_separator;
};

typedef std::vector<parameter_cla_id> parameter_cla_ids;

// ************************************************************************** //
// **************             runtime::basic_param             ************** //
// ************************************************************************** //

class basic_param {
    typedef unit_test::readwrite_property<std::string>  string_property;
    typedef unit_test::readwrite_property<bool>         bool_property;

public:
    /// Constructor with modifiers
    template<typename Modifiers>
    basic_param( cstring name, Modifiers const& m )
    : p_name( std::string(name.begin(), name.end()) )
    , p_optional( true )
    , p_multiplicable( false )
    , p_optional_value( false )
    {
        nfp::optionally_assign( p_description.value, m, description );
        nfp::optionally_assign( p_env_var.value, m, env_var );

        if( m.has( optional ) )
            p_optional.value = true;

        if( m.has( required ) )
            p_optional.value = false;

        if( m.has( multiplicable ) )
            p_multiplicable.value = true;

        if( m.has( optional_value ) )
            p_optional_value.value = true;
    }
    virtual                 ~basic_param() {}

    // interface for cloning typed parameters
    virtual basic_param_ptr clone() const = 0;

    // Pubic properties
    string_property         p_name;
    string_property         p_description;
    string_property         p_env_var;
    bool_property           p_optional;
    bool_property           p_multiplicable;
    bool_property           p_optional_value;

    // Access methods
    parameter_cla_ids const& cla_ids() const { return m_cla_ids; }
    void                    add_cla_id( cstring prefix, cstring full_name, cstring value_separator )
    {
        m_cla_ids.push_back( parameter_cla_id( prefix, full_name, value_separator ) );
    }

private:
    // Data members
    parameter_cla_ids       m_cla_ids;
};

// ************************************************************************** //
// **************              runtime::parameter              ************** //
// ************************************************************************** //

template<typename ValueType>
class parameter : public basic_param {
public:
    /// Constructor with modifiers
    template<typename Modifiers=nfp::no_params_type>
    parameter( cstring name, Modifiers const& m = nfp::no_params )
    : basic_param( name, m )
    {}

    virtual basic_param_ptr clone() const
    {
        return basic_param_ptr( new parameter<ValueType>( *this ) );
    }
};

// ************************************************************************** //
// **************           runtime::parameters_store          ************** //
// ************************************************************************** //

class parameters_store {
public:
    typedef std::map<std::string, basic_param_ptr> storage_type;

    /// Adds parameter into the persistent store
    void                    add( basic_param const& in )
    {
        basic_param_ptr p = in.clone();

        if( !m_parameters.insert( std::make_pair( p->p_name, p ) ).second )
            BOOST_TEST_IMPL_THROW( setup_error() << "Parameter " << p->p_name << " is duplicate." );
    }

    /// Returns true if there is no parameters registered
    bool                    is_empty() const    { return m_parameters.empty(); }
    /// Returns map of all the registered parameter
    storage_type const&     all() const         { return m_parameters; }
    /// Returns map of all the registered parameter
    basic_param_ptr         get( cstring name ) const { return m_parameters.at( std::string( name.begin(), name.size() ) ); }

private:
    // Data members
    storage_type            m_parameters;
};

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_PARAMETER_HPP
