//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : contains definition for all test tools in test toolbox
// ***************************************************************************

#ifndef BOOST_TEST_TEST_TOOLS_HPP_012705GER
#define BOOST_TEST_TEST_TOOLS_HPP_012705GER

// Boost.Test
#include <boost/test/predicate_result.hpp>

#include <boost/test/detail/config.hpp>
#include <boost/test/detail/global_typedef.hpp>

#include <boost/test/utils/wrap_stringstream.hpp>
#include <boost/test/utils/basic_cstring/io.hpp>

// Boost
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/enum.hpp> 

// STL
#include <cstddef>          // for std::size_t
#include <iosfwd>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

// ************************************************************************** //
// **************                    TOOL BOX                  ************** //
// ************************************************************************** //

// In macros below following argument abbreviations are used:
// P - predicate
// M - message
// S - statement
// E - excetion
// L - left argument
// R - right argument
// TL - tool level
// CT - check type
// ARGS - arguments list

#define BOOST_CHECK_IMPL_BASE( P, check_descr, TL, CT )     \
    boost::test_tools::tt_detail::check_impl(               \
        P,                                                  \
        boost::wrap_stringstream().ref() << check_descr,    \
        BOOST_TEST_L(__FILE__),                             \
        (std::size_t)__LINE__,                              \
        boost::test_tools::tt_detail::TL,                   \
        boost::test_tools::tt_detail::CT                    \
/**/

//____________________________________________________________________________//

#define BOOST_CHECK_IMPL( P, check_descr, TL, CT )  BOOST_CHECK_IMPL_BASE( P, check_descr, TL, CT ), 0 )

//____________________________________________________________________________//

#define BOOST_TEST_PASS_ARG_INFO( r, data, arg )                                \
    , BOOST_STRINGIZE( arg )                                                    \
    , (boost::wrap_stringstream().ref() <<                                      \
            boost::test_tools::tt_detail::print_helper( arg )).str().c_str()    \
/**/

#define BOOST_CHECK_WITH_ARGS_IMPL( P, check_descr, TL, CT, ARGS )              \
    BOOST_CHECK_IMPL_BASE( P, check_descr, TL, CT ),                            \
    BOOST_PP_SEQ_SIZE( ARGS )                                                   \
    BOOST_PP_SEQ_FOR_EACH( BOOST_TEST_PASS_ARG_INFO, '_', ARGS ) )              \
/**/

//____________________________________________________________________________//

#define BOOST_WARN( P )                     BOOST_CHECK_IMPL( (P), BOOST_TEST_STRINGIZE( P ), WARN, CHECK_PRED )
#define BOOST_CHECK( P )                    BOOST_CHECK_IMPL( (P), BOOST_TEST_STRINGIZE( P ), CHECK, CHECK_PRED )
#define BOOST_REQUIRE( P )                  BOOST_CHECK_IMPL( (P), BOOST_TEST_STRINGIZE( P ), REQUIRE, CHECK_PRED )

//____________________________________________________________________________//

#define BOOST_WARN_MESSAGE( P, M )          BOOST_CHECK_IMPL( (P), M, WARN, CHECK_MSG )
#define BOOST_CHECK_MESSAGE( P, M )         BOOST_CHECK_IMPL( (P), M, CHECK, CHECK_MSG )
#define BOOST_REQUIRE_MESSAGE( P, M )       BOOST_CHECK_IMPL( (P), M, REQUIRE, CHECK_MSG )

//____________________________________________________________________________//

#define BOOST_ERROR( M )                    BOOST_CHECK_MESSAGE( false, M )
#define BOOST_FAIL( M )                     BOOST_REQUIRE_MESSAGE( false, M )

//____________________________________________________________________________//

#define BOOST_MESSAGE( M )                  BOOST_CHECK_IMPL( false, M, WARN, MSG_ONLY )

//____________________________________________________________________________//

#define BOOST_CHECKPOINT( M )               BOOST_CHECK_IMPL( false, M, WARN, SET_CHECKPOINT )

//____________________________________________________________________________//

#define BOOST_CHECK_THROW_IMPL( S, E, P, prefix, TL )                                                   \
    try {                                                                                               \
        S;                                                                                              \
        BOOST_CHECK_IMPL( false, "exception " BOOST_STRINGIZE( E ) " is expected", TL, CHECK_MSG ); }   \
    catch( E const& ex ) {                                                                              \
        ex;                                                                                             \
        BOOST_CHECK_IMPL( P, prefix BOOST_STRINGIZE( E ) " is caught", TL, CHECK_MSG );                 \
    }                                                                                                   \
