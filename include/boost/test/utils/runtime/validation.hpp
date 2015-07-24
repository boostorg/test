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
//  Description : defines exceptions and validation tools
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_VALIDATION_HPP
#define BOOST_TEST_UTILS_RUNTIME_VALIDATION_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>

// Boost.Test
#include <boost/test/utils/class_properties.hpp>
#include <boost/test/detail/throw_exception.hpp>

// Boost
#include <boost/shared_ptr.hpp>

// STL
#include <stdexcept>

namespace boost {
namespace runtime {

// ************************************************************************** //
// **************             runtime::logic_error             ************** //
// ************************************************************************** //

class logic_error : public std::exception {
public:
    explicit                logic_error( cstring msg ) : m_msg( msg.begin(), msg.size() ) {}

    std::string const&      msg() const                             { return m_msg; }
    virtual char const*     what() const BOOST_NOEXCEPT_OR_NOTHROW  { return m_msg.c_str(); }

private:
    std::string             m_msg;
};

// ************************************************************************** //
// **************          runtime::report_logic_error         ************** //
// ************************************************************************** //

inline void
report_logic_error( format_stream& msg )
{
    BOOST_TEST_IMPL_THROW( runtime::logic_error( msg.str() ) );
}

//____________________________________________________________________________//

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_VALIDATION_HPP
