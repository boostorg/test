//  (C) Copyright Gennadiy Rozental 2001.
//  (C) Antony Polukhin 2018.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

// This header must be included ONLY in the *.ipp files to avoid issues with visibility.
// See the long discussion starting
// from https://github.com/boostorg/boost/pull/190#issuecomment-422197454

#ifndef BOOST_TEST_IMPL_TRIVIAL_SIGNLETON_IPP
#define BOOST_TEST_IMPL_TRIVIAL_SIGNLETON_IPP

#include <boost/test/utils/trivial_singleton.hpp>

namespace boost { namespace unit_test {

template<typename Derived>
Derived& singleton<Derived>::instance() {
  static Derived the_inst;
  return the_inst;
}

}}

#endif // BOOST_TEST_IMPL_TRIVIAL_SIGNLETON_IPP
