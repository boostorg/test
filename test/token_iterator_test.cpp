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
//  Description : string_token_iterator unit test
// *****************************************************************************

// Boost.Test
#include <boost/test/unit_test.hpp>

#include <boost/test/detail/iterator/token_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>

namespace utf = boost::unit_test;

#include <iostream>
#include <list>
#include <iterator>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std{ using ::toupper; using ::tolower; }
#endif

//____________________________________________________________________________//

static utf::string_token_iterator  sti_end;
static utf::wstring_token_iterator wsti_end;
void test_default_delim_policy()
{
    utf::string_token_iterator tit( "This is\n,  a \ttest" );
    char const* res[] = { "This", "is", ",", "a", "test" };

    BOOST_CHECK_EQUAL_COLLECTIONS( tit, sti_end, res );
}

//____________________________________________________________________________//

void test_wide()
{
    utf::wstring_token_iterator tit( L"Почему бы и нет" );
    wchar_t const* res[4] = { L"Почему", L"бы", L"и", L"нет" };

    BOOST_CHECK_EQUAL_COLLECTIONS( tit, wsti_end, res );
}

//____________________________________________________________________________//

void test_custom_drop_delim()
{
    utf::string_token_iterator tit( "My:-:\t: :string, :", utf::dropped_delimeters = ":" );
    char const* res[] = { "My", "-", "\t", " ", "string", ",", " " };

    BOOST_CHECK_EQUAL_COLLECTIONS( tit, sti_end, res );
}

//____________________________________________________________________________//

void test_custom_keep_delim()
{
    utf::string_token_iterator tit( "abc = \t\t 123, int", utf::kept_delimeters = "=," );
    char const* res[] = { "abc", "=", "123", ",", "int" };

    BOOST_CHECK_EQUAL_COLLECTIONS( tit, sti_end, res );
}

//____________________________________________________________________________//

void test_keep_empty_tokens()
{
    utf::string_token_iterator tit( "fld,, 456,a=4,", 
                                    utf::dropped_delimeters = " ,",
                                    utf::kept_delimeters    = "=",
                                    utf::keep_empty_tokens ); 
    char const* res[] = { "fld", "", "", "456", "a", "=", "4", "" };

    BOOST_CHECK_EQUAL_COLLECTIONS( tit, sti_end, res );
}

//____________________________________________________________________________//

struct ci_comp {
    bool operator()( char c1, char c2 )
    {
        return (std::toupper)( c1 ) == (std::toupper)( c2 );
    }
};

void test_custom_compare()
{
    typedef utf::basic_string_token_iterator<char,ci_comp> my_token_iterator;

    my_token_iterator tit( "093514T120104", utf::dropped_delimeters = "t" );
    char const* res[] = { "093514", "120104" };

    my_token_iterator end;
    BOOST_CHECK_EQUAL_COLLECTIONS( tit, end, res );
}

//____________________________________________________________________________//

void test_range_token_iterator()
{
    typedef utf::range_token_iterator<std::list<char>::iterator> my_token_iterator;

    std::list<char> l;
    char const* pattern = "a bc , cd";
    std::copy( pattern, pattern+9, std::back_inserter( l ) );

    my_token_iterator tit( l.begin(), l.end() );
    char const* res[] = { "a", "bc", ",", "cd" };

    my_token_iterator end;
    BOOST_CHECK_EQUAL_COLLECTIONS( tit, end, res );
}

//____________________________________________________________________________//

void test_istream_token_iterator()
{
    typedef utf::range_token_iterator<std::istream_iterator<char> > my_token_iterator;

    std::istream_iterator<char> in_it( std::cin );

    my_token_iterator tit( in_it, std::istream_iterator<char>(), utf::dropped_delimeters = ":" );

    while( tit != my_token_iterator() ) {
        std::cout << '<' << *tit << '>';
        ++tit;
    }
}

//____________________________________________________________________________//

template<typename Iter>
void moo( Iter b )
{
    char const* res[] = { "ABC", "SDF", " ", "SD", "FG", " " };

    Iter end;
    BOOST_CHECK_EQUAL_COLLECTIONS( b, end, res );
}

template<typename Iter>
void foo( Iter b, Iter e )
{
    moo( utf::make_range_token_iterator( b, e, utf::kept_delimeters = utf::use_isspace, utf::dropped_delimeters = "2" ) );
}

inline char loo( char c ) { return (std::toupper)( c ); }

void test_make_range_token_iterator()
{
    char const* str = "Abc22sdf sd2fg ";

    foo( boost::make_transform_iterator( str, loo ),
         boost::make_transform_iterator( str+15, loo ) );
}

//____________________________________________________________________________//

utf::test_suite*
init_unit_test_suite( int argc, char* argv[] )
{
    utf::test_suite* test= BOOST_TEST_SUITE( "token iterator unit test" );

    test->add( BOOST_TEST_CASE( &test_default_delim_policy ) );
    test->add( BOOST_TEST_CASE( &test_wide ) );
    test->add( BOOST_TEST_CASE( &test_custom_drop_delim ) );
    test->add( BOOST_TEST_CASE( &test_custom_keep_delim ) );
    test->add( BOOST_TEST_CASE( &test_keep_empty_tokens ) );
    test->add( BOOST_TEST_CASE( &test_custom_compare ) );
    test->add( BOOST_TEST_CASE( &test_range_token_iterator ) );
//    test->add( BOOST_TEST_CASE( &test_istream_token_iterator ) );
    test->add( BOOST_TEST_CASE( &test_make_range_token_iterator ) );

    return test;
}

//____________________________________________________________________________//

// *****************************************************************************
// History :
//
// $Log$
// Revision 1.1  2004/06/05 11:04:50  rogeeff
// no message
//
// *****************************************************************************

// EOF
