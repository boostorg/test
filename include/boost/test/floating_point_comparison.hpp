//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines algoirthms for comparing 2 floating point values
// ***************************************************************************

#ifndef BOOST_FLOATING_POINT_COMPARISON_HPP
#define BOOST_FLOATING_POINT_COMPARISON_HPP

#include <boost/limits.hpp>  // for std::numeric_limits

#include <boost/test/detail/class_properties.hpp>

namespace boost {

namespace test_toolbox {

// ************************************************************************** //
// **************        floating_point_comparison_type        ************** //
// ************************************************************************** //

enum floating_point_comparison_type { FPC_STRONG, FPC_WEAK };

// ************************************************************************** //
// **************                    details                   ************** //
// ************************************************************************** //

namespace detail {

template<typename FPT>
inline FPT
fpt_abs( FPT arg ) 
{
    return arg < 0 ? -arg : arg;
}

//____________________________________________________________________________//

// both f1 and f2 are unsigned here
template<typename FPT>
inline FPT 
safe_fpt_division( FPT f1, FPT f2 )
{
    return  (f2 < 1 && f1 > f2 * std::numeric_limits<FPT>::max())   ? std::numeric_limits<FPT>::max() :
           ((f2 > 1 && f1 < f2 * std::numeric_limits<FPT>::min() || 
             f1 == 0)                                               ? 0                               :
                                                                      f1/f2 );
}

//____________________________________________________________________________//

} // namespace detail

// ************************************************************************** //
// **************             close_at_tolerance               ************** //
// ************************************************************************** //

template<typename FPT, typename PersentType = FPT >
class close_at_tolerance {
public:
    explicit    close_at_tolerance( PersentType tolerance, floating_point_comparison_type fpc_type = FPC_STRONG ) 
    : p_fraction_tolerance( static_cast<FPT>(0.01)*tolerance ), p_strong_or_weak( fpc_type ==  FPC_STRONG ) {}

    bool        operator()( FPT left, FPT right ) const
    {
        FPT diff = detail::fpt_abs( left - right );
        FPT d1   = detail::safe_fpt_division( diff, detail::fpt_abs( right ) );
        FPT d2   = detail::safe_fpt_division( diff, detail::fpt_abs( left ) );
        
        return p_strong_or_weak ? (d1 <= p_fraction_tolerance.get() && d2 <= p_fraction_tolerance.get()) 
                                : (d1 <= p_fraction_tolerance.get() || d2 <= p_fraction_tolerance.get());
    }

    // Public properties
    BOOST_READONLY_PROPERTY( FPT, 0, () )	p_fraction_tolerance;
    BOOST_READONLY_PROPERTY( bool, 0, () )	p_strong_or_weak;
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************               check_is_closed                ************** //
// ************************************************************************** //

template<typename FPT, typename PersentType>
bool
check_is_closed( FPT left, FPT right, PersentType tolerance, floating_point_comparison_type fpc_type = FPC_STRONG )
{
    close_at_tolerance<FPT,PersentType> pred( tolerance, fpc_type );

    return pred( left, right );
}

//____________________________________________________________________________//

template<typename FPT>
FPT
compute_tolerance( FPT tolerance )
{
    close_at_tolerance<FPT> pred( tolerance );

    return pred.p_fraction_tolerance.get();
}

//____________________________________________________________________________//

} // namespace test_toolbox
} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.12  2003/11/06 07:38:57  rogeeff
//  Licence update
//  Floating point check reworked (Is not backward compartible!!!)
//  Some annoying MSVC wrnings suppressed
//
//  Revision 1.11  2003/10/27 07:13:12  rogeeff
//  licence update
//
//  Revision 1.10  2003/07/15 09:00:46  rogeeff
//  eliminate tolerance definition by number of rounding errors
//
//  Revision 1.9  2003/07/02 09:15:57  rogeeff
//  move log formatter in public interface
//
//  Revision 1.8  2003/06/09 08:43:47  rogeeff
//  added comparison type and algorithm constructor accepting it
//

// ***************************************************************************

#endif // BOOST_FLOATING_POINT_COMAPARISON_HPP
