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
#include <boost/test/utils/runtime/fwd.hpp>

// Boost.Test
#include <boost/test/utils/named_params.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

namespace boost {
namespace runtime {

// ************************************************************************** //
// **************         environment variable modifiers       ************** //
// ************************************************************************** //

namespace {

nfp::typed_keyword<cstring,struct description_t>    description;
nfp::typed_keyword<cstring,struct env_var_t>        env_var;
nfp::typed_keyword<cstring,struct end_of_params_t>  end_of_params;
nfp::typed_keyword<cstring,struct neg_prefix_t>     negation_prefix;
nfp::typed_keyword<cstring,struct value_hint_t>     value_hint;

nfp::keyword<struct optional_value_t>               optional_value;
nfp::keyword<struct default_value_t>                default_value;
nfp::keyword<struct handler_t>                      handler;

} // local namespace

} // namespace runtime
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UTILS_RUNTIME_MODIFIER_HPP
