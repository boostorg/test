//  (C) Copyright Gennadiy Rozental 2001-2002.
//  (C) Copyright Ullrich Koethe 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Id$
//
//  Description : contains definition for all test tools in test toolbox
// ***************************************************************************

#ifndef BOOST_TEST_TOOLS_HPP
#define BOOST_TEST_TOOLS_HPP

// LOCAL
#include <boost/test/unit_test_log.hpp>
#include <boost/test/detail/class_properties.hpp>
#include <boost/test/detail/grinning_ptr.hpp>

// BOOST
#include <boost/cstdlib.hpp> // for boost::exit_success;
#include <boost/config.hpp>

// STL
#ifdef BOOST_NO_STRINGSTREAM
#include <strstream>
#else
#include <sstream>
#endif // BOOST_NO_STRINGSTREAM

#include <cstdlib>
#include <cstddef>
#include <stdexcept>
#include <memory>

// ************************************************************************** //
// **************                    TOOL BOX                  ************** //
// ************************************************************************** //

#define BOOST_CHECKPOINT(message_) \
    boost::test_toolbox::detail::checkpoint_impl( \
        boost::test_toolbox::detail::wrapstrstream() << message_, __FILE__, __LINE__)

#define BOOST_WARN(predicate) \
    boost::test_toolbox::detail::warn_and_continue_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << #predicate, __FILE__, __LINE__)

#define BOOST_CHECK(predicate) \
    boost::test_toolbox::detail::test_and_continue_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << #predicate, __FILE__, __LINE__)

#define BOOST_CHECK_EQUAL(left_, right_) \
    boost::test_toolbox::detail::equal_and_continue_impl((left_), (right_), \
        boost::test_toolbox::detail::wrapstrstream() << #left_ " == " #right_, __FILE__, __LINE__)

#define BOOST_CHECK_CLOSE(left_, right_, tolerance_src) \
    boost::test_toolbox::detail::compare_and_continue_impl((left_), (right_), (tolerance_src),\
        boost::test_toolbox::detail::wrapstrstream() << #left_ " ~= " #right_, __FILE__, __LINE__)

#define BOOST_REQUIRE(predicate) \
    boost::test_toolbox::detail::test_and_throw_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << #predicate, __FILE__, __LINE__)

#define BOOST_MESSAGE(message_) \
    boost::test_toolbox::detail::message_impl( \
        boost::test_toolbox::detail::wrapstrstream() << message_, __FILE__, __LINE__)

#define BOOST_WARN_MESSAGE(predicate, message_) \
    boost::test_toolbox::detail::warn_and_continue_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << message_, __FILE__, __LINE__,false)

#define BOOST_CHECK_MESSAGE(predicate, message_) \
    boost::test_toolbox::detail::test_and_continue_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << message_, __FILE__, __LINE__,false)

#define BOOST_REQUIRE_MESSAGE(predicate, message_) \
    boost::test_toolbox::detail::test_and_throw_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << message_, __FILE__, __LINE__,false)

