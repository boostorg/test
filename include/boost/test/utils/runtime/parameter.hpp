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
#include <boost/test/utils/runtime/fwd.hpp>
#include <boost/test/utils/runtime/modifier.hpp>
#include <boost/test/utils/runtime/argument.hpp>
#include <boost/test/utils/runtime/interpret_argument_value.hpp>

// Boost.Test
#include <boost/test/utils/class_properties.hpp>
#include <boost/test/utils/foreach.hpp>

// STL
#include <algorithm>

#include <boost/test/detail/suppress_warnings.hpp>

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

        if( !std::all_of( m_prefix.begin(), m_prefix.end(), valid_prefix_char ) )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << m_full_name
                                                    << " has invalid characters in prefix." );

        if( !std::all_of( m_full_name.begin(), m_full_name.end(), valid_name_char ) )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << m_full_name 
                                                    << " has invalid characters in name." );

        if( !std::all_of( m_value_separator.begin(), m_value_separator.end(), valid_separator_char ) )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << m_full_name
                                                    << " has invalid characters in value separator." );
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
    : p_name( name.begin(), name.end() )
    , p_description( nfp::optionally_get<std::string>( m, description ) )
    , p_optional( is_optional )
    , p_repeatable( false )
    , p_has_optional_value( false )
    , p_has_default_value( false )
    {
        nfp::optionally_assign( p_env_var.value, m, env_var );
    }

public:
    virtual                 ~basic_param() {}

    /// interface for cloning typed parameters
    virtual basic_param_ptr clone() const = 0;

    /// interfaces for help message reporting
    virtual void            usage( std::ostream& ostr, cstring negation_prefix )
    {
        ostr     << "Parameter: " << p_name << "\n";
        if( !p_description.empty() )
            ostr << ' ' << p_description << "\n";
        ostr     << "  Command line formats:\n";
        BOOST_TEST_FOREACH( parameter_cla_id const&, id, cla_ids() ) {
            ostr << "   " << id.m_prefix;
            cla_name_help( ostr, id.m_full_name, negation_prefix );

            bool optional_value = false;

            if( id.m_value_separator.empty() )
                ostr << " ";
            else {
                if( p_has_optional_value ) {
                    optional_value = true;
                    ostr << '[';
                }

                ostr << id.m_value_separator;
            }

            value_help( ostr );

            if( optional_value )
                ostr << ']';

            ostr << '\n';
        }
        if( !p_env_var->empty() )
            ostr << "  Environment variable: " << p_env_var << '\n';
    }

    virtual void            help( std::ostream& ostr, cstring negation_prefix ) {}

    // Pubic properties
    std::string const       p_name;
    std::string const       p_description;
    string_property         p_env_var;
    bool const              p_optional;
    bool_property           p_repeatable;
    bool_property           p_has_optional_value;
    bool_property           p_has_default_value;

    // Access methods
    parameter_cla_ids const& cla_ids() const { return m_cla_ids; }
    void                    add_cla_id( cstring prefix, cstring full_name, cstring value_separator )
    {
        add_cla_id_impl( prefix, full_name, value_separator, true );
    }
    
    /// interface for producing argument values for this parameter
    virtual void            produce_argument( cstring token, bool negative_form, arguments_store& store ) const = 0;
    virtual void            set_default( arguments_store& store ) const = 0;

protected:
    void                    add_cla_id_impl( cstring prefix, 
                                             cstring full_name, 
                                             cstring value_separator,
                                             bool validate_value_separator )
    {
        if( full_name.is_empty() )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter can't have an empty name." );
        if( prefix.is_empty() )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << full_name << " can't have an empty prefix." );
        if( value_separator.is_empty() )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << full_name << " can't have an empty value separator." );

        // We trim value separaotr from all the spaces, so token end will indicate separator
        value_separator.trim();
        if( validate_value_separator && value_separator.is_empty() && !!p_has_optional_value )
            BOOST_TEST_IMPL_THROW( invalid_cla_id() << "Parameter " << full_name << " with optional value attribute can't use space as value separator." );

        m_cla_ids.push_back( parameter_cla_id( prefix, full_name, value_separator ) );
    }

