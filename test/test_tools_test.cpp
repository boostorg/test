//  (C) Copyright Gennadiy Rozental 2001-2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests all Test Tools but output_test_stream
// ***************************************************************************

// Boost.Test
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/unit_test_result.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/output_test_stream.hpp>
using namespace boost::unit_test;

// BOOST
#include <boost/bind.hpp>

// STL
#include <iostream>
#include <iomanip>
#include <list>
#include <typeinfo>
#include <cassert>

//____________________________________________________________________________//

#define CHECK_TOOL_USAGE( tool_usage, check )               \
{                                                           \
    boost::test_tools::output_test_stream output;           \
                                                            \
    unit_test_log.set_stream( output );                     \
    { unit_test_result_saver saver;                         \
      tool_usage;                                           \
    }                                                       \
    unit_test_log.set_stream( std::cout );                  \
    BOOST_CHECK((check));                                   \
}

//____________________________________________________________________________//

#define CHECK_CRITICAL_TOOL_USAGE( tool_usage, nothrow_check, throw_check ) \
{                                                                           \
    boost::test_tools::output_test_stream output;                           \
                                                                            \
    unit_test_log.set_stream( output );                                     \
    try {                                                                   \
        {   unit_test_result_saver saver;                                   \
            tool_usage;                                                     \
        }                                                                   \
        unit_test_log.set_stream( std::cout );                              \
        BOOST_CHECK( nothrow_check );                                       \
    } catch( boost::test_tools::tt_detail::test_tool_failed const&) {       \
        unit_test_log.set_stream( std::cout );                              \
        BOOST_CHECK( throw_check );                                         \
    }                                                                       \
}

//____________________________________________________________________________//

static char qu = '"';

char
set_unix_slash( char in )
{
    return in == '\\' ? '/' : in;
}

static std::string const&
normalize_file_name( char const* f )
{
    static std::string buffer;

    buffer = f;

    std::transform( buffer.begin(), buffer.end(), buffer.begin(), &set_unix_slash );

    return buffer;
}

//____________________________________________________________________________//

#ifdef BOOST_TEST_SHIFTED_LINE

#define CHECK_PATTERN( msg, shift ) \
    (boost::wrap_stringstream().ref() << normalize_file_name( __FILE__ ) << "(" << (__LINE__-shift) << "): " << msg).str()

#else

#define CHECK_PATTERN( msg, shift ) \
    (boost::wrap_stringstream().ref() << normalize_file_name( __FILE__ ) << "(" << __LINE__ << "): " << msg).str()

#endif

//____________________________________________________________________________//