#define BOOST_CHECK_PREDICATE( predicate, arg_list_size, arg_list ) \
    boost::test_toolbox::detail::test_and_continue_impl(predicate, BOOST_PLACE_PREDICATE_ARGS ## arg_list_size arg_list, \
        boost::test_toolbox::detail::wrapstrstream() << #predicate << "("\
        << BOOST_PRINT_PREDICATE_ARGS ## arg_list_size arg_list << ")", __FILE__, __LINE__)

#define BOOST_REQUIRE_PREDICATE( predicate, arg_list_size, arg_list ) \
    boost::test_toolbox::detail::test_and_throw_impl(predicate, BOOST_PLACE_PREDICATE_ARGS ## arg_list_size arg_list, \
        boost::test_toolbox::detail::wrapstrstream() << #predicate << "("\
        << BOOST_PRINT_PREDICATE_ARGS ## arg_list_size arg_list << ")", __FILE__, __LINE__)

#define BOOST_ERROR(message_) BOOST_CHECK_MESSAGE( false, message_ )

#define BOOST_FAIL(message_) BOOST_REQUIRE_MESSAGE( false, message_ )

#define BOOST_CHECK_THROW( statement, exception ) \
    try { statement; BOOST_ERROR( "exception "#exception" is expected" ); } \
    catch( exception const& ) { \
        BOOST_CHECK_MESSAGE( true, "exception "#exception" is caught" ); \
    }

#define BOOST_CHECK_EQUAL_COLLECTIONS(left_begin_, left_end_, right_begin_) \
    boost::test_toolbox::detail::equal_and_continue_impl( (left_begin_), (left_end_), (right_begin_),\
        boost::test_toolbox::detail::wrapstrstream() << \
            "{" #left_begin_ ", " #left_end_ "}" " == {" #right_begin_ ", ...}", __FILE__, __LINE__)

#define BOOST_IS_DEFINED(symb) boost::test_toolbox::detail::is_defined_impl( #symb, BOOST_STRINGIZE(=symb) )

// ***************************** //
// helper macros

#define BOOST_PLACE_PREDICATE_ARGS1( first_ ) first_
#define BOOST_PLACE_PREDICATE_ARGS2( first_, second_ ) first_, second_

#define BOOST_PRINT_PREDICATE_ARGS1( first_ ) #first_
#define BOOST_PRINT_PREDICATE_ARGS2( first_, second_ ) #first_ << ", " << #second_

// ***************************** //
// depricated interface

#define BOOST_TEST(predicate)           BOOST_CHECK(predicate)
#define BOOST_CRITICAL_TEST(predicate)  BOOST_REQUIRE(predicate)
#define BOOST_CRITICAL_ERROR(message_)   BOOST_FAIL(message_)

namespace boost {

namespace test_toolbox {

namespace detail {

#ifdef BOOST_NO_STRINGSTREAM
typedef std::ostrstream     out_stringstream;
#else
typedef std::ostringstream  out_stringstream;
#endif // BOOST_NO_STRINGSTREAM

// ************************************************************************** //
// **************                 wrapstrstream                ************** //
// ************************************************************************** //

struct wrapstrstream {
    mutable out_stringstream    m_buf;
    mutable std::string         m_str;

    std::string&                str() const;
};

//____________________________________________________________________________//

template <class T>
inline wrapstrstream const&
operator<<( wrapstrstream const& targ, T const& t )
{ 
    targ.m_buf << t;
    return targ;
}

//____________________________________________________________________________//

inline wrapstrstream const& 
operator<<( wrapstrstream const& targ, wrapstrstream const& src )
{ 
    targ << src.str();
    return targ;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************            extended_predicate_value          ************** //
// ************************************************************************** //

struct extended_predicate_value {
    // Constructor
    explicit    extended_predicate_value( bool predicate_value_ )
    : p_predicate_value( predicate_value_ ), p_message( new wrapstrstream ) {}

    bool        operator!() const { return !p_predicate_value.get(); }

    BOOST_READONLY_PROPERTY( bool, 0, () )  p_predicate_value;
    std::auto_ptr<wrapstrstream>            p_message;
};

// ************************************************************************** //
// **************                test_tool_failed              ************** //
// ************************************************************************** //

// exception used to implement critical checks

struct test_tool_failed : public std::exception {
    explicit            test_tool_failed( char const* message_ ) 
    : m_what( message_ )                                {}
    
                        ~test_tool_failed() throw()     {}

    virtual char const* what() const throw()            { return m_what.c_str(); }

private:
    std::string         m_what;
};

// ************************************************************************** //
// **************            TOOL BOX Implementation           ************** //
// ************************************************************************** //

void
checkpoint_impl( wrapstrstream const& message_, char const* file_name_, int line_num_ );

//____________________________________________________________________________//

void
message_impl( wrapstrstream const& message_, char const* file_name_, int line_num_ );

//____________________________________________________________________________//

// ************************************* //

void
warn_and_continue_impl( bool predicate_, wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        bool add_fail_pass_ = true );

//____________________________________________________________________________//

void
warn_and_continue_impl( extended_predicate_value const& v_, wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        bool add_fail_pass_ = true );

//____________________________________________________________________________//

// ************************************* //

bool  // return true if error detected
test_and_continue_impl( bool predicate_, wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        bool add_fail_pass_ = true, 
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors );
void
test_and_throw_impl   ( bool predicate_, wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        bool add_fail_pass_ = true, 
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_fatal_errors );

//____________________________________________________________________________//

bool
test_and_continue_impl( extended_predicate_value const& v_, wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        bool add_fail_pass_ = true, 
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors );

//____________________________________________________________________________//

// Borland bug workaround
#if defined(__BORLANDC__) && (__BORLANDC__ < 0x560)
bool
test_and_continue_impl( void* ptr, wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        bool add_fail_pass_ = true, 
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors )
{
    return test_and_continue_impl( !!ptr, message_, file_name_, line_num_, add_fail_pass_, log_level_ );
}
#endif

//____________________________________________________________________________//

void
test_and_throw_impl   ( extended_predicate_value const& v_, wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        bool add_fail_pass_ = true, 
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_fatal_errors );

//____________________________________________________________________________//

template<typename ArgType, typename Predicate>
inline bool
test_and_continue_impl( Predicate const& pred_, ArgType const& arg_,
                        wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors )
{
    bool predicate = pred_( arg_ );

    if( !predicate ) {
        return test_and_continue_impl( predicate,
                                       wrapstrstream() << "test " << message_ << " failed for " << arg_,
                                       file_name_, line_num_, false, log_level_ );
    }

    return test_and_continue_impl( predicate, message_, file_name_, line_num_, true, log_level_ );
}

//____________________________________________________________________________//

template<typename ArgType, typename Predicate>
inline void
test_and_throw_impl   ( Predicate const& pred_, ArgType const& arg_,
                        wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_fatal_errors )
{
    if( test_and_continue_impl( arg_, pred_, message_, file_name_, line_num_, log_level_ ) ) {
        throw test_tool_failed( "" ); // error already reported by test_and_continue_impl
    }
}

//____________________________________________________________________________//

template<typename First, typename Second, typename Predicate>
inline bool
test_and_continue_impl( Predicate const& pred_, First const& first_, Second const& second_,
                        wrapstrstream const& message_,
                        char const* file_name_, int line_num_,
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors )
{
    bool predicate = pred_( first_, second_ );

    if( !predicate ) {
        return test_and_continue_impl( predicate,
            wrapstrstream() << "test " << message_ << " failed for (" << first_ << ", " << second_ << ")",
            file_name_, line_num_, false, log_level_ );
    }

    return test_and_continue_impl( predicate, message_, file_name_, line_num_, true, log_level_ );
}

//____________________________________________________________________________//

template<typename First, typename Second, typename Predicate>
inline void
test_and_throw_impl   ( First const& first_, Second const& second_, Predicate const& pred_,
                        wrapstrstream const& message_, char const* file_name_, int line_num_,
                        unit_test_framework::report_level log_level_ = unit_test_framework::report_fatal_errors )
{
    if( test_and_continue_impl( first_, second_, pred_, message_, file_name_, line_num_, log_level_ ) ) {
        throw test_tool_failed( "" ); // error already reported by test_and_continue_impl
    }
}

//____________________________________________________________________________//

// ************************************* //

bool
equal_and_continue_impl( char const* left_, char const* right_, wrapstrstream const& message_,
                         char const* file_name_, int line_num_,
                         unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors );

//____________________________________________________________________________//

template <class Left, class Right>
inline bool
equal_and_continue_impl( Left const& left_, Right const& right_,
                         wrapstrstream const& message_, char const* file_name_, int line_num_,
                         unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors )
{
    bool predicate = (left_ == right_);

    if( !predicate ) {
        return test_and_continue_impl( predicate,
            wrapstrstream() << "test " << message_ 
                            << " failed [" << left_ << " != " << right_ << "]",
            file_name_, line_num_, false, log_level_ );
    }

    return test_and_continue_impl( predicate, message_, file_name_, line_num_, true, log_level_ );
}

//____________________________________________________________________________//

template <class Left, class Right>
inline void
equal_and_continue_impl( Left left_begin_, Left left_end_, Right right_begin_, 
                         wrapstrstream const& message_,
                         char const* file_name_, int line_num_,
                         unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors )
{
    for( ;left_begin_ != left_end_; ++left_begin_, ++right_begin_ )
        equal_and_continue_impl( *left_begin_, *right_begin_, message_, file_name_, line_num_, log_level_ );
}

//____________________________________________________________________________//

// ************************************* //

template<typename FPT, typename ToleranceSource>
inline bool
compare_and_continue_impl( FPT left_, FPT right_, ToleranceSource tolerance_src,
                           wrapstrstream const& message_,
                           char const* file_name_, int line_num_,
                           unit_test_framework::report_level log_level_ = unit_test_framework::report_all_errors )
{
    bool predicate = check_is_closed( left_, right_, tolerance_src );

    if( !predicate ) {
        return test_and_continue_impl( predicate,
            wrapstrstream() << "test " << message_ 
                            << " failed [" << left_ << " !~= " << right_ 
                            << " (+/-" << compute_tolerance( tolerance_src, left_ ) << ")]",
            file_name_, line_num_, false, log_level_ );
    }

    return test_and_continue_impl( predicate, message_, file_name_, line_num_, true, log_level_ );
}

//____________________________________________________________________________//

// ************************************* //

bool
is_defined_impl( char const* symbol_name_, char const* symbol_value_ );

//____________________________________________________________________________//

} // namespace detail

// ************************************************************************** //
// **************               output_test_stream             ************** //
// ************************************************************************** //

// class to be used to simplify testing of ostream print functions

class output_test_stream : public detail::out_stringstream {
    typedef detail::extended_predicate_value result_type;
public:
    // Constructor
    explicit        output_test_stream( char const* pattern_file = NULL, bool match_or_save = true );

    // Destructor
    ~output_test_stream();

    // checking function
    result_type     is_empty( bool flush_stream_ = true );
    result_type     check_length( std::size_t length_, bool flush_stream_ = true );
    result_type     is_equal( char const* arg_, bool flush_stream_ = true );
    result_type     is_equal( std::string const& arg_, bool flush_stream_ = true );
    result_type     is_equal( char const* arg_, std::size_t n_, bool flush_stream_ = true );
    bool            match_pattern( bool flush_stream_ = true );

    // helper function
    void            flush();
    std::size_t     length();

private:
    void            sync();

    struct Impl;
    unit_test_framework::detail::grinning_ptr<Impl> m_pimpl;
};

} // namespace test_toolbox

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.18  2002/09/15 11:52:47  johnmaddock
//  Added needed <cstddef> include
//
//  Revision 1.17  2002/09/09 09:07:02  rogeeff
//  descriptions added
//
//  Revision 1.16  2002/08/26 08:31:28  rogeeff
//  Borlan overloading bug workaround
//
//  Revision 1.15  2002/08/21 15:01:23  rogeeff
//  gcc specific fix removed since general is provided
//
//  Revision 1.14  2002/08/20 22:24:53  rogeeff
//  all formal arguments trailed with underscore
//
//  Revision 1.13  2002/08/20 20:57:01  david_abrahams
//  VC6 workaround
//
//  Revision 1.12  2002/08/20 08:52:40  rogeeff
//  cvs keywords added
//
//   5 Oct 01  Reworked version (Gennadiy Rozental)
//   ? ??? 01  Initial version (Ullrich Koethe)

// ***************************************************************************

#endif // BOOST_TEST_TOOLS_HPP
