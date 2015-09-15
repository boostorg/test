//  (C) Copyright Gennadiy Rozental 2011-2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
///@file
///Defines monomorphic dataset based on C++11 initializer_list template
// ***************************************************************************

#ifndef BOOST_TEST_DATA_MONOMORPHIC_INITIALIZATION_LIST_HPP_091515GER
#define BOOST_TEST_DATA_MONOMORPHIC_INITIALIZATION_LIST_HPP_091515GER

// Boost.Test
#include <boost/test/data/config.hpp>
#include <boost/test/data/monomorphic/dataset.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {

// ************************************************************************** //
// **************                     array                    ************** //
// ************************************************************************** //

/// Dataset view of a C array
template<typename T>
class init_list : public monomorphic::dataset<T> {
    typedef monomorphic::dataset<T> base;
    typedef typename base::iter_ptr iter_ptr;

    struct iterator : public base::iterator {
        // Constructor
        explicit    iterator( T const* begin, data::size_t size )
        : m_it( begin )
        , m_singleton( size == 1 )
        {}

        // forward iterator interface
        virtual T const&    operator*()     { return *m_it; }
        virtual void        operator++()    { if( !m_singleton ) ++m_it; }

    private:
        // Data members
        T const*            m_it;
        bool                m_singleton;
    };

public:
    enum { arity = 1 };

    // Constructor
    init_list( std::initializer_list<T>&& il )
    : m_data( std::forward<std::initializer_list<T>>( il ) )
    {}

    // dataset interface
    virtual data::size_t    size() const            { return m_data.size(); }
    virtual iter_ptr        begin() const           { return boost::make_shared<iterator>( m_data.begin(), m_data.size() ); }

private:
    // Data members
    std::initializer_list<T> m_data;    
};

//____________________________________________________________________________//

//! An array dataset is a dataset
template<typename T>
struct is_dataset<init_list<T>> : mpl::true_ {};

} // namespace monomorphic

//____________________________________________________________________________//

//! @overload boost::unit_test::data::make()
template<typename T>
inline monomorphic::init_list<T>
make( std::initializer_list<T>&& il )
{
    return monomorphic::init_list<T>( std::forward<std::initializer_list<T>>( il ) );
}

} // namespace data
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_DATA_MONOMORPHIC_INITIALIZATION_LIST_HPP_091515GER

