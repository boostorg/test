//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for most recent version including documentation.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines algoirthms for comparing 2 floating point values
// ***************************************************************************

#ifndef BOOST_FLOATING_POINT_COMPARISON_HPP
#define BOOST_FLOATING_POINT_COMPARISON_HPP

#include <boost/limits.hpp>  // for std::numareic_limits

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

template<typename FPT>
class close_at_tolerance {
public:
    explicit    close_at_tolerance( FPT tolerance, floating_point_comparison_type fpc_type = FPC_STRONG ) 
    : p_tolerance( tolerance ), m_strong_or_weak( fpc_type ==  FPC_STRONG ) {}

    explicit    close_at_tolerance( int number_of_rounding_errors, floating_point_comparison_type fpc_type = FPC_STRONG ) 
    : p_tolerance( std::numeric_limits<FPT>::epsilon() * number_of_rounding_errors/2 ), 
      m_strong_or_weak( fpc_type ==  FPC_STRONG ) {}

    bool        operator()( FPT left, FPT right ) const
    {
        FPT diff = detail::fpt_abs( left - right );
        FPT d1   = detail::safe_fpt_division( diff, detail::fpt_abs( right ) );
        FPT d2   = detail::safe_fpt_division( diff, detail::fpt_abs( left ) );
        
        return m_strong_or_weak ? (d1 <= p_tolerance.get() && d2 <= p_tolerance.get()) 
                                : (d1 <= p_tolerance.get() || d2 <= p_tolerance.get());
    }

    // Data members
    BOOST_READONLY_PROPERTY( FPT, 0, () )
                p_tolerance;
private:
    bool        m_strong_or_weak;
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************               check_is_closed                ************** //
// ************************************************************************** //

template<typename FPT, typename ToleranceSource>
bool
check_is_closed( FPT left, FPT right, ToleranceSource tolerance, floating_point_comparison_type fpc_type = FPC_STRONG )
{
    close_at_tolerance<FPT> pred( tolerance, fpc_type );

    return pred( left, right );
}

//____________________________________________________________________________//

template<typename FPT, typename ToleranceSource>
FPT
compute_tolerance( ToleranceSource tolerance, FPT /* unfortunately we need to pass type information this way*/ )
{
    close_at_tolerance<FPT> pred( tolerance );

    return pred.p_tolerance.get();
}

//____________________________________________________________________________//

} // namespace test_toolbox
} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.8  2003/06/09 08:43:47  rogeeff
//  added comparison type and algorithm constructor accepting it
//

// ***************************************************************************

#endif // BOOST_FLOATING_POINT_COMAPARISON_HPP
