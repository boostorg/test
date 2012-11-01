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
//  Description : defines single element monomorphic dataset
// ***************************************************************************

#ifndef BOOST_TEST_DATA_MONOMORPHIC_SINGLETON_HPP_102211GER
#define BOOST_TEST_DATA_MONOMORPHIC_SINGLETON_HPP_102211GER

// Boost.Test
#include <boost/test/data/config.hpp>
#include <boost/test/data/size.hpp>
#include <boost/test/data/monomorphic/dataset.hpp>

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {

// ************************************************************************** //
// **************                    singleton                  ************** //
// ************************************************************************** //
// Models single element data set

template<typename T>
class singleton : public monomorphic::dataset<typename std::decay<T>::type> {
    typedef monomorphic::dataset<typename std::decay<T>::type> base;
    typedef typename base::iter_ptr  iter_ptr;

    struct iterator : public base::iterator {
        // Constructor
        explicit            iterator( singleton<T> const& owner ) : m_owner( owner ) {}

        // forward iterator interface 
        virtual typename base::data_type const&
                            operator*()     { return m_owner.value(); }
        virtual void        operator++()    {}

    private:
        singleton<T> const& m_owner;
    };

public:
    enum { arity = 1 };

    // Constructor
    explicit        singleton( T&& value ) : m_value( std::forward<T>( value ) ) {}

    // Move constructor
    singleton( singleton&& s ) : m_value( std::forward<T>( s.m_value ) ) {}

    // Access methods
    T const&        value() const                   { return m_value; }

    // dataset interface
    virtual data::size_t    size() const            { return 1; }
    virtual iter_ptr        begin() const           { return std::make_shared<iterator>( *this ); }

private:
    // Data members
    T               m_value;
};

//____________________________________________________________________________//

template<typename T>
struct is_dataset<singleton<T>> : std::true_type {};

} // namespace monomorphic

template<typename T>
inline typename std::enable_if<!ds_detail::is_std_collection<T>::value && 
                               !monomorphic::is_dataset<T>::value, 
                               monomorphic::singleton<T> >::type
make( T&& v )
{
    return monomorphic::singleton<T>( std::forward<T>( v ) );
}

//____________________________________________________________________________//

inline monomorphic::singleton<char*>
make( char* str )
{
    return monomorphic::singleton<char*>( std::forward<char*>( str ) );
}

//____________________________________________________________________________//

inline monomorphic::singleton<char const*>
make( char const* str )
{
    return monomorphic::singleton<char const*>( std::forward<char const*>( str ) );
}

//____________________________________________________________________________//

} // namespace data
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_DATA_MONOMORPHIC_SINGLETON_HPP_102211GER

