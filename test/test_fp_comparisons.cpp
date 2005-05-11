//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : tests floating point comparison algorithms
// ***************************************************************************

// Boost.Test
#include <boost/test/floating_point_comparison.hpp>
#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/test/output/compiler_log_formatter.hpp>
#include <boost/test/framework.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/output_test_stream.hpp>
#include <boost/test/detail/unit_test_parameters.hpp>
using namespace boost::unit_test;
using namespace boost::test_tools;

// STL
#include <iostream>
#include <stdio.h>

// Boost
#include <boost/mpl/list.hpp>

namespace boost { namespace test_tools { namespace tt_detail {      
template<>                                                          
struct print_log_value<float> {                                     
    void operator()( std::ostream& ostr, float t )
    {
        ostr.precision( 6 ); 

        ostr << t;
    }
};                                                                  
}}}                                                                 

namespace boost { namespace test_tools { namespace tt_detail {      
template<>                                                          
struct print_log_value<double> {                                     
    void operator()( std::ostream& ostr, double t )
    {
        ostr.precision( 10 ); 
        ostr << t;
    }
};                                                                  
}}}                                                                 

namespace boost { namespace test_tools { namespace tt_detail {      
template<>                                                          
struct print_log_value<long double> {                                     
    void operator()( std::ostream& ostr, long double t )
    {
        ostr.precision( 10 ); 
        ostr << t;
    }
};                                                                  
}}}                                                                 

//____________________________________________________________________________//

struct shorten_lf : public boost::unit_test::output::compiler_log_formatter
{
    void    print_prefix( std::ostream& output, boost::unit_test::const_string, std::size_t line )
    {
        output << line << ": ";
    }
};

//____________________________________________________________________________//

std::string match_file_name( "./test_files/test_fp_comparisons.pattern" );
std::string save_file_name( "test_fp_comparisons.pattern" );

output_test_stream& ots()
{
    static boost::shared_ptr<output_test_stream> inst;

    if( !inst ) {
        inst.reset( 
            auto_unit_test_suite()->argc <= 1
                ? new output_test_stream( runtime_config::save_pattern() ? save_file_name : match_file_name, !runtime_config::save_pattern() )
                : new output_test_stream( auto_unit_test_suite()->argv[1], !runtime_config::save_pattern() ) );
    }

    return *inst;
}

//____________________________________________________________________________//

#define TEST_CASE( name )                                   \
void name ## _impl();                                       \
void name ## _impl_defer();                                 \
                                                            \
