//  (C) Copyright Gennadiy Rozental 2001-2002.
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
//  Description : tests all Test Tools but output_test_stream
// ***************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_result.hpp>
#include <boost/test/floating_point_comparison.hpp>
using namespace boost::unit_test_framework;

// BOOST
#include <boost/compose.hpp>

// STL
#include <iostream>
#include <list>
#include <typeinfo>

//____________________________________________________________________________//

#define CHECK_TOOL_USAGE( tool_usage, check )               \
{                                                           \
    boost::test_toolbox::output_test_stream output;         \
                                                            \
    unit_test_log::instance().set_log_stream( output );     \
    unit_test_result::reset_current_result_set();           \
    tool_usage;                                             \
                                                            \
    unit_test_result::reset_current_result_set();           \
    unit_test_log::instance().set_log_stream( std::cout );  \
    BOOST_CHECK( check );                                   \
}

//____________________________________________________________________________//

#define CHECK_CRITICAL_TOOL_USAGE( tool_usage, nothrow_check, throw_check ) \
{                                                                           \
    boost::test_toolbox::output_test_stream output;                         \
                                                                            \
    unit_test_log::instance().set_log_stream( output );                     \
    unit_test_result::reset_current_result_set();                           \
    try {                                                                   \
        tool_usage;                                                         \
        unit_test_log::instance().set_log_stream( std::cout );              \
        unit_test_result::reset_current_result_set();                       \
        BOOST_CHECK( nothrow_check );                                       \
    } catch( boost::test_toolbox::detail::test_tool_failed const&) {        \
        unit_test_log::instance().set_log_stream( std::cout );              \
        unit_test_result::reset_current_result_set();                       \
        BOOST_CHECK( throw_check );                                         \
    }                                                                       \
}

//____________________________________________________________________________//

#if !defined(__BORLANDC__)
#define CHECK_PATTERN( msg, shift ) \
    (boost::test_toolbox::detail::wrapstrstream() << __FILE__ << "(" << __LINE__ << "): " << msg).str()

#else

#define CHECK_PATTERN( msg, shift ) \
    (boost::test_toolbox::detail::wrapstrstream() << __FILE__ << "(" << (__LINE__-shift) << "): " << msg).str()

#endif
//____________________________________________________________________________//

class bool_convertible
{
    struct Tester {};
public:
    operator Tester*() { return static_cast<Tester*>( 0 ) + 1; }
};

void
test_BOOST_CHECK() {
#define TEST_CASE_NAME << '\"' << "test_BOOST_CHECK" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );

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

    unit_test_log::instance().set_log_threshold_level( report_successful_tests );
    CHECK_TOOL_USAGE(
        BOOST_CHECK( i==2 ),
        output.is_equal( CHECK_PATTERN( "info: test i==2 passed\n", 2 ) )
    );
    unit_test_log::instance().set_log_threshold_level( report_all_errors );
}

//____________________________________________________________________________//

void
test_BOOST_REQUIRE() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_REQUIRE" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );

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
        false, output.is_equal( CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": test j > 5 failed\n", 2 ) )
    );

    unit_test_log::instance().set_log_threshold_level( report_successful_tests );
    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE( j < 5 ),
        output.is_equal( CHECK_PATTERN( "info: test j < 5 passed\n", 1 ) ) , false
    );
    unit_test_log::instance().set_log_threshold_level( report_all_errors );

}

//____________________________________________________________________________//

struct A {
    friend std::ostream& operator<<( std::ostream& str, A const& a ) { str << "struct A"; return str;}
};

void
test_BOOST_MESSAGE() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_REQUIRE" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_messages );

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
}

//____________________________________________________________________________//

void
test_BOOST_WARN() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_WARN" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_warnings );

    CHECK_TOOL_USAGE(
        BOOST_WARN( sizeof(int) == sizeof(short) ),
        output.is_equal( CHECK_PATTERN( "warning in " TEST_CASE_NAME
                                        ": condition sizeof(int) == sizeof(short) is not satisfied\n", 3 ) )
    );
}

