//  (C) Copyright Gennadiy Rozental 2001-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file 
//!@brief algorithms for comparing floating point values
// ***************************************************************************

#ifndef BOOST_TEST_FLOATING_POINT_COMPARISON_HPP_071894GER
#define BOOST_TEST_FLOATING_POINT_COMPARISON_HPP_071894GER

// Boost.Test
#include <boost/test/detail/global_typedef.hpp>
#include <boost/test/tools/assertion_result.hpp>

// Boost
#include <boost/limits.hpp>  // for std::numeric_limits
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/type_traits/is_floating_point.hpp>

// STL
#include <iosfwd>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace math { 
namespace fpc {

// ************************************************************************** //
// **************              fpc::tolerance_based            ************** //
// ************************************************************************** //

template<typename T>
struct tolerance_based : mpl::bool_<
    is_floating_point<T>::value ||
    (std::numeric_limits<T>::is_specialized &&
    !std::numeric_limits<T>::is_integer &&
    !std::numeric_limits<T>::is_exact) > {};

// ************************************************************************** //
// **************                 fpc::strength                ************** //
// ************************************************************************** //

//! Method for comparison of floating point numbers
enum strength {
    FPC_STRONG, //!< "Very close"   - equation 2' in docs, the default
    FPC_WEAK    //!< "Close enough" - equation 3' in docs.
};


// ************************************************************************** //
// **************         tolerance presentation types         ************** //
// ************************************************************************** //

template<typename FPT>
struct percent_tolerance_t {
    explicit    percent_tolerance_t( FPT v ) : m_value( v ) {}

