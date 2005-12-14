//  (C) Copyright Gennadiy Rozental 2005.
//  Use, modification, and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : 
// ***************************************************************************

#ifndef BOOST_TEST_INTERACTION_BASED_IPP_112105GER
#define BOOST_TEST_INTERACTION_BASED_IPP_112105GER

// Boost.Test
#include <boost/test/detail/config.hpp>
#include <boost/test/utils/callback.hpp>
#include <boost/test/interaction_based.hpp>
#include <boost/test/mock_object.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

// STL
#include <stdexcept>
#include <string>
#include <cstdlib>

# ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::malloc; using ::free; }
# endif

//____________________________________________________________________________//

namespace boost {

namespace itest { // interaction-based testing

// ************************************************************************** //
// **************                    manager                   ************** //
// ************************************************************************** //

manager::manager()
{
    instance_ptr( true, this );
}

//____________________________________________________________________________//

manager::~manager()
{
    instance_ptr( true );
}

//____________________________________________________________________________//
    
manager*
manager::instance_ptr( bool reset, manager* new_ptr )
{
    static manager dummy( 0 );
    
    static manager* ptr = &dummy;
    
    if( reset ) {
        if( new_ptr ) {
            if( ptr != &dummy )
                throw std::logic_error( std::string( "Couldn't run two interation based test the same time" ) );
                
            ptr = new_ptr;
        }
        else
            ptr = &dummy;
    }
    
    return ptr;
}
    
// ************************************************************************** //
// **************                  mock_object                 ************** //
// ************************************************************************** //

mock_object<> simple_mock;

}  // namespace itest

}  // namespace boost

//____________________________________________________________________________//

// ************************************************************************** //
// **************              operator new overload           ************** //
// ************************************************************************** //

#if !defined(BOOST_ITEST_NO_NEW_OVERLOADS) && !BOOST_WORKAROUND(BOOST_MSVC, <1300)

void*
operator new( std::size_t s, ::boost::itest::location const& l )
{
    void* res = std::malloc( s );

    if( res )
        ::boost::itest::manager::instance().allocated( l.m_file_name, l.m_line_num, res, s );
    else
        throw std::bad_alloc();
        
    return res;
}

//____________________________________________________________________________//

void*
operator new[]( std::size_t s, ::boost::itest::location const& l )
{
    void* res = std::malloc( s );

    if( res )
        ::boost::itest::manager::instance().allocated( l.m_file_name, l.m_line_num, res, s );
    else
        throw std::bad_alloc();
        
    return res;
}

//____________________________________________________________________________//

void operator delete( void* p, ::boost::itest::location const& )
{
    ::boost::itest::manager::instance().freed( p );

    std::free( p );
}

//____________________________________________________________________________//

void
operator delete[]( void* p, ::boost::itest::location const& )
{
    ::boost::itest::manager::instance().freed( p );

    std::free( p );
}

//____________________________________________________________________________//

#endif

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.1  2005/12/14 05:56:56  rogeeff
//  Interraction based / logged expectation testing is introduced
//
// ***************************************************************************

#endif // BOOST_TEST_INTERACTION_BASED_IPP_112105GER
