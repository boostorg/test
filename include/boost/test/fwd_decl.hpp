//  (C) Copyright Gennadiy Rozental 2005.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : contains forward eclarations for Boost.Test data types
// ***************************************************************************

#ifndef BOOST_FWD_DECL_HPP_011605GER
#define BOOST_FWD_DECL_HPP_011605GER

namespace boost {

namespace unit_test {

class  test_case;
class  unit_test_log_formatter;
struct log_entry_data;
struct log_checkpoint_data;
class  unit_test_log_t;

} // namespace unit_test

} // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.1  2005/01/18 08:27:30  rogeeff
//  unit_test_log rework:
//     eliminated need for ::instance()
//     eliminated need for << end and ...END macro
//     straitend interface between log and formatters
//     change compiler like formatter name
//     minimized unit_test_log interface and reworked to use explicit calls
//
// ***************************************************************************

#endif // BOOST_FWD_DECL_HPP_011605GER