/**/

//____________________________________________________________________________//

#define BOOST_WARN_THROW( S, E )            BOOST_CHECK_THROW_IMPL( S, E, true, "exception ", WARN )
#define BOOST_CHECK_THROW( S, E )           BOOST_CHECK_THROW_IMPL( S, E, true, "exception ", CHECK )
#define BOOST_REQUIRE_THROW( S, E )         BOOST_CHECK_THROW_IMPL( S, E, true, "exception ", REQUIRE )

//____________________________________________________________________________//

#define BOOST_WARN_EXCEPTION( S, E, P )     BOOST_CHECK_THROW_IMPL( S, E, P( ex ), "incorrect exception ", WARN )
#define BOOST_CHECK_EXCEPTION( S, E, P )    BOOST_CHECK_THROW_IMPL( S, E, P( ex ), "incorrect exception ", CHECK )
#define BOOST_REQUIRE_EXCEPTION( S, E, P )  BOOST_CHECK_THROW_IMPL( S, E, P( ex ), "incorrect exception ", REQUIRE )

//____________________________________________________________________________//

#define BOOST_IGNORE_CHECK( e )             true

//____________________________________________________________________________//

#define BOOST_CHECK_NO_THROW_IMPL( S, TL )                                                          \
    try {                                                                                           \
        S;                                                                                          \
        BOOST_CHECK_IMPL( true, "no exceptions thrown by " BOOST_STRINGIZE( S ), TL, CHECK_MSG ); } \
    catch( ... ) {                                                                                  \
        BOOST_CHECK_IMPL( false, "exception thrown by " BOOST_STRINGIZE( S ), TL, CHECK_MSG );      \
    }                                                                                               \
/**/

#define BOOST_WARN_NO_THROW( S )            BOOST_CHECK_NO_THROW_IMPL( S, WARN )
#define BOOST_CHECK_NO_THROW( S )           BOOST_CHECK_NO_THROW_IMPL( S, CHECK )
#define BOOST_REQUIRE_NO_THROW( S )         BOOST_CHECK_NO_THROW_IMPL( S, REQUIRE )

//____________________________________________________________________________//

#define BOOST_WARN_EQUAL( L, R ) \
    BOOST_CHECK_WITH_ARGS_IMPL( boost::test_tools::tt_detail::equal_impl(L,R), "", WARN, CHECK_EQUAL, (L)(R) )
#define BOOST_CHECK_EQUAL( L, R ) \
    BOOST_CHECK_WITH_ARGS_IMPL( boost::test_tools::tt_detail::equal_impl(L,R), "", CHECK, CHECK_EQUAL, (L)(R) )
#define BOOST_REQUIRE_EQUAL( L, R ) \
    BOOST_CHECK_WITH_ARGS_IMPL( boost::test_tools::tt_detail::equal_impl(L,R), "", REQUIRE, CHECK_EQUAL, (L)(R) )

//____________________________________________________________________________//

#define BOOST_WARN_CLOSE( L, R, T ) \
    BOOST_CHECK_WITH_ARGS_IMPL( boost::test_tools::check_is_close( L, R, T ), "", WARN, CHECK_CLOSE, (L)(R)(T) )
#define BOOST_CHECK_CLOSE( L, R, T ) \
    BOOST_CHECK_WITH_ARGS_IMPL( boost::test_tools::check_is_close( L, R, T ), "", CHECK, CHECK_CLOSE, (L)(R)(T) )
#define BOOST_REQUIRE_CLOSE( L, R, T ) \
    BOOST_CHECK_WITH_ARGS_IMPL( boost::test_tools::check_is_close( L, R, T ), "", REQUIRE, CHECK_CLOSE, (L)(R)(T) )

//____________________________________________________________________________//

#define BOOST_WARN_PREDICATE( P, ARGS ) \
    BOOST_CHECK_WITH_ARGS_IMPL( P( BOOST_PP_SEQ_ENUM( ARGS ) ), BOOST_TEST_STRINGIZE( P ), WARN, CHECK_PRED_WITH_ARGS, ARGS )
#define BOOST_CHECK_PREDICATE( P, ARGS ) \
    BOOST_CHECK_WITH_ARGS_IMPL( P( BOOST_PP_SEQ_ENUM( ARGS ) ), BOOST_TEST_STRINGIZE( P ), CHECK, CHECK_PRED_WITH_ARGS, ARGS )
