//  (C) Copyright Gennadiy Rozental 2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for most recent version including documentation.
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
//  Revision 1.2  2003/10/27 07:13:32  rogeeff
//  licence update
//
//  Revision 1.1  2003/06/09 09:21:47  rogeeff
//  test the Execution Monitor logic regards cpp_main return values
//

// ***************************************************************************

// EOF
