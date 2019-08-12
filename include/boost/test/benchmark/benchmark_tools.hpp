//  (C) Copyright Raffi Enficiaud 2019.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief Contains the tools for writting benchmark tests
// ***************************************************************************

#ifndef BOOST_TEST_BENCHMARK_TOOLS_HPP__
#define BOOST_TEST_BENCHMARK_TOOLS_HPP__

//! Registers a new benchmark to be performed
void register_benchmark(
  std::string const &benchmark_name,
  std::function<void ()> benchmark_function);



#define BOOST_TEST_BENCHMARK(benchmark_name) \
  struct benchmark ## benchmark_name { \
\
\
};


#if defined(BOOST_TEST_BENCHMARK_MODULE)
int main(int, char **) {

}
#endif


#endif /* benchmark_tools_h */
