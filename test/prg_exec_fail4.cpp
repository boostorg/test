//  (C) Copyright Gennadiy Rozental 2003.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

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
//  Revision 1.1  2003/06/09 09:21:47  rogeeff
//  test the Execution Monitor logic regards cpp_main return values
//

// ***************************************************************************

// EOF