BOOST_AUTO_TEST_CASE( name )                                \
{                                                           \
    test_case* impl = make_test_case( &name ## _impl, #name ); \
                                                            \
    unit_test_log.set_stream( ots() );                      \
    unit_test_log.set_threshold_level( log_nothing );       \
    unit_test_log.set_formatter( new shorten_lf );          \
    framework::run( impl );                                 \
                                                            \
    unit_test_log.set_threshold_level(                      \
        runtime_config::log_level() != invalid_log_level    \
            ? runtime_config::log_level()                   \
            : log_all_errors );                             \
    unit_test_log.set_format( runtime_config::log_format());\
    unit_test_log.set_stream( std::cout );                  \
    BOOST_CHECK( ots().match_pattern() );                   \
}                                                           \
                                                            \
void name ## _impl()                                        \
{                                                           \
    unit_test_log.set_threshold_level( log_all_errors );    \
                                                            \
    name ## _impl_defer();                                  \
                                                            \
    unit_test_log.set_threshold_level( log_nothing );       \
}                                                           \
                                                            \
void name ## _impl_defer()                                  \
/**/

//____________________________________________________________________________//

template<typename FPT>
struct test_BOOST_CHECK_CLOSE_body {
    void operator()() {
#define DO_BOOST_CHECK_CLOSE( first, second, e )   \
        fp1     = static_cast<FPT>(first);         \
        fp2     = static_cast<FPT>(second);        \
        epsilon = static_cast<FPT>(e);             \
                                                   \
        BOOST_CHECK_CLOSE( fp1, fp2, epsilon );    \
/**/
        unit_test_log.set_threshold_level( log_all_errors );

        FPT fp1, fp2, epsilon;

        DO_BOOST_CHECK_CLOSE( 1, 1, 0 );

        DO_BOOST_CHECK_CLOSE( 0, 1e-20, 1e-5 );
        DO_BOOST_CHECK_CLOSE( 0, 1e-30, 1e-5 );
        DO_BOOST_CHECK_CLOSE( 0, -1e-10, 0.1 );
        DO_BOOST_CHECK_CLOSE( 0.123456, 0.123457, 1e-4 );

        DO_BOOST_CHECK_CLOSE( 0.123456, 0.123457, 1e-3 );

        DO_BOOST_CHECK_CLOSE( 0.123456, -0.123457, 1e-3 );

        DO_BOOST_CHECK_CLOSE( 1.23456e28, 1.23457e28, 1e-3 );

        DO_BOOST_CHECK_CLOSE( 1.23456e-10, 1.23457e-11, 1e-3 );
        DO_BOOST_CHECK_CLOSE( 1.111e-10, 1.112e-10, 0.08999 );
        DO_BOOST_CHECK_CLOSE( 1.112e-10, 1.111e-10, 0.08999 );

        DO_BOOST_CHECK_CLOSE( 1     , 1.0001, 1.1e-2 );
        DO_BOOST_CHECK_CLOSE( 1.0002, 1.0001, 1.1e-2 );

        DO_BOOST_CHECK_CLOSE( 1     , 1.0002, 1.1e-2 );

        unit_test_log.set_threshold_level( log_nothing );
    }
};

//____________________________________________________________________________//

typedef boost::mpl::list<float,double,long double> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE( test_BOOST_CHECK_CLOSE, FPT, test_types )
{
    test_case* impl = make_test_case( test_BOOST_CHECK_CLOSE_body<FPT>(), "test_BOOST_CHECK_CLOSE" );

    unit_test_log.set_stream( ots() );
    unit_test_log.set_threshold_level( log_nothing );
    unit_test_log.set_formatter( new shorten_lf );
    
    framework::run( impl );

    unit_test_log.set_threshold_level(
        runtime_config::log_level() != invalid_log_level
            ? runtime_config::log_level()
            : log_all_errors );
    unit_test_log.set_format( runtime_config::log_format());
    unit_test_log.set_stream( std::cout );
    BOOST_CHECK( ots().match_pattern() );
}

//____________________________________________________________________________//

TEST_CASE( test_CHECK_SMALL )
{
    BOOST_CHECK_SMALL( 1e-6, 1e-5 );
    BOOST_CHECK_SMALL( 1e-6, 1e-7 );
    BOOST_CHECK_SMALL( -1e-6, 1e-5 );
}

//____________________________________________________________________________//

TEST_CASE( test_close_at_tolerance )
{
    double fp1     = 1.00000001;
    double fp2     = 1.00000002;
    double epsilon = 1e-6;

    close_at_tolerance<double> pred( epsilon, FPC_WEAK );
    BOOST_CHECK_PREDICATE( pred, (fp1)(fp2) );

    BOOST_CHECK_CLOSE( fp1, fp2, epsilon );

    fp1     = 1.23456e-10;
    fp2     = 1.23457e-10;
    epsilon = 8.1e-4;

    BOOST_CHECK_PREDICATE( close_at_tolerance<double>( epsilon, FPC_WEAK ), (fp1)(fp2) );
    BOOST_CHECK_PREDICATE( close_at_tolerance<double>( epsilon ), (fp1)(fp2) );
}

//____________________________________________________________________________//

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.21  2005/05/11 05:07:57  rogeeff
//  licence update
//
//  Revision 1.20  2005/04/17 15:49:17  rogeeff
//  *** empty log message ***
//
//  Revision 1.19  2005/03/22 07:14:44  rogeeff
//  no message
//
//  Revision 1.18  2005/02/21 10:29:04  rogeeff
//  no message
//
//  Revision 1.17  2005/02/20 08:28:34  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
// ***************************************************************************

// EOF
