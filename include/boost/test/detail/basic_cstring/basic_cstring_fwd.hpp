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
//  Description : basic_cstring class wrap C string and provide std_string like 
//                interface
// ***************************************************************************

#ifndef BASIC_CSTRING_FWD_HPP
#define BASIC_CSTRING_FWD_HPP

namespace boost {

namespace unit_test {

template<class CharT> class         basic_cstring;
typedef basic_cstring<char const>   const_string;
typedef const_string const          literal_string;
typedef char const* const           c_literal_string;

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2004/05/11 11:00:55  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//

// ***************************************************************************

#endif // BASIC_CSTRING_FWD_HPP

