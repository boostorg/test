//  (C) Copyright Gennadiy Rozental & Ullrich Koethe 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_TEST_TOOLS_HPP
#define BOOST_TEST_TOOLS_HPP

// LOCAL
#include <boost/test/unit_test_log.hpp>
#include <boost/test/detail/class_properties.hpp>
#include <boost/test/detail/grinning_ptr.hpp>
#if defined __GNUC__
#include <boost/test/detail/floating_point_comparison.hpp>
#endif

// BOOST
#include <boost/cstdlib.hpp> // for boost::exit_success; people expecting it
#include <boost/config.hpp>

// STL
#include <strstream> //!!
#include <cstdlib>
#include <stdexcept>
#include <memory>

// ************************************************************************** //
// **************                    TOOL BOX                  ************** //
// ************************************************************************** //

#define BOOST_CHECKPOINT(message) \
    boost::test_toolbox::detail::checkpoint_impl( \
        boost::test_toolbox::detail::wrapstrstream() << message, __FILE__, __LINE__)

#define BOOST_WARN(predicate) \
    boost::test_toolbox::detail::warn_and_continue_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << #predicate, __FILE__, __LINE__)

#define BOOST_CHECK(predicate) \
    boost::test_toolbox::detail::test_and_continue_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << #predicate, __FILE__, __LINE__)

#define BOOST_CHECK_EQUAL(left, right) \
    boost::test_toolbox::detail::equal_and_continue_impl((left), (right), \
        boost::test_toolbox::detail::wrapstrstream() << #left " == " #right, __FILE__, __LINE__)

#define BOOST_CHECK_CLOSE(left, right, tolerance_src) \
    boost::test_toolbox::detail::compare_and_continue_impl((left), (right), (tolerance_src),\
        boost::test_toolbox::detail::wrapstrstream() << #left " ~= " #right, __FILE__, __LINE__)

#define BOOST_REQUIRE(predicate) \
    boost::test_toolbox::detail::test_and_throw_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << #predicate, __FILE__, __LINE__)

#define BOOST_MESSAGE(message) \
    boost::test_toolbox::detail::message_impl( \
        boost::test_toolbox::detail::wrapstrstream() << message, __FILE__, __LINE__)

#define BOOST_WARN_MESSAGE(predicate, message) \
    boost::test_toolbox::detail::warn_and_continue_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << message, __FILE__, __LINE__,false)

#define BOOST_CHECK_MESSAGE(predicate, message) \
    boost::test_toolbox::detail::test_and_continue_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << message, __FILE__, __LINE__,false)

#define BOOST_REQUIRE_MESSAGE(predicate, message) \
    boost::test_toolbox::detail::test_and_throw_impl((predicate), \
        boost::test_toolbox::detail::wrapstrstream() << message, __FILE__, __LINE__,false)

