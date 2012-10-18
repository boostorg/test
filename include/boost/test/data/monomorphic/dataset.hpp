//  (C) Copyright Gennadiy Rozental 2011-2012.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines monomorphic dataset interface
// ***************************************************************************

#ifndef BOOST_TEST_DATA_MONOMORPHIC_DATASET_HPP_102211GER
#define BOOST_TEST_DATA_MONOMORPHIC_DATASET_HPP_102211GER

// Boost.Test
#include <boost/test/data/config.hpp>
#include <boost/test/data/size.hpp>

// STL
#include <vector>
#include <list>
#include <tuple>

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

// !! ?? reimplement using variadics
template<typename T1, typename T2>
struct traits<std::tuple<T1,T2>> {
    // type of the reference to sample returned by iterator
    typedef std::tuple<T1 const&,T2 const&>     ref_type;

    template<typename Action>
    static void
    invoke_action( ref_type arg, Action const& action )
    {
        action( get<0>(arg), get<1>(arg) );
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
        action( get<0>(arg), get<1>(arg), get<2>(arg) );
    }
};

//____________________________________________________________________________//

// ************************************************************************** //
// **************             monomorphic::dataset             ************** //
// ************************************************************************** //

template<typename T>
class dataset {
public:
    // type of the data in this dataset
    typedef T data_type;

    // type of dataset iterator
    class iterator {
    public:
        typedef typename monomorphic::traits<T>::ref_type ref_type;

        virtual             ~iterator() {}

        // forward iterator interface 
        virtual ref_type    operator*() = 0;
        virtual void        operator++() = 0;
    };

    typedef std::shared_ptr<iterator> iter_ptr;

    // dataset size
    virtual data::size_t    size() const = 0;
    // iterator to use to iterate over this dataset
    virtual iter_ptr        begin() const = 0;
};

// ************************************************************************** //
// **************            monomorphic::is_dataset           ************** //
// ************************************************************************** //

template<typename DS>
struct is_dataset : std::false_type {};

//____________________________________________________________________________//

template<typename DS>
struct is_dataset<DS&> : is_dataset<DS> {};

//____________________________________________________________________________//

template<typename DS>
struct is_dataset<DS const> : is_dataset<DS> {};

//____________________________________________________________________________//

} // namespace monomorphic

// ************************************************************************** //
// **************                  data::make                  ************** //
// ************************************************************************** //

template<typename DS>
inline typename std::enable_if<monomorphic::is_dataset<DS>::value,DS>::type
make(DS&& ds)
{
    return std::move( ds );
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************                for_each_sample               ************** //
// ************************************************************************** //

template<typename SampleType, typename Action>
inline void
for_each_sample( monomorphic::dataset<SampleType> const& ds,
                 Action const&                           act,
                 data::size_t                            number_of_samples = BOOST_TEST_DS_INFINITE_SIZE )
{
    auto size = (std::min)( ds.size(), number_of_samples );
    BOOST_TEST_DS_ASSERT( !size.is_inf(), "Dataset has infinite size. Please specify the number of samples" );

    auto it = ds.begin();

    while( size-- > 0 ) {
        monomorphic::traits<SampleType>::invoke_action( **it, act );
        ++(*it);
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

namespace ds_detail {

// ************************************************************************** //
// **************              is_std_collection               ************** //
// ************************************************************************** //

template<typename T>
struct is_std_collection : std::false_type {};

template<typename T>
struct is_std_collection<T const> : is_std_collection<T> {};

template<typename T>
struct is_std_collection<T&> : is_std_collection<T> {};

template<typename T>
struct is_std_collection<std::vector<T>> : std::true_type {};

template<typename T>
struct is_std_collection<std::list<T>> : std::true_type {};

} // namespace ds_detail

} // namespace data
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_DATA_MONOMORPHIC_DATASET_HPP_102211GER

