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
//  Description : 
// ***************************************************************************

#ifndef BOOST_ISTREAM_LINE_ITERATOR_HPP
#define BOOST_ISTREAM_LINE_ITERATOR_HPP

// Boost
#include <boost/test/detail/basic_cstring/basic_cstring.hpp>
#include <boost/test/detail/iterator/input_iterator_facade.hpp>

// STL
#include <iosfwd>

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************         basic_istream_line_iterator          ************** //
// ************************************************************************** //

//!! Should we support policy based delimitation

template<typename CharT>
class basic_istream_line_iterator
: public input_iterator_facade<basic_istream_line_iterator<CharT>,
                               std::basic_string<CharT>,
                               basic_cstring<CharT const> > {
    typedef input_iterator_facade<basic_istream_line_iterator<CharT>,
                                  std::basic_string<CharT>,
                                  basic_cstring<CharT const> > base;
public:
    // Constructors
    basic_istream_line_iterator() {}
    basic_istream_line_iterator( std::basic_istream<CharT>& input, CharT delimeter )
    : m_input_stream( &input ), m_delimeter( delimeter )
    {
        init();
    }
    explicit basic_istream_line_iterator( std::basic_istream<CharT>& input )
    : m_input_stream( &input ), m_delimeter( input.widen( '\n' ) )
    {
        init();
    }

private:
    friend class input_iterator_core_access;

    // increment implementation
    bool                     get()
    {
        return std::getline( *m_input_stream, m_value, m_delimeter );
    }

    // Data members
    std::basic_istream<CharT>*  m_input_stream;
    CharT                       m_delimeter;
};

typedef basic_istream_line_iterator<char>       istream_line_iterator;
typedef basic_istream_line_iterator<wchar_t>    wistream_line_iterator;

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.4  2004/06/05 11:03:12  rogeeff
//  input_iterator_adaptor simplified
//  token_iterator added
//
//  Revision 1.3  2004/05/27 07:01:49  rogeeff
//  portability workarounds
//
//  Revision 1.2  2004/05/25 10:29:09  rogeeff
//  use standard getline
//  eliminate initialize
//  proper handle \n in wide case
//
//  Revision 1.1  2004/05/21 06:30:10  rogeeff
//  ifstream_line_iterator added
//
// ***************************************************************************

#endif // BOOST_ISTREAM_LINE_ITERATOR_HPP