#define BOOST_CHECK_PREDICATE( prediate, arg_list_size, arg_list ) \
    boost::test_toolbox::detail::test_and_continue_impl(prediate, BOOST_PLACE_PREDICATE_ARGS ## arg_list_size arg_list, \
        boost::test_toolbox::detail::wrapstrstream() << #prediate << "("\
        << BOOST_PRINT_PREDICATE_ARGS ## arg_list_size arg_list << ")", __FILE__, __LINE__)

#define BOOST_REQUIRE_PREDICATE( prediate, arg_list_size, arg_list ) \
    boost::test_toolbox::detail::test_and_throw_impl(prediate, BOOST_PLACE_PREDICATE_ARGS ## arg_list_size arg_list, \
        boost::test_toolbox::detail::wrapstrstream() << #prediate << "("\
        << BOOST_PRINT_PREDICATE_ARGS ## arg_list_size arg_list << ")", __FILE__, __LINE__)

#define BOOST_ERROR(message) BOOST_CHECK_MESSAGE( false, message )

#define BOOST_FAIL(message) BOOST_REQUIRE_MESSAGE( false, message )

#define BOOST_CHECK_THROW( statement, exception ) \
    try { statement; BOOST_ERROR( "exception "#exception" is expected" ); } \
    catch( exception const& ) { \
        BOOST_CHECK_MESSAGE( true, "exception "#exception" is caught" ); \
    }

#define BOOST_CHECK_EQUAL_COLLECTIONS(left_begin, left_end, right_begin) \
    boost::test_toolbox::detail::equal_and_continue_impl( (left_begin), (left_end), (right_begin),\
        boost::test_toolbox::detail::wrapstrstream() << \
            "{" #left_begin ", " #left_end "}" " == {" #right_begin ", ...}", __FILE__, __LINE__)

#define BOOST_IS_DEFINED(symb) boost::test_toolbox::detail::is_defined_impl( #symb, BOOST_STRINGIZE(=symb) )

// ***************************** //
// helper macros

#define BOOST_PLACE_PREDICATE_ARGS1( first ) first
#define BOOST_PLACE_PREDICATE_ARGS2( first, second ) first, second

#define BOOST_PRINT_PREDICATE_ARGS1( first ) #first
#define BOOST_PRINT_PREDICATE_ARGS2( first, second ) #first << ", " << #second

// ***************************** //
// depricated interface

#define BOOST_TEST(predicate)           BOOST_CHECK(predicate)
#define BOOST_CRITICAL_TEST(predicate)  BOOST_REQUIRE(predicate)
#define BOOST_CRITICAL_ERROR(message)   BOOST_FAIL(message)

namespace boost {

namespace test_toolbox {

namespace detail {

// ************************************************************************** //
// **************                 wrapstrstream                ************** //
// ************************************************************************** //

struct wrapstrstream {
    mutable std::ostrstream buf;

    char const*          str() const;
};

//____________________________________________________________________________//

template <class T>
inline wrapstrstream const&
operator<<( wrapstrstream const& targ, T const& t )
{ 
    targ.buf << t;
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
    explicit    extended_predicate_value( bool predicate_value ) 
    : p_predicate_value( predicate_value ), p_message( new wrapstrstream ) {}

    bool        operator!() { return !p_predicate_value.get(); }

    BOOST_READONLY_PROPERTY( bool, 0, () )  p_predicate_value;
    std::auto_ptr<wrapstrstream>            p_message;
};

// ************************************************************************** //
// **************                test_tool_failed              ************** //
// ************************************************************************** //

// exception used to implement critical checks

struct test_tool_failed : public std::exception {
    explicit            test_tool_failed( char const* message ) 
    : m_what(message)                                   {}
    
                        ~test_tool_failed() throw()     {}

    virtual char const* what() const throw()            { return m_what.c_str(); }

private:
    std::string m_what;
};

// ************************************************************************** //
// **************            TOOL BOX Implementation           ************** //
// ************************************************************************** //

void
checkpoint_impl( wrapstrstream const& message, char const* file_name, int line_num );

//____________________________________________________________________________//

void
message_impl( wrapstrstream const& message, char const* file_name, int line_num );

//____________________________________________________________________________//

// ************************************* //

void
warn_and_continue_impl( bool predicate, wrapstrstream const& message, char const* file_name, int line_num,
                        bool add_fail_pass = true );

//____________________________________________________________________________//

void
warn_and_continue_impl( extended_predicate_value v, wrapstrstream const& message, char const* file_name, int line_num,
                        bool add_fail_pass = true );

//____________________________________________________________________________//

// ************************************* //

bool  // return true if error detected
test_and_continue_impl( bool predicate, wrapstrstream const& message, char const* file_name, int line_num,
                        bool add_fail_pass = true, 
                        unit_test_framework::report_level loglevel = unit_test_framework::report_all_errors );
void
test_and_throw_impl   ( bool predicate, wrapstrstream const& message, char const* file_name, int line_num,
                        bool add_fail_pass = true, 
                        unit_test_framework::report_level loglevel = unit_test_framework::report_fatal_errors );

//____________________________________________________________________________//

bool
test_and_continue_impl( extended_predicate_value v, wrapstrstream const& message, char const* file_name, int line_num,
                        bool add_fail_pass = true, 
                        unit_test_framework::report_level loglevel = unit_test_framework::report_all_errors );

//____________________________________________________________________________//

void
test_and_throw_impl   ( extended_predicate_value v, wrapstrstream const& message, char const* file_name, int line_num,
                        bool add_fail_pass = true, 
                        unit_test_framework::report_level loglevel = unit_test_framework::report_fatal_errors );

//____________________________________________________________________________//

template<typename ArgType, typename Predicate>
inline bool
test_and_continue_impl( Predicate const& pred, ArgType const& arg,
                        wrapstrstream const& message, char const* file_name, int line_num,
                        unit_test_framework::report_level loglevel = unit_test_framework::report_all_errors )
{
    bool predicate = pred( arg );

    if( !predicate ) {
        return test_and_continue_impl( predicate,
                                       wrapstrstream() << "test " << message << " failed for " << arg,
                                       file_name, line_num, false, loglevel );
    }

    return test_and_continue_impl( predicate, message, file_name, line_num, true, loglevel );
}

//____________________________________________________________________________//

template<typename ArgType, typename Predicate>
inline void
test_and_throw_impl   ( Predicate const& pred, ArgType const& arg,
                        wrapstrstream const& message, char const* file_name, int line_num,
                        unit_test_framework::report_level loglevel = unit_test_framework::report_fatal_errors )
{
    if( test_and_continue_impl( arg, pred, message, file_name, line_num, loglevel ) ) {
        throw test_tool_failed( "" ); // error already reported by test_and_continue_impl
    }
}

//____________________________________________________________________________//

template<typename First, typename Second, typename Predicate>
inline bool
test_and_continue_impl( Predicate const& pred, First const& first, Second const& second,
                        wrapstrstream const& message, char const* file_name, int line_num,
                        unit_test_framework::report_level loglevel = unit_test_framework::report_all_errors )
{
    bool predicate = pred( first, second );

    if( !predicate ) {
        return test_and_continue_impl( predicate,
            wrapstrstream() << "test " << message << " failed for (" << first << ", " << second << ")",
            file_name, line_num, false, loglevel );
    }

    return test_and_continue_impl( predicate, message, file_name, line_num, true, loglevel );
}

//____________________________________________________________________________//

template<typename First, typename Second, typename Predicate>
inline void
test_and_throw_impl   ( First const& first, Second const& second, Predicate const& pred,
                        wrapstrstream const& message, char const* file_name, int line_num,
                        unit_test_framework::report_level loglevel = unit_test_framework::report_fatal_errors )
{
    if( test_and_continue_impl( first, second, pred, message, file_name, line_num, loglevel ) ) {
        throw test_tool_failed( "" ); // error already reported by test_and_continue_impl
    }
}

//____________________________________________________________________________//

// ************************************* //

bool
equal_and_continue_impl( char const* left, char const* right, wrapstrstream const& message,
                         char const* file_name, int line_num,
                         unit_test_framework::report_level loglevel = unit_test_framework::report_all_errors );

//____________________________________________________________________________//

template <class Left, class Right>
inline bool
equal_and_continue_impl( Left const& left, Right const& right,
                         wrapstrstream const& message, char const* file_name, int line_num,
                         unit_test_framework::report_level loglevel = unit_test_framework::report_all_errors )
{
    bool predicate = (left == right);

    if( !predicate ) {
        return test_and_continue_impl( predicate,
                                       wrapstrstream() << "test " << message << " failed [" << left << " != " << right << "]",
                                       file_name, line_num, false, loglevel );
    }

    return test_and_continue_impl( predicate, message, file_name, line_num, true, loglevel );
}

//____________________________________________________________________________//

template <class Left, class Right>
inline void
equal_and_continue_impl( Left left_begin, Left left_end, Right right_begin, 
                         wrapstrstream const& message, char const* file_name, int line_num,
                         unit_test_framework::report_level loglevel = unit_test_framework::report_all_errors )
{
    for( ;left_begin != left_end; ++left_begin, ++right_begin )
        equal_and_continue_impl( *left_begin, *right_begin, message, file_name, line_num, loglevel );
}

//____________________________________________________________________________//

// ************************************* //

template<typename FPT, typename ToleranceSource>
inline bool
compare_and_continue_impl( FPT left, FPT right, ToleranceSource tolerance_src,
                        wrapstrstream const& message, char const* file_name, int line_num,
                        unit_test_framework::report_level loglevel = unit_test_framework::report_all_errors )
{
    close_at_tolerance<FPT> pred( tolerance_src );
    bool predicate = pred( left, right );

    if( !predicate ) {
        return test_and_continue_impl( predicate,
                                       wrapstrstream() << "test " << message 
                                                       << " failed [" << left << " !~= " << right << " (+/-" << pred.p_tolerance.get() << ")]",
                                       file_name, line_num, false, loglevel );
    }

    return test_and_continue_impl( predicate, message, file_name, line_num, true, loglevel );
}

//____________________________________________________________________________//

// ************************************* //

bool
is_defined_impl( char const* symbol_name, char const* symbol_value );

//____________________________________________________________________________//

} // namespace detail

// ************************************************************************** //
// **************               output_test_stream             ************** //
// ************************************************************************** //

// class to be used to simplify testing of ostream print functions

class output_test_stream : public std::ostrstream {
    typedef detail::extended_predicate_value result_type;
public:
    // Constructor
    explicit        output_test_stream( char const* pattern_file = NULL, bool match_or_save = true );

    // Destructor
    ~output_test_stream();

    // checking function
    result_type     is_empty( bool flush_stream = true );
    result_type     check_length( std::size_t length, bool flush_stream = true );
    result_type     is_equal( char const* arg, bool flush_stream = true );
    result_type     is_equal( char const* arg, std::size_t n, bool flush_stream = true );
    bool            match_pattern( bool flush_stream = true );

    // helper function
    void            flush();
    std::size_t     length();

private:
    struct Impl;
    unit_test_framework::detail::grinning_ptr<Impl> m_pimpl;
};

} // namespace test_toolbox

} // namespace boost

//  Revision History
//   ? ??? 01  Initial version (Ullrich Koethe)
//   5 Oct 01  Reworked version (Gennadiy Rozental)

#endif // BOOST_TEST_TOOLS_HPP
