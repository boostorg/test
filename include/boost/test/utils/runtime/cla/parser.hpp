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

#include <boost/test/utils/runtime/cla/fwd.hpp>
#include <boost/test/utils/runtime/cla/modifier.hpp>
#include <boost/test/utils/runtime/cla/argv_traverser.hpp>
#include <boost/test/utils/runtime/cla/parameter.hpp>

// Boost.Test
#include <boost/test/utils/foreach.hpp>
#include <boost/test/detail/throw_exception.hpp>

namespace boost {
namespace runtime {
namespace cla {

// ************************************************************************** //
// **************         runtime::cla::parameter_trie         ************** //
// ************************************************************************** //

namespace rt_cla_detail {

class parameter_trie;
typedef shared_ptr<parameter_trie> parameter_trie_ptr;
typedef std::map<char,parameter_trie_ptr> trie_per_char;

class parameter_trie {
public:
    // If subtrie corresponding to the char c exists returns it otherwise creates new 
    parameter_trie_ptr  make_subtrie( char c )
    {
        trie_per_char::const_iterator it = m_subtrie.find( c );

        if( it == m_subtrie.end() )
            it = m_subtrie.insert( std::make_pair( c, parameter_trie_ptr( new parameter_trie ) ) ).first;

        return it->second;
    }

    parameter_trie_ptr  make_subtrie( cstring s )
    {
        parameter_trie_ptr res;

        BOOST_TEST_FOREACH( char, c, s )
            res = (res ? res->make_subtrie( c ) : make_subtrie( c ));

        return res;
    }

    void                add_param_candidate( basic_param_ptr param, bool final )
    {
        if( final ) {
            if( !m_candidates.empty() )
                BOOST_TEST_IMPL_THROW( ambigues_param_definition( param,  m_candidates.front() ) );
            m_final_candidate = param;
        }
        else {
            if( m_final_candidate )
                BOOST_TEST_IMPL_THROW( ambigues_param_definition( param,  m_final_candidate ) );
        }

        m_candidates.push_back( param );
    }

private:
    // Data members
    trie_per_char       m_subtrie;
    param_list          m_candidates;
    basic_param_ptr     m_final_candidate;
};

} // namespace rt_cla_detail

// ************************************************************************** //
// **************             runtime::cla::parser             ************** //
// ************************************************************************** //

class parser {
public:
    // Constructor
    template<typename Params=nfp::no_params_type>
    explicit    parser( Params const& params = nfp::no_params )
    {
    }

    // parameter list construction interface
    void        add( basic_param const& in )
    {
        // 10. Clone parameter for storing in persistent location.
        basic_param_ptr p = in.clone();
        m_parameters[p->p_name] = p;

        // 20. Register all parameter's ids in trie.
        BOOST_TEST_FOREACH( parameter_id const&, id, p->ids() ) {
            // 30. This is going to be cursor pointing to tail trie. Process prefix chars first.
            trie_ptr next_trie = m_param_trie->make_subtrie( id.m_prefix );

            // 40. Process name chars.
            for( size_t index = 0; index < id.m_full_name.size(); ++index ) {
                next_trie = next_trie->make_subtrie( id.m_full_name[index] );

                next_trie->add_param_candidate( p, index == (id.m_full_name.size() - 1) );
            }
        }
    }

    // input processing method
    int         parse( int argc, char** argv, runtime::argument_store& res )
    {
        argv_traverser tr( argc, argv );
    }

    // help/usage
    void        usage( std::ostream& ostr );

private:
    typedef rt_cla_detail::parameter_trie_ptr trie_ptr;

    // Data members
    std::string m_program_name;
    param_store m_parameters;
    trie_ptr    m_param_trie;
};

} // namespace cla
} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_CLA_PARSER_HPP
