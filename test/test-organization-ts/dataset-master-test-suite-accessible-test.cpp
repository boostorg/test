//  (C) Copyright Raffi Enficiaud 2018
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
/// @file
/// @brief tests the access to the master test suite from the datasets, trac 12953
// ***************************************************************************

#define BOOST_TEST_MODULE "Access master test suite arguments from datasets"
#include <boost/test/included/unit_test.hpp>

#include <boost/test/data/test_case.hpp>
#include <iostream>

class dataset_loader
{
public:
    typedef std::string sample;
    enum { arity = 1 };
  
    // this constructor access the master test suite
    dataset_loader();
    dataset_loader(std::initializer_list<std::string> init_list)
    : m_input_extension(init_list)
    {}
    dataset_loader(std::string s1, std::string s2)
    {
      m_input_extension.push_back(s1);
      m_input_extension.push_back(s2);
    }

    struct iterator {
        iterator(std::vector<std::string>::const_iterator const& v_iterator)
        : m_iterator(v_iterator)
        {}

        // bug in joins, see 13380, can safely be changed to std::string once fixed
        const std::string& operator*() const   { return *m_iterator; }
        void operator++()
        {
            ++m_iterator;
        }
    private:
        std::vector<std::string>::const_iterator m_iterator;
    };

    boost::unit_test::data::size_t size() const {
      return m_input_extension.size();
    }

    // iterator
    iterator        begin() const   { return iterator(m_input_extension.begin()); }
  
private:
    std::vector<std::string> m_input_extension;
};


dataset_loader::dataset_loader()
{
    BOOST_TEST_INFO("dataset_loader");

    int argc = boost::unit_test::framework::master_test_suite().argc;
    char** argv = boost::unit_test::framework::master_test_suite().argv;

    // not taking into account the name of the program (first argument)
    for(unsigned i = 1; i != argc; i++) {
        m_input_extension.push_back(argv[i]);
    }
}

//------------------------------------------------------------------------------

namespace boost { namespace unit_test { namespace data {

namespace monomorphic {
  template <>
  struct is_dataset<dataset_loader> : boost::mpl::true_ {};
}
}}}

BOOST_AUTO_TEST_SUITE( concrete_testsuite )

// parameters passed on the command line
char const* expected[] = {"--param1=1", "--param2=2"};

BOOST_DATA_TEST_CASE(master_access_zips,
    boost::unit_test::data::make_delayed<dataset_loader>( ) ^ boost::unit_test::data::make(expected),
    input, expect)
{
    // ...
    BOOST_TEST(input == expect);
}


BOOST_DATA_TEST_CASE(master_access_zips_flip,
    boost::unit_test::data::make_delayed<dataset_loader>( ) ^ dataset_loader({"--param1=1", "--param2=2"}),
    input, expect)
{
    BOOST_TEST(input == expect);
}

// checking the forward of the arguments
BOOST_DATA_TEST_CASE(master_access_zips_args_forward,
    boost::unit_test::data::make_delayed<dataset_loader>( "--param1=1", "--param2=2" ) ^ boost::unit_test::data::make_delayed<dataset_loader>( ),
    input, expect)
{
    BOOST_TEST(input == expect);
}

BOOST_DATA_TEST_CASE(master_access_grid,
    boost::unit_test::data::make_delayed<dataset_loader>( ) * dataset_loader({"--param1=1", "--param2=2"}),
    input, expect)
{
    BOOST_TEST((input == expect || input == "--param1=1" || expect == "--param1=1" ));
}

BOOST_DATA_TEST_CASE(master_access_grid_flip,
    dataset_loader({"--param1=1", "--param2=2"}) * boost::unit_test::data::make_delayed<dataset_loader>( ),
    input, expect)
{
    BOOST_TEST((input == expect || input == "--param1=1" || expect == "--param1=1" ));
}

int counter4 = 0;
BOOST_DATA_TEST_CASE(master_access_joins,
    boost::unit_test::data::make_delayed<dataset_loader>( ) + dataset_loader({"--param1=1", "--param2=2"}),
    input)
{
    static const std::string values[] = {"--param1=1", "--param2=2", "--param1=1", "--param2=2"};
    BOOST_TEST(input == values[counter4++]);
}

int counter41 = 0;
BOOST_DATA_TEST_CASE(master_access_joins_flip,
    dataset_loader({"--param2=2", "--param1=1"}) + boost::unit_test::data::make_delayed<dataset_loader>( ),
    input)
{
    static const std::string values[] = {"--param2=2", "--param1=1", "--param1=1", "--param2=2"};
    BOOST_TEST(input == values[counter41++]);
}

BOOST_AUTO_TEST_SUITE_END()
