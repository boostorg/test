//  (C) Copyright Gennadiy Rozental 2004.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : 
// ***************************************************************************

#ifndef BOOST_INPUT_ITERATOR_FACADE_HPP
#define BOOST_INPUT_ITERATOR_FACADE_HPP

// Boost
#include <boost/iterator/iterator_facade.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/if.hpp>

namespace boost {

namespace unit_test {

namespace detail {

template<typename T>
struct value_type_ident 
{
    typedef T value_type;
};

template<typename T>
struct ref_type_ident 
{
    typedef T reference;
};

} // namespace detail

// ************************************************************************** //
// **************            input_iterator_facade             ************** //
// ************************************************************************** //

template<typename ImplPolicy,typename Derived = mpl::void_,typename ValueType = mpl::void_,typename Reference = mpl::void_>
class input_iterator_facade : public iterator_facade<
    typename mpl::if_<mpl::is_void_<Derived>,input_iterator_facade<ImplPolicy>,Derived>::type,
    typename mpl::if_<mpl::is_void_<ValueType>,ImplPolicy,detail::value_type_ident<ValueType> >::type::value_type,
    forward_traversal_tag,
    typename mpl::if_<mpl::is_void_<Reference>,ImplPolicy,detail::ref_type_ident<Reference> >::type::reference>
{
public:
    // Constructor
    explicit            input_iterator_facade( ImplPolicy const& p = ImplPolicy() )
    : m_policy( p )
    {
        m_valid = m_policy.initialize();
        
        increment();
    }

    //!! copy constructor/assigment  - should we make rhs invalid after copy??
    // input_iterator_facade( input_iterator_facade const& rhs )
    // : m_policy( rhs.m_policy ), m_valid( rhs.m_valid )
    // {
    //     const_cast<input_iterator_adaptor&>( rhs ).m_valid = false;
    // }
    // 
    // void             operator=( input_iterator_facade const& rhs )
    // {
    //      m_policies = rhs.p_policies;
    //      m_valid    = rhs.m_valid;
    //
    //     const_cast<input_iterator_adaptor&>( rhs ).m_valid = false;
    // }

    //!! do we need reset method?
    // void             reset()
    // {
    //     m_valid = m_policy.intialize();
    // 
    //     increment();
    // }

protected: // provide access to the derived
    // Data members
    ImplPolicy          m_policy;

private:
    typedef typename mpl::if_<mpl::is_void_<ValueType>,detail::value_type_ident<ValueType>,ImplPolicy>::type::value_type value_type;
    typedef typename mpl::if_<mpl::is_void_<Reference>,ImplPolicy,detail::ref_type_ident<Reference> >::type::reference   reference;

    friend class iterator_core_access;

    // iterator facade interface implementation
    void                increment()
    {
        // we make post-end incrementation indefinetly safe 
        if( m_valid )
            m_valid = m_policy.get();
    }
    bool                equal( input_iterator_facade const& rhs ) const
    {
        // two invalid iterator equals, two valid need to be compared, invalid never equal to valid
        return !m_valid && !rhs.m_valid || m_valid && rhs.m_valid && m_policy.equal( rhs.m_policy );
    }
    reference           dereference() const
    {
        //!! should we call some kind of assert( m_valid ) here instead or do nothing???

        // we pass m_valid inside the policy to decide what to do on attempt to dereference invalid iterator
        return m_policy.dereference( m_valid );
    }

    // Data members
    bool                m_valid;
};

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2004/05/21 06:30:10  rogeeff
//  ifstream_line_iterator added
//
// ***************************************************************************

#endif // BOOST_INPUT_ITERATOR_FACADE_HPP