#define BOOST_REQUIRE_PREDICATE( P, ARGS ) \
    BOOST_CHECK_WITH_ARGS_IMPL( P( BOOST_PP_SEQ_ENUM( ARGS ) ), BOOST_TEST_STRINGIZE( P ), REQUIRE, CHECK_PRED_WITH_ARGS, ARGS )

//____________________________________________________________________________//

#define BOOST_EQUAL_COLLECTIONS_IMPL( L_begin, L_end, R_begin, R_end, TL )      \
    BOOST_CHECK_IMPL_BASE( boost::test_tools::tt_detail::equal_impl(            \
        (L_begin), (L_end), (R_begin), (R_end) ), "", TL, CHECK_EQUAL_COLL ),   \
    4,                                                                          \
    BOOST_STRINGIZE( L_begin ), BOOST_STRINGIZE( L_end ),                       \
    BOOST_STRINGIZE( R_begin ), BOOST_STRINGIZE( R_end ) )                      \
/**/

#define BOOST_WARN_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )          \
    BOOST_EQUAL_COLLECTIONS_IMPL( L_begin, L_end, R_begin, R_end, WARN )
#define BOOST_CHECK_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )         \
    BOOST_EQUAL_COLLECTIONS_IMPL( L_begin, L_end, R_begin, R_end, CHECK )
#define BOOST_REQUIRE_EQUAL_COLLECTIONS( L_begin, L_end, R_begin, R_end )       \
    BOOST_EQUAL_COLLECTIONS_IMPL( L_begin, L_end, R_begin, R_end, REQUIRE )

//____________________________________________________________________________//

#define BOOST_BITWISE_EQUAL_IMPL( L, R, TL )                                    \
    BOOST_CHECK_IMPL_BASE( boost::test_tools::tt_detail::bitwise_equal_impl(    \
        (L), (R) ), "", TL, CHECK_BITWISE_EQUAL ),                              \
    2, BOOST_STRINGIZE( L ), BOOST_STRINGIZE( R ) )                             \
/**/

#define BOOST_WARN_BITWISE_EQUAL( L, R )    BOOST_BITWISE_EQUAL_IMPL( L, R, WARN )
#define BOOST_CHECK_BITWISE_EQUAL( L, R )   BOOST_BITWISE_EQUAL_IMPL( L, R, CHECK )
#define BOOST_REQUIRE_BITWISE_EQUAL( L, R ) BOOST_BITWISE_EQUAL_IMPL( L, R, REQUIRE )

//____________________________________________________________________________//

#define BOOST_IS_DEFINED( symb )            boost::test_tools::tt_detail::is_defined_impl( #symb, BOOST_STRINGIZE(= symb) )

//____________________________________________________________________________//

// ***************************** //
// deprecated interface

#define BOOST_BITWISE_EQUAL( L, R )         BOOST_CHECK_BITWISE_EQUAL( L, R )

namespace boost {

namespace test_tools {

typedef unit_test::const_string      const_string;

namespace tt_detail {

// ************************************************************************** //
// **************              tools classification            ************** //
// ************************************************************************** //

enum check_type {
    CHECK_PRED, 
    CHECK_MSG,
    CHECK_EQUAL,
    CHECK_CLOSE,
    CHECK_BITWISE_EQUAL,
    MSG_ONLY,
    SET_CHECKPOINT, 
    CHECK_PRED_WITH_ARGS,
    CHECK_EQUAL_COLL
};

enum tool_level {
    WARN, CHECK, REQUIRE, PASS
};

// ************************************************************************** //
// **************               log print helper               ************** //
// ************************************************************************** //

template<typename T>
struct print_log_value {
    void    operator()( std::ostream& ostr, T const& t )
    {
        ostr << t; // by default print the value
    }
};

//____________________________________________________________________________//

#define BOOST_TEST_DONT_PRINT_LOG_VALUE( the_type )                 \
namespace boost { namespace test_tools { namespace tt_detail {      \
template<>                                                          \
struct print_log_value<the_type > {                                 \
    void operator()( std::ostream& ostr, the_type const& t ) {}     \
};                                                                  \
}}}                                                                 \
/**/

//____________________________________________________________________________//

template<>
struct print_log_value<char> {
    void    operator()( std::ostream& ostr, char t );
};

//____________________________________________________________________________//

template<>
struct print_log_value<unsigned char> {
    void    operator()( std::ostream& ostr, unsigned char t );
};

//____________________________________________________________________________//

template<>
struct print_log_value<char const*> {
    void    operator()( std::ostream& ostr, char const* t );
};

//____________________________________________________________________________//

template<>
struct print_log_value<wchar_t const*> {
    void    operator()( std::ostream& ostr, wchar_t const* t );
};

//____________________________________________________________________________//

template<typename T>
struct print_helper_t {
    explicit    print_helper_t( T const& t ) : m_t( t ) {}

