//  (C) Copyright Ullrich Koethe 2001, Gennadiy Rozental 2001-2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

// LOCAL
#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_result.hpp>

// BOOST
#include <boost/config.hpp>

// STL
#include <strstream>
#include <fstream>
#include <algorithm>
#include <cstring>
#include <string>

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::strcmp; using ::strncmp; using ::strlen; }
# endif

namespace boost {

using namespace unit_test_framework;

namespace test_toolbox {

namespace detail {

// ************************************************************************** //
// **************                 wrapstrstream                ************** //
// ************************************************************************** //

std::string&
wrapstrstream::str() const {

#ifdef BOOST_NO_STRINGSTREAM
    m_str.assign( m_buf.str(), m_buf.pcount() );
    m_buf.freeze( false );
#else
    m_str = m_buf.str();
#endif

    return m_str;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************            TOOL BOX Implementation           ************** //
// ************************************************************************** //

void
checkpoint_impl( wrapstrstream const& message, char const* file_name, int line_num )
{
    BOOST_UT_LOG_BEGIN( file_name, line_num, report_test_suites )
        checkpoint( message.str() )
    BOOST_UT_LOG_END
}

//____________________________________________________________________________//

void
message_impl( wrapstrstream const& message, char const* file_name, int line_num )
{
    BOOST_UT_LOG_BEGIN( file_name, line_num, report_messages )
        message.str()
    BOOST_UT_LOG_END
}

//____________________________________________________________________________//

void
warn_and_continue_impl( bool predicate, wrapstrstream const& message,
                        char const* file_name, int line_num, bool add_fail_pass )
{
    if( !predicate ) {
        BOOST_UT_LOG_BEGIN( file_name, line_num, report_warnings )
            (add_fail_pass ? "condition " : "") << message.str() << (add_fail_pass ? " is not satisfied" : "" )
        BOOST_UT_LOG_END
    }
    else {
        BOOST_UT_LOG_BEGIN( file_name, line_num, report_successful_tests )
            "condition " << message.str() << " is satisfied"
        BOOST_UT_LOG_END
    }
}

//____________________________________________________________________________//

void
warn_and_continue_impl( extended_predicate_value v, wrapstrstream const& message, char const* file_name, int line_num,
                        bool add_fail_pass )
{
    warn_and_continue_impl( !!v,
        message << (add_fail_pass && !v ? " is not satisfied" : "" ) << *(v.p_message),
        file_name, line_num, false );
}

//____________________________________________________________________________//

bool
test_and_continue_impl( bool predicate, wrapstrstream const& message,
                        char const* file_name, int line_num,
                        bool add_fail_pass, report_level loglevel )
{
    if( !predicate ) {
        unit_test_result::instance().inc_failed_assertions();

        BOOST_UT_LOG_BEGIN( file_name, line_num, loglevel )
            (add_fail_pass ? "test " : "") << message.str() << (add_fail_pass ? " failed" : "")
        BOOST_UT_LOG_END

        return true;
    }
    else {
        unit_test_result::instance().inc_passed_assertions();

        BOOST_UT_LOG_BEGIN( file_name, line_num, report_successful_tests )
            (add_fail_pass ? "test " : "") << message.str() << (add_fail_pass ? " passed" : "")
        BOOST_UT_LOG_END

        return false;
    }
}

//____________________________________________________________________________//

bool
test_and_continue_impl( extended_predicate_value v, wrapstrstream const& message,
                        char const* file_name, int line_num,
                        bool add_fail_pass, report_level loglevel )
{
    return test_and_continue_impl( !!v,
        message << (add_fail_pass ? (!v ? " failed" : " passed") : "") << *(v.p_message),
        file_name, line_num, false, loglevel );
}

//____________________________________________________________________________//

void
test_and_throw_impl( bool predicate, wrapstrstream const& message,
                     char const* file_name, int line_num,
                     bool add_fail_pass, report_level loglevel )
{
    if( test_and_continue_impl( predicate, message, file_name, line_num, add_fail_pass, loglevel ) ) {
        throw test_tool_failed( "" ); // error already reported by test_and_continue_impl
    }
}

//____________________________________________________________________________//

void
test_and_throw_impl( extended_predicate_value v, wrapstrstream const& message,
                     char const* file_name, int line_num,
                     bool add_fail_pass, report_level loglevel )
{
    if( test_and_continue_impl( v, message, file_name, line_num, add_fail_pass, loglevel ) ) {
        throw test_tool_failed( "" ); // error already reported by test_and_continue_impl
    }
}

//____________________________________________________________________________//

bool
equal_and_continue_impl( char const* left, char const* right, wrapstrstream const& message,
                         char const* file_name, int line_num,
                         report_level loglevel )
{
    bool predicate = std::strcmp( left, right ) == 0;

    if( !predicate ) {
        return test_and_continue_impl( predicate,
            wrapstrstream() << "test " << message.str() << " failed [" << left << " != " << right << "]",
            file_name, line_num, false, loglevel );
    }

    return test_and_continue_impl( predicate, message, file_name, line_num, true, loglevel );
}

//____________________________________________________________________________//

bool
is_defined_impl( char const* symbol_name, char const* symbol_value )
{
    return std::strcmp( symbol_name, symbol_value+1 ) != 0;
}

//____________________________________________________________________________//

} // namespace detail

// ************************************************************************** //
// **************               output_test_stream             ************** //
// ************************************************************************** //

struct output_test_stream::Impl
{
    std::fstream    m_pattern_to_match_or_save;
    bool            m_match_or_save;
    std::string     m_synced_string;
};

//____________________________________________________________________________//

output_test_stream::output_test_stream( char const* pattern_file, bool match_or_save )
: m_pimpl( new Impl ) {
    if( pattern_file != NULL && pattern_file[0] != '\0' )
        m_pimpl->m_pattern_to_match_or_save.open( pattern_file, match_or_save ? std::ios::in : std::ios::out );
    m_pimpl->m_match_or_save = match_or_save;
}

//____________________________________________________________________________//

output_test_stream::~output_test_stream() {
}

//____________________________________________________________________________//

detail::extended_predicate_value
output_test_stream::is_empty( bool flush_stream ) {
    sync();

    result_type res( m_pimpl->m_synced_string.empty() );

    if( !res.p_predicate_value.get() ) {
        *(res.p_message) << ". Output content: \"" << m_pimpl->m_synced_string << '\"';

        if( flush_stream )
            flush();
    }

    return res;
}

//____________________________________________________________________________//

detail::extended_predicate_value
output_test_stream::check_length( std::size_t length_, bool flush_stream )
{
    sync();

    result_type res( m_pimpl->m_synced_string.length() == length_ );

    if( !res.p_predicate_value.get() )
        *(res.p_message) << ". Output length is " << m_pimpl->m_synced_string.length();

    if( flush_stream )
        flush();

    return res;
}

//____________________________________________________________________________//

detail::extended_predicate_value
output_test_stream::is_equal( char const* arg, bool flush_stream )
{
    sync();

    result_type res( m_pimpl->m_synced_string == arg );

    if( !res.p_predicate_value.get() )
        *(res.p_message) << ". Output content: \"" << m_pimpl->m_synced_string << '\"';

    if( flush_stream )
        flush();

    return res;
}

//____________________________________________________________________________//

detail::extended_predicate_value
output_test_stream::is_equal( std::string const& arg, bool flush_stream )
{
    return is_equal( arg.c_str(), arg.length(), flush_stream );
}

//____________________________________________________________________________//

detail::extended_predicate_value
output_test_stream::is_equal( char const* arg, std::size_t n, bool flush_stream )
{
    sync();

    result_type res( m_pimpl->m_synced_string == std::string( arg, n ) );

    if( !res.p_predicate_value.get() )
        *(res.p_message) << ". Output content: \"" << m_pimpl->m_synced_string << '\"';

    if( flush_stream )
        flush();

    return res;
}

//____________________________________________________________________________//

bool
output_test_stream::match_pattern( bool flush_stream )
{
    sync();

    bool result = true;

    if( !m_pimpl->m_pattern_to_match_or_save.is_open() )
        result = false;
    else {
        if( m_pimpl->m_match_or_save ) {
            char const* ptr = m_pimpl->m_synced_string.c_str();

            for( std::size_t i = 0; i != m_pimpl->m_synced_string.length(); i++, ptr++ ) {
                char c;
                m_pimpl->m_pattern_to_match_or_save.get( c );

                if( m_pimpl->m_pattern_to_match_or_save.fail() ||
                    m_pimpl->m_pattern_to_match_or_save.eof() )
                {
                    result = false;
                    break;
                }

                if( *ptr != c ) {
                    result = false;
                }
            }
        }
        else {
            m_pimpl->m_pattern_to_match_or_save.write( m_pimpl->m_synced_string.c_str(), m_pimpl->m_synced_string.length() );
            m_pimpl->m_pattern_to_match_or_save.flush();
        }
    }

    if( flush_stream )
        flush();

    return result;
}

//____________________________________________________________________________//

void
output_test_stream::flush()
{
    m_pimpl->m_synced_string.erase();

    seekp( 0, std::ios::beg );

#ifndef BOOST_NO_STRINGSTREAM
    str( std::string() );
#endif
}

//____________________________________________________________________________//

std::size_t
output_test_stream::length()
{
    sync();

    return m_pimpl->m_synced_string.length();
}

//____________________________________________________________________________//

void
output_test_stream::sync()
{
#ifdef BOOST_NO_STRINGSTREAM
    m_pimpl->m_synced_string.assign( str(), pcount() );
    freeze( false );
#else
    m_pimpl->m_synced_string = str();
#endif
}

//____________________________________________________________________________//

} // namespace test_toolbox

} // namespace boost

//  Revision History
//   ? ??? 01  Initial version (Ullrich Koethe)
//   5 Oct 01  Reworked version (Gennadiy Rozental)

// EOF
