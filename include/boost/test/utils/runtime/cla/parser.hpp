//  (C) Copyright Gennadiy Rozental 2005-2015.
//  Use, modification, and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : defines parser - public interface for CLA parsing and accessing
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_CLA_PARSER_HPP
#define BOOST_TEST_UTILS_RUNTIME_CLA_PARSER_HPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>
#include <boost/test/utils/runtime/argument.hpp>
#include <boost/test/utils/runtime/modifier.hpp>
#include <boost/test/utils/runtime/parameter.hpp>

#include <boost/test/utils/runtime/cla/argv_traverser.hpp>

// Boost.Test
#include <boost/test/utils/foreach.hpp>
#include <boost/test/detail/throw_exception.hpp>

#include <iostream>

namespace boost {
namespace runtime {
namespace cla {

// ************************************************************************** //
// **************         runtime::cla::parameter_trie         ************** //
// ************************************************************************** //

namespace rt_cla_detail {

struct parameter_trie;
typedef shared_ptr<parameter_trie> parameter_trie_ptr;
typedef std::map<char,parameter_trie_ptr> trie_per_char;

struct parameter_trie {
    parameter_trie() : m_has_final_candidate( false ) {}

    /// If subtrie corresponding to the char c exists returns it otherwise creates new
    parameter_trie_ptr  make_subtrie( char c )
    {
        trie_per_char::const_iterator it = m_subtrie.find( c );

        if( it == m_subtrie.end() )
            it = m_subtrie.insert( std::make_pair( c, parameter_trie_ptr( new parameter_trie ) ) ).first;

        return it->second;
    }

    /// Creates series of sub-tries per characters in a string
    parameter_trie_ptr  make_subtrie( cstring s )
    {
        parameter_trie_ptr res;

        BOOST_TEST_FOREACH( char, c, s )
            res = (res ? res->make_subtrie( c ) : make_subtrie( c ));

        return res;
    }

    /// Registers candidate parameter for this subtrie. If final, it needs to be unique
    void                add_param_candidate( parameter_cla_id const& param_id, bool final )
    {
        if( m_has_final_candidate || final && !m_candidates.empty() ) {
            BOOST_TEST_IMPL_THROW( conflicting_param() << "Parameter " << param_id.m_full_name << " conflicts with the "
                                                       << "parameter " << m_candidates.back().get().m_full_name );
        }

        m_has_final_candidate = final;
        m_candidates.push_back( param_id );
    }

    /// Gets subtrie for specified char if present or nullptr otherwise
    parameter_trie_ptr  get_subtrie( char c ) const
    {
        trie_per_char::const_iterator it = m_subtrie.find( c );

        return it != m_subtrie.end() ? it->second : parameter_trie_ptr();
    }

    typedef std::vector<std::reference_wrapper<parameter_cla_id const> > param_id_list;

    // Data members
    trie_per_char       m_subtrie;
    param_id_list       m_candidates;
    bool                m_has_final_candidate;
};

} // namespace rt_cla_detail

// ************************************************************************** //
// **************             runtime::cla::parser             ************** //
// ************************************************************************** //

class parser {
public:
    /// Initializes a parser and builds internal trie representation used for
    /// parsing based on the supplied parameters
    template<typename Modifiers=nfp::no_params_type>
    parser( parameters_store const& parameters, Modifiers const& m = nfp::no_params)
    {
        build_trie( parameters );
    }

