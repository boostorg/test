//  (C) Copyright Gennadiy Rozental 2004-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : suppress some warnings 
// ***************************************************************************

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable: 4511) // copy constructor could not be generated
# pragma warning(disable: 4512) // assignment operator could not be generated
# pragma warning(disable: 4100) // unreferenced formal parameter 
# pragma warning(disable: 4996) // <symbol> was declared deprecated 
#endif

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.3  2005/02/01 06:40:07  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.2  2005/01/31 06:00:37  rogeeff
//  deprecated std symbols warning suppressed
//
// ***************************************************************************
