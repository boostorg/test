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
//  Description : cla subsystem forward declarations
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_CLA_FWD_HPP
#define BOOST_TEST_UTILS_RUNTIME_CLA_FWD_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>
#include <boost/test/utils/runtime/fwd.hpp>

// Boost
#include <boost/shared_ptr.hpp>

namespace boost {
namespace runtime {
namespace cla {

class parser;
class basic_param;
class argv_traverser;

typedef shared_ptr<basic_param> basic_param_ptr;
typedef std::vector<basic_param_ptr> param_list;
typedef std::map<std::string, basic_param_ptr> param_store;

// Exception type
typedef std::pair<basic_param_ptr, basic_param_ptr> ambigues_param_definition;

} // namespace cla
} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_CLA_FWD_HPP