class bool_convertible
{
    struct Tester {};
public:
    operator Tester*() const { return static_cast<Tester*>( 0 ) + 1; }
};

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_WARN )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_WARN" << qu <<

    unit_test_log.set_threshold_level( log_warnings );

    boost::wrap_stringstream ptrn;
    ptrn << normalize_file_name( __FILE__ ) << "(" << (__LINE__+3) << "): "
         << "warning in " TEST_CASE_NAME ": condition sizeof(int) == sizeof(short) is not satisfied\n";

    CHECK_TOOL_USAGE( BOOST_WARN( sizeof(int) == sizeof(short) ), output.is_equal( ptrn.str() ) );

    unit_test_log.set_threshold_level( log_successful_tests );

    CHECK_TOOL_USAGE(
        BOOST_WARN( sizeof(unsigned char) == sizeof(char) ),
        output.is_equal( CHECK_PATTERN( "info: test sizeof(unsigned char) == sizeof(char) passed\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_TOOL_USAGE(
        BOOST_CHECK( true ),
        output.is_empty()
    );

    bool_convertible bc;

    CHECK_TOOL_USAGE(
        BOOST_CHECK( bc ),
        output.is_empty()
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK( false ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test false failed\n", 2 ) )
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK( 1==2 ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test 1==2 failed\n", 2 ) )
    );

    int i=2;

    CHECK_TOOL_USAGE(
        BOOST_CHECK( i==1 ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test i==1 failed\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_successful_tests );
    CHECK_TOOL_USAGE(
        BOOST_CHECK( i==2 ),
        output.is_equal( CHECK_PATTERN( "info: test i==2 passed\n", 2 ) )
    );
    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_REQUIRE )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_REQUIRE" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE( true ),
        true, false
    );

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE( false ),
        false, true
    );

    int j = 3;

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE( j > 5 ),
        false, output.is_equal( CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": critical test j > 5 failed\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_successful_tests );
    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE( j < 5 ),
        output.is_equal( CHECK_PATTERN( "info: test j < 5 passed\n", 1 ) ) , false
    );
    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_WARN_MESSAGE )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_WARN_MESSAGE" << qu <<

    unit_test_log.set_threshold_level( log_warnings );

    CHECK_TOOL_USAGE(
        BOOST_WARN_MESSAGE( sizeof(int) == sizeof(short), "memory won't be used efficiently" ),
        output.is_equal( CHECK_PATTERN( "warning in " TEST_CASE_NAME ": memory won't be used efficiently\n", 2 ) )
    );

    int obj_size = 33;

    CHECK_TOOL_USAGE(
        BOOST_WARN_MESSAGE( obj_size <= 8, "object size " << obj_size << " is too big to be efficiently passed by value" ),
        output.is_equal( CHECK_PATTERN( "warning in " TEST_CASE_NAME
                                        ": object size 33 is too big to be efficiently passed by value\n", 3 ) )
    );

    unit_test_log.set_threshold_level( log_successful_tests );
    CHECK_TOOL_USAGE(
        BOOST_WARN_MESSAGE( obj_size > 8, "object size " << obj_size << " is too small" ),
        output.is_equal( CHECK_PATTERN( "info: object size 33 is too small\n", 2 ) )
    );
    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

boost::test_tools::predicate_result
test_pred1()
{
    boost::test_tools::predicate_result res( false );

    res.message() << "Some explanation";

    return res;
}

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK_MESSAGE )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK_MESSAGE" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_MESSAGE( 2+2 == 5, "Well, may be that what I belive in" ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": Well, may be that what I belive in\n", 2 ) )
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_MESSAGE( test_pred1(), "Checking predicate failed" ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": Checking predicate failed. Some explanation\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_successful_tests );
    CHECK_TOOL_USAGE(
        BOOST_CHECK_MESSAGE( 2+2 == 4, "Could it fail?" ),
        output.is_equal( CHECK_PATTERN( "info: Could it fail?\n", 2 ) )
    );
    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_REQUIRE_MESSAGE )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_REQUIRE_MESSAGE" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_MESSAGE( false, "Here we should stop" ),
        false, output.is_equal(
            CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": Here we should stop" << "\n", 3 ) )
    );

    unit_test_log.set_threshold_level( log_successful_tests );
    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_MESSAGE( true, "That's OK" ),
        output.is_equal( CHECK_PATTERN( "info: That's OK\n", 1 ) ),
        false
    );
    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_ERROR )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_ERROR" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_TOOL_USAGE(
        BOOST_ERROR( "Fail to miss an error" ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": Fail to miss an error\n", 2 ) )
    );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_FAIL )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_FAIL" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_FAIL( "No! No! Show must go on." ),
        false, output.is_equal( CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": No! No! Show must go on.\n", 2 ) )
    );
}

//____________________________________________________________________________//

struct my_exception {
    explicit my_exception( int ec = 0 ) : m_error_code( ec ) {}
    
    int m_error_code;
};

bool is_critical( my_exception const& ex ) { return ex.m_error_code < 0; }

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK_THROW )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK_THROW" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    int i=0;
    CHECK_TOOL_USAGE(
        BOOST_CHECK_THROW( i++, my_exception ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": exception my_exception is expected\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_warnings );

    CHECK_TOOL_USAGE(
        BOOST_WARN_THROW( i++, my_exception ),
        output.is_equal( CHECK_PATTERN( "warning in " TEST_CASE_NAME ": exception my_exception is expected\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_THROW( i++, my_exception ),
        false,
        output.is_equal( CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": exception my_exception is expected\n", 3 ) )
    );
    unit_test_log.set_threshold_level( log_successful_tests );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_THROW( throw my_exception(), my_exception ),
        output.is_equal( CHECK_PATTERN( "info: exception my_exception is caught\n", 2 ) )
        );

    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK_EXCEPTION )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK_EXCEPTION" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EXCEPTION( throw my_exception( 1 ), my_exception, is_critical ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": incorrect exception my_exception is caught\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_successful_tests );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EXCEPTION( throw my_exception( -1 ), my_exception, is_critical ),
        output.is_equal( CHECK_PATTERN( "info: incorrect exception my_exception is caught\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK_NO_THROW )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK_NO_THROW" << qu <<

    int i=0;
    CHECK_TOOL_USAGE(
        BOOST_CHECK_NO_THROW( i++ ),
        output.is_empty() 
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_NO_THROW( throw my_exception() ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": exception thrown by throw my_exception()\n", 2 ) )
    );
}

