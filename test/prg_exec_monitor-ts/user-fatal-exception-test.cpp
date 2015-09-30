//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief tests an ability of the Program Execution Monitor to catch user fatal exceptions. Should fail during run. But not crash.
// ***************************************************************************

#include <cassert>

int cpp_main( int, char *[] )  // note the name
{
#if NDEBUG
#pragma message("assert check turned off under NDEBUG")

    return 1;
#else
    int div = 0;

    assert( div != 0 );

    return 0;
#endif
}

//____________________________________________________________________________//

// EOF
