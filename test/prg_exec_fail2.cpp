//  Divide by zero test

//  Copyright Gennadiy Rozental 2001-2003.
//  Copyright Beman Dawes 2001. 
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org/libs/test for documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests an ability of Program Execution Monitor to catch 
//  system exceptions. Should fail during run.
// ***************************************************************************

int cpp_main( int, char *[] )  // note the name
{
    int div = 0;
    return 10 / div;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.8  2003/09/14 12:42:22  beman_dawes
//  Change to new license (with Gennadiy's permission)
//
//  Revision 1.7  2003/06/09 09:25:24  rogeeff
//  1.30.beta1
//
//  Revision 1.6  2002/12/09 05:16:50  rogeeff
//  *** empty log message ***
//
//  Revision 1.5  2002/11/02 20:04:43  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