//____________________________________________________________________________//

struct B {
    B( int i ) : m_i( i ) {}

    int m_i;
};

bool          operator==( B const& b1, B const& b2 ) { return b1.m_i == b2.m_i; }
std::ostream& operator<<( std::ostream& str, B const& b ) { return str << "B(" << b.m_i << ")"; }

//____________________________________________________________________________//

struct C {
    C( int i, int id ) : m_i( i ), m_id( id ) {}

    int m_i;
    int m_id;
};

boost::test_tools::predicate_result
operator==( C const& c1, C const& c2 )
{
    boost::test_tools::predicate_result res( c1.m_i == c2.m_i && c1.m_id == c2.m_id );

    if( !res ) {
        if( c1.m_i != c2.m_i )
            res.message() << "Index mismatch";
        else
            res.message() << "Id mismatch";
    }

    return res;
}

std::ostream& operator<<( std::ostream& str, C const& c ) { return str << "C(" << c.m_i << ',' << c.m_id << ")"; }

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK_EQUAL )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK_EQUAL" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    int i=1;
    int j=2;

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL( i, j ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test i == j failed [1 != 2]\n", 2 ) )
    );

    char const* str1 = "test1";
    char const* str2 = "test12";

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL( str1, str2 ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test str1 == str2 failed [test1 != test12]\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_successful_tests );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL( i+1, j ),
        output.is_equal( CHECK_PATTERN( "info: test i+1 == j passed\n", 2 ) )
    );

    char const* str3 = "1test1";

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL( str1, str3+1 ),
        output.is_equal( CHECK_PATTERN( "info: test str1 == str3+1 passed\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_all_errors );

    str1 = NULL;
    str2 = NULL;

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL( str1, str2 ),
        output.is_empty()
    );

    str1 = "test";
    str2 = NULL;

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_EQUAL( str1, str2 ),
        false,
        output.is_equal( CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": critical test str1 == str2 failed [test != null string]\n", 3 ) )
    );

    B b1(1);
    B b2(2);

    unit_test_log.set_threshold_level( log_warnings );

    CHECK_TOOL_USAGE(
        BOOST_WARN_EQUAL( b1, b2 ),
        output.is_equal( CHECK_PATTERN( "warning in " TEST_CASE_NAME ": condition b1 == b2 is not satisfied [B(1) != B(2)]\n", 2 ) )
    );

    unit_test_log.set_threshold_level( log_all_errors );

    C c1( 0, 100 );
    C c2( 0, 101 );
    C c3( 1, 102 );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL( c1, c3 ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test c1 == c3 failed [C(0,100) != C(1,102)]. Index mismatch\n", 2 ) )
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL( c1, c2 ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test c1 == c2 failed [C(0,100) != C(0,101)]. Id mismatch\n", 2 ) )
    );
}

//____________________________________________________________________________//

bool is_even( int i )        { return i%2 == 0;  }
int  foo( int arg, int mod ) { return arg % mod; }
bool moo( int arg1, int arg2, int mod ) { return ((arg1+arg2) % mod) == 0; }

BOOST_TEST_DONT_PRINT_LOG_VALUE( std::list<int> )

