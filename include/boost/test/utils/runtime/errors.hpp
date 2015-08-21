//  (C) Copyright Gennadiy Rozental 2005-2015.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines runtime parameters setup error
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_INIT_ERROR_HPP
#define BOOST_TEST_UTILS_RUNTIME_INIT_ERROR_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/fwd.hpp>

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>

// STL
#include <exception>

#include <boost/test/detail/suppress_warnings.hpp>

namespace boost {
namespace runtime {

// ************************************************************************** //
// **************             runtime::param_error             ************** //
// ************************************************************************** //

class param_error : public std::exception {
public:
    virtual const char * what() const BOOST_NOEXCEPT_OR_NOTHROW
    {
        return msg.c_str();
    }

    std::string msg;
};

//____________________________________________________________________________//

class init_error : public param_error {};
class input_error : public param_error {};

//____________________________________________________________________________//

template<typename Derived, typename Base>
class specific_param_error : public Base {
public:
};

//____________________________________________________________________________//

template<typename Derived, typename Base>
inline Derived
operator<<(specific_param_error<Derived, Base>&& ex, char const* val)
{
    ex.msg.append( val );

    return reinterpret_cast<Derived&&>(ex);
}

//____________________________________________________________________________//

template<typename Derived, typename Base, typename T>
inline Derived
operator<<(specific_param_error<Derived, Base>&& ex, T const& val)
{
    ex.msg.append( lexical_cast<std::string>( val ) );

    return reinterpret_cast<Derived&&>(ex);
}

//____________________________________________________________________________//

// ************************************************************************** //
// **************           specific exception types           ************** //
// ************************************************************************** //

class invalid_cla_id : public specific_param_error<invalid_cla_id, init_error> {};
class duplicate_param : public specific_param_error<duplicate_param, init_error> {};
class conflicting_param : public specific_param_error<conflicting_param, init_error> {};
class unknown_param : public specific_param_error<unknown_param, init_error> {};
class missing_argument : public specific_param_error<missing_argument, init_error> {};
class arg_type_mismatch : public specific_param_error<arg_type_mismatch, init_error> {};
class invalid_param_spec : public specific_param_error<invalid_param_spec, init_error> {};

class format_error : public specific_param_error<format_error, input_error> {};
class ambiguous_param : public specific_param_error<ambiguous_param, input_error> {};
class duplicate_arg : public specific_param_error<duplicate_arg, input_error> {};
class missing_arg : public specific_param_error<missing_arg, input_error> {};

class unrecognized_param : public specific_param_error<unrecognized_param, input_error> {
public:
    explicit unrecognized_param( std::vector<cstring>&& type_candidates ) : m_typo_candidates( std::move( type_candidates ) ) {}

    std::vector<cstring> m_typo_candidates;
};

} // namespace runtime
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_UTILS_RUNTIME_INIT_ERROR_HPP
