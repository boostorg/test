//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : enhanced result for test predicate that include message explaining failure
// ***************************************************************************

#ifndef BOOST_TEST_PREDICATE_RESULT_HPP_012705GER
#define BOOST_TEST_PREDICATE_RESULT_HPP_012705GER

// Boost.Test
#include <boost/test/utils/class_properties.hpp>
#include <boost/test/utils/wrap_stringstream.hpp>

// BOOST
#include <boost/shared_ptr.hpp>

// STL
#include <cstddef>          // for std::size_t

#include <boost/test/detail/suppress_warnings.hpp>

namespace boost {

namespace test_tools {

// ************************************************************************** //
// **************                predicate_result              ************** //
// ************************************************************************** //

class predicate_result {
    typedef boost::unit_test::const_string      const_string;
public:
    // Constructor
    predicate_result( bool pv_ ) : p_predicate_value( pv_ ) {}
    template<typename BoolConvertable>
    predicate_result( BoolConvertable const& pv_ ) : p_predicate_value( !!pv_ ) {}

    bool                operator!() const           { return !p_predicate_value; }
    void                operator=( bool pv_ )       { p_predicate_value.value = pv_; }

    // Public properties
    BOOST_READONLY_PROPERTY( bool, (predicate_result) ) p_predicate_value;

    // Access methods
    bool                has_empty_message() const   { return !m_message; }
    wrap_stringstream&  message()
    {
        if( !m_message )
            m_message.reset( new wrap_stringstream );

        return *m_message;
    }
    const_string        message() const                   { return !m_message ? const_string() : const_string( m_message->str() ); }

private:
    // Data members
    boost::shared_ptr<wrap_stringstream> m_message;
};

} // namespace test_tools

} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.1  2005/01/30 03:24:51  rogeeff
//  extended_predicate_result renamed ot predicate_result and moved into separate file
//
// ***************************************************************************

#endif // BOOST_TEST_PREDICATE_RESULT_HPP_012705GER