private:
    /// interface for usage/help customization
    virtual void            cla_name_help( std::ostream& ostr, cstring cla_full_name, cstring negation_prefix )
    {
        ostr << cla_full_name;
    }
    virtual void            value_help( std::ostream& ostr ) = 0;

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
        if( m.has( optional_value ) ) {
            p_has_optional_value.value = true;
            m_optional_value = (ValueType)(m[optional_value]);
        }

        if( m.has( default_value ) ) {
            p_has_default_value.value = true;
            m_default_value = (ValueType)(m[default_value]);
        }
    }

    virtual basic_param_ptr clone() const
    {
        return basic_param_ptr( new Derived( *static_cast<Derived const*>(this) ) );
    }

    virtual void            set_default( arguments_store& store ) const {
        if( !p_has_default_value )
            return;

        store.set( p_name, m_default_value );
    }

    // Data members
    ValueType   m_optional_value;
    ValueType   m_default_value;

private:
    void                    value_help( std::ostream& ostr )
    {
        ostr << "<value>";
    }
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
        if( m.has( default_value ) && a == runtime::REQUIRED )
            BOOST_TEST_IMPL_THROW( invalid_param_spec() << "Parameter " << name << " is required and can't have default_value." );
    }

    virtual void    produce_argument( cstring token, bool negative_form, arguments_store& store ) const
    {
        if( negative_form )
            BOOST_TEST_IMPL_THROW( format_error() << "Parameter " << p_name
                                                  << " does not support negative form." );

        ValueType value = m_optional_value;

        if( !token.empty() || !p_has_optional_value )
            interpret_argument_value( token, value );

        store.set( p_name, value );
    }
};

//____________________________________________________________________________//

template<typename ValueType>
class parameter<ValueType, runtime::REPEATABLE> : public typed_param<std::vector<ValueType>, parameter<ValueType, runtime::REPEATABLE>> {
    typedef typed_param<std::vector<ValueType>, parameter<ValueType, runtime::REPEATABLE>> base;
public:
    /// Constructor with modifiers
    template<typename Modifiers=nfp::no_params_type>
    parameter( cstring name, Modifiers const& m = nfp::no_params )
    : base( name, true, (m, default_value = std::vector<ValueType>{} ) )
    {
        p_repeatable.value = true;

        if( m.has( default_value ) )
            BOOST_TEST_IMPL_THROW( invalid_param_spec() << "Parameter " << p_name 
                                                        << " is repeatable with a default value of empty list." );
    }

    virtual void    produce_argument( cstring token, bool negative_form, arguments_store& store ) const
    {
        if( negative_form )
            BOOST_TEST_IMPL_THROW( format_error() << "Parameter " << p_name
                                                  << " does not support negative form." );

        ValueType value;
        interpret_argument_value( token, value );

        if( store.has( p_name ) ) {
            std::vector<ValueType>& values = store.get<std::vector<ValueType> >( p_name );
            values.push_back( value );
        }
        else {
            std::vector<ValueType> values( 1, value );

            store.set( p_name, values );
        }
    }
};

//____________________________________________________________________________//

class option : public typed_param<bool, option> {
    typedef typed_param<bool, option> base;
public:
    /// Constructor with modifiers
    template<typename Modifiers=nfp::no_params_type>
    option( cstring name, Modifiers const& m = nfp::no_params )
    : base( name, true, (m, optional_value=true) )
    {
        p_repeatable.value = false;

        if( !m.has( default_value ) ) {
            p_has_default_value.value = true;
            m_default_value = false;
        }
    }

    void            add_cla_id( cstring prefix, cstring full_name, cstring value_separator )
    {
        add_cla_id_impl( prefix, full_name, value_separator, false );
    }

    virtual void    produce_argument( cstring token, bool negative_form, arguments_store& store ) const
    {
        bool value = !negative_form;

        if( !token.empty() ) {
            if( negative_form )
                BOOST_TEST_IMPL_THROW( format_error() << "Negative form of the argument does not allow value beeing set." );
            
            interpret_argument_value( token, value );
        }

        store.set( p_name, value );
    }
private:
    virtual void            cla_name_help( std::ostream& ostr, cstring cla_full_name, cstring negation_prefix )
    {
        if( negation_prefix.is_empty() )
            ostr << cla_full_name;
        else
            ostr << '[' << negation_prefix << ']' << cla_full_name;
    }
    virtual void            value_help( std::ostream& ostr )
    {
        ostr << "<Y|YES|1|N|NO|0>";
    }

};

//____________________________________________________________________________//

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

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UTILS_RUNTIME_PARAMETER_HPP
