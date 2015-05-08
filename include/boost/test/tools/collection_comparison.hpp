//  (C) Copyright Gennadiy Rozental 2014-2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief Collection comparison with enhanced reporting
// ***************************************************************************

#ifndef BOOST_TEST_TOOLS_ASSERTION_HPP_050815GER
#define BOOST_TEST_TOOLS_ASSERTION_HPP_050815GER

// Boost.Test

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace test_tools {
namespace assertion {

// ************************************************************************** //
// **************            lexicographic_compare             ************** //
// ************************************************************************** //

namespace op {

template <typename OP, bool can_be_equal, bool prefer_shorter, 
          typename Lhs, typename Rhs>
inline assertion_result
lexicographic_compare( Lhs const& lhs, Rhs const& rhs )
{
    assertion_result ar( true );

    typename Lhs::const_iterator first1 = lhs.begin();
    typename Rhs::const_iterator first2 = rhs.begin();
    typename Lhs::const_iterator last1  = lhs.end();
    typename Rhs::const_iterator last2  = rhs.end();
    std::size_t                  pos    = 0;

    for( ; (first1 != last1) && (first2 != last2); ++first1, ++first2, ++pos ) {
        assertion_result const& element_ar = OP::eval(*first1, *first2);
        if( !can_be_equal && element_ar )
            return ar; // a < b

        assertion_result const& reverse_ar = OP::eval(*first2, *first1);
        if( element_ar && !reverse_ar )                     
            return ar; // a<=b and !(b<=a) => a < b => return true
        
        if( element_ar || !reverse_ar ) 
            continue; // (a<=b and b<=a) or (!(a<b) and !(b<a)) => a == b => keep looking                   

        // !(a<=b) and b<=a => b < a => return false            
        ar = false;
        ar.message() << "\nFailure at position " << pos << ": "
                     << tt_detail::print_helper(*first1)
                     << OP::revert()
                     << tt_detail::print_helper(*first2)
                     << ". " << element_ar.message();
        return ar;
    }

    
    if( first1 != last1 ) {
        if( prefer_shorter ) {
            ar = false;
            ar.message() << "\nFirst collection has extra trailing elements.";
        }
    }
    else if( first2 != last2 ) {
        if( !prefer_shorter ) {
            ar = false;
            ar.message() << "\nSecond collection has extra trailing elements.";
        }
    }
    else if( !can_be_equal ) {
        ar = false;
        ar.message() << "\nCollections appear to be equal.";
    }

    return ar;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************               equality_compare               ************** //
// ************************************************************************** //

template <typename OP, typename Lhs, typename Rhs>
inline assertion_result
element_compare( Lhs const& lhs, Rhs const& rhs )
{
    assertion_result ar( true );

    if( lhs.size() != rhs.size() ) {
        ar = false;
        ar.message() << "Collections size mismatch: " << lhs.size() << " != " << rhs.size();
        return ar;
    }

    typename Lhs::const_iterator left  = lhs.begin();
    typename Rhs::const_iterator right = rhs.begin();
    std::size_t                  pos   = 0;

    for( ; pos < lhs.size(); ++left, ++right, ++pos ) {
        assertion_result const element_ar = OP::eval( *left, *right );
        if( element_ar )
            continue;

        ar = false;
        ar.message() << "\nMismatch at position " << pos << ": "
                     << tt_detail::print_helper(*left)
                     << OP::revert()
                     << tt_detail::print_helper(*right)
                     << ". " << element_ar.message();
    }

    return ar;
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************             non_equality_compare             ************** //
// ************************************************************************** //

template <typename OP, typename Lhs, typename Rhs>
inline assertion_result
non_equality_compare( Lhs const& lhs, Rhs const& rhs )
{
    assertion_result ar( true );

    if( lhs.size() != rhs.size() )
        return ar;

    typename Lhs::const_iterator left  = lhs.begin();
    typename Rhs::const_iterator right = rhs.begin();
    typename Lhs::const_iterator end   = lhs.end();

    for( ; left != end; ++left, ++right ) {
        if( OP::eval( *left, *right ) )
            return ar;
    }

    ar = false;
    ar.message() << "\nCollections appear to be equal";

    return ar;
}

//____________________________________________________________________________//

} // namespace op
} // namespace assertion
} // namespace test_tools
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_TOOLS_ASSERTION_HPP_050815GER

