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
    typedef basic_cstring<CharT> value_type;
    typedef basic_cstring<CharT> reference;

    explicit istream_line_iterator_impl( std::basic_istream<CharT>& input, CharT delimeter, std::size_t buffer_size )
    : m_input_stream( &input ), m_buffer_size( buffer_size ), m_input_length( 0 ), m_delimeter( delimeter )
    {
    }

    bool                initialize()
    {
        CharT c;
        m_input_stream->get( c );
        m_input_stream->unget();

        if( m_input_stream->good() ) {
            m_buffer.reset( new CharT[m_buffer_size] );

            return true;
        }

        return false;
    }

    bool                get()
    {
        if( m_input_stream->eof() )
            return false;

        m_input_stream->getline( m_buffer.get(), m_buffer_size, m_delimeter );
        m_input_length = m_input_stream->eof() 
            ? m_input_stream->gcount() 
            : (m_input_stream->gcount() ? m_input_stream->gcount()-1 : 0 );

        return true;
    }

    reference           dereference( bool valid ) const
    {
        return valid ? basic_cstring<CharT>( m_buffer.get(), m_input_length ) : basic_cstring<CharT>();
    }

    bool                equal( istream_line_iterator_impl const& ) const { return false; }

private:
    // Data members
    std::basic_istream<CharT>* m_input_stream;

    shared_array<CharT> m_buffer;
    std::size_t         m_buffer_size;
    std::size_t         m_input_length;
    CharT               m_delimeter;
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
    explicit basic_istream_line_iterator( std::basic_istream<CharT>& input,
                                          CharT                      delimeter   = CharT( '\n' ),
                                          std::size_t                buffer_size = 512 )
    : base( impl( input, delimeter, buffer_size ) )
    {}
};

typedef basic_istream_line_iterator<char>       istream_line_iterator;
typedef basic_istream_line_iterator<wchar_t>    wistream_line_iterator;

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2004/05/21 06:30:10  rogeeff
//  ifstream_line_iterator added
//
// ***************************************************************************

#endif // BOOST_ISTREAM_LINE_ITERATOR_HPP

