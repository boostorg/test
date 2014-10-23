//  (C) Copyright Jurko Gospodnetić 2014.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : unit test for UTF command-line argument parsing
// ***************************************************************************
// Tests marked with <REGRESSION #1> are regression tests against a command-line
// argument parsing implementation bug in Boost.Test causing a buffer overrun
// and thus triggering access-violation/segfault errors when one of the
// arguments starts with a space or the leading argument is empty.
//
// Test cases marked with <UNDESIRABLE RESULT #1> do not check the command-line
// arguments after they are processed by UTF because the current UTF
// implementation seems to mess them up.

// disable MSVC warnings about std::strcpy() being unsafe
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS 1
#endif

// Boost.Test
#define BOOST_TEST_MODULE Boost.Test command-line argument parsing test
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_parameters.hpp>

#include <cstring>
#include <vector>

namespace utf = boost::unit_test;

// Test utility class for checking UTF command-line argument processing results.
//
class ArgChecker
{
public:
    ArgChecker(int const input_data_count, char const * const input_data[])
        :
        argc_(input_data_count)
    {
        BOOST_REQUIRE_GT(input_data_count, 0);
        BOOST_REQUIRE(input_data);

        // calculate required argument data size and offsets
        std::vector<std::size_t> data_offset(input_data_count);
        std::size_t data_size = 0;
        for (int i = 0; i < input_data_count; ++i) {
            data_offset[i] = data_size;
            data_size += strlen(input_data[i]) + 1;
        }

        // prepare argument data (data + argv)
        data_.resize(data_size);
        argv_.resize(input_data_count + 1 + sentinel_count);
        for (int i = 0; i < input_data_count; ++i) {
            argv_[i] = &data_[data_offset[i]];
            std::strcpy(argv_[i], input_data[i]);
        }

        // argv[argc] must be NULL according to ANSI C standard
        argv_[argc_] = NULL;

        // add sentinel values for argv buffer overrun checking
        for (int i = 0; i < sentinel_count; ++i)
            argv_[argc_ + 1 + i] = const_cast<char *>(sentinel_value);

        // save original data & argv content
        saved_argc_ = argc_;
        saved_data_ = data_;
        saved_argv_ = argv_;

        // run actual UTF command-line argument processing
        utf::runtime_config::init(argc_, &argv_[0]);
    }

    // Check that the command-line arguments have not been changed while
    // processed by UTF. UTF may replace them with copies, but their content
    // must remain the same.
    //
    void check_unchanged() const
    {
        check(saved_argc_, &saved_argv_[0]);
    }

    // Check that the command-line arguments left behind after being processed
    // by UTF.
    //
    void check(int const expected_argc,
        char const * const expected_argv[]) const
    {
        BOOST_CHECK(saved_data_ == data_);  // input data unchanged
        BOOST_CHECK_EQUAL(expected_argc, argc_);
        if (expected_argc == argc_)
            for (int i = 0; i < argc_; ++i)
                BOOST_CHECK(!std::strcmp(expected_argv[i], argv_[i]));
        check_for_argv_overrun();
    }

    // Check that UTF command-line argument processing did not attempt to write
    // passed the end of the argv array.
    //
    void check_for_argv_overrun() const
    {
        for (int i = 0; i < sentinel_count; ++i)
            if (argv_[saved_argc_ + 1 + i] != sentinel_value) {
                BOOST_ERROR("argv buffer overrun detected at index " <<
                    saved_argc_ + 1 + i);
                return;
            }
    }

private:
    ArgChecker(ArgChecker const &);
    void operator=(ArgChecker const &) const;

private:
    static int const sentinel_count;
    static char const * const sentinel_value;

private:
    int argc_;
    std::vector<char> data_;
    std::vector<char *> argv_;
    int saved_argc_;
    std::vector<char> saved_data_;
    std::vector<char *> saved_argv_;
};

// argv buffer overrun testing configuration
//
// Keep the sentinel count large to reduce the chances of our test data causing
// a large enough buffer overrun to have it detected by OS or RTL's debug
// checks. This allows our more specialized checks to report such errors more
// clearly to the user.
int const ArgChecker::sentinel_count = 1000;
char const * const ArgChecker::sentinel_value = (char *)0xBAADF00D;

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE(empty_argument)

    //<REGRESSION #1>
    BOOST_AUTO_TEST_CASE(test_initial)
    {
        char * argv[] = {"a.exe", "", "x"};
        ArgChecker arg_checker(sizeof(argv)/sizeof(argv[0]), argv);
        arg_checker.check_unchanged();
    }

    BOOST_AUTO_TEST_CASE(test_inside)
    {
        char * argv[] = {"a.exe", "x", "", "y"};
        ArgChecker arg_checker(sizeof(argv)/sizeof(argv[0]), argv);
        arg_checker.check_unchanged();
    }

    BOOST_AUTO_TEST_CASE(test_inside_sequence)
    {
        char * argv[] = {"a.exe", "x", "", "", "y"};
        ArgChecker arg_checker(sizeof(argv)/sizeof(argv[0]), argv);
        arg_checker.check_unchanged();
    }

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE(argument_starting_with_space)

    //<REGRESSION #1>
    BOOST_AUTO_TEST_CASE(test_only)
    {
        char * argv[] = {"a.exe", " x"};
        ArgChecker arg_checker(sizeof(argv)/sizeof(argv[0]), argv);
        // <UNDESIRABLE RESULT #1> - should not split on embedded spaces
    }

    //<REGRESSION #1>
    BOOST_AUTO_TEST_CASE(test_initial)
    {
        char * argv[] = {"a.exe", " x", "y"};
        ArgChecker arg_checker(sizeof(argv)/sizeof(argv[0]), argv);
        // <UNDESIRABLE RESULT #1> - should not split on embedded spaces
    }

    //<REGRESSION #1>
    BOOST_AUTO_TEST_CASE(test_non_initial)
    {
        char * argv[] = {"a.exe", "x", " y"};
        ArgChecker arg_checker(sizeof(argv)/sizeof(argv[0]), argv);
        // <UNDESIRABLE RESULT #1> - should not split on embedded spaces
    }

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//

BOOST_AUTO_TEST_SUITE(non_empty_arguments_without_spaces)

    BOOST_AUTO_TEST_CASE(test_one)
    {
        char const * const argv[] = {"a.exe", "x"};
        ArgChecker arg_checker(sizeof(argv)/sizeof(argv[0]), argv);
        arg_checker.check_unchanged();
    }

    BOOST_AUTO_TEST_CASE(test_multiple)
    {
        char * argv[] = {"a.exe", "x", "y"};
        ArgChecker arg_checker(sizeof(argv)/sizeof(argv[0]), argv);
        arg_checker.check_unchanged();
    }

BOOST_AUTO_TEST_SUITE_END()

//____________________________________________________________________________//

// EOF
