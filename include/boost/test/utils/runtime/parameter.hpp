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
#include <boost/test/utils/runtime/argument_factory.hpp>

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
    parameter_cla_id( cstring prefix, cstring full_name, cstring value_separator, bool negatable )
    : m_prefix( prefix.begin(), prefix.size() )
    , m_full_name( full_name.begin(), full_name.size() )
    , m_value_separator( value_separator.begin(), value_separator.size() )
    , m_negatable( negatable )
    {

        BOOST_TEST_I_ASSRT( std::all_of( m_prefix.begin(), m_prefix.end(), valid_prefix_char ),
                            invalid_cla_id() << "Parameter " << m_full_name
                                             << " has invalid characters in prefix." );

        BOOST_TEST_I_ASSRT( std::all_of( m_full_name.begin(), m_full_name.end(), valid_name_char ),
                            invalid_cla_id() << "Parameter " << m_full_name
                                             << " has invalid characters in name." );

        BOOST_TEST_I_ASSRT( std::all_of( m_value_separator.begin(), m_value_separator.end(), valid_separator_char ),
                            invalid_cla_id() << "Parameter " << m_full_name
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
    bool                    m_negatable;
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
    basic_param( cstring name, bool is_optional, bool is_repeatable, Modifiers const& m )
    : p_name( name.begin(), name.end() )
    , p_description( nfp::optionally_get<std::string>( m, description ) )
    , p_optional( is_optional )
    , p_repeatable( is_repeatable )
    , p_has_optional_value( m.has( optional_value ) )
    , p_has_default_value( m.has( default_value ) || is_repeatable )
    {
        nfp::optionally_assign( p_env_var.value, m, env_var );
        nfp::optionally_assign( p_value_hint.value, m, value_hint );
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

        ostr     << " Command line formats:\n";
        BOOST_TEST_FOREACH( parameter_cla_id const&, id, cla_ids() ) {
            ostr << "   " << id.m_prefix;
            if( id.m_negatable )
                cla_name_help( ostr, id.m_full_name, negation_prefix );
            else
                cla_name_help( ostr, id.m_full_name, "" );

            bool optional_value = false;

            if( id.m_value_separator.empty() )
                ostr << " ";
            else {
                ostr << id.m_value_separator;
            }

            if( p_has_optional_value ) {
                optional_value = true;
                ostr << '[';
            }

            value_help( ostr );

            if( optional_value )
                ostr << ']';

            ostr << '\n';
        }
        if( !p_env_var->empty() )
            ostr << " Environment variable: " << p_env_var << '\n';
    }

    virtual void            help( std::ostream& ostr, cstring negation_prefix )
    {
        usage( ostr, negation_prefix );
    }

    // Pubic properties
    std::string const       p_name;
    std::string const       p_description;
    string_property         p_env_var;
    string_property         p_value_hint;
    bool const              p_optional;
    bool const              p_repeatable;
    bool_property           p_has_optional_value;
    bool_property           p_has_default_value;

    // Access methods
    parameter_cla_ids const& cla_ids() const { return m_cla_ids; }
    void                    add_cla_id( cstring prefix, cstring full_name, cstring value_separator )
    {
        add_cla_id_impl( prefix, full_name, value_separator, false, true );
    }

    /// interface for producing argument values for this parameter
    virtual void            produce_argument( cstring token, bool negative_form, arguments_store& store ) const = 0;
    virtual void            produce_default( arguments_store& store ) const = 0;

protected:
    void                    add_cla_id_impl( cstring prefix,
                                             cstring full_name,
                                             cstring value_separator,
                                             bool negatable,
                                             bool validate_value_separator )
    {
        BOOST_TEST_I_ASSRT( !full_name.is_empty(),
                            invalid_cla_id() << "Parameter can't have an empty name." );

        BOOST_TEST_I_ASSRT( !prefix.is_empty(),
                            invalid_cla_id() << "Parameter " << full_name
                                             << " can't have an empty prefix." );

        BOOST_TEST_I_ASSRT( !value_separator.is_empty(),
                            invalid_cla_id() << "Parameter " << full_name
                                             << " can't have an empty value separator." );

        // We trim value separator from all the spaces, so token end will indicate separator
        value_separator.trim();
        BOOST_TEST_I_ASSRT( !validate_value_separator || !value_separator.is_empty() || !p_has_optional_value,
                            invalid_cla_id() << "Parameter " << full_name
                                             << " with optional value attribute can't use space as value separator." );

        m_cla_ids.push_back( parameter_cla_id( prefix, full_name, value_separator, negatable ) );
    }

private:
    /// interface for usage/help customization
    virtual void            cla_name_help( std::ostream& ostr, cstring cla_full_name, cstring negation_prefix )
    {
        ostr << cla_full_name;
    }
    virtual void            value_help( std::ostream& ostr )
    {
        if( p_value_hint->empty() )
            ostr << "<value>";
        else
            ostr << p_value_hint;
    }

    // Data members
    parameter_cla_ids       m_cla_ids;
};

// ************************************************************************** //
// **************              runtime::parameter              ************** //
// ************************************************************************** //

enum args_amount {
    OPTIONAL_PARAM,   // 0-1
    REQUIRED_PARAM,   // exactly 1
    REPEATABLE_PARAM  // 0-N
};

//____________________________________________________________________________//

template<typename ValueType, args_amount a = runtime::OPTIONAL_PARAM, bool is_enum = false>
class parameter : public basic_param {
public:
    /// Constructor with modifiers
    template<typename Modifiers=nfp::no_params_type>
    parameter( cstring name, Modifiers const& m = nfp::no_params )
    : basic_param( name, a != runtime::REQUIRED_PARAM, a == runtime::REPEATABLE_PARAM, m )
    , m_arg_factory( m )
    {
        BOOST_TEST_I_ASSRT( !m.has( default_value ) || a == runtime::OPTIONAL_PARAM,
                            invalid_param_spec() << "Parameter " << name
                                                 << " is not optional and can't have default_value." );

        BOOST_TEST_I_ASSRT( !m.has( optional_value ) || !this->p_repeatable,
                            invalid_param_spec() << "Parameter " << name
                                                 << " is repeatable and can't have optional_value." );
    }

private:
    virtual void    produce_argument( cstring token, bool , arguments_store& store ) const
    {
        m_arg_factory.produce_argument( token, this->p_name, store );
    }

    virtual void    produce_default( arguments_store& store ) const
    {
        if( !this->p_has_default_value )
            return;

        m_arg_factory.produce_default( this->p_name, store );
    }

    virtual basic_param_ptr clone() const
    {
        return basic_param_ptr( new parameter( *this ) );
    }

    // Data members
    typedef argument_factory<ValueType, is_enum, a == runtime::REPEATABLE_PARAM> factory_t;
    factory_t       m_arg_factory;
};

template<typename ValueType, args_amount a = runtime::OPTIONAL_PARAM>
using enum_parameter = parameter<ValueType, a, true>;

//____________________________________________________________________________//

class option : public basic_param {
public:
    /// Constructor with modifiers
    template<typename Modifiers=nfp::no_params_type>
    option( cstring name, Modifiers const& m = nfp::no_params )
    : basic_param( name, true, false, (m, optional_value = true, default_value = false) )
    , m_arg_factory(( m, optional_value = true, default_value = false ))
    {
    }

    void            add_cla_id( cstring prefix, cstring full_name, cstring value_separator, bool negatable = false )
    {
        add_cla_id_impl( prefix, full_name, value_separator, negatable, false );
    }

private:
    virtual basic_param_ptr clone() const
    {
        return basic_param_ptr( new option( *this ) );
    }

    virtual void    produce_argument( cstring token, bool negative_form, arguments_store& store ) const
    {
        if( token.empty() )
            store.set( p_name, !negative_form );
        else {
            BOOST_TEST_I_ASSRT( !negative_form,
                                format_error() << "Can't set value to negative form of the argument." );

            m_arg_factory.produce_argument( p_name, token, store );
        }
    }

    virtual void    produce_default( arguments_store& store ) const
    {
        m_arg_factory.produce_default( p_name, store );
    }
    virtual void    cla_name_help( std::ostream& ostr, cstring cla_full_name, cstring negation_prefix )
    {
        if( negation_prefix.is_empty() )
            ostr << cla_full_name;
        else
            ostr << '[' << negation_prefix << ']' << cla_full_name;
    }
    virtual void    value_help( std::ostream& ostr )
    {
        if( p_value_hint->empty() )
            ostr << "<yes/no value>";
        else
            ostr << p_value_hint;
    }

    // Data members
    typedef argument_factory<bool, false, false> factory_t;
    factory_t       m_arg_factory;
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************           runtime::parameters_store          ************** //
// ************************************************************************** //

class parameters_store {
    struct lg_compare {
        bool operator()( cstring lh, cstring rh ) const
        {
            return std::lexicographical_compare(lh.begin(), lh.end(),
                                                rh.begin(), rh.end());
        }
    };
public:

    typedef std::map<cstring, basic_param_ptr, lg_compare> storage_type;

    /// Adds parameter into the persistent store
    void                    add( basic_param const& in )
    {
        basic_param_ptr p = in.clone();

        BOOST_TEST_I_ASSRT( m_parameters.insert( std::make_pair( cstring(p->p_name), p ) ).second,
                            duplicate_param() << "Parameter " << p->p_name << " is duplicate." );
    }

    /// Returns true if there is no parameters registered
    bool                    is_empty() const    { return m_parameters.empty(); }
    /// Returns map of all the registered parameter
    storage_type const&     all() const         { return m_parameters; }
    /// Returns true if parameter with psecified name is registered
    bool                    has( cstring name ) const
    {
        return m_parameters.find( name ) != m_parameters.end();
    }
    /// Returns map of all the registered parameter
    basic_param_ptr         get( cstring name ) const
    {
        auto const& found = m_parameters.find( name );
        BOOST_TEST_I_ASSRT( found != m_parameters.end(),
                            unknown_param() << "Parameter " << name << " is unknown." );

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
