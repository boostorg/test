//  (C) Copyright Gennadiy Rozental 2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : test the Execution Monitor logic regards cpp_main return values
// ***************************************************************************
#include <stdexcept>

int cpp_main( int, char* [] )  // note the name
{
    return -1;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.3  2003/11/06 07:31:42  rogeeff
//  Licence update
//
//  Revision 1.2  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.1  2003/06/09 09:21:47  rogeeff
//  test the Execution Monitor logic regards cpp_main return values
//

// ***************************************************************************

// EOF