//____________________________________________________________________________//

class bad_func_container : public test_case
{
public:
    bad_func_container() : test_case( "test_BOOST_CHECKPOINT", 1 ) {}
    void do_run() {
        throw "some error";
    }
} bad;

void
test_BOOST_CHECKPOINT() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_CHECKPOINT" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );

    BOOST_CHECKPOINT( "Going to do a silly things" );

    CHECK_TOOL_USAGE(
        bad.run(),
        output.is_equal(
            (boost::test_toolbox::detail::wrapstrstream()
                << "Exception in " TEST_CASE_NAME ": C string: some error\n"
                << __FILE__ << "(" << (__LINE__ - 10) << ") : "
                << "last checkpoint: Going to do a silly things\n").str()
        )
    );
}

//____________________________________________________________________________//

void
test_BOOST_WARN_MESSAGE() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_WARN_MESSAGE" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_warnings );

    CHECK_TOOL_USAGE(
        BOOST_WARN_MESSAGE( sizeof(int) == sizeof(short), "memory won't be used efficiently" ),
        output.is_equal( CHECK_PATTERN( "warning in " TEST_CASE_NAME ": memory won't be used efficiently\n", 2 ) )
    );

    int obj_size = 33;

    CHECK_TOOL_USAGE(
        BOOST_WARN_MESSAGE( obj_size <= 8, "object size " << obj_size << " too big to be efficiently passed by value" ),
        output.is_equal( CHECK_PATTERN( "warning in " TEST_CASE_NAME
                                        ": object size 33 too big to be efficiently passed by value\n", 3 ) )
    );

}

//____________________________________________________________________________//

void
test_BOOST_CHECK_MESSAGE() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_CHECK_MESSAGE" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );


    CHECK_TOOL_USAGE(
        BOOST_CHECK_MESSAGE( 2+2 == 5, "Well, may be that what I belive in" ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": Well, may be that what I belive in\n", 2 ) )
    );

}

//____________________________________________________________________________//

void
test_BOOST_REQUIRE_MESSAGE() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_REQUIRE_MESSAGE" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_MESSAGE( false, "Here we should stop" ),
        false, output.is_equal(
            CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": Here we should stop" << "\n", 3 ) )
    );
}

//____________________________________________________________________________//

struct B
{
    B( int i ) : m_i( i ) {}

    friend bool operator==( B const& b1, B const& b2 ) { return b1.m_i == b2.m_i; }
    friend std::ostream& operator<<( std::ostream& str, B const& b ) { str << "B(" << b.m_i << ")"; return str; }

    int m_i;
};

void
test_BOOST_CHECK_EQUAL() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_CHECK_EQUAL" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );

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

    B b1(1);
    B b2(2);

    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL( b1, b2 ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test b1 == b2 failed [B(1) != B(2)]\n", 2 ) )
    );
}

//____________________________________________________________________________//

