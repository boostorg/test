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
//  Description : model of actual argument (both typed and abstract interface)
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_ARGUMENT_HPP
#define BOOST_TEST_UTILS_RUNTIME_ARGUMENT_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>
#include <boost/test/utils/runtime/fwd.hpp>

// Boost.Test
#include <boost/test/utils/class_properties.hpp>
#include <boost/test/utils/rtti.hpp>

// STL
#include <cassert>

namespace boost {
namespace runtime {

// ************************************************************************** //
// **************              runtime::argument               ************** //
// ************************************************************************** //

class argument {
public:
    // Constructor
    argument( basic_param const& p, rtti::id_t value_type )
    : p_formal_parameter( p )
    , p_value_type( value_type )
    {}

    // Destructor
    virtual     ~argument()  {}

    // Public properties
    unit_test::readonly_property<basic_param const&> p_formal_parameter;
    unit_test::readonly_property<rtti::id_t>         p_value_type;
};

// ************************************************************************** //
// **************             runtime::typed_argument          ************** //
// ************************************************************************** //

template<typename T>
class typed_argument : public argument {
public:
    // Constructor
    explicit typed_argument( T const& v )
    : argument( rtti::type_id<T>() )
    , p_value( t )
    {}

    unit_test::readwrite_property<T>    p_value;
};

// ************************************************************************** //
// **************               runtime::arg_value             ************** //
// ************************************************************************** //

template<typename T>
inline T const&
arg_value( argument const& arg_ )
{
    assert( arg_.p_value_type == rtti::type_id<T>() ); // detect logic error

    return static_cast<typed_argument<T> const&>( arg_ ).p_value.value;
}

//____________________________________________________________________________//

template<typename T>
inline T&
arg_value( argument& arg_ )
{
    assert( arg_.p_value_type == rtti::type_id<T>() ); // detect logic error

    return static_cast<typed_argument<T>&>( arg_ ).p_value.value;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************           runtime::arguments_store          ************** //
// ************************************************************************** //

class arguments_store {
public:
    typedef std::map<std::string, const_argument_ptr> storage_type;

    /// Returns true if store is empty
    bool        is_empty() const    { return m_arguments.empty(); }

    /// Clears the store for reuse
    void        clear()             { m_arguments.clear(); }

    /// Returns true if there is an argument corresponding to the specified parameter name
    bool        has( std::string const& parameter_name ) const
    {
        return m_arguments.find( parameter_name ) != m_arguments.end();
    }

    /// Provides types access to argument value by parameter name
    template<typename T>
    T const&    get( std::string const& parameter_name ) const;

    /// Set's the argument value for specified parameter name
    template<typename T>
    void        set( std::string const& parameter_name, T const& value ) const;

private:
    // Data members
    storage_type            m_arguments;
};

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_ARGUMENT_HPP
