//  (C) Copyright Gennadiy Rozental 2002-2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests an ability of the Program Execution Monitor to catch 
//  user fatal exceptions. Should fail during run. But not crash.
// ***************************************************************************

#include <cassert>

int cpp_main( int, char *[] )  // note the name
{
  int div = 0;

  assert( div != 0 );

  return 0;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.4  2003/11/06 07:31:42  rogeeff
//  Licence update
//
//  Revision 1.3  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.2  2003/06/09 09:25:24  rogeeff
//  1.30.beta1
//
//  Revision 1.1  2002/12/09 05:13:31  rogeeff
//  Initial commit
//

// ***************************************************************************

// EOF
