//  (C) Copyright Gennadiy Rozental 2011.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 62023 $
//
//  Description : unit test for new assertion construction based on input expression
// ***************************************************************************

// Boost.Test
#define BOOST_TEST_MODULE Boost.Test assertion consruction test
#include <boost/test/unit_test.hpp>
#include <boost/test/tools/assertion.hpp>

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_basic_value_expression_construction )
{
    using namespace boost::test_tools;
    assertion::seed seed;

    {
        assertion::expression const& E = assertion::seed()->*1;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( res );
        BOOST_CHECK( res.message().is_empty() );
    }

    {
        assertion::expression const& E = seed->*0;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)0 is false" );
    }

    {
        assertion::expression const& E = seed->*true;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( res );
        BOOST_CHECK( res.message().is_empty() );
    }

    {
        assertion::expression const& E = seed->*1.5;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( res );
    }

#ifndef BOOST_NO_DECLTYPE
    {
        assertion::expression const& E = seed->* "abc";
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( res );
    }
#endif

    {
        assertion::expression const& E = seed->* 1>2;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "1<=2" );
    }

}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_comparison_expression )
{
    using namespace boost::test_tools;
    assertion::seed seed;

    {
        assertion::expression const& E = seed->* 1>2;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "1<=2" );
    }

    {
        assertion::expression const& E = seed->* 100 < 50;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "100>=50" );
    }

    {
        assertion::expression const& E = seed->* 5<=4;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "5>4" );
    }

    {
        assertion::expression const& E = seed->* 10>=20;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "10<20" );
    }

    {
        int i = 10;
        assertion::expression const& E = seed->* i != 10;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "10==10" );
    }

    {
        int i = 5;
        assertion::expression const& E = seed->* i == 3;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "5!=3" );
    }
}

//____________________________________________________________________________//

#ifndef BOOST_NO_DECLTYPE

BOOST_AUTO_TEST_CASE( test_arithmetic_ops )
{
    using namespace boost::test_tools;
    assertion::seed seed;

    {
        int i = 3;
        int j = 5;
        assertion::expression const& E = seed->* i+j !=8;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "3+5==8" );
    }

    {
        int i = 3;
        int j = 5;
        assertion::expression const& E = seed->* 2*i-j > 1;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "2*3-5<=1" );
    }

    {
        int j = 5;
        assertion::expression const& E = seed->* 2<<j < 30;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "2<<5>=30" );
    }

    {
        int i = 2;
        int j = 5;
        assertion::expression const& E = seed->* i&j;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "2&5" );
    }

    {
        int i = 3;
        int j = 5;
        assertion::expression const& E = seed->* i^j^6;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "3^5^6" );
    }

    // do not support
    // assertion::expression const& E = seed->*99/2 == 48 || 101/2 > 50;
    // assertion::expression const& E = seed->* a ? 100 < 50 : 25*2 == 50;
    // assertion::expression const& E = seed->* true,false;
}

//____________________________________________________________________________//

#endif // BOOST_NO_DECLTYPE

struct Testee {
    static int s_copy_counter;

    Testee() : m_value( false ) {}
    Testee( Testee const& ) : m_value(false) { s_copy_counter++; }
#ifndef BOOST_NO_RVALUE_REFERENCES
    Testee( Testee&& ) : m_value(false)     {}
    Testee( Testee const&& ) : m_value(false)     {}
#endif

    bool foo() { return m_value; }
    operator bool() const { return m_value; }

    friend std::ostream& operator<<( std::ostream& ostr, Testee const& ) { return ostr << "Testee"; }

    bool m_value;
};

int Testee::s_copy_counter = 0;

#ifndef BOOST_NO_RVALUE_REFERENCES
Testee          get_obj() { return std::move( Testee() ); }
Testee const    get_const_obj() { return std::move( Testee() ); }
#else
Testee          get_obj() { return Testee(); }
Testee const    get_const_obj() { return Testee(); }
#endif

