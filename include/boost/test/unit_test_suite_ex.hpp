//  (C) Copyright Gennadiy Rozental 2001-2002.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_UNIT_TEST_SUITE_EX_HPP
#define BOOST_UNIT_TEST_SUITE_EX_HPP

// LOCAL
#include <boost/test/unit_test_suite.hpp>

// BOOST
#include <boost/function.hpp>

namespace boost {

namespace unit_test_framework {

// ************************************************************************** //
// **************            function_test_case_ex             ************** //
// ************************************************************************** //

class function_test_case_ex : public test_case {
public:
    typedef function0<void> function_type;

    // Constructor
    function_test_case_ex( function_type f, char const* name )
    : test_case( name, 1 ), m_function( f ) {}

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
class parametrized_function_test_case_ex : public test_case {
public:
    typedef function1<void,ParameterType> function_type;

    // Constructor
    parametrized_function_test_case_ex( function_type f, char const* name,
                                     ParamIterator const& begin, ParamIterator const& end )
    : test_case( name ), m_first_parameter( begin ), m_last_parameter( end ), m_function( f ) {
#ifndef BOOST_NO_STD_DISTANCE
        p_stages_amount.set( std::distance( begin, end ) );
#else
       unit_test_counter dist = 0;
        std::distance( begin, end, dist );
        p_stages_amount.set( dist );
#endif
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
create_test_case( function0<void> const& fct, std::string name )
{
    return new function_test_case_ex( fct, detail::normalize_test_case_name( name ) );
}

template<typename ParamIterator, typename ParameterType>
inline test_case*
create_test_case( function1<void,ParameterType> const& fct, std::string name, ParamIterator const& begin, ParamIterator const& end )
{
    return new parametrized_function_test_case_ex<ParamIterator,ParameterType>(
                    fct, detail::normalize_test_case_name( name ), begin, end );
}

} // unit_test_framework

} // namespace boost

//  Revision History
//  12 Dec 01  Initial version (Gennadiy Rozental)

#endif // BOOST_UNIT_TEST_SUITE_EX_HPP
