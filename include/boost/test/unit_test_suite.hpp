//  (C) Copyright Gennadiy Rozental & Ullrich Koethe 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_UNIT_TEST_SUITE_HPP
#define BOOST_UNIT_TEST_SUITE_HPP

// BOOST
#include <boost/config.hpp> // for distance workaround
#include <boost/shared_ptr.hpp>

// LOCAL
#include <boost/test/detail/unit_test_monitor.hpp>
#include <boost/test/detail/unit_test_config.hpp>
#include <boost/test/detail/class_properties.hpp>
#include <boost/test/detail/grinning_ptr.hpp>

// STL
#include <stdexcept>
#include <string>

#define BOOST_TEST_CASE( function ) \
boost::unit_test_framework::create_test_case((function), #function )
#define BOOST_CLASS_TEST_CASE( function, tc_instance ) \
boost::unit_test_framework::create_test_case((function), #function, tc_instance )
#define BOOST_PARAM_TEST_CASE( function, begin, end ) \
boost::unit_test_framework::create_test_case((function), #function, (begin), (end) )
#define BOOST_PARAM_CLASS_TEST_CASE( function, tc_instance, begin, end ) \
boost::unit_test_framework::create_test_case((function), #function, tc_instance, (begin), (end) )
#define BOOST_TEST_SUITE( testsuite_name ) \
( new boost::unit_test_framework::test_suite( testsuite_name ) )

namespace boost {

namespace unit_test_framework {

// ************************************************************************** //
// **************                   test_case                  ************** //
// ************************************************************************** //

class test_case {
public:
    typedef detail::unit_test_monitor::error_level error_level_type;

    // Destructor
    virtual             ~test_case()    {}

    // number of tests in this test case
    virtual unit_test_counter size() const;

    // execute this method to run this test case; for now it is not supposed to be overwritten;
    void                run();

    // public properties
    BOOST_READONLY_PROPERTY( int, 2, (test_case,test_suite) )
                        p_timeout;                  // timeout for the excecution monitor
    BOOST_READONLY_PROPERTY( unit_test_counter, 2, (test_case,test_suite) )
                        p_expected_failures;        // number of aseertions that are expected to fail in this test case

protected:
    // protected properties
    BOOST_READONLY_PROPERTY( std::string, 1, (test_case) )
                        p_name;                     // name for this test case
    BOOST_READONLY_PROPERTY( bool, 2, (test_case,test_suite) )
                        p_compound_stage;           // used to properly manage progress report
    BOOST_READWRITE_PROPERTY( unit_test_counter )
                        p_stages_amount;            // number of stages this test consist of; stage could another test case
                                                    // like with test_suite, another parameterized test for parameterized_test_case
                                                    // or 1 stage that reflect single test_case behaviour

    // access methods
    void                curr_stage_is_compound();

    // Constructor
    explicit            test_case( char const*          name            = "Unnamed",
                                   unit_test_counter    stages_number   = 0,
                                   bool                 monitor_run     = true );

    // test case implementation hooks to be called with unit_test_monitor or alone
    virtual void        do_init()       {}
    virtual void        do_run()        {}
    virtual void        do_destroy()    {}

private:
    // Data members
    bool                m_monitor_run;              // true - unit_test_monitor will be user to monitor running
                                                    // of implementation function
    static bool         s_abort_testing;            // used to flag critical error and try gracefully stop testing
};

//____________________________________________________________________________//

inline
test_case::test_case( char const* name, unit_test_counter stages_number, bool monitor_run )
: p_timeout( 0 ), p_expected_failures( 0 ),
  p_name( name ), p_compound_stage( false ), p_stages_amount( stages_number ),
  m_monitor_run( monitor_run )
{
}

//____________________________________________________________________________//

inline unit_test_counter
test_case::size() const
{
    return p_stages_amount;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************              function_test_case              ************** //
// ************************************************************************** //

class function_test_case : public test_case {
public:
    typedef void  (*function_type)();

    // Constructor
    function_test_case( function_type f, char const* name )
    : test_case( name, 1 ), m_function( f ) {}

protected:
    // test case implementation
    void                do_run()        { m_function(); }

private:
    // Data members
    function_type       m_function;
};

// ************************************************************************** //
// **************                class_test_case               ************** //
// ************************************************************************** //

template<class UserTestCase>
class class_test_case : public test_case {
public:
    typedef void  (UserTestCase::*function_type)();

    // Constructor
    class_test_case( function_type f, char const* name, boost::shared_ptr<UserTestCase>& user_test_case )
    : test_case( name, 1 ), m_user_test_case( user_test_case ), m_function( f ) {}

private:
    // test case implementation
    void                do_run()        { ((*m_user_test_case).*m_function)(); }
    void                do_destroy()    { m_user_test_case.reset(); }

    // Data members
    boost::shared_ptr<UserTestCase> m_user_test_case;
    function_type       m_function;
};

// ************************************************************************** //
// **************        parametrized_function_test_case       ************** //
// ************************************************************************** //

template <typename ParamIterator, typename ParameterType>
class parametrized_function_test_case : public test_case {
public:
    typedef void  (*function_type)( ParameterType );

    // Constructor
    parametrized_function_test_case( function_type f, char const* name,
                                     ParamIterator const& par_begin, ParamIterator const& par_end )
    : test_case( name ), m_first_parameter( par_begin ), m_last_parameter( par_end ), m_function( f )
    {
       p_stages_amount.set( detail::distance( par_begin, par_end ) );
    }

    // test case implementation
    void                do_init()       { m_curr_parameter = m_first_parameter; }
    void                do_run()        { m_function( *m_curr_parameter ); ++m_curr_parameter; }

private:
    // Data members
    ParamIterator       m_first_parameter;
    ParamIterator       m_last_parameter;
    ParamIterator       m_curr_parameter;

    function_type       m_function;
};

// ************************************************************************** //
// **************         parametrized_class_test_case         ************** //
// ************************************************************************** //

template <class UserTestCase, class ParamIterator, typename ParameterType>
class parametrized_class_test_case : public test_case {
public:
    typedef void  (UserTestCase::*function_type)( ParameterType );

    // Constructor
    parametrized_class_test_case( function_type f, char const* name, boost::shared_ptr<UserTestCase>& user_test_case,
                                  ParamIterator const& par_begin, ParamIterator const& par_end )
    : test_case( name ), m_first_parameter( par_begin ), m_last_parameter( par_end ),
      m_user_test_case( user_test_case ), m_function( f )
    {
       p_stages_amount.set( detail::distance( par_begin, par_end ) );
    }

    // test case implementation
    void                do_init()       { m_curr_parameter = m_first_parameter; }
    void                do_run()        { ((*m_user_test_case).*m_function)( *m_curr_parameter ); ++m_curr_parameter; }
    void                do_destroy()    { m_user_test_case.reset(); }

private:
    // Data members
    ParamIterator       m_first_parameter;
    ParamIterator       m_last_parameter;
    ParamIterator       m_curr_parameter;

    boost::shared_ptr<UserTestCase> m_user_test_case;
    function_type       m_function;
};

// ************************************************************************** //
// **************                  test_suite                  ************** //
// ************************************************************************** //

class test_suite : public test_case {
public:
    // Constructor
    explicit test_suite( char const* name = "Master" );

    // Destructor
    virtual             ~test_suite();

    // test case list management
    void                add( test_case* tc, unit_test_counter expected_failures = 0, int timeout = 0 );

    // access methods
    unit_test_counter   size() const;

    // test case implementation
    void                do_init();
    void                do_run();

private:
    // Data members
    struct Impl;
    unit_test_framework::detail::grinning_ptr<Impl> m_pimpl;
};

// ************************************************************************** //
// **************               object generators              ************** //
// ************************************************************************** //

namespace detail {

inline char const*
normalize_test_case_name( std::string& name )
{
    if( name[0] == '&' )
        name.erase( 0, 1 );

    return name.data();
}

} // namespace detail

inline test_case*
create_test_case( void (*fct)(), std::string name )
{
    return new function_test_case( fct, detail::normalize_test_case_name( name ) );
}

template<class UserTestCase>
inline test_case*
create_test_case( void (UserTestCase::*fct)(), std::string name, boost::shared_ptr<UserTestCase>& user_test_case )
{
    return new class_test_case<UserTestCase>( fct, detail::normalize_test_case_name( name ), user_test_case );
}

template<typename ParamIterator, typename ParamType>
inline test_case*
create_test_case( void (*fct)( ParamType ), std::string name, ParamIterator const& par_begin, ParamIterator const& par_end )
{
    return new parametrized_function_test_case<ParamIterator,ParamType>(
        fct, detail::normalize_test_case_name( name ), par_begin, par_end );
}

template<class UserTestCase, typename ParamIterator, typename ParamType>
inline test_case*
create_test_case( void (UserTestCase::*fct)( ParamType ), std::string name, boost::shared_ptr<UserTestCase>& user_test_case,
                  ParamIterator const& par_begin, ParamIterator const& par_end )
{
    return new parametrized_class_test_case<UserTestCase,ParamIterator,ParamType>(
        fct, detail::normalize_test_case_name( name ), user_test_case, par_begin, par_end );
}

} // unit_test_framework

} // namespace boost

//  Revision History
//   ? ??? 01  Initial version (Ullrich Koethe)
//   5 Oct 01  New design (Gennadiy Rozental)

#endif // BOOST_UNIT_TEST_SUITE_HPP
