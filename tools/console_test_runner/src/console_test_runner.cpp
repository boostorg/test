//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// Boost.Test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

// Boost.Runtime.Param
#include <boost/test/utils/runtime/cla/named_parameter.hpp>
#include <boost/test/utils/runtime/cla/parser.hpp>

namespace rt  = boost::runtime;
namespace cla = boost::runtime::cla;

// STL
#include <iostream>

// PLATFORM HEADER
#if defined(BOOST_WINDOWS) && !defined(BOOST_DISABLE_WIN32)
#define USE_WIN32_API
#include <windows.h>
#endif

//____________________________________________________________________________//

static std::string test_lib_name;
static std::string init_func_name( "init_unit_test" );
#ifdef USE_WIN32_API
static HINSTANCE test_lib_handle;
#endif

bool load_test_lib()
{
    typedef bool (*init_func_ptr)();
    init_func_ptr init_func;

#ifdef USE_WIN32_API
    HINSTANCE test_lib_handle = LoadLibrary( test_lib_name.c_str() );
    if( !test_lib_handle )
        throw std::logic_error( std::string("Fail to load test library ").append( test_lib_name ) );

    init_func = 
        reinterpret_cast<init_func_ptr>( ::GetProcAddress( test_lib_handle, init_func_name.c_str() ) );
    
    if( !init_func )
        throw std::logic_error( std::string("Couldn't locate test initilization function ")
                                .append( init_func_name ) );
#endif

    return (*init_func)();
}

//____________________________________________________________________________//

int main( int argc, char* argv[] )
{
    try {
        cla::parser P;

        P - cla::ignore_mismatch 
            << cla::named_parameter<rt::cstring>( "test" ) - (cla::prefix = "--")
            << cla::named_parameter<rt::cstring>( "init" ) - (cla::prefix = "--",cla::optional);

        P.parse( argc, argv );

        assign_op( test_lib_name, P.get( "test" ), 0 );
        if( P["init"] )
            assign_op( init_func_name, P.get( "init" ), 0 );

        bool res = ::boost::unit_test::unit_test_main( &load_test_lib, argc, argv );

#ifdef USE_WIN32_API
        if( test_lib_handle )
            FreeLibrary( test_lib_handle );
#endif
        return res;
    }
    catch( rt::logic_error const& ex ) {
        std::cout << "Fail to parse command line arguments: " << ex.msg() << std::endl;
        return -1;
    }
}

//____________________________________________________________________________//

// EOF
