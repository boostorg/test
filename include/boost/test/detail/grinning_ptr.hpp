//  (C) Copyright Gennadiy Rozental 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

#ifndef BOOST_UNIT_TEST_GRINNING_PTR_HPP
#define BOOST_UNIT_TEST_GRINNING_PTR_HPP

#include <boost/utility.hpp>

namespace boost {

namespace unit_test_framework {

namespace detail {

//  ginning_ptr<> is a fusion of boost::scoped_ptr<> and
//
//   Alan Griffith grin_ptr<> (http://www.octopull.demon.co.uk/arglib/TheGrin.html)
//
//  ginning_ptr mimics a built-in pointer except that it guarantees deletion
//  of the object pointed to, either on destruction of the ginning_ptr or via
//  an explicit reset().  ginning_ptr is a simple solution for the implementation
//  of the PIMPL idiom that works with the Borland compiler (unlike scoped_ptr);
//
//  Alan's technique allows the compiler to parse ~ginning_ptr() without compiling
//  delete p ; and therefore without failing if T is an incomplete type,
//  which is the case with the pimpl idiom.
//  delete p is parsed only when ginning_ptr<> is constructed, which in the
//  case of the pimpl idiom is on the implemetation file when T is complete.
//
//  Sice ~ginning_ptr() doesn't seen (statically) "delete p", this smart pointer
//  allows the pimpl idiom to be safely implemented even for Borlands C++ 5.5.1
//  (which erroneously parses the pimpl destructor while T is still incomplete)
//
// NOTE: checked_delete(p) is used instead of delete p to catch the
// case where the constructor of the outer handle class is visible
// before T is complete
// (something that should happen in a pimpl implementation, though)
//
template<typename T>
class grinning_ptr : noncopyable {
public:
    typedef T       element_type;
    
    explicit        grinning_ptr( T* p=0 ) 
    : ptr(p) , do_delete(do_delete_impl)        {}                  // never throws
    ~grinning_ptr()                             { do_delete(ptr); }
    void            reset( T* p=0 )             { if ( ptr != p ) { do_delete(ptr); ptr = p; } }
    T&              operator*() const           { return *ptr; }    // never throws
    T*              operator->() const          { return ptr; }     // never throws
    T*              get() const                 { return ptr; }     // never throws
private:
    typedef void (*do_delete_func) ( T* p ) ;
    static void     do_delete_impl( T* p )      { checked_delete(p) ; }

    T*              ptr;
    do_delete_func  do_delete ;
};  // grinning_ptr

} // namespace detail

} // namespace unit_test_framework

} // namespace boost

//  Revision History
//   21 Dic 01  Initial version (Fernando Cacciola)
//   24 Dic 01  Reformat a bit  (Gennadiy Rozental)

#endif // BOOST_UNIT_TEST_GRINNING_PTR_HPP
