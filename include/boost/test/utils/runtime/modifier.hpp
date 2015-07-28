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
//  Description : parameter modifiers
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_MODIFIER_HPP
#define BOOST_TEST_UTILS_RUNTIME_MODIFIER_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>

// Boost.Test
#include <boost/test/utils/named_params.hpp>

namespace boost {
namespace runtime {

// ************************************************************************** //
// **************         environment variable modifiers       ************** //
// ************************************************************************** //

namespace {

nfp::typed_keyword<bool,struct optional_t>          optional;
nfp::typed_keyword<bool,struct required_t>          required;
nfp::typed_keyword<bool,struct multiplicable_t>     multiplicable;
nfp::typed_keyword<bool,struct optional_value_t>    optional_value;

nfp::typed_keyword<cstring,struct description_t>    description;
nfp::typed_keyword<cstring,struct env_var_t>        env_var;

nfp::keyword<struct default_value_t>                default_value;
nfp::keyword<struct handler_t>                      handler;

} // local namespace

} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_MODIFIER_HPP
