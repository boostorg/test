//  (C) Copyright Gennadiy Rozental 2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : generic char traits class; wraps std::char_traits
// ***************************************************************************

#ifndef BCS_CHAR_TRAITS_HPP
#define BCS_CHAR_TRAITS_HPP

#include <boost/config.hpp>
#include <boost/detail/workaround.hpp>

// STL
#include <string>

namespace boost {

namespace unit_test {

namespace detail {

template<typename CharT> struct bcs_base_char           { typedef CharT type; };

template<> struct bcs_base_char<char const>             { typedef char type; };
template<> struct bcs_base_char<unsigned char>          { typedef char type; };
#if !BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x551))
template<> struct bcs_base_char<unsigned char const>    { typedef char type; };
#endif

template<> struct bcs_base_char<wchar_t const>          { typedef wchar_t type; };

// ************************************************************************** //
// **************               bcs_char_traits                ************** //
// ************************************************************************** //

template<typename CharT>
struct bcs_char_traits_impl
{
    static bool eq( CharT const& c1, CharT const& c2 )
    {
        return c1 == c2;
    }
    static bool lt( CharT const& c1, CharT const& c2 )
    {
        return c1 < c2;
    }

    static int compare( CharT const* cstr1, CharT const* cstr2, std::size_t n )
    {
        while( n > 0 ) {
            if( !eq( *cstr1, *cstr2 ) )
                return lt( *cstr1, *cstr2 ) ? -1 : 1;
            ++cstr1;
            ++cstr2;
            --n;
        }

        return 0;
    }

    static std::size_t length( CharT const* cstr )
    {
        CharT const null_char = CharT();

        CharT const* ptr = cstr;
        while( !eq( *ptr, null_char ) )
            ++ptr;

        return ptr - cstr;
    }

    static CharT const* find( CharT const* s, std::size_t n, CharT c )
    {
        while( n > 0 ) {
            if( eq( *s, c ) )
                return s;

            ++s;
            --n;
        }
        return 0;
    }
};

#if  BOOST_WORKAROUND(__GNUC__, < 3) && !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
template<> struct bcs_char_traits_impl<char> : std::string_char_traits<char> {};
template<> struct bcs_char_traits_impl<wchar_t> : std::string_char_traits<wchar_t> {};
#else
template<> struct bcs_char_traits_impl<char> : std::char_traits<char> {};
template<> struct bcs_char_traits_impl<wchar_t> : std::char_traits<wchar_t> {};
#endif

template<typename CharT>
class bcs_char_traits : public bcs_char_traits_impl<CharT> {
    typedef typename detail::bcs_base_char<CharT>::type                                 the_base_char;
public:
#if  BOOST_WORKAROUND(__GNUC__, < 3) && !defined(__SGI_STL_PORT) && !defined(_STLPORT_VERSION)
    typedef std::basic_string<the_base_char, std::string_char_traits<the_base_char> >   std_string;
#else
    typedef std::basic_string<the_base_char, std::char_traits<the_base_char> >          std_string;
#endif
};

} // namespace detail

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2004/06/05 11:02:15  rogeeff
//  std::traits usage reworked
//
// ***************************************************************************

#endif // BCS_CHAR_TRAITS_HPP
