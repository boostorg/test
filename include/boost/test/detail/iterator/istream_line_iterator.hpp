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
#include <boost/shared_array.hpp>

// STL
#include <iosfwd>

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************         basic_istream_line_iterator          ************** //
// ************************************************************************** //

namespace detail {

template<typename CharT>
class istream_line_iterator_impl {
public:
    typedef basic_cstring<CharT const> value_type;
    typedef basic_cstring<CharT const> reference;

                istream_line_iterator_impl( std::basic_istream<CharT>& input, CharT delimeter )
    : m_input_stream( &input ), m_delimeter( delimeter )            {}

    bool        get()                                               { return getline( *m_input_stream, m_buffer, m_delimeter ); }
    reference   dereference( bool valid ) const                     { return m_buffer; }
    bool        equal( istream_line_iterator_impl const& ) const    { return false; }

private:
    // Data members
    std::basic_istream<CharT>*  m_input_stream;
    std::basic_string<CharT>    m_buffer;
    CharT                       m_delimeter;
};

} // namespace detail

//!! Should we support policy based delimitation

template<typename CharT>
class basic_istream_line_iterator
: public input_iterator_facade<detail::istream_line_iterator_impl<CharT>,basic_istream_line_iterator<CharT> > {
    typedef detail::istream_line_iterator_impl<CharT>                       impl;
    typedef input_iterator_facade<impl,basic_istream_line_iterator<CharT> > base;
public:
    // Constructor
    basic_istream_line_iterator( std::basic_istream<CharT>& input, CharT delimeter )
    : base( impl( input, delimeter ) ) {}

    explicit basic_istream_line_iterator( std::basic_istream<CharT>& input )
    : base( impl( input, input.widen( '\n' ) ) ) {}
};

typedef basic_istream_line_iterator<char>       istream_line_iterator;
typedef basic_istream_line_iterator<wchar_t>    wistream_line_iterator;

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
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