    // input processing method
    int
    parse( int argc, char** argv, runtime::arguments_store& res )
    {
        // 10. Set up the traverser
        argv_traverser tr( argc, (char const**)argv );

        // 20. Loop till we reach end of input
        while( !tr.eoi() ) {
            cstring prefix;
            cstring name;
            cstring value_separator;

            // Perform format validations and split the argument into prefix, name and separator
            validate_token_format( tr.current_token(), prefix, name, value_separator );

            // Locate trie corresponding to found prefix and skip it in the input
            trie_ptr curr_trie = m_param_trie[prefix];

            if( !curr_trie ) {
                BOOST_TEST_IMPL_THROW( format_error() << "Unrecognized parameter prefix in the argument " 
                                                      << tr.current_token() );
            }
            tr.skip( prefix.size() );

            // Locate parameter based on name and skip it in the input
            parameter_cla_id const& found_id    = locate_parameter( curr_trie, name, tr.current_token() );
            basic_param const&      found_param = found_id.m_owner;
            tr.skip( name.size() );

            // Validate and skip value separator in the input
            if( value_separator.is_empty() && found_param.p_optional_value ) {
                // !!!!
                continue;
            }
            else if( found_id.m_value_separator != value_separator ) {
                BOOST_TEST_IMPL_THROW( format_error() << "Invalid separator for the parameter " << found_param.p_name 
                                                      << " in the argument " << tr.current_token() );
            }
            tr.skip( value_separator.size() );

            // Obtain the parameter value
            cstring value = tr.get_token();

            if( value.is_empty() )
                BOOST_TEST_IMPL_THROW( format_error() << "Missing an argument value for the parameter " << found_param.p_name 
                                                      << " in the argument " << tr.current_token() );

            if( res.has( found_param.p_name.get() ) )
                BOOST_TEST_IMPL_THROW( duplicate_arg() << "Duplicate argument value for the parameter "  << found_param.p_name 
                                                       << " in the argument " << tr.current_token() );

            found_param.produce_argument( value, res );
        }

        return tr.remainder();
    }

    // help/usage
    void        usage( std::ostream& ostr );

private:
    typedef rt_cla_detail::parameter_trie_ptr trie_ptr;
    typedef std::map<cstring,trie_ptr> str_to_trie;

    void
    build_trie( parameters_store const& parameters )
    {
        // 10. Iterate over all parameters
        BOOST_TEST_FOREACH( parameters_store::storage_type::value_type const&, v, parameters.all() ) {
            basic_param_ptr param = v.second;

            // 20. Register all parameter's ids in trie.
            BOOST_TEST_FOREACH( parameter_cla_id const&, id, param->cla_ids() ) {
                // 30. This is the trie corresponding to the prefix.
                trie_ptr next_trie = m_param_trie[id.m_prefix];
                if( !next_trie )
                    next_trie = m_param_trie[id.m_prefix] = trie_ptr( new rt_cla_detail::parameter_trie );

                // 40. Build the trie, by following parameter id's full name
                //     and register this parameter as candidate on each level
                for( size_t index = 0; index < id.m_full_name.size(); ++index ) {
                    next_trie = next_trie->make_subtrie( id.m_full_name[index] );

                    next_trie->add_param_candidate( id, index == (id.m_full_name.size() - 1) );
                }
            }
        }
    }

    void
    validate_token_format( cstring token, cstring& prefix, cstring& name, cstring& separator )
    {
        auto it = token.begin();
        while( it != token.end() && parameter_cla_id::valid_prefix_char( *it ) )
            ++it;

        prefix.assign( token.begin(), it );

        while( it != token.end() && parameter_cla_id::valid_name_char( *it ) )
            ++it;

        name.assign( prefix.end(), it );

        if( name.empty() )
            BOOST_TEST_IMPL_THROW( format_error() << "Invalid format for an actual argument " << token );

        while( it != token.end() && parameter_cla_id::valid_separator_char(*it) )
            ++it;

        separator.assign( name.end(), it );         
    }

    parameter_cla_id const&
    locate_parameter( trie_ptr curr_trie, cstring name, cstring token )
    {
        BOOST_TEST_FOREACH( char, c, name ) {
            curr_trie = curr_trie->get_subtrie( c );
            if( !curr_trie )
                break;
        }

        if( !curr_trie ) {
            BOOST_TEST_IMPL_THROW( unrecognized_param() << "An unrecognized parameter in the argument " << token );
        }

        if( curr_trie->m_candidates.size() > 1 )            
            BOOST_TEST_IMPL_THROW( ambiguous_param() << "An ambiguous parameter name in the argument " << token );

        return curr_trie->m_candidates.back().get();
    }

    // Data members
    std::string m_program_name;
    str_to_trie m_param_trie;
};

} // namespace cla
} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_CLA_PARSER_HPP
