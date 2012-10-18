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
//  Description : defines dataset join operation
// ***************************************************************************

#ifndef BOOST_TEST_DATA_MONOMORPHIC_JOIN_HPP_112711GER
#define BOOST_TEST_DATA_MONOMORPHIC_JOIN_HPP_112711GER

// Boost.Test
#include <boost/test/data/config.hpp>
#include <boost/test/data/size.hpp>
#include <boost/test/data/monomorphic/dataset.hpp>

// Boost
#include <boost/utility/enable_if.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {

// ************************************************************************** //
// **************                      join                    ************** //
// ************************************************************************** //

template<typename DS1, typename DS2>
class join : public monomorphic::dataset<typename std::decay<DS1>::type::data_type> {
    typedef typename std::decay<DS1>::type::data_type T;
    typedef monomorphic::dataset<T> base;

    struct iterator : public base::iterator {
        // Constructor
        explicit    iterator( iter_ptr it1, iter_ptr it2, data::size_t first_size )
        : m_it1( std::move(it1) )
        , m_it2( std::move(it2) )
        , m_first_size( first_size )
        {}

        // forward iterator interface 
        virtual T const&    operator*()     { return m_first_size > 0 ? **m_it1 : **m_it2; }
        virtual void        operator++()    { m_first_size > 0 ? (--m_first_size,++(*m_it1)) : ++(*m_it2); }

    private:
        // Data members
        iter_ptr        m_it1;
        iter_ptr        m_it2;
        data::size_t    m_first_size;
    };

public:
    enum { arity = std::decay<DS1>::type::arity };

    // Constructor
    join( DS1&& ds1, DS2&& ds2 )
    : m_ds1( std::forward<DS1>( ds1 ) )
    , m_ds2( std::forward<DS2>( ds2 ) )
    {}

    // Move constructor
    join( join&& j ) 
    : m_ds1( std::forward<DS1>( j.m_ds1 ) )
    , m_ds2( std::forward<DS2>( j.m_ds2 ) )
    {}

    // dataset interface
    virtual data::size_t    size() const            { return m_ds1.size() + m_ds2.size(); } 
    virtual iter_ptr        begin() const           { return std::make_shared<iterator>( m_ds1.begin(), 
                                                                                         m_ds2.begin(),
                                                                                         m_ds1.size() ); }

private:
    // Data members
    DS1 m_ds1;
    DS2 m_ds2;
};

//____________________________________________________________________________//

template<typename DS1, typename DS2>
struct is_dataset<join<DS1,DS2>> : std::true_type {};

//____________________________________________________________________________//

namespace ds_detail {

template<typename DS1, typename DS2>
struct explicit_join_type
{
    typedef join<DS1,DS2> type;
};

} // ds_detail

template<typename DS1, typename DS2>
inline typename boost::lazy_enable_if<mpl::and_<is_dataset<DS1>,is_dataset<DS2>>, 
                                      ds_detail::explicit_join_type<DS1,DS2>>::type
operator+( DS1&& ds1, DS2&& ds2 )
{
    return join<DS1,DS2>( std::forward<DS1>( ds1 ),  std::forward<DS2>( ds2 ) );
}

//____________________________________________________________________________//

template<typename DS1, typename DS2>
inline auto 
operator+( DS1&& ds1, DS2&& ds2 ) ->
typename std::enable_if<is_dataset<DS1>::value && !is_dataset<DS2>::value, decltype(ds1 + data::make(ds2))>::type
{
    return ds1 + data::make(ds2);
}

//____________________________________________________________________________//

template<typename DS1, typename DS2>
inline auto 
operator+( DS1&& ds1, DS2&& ds2 ) ->
typename std::enable_if<!is_dataset<DS1>::value && is_dataset<DS2>::value, decltype(data::make(ds1) + ds2)>::type
{
    return data::make(ds1) + ds2;
}

//____________________________________________________________________________//

} // namespace monomorphic

} // namespace data
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_DATA_MONOMORPHIC_JOIN_HPP_112711GER

