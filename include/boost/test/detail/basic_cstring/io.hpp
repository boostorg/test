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
//  Description : basic_cstring i/o implementation
// ***************************************************************************

#ifndef  BASIC_CSTRING_IO_HPP
#define  BASIC_CSTRING_IO_HPP

// Boost.Test
#include <boost/test/detail/basic_cstring/basic_cstring.hpp>

// STL
#include <iosfwd>
#include <string>

//____________________________________________________________________________//

namespace boost {

namespace unit_test {

template<typename CharT1, typename Tr,typename CharT2>
inline std::basic_ostream<CharT1,Tr>&
operator<<( std::basic_ostream<CharT1,Tr>& os, basic_cstring<CharT2> const& str )
{
    CharT1 const* const beg = reinterpret_cast<CharT1 const* const>( str.begin() ); //!!
    CharT1 const* const end = reinterpret_cast<CharT1 const* const>( str.end() );
    os << std::basic_string<CharT1,Tr>( beg, end - beg );

    return os;
}

//____________________________________________________________________________//


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

#endif // BASIC_CSTRING_IO_HPP
