//  (C) Copyright Gennadiy Rozental 2001-2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : output_test_stream class definition
// ***************************************************************************

#ifndef BOOST_TEST_OUTPUT_TEST_STREAM_HPP_012705GER
#define BOOST_TEST_OUTPUT_TEST_STREAM_HPP_012705GER

// Boost.Test
#include <boost/test/detail/unit_test_config.hpp>
#include <boost/test/utils/wrap_stringstream.hpp>

// Boost
#include <boost/config.hpp>  // compilers workarounds
#include <boost/shared_ptr.hpp>

// STL
#include <cstddef>          // for std::size_t

#include <boost/test/detail/suppress_warnings.hpp>

// ************************************************************************** //
// **************               output_test_stream             ************** //
// ************************************************************************** //

// class to be used to simplify testing of ostream-based output operations

namespace boost {

namespace test_tools {

class output_test_stream : public wrap_stringstream::wrapped_stream
{
    typedef boost::unit_test::const_string      const_string;
    typedef predicate_result                    result_type;
public:
    // Constructor
    explicit        output_test_stream( const_string    pattern_file_name = const_string(),
                                        bool            match_or_save     = true );

    // Destructor
    ~output_test_stream();

    // checking function
    result_type     is_empty( bool flush_stream_ = true );
    result_type     check_length( std::size_t length_, bool flush_stream_ = true );
    result_type     is_equal( const_string arg_, bool flush_stream_ = true );
    result_type     match_pattern( bool flush_stream_ = true );

    // explicit flush
    void            flush();

private:
    // helper functions
    std::size_t     length();
    void            sync();

    struct Impl;
    Impl*           m_pimpl;
};

} // namespace test_tools

} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

// ***************************************************************************
//  Revision History :
//
//  $Log$
//  Revision 1.2  2005/02/01 06:40:06  rogeeff
//  copyright update
//  old log entries removed
//  minor stilistic changes
//  depricated tools removed
//
//  Revision 1.1  2005/01/30 03:25:24  rogeeff
//  output_test_stream moved into separate file
//
// ***************************************************************************

#endif // BOOST_TEST_OUTPUT_TEST_STREAM_HPP_012705GER
