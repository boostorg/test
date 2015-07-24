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
//  Description : defines facility to hide input traversing details
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_CLA_ARGV_TRAVERSER_HPP
#define BOOST_TEST_UTILS_RUNTIME_CLA_ARGV_TRAVERSER_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>

namespace boost {
namespace runtime {
namespace cla {

// ************************************************************************** //
// **************          runtime::cla::argv_traverser        ************** //
// ************************************************************************** //

class argv_traverser {
public:
    static const char END_OF_TOKEN;
 
    argv_traverser( int argc, char** argv )
    : m_argc( argc )
    , m_curr_arg( 0 )
    , m_arg_size( 0 )
    , m_arg_pos( 0 )
    , m_argv( argv )
    {
        next_arg();
    }

    void        remainder( int& argc, char** argv )
    {
        argc = m_argc - m_curr_arg;

        for( int i = 0; i < argc ; ++i )
            argv[i] = m_argv[m_curr_arg+i];
    }

    bool        eoi() const
    {
        return m_curr_arg == m_argc;
    }

    char    get_char()
    {
        if( eoi() )
            return END_OF_TOKEN;

        char res = m_argv[m_curr_arg][m_arg_pos];

        if( ++m_arg_pos == m_arg_size )
            next_arg();

        return res;
    }

    cstring     get_token()
    {
        if( eoi() )
            return cstring();

        cstring token( m_argv[m_curr_arg] + m_arg_pos, m_arg_size - m_arg_pos );

        next_arg();

        return token;
    }

    bool        match_front( cstring str )
    {
        bool res = m_arg_size - m_arg_pos < str.size() 
                    ? false 
                    : cstring( m_argv[m_curr_arg] + m_arg_pos, str.size() ) == str;

        if( res ) {
            m_arg_pos += str.size();

            if( m_arg_pos == m_arg_size )
                next_arg();
        }
            
        return res;
    }

private:
    void        next_arg()
    {
        ++m_curr_arg;

        if( !eoi() ) {
            m_arg_size = ::strlen( m_argv[m_curr_arg] );
            m_arg_pos  = 0;
        }
    }

    // Data members
    std::size_t m_argc;         // total number of arguments
    std::size_t m_curr_arg;     // current argument index in argv
    std::size_t m_arg_size;     // current argument size
    std::size_t m_arg_pos;      // current argument position
    char**      m_argv;         // all arguments
};

const char argv_traverser::END_OF_TOKEN = '\0';

} // namespace cla
} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_CLA_ARGV_TRAVERSER_HPP