boost::test_tools::predicate_result
compare_lists( std::list<int> const& l1, std::list<int> const& l2 )
{
    if( l1.size() != l2.size() ) {
        boost::test_tools::predicate_result res( false );

        res.message() << "Different sizes [" << l1.size() << "!=" << l2.size() << "]";

        return res;
    }

    return true;
}

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK_PREDICATE )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK_PREDICATE" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( is_even, (14) ),
        output.is_empty()
    );

    int i = 17;
    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( is_even, (i) ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test is_even( i ) failed for ( 17 )\n", 2 ) )
    );

    using std::not_equal_to;

    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( not_equal_to<int>(), (i)(17) ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test not_equal_to<int>()( i, 17 ) failed for ( 17, 17 )\n", 2 ) )
    );

    int j=15;
    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( boost::bind( is_even, boost::bind( &foo, _1, _2 ) ), (i)(j) ),
        output.is_empty()
    );

    unit_test_log.set_threshold_level( log_warnings );

    CHECK_TOOL_USAGE(
        BOOST_WARN_PREDICATE( moo, (12)(i)(j) ),
        output.is_equal( CHECK_PATTERN( 
            "warning in " TEST_CASE_NAME ": condition moo( 12, i, j ) is not satisfied for ( 12, 17, 15 )\n", 3 ) )
    );

    unit_test_log.set_threshold_level( log_all_errors );

    std::list<int> l1, l2, l3;
    l1.push_back( 1 );
    l3.push_back( 1 );
    l1.push_back( 2 );
    l3.push_back( 3 );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( compare_lists, (l1)(l2) ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test compare_lists( l1, l2 ) failed for ( ,  ). Different sizes [2!=0]\n", 2 ) )
    );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_REQUIRE_PREDICATE )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_REQUIRE_PREDICATE" << qu <<

    int arg1 = 1;
    int arg2 = 2;

    using std::less_equal;

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_PREDICATE( less_equal<int>(), (arg1)(arg2) ),
        output.is_empty(), false
    );

   CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_PREDICATE( less_equal<int>(), (arg2)(arg1) ),
        false, output.is_equal( CHECK_PATTERN( 
                    "fatal error in " TEST_CASE_NAME ": critical test less_equal<int>()( arg2, arg1 ) "
                    "failed for ( " << arg2 << ", " << arg1 << " )\n", 4 ) )
    );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK_EQUAL_COLLECTIONS )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK_EQUAL_COLLECTIONS" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    int pattern [] = { 1, 2, 3, 4, 5, 6, 7 };

    std::list<int> testlist;

    testlist.push_back( 1 );
    testlist.push_back( 2 );
    testlist.push_back( 4 ); // 3
    testlist.push_back( 4 );
    testlist.push_back( 5 );
    testlist.push_back( 7 ); // 6
    testlist.push_back( 7 );

    boost::wrap_stringstream ptrn;

    ptrn << "error in " TEST_CASE_NAME ": test { testlist.begin(), testlist.end() } == { pattern, pattern+7 } failed. "
         << "\nMismatch in a position 2: 4 != 3"
         << "\nMismatch in a position 5: 7 != 6\n";

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL_COLLECTIONS( testlist.begin(), testlist.end(), pattern, pattern+7 ),
            output.is_equal( CHECK_PATTERN( ptrn.str(), 2 ) )
    );

    boost::wrap_stringstream ptrn1;

    ptrn1 << "error in " TEST_CASE_NAME ": test { testlist.begin(), testlist.end() } == { pattern, pattern+2 } failed. "
          << "\nCollections size mismatch: 7 != 2\n";

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL_COLLECTIONS( testlist.begin(), testlist.end(), pattern, pattern+2 ),
        output.is_equal( CHECK_PATTERN( ptrn1.str(), 2 ) )
    );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECK_BITWISE_EQUAL )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECK_BITWISE_EQUAL" << qu <<

    CHECK_TOOL_USAGE(
        BOOST_CHECK_BITWISE_EQUAL( 0x16, 0x16 ),
        output.is_empty()
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_BITWISE_EQUAL( (char)0x06, (char)0x16 ),
        output.is_equal( 
          CHECK_PATTERN( "error in " TEST_CASE_NAME ": test (char)0x06 =.= (char)0x16 failed. \nMismatch in a position 4\n", 3 ) )
    );

    unit_test_log.set_threshold_level( log_warnings );

    CHECK_TOOL_USAGE(
        BOOST_WARN_BITWISE_EQUAL( (char)0x26, (char)0x04 ),
        output.is_equal( 
            CHECK_PATTERN( "warning in " TEST_CASE_NAME ": condition (char)0x26 =.= (char)0x04 is not satisfied. "
                           "\nMismatch in a position 1"
                           "\nMismatch in a position 5\n", 5 ) )
    );

    unit_test_log.set_threshold_level( log_all_errors );

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_BITWISE_EQUAL( (char)0x26, (int)0x26 ),
        false,
        output.is_equal( 
            CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": critical test (char)0x26 =.= (int)0x26 failed. "
                            "\nOperands bit sizes mismatch: 8 != 32\n", 5 ) )
    );

    unit_test_log.set_threshold_level( log_all_errors );
}

