//  (C) Copyright Gennadiy Rozental 2002-2003.
//  See accompanying license for terms and conditions of use.

//  See http://www.boost.org for most recent version including documentation.
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

namespace boost {

// ************************************************************************** //
// **************               wrap_stringstream              ************** //
// ************************************************************************** //

class wrap_stringstream {
#ifdef BOOST_NO_STRINGSTREAM
    typedef std::ostrstream     wrapped_stream;
#else
    typedef std::ostringstream  wrapped_stream;
#endif // BOOST_NO_STRINGSTREAM
public:

    // access methods
    wrap_stringstream&          ref();
    wrapped_stream&             stream();
    std::string const&          str();

private:
    // Data members
    wrapped_stream              m_stream;
    std::string                 m_str;
};

//____________________________________________________________________________//

template <class T>
inline wrap_stringstream&
operator<<( wrap_stringstream& targ, T const& t )
{
    targ.stream() << t;
    return targ;
}

//____________________________________________________________________________//

inline wrap_stringstream::wrapped_stream&
wrap_stringstream::stream()
{
    return m_stream;
}

//____________________________________________________________________________//

inline wrap_stringstream&
wrap_stringstream::ref()
{ 
    return *this;
}

//____________________________________________________________________________//

inline std::string const&
wrap_stringstream::str()
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

inline wrap_stringstream&
operator<<( wrap_stringstream& targ, wrap_stringstream& src )
{
    targ << src.str();
    return targ;
}

//____________________________________________________________________________//

inline wrap_stringstream&
operator<<( wrap_stringstream& targ, std::ios_base& (*man)(std::ios_base&) )
{
	targ.stream() << man;
	return targ;
}

//____________________________________________________________________________//

template<typename Elem,typename Tr>
inline wrap_stringstream&
operator<<( wrap_stringstream& targ, std::basic_ostream<Elem,Tr>& (*man)(std::basic_ostream<Elem, Tr>&) )
{
	targ.stream() << man;
	return targ;
}

//____________________________________________________________________________//

template<typename Elem,typename Tr>
inline wrap_stringstream&
operator<<( wrap_stringstream& targ, std::basic_ios<Elem, Tr>& (*man)(std::basic_ios<Elem, Tr>&) )
{
	targ.stream() << man;
	return targ;
}

//____________________________________________________________________________//

}  // namespace boost

// ***************************************************************************
//  Revision History :
//  
//  $Log$
//  Revision 1.6  2003/11/02 06:21:57  rogeeff
//  added stl manipulators support
//
//  Revision 1.5  2003/10/27 07:13:13  rogeeff
//  licence update
//
//  Revision 1.4  2003/07/09 21:22:24  jmaurer
//  define str() before it is first used to avoid "redeclared inline after
//  begin called" error on IRIX MIPSpro
//
//  Revision 1.3  2003/06/09 08:39:28  rogeeff
//  1.30.beta1
//

// ***************************************************************************

#endif  // BOOST_WRAP_STRINGSTREAM_HPP
