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
//  Description : common code used by any agent serving as XML printer
// ***************************************************************************

#ifndef BOOST_TEST_XML_PRINTER_HPP_071894GER
#define BOOST_TEST_XML_PRINTER_HPP_071894GER

// Boost.Test
#include <boost/test/detail/basic_cstring/basic_cstring.hpp>
#include <boost/test/detail/fixed_mapping.hpp>
#include <boost/test/detail/custom_manip.hpp>

// BOOST
#include <boost/config.hpp>

// STL
#include <iostream>

#include <boost/test/detail/suppress_warnings.hpp>

namespace boost {

namespace unit_test {

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP) && !BOOST_WORKAROUND(__BORLANDC__, < 0x564)
namespace ut_detail {
#endif

// ************************************************************************** //
// **************               xml print helpers              ************** //
// ************************************************************************** //

inline void
print_escaped( std::ostream& where_to, const_string value )
{
    static fixed_mapping<char,char const*> char_type(
        '<' , "lt",
        '>' , "gt",
        '&' , "amp",
        '\'', "apos" ,
        '"' , "quot",

        0
    );

    for( const_string::iterator it = value.begin(); it != value.end(); ++it ) {
        char const* ref = char_type[*it];

        if( ref )
            where_to << '&' << ref << ';';
        else
            where_to << *it;
    }
}

//____________________________________________________________________________//

typedef custom_manip<struct attr_value_t> attr_value;

inline std::ostream&
operator<<( custom_printer<attr_value> const& p, const_string value )
{
    *p << "=\"";
    print_escaped( *p, value );
    *p << '"';

    return *p;
}

//____________________________________________________________________________//

template<typename T>
inline std::ostream&
operator<<( custom_printer<attr_value> const& p, T const& value )
{
    return *p<< "=\"" << value << '"';
}

//____________________________________________________________________________//

typedef custom_manip<struct pcdata_t> pcdata;

inline std::ostream&
operator<<( custom_printer<pcdata> const& p, const_string value )
{
    print_escaped( *p, value );

    return *p;
}

//____________________________________________________________________________//

#if !defined(BOOST_NO_ARGUMENT_DEPENDENT_LOOKUP) && !BOOST_WORKAROUND(__BORLANDC__, < 0x564)
} // namespace ut_detail
#endif

} // namespace unit_test

} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.3  2005/01/21 07:31:44  rogeeff
//  xml helper facilities reworked to present manipulator interfaces
//
//  Revision 1.2  2004/08/04 02:50:27  rogeeff
//  darwin workarounds
//
//  Revision 1.1  2004/07/19 12:22:15  rogeeff
//  shared xml printer utils
//
// ***************************************************************************

#endif // BOOST_TEST_XML_PRINTER_HPP_071894GER
