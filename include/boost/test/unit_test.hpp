//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : wrapper include . To be used by end-user
// ***************************************************************************

#ifndef BOOST_UNIT_TEST_HPP
#define BOOST_UNIT_TEST_HPP

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>

#ifndef BOOST_TEST_NO_AUTO_LINK

#define BOOST_LIB_NAME unix_test_framework
#define BOOST_LIB_DIAGNOSTIC yes
#include <boost/config/auto_link.hpp>

#endif

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.10  2003/12/01 00:41:56  rogeeff
//  prerelease cleaning
//

// ***************************************************************************

#endif // BOOST_UNIT_TEST_HPP
