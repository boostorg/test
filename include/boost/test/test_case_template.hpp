//  (C) Copyright Gennadiy Rozental 2003-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : implements support for test cases templates instantiated with 
//                sequence of test types
// ***************************************************************************

#ifndef BOOST_TEST_CASE_TEMPLATE_HPP_071894GER
#define BOOST_TEST_CASE_TEMPLATE_HPP_071894GER

// Boost.Test
#include <boost/test/unit_test_suite.hpp>

// Boost
#include "boost/mpl/size.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/identity.hpp>

//____________________________________________________________________________//

#define BOOST_META_FUNC_TEST_CASE( the_function )   \
struct BOOST_JOIN( meta_, the_function ) {          \
    template<typename T>                            \
    static void execute( T* = 0 )                   \
    {                                               \
        the_function<T>();                          \
    }                                               \
}                                                   \
/**/

#define BOOST_FUNC_TEMPLATE_TEST_CASE( the_function, typelist ) \
boost::unit_test::create_test_case_template( BOOST_JOIN( meta_, the_function )(), typelist(), BOOST_TEST_STRINGIZE( the_function ) )
    
namespace boost {
namespace unit_test {
namespace ut_detail {

// ************************************************************************** //
// **************          test_case_template_instance         ************** //
// ************************************************************************** //
// Generate test case by supplied test case template and test type

template<typename TestCaseTemplate,typename TestType>
class test_case_template_instance : public test_case {
    typedef TestType*   test_type_ptr;
public:
    explicit            test_case_template_instance( const_string template_name_ )
    : test_case( template_name_, true, 1 )  {}
    
protected:
    // test case implementation
    void                do_run()            { TestCaseTemplate::execute( test_type_ptr() ); }

};

//____________________________________________________________________________//

// ************************************************************************** //
// **************           test_case_instance_runner          ************** //
// ************************************************************************** //
// Instantiate generated test case and run it.

template<typename TestCaseTemplate>
struct test_case_instance_runner {
    explicit            test_case_instance_runner( const_string template_name_ )
    : m_template_name( template_name_ ) {}

    template<typename TestType>
    void                operator()( ::boost::mpl::identity<TestType> )
    {
        test_case_template_instance<TestCaseTemplate,TestType> the_instance( m_template_name ); //!! could this throw?

        the_instance.run();
    }

    const_string    m_template_name;
};

} // namespace ut_detail

// ************************************************************************** //
// **************              test_case_template              ************** //
// ************************************************************************** //

template<typename TestCaseTemplate,typename TestTypesList>
class test_case_template : public test_case {
public:
    // Constructor
    explicit            test_case_template( const_string name_ )
    : test_case( name_, false, 1, false ), m_template_holder( p_name.get() ) {}

    // access methods
    counter_t   size() const    { return ::boost::mpl::size<TestTypesList>::value; }

protected:
    
    // test case implementation
    void                do_run()
    {
        ::boost::mpl::for_each<TestTypesList,mpl::make_identity<boost::mpl::_> >( m_template_holder );
    }

    // Data members
    ut_detail::test_case_instance_runner<TestCaseTemplate> m_template_holder; // need instance to match for_each interface
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************               object generators              ************** //
// ************************************************************************** //

template<typename TestCaseTemplate, typename TestTypesList>
inline test_case*
create_test_case_template( TestCaseTemplate, TestTypesList, const_string name_ )
{
    return new test_case_template<TestCaseTemplate,TestTypesList>( ut_detail::normalize_test_case_name( name_ ) );
}

//____________________________________________________________________________//

} // unit_test
} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.10  2005/02/01 06:40:06  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.9  2005/01/30 03:20:38  rogeeff
//  use BOOST_JOIN and BOOST_TEST_STRINGIZE
//
// ***************************************************************************

#endif // BOOST_TEST_CASE_TEMPLATE_HPP_071894GER

