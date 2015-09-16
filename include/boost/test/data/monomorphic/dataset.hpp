//  (C) Copyright Gennadiy Rozental 2011-2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// Defines monomorphic dataset interface
//
// ***************************************************************************

#ifndef BOOST_TEST_DATA_MONOMORPHIC_DATASET_HPP_102211GER
#define BOOST_TEST_DATA_MONOMORPHIC_DATASET_HPP_102211GER

// Boost.Test
#include <boost/test/data/config.hpp>
#include <boost/test/data/monomorphic/fwd.hpp>

// STL
#ifndef BOOST_NO_CXX11_HDR_TUPLE
#include <tuple>
#endif
//#include <stdexcept>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {

// ************************************************************************** //
// **************              monomorphic::traits             ************** //
// ************************************************************************** //

template<typename T>
struct traits {
    // type of the reference to sample returned by iterator
    typedef T const&                            ref_type;

    template<typename Action>
    static void
    invoke_action( ref_type arg, Action const& action )
    {
        action( arg );
    }
};

//____________________________________________________________________________//

#ifndef BOOST_NO_CXX11_HDR_TUPLE
// !! ?? reimplement using variadics
template<typename T1, typename T2>
struct traits<std::tuple<T1,T2>> {
    // type of the reference to sample returned by iterator
    typedef std::tuple<T1 const&,T2 const&>     ref_type;

    template<typename Action>
    static void
    invoke_action( ref_type arg, Action const& action )
    {
        action( std::get<0>(arg), std::get<1>(arg) );
    }
};

//____________________________________________________________________________//

template<typename T1, typename T2, typename T3>
struct traits<std::tuple<T1,T2,T3>> {
    // type of the reference to sample returned by iterator
    typedef std::tuple<T1 const&,T2 const&,T3 const&>   ref_type;

    template<typename Action>
    static void
    invoke_action( ref_type arg, Action const& action )
    {
        action( std::get<0>(arg), std::get<1>(arg), std::get<2>(arg) );
    }
};

//____________________________________________________________________________//

#endif

// ************************************************************************** //
// **************             monomorphic::dataset             ************** //
// ************************************************************************** //

//!@brief Dataset base class
//!
//! This class defines the dataset concept, which is an implementation of a sequence.
//! Each dataset should implement
//! - the @c size
//! - the @c begin function, which provides a forward iterator on the beginning of the sequence. The returned
//!   iterator should be incrementable a number of times corresponding to the returned size.
//!
template<typename T, typename SpecificDS>
class dataset {
public:
    SpecificDS const& specific() const  { return *static_cast<SpecificDS const*>(this); }
};

} // namespace monomorphic

// ************************************************************************** //
// **************                for_each_sample               ************** //
// ************************************************************************** //

template<typename SampleType, typename SpecificDS, typename Action>
inline void
for_each_sample( monomorphic::dataset<SampleType, SpecificDS> const& ds,
                 Action const&  act,
                 data::size_t   number_of_samples = BOOST_TEST_DS_INFINITE_SIZE )
{
    data::size_t size = (std::min)( ds.specific().size(), number_of_samples );
    BOOST_TEST_DS_ASSERT( !size.is_inf(), "Dataset has infinite size. Please specify the number of samples" );

    auto it = ds.specific().begin();

    while( size-- > 0 ) {
        monomorphic::traits<SampleType>::invoke_action( *it, act );
        ++it;
    }
}

//____________________________________________________________________________//

template<typename SampleType, typename Action>
inline typename std::enable_if<!monomorphic::is_dataset<SampleType>::value,void>::type
for_each_sample( SampleType const&  samples,
                 Action const&      act,
                 data::size_t       number_of_samples = BOOST_TEST_DS_INFINITE_SIZE )
{
    data::for_each_sample( data::make( samples ), act, number_of_samples );
}

//____________________________________________________________________________//

} // namespace data
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_DATA_MONOMORPHIC_DATASET_HPP_102211GER

