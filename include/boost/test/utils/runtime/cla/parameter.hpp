//  (C) Copyright Gennadiy Rozental 2005-2015.
//  Use, modification, and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines model of formal parameter
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_CLA_PARAMETER_HPP
#define BOOST_TEST_UTILS_RUNTIME_CLA_PARAMETER_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>

#include <boost/test/utils/runtime/fwd.hpp>
#include <boost/test/utils/runtime/parameter.hpp>
#include <boost/test/utils/runtime/validation.hpp>

#include <boost/test/utils/runtime/cla/fwd.hpp>
#include <boost/test/utils/runtime/cla/modifier.hpp>

// Boost.Test
#include <boost/test/utils/rtti.hpp>

namespace boost {
namespace runtime {
namespace cla {

// ************************************************************************** //
// **************          runtime::cla::parameter_id          ************** //
// ************************************************************************** //
// set of attrbuted identifying the parameter in the command line

struct parameter_id {
    parameter_id( cstring prefix, cstring full_name, cstring value_separator )
    : m_prefix( prefix.begin(), prefix.size() )
    , m_full_name( full_name.begin(), full_name.size() )
    , m_value_separator( value_separator.begin(), value_separator.size() )
    {}

    std::string m_prefix;
    std::string m_full_name;
    std::string m_value_separator;
};

typedef std::vector<parameter_id> parameter_ids;

// ************************************************************************** //
// **************         runtime::cla::basic_param        ************** //
// ************************************************************************** //

class basic_param : public runtime::parameter {
public:
    template<typename Params>
    basic_param( cstring name, Params const& params )
    : p_name( name.begin(), name.end() )
    , p_optional( true )
    , p_multiplicable( false )
    , p_optional_value( false )
    {
        nfp::optionally_assign( p_description.value, params, description );

        if( params.has( optional_m ) )
            p_optional.value = true;

        if( params.has( required_m ) )
            p_optional.value = false;

        if( params.has( multiplicable_m ) )
            p_multiplicable.value = true;

        if( params.has( optional_value_m ) )
            p_optional_value.value = true;
    }
    virtual                 ~basic_param() {}
    virtual basic_param_ptr clone() const = 0;

    // Pubic properties
    unit_test::readwrite_property<std::string>  p_name;
    unit_test::readwrite_property<std::string>  p_description;
    unit_test::readwrite_property<bool>         p_optional;
    unit_test::readwrite_property<bool>         p_multiplicable;
    unit_test::readwrite_property<bool>         p_optional_value;

    // Access methods
    parameter_ids const&    ids() const { return m_ids; }
    void                    add_id( cstring prefix, cstring full_name, cstring value_separator )
    {
        m_ids.push_back( parameter_id( prefix, full_name, value_separator ) );
    }

private:
    // Data members
    parameter_ids           m_ids;
};

// ************************************************************************** //
// **************            runtime::cla::parameter           ************** //
// ************************************************************************** //

template<typename ValueType>
class parameter : public basic_param {
public:
    template<typename Params>
    parameter( std::string name, Params const& params )
    : basic_param( name, params )
    {
    }
    virtual basic_param_ptr clone() const
    {
        return basic_param_ptr( new parameter<ValueType>( *this ) );
    }
};

//____________________________________________________________________________//


} // namespace cla
} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_CLA_PARAMETER_HPP
