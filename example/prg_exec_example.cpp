//  (C) Copyright Gennadiy Rozental 2001-2003.
//  See accompanying license for terms and conditions of use.
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