    T const&    m_t;
};

//____________________________________________________________________________//

template<typename T>
inline print_helper_t<T> print_helper( T const& t )
{
    return print_helper_t<T>( t );
}

//____________________________________________________________________________//

template<typename T>
inline std::ostream& 
operator<<( std::ostream& ostr, print_helper_t<T> const& ph )
{
    print_log_value<T>()( ostr, ph.m_t );

    return ostr;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************            TOOL BOX Implementation           ************** //
// ************************************************************************** //

void check_impl( predicate_result const& pr_, wrap_stringstream& check_descr,
                 const_string file_name_, std::size_t line_num_,
                 tool_level tool_level, check_type ct_,
                 std::size_t num_args, ... );

//____________________________________________________________________________//

template <class Left, class Right>
inline
predicate_result    equal_impl( Left const& left_, Right const& right_ )
{
    return left_ == right_;
}

//____________________________________________________________________________//

predicate_result    equal_impl( char const* left_, char const* right_ );
#if !defined( BOOST_NO_CWCHAR )
predicate_result    equal_impl( wchar_t const* left, wchar_t const* right );
#endif

//____________________________________________________________________________//

template <typename Left, typename Right>
inline predicate_result
equal_impl( Left left_begin_, Left left_end_, Right right_begin_, Right right_end_ )
{
    predicate_result    res( true );
    std::size_t         pos = 0;

    for( ; left_begin_ != left_end_ && right_begin_ != right_end_; ++left_begin_, ++right_begin_, ++pos ) {
        if( *left_begin_ != *right_begin_ ) {
            res = false;
            res.message() << "\nMismatch in a position " << pos << ": "  << *left_begin_ << " != " << *right_begin_;
        }
    }

    if( left_begin_ != left_end_ ) {
        std::size_t r_size = pos;
        while( left_begin_ != left_end_ ) {
            ++pos;
            ++left_begin_;
        }

        res = false;
        res.message() << "\nCollections size mismatch: " << pos << " != " << r_size;
    }

    if( right_begin_ != right_end_ ) {
        std::size_t l_size = pos;
        while( right_begin_ != right_end_ ) {
            ++pos;
            ++right_begin_;
        }

        res = false;
        res.message() << "\nCollections size mismatch: " << l_size << " != " << pos;
    }

    return res;
}

//____________________________________________________________________________//

template <class Left, class Right>
inline predicate_result
bitwise_equal_impl( Left const& left_, Right const& right_ )
{
    predicate_result    res( true );

    std::size_t left_bit_size  = sizeof(Left)*CHAR_BIT;
    std::size_t right_bit_size = sizeof(Right)*CHAR_BIT;

    static Left const  L1( 1 );
    static Right const R1( 1 );

    std::size_t total_bits = left_bit_size < right_bit_size ? left_bit_size : right_bit_size;

    for( std::size_t counter = 0; counter < total_bits; ++counter ) {
        if( ( left_ & ( L1 << counter ) ) != ( right_ & ( R1 << counter ) ) ) {
            res = false;
            res.message() << "\nMismatch in a position " << counter;
        }
    }

    if( left_bit_size != right_bit_size ) {
        res = false;
        res.message() << "\nOperands bit sizes mismatch: " << left_bit_size << " != " << right_bit_size;
    }

    return res;
}

//____________________________________________________________________________//

bool is_defined_impl( const_string symbol_name_, const_string symbol_value_ );

//____________________________________________________________________________//

} // namespace tt_detail

} // namespace test_tools

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.50  2005/02/20 08:27:06  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
//  Revision 1.49  2005/02/01 06:40:06  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.48  2005/01/30 03:32:57  rogeeff
//  Test Tools completely reworked:
//    interfaces streamlined to provide 3 version for each tool
//    implementation revorked to use single vararg formatter function
//    CHECK_COLLECTION now expect 4 arguments
//    BITWISE_EQUAL renamed to CHECK_BITWISE_EQUAL but still provided as depricated
//    CHECK_COLLECTION interface changed to use PP_SEQ and as a result support arbitrary number of predicate arguments
//    most of templates eliminated
//    depricated tools removed
//    print_helper object generator added
//
// ***************************************************************************

#endif // BOOST_TEST_TEST_TOOLS_HPP_012705GER
