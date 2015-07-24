//  (C) Copyright Gennadiy Rozental 2005-2015.
//  Use, modification, and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : implements facility to hide input traversing details
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_CLA_ARGV_TRAVERSER_IPP
#define BOOST_TEST_UTILS_RUNTIME_CLA_ARGV_TRAVERSER_IPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/trace.hpp>

#include <boost/test/utils/runtime/cla/argv_traverser.hpp>

// STL
#include <memory>
#include <cstring>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::memcpy; }
#endif

namespace boost {

namespace runtime {

namespace cla {

// ************************************************************************** //
// **************          runtime::cla::argv_traverser        ************** //
// ************************************************************************** //

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE
argv_traverser::argv_traverser()
: p_ignore_mismatch( false ), p_separator( ' ' )
{
}

//____________________________________________________________________________//

inline char space_to_VT( char in ) { return in == ' ' ? '\0B' : in; }
inline char VT_to_space( char in ) { return in == '\0B' ? ' ' : in; }

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
argv_traverser::init( int argc, char** argv )
{
    m_buffer.clear();

    for( int index = 1; index < argc; ++index ) {
        std::size_t beg_pos = m_buffer.size();
        m_buffer += argv[index];

        std::transform( m_buffer.begin() + beg_pos, m_buffer.end(),
                        m_buffer.begin() + beg_pos,
                        &space_to_VT );

        if( index != argc-1 )
            m_buffer += ' ';
    }

    m_remainder.reset( new char[m_buffer.size()+1] );
    m_remainder_size    = 0;
    m_work_buffer       = m_buffer;
    m_commited_end      = m_work_buffer.begin();

    next_token();
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
argv_traverser::remainder( int& argc, char** argv )
{
    argc = 1;
    std::size_t pos = 0;
    while(pos < m_remainder_size ) {
        argv[argc++] = m_remainder.get() + pos;

        pos = std::find( m_remainder.get() + pos, m_remainder.get() + m_remainder_size, ' ' ) - m_remainder.get();
        m_remainder[pos++] = '\0';
    }

    std::transform( m_remainder.get(), m_remainder.get() + m_remainder_size,
                    m_remainder.get(),
                    &VT_to_space );
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE cstring
argv_traverser::token() const
{
    return m_token;
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
argv_traverser::next_token()
{
    if( m_work_buffer.is_empty() )
        return;

    m_work_buffer.trim_left( m_token.size() ); // skip remainder of current token

    if( m_work_buffer.size() != m_buffer.size() ) // !! is there a better way to identify first token
        m_work_buffer.trim_left( 1 ); // skip separator if not first token;

    m_token.assign( m_work_buffer.begin(),
                    std::find( m_work_buffer.begin(), m_work_buffer.end(), p_separator ) );
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE cstring
argv_traverser::input() const
{
    return m_work_buffer;
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
argv_traverser::trim( std::size_t size )
{
    m_work_buffer.trim_left( size );

    if( size <= m_token.size() )
        m_token.trim_left( size );
    else {
        m_token.assign( m_work_buffer.begin(),
                        std::find( m_work_buffer.begin(), m_work_buffer.end(), p_separator ) );
    }
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE bool
argv_traverser::match_front( cstring str )
{
    return m_work_buffer.size() < str.size() ? false : m_work_buffer.substr( 0, str.size() ) == str;
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE bool
argv_traverser::match_front( char c )
{
    return first_char( m_work_buffer ) == c;
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE bool
argv_traverser::eoi() const
{
    return m_work_buffer.is_empty();
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
argv_traverser::commit()
{
    m_commited_end = m_work_buffer.begin();
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
argv_traverser::rollback()
{
    m_work_buffer.assign( m_commited_end, m_work_buffer.end() );
    m_token.assign( m_work_buffer.begin(),
                    std::find( m_work_buffer.begin(), m_work_buffer.end(), p_separator ) );

}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE std::size_t
argv_traverser::input_pos() const
{
    return m_work_buffer.begin() - m_commited_end;
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE bool
argv_traverser::handle_mismatch()
{
    if( !p_ignore_mismatch )
        return false;

    std::memcpy( m_remainder.get() + m_remainder_size, token().begin(), token().size() );
    m_remainder_size += token().size();
    m_remainder[m_remainder_size++] = p_separator;

    next_token();
    commit();

    return true;
}

//____________________________________________________________________________//

} // namespace cla

} // namespace runtime

} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_CLA_ARGV_TRAVERSER_IPP