template<typename FPT>
void
test_BOOST_CHECK_CLOSE( FPT ) {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_CHECK_CLOSE_all" << "\"" <<
    unit_test_log::instance().set_log_threshold_level( report_messages );

    BOOST_MESSAGE( "testing BOOST_CHECK_CLOSE for " << typeid(FPT).name() );


#define BOOST_CHECK_CLOSE_SHOULD_PASS( first, second, e )       \
    fp1     = static_cast<FPT>(first);                          \
    fp2     = static_cast<FPT>(second);                         \
    epsilon = static_cast<FPT>(e);                              \
                                                                \
    CHECK_TOOL_USAGE(                                           \
        BOOST_CHECK_CLOSE( fp1, fp2, epsilon ),                 \
        output.is_empty()                                       \
    )

#define BOOST_CHECK_CLOSE_SHOULD_PASS_N( first, second, num )   \
    fp1     = static_cast<FPT>(first);                          \
    fp2     = static_cast<FPT>(second);                         \
                                                                \
    CHECK_TOOL_USAGE(                                           \
        BOOST_CHECK_CLOSE( fp1, fp2, (num) ),                   \
        output.is_empty()                                       \
    )

#define BOOST_CHECK_CLOSE_SHOULD_FAIL( first, second, e )       \
    fp1     = static_cast<FPT>(first);                          \
    fp2     = static_cast<FPT>(second);                         \
    epsilon = static_cast<FPT>(e);                              \
                                                                \
    CHECK_TOOL_USAGE(                                           \
        BOOST_CHECK_CLOSE( fp1, fp2, epsilon ),                 \
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test fp1 ~= fp2 failed [" \
                                        << fp1 << " !~= " << fp2 << " (+/-" << epsilon << ")]\n", 0 ) ) \
    )

#define BOOST_CHECK_CLOSE_SHOULD_FAIL_N( first, second, num )   \
    fp1     = static_cast<FPT>(first);                          \
    fp2     = static_cast<FPT>(second);                         \
    epsilon = num * std::numeric_limits<FPT>::epsilon()/2;      \
                                                                \
    CHECK_TOOL_USAGE(                                           \
        BOOST_CHECK_CLOSE( fp1, fp2, num ),                     \
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test fp1 ~= fp2 failed [" \
        << fp1 << " !~= " << fp2 << " (+/-" << epsilon << ")]\n", 0 ) ) \
    )

    FPT fp1, fp2, epsilon, tmp;

    BOOST_CHECK_CLOSE_SHOULD_PASS( 1, 1, 0 );

    BOOST_CHECK_CLOSE_SHOULD_FAIL( 0, 1e-20, 1e-7 );
    BOOST_CHECK_CLOSE_SHOULD_FAIL( 0, 1e-30, 1e-7 );
    BOOST_CHECK_CLOSE_SHOULD_FAIL( 0, -1e-10, 1e-3 );
    BOOST_CHECK_CLOSE_SHOULD_FAIL( 0.123456, 0.123457, 1e-6 );

    BOOST_CHECK_CLOSE_SHOULD_PASS( 0.123456, 0.123457, 1e-5 );

    BOOST_CHECK_CLOSE_SHOULD_FAIL( 0.123456, -0.123457, 1e-5 );

    BOOST_CHECK_CLOSE_SHOULD_PASS( 1.23456e28, 1.23457e28, 1e-5 );

    BOOST_CHECK_CLOSE_SHOULD_FAIL( 1.23456e-10, 1.23457e-11, 1e-5 );
    BOOST_CHECK_CLOSE_SHOULD_FAIL( 1.111e-10, 1.112e-10, 0.0008999 );
    BOOST_CHECK_CLOSE_SHOULD_FAIL( 1.112e-10, 1.111e-10, 0.0008999 );

    BOOST_CHECK_CLOSE_SHOULD_PASS( 1     , 1.0001, 1.1e-4 );
    BOOST_CHECK_CLOSE_SHOULD_PASS( 1.0002, 1.0001, 1.1e-4 );
    
    BOOST_CHECK_CLOSE_SHOULD_FAIL( 1     , 1.0002, 1.1e-4 );

    BOOST_CHECK_CLOSE_SHOULD_PASS_N( 1, 1+std::numeric_limits<FPT>::epsilon() / 2, 1 );
    
    tmp = static_cast<FPT>(1e-10);
    BOOST_CHECK_CLOSE_SHOULD_PASS_N( tmp+tmp, 2e-10, 1+2 );

    tmp = static_cast<FPT>(3.1);
    BOOST_CHECK_CLOSE_SHOULD_PASS_N( tmp*tmp, 9.61, 1+2 );

    tmp = 11;
    tmp /= 10;
    BOOST_CHECK_CLOSE_SHOULD_PASS_N( (tmp*tmp-tmp), 11./100, 1+3 );
    BOOST_CHECK_CLOSE_SHOULD_FAIL_N( 100*(tmp*tmp-tmp), 11, 3 );

    tmp = static_cast<FPT>(1e15+1e-10);
    BOOST_CHECK_CLOSE_SHOULD_PASS_N( tmp*tmp+tmp*tmp, 2e30+2e-20+4e5, 3+5 );

    fp1     = static_cast<FPT>(1.0001);
    fp2     = static_cast<FPT>(1001.1001);
    tmp     = static_cast<FPT>(1.0001);

    for( int i=0; i < 1000; i++ )
        fp1 = fp1 + tmp;

    CHECK_TOOL_USAGE(
        BOOST_CHECK_CLOSE( fp1, fp2, 1000 ),
        output.is_empty()
    );
}

void
test_BOOST_CHECK_CLOSE_all() {
    test_BOOST_CHECK_CLOSE<float>( (float)0 );
    test_BOOST_CHECK_CLOSE<double>( (double)0 );
    test_BOOST_CHECK_CLOSE<long double>( (long double)0 );
}

//____________________________________________________________________________//

bool is_even( int i ) {
    return i%2 == 0;
}

int foo( int arg, int mod ) 
{
    return arg % mod;
}

void
test_BOOST_CHECK_PREDICATE() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_CHECK_PREDICATE" << '\"' <<

    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( &is_even, 1, (14) ),
        output.is_empty()
    );

    int i = 17;
    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( &is_even, 1, (i) ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test &is_even(i) failed for 17\n", 2 ) )
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( std::not_equal_to<int>(), 2, (i,17) ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test std::not_equal_to<int>()(i, 17) failed for (17, 17)\n", 2 ) )
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( boost::compose_f_gxy( std::ptr_fun( &is_even ), std::ptr_fun( &foo ) ), 2, (i,15) ),
        output.is_empty()
    );

    double fp1     = 1.00000001;
    double fp2     = 1.00000002;
    double epsilon = 1e-8;

    CHECK_TOOL_USAGE(
        BOOST_CHECK_PREDICATE( close_at_tolerance<double>( epsilon, false ), 2, ( fp1, fp2 ) ),
        output.is_empty()
    );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_CLOSE( fp1, fp2, epsilon ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": test fp1 ~= fp2 failed [" 
                                        << fp1 << " !~= " << fp2 << " (+/-" << epsilon << ")]\n", 3 ) )
    );
}

