//  (C) Copyright Gennadiy Rozental 2001-2003.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

int add( int i, int j ) { return i+j; }

int cpp_main( int, char *[] )  // note the name!
{ 
    // two ways to detect and report the same error:
    if ( add(2,2) != 4 ) throw "Oops..."; // #1 throws on error

    return add(2,2) == 4 ? 15 : 1;         // #2 returns error directly
}

// EOF
