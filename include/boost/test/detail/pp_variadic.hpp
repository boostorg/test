//  (C) Copyright Gennadiy Rozental 2001.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//!@file
//!@brief few helpers for working with variadic macros
// ***************************************************************************

#ifndef BOOST_TEST_PP_VARIADIC_HPP_021515GER
#define BOOST_TEST_PP_VARIADIC_HPP_021515GER

#include <boost/test/detail/config.hpp>

#if BOOST_TEST_USE_BOOST==0

// BOOST_TEST_PP_NARG(...) argument count

// BOOST_TEST_PP_NARG returns the number of arguments of a variable argument list
// https://stackoverflow.com/a/2124385/1617295
#if defined(__GNUC__)
  #define BOOST_TEST_PP_NARG(...)     BOOST_TEST_PP_NARG_(DUMMY, ##__VA_ARGS__, BOOST_TEST_PP_RSEQ_N_WITH_DUMMY())
#elif defined(MSVC)
  #define BOOST_TEST_PP_NARG(...)     BOOST_TEST_PP_NARG_(DUMMY, __VA_ARGS__, BOOST_TEST_PP_RSEQ_N_WITH_DUMMY())
#else
  #error not supported yet
#endif

#define BOOST_TEST_PP_NARG_(...)    BOOST_TEST_PP_ARG_N(__VA_ARGS__)
#define BOOST_TEST_PP_ARG_N(\
 _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16,\
_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,\
_33,_34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,\
_49,_50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,  N, ...) N

#define BOOST_TEST_PP_RSEQ_N()\
63,62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,\
47,46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,\
31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,\
15,14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1

// same as BOOST_TEST_PP_RSEQ_N but shifted according to an existing dummy data
#define BOOST_TEST_PP_RSEQ_N_WITH_DUMMY()\
62,61,60,59,58,57,56,55,54,53,52,51,50,49,48,47,\
46,45,44,43,42,41,40,39,38,37,36,35,34,33,32,31,\
30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,\
14,13,12,11,10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

// BOOST_TEST_PP_IIF(bit, t, f) -> if bit == 1, expands to t, otherwise to f

#define CAT(a, ...) PRIMITIVE_CAT(a, __VA_ARGS__)
#define PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__

#define COMPL(b) PRIMITIVE_CAT(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0

#define CHECK_N(x, n, ...) n
#define CHECK(...) CHECK_N(__VA_ARGS__, 0,)
#define PROBE(x) x, 1,
#define NOT(x) CHECK(PRIMITIVE_CAT(NOT_, x))
#define NOT_0 PROBE(~)
#define BOOL(x) COMPL(NOT(x))


#define BOOST_TEST_PP_IIF(bit, t, f) BOOST_TEST_PP_IIF_I(BOOL(bit), t, f)
#define BOOST_TEST_PP_IIF_I(bit, t, f) CAT(BOOST_TEST_PP_IIF_, bit)(t, f)
#define BOOST_TEST_PP_IIF_0(t, f) f
#define BOOST_TEST_PP_IIF_1(t, f) t

#else /* BOOST_TEST_USE_BOOST==0 */

// Boost
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/comparison/equal.hpp>
#include <boost/preprocessor/variadic/size.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_TEST_PP_IF(x, t, f) BOOST_PP_IF(x, t, f)
#define BOOST_TEST_PP_IIF(x, t, f) BOOST_PP_IIF(x, t, f)
#define BOOST_TEST_PP_EQUAL(a, b) BOOST_PP_EQUAL(a, b)

#if defined(BOOST_PP_VARIADIC_SIZE)
#  define BOOST_TEST_PP_NARG BOOST_PP_VARIADIC_SIZE
#endif

//____________________________________________________________________________//

#if BOOST_PP_VARIADICS

#if BOOST_PP_VARIADICS_MSVC
#  define BOOST_TEST_INVOKE_VARIADIC( tool, ... ) BOOST_PP_CAT( tool (__VA_ARGS__), )
#else
#  define BOOST_TEST_INVOKE_VARIADIC( tool, ... ) tool (__VA_ARGS__)
#endif

//____________________________________________________________________________//

/// if sizeof(__VA_ARGS__) == N: F1(__VA_ARGS__)
/// else:                        F2(__VA_ARGS__)
#define BOOST_TEST_INVOKE_IF_N_ARGS( N, F1, F2, ... )               \
    BOOST_TEST_INVOKE_VARIADIC(                                     \
        BOOST_TEST_PP_IIF(                                               \
            BOOST_TEST_PP_EQUAL(BOOST_TEST_PP_NARG(__VA_ARGS__), N), \
            F1,                                                     \
            F2),                                                    \
        __VA_ARGS__ )                                               \
/**/

//____________________________________________________________________________//

#endif /* BOOST_PP_VARIADICS */



#endif /* BOOST_TEST_USE_BOOST */

#endif // BOOST_TEST_PP_VARIADIC_HPP_021515GER

// EOF