//____________________________________________________________________________//

void
test_BOOST_REQUIRE_PREDICATE() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_REQUIRE_PREDICATE" << '\"' <<

    double fp1;
    double fp2;
    double epsilon;

    fp1     = 1.23456e-10;
    fp2     = 1.23457e-10;
    epsilon = 8.1e-6;

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_PREDICATE( close_at_tolerance<double>( epsilon, false ), 2, ( fp1, fp2 ) ),
        output.is_empty(), false
    );

   CHECK_CRITICAL_TOOL_USAGE(
        BOOST_REQUIRE_PREDICATE( close_at_tolerance<double>( epsilon ), 2, ( fp1, fp2 ) ),
        false, output.is_equal( CHECK_PATTERN( 
                    "fatal error in " TEST_CASE_NAME ": test close_at_tolerance<double>( epsilon )(fp1, fp2) "
                    "failed for (" << fp1 << ", " << fp2 << ")\n", 4 ) )
    );
}

//____________________________________________________________________________//

void
test_BOOST_ERROR() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_ERROR" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );

    CHECK_TOOL_USAGE(
        BOOST_ERROR( "Fail to miss an error" ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": Fail to miss an error\n", 2 ) )
    );

    CHECK_CRITICAL_TOOL_USAGE(
        BOOST_FAIL( "No! No! Show must go on." ),
        false, output.is_equal( CHECK_PATTERN( "fatal error in " TEST_CASE_NAME ": No! No! Show must go on.\n", 2 ) )
    );
}

//____________________________________________________________________________//

