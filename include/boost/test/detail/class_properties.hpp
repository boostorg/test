//  (C) Copyright Gennadiy Rozental 2001.
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied warranty,
//  and with no claim as to its suitability for any purpose.
//
//  See http://www.boost.org for most recent version including documentation.

//  Original idea Henrik Ravn.

#ifndef CLASS_PROPERTY_HPP
#define CLASS_PROPERTY_HPP

// ************************************************************************** //
// **************               readonly_property              ************** //
// ************************************************************************** //

#define BOOST_FRIENDS_DECLARER0()
#define BOOST_FRIENDS_DECLARER1( friend1 )                            friend class friend1;
#define BOOST_FRIENDS_DECLARER2( friend1, friend2 )                   BOOST_FRIENDS_DECLARER1( friend1 ) friend class friend2;
#define BOOST_FRIENDS_DECLARER3( friend1, friend2, friend3 )          BOOST_FRIENDS_DECLARER2( friend1, friend2 ) friend class friend3;
#define BOOST_FRIENDS_DECLARER4( friend1, friend2, friend3, friend4 ) BOOST_FRIENDS_DECLARER3( friend1, friend2, friend3 ) friend class friend4;

#define BOOST_READONLY_PROPERTY( property_type, friends_num, friends )                                              \
class BOOST_JOIN( readonly_property, __LINE__ )                                                                     \
{                                                                                                                   \
    BOOST_FRIENDS_DECLARER ## friends_num friends                                                                   \
public:                                                                                                             \
    explicit BOOST_JOIN( readonly_property, __LINE__ )( property_type const& init_value  ) : value( init_value ) {} \
                                                                                                                    \
    operator                property_type const &() const       { return value; }                                   \
    property_type const&    get() const                         { return value; }                                   \
private:                                                                                                            \
    property_type           value;                                                                                  \
}

// ************************************************************************** //
// **************              readwrite_property              ************** //
// ************************************************************************** //

#define BOOST_READWRITE_PROPERTY( property_type )                                                                   \
class BOOST_JOIN( readwrite_property, __LINE__ )                                                                    \
{                                                                                                                   \
public:                                                                                                             \
             BOOST_JOIN( readwrite_property, __LINE__ )()        {}                                                 \
    explicit BOOST_JOIN( readwrite_property, __LINE__ )( property_type const& init_value  ) : value( init_value ) {}\
                                                                                                                    \
    operator                property_type const &() const       { return value; }                                   \
    property_type const&    get() const                         { return value; }                                   \
    void                    set( property_type const& v )       { value = v; }                                      \
private:                                                                                                            \
    property_type value;                                                                                            \
}

//  Revision History
//   5 Oct 01  Initial version (Gennadiy Rozental)

#endif // CLASS_PROPERTY_HPP
