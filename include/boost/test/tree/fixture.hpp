//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// Defines fixture interface and object makers
// ***************************************************************************

#ifndef BOOST_TEST_TREE_FIXTURE_HPP_100311GER
#define BOOST_TEST_TREE_FIXTURE_HPP_100311GER

// Boost.Test
#include <boost/test/detail/config.hpp>

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/function/function0.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {

// ************************************************************************** //
// **************               test_unit_fixture              ************** //
// ************************************************************************** //

class BOOST_TEST_DECL test_unit_fixture {
public:
    virtual ~test_unit_fixture() {}

    // Fixture interface
    virtual void    setup() = 0;
    virtual void    teardown() = 0;
};

typedef shared_ptr<test_unit_fixture> test_unit_fixture_ptr;

// ************************************************************************** //
// **************               fixture helper functions       ************** //
// ************************************************************************** //


namespace fixture_details {

    template<typename T>
    struct has_setup {
        template<typename U, void (U::*)()> struct detect {};
        template<typename U> static char Test(detect<U, &U::setup>*);
        template<typename U> static int Test(...);
        static const bool value = sizeof(Test<T>(0)) == sizeof(char);
    };

    template<typename T>
    struct has_teardown {
        template<typename U, void (U::*)()> struct detect {};
        template<typename U> static char Test(detect<U, &U::teardown>*);
        template<typename U> static int Test(...);
        static const bool value = sizeof(Test<T>(0)) == sizeof(char);
    };

    template <bool has_setup = false>
    struct call_setup          { template <class U> void operator()(U& u) { }               };

    template <>
    struct call_setup<true>    { template <class U> void operator()(U& u) { u.setup(); }    };

    template <bool has_teardown = false>
    struct call_teardown       { template <class U> void operator()(U& u) { }               };

    template <>
    struct call_teardown<true> { template <class U> void operator()(U& u) { u.teardown(); } };

    //! Calls the fixture "setup" if detected by the compiler, otherwise does nothing.
    template <class U>
    void setup_conditional(U& u) {
        return call_setup<has_setup<U>::value>()(u);
    }

    //! Calls the fixture "teardown" if detected by the compiler, otherwise does nothing.
    template <class U>
    void teardown_conditional(U& u) {
        return call_teardown<has_teardown<U>::value>()(u);
    }

}


// ************************************************************************** //
// **************              class_based_fixture             ************** //
// ************************************************************************** //

template<typename F, typename Arg=void>
class class_based_fixture : public test_unit_fixture {
public:
    // Constructor
    explicit class_based_fixture( Arg const& arg ) : m_inst(), m_arg( arg ) {}

private:
    // Fixture interface
    virtual void    setup()         { m_inst.reset( new F( m_arg ) ); fixture_details::setup_conditional(*m_inst); }
    virtual void    teardown()      { fixture_details::teardown_conditional(*m_inst); m_inst.reset(); }

    // Data members
    scoped_ptr<F>   m_inst;
    Arg             m_arg;
};

//____________________________________________________________________________//

template<typename F>
class class_based_fixture<F,void> : public test_unit_fixture {
public:
    // Constructor
    class_based_fixture() : m_inst( 0 ) {}

private:
    // Fixture interface
    virtual void    setup()         { m_inst.reset( new F ); fixture_details::setup_conditional(*m_inst); }
    virtual void    teardown()      { fixture_details::teardown_conditional(*m_inst); m_inst.reset(); }

    // Data members
    scoped_ptr<F>   m_inst;
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************            function_based_fixture            ************** //
// ************************************************************************** //

class function_based_fixture : public test_unit_fixture {
public:
    // Constructor
    function_based_fixture( boost::function<void ()> const& setup_, boost::function<void ()> const& teardown_ )
    : m_setup( setup_ )
    , m_teardown( teardown_ )
    {
    }

private:
    // Fixture interface
    virtual void                setup()     { if( m_setup ) m_setup(); }
    virtual void                teardown()  { if( m_teardown ) m_teardown(); }

    // Data members
    boost::function<void ()>    m_setup;
    boost::function<void ()>    m_teardown;
};

} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_TREE_FIXTURE_HPP_100311GER

