//  (C) Copyright Gennadiy Rozental 2002-2003.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : wraps strstream and stringstream (depends with one is present )
//                to prodive the unified interface
// ***************************************************************************

#ifndef BOOST_WRAP_STRINGSTREAM_HPP
#define BOOST_WRAP_STRINGSTREAM_HPP

// STL
#ifdef BOOST_NO_STRINGSTREAM
#include <strstream>        // for std::ostrstream
#else
#include <sstream>          // for std::ostringstream
#endif // BOOST_NO_STRINGSTREAM

#ifdef BOOST_MSVC
# pragma warning(push)
# pragma warning(disable: 4511) // copy constructor could not be generated
# pragma warning(disable: 4512) // assignment operator could not be generated
#endif

namespace boost {

// ************************************************************************** //
// **************            basic_wrap_stringstream           ************** //
// ************************************************************************** //

template<typename CharT>
class basic_wrap_stringstream {
#ifdef BOOST_NO_STRINGSTREAM
    typedef std::basic_ostrstream<CharT>     wrapped_stream;
#else
    typedef std::basic_ostringstream<CharT>  wrapped_stream;
#endif // BOOST_NO_STRINGSTREAM
public:

    // access methods
    basic_wrap_stringstream&        ref();
    wrapped_stream&                 stream();
    std::basic_string<CharT> const& str();

private:
    // Data members
    wrapped_stream                  m_stream;
    std::basic_string<CharT>        m_str;
};

//____________________________________________________________________________//

template <typename CharT, typename T>
inline basic_wrap_stringstream<CharT>&
operator<<( basic_wrap_stringstream<CharT>& targ, T const& t )
{
    targ.stream() << t;
    return targ;
}

//____________________________________________________________________________//

template <typename CharT>
inline typename basic_wrap_stringstream<CharT>::wrapped_stream&
basic_wrap_stringstream<CharT>::stream()
{
    return m_stream;
}

//____________________________________________________________________________//

template <typename CharT>
inline basic_wrap_stringstream<CharT>&
basic_wrap_stringstream<CharT>::ref()
{ 
    return *this;
}

//____________________________________________________________________________//

template <typename CharT>
inline std::basic_string<CharT> const&
basic_wrap_stringstream<CharT>::str()
{

#ifdef BOOST_NO_STRINGSTREAM
    m_str.assign( m_stream.str(), m_stream.pcount() );
    m_stream.freeze( false );
#else
    m_str = m_stream.str();
#endif

    return m_str;
}

//____________________________________________________________________________//

template <typename CharT>
inline basic_wrap_stringstream<CharT>&
operator<<( basic_wrap_stringstream<CharT>& targ, basic_wrap_stringstream<CharT>& src )
{
    targ << src.str();
    return targ;
}

#ifndef BOOST_NO_STD_LOCALE
//____________________________________________________________________________//

template <typename CharT>
inline basic_wrap_stringstream<CharT>&
operator<<( basic_wrap_stringstream<CharT>& targ, std::ios_base& (*man)(std::ios_base&) )
{
    targ.stream() << man;
    return targ;
}

//____________________________________________________________________________//

template<typename CharT,typename Elem,typename Tr>
inline basic_wrap_stringstream<CharT>&
operator<<( basic_wrap_stringstream<CharT>& targ, std::basic_ostream<Elem,Tr>& (*man)(std::basic_ostream<Elem, Tr>&) )
{
    targ.stream() << man;
    return targ;
}

//____________________________________________________________________________//

template<typename CharT,typename Elem,typename Tr>
inline basic_wrap_stringstream<CharT>&
operator<<( basic_wrap_stringstream<CharT>& targ, std::basic_ios<Elem, Tr>& (*man)(std::basic_ios<Elem, Tr>&) )
{
    targ.stream() << man;
    return targ;
}

#endif
//____________________________________________________________________________//

// ************************************************************************** //
// **************               wrap_stringstream              ************** //
// ************************************************************************** //

typedef basic_wrap_stringstream<char>       wrap_stringstream;
typedef basic_wrap_stringstream<wchar_t>    wrap_wstringstream;

}  // namespace boost

#ifdef BOOST_MSVC
# pragma warning(default: 4511) // copy constructor could not be generated
# pragma warning(default: 4512) // assignment operator could not be generated
# pragma warning(pop)
#endif

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.11  2004/05/21 06:19:35  rogeeff
//  licence update
//
//  Revision 1.10  2004/05/11 11:00:53  rogeeff
//  basic_cstring introduced and used everywhere
//  class properties reworked
//
//  Revision 1.9  2003/12/01 00:41:56  rogeeff
//  prerelease cleaning
//
// ***************************************************************************

#endif  // BOOST_WRAP_STRINGSTREAM_HPP
