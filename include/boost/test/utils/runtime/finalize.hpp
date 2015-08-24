//  (C) Copyright Gennadiy Rozental 2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : runtime parameters initialization final step
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_FINALIZE_HPP
#define BOOST_TEST_UTILS_RUNTIME_FINALIZE_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/parameter.hpp>
#include <boost/test/utils/runtime/argument.hpp>

// Boost.Test
#include <boost/test/utils/foreach.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

namespace boost {
namespace runtime {

inline void
finalize_arguments( parameters_store const& params, runtime::arguments_store& args )
{
    BOOST_TEST_FOREACH( parameters_store::storage_type::value_type const&, v, params.all() ) {
        basic_param_ptr param = v.second;

        if( !args.has( param->p_name ) ) {
            if( param->p_has_default_value )
                param->set_default( args );
        }

        if( !args.has( param->p_name ) ) {
            if( !param->p_optional )
                BOOST_TEST_IMPL_THROW( missing_arg() << "Required argument " << param->p_name << " is missing." );
        }
    }    
}

} // namespace runtime
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UTILS_RUNTIME_FINALIZE_HPP
