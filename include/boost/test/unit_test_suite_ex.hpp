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
//  Description : provides extention for unit test framework that allows usage
//  boost::function as a test case base function.
// ***************************************************************************

#ifndef BOOST_TEST_SUITE_EX_HPP_071894GER
#define BOOST_TEST_SUITE_EX_HPP_071894GER

// Boost.Test
#include <boost/test/unit_test_suite.hpp>
#include <boost/test/detail/workarounds.hpp>

// Boost
#include <boost/function/function0.hpp>
#include <boost/function/function1.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {

namespace unit_test {

// ************************************************************************** //
// **************            boost_function_test_case             ************** //
// ************************************************************************** //

class boost_function_test_case : public test_case {
public:
    typedef function0<void> function_type;

    // Constructor
    boost_function_test_case( function_type f_, const_string name_ )
    : test_case( name_, true, 1 ), m_function( f_ ) {}

protected:
    // test case implementation
    void                do_run()        { m_function(); }

private:
    // Data members
    function_type       m_function;
};

// ************************************************************************** //
// **************        parametrized_function_test_case       ************** //
// ************************************************************************** //

template <typename ParamIterator, typename ParameterType>
class parametrized_boost_function_test_case : public test_case {
public:
    typedef function1<void,ParameterType> function_type;

    // Constructor
    parametrized_boost_function_test_case( function_type f_, const_string name_,
                                        ParamIterator const& begin_, ParamIterator const& end_ )
    : test_case( name_, true, 0 ), m_first_parameter( begin_ ), m_last_parameter( end_ ), m_function( f_ )
    {
        p_stages_amount.set( ut_detail::distance( begin_, end_ ) );
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
// **************               object generators              ************** //
// ************************************************************************** //

inline test_case*
make_test_case( function0<void> const& fct_, const_string name_ )
{
    return new boost_function_test_case( fct_, ut_detail::normalize_test_case_name( name_ ) );
}

template<typename ParamIterator, typename ParameterType>
inline test_case*
make_test_case( function1<void,ParameterType> const& fct_, const_string name_, 
                  ParamIterator const& begin_, ParamIterator const& end_ )
{
    return new parametrized_boost_function_test_case<ParamIterator,ParameterType>(
                    fct_, ut_detail::normalize_test_case_name( name_ ), begin_, end_ );
}

} // unit_test

} // namespace boost

//____________________________________________________________________________//

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.22  2005/02/20 08:27:06  rogeeff
//  This a major update for Boost.Test framework. See release docs for complete list of fixes/updates
//
//  Revision 1.21  2005/02/01 06:40:07  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.20  2005/01/30 03:21:34  rogeeff
//  interface changed to use const_string
//
// ***************************************************************************

#endif // BOOST_TEST_SUITE_EX_HPP_071894GER
