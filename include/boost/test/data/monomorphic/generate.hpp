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
//  Description : defines generic interface for monomorphic dataset based on generator
// ***************************************************************************

#ifndef BOOST_TEST_DATA_MONOMORPHIC_GENERATE_HPP_112011GER
#define BOOST_TEST_DATA_MONOMORPHIC_GENERATE_HPP_112011GER

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
// **************                  generated_by                ************** //
// ************************************************************************** //

template<typename Generator>
class generated_by : public monomorphic::dataset<typename Generator::data_type> {
    typedef typename Generator::data_type T;
    typedef monomorphic::dataset<T> base;
    typedef typename base::iter_ptr iter_ptr;

    struct iterator : public base::iterator {
        // Constructor
        explicit    iterator( Generator& gen )
        : m_gen( gen )
        {}

        // forward iterator interface 
        virtual T const&    operator*()     { m_curr_sample = m_gen.next(); return m_curr_sample; }
        virtual void        operator++()    {}

    private:
        // Data members
        Generator&          m_gen;
        T                   m_curr_sample;
    };
public:
    enum { arity = 1 };
    typedef Generator generator_type;

    // Constructor
    explicit                generated_by( Generator&& G )
    : m_generator( std::forward<Generator>(G) )
    {}

    // Access methods
    data::size_t            size() const            { return m_generator.capacity(); }
    virtual iter_ptr        begin() const           { return std::make_shared<iterator>( const_cast<Generator&>(m_generator) ); }

private:
    // Data members
    Generator               m_generator;
};

//____________________________________________________________________________//

template<typename Generator>
struct is_dataset<generated_by<Generator>> : std::true_type {};

//____________________________________________________________________________//

} // namespace monomorphic
} // namespace data
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_DATA_MONOMORPHIC_GENERATE_HPP_112011GER