//____________________________________________________________________________//

struct A {
    friend std::ostream& operator<<( std::ostream& str, A const& a ) { str << "struct A"; return str;}
};

BOOST_AUTO_UNIT_TEST( test_BOOST_MESSAGE )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_REQUIRE" << qu <<

    unit_test_log.set_threshold_level( log_messages );

    CHECK_TOOL_USAGE(
        BOOST_MESSAGE( "still testing" ),
        output.is_equal( "still testing\n" )
    );

    CHECK_TOOL_USAGE(
        BOOST_MESSAGE( "1+1=" << 2 ),
        output.is_equal( "1+1=2\n" )
    );

    int i = 2;
    CHECK_TOOL_USAGE(
        BOOST_MESSAGE( i << "+" << i << "=" << (i+i) ),
        output.is_equal( "2+2=4\n" )
    );

    A a = A();
    CHECK_TOOL_USAGE(
        BOOST_MESSAGE( a ),
        output.is_equal( "struct A\n" )
    );

#if !defined(BOOST_NO_STD_LOCALE) && ( !defined(BOOST_MSVC) || BOOST_WORKAROUND(BOOST_MSVC, >= 1310))

    CHECK_TOOL_USAGE(
        BOOST_MESSAGE( std::hex << std::showbase << 20 ),
        output.is_equal( "0x14\n" )
    );

#endif

    CHECK_TOOL_USAGE(
        BOOST_MESSAGE( std::setw( 4 ) << 20 ),
        output.is_equal( "  20\n" )
        );
}

//____________________________________________________________________________//

class bad_func_container : public test_case
{
public:
    bad_func_container() : test_case( "test_BOOST_CHECKPOINT", true, 1 ) {}
    void do_run() {
        throw "some error";
    }
} bad;

BOOST_AUTO_UNIT_TEST( test_BOOST_CHECKPOINT )
{
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << qu << "test_BOOST_CHECKPOINT" << qu <<

    unit_test_log.set_threshold_level( log_all_errors );

    BOOST_CHECKPOINT( "Going to do a silly things" );

    boost::wrap_stringstream ptrn;
    ptrn << "Exception in " TEST_CASE_NAME ": C string: some error\n"
         << normalize_file_name( __FILE__ ) << "(" << (__LINE__ - 4) << "): "
         << "last checkpoint: Going to do a silly things\n";

    CHECK_TOOL_USAGE(
        bad.run(),
        output.is_equal( ptrn.str() )
    );
}

//____________________________________________________________________________//

BOOST_AUTO_UNIT_TEST( test_BOOST_IS_DEFINED )
{
#define SYMBOL1
#define SYMBOL2 std::cout
#define ONE_ARG( arg ) arg
#define TWO_ARG( arg1, arg2 ) BOOST_JOIN( arg1, arg2 )

    BOOST_CHECK( BOOST_IS_DEFINED(SYMBOL1) );
    BOOST_CHECK( BOOST_IS_DEFINED(SYMBOL2) );
    BOOST_CHECK( !BOOST_IS_DEFINED( SYMBOL3 ) );
    BOOST_CHECK( BOOST_IS_DEFINED( ONE_ARG(arg1) ) );
    BOOST_CHECK( !BOOST_IS_DEFINED( ONE_ARG1(arg1) ) );
    BOOST_CHECK( BOOST_IS_DEFINED( TWO_ARG(arg1,arg2) ) );
    BOOST_CHECK( !BOOST_IS_DEFINED( TWO_ARG1(arg1,arg2) ) );
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.38  2005/01/31 06:04:31  rogeeff
//  workaround fixed
//
//  Revision 1.37  2005/01/30 03:35:55  rogeeff
//  no message
//
//  Revision 1.35  2005/01/18 08:30:09  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
// ***************************************************************************

// EOF
