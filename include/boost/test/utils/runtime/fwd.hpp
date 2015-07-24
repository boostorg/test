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
//  Description : global framework level forward declaration
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_FWD_HPP
#define BOOST_TEST_UTILS_RUNTIME_FWD_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>

// Boost
#include <boost/shared_ptr.hpp>

// STL
#include <map>

namespace boost {
namespace runtime {

class parameter;

class argument;
typedef shared_ptr<argument> argument_ptr;
typedef shared_ptr<argument const> const_argument_ptr;
typedef std::map<std::string, const_argument_ptr> argument_store;

template<typename T> class typed_argument;

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_FWD_HPP