class my_exception {};

void
test_BOOST_CHECK_THROW() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_CHECK_THROW" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );

    int i=0;
    CHECK_TOOL_USAGE(
        BOOST_CHECK_THROW( i++, my_exception ),
        output.is_equal( CHECK_PATTERN( "error in " TEST_CASE_NAME ": exception my_exception is expected\n", 2 ) )
    );

    unit_test_log::instance().set_log_threshold_level( report_successful_tests );

    CHECK_TOOL_USAGE(
        BOOST_CHECK_THROW( throw my_exception(), my_exception ),
        output.is_equal( CHECK_PATTERN( "info: exception my_exception is caught\n", 2 ) )
    );

    unit_test_log::instance().set_log_threshold_level( report_all_errors );
}

//____________________________________________________________________________//

void
test_BOOST_CHECK_EQUAL_COLLECTIONS() {
#undef  TEST_CASE_NAME
#define TEST_CASE_NAME << '\"' << "test_BOOST_CHECK_EQUAL_COLLECTIONS" << '\"' <<

    unit_test_log::instance().set_log_threshold_level( report_all_errors );

    int pattern [] = { 1, 2, 3, 4, 5, 6, 7 };

    std::list<int> testlist;

    testlist.push_back( 1 );
    testlist.push_back( 2 );
    testlist.push_back( 4 ); // 3
    testlist.push_back( 4 );
    testlist.push_back( 5 );
    testlist.push_back( 7 ); // 6
    testlist.push_back( 7 );

#if !defined(__BORLANDC__)
    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL_COLLECTIONS( testlist.begin(), testlist.end(), pattern ),
            output.is_equal( CHECK_PATTERN( 
              "error in " TEST_CASE_NAME ": test {testlist.begin(), testlist.end()} == {pattern, ...} failed [4 != 3]\n"
              __FILE__ << "(" << __LINE__ << "): " << 
              "error in " TEST_CASE_NAME ": test {testlist.begin(), testlist.end()} == {pattern, ...} failed [7 != 6]\n"
              , 6 ) )
    );
#else
    CHECK_TOOL_USAGE(
        BOOST_CHECK_EQUAL_COLLECTIONS( testlist.begin(), testlist.end(), pattern ),
            output.is_equal( CHECK_PATTERN( 
              "error in " TEST_CASE_NAME ": test {testlist.begin(), testlist.end()} == {pattern, ...} failed [4 != 3]\n"
              __FILE__ << "(" << (__LINE__-6) << "): " << 
              "error in " TEST_CASE_NAME ": test {testlist.begin(), testlist.end()} == {pattern, ...} failed [7 != 6]\n"
              , 6 ) )
    );
#endif

}

//____________________________________________________________________________//

void
test_BOOST_IS_DEFINED() {
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

test_suite*
init_unit_test_suite( int argc, char* argv[] ) {
    test_suite* test = BOOST_TEST_SUITE("Test Tools test");

    test->add( BOOST_TEST_CASE( &test_BOOST_CHECK ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_REQUIRE ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_MESSAGE ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_WARN ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_CHECKPOINT ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_WARN_MESSAGE ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_CHECK_MESSAGE ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_REQUIRE_MESSAGE ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_CHECK_EQUAL ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_CHECK_CLOSE_all ), 
#if defined(__BORLANDC__) || defined(__GNUC__)
        6
#elif defined (_MSC_VER)
        2
#else
        0
#endif
);
    test->add( BOOST_TEST_CASE( &test_BOOST_ERROR ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_CHECK_THROW ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_CHECK_EQUAL_COLLECTIONS ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_IS_DEFINED ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_CHECK_PREDICATE ) );
    test->add( BOOST_TEST_CASE( &test_BOOST_REQUIRE_PREDICATE ) );

    return test;
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.12  2002/11/02 20:04:43  rogeeff
//  release 1.29.0 merged into the main trank
//

// ***************************************************************************

// EOF
