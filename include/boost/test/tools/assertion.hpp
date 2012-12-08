//  (C) Copyright Gennadiy Rozental 2011-2012.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at 
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision: 74663 $
//
//  Description : defines framework for automated assertion construction
// ***************************************************************************

#ifndef BOOST_TEST_TOOLS_ASSERTION_HPP_100911GER
#define BOOST_TEST_TOOLS_ASSERTION_HPP_100911GER

// Boost.Test
#include <boost/test/utils/is_forward_iterable.hpp>

// Boost
#include <boost/mpl/assert.hpp>
#include <boost/utility/declval.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/remove_reference.hpp>

// STL
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
#include <utility>
#endif

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace test_tools {
namespace assertion {

// ************************************************************************** //
// **************             assertion::operators             ************** //
// ************************************************************************** //
// precedence 4: ->*, .*
// precedence 5: *, /, %
// precedence 6: +, -
// precedence 7: << , >>
// precedence 8: <, <=, > and >=
// precedence 9: == and !=
// precedence 10: bitwise AND
// precedence 11: bitwise XOR
// precedence 12: bitwise OR
// precedence 13: logical AND
//  disabled
// precedence 14: logical OR
//  disabled
// precedence 15: ternary conditional
//  disabled
// precedence 16: = and OP= operators
// precedence 17: throw operator
//  not supported
// precedence 18: comma
//  not supported

namespace op {

#define BOOST_TEST_FOR_EACH_COMP_OP(action) \
    action( < , LT, >= )                    \
    action( <=, LE, >  )                    \
    action( > , GT, <= )                    \
    action( >=, GE, <  )                    \
    action( ==, EQ, != )                    \
    action( !=, NE, == )                    \
/**/

//____________________________________________________________________________//

#ifndef BOOST_NO_CXX11_DECLTYPE

#define BOOST_TEST_FOR_EACH_CONST_OP(action)\
    action(->*, MEMP, ->* )                 \
                                            \
    action( * , MUL, * )                    \
    action( / , DIV, / )                    \
    action( % , MOD, % )                    \
                                            \
    action( + , ADD, + )                    \
    action( - , SUB, - )                    \
                                            \
    action( <<, LSH, << )                   \
    action( >>, RSH, >> )                   \
                                            \
    BOOST_TEST_FOR_EACH_COMP_OP(action)     \
                                            \
    action( & , BAND, & )                   \
    action( ^ , XOR, ^ )                    \
    action( | , BOR, | )                    \
/**/

#else

#define BOOST_TEST_FOR_EACH_CONST_OP(action)\
    BOOST_TEST_FOR_EACH_COMP_OP(action)     \
/**/

#endif

//____________________________________________________________________________//

#define BOOST_TEST_FOR_EACH_MUT_OP(action)  \
    action( = , SET , =  )                  \
    action( +=, IADD, += )                  \
    action( -=, ISUB, -= )                  \
    action( *=, IMUL, *= )                  \
    action( /=, IDIV, /= )                  \
    action( %=, IMOD, %= )                  \
    action(<<=, ILSH, <<=)                  \
    action(>>=, IRSH, >>=)                  \
    action( &=, IAND, &= )                  \
    action( ^=, IXOR, ^= )                  \
    action( |=, IOR , |= )                  \
/**/

//____________________________________________________________________________//

#ifndef BOOST_NO_CXX11_DECLTYPE
#   define DEDUCE_RESULT_TYPE( oper )                                   \
    decltype(boost::declval<Lhs>() oper boost::declval<Rhs>() ) optype; \
    typedef typename boost::remove_reference<optype>::type              \
/**/
#else
#   define DEDUCE_RESULT_TYPE( oper ) bool
#endif

#define DEFINE_CONST_OPER( oper, name, rev )        \
template<typename Lhs, typename Rhs,                \
         typename Enabler=void>                     \
struct name {                                       \
    typedef DEDUCE_RESULT_TYPE( oper ) result_type; \
                                                    \
    static result_type                              \
    eval( Lhs const& lhs, Rhs const& rhs )          \
    {                                               \
        return lhs oper rhs;                        \
    }                                               \
                                                    \
    template<typename PrevExprType>                 \
    static void                                     \
    report( std::ostream&       ostr,               \
            PrevExprType const& lhs,                \
            Rhs const&          rhs)                \
    {                                               \
        lhs.report( ostr );                         \
        ostr << revert() << rhs;                    \
    }                                               \
                                                    \
    static char const* revert()                     \
    { return " " #rev " "; }                        \
};                                                  \
/**/

BOOST_TEST_FOR_EACH_CONST_OP( DEFINE_CONST_OPER )

#undef DEDUCE_RESULT_TYPE
#undef DEFINE_CONST_OPER

//____________________________________________________________________________//

template <typename OP, typename Lhs, typename Rhs>
inline predicate_result
compare_collections( Lhs const& lhs, Rhs const& rhs )
{
    predicate_result pr( true );

    if( lhs.size() != rhs.size() ) {
        pr = false;
        pr.message() << "Collections size mismatch: " << lhs.size() << " != " << rhs.size();
        return pr;
    }
        
    typename Lhs::const_iterator left  = lhs.begin();
    typename Rhs::const_iterator right = rhs.begin();
    std::size_t                  pos   = 0;

    for( ; pos < lhs.size(); ++left, ++right, ++pos ) {
        if( OP::eval( *left, *right ) )
            continue;

        pr = false;
        pr.message() << "\nMismatch in a position " << pos << ": "  << *left << OP::revert() << *right;
    }

    return pr;
}

//____________________________________________________________________________//

#define DEFINE_COLLECTION_COMPARISON( oper, name, _ )       \
template<typename Lhs,typename Rhs>                         \
struct name<Lhs,Rhs,typename boost::enable_if_c<            \
unit_test::is_forward_iterable<Lhs>::value &&               \
unit_test::is_forward_iterable<Rhs>::value>::type> {        \
public:                                                     \
    typedef predicate_result result_type;                   \
                                                            \
    static predicate_result                                 \
    eval( Lhs const& lhs, Rhs const& rhs)                   \
    {                                                       \
        typedef name<typename boost::remove_reference<Lhs>  \
                        ::type::value_type,                 \
                     typename boost::remove_reference<Rhs>  \
                        ::type::value_type> elem_comp_op;   \
        return compare_collections<elem_comp_op>(lhs, rhs); \
    }                                                       \
                                                            \
    template<typename PrevExprType>                         \
    static void                                             \
    report( std::ostream&,                                  \
            PrevExprType const&,                            \
            Rhs const& ) {}                                 \
};                                                          \
/**/

//____________________________________________________________________________//

BOOST_TEST_FOR_EACH_COMP_OP( DEFINE_COLLECTION_COMPARISON )

} // namespace op

// ************************************************************************** //
// **************             assertion::expression            ************** //
// ************************************************************************** //

#ifdef BOOST_NO_CXX11_AUTO_DECLARATIONS
class expression {
public:
    // expression interface
    virtual predicate_result    evaluate() const = 0;
};
#endif

// ************************************************************************** //
// **************          assertion::expression_base          ************** //
// ************************************************************************** //
// Defines expression operators

template<typename Lhs, typename Rhs, typename OP> class binary_expr;

template<typename ExprType,typename ValType>
class expression_base 
#ifdef BOOST_NO_CXX11_AUTO_DECLARATIONS
: public expression
#endif
{
public:

#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES

#define ADD_OP_SUPPORT( oper, name, _ )                         \
    template<typename T>                                        \
    binary_expr<ExprType,T,op::name<ValType,T> >                \
    operator oper( T&& rhs ) const                              \
    {                                                           \
        return binary_expr<ExprType,T,op::name<ValType,T> >(    \
            *static_cast<ExprType const*>(this),                \
            std::forward<T>(rhs) );                             \
    }                                                           \
/**/
#else

#define ADD_OP_SUPPORT( oper, name, _ )                         \
    template<typename T>                                        \
    binary_expr<ExprType,T,op::name<ValType,T> >                \
    operator oper( T const& rhs ) const                         \
    {                                                           \
        return binary_expr<ExprType,T,op::name<ValType,T> >(    \
            *static_cast<ExprType const*>(this),                \
            rhs );                                              \
    }                                                           \
/**/
#endif

    BOOST_TEST_FOR_EACH_CONST_OP( ADD_OP_SUPPORT )
    #undef ADD_OP_SUPPORT

    // Disabled operators
    template<typename T>
    ExprType&
    operator ||( T const& )
    {
        BOOST_MPL_ASSERT_MSG(false, CANT_USE_LOGICAL_OPERATOR_OR_WITHIN_THIS_TESTING_TOOL, () );

        return *static_cast<ExprType*>(this);
    }

    template<typename T>
    ExprType&
    operator &&( T const& )
    {
        BOOST_MPL_ASSERT_MSG(false, CANT_USE_LOGICAL_OPERATOR_AND_WITHIN_THIS_TESTING_TOOL, () );

        return *static_cast<ExprType*>(this);
    }

    operator bool()
    {
        BOOST_MPL_ASSERT_MSG(false, CANT_USE_TERNARY_OPERATOR_WITHIN_THIS_TESTING_TOOL, () );

        return false;
    }
};

// ************************************************************************** //
// **************            assertion::value_expr             ************** //
// ************************************************************************** //
// simple value expression

template<typename T>
class value_expr : public expression_base<value_expr<T>,T> {
public:
    // Public types
    typedef T                   result_type;

    // Constructor
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    explicit                    value_expr( T&& val )
    : m_value( std::forward<T>(val) )
#else
    explicit                    value_expr( T const& val )
    : m_value( val )
#endif
    {}

    // Specific expresson interface
    T const&                    value() const
    {
        return m_value;
    }
    void                        report( std::ostream& ostr ) const
    {
        ostr << m_value;
    }

    // Mutating operators
#define ADD_OP_SUPPORT( OPER, ID, _ )   \
    template<typename U>                \
    value_expr<T>&                      \
    operator OPER( U const& rhs )       \
    {                                   \
        m_value OPER rhs;               \
                                        \
        return *this;                   \
    }                                   \
/**/

    BOOST_TEST_FOR_EACH_MUT_OP( ADD_OP_SUPPORT )
#undef ADD_OP_SUPPORT

    // expression interface
#ifdef BOOST_NO_CXX11_AUTO_DECLARATIONS
    virtual predicate_result    evaluate() const
#else
    predicate_result            evaluate() const
#endif
    {
        predicate_result res( value() );
        if( !res )
            format_message( res.message(), value() );

        return res;
    }

private:
    template<typename U>
    static void format_message( wrap_stringstream& ostr, U const& v )    { ostr << "(bool)" << v << " is false"; }
    static void format_message( wrap_stringstream& , bool )        {}

    // Data members
    T                           m_value;
};

// ************************************************************************** //
// **************            assertion::binary_expr            ************** //
// ************************************************************************** //
// binary expression

template<typename LExpr, typename Rhs, typename OP>
class binary_expr : public expression_base<binary_expr<LExpr,Rhs,OP>,typename OP::result_type>
{
public:
    // Public types
    typedef typename OP::result_type result_type;

    // Constructor
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    binary_expr( LExpr const& lhs, Rhs&& rhs )
    : m_lhs( lhs )
    , m_rhs( std::forward<Rhs>(rhs) )
    {}
#else
    binary_expr( LExpr const& lhs, Rhs const& rhs )
    : m_lhs( lhs )
    , m_rhs( rhs )
    {}
#endif

    // Specific expression interface
    result_type                 value() const
    {
        return OP::eval( m_lhs.value(), m_rhs );
    }
    void                        report( std::ostream& ostr ) const
    {
        return OP::report( ostr, m_lhs, m_rhs );
    }

#ifdef BOOST_NO_CXX11_AUTO_DECLARATIONS
    virtual predicate_result    evaluate() const
#else
    predicate_result            evaluate() const
#endif
    {
        predicate_result res( value() );
        if( !res )
            report( res.message().stream() );

        return res;
    }

private:
    // Data members
    LExpr                       m_lhs;
    Rhs                         m_rhs;
};

// ************************************************************************** //
// **************               assertion::seed                ************** //
// ************************************************************************** //
// seed added ot the input expression to form an assertion expression

class seed {
public:
    // ->* is highest precedence left to right operator
    template<typename T>
    value_expr<T>
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
    operator->*( T&& v ) const
    {
        return value_expr<T>( std::forward<T>( v ) );
    }
#else
    operator->*( T const& v )  const
    {
        return value_expr<T>( v );
    }
#endif

};

#undef BOOST_TEST_FOR_EACH_CONST_OP

} // namespace assertion
} // namespace test_tools
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_TOOLS_ASSERTION_HPP_100911GER

