//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Use, modification, and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : 
// ***************************************************************************

#ifndef BOOST_ALGORITHM_HPP
#define BOOST_ALGORITHM_HPP

#include <utility>

namespace boost {

namespace unit_test {

template <class InputIter1, class InputIter2>
inline std::pair<InputIter1, InputIter2>
mismatch( InputIter1 first1, InputIter1 last1,
          InputIter2 first2, InputIter2 last2 )
{
    while( first1 != last1 && first2 != last2 && *first1 == *first2 ) {
        ++first1;
        ++first2;
    }

    return std::pair<InputIter1, InputIter2>(first1, first2);
}

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2004/05/11 11:00:53  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//

// ***************************************************************************

#endif // BOOST_ALGORITHM_HPP