    FPT m_value;
};

//____________________________________________________________________________//

template<typename FPT>
inline std::ostream& operator<<( std::ostream& out, percent_tolerance_t<FPT> t )
{
    return out << t.m_value;
}

//____________________________________________________________________________//

template<typename FPT>
inline percent_tolerance_t<FPT>
percent_tolerance( FPT v )
{
    return percent_tolerance_t<FPT>( v );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                    details                   ************** //
// ************************************************************************** //

namespace fpc_detail {

// FPT is Floating-Point Type: float, double, long double or User-Defined.
template<typename FPT>
inline FPT
fpt_abs( FPT fpv ) 
{
    return fpv < static_cast<FPT>(0) ? -fpv : fpv;
}

//____________________________________________________________________________//

template<typename FPT>
struct fpt_limits {
    static FPT    min_value()
    {
        return std::numeric_limits<FPT>::is_specialized
                    ? (std::numeric_limits<FPT>::min)()
                    : static_cast<FPT>(0);
    }
    static FPT    max_value()
    {
        return std::numeric_limits<FPT>::is_specialized
                    ? (std::numeric_limits<FPT>::max)()
                    : static_cast<FPT>(1000000); // for our purposes it doesn't really matter what value is returned here
    }
};

//____________________________________________________________________________//

// both f1 and f2 are unsigned here
template<typename FPT>
inline FPT
safe_fpt_division( FPT f1, FPT f2 )
{
    // Avoid overflow.
    if( (f2 < static_cast<FPT>(1))  && (f1 > f2*fpt_limits<FPT>::max_value()) )
        return fpt_limits<FPT>::max_value();

    // Avoid underflow.
    if( (f1 == static_cast<FPT>(0)) ||
        ((f2 > static_cast<FPT>(1)) && (f1 < f2*fpt_limits<FPT>::min_value())) )
        return static_cast<FPT>(0);

    return f1/f2;
}

//____________________________________________________________________________//

template<typename FPT, typename ToleranceType>
inline FPT
fraction_tolerance( ToleranceType tolerance )
{
  return static_cast<FPT>(tolerance);
} 

//____________________________________________________________________________//

template<typename FPT2, typename FPT>
inline FPT2
fraction_tolerance( percent_tolerance_t<FPT> tolerance )
{
    return static_cast<FPT2>(tolerance.m_value)*static_cast<FPT2>(0.01); 
}

//____________________________________________________________________________//

} // namespace fpc_detail

// ************************************************************************** //
// **************             close_at_tolerance               ************** //
// ************************************************************************** //


/*!@brief Helper function object for comparing floating points
 * @internal
 *
 * This function object is used to compare floating points with a tolerance and to store the 
 * part failing the tolerance (@ref close_at_tolerance::failed_fraction).
 *
 * The methods for comparing floating points are detailed in the documentation. The method is chosen
 * by the fpc::strength given at construction. The 
 *
 * If the comparison is part of a negation, the @c negate should be provided to the @c operator() instead
 * 
 */
template<typename FPT>
class close_at_tolerance {
public:
    // Public typedefs
    typedef bool result_type;

    // Constructor
    template<typename ToleranceType>
    explicit    close_at_tolerance( ToleranceType tolerance, fpc::strength fpc_strength = FPC_STRONG ) 
    : m_fraction_tolerance( fpc_detail::fraction_tolerance<FPT>( tolerance ) )
    , m_strength( fpc_strength )
    , m_failed_fraction()
    {
        BOOST_ASSERT_MSG( m_fraction_tolerance >= 0, "tolerance must not be negative!" ); // no reason for tolerance to be negative
    }

    // Access methods
    FPT                 fraction_tolerance() const  { return m_fraction_tolerance; }
    fpc::strength       strength() const            { return m_strength; }
    FPT                 failed_fraction() const     { return m_failed_fraction; }

    /*! Compares two floating points according to comparison method and against a tolerance.
     *
     *  @param[in] left first floating point to be compared
     *  @param[in] right second floating point to be compared
     *  @param[in] negate if true, indicates that this comparison is part of a negation. This is used 
     *             to store the part of the comparison that is failing (for the reports).
     *
     * For the comparison method "close enough", the failing fraction is stored. If both fractions are failing
     * the minimum of those fractions is stored. For the comparison method "very close", the minimum of those
     * fraction is stored.
     */
    bool                operator()( FPT left, FPT right, bool negate = false ) const
    {
        FPT diff              = fpc_detail::fpt_abs<FPT>( left - right );
        FPT fraction_of_right = fpc_detail::safe_fpt_division( diff, fpc_detail::fpt_abs( right ) );
        FPT fraction_of_left  = fpc_detail::safe_fpt_division( diff, fpc_detail::fpt_abs( left ) );

        bool method_is_strong = (m_strength == FPC_STRONG) ^ negate;

        bool res = method_is_strong ? 
              (fraction_of_right <= m_fraction_tolerance && fraction_of_left <= m_fraction_tolerance) 
            : (fraction_of_right <= m_fraction_tolerance || fraction_of_left <= m_fraction_tolerance);

        res ^= negate;

        if( !res )
        {
          if(method_is_strong)
          {
            m_failed_fraction = std::min(fraction_of_left, fraction_of_right);
          }
          else
          {
            // if they fail both, we show the minimum
            m_failed_fraction = (fraction_of_right > m_fraction_tolerance ? 
                ( (fraction_of_left > m_fraction_tolerance) ? 
                    std::min(fraction_of_left, fraction_of_right) 
                    : fraction_of_right) 
                : fraction_of_left);
          }
        }

        return res;
    }

private:
    // Data members
    FPT                 m_fraction_tolerance;
    fpc::strength       m_strength;
    mutable FPT         m_failed_fraction;
};

// ************************************************************************** //
// **************            small_with_tolerance              ************** //
// ************************************************************************** //

template<typename FPT>
class small_with_tolerance {
public:
    // Public typedefs
    typedef bool result_type;

    // Constructor
    explicit    small_with_tolerance( FPT tolerance ) // <= absolute tolerance
    : m_tolerance( tolerance )
    {
        BOOST_ASSERT( m_tolerance >= 0 ); // no reason for the tolerance to be negative
    }

    // Action method
    bool        operator()( FPT fpv ) const
    {
        return fpc::fpc_detail::fpt_abs( fpv ) < m_tolerance;
    }

private:
    // Data members
    FPT         m_tolerance;
};

// ************************************************************************** //
// **************                  is_small                    ************** //
// ************************************************************************** //

template<typename FPT>
inline bool
is_small( FPT fpv, FPT tolerance )
{
    return small_with_tolerance<FPT>( tolerance )( fpv );
}

//____________________________________________________________________________//

} // namespace fpc
} // namespace math
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_FLOATING_POINT_COMAPARISON_HPP_071894GER
