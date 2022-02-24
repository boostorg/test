//  (C) Copyright Raffi Enficiaud 2020.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.

// Checks the macro indirections

//#include <boost/test/detail/pp_variadic.hpp>


/* "clang++" -x c++ -fvisibility-inlines-hidden -fPIC -m64 -O0 -fno-inline -Wall -g -fvisibility=hidden -Werror=address-of-temporary -Werror=return-stack-address -Wno-c99-extensions -Wno-variadic-macros -DBOOST_ALL_NO_LIB=1 -DBOOST_TEST_NO_AUTO_LINK=1 -I"../../.." -c -o "../../../bin.v2/libs/test/test/helper-macros-test.test/clang-darwin-11.0/debug/threading-multi/visibility-hidden/utils-ts/helper-macros-test.o" "utils-ts/helper-macros-test.cpp"
*/ 

#define BOOST_TEST_USE_BOOST 0
#include <iostream>
#include <boost/test/detail/pp_variadic.hpp>

/*

#define REST(...)                 REST_HELPER(NUM(__VA_ARGS__), __VA_ARGS__)
#define REST_HELPER(qty, ...)     REST_HELPER2(qty, __VA_ARGS__)
#define REST_HELPER2(qty, ...)    REST_HELPER_##qty(__VA_ARGS__)
#define REST_HELPER_ONE(first)
#define REST_HELPER_TWOORMORE(first, ...) , __VA_ARGS__
#define NUM(...) \
    SELECT_10TH(__VA_ARGS__, TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE,\
                TWOORMORE, TWOORMORE, TWOORMORE, TWOORMORE, ONE, throwaway)
#define SELECT_10TH(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, ...) a10
*/


#if BOOST_TEST_USE_BOOST==1
#error not right
#endif

//BOOST_TEST_PP_NARG_(BOOST_TEST_PP_RSEQ_N())

int main() {
#if 0
#define TRUC6(...)     BOOST_TEST_PP_NARG_(SENTINEL, ##__VA_ARGS__, BOOST_TEST_PP_RSEQ_N_WITH_SENTINEL())

#define TRUC8(...)     toto_args_ ## TRUC6(__VA_ARGS__)

#define GET_FIRST(first, ...) first
#define MERGE(a, b, ...) a ## b
#define MERGE0(a, b, ...) MERGE(a, b, __VA_ARGS__)
#define TRUC3(...)     MERGE0(GET_FIRST(__VA_ARGS__, BOOST_TEST_PP_RSEQ_N()), BOOST_TEST_PP_RSEQ_N())


TRUC6()
TRUC6(0)
TRUC6(0,2,2,2,2,2)
TRUC6(PLOUF)

#define TRUC7(x) TRUC6(#x)

TRUC7(PLIF)
TRUC7()
#endif

  if(BOOST_TEST_PP_NARG() != 0) {
    std::cout << "non-zero variadic size: " << BOOST_TEST_PP_NARG() << std::endl;
    return 1;
  }

  if(BOOST_TEST_PP_NARG(a,b,c) != 3) {
    std::cout << "incorrect variadic size" << std::endl;
    return 1;
  }
    
    BOOST_TEST_PP_IIF(1,
                      (std::cout << "this is the true" << std::endl),
                      (std::cout << "this is the false" << std::endl));

    BOOST_TEST_PP_IIF(0,
                      (std::cout << "this is the true" << std::endl),
                      (std::cout << "this is the false" << std::endl));
    
    BOOST_TEST_PP_IIF(BOOST_TEST_PP_NARG(a,b,c) == 3,
                      (std::cout << "this is the true" << std::endl),
                      (std::cout << "this is the false" << std::endl));
  return 0;
}
