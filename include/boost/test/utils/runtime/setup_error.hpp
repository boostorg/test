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
//  Description : defines runtime parameters setup error
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_SETUP_ERROR_HPP
#define BOOST_TEST_UTILS_RUNTIME_SETUP_ERROR_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>
#include <boost/test/utils/runtime/fwd.hpp>

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

namespace boost {
namespace runtime {

// ************************************************************************** //
// **************             runtime::setup_error             ************** //
// ************************************************************************** //

class setup_error {
public:
    std::string msg;
};

//____________________________________________________________________________//

inline setup_error
operator<<(setup_error&& err, char const* val)
{
    err.msg.append( val );

    return err;
}

//____________________________________________________________________________//

template<typename T>
inline setup_error
operator<<(setup_error&& err, T&& val)
{
    err.msg.append( lexical_cast<std::string>( val ) );

    return err;
}

//____________________________________________________________________________//

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_SETUP_ERROR_HPP