BOOST_AUTO_TEST_CASE( test_objects )
{
    using namespace boost::test_tools;
    assertion::seed seed;

    {
        Testee obj;
        Testee::s_copy_counter = 0;

        assertion::expression const& E = seed->* obj;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)Testee is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( Testee::s_copy_counter, 0 );
#else
        BOOST_CHECK_EQUAL( Testee::s_copy_counter, 1 );
#endif
    }

    {
        Testee const obj;
        Testee::s_copy_counter = 0;

        assertion::expression const& E = seed->* obj;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)Testee is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( Testee::s_copy_counter, 0 );
#else
        BOOST_CHECK_EQUAL( Testee::s_copy_counter, 1 );
#endif
    }

    {
        Testee::s_copy_counter = 0;

        assertion::expression const& E = seed->* get_obj();
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)Testee is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( Testee::s_copy_counter, 0 );
#else
        BOOST_CHECK_EQUAL( Testee::s_copy_counter, 1 );
#endif
    }

    {
        Testee::s_copy_counter = 0;

        assertion::expression const& E = seed->* get_const_obj();
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)Testee is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( Testee::s_copy_counter, 0 );
#else
        BOOST_CHECK_EQUAL( Testee::s_copy_counter, 1 );
#endif
    }
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_pointers )
{
    using namespace boost::test_tools;
    assertion::seed seed;

    {
        Testee* ptr = 0;

        assertion::expression const& E = seed->* ptr;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
    }

    {
        Testee obj1;
        Testee obj2;

        assertion::expression const& E = seed->* &obj1 == &obj2;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
    }

    {
        Testee obj;
        Testee* ptr =&obj;

        assertion::expression const& E = seed->* *ptr;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)Testee is false" );
    }

#ifndef BOOST_NO_DECLTYPE
    {
        Testee obj;
        Testee* ptr =&obj;
        bool Testee::* mem_ptr =&Testee::m_value;

        assertion::expression const& E = seed->* ptr->*mem_ptr;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
    }
#endif

    // do not support
    // Testee obj;
    // bool Testee::* mem_ptr =&Testee::m_value;
    // assertion::expression const& E = seed->* obj.*mem_ptr;
}

//____________________________________________________________________________//

BOOST_AUTO_TEST_CASE( test_mutating_ops )
{
    using namespace boost::test_tools;
    assertion::seed seed;

    {
        int j = 5;

        assertion::expression const& E = seed->* j = 0;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)0 is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( j, 0 );
#else
        BOOST_CHECK_EQUAL( j, 5 );
#endif
    }

    {
        int j = 5;

        assertion::expression const& E = seed->* j -= 5;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)0 is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( j, 0 );
#else
        BOOST_CHECK_EQUAL( j, 5 );
#endif
    }

    {
        int j = 5;

        assertion::expression const& E = seed->* j *= 0;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)0 is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( j, 0 );
#else
        BOOST_CHECK_EQUAL( j, 5 );
#endif
    }

    {
        int j = 5;

        assertion::expression const& E = seed->* j /= 10;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)0 is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( j, 0 );
#else
        BOOST_CHECK_EQUAL( j, 5 );
#endif
    }

    {
        int j = 4;

        assertion::expression const& E = seed->* j %= 2;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)0 is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( j, 0 );
#else
        BOOST_CHECK_EQUAL( j, 4 );
#endif
    }

    {
        int j = 5;

        assertion::expression const& E = seed->* j ^= j;
        predicate_result const& res = E.evaluate();
        BOOST_CHECK( !res );
        BOOST_CHECK_EQUAL( res.message(), "(bool)0 is false" );
#ifndef BOOST_NO_RVALUE_REFERENCES
        BOOST_CHECK_EQUAL( j, 0 );
#else
        BOOST_CHECK_EQUAL( j, 5 );
#endif
    }
}

//____________________________________________________________________________//

// EOF

