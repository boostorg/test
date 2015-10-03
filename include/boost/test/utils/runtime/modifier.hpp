//  (C) Copyright Gennadiy Rozental 2001.
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

// Boost.Test Runtime parameters
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

auto const& description     = unit_test::static_constant<nfp::typed_keyword<cstring,struct description_t>>::value;
auto const& help            = unit_test::static_constant<nfp::typed_keyword<cstring,struct help_t>>::value;
auto const& env_var         = unit_test::static_constant<nfp::typed_keyword<cstring,struct env_var_t>>::value;
auto const& end_of_params   = unit_test::static_constant<nfp::typed_keyword<cstring,struct end_of_params_t>>::value;
auto const& negation_prefix = unit_test::static_constant<nfp::typed_keyword<cstring,struct neg_prefix_t>>::value;
auto const& value_hint      = unit_test::static_constant<nfp::typed_keyword<cstring,struct value_hint_t>>::value;
auto const& optional_value  = unit_test::static_constant<nfp::keyword<struct optional_value_t>>::value;
auto const& default_value   = unit_test::static_constant<nfp::keyword<struct default_value_t>>::value;
auto const& callback        = unit_test::static_constant<nfp::keyword<struct callback_t>>::value;

template<typename EnumType>
using enum_values = unit_test::static_constant<
  nfp::typed_keyword<std::initializer_list<std::pair<const cstring,EnumType>>, struct enum_values_t>
>;

} // local namespace

} // namespace runtime
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UTILS_RUNTIME_MODIFIER_HPP
