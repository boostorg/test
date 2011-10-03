//  (C) Copyright Gennadiy Rozental 2003-2010.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 62023 $
//
//  Description : unit test for class properties facility
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE Boost.Test run by name/label implementation test
#include <boost/test/unit_test.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/tree/test_case_counter.hpp>
#include <boost/test/tree/traverse.hpp>

namespace utf = boost::unit_test;

void A() {}
void B() {}
void C() {}
void D() {}
void E() {}
void F() {}
void longnameA() {}
void Blongname() {}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_run_by_name )
{
    utf::test_suite* master_ts = BOOST_TEST_SUITE("local master");

    utf::test_suite* ts1 = BOOST_TEST_SUITE("ts1");
    ts1->add( BOOST_TEST_CASE( &C ) );
    ts1->add( BOOST_TEST_CASE( &D ) );

    utf::test_suite* ts2 = BOOST_TEST_SUITE("ts2");
    ts2->add( BOOST_TEST_CASE( &A ) );
    ts2->add( BOOST_TEST_CASE( &C ) );
    ts2->add( BOOST_TEST_CASE( &longnameA ) );
    ts2->add( ts1 );

    master_ts->add( BOOST_TEST_CASE( &A ) );
    master_ts->add( BOOST_TEST_CASE( &B ) );
    master_ts->add( BOOST_TEST_CASE( &longnameA ) );
    master_ts->add( BOOST_TEST_CASE( &Blongname ) );
    master_ts->add( ts2 );

    {
        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 9U );
    }

    {
        char* argv[] = { "a.exe", "--run=*" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 9U );
    }

    {
        char* argv[] = { "a.exe", "--run=*/*" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 9U );
    }

    {
        char* argv[] = { "a.exe", "--run=*/*/*" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 9U );
    }

    {
        char* argv[] = { "a.exe", "--run=klmn" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 0U );
    }

    {
        char* argv[] = { "a.exe", "--run=A" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 1U );
    }

    {
        char* argv[] = { "a.exe", "--run=*A" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=B*" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=*ngn*" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 5U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/*" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 5U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/C" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 1U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/*A" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/ts1" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/ts1/C" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 1U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/ts1/*D*" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 1U );
    }

    {
        char* argv[] = { "a.exe", "--run=A,B" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=*A,B" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 3U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/C,ts1" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 3U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/C,ts1/D" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=A", "--run=B" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

        {
        char* argv[] = { "a.exe", "--run=A", "--run=ts2/ts1/C" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_run_by_label )
{
    utf::test_case* tc; 

    utf::test_suite* master_ts = BOOST_TEST_SUITE("local master");

    utf::test_suite* ts1 = BOOST_TEST_SUITE("ts1");
    ts1->add_label( "f2" );
    ts1->add( tc=BOOST_TEST_CASE( &C ) );
    tc->add_label( "L1" );
    ts1->add( tc=BOOST_TEST_CASE( &D ) );
    tc->add_label( "L1" );
    tc->add_label( "l2" );

    utf::test_suite* ts2 = BOOST_TEST_SUITE("ts2");
    ts2->add_label( "FAST" );
    ts2->add( tc=BOOST_TEST_CASE( &A ) );
    tc->add_label( "L1" );
    ts2->add( BOOST_TEST_CASE( &C ) );
    ts2->add( BOOST_TEST_CASE( &longnameA ) );
    ts2->add( ts1 );

    master_ts->add( BOOST_TEST_CASE( &A ) );
    master_ts->add( BOOST_TEST_CASE( &B ) );
    master_ts->add( tc=BOOST_TEST_CASE( &longnameA ) );
    tc->add_label( "f2" );
    master_ts->add( BOOST_TEST_CASE( &Blongname ) );
    master_ts->add( ts2 );

    {
        char* argv[] = { "a.exe", "--run=@L1" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 3U );
    }

    {
        char* argv[] = { "a.exe", "--run=@l2" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 1U );
    }

    {
        char* argv[] = { "a.exe", "--run=@inval" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 0U );
    }

    {
        char* argv[] = { "a.exe", "--run=@FAST" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 5U );
    }

    {
        char* argv[] = { "a.exe", "--run=@f2" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 3U );
    }

    {
        char* argv[] = { "a.exe", "--run=@L1", "--run=@l2" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 3U );
    }
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_dependency_handling )
{
    utf::test_case* tc;
    utf::test_case* tcB;

    //            M
    //         /  |  \
    //        /   |   \
    //     TS2   TS4  TS3
    //     / \    |   / \
    //    C   D  TS1 E   F
    //           / \
    //          A   B
    //
    //  D => TS1
    //  B => F

    utf::test_suite* master_ts = BOOST_TEST_SUITE("local master");

    utf::test_suite* ts1 = BOOST_TEST_SUITE("ts1");
    ts1->add( BOOST_TEST_CASE( &A ) );
    ts1->add( tcB=BOOST_TEST_CASE( &B ) );

    utf::test_suite* ts2 = BOOST_TEST_SUITE("ts2");
    ts2->add( BOOST_TEST_CASE( &C ) );
    ts2->add( tc=BOOST_TEST_CASE( &D ) );
    tc->depends_on( ts1 );

    utf::test_suite* ts3 = BOOST_TEST_SUITE("ts3");
    ts3->add( BOOST_TEST_CASE( &E ) );
    ts3->add( tc=BOOST_TEST_CASE( &F ) );
    tcB->depends_on( tc );

    utf::test_suite* ts4 = BOOST_TEST_SUITE("ts4");
    ts4->add( ts1 );

    master_ts->add( ts2 );
    master_ts->add( ts3 );
    master_ts->add( ts4 );

    {
        char* argv[] = { "a.exe", "--run=ts2/C" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 1U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts3" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2/C" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 1U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts4/ts1/B" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 2U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts4/ts1" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 3U );
    }

    {
        char* argv[] = { "a.exe", "--run=ts2" };
        int argc     =  sizeof(argv)/sizeof(char*);

        utf::runtime_config::init( argc, argv );
        utf::framework::impl::apply_filters( master_ts->p_id );

        utf::test_case_counter tcc;
        utf::traverse_test_tree( master_ts->p_id, tcc );
        BOOST_CHECK_EQUAL( tcc.p_count, 5U );
    }
}

//____________________________________________________________________________//

// EOF

