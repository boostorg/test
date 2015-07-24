/  (C) Copyright Gennadiy Rozental 2005-2015.
//  Use, modification, and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : implements parser - public interface for CLA parsing and accessing
// ***************************************************************************

#ifndef BOOST_TEST_UTILS_RUNTIME_CLA_PARSER_IPP
#define BOOST_TEST_UTILS_RUNTIME_CLA_PARSER_IPP

// Boost.Runtime.Parameter
#include <boost/test/utils/runtime/config.hpp>
#include <boost/test/utils/runtime/trace.hpp>
#include <boost/test/utils/runtime/argument.hpp>

#include <boost/test/utils/runtime/cla/argv_traverser.hpp>
#include <boost/test/utils/runtime/cla/parameter.hpp>
#include <boost/test/utils/runtime/cla/modifier.hpp>
#include <boost/test/utils/runtime/cla/validation.hpp>
#include <boost/test/utils/runtime/cla/parser.hpp>

// Boost.Test
#include <boost/test/utils/basic_cstring/io.hpp>
#include <boost/test/utils/foreach.hpp>

// Boost
#include <boost/lexical_cast.hpp>

namespace boost {
namespace runtime {
namespace cla {

// ************************************************************************** //
// **************             runtime::cla::parser             ************** //
// ************************************************************************** //

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE
parser::parser( cstring program_name )
{
    assign_op( m_program_name, program_name, 0 );
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE parser::param_iterator
parser::first_param() const
{
    return m_parameters.begin();
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE parser::param_iterator
parser::last_param() const
{
    return m_parameters.end();
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE argument const&
parser::valid_argument( cstring string_id ) const
{
    const_argument_ptr arg = (*this)[string_id];

    if( !arg )
        report_logic_error( format_stream().ref() << "Actual argument for parameter " << string_id << " is not present" )

    return *arg;
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE parser&
parser::operator<<( parameter_ptr new_param )
{
    BOOST_TEST_FOREACH( parameter_ptr, old_param, m_parameters ) {
        if( old_param->conflict_with( *new_param ) )
            report_logic_error( format_stream().ref() << 
                "Definition of parameter "                << new_param->id_2_report() <<
                " conflicts with defintion of parameter " << old_param->id_2_report() )
    }

    m_parameters.push_back( new_param );

    return *this;
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
parser::parse( int& argc, char** argv )
{
    if( m_program_name.empty() ) {
        m_program_name.assign( argv[0] );
        std::string::size_type pos = m_program_name.find_last_of( "/\\" );

        if( pos != static_cast<std::string::size_type>(cstring::npos) )
            m_program_name.erase( 0, pos+1 );
    }

    m_traverser.init( argc, argv );

    BOOST_TEST_IMPL_TRY {
        while( !m_traverser.eoi() ) {
            parameter_ptr found_param;

            BOOST_TEST_FOREACH( parameter_ptr const&, curr_param, m_parameters ) {
                if( curr_param->matching( m_traverser, !found_param ) ) {
                    BOOST_TEST_UTILS_RUNTIME_CLA_VALIDATE_INPUT( !found_param, (m_traverser.rollback(),m_traverser), "Ambiguous input" );

                    found_param = curr_param;
                }

                m_traverser.rollback();
            }

            if( !found_param ) {
                BOOST_TEST_UTILS_RUNTIME_CLA_VALIDATE_INPUT( m_traverser.handle_mismatch(), m_traverser,
                                                             "Unexpected input" );

                continue;
            }

            found_param->produce_argument( m_traverser );

            m_traverser.commit();
        }

        BOOST_TEST_FOREACH( parameter_ptr const&, curr_param, m_parameters ) {
            if( !curr_param->p_optional && !curr_param->actual_argument() ) {
                curr_param->produce_argument( *this );

                BOOST_TEST_UTILS_RUNTIME_PARAM_VALIDATE_LOGIC( curr_param->actual_argument(),
                    "Required argument for parameter " << curr_param->id_2_report()
                        << " is missing" );
            }
        }
    }
    BOOST_TEST_IMPL_CATCH0( bad_lexical_cast ) {
        BOOST_TEST_UTILS_RUNTIME_PARAM_REPORT_LOGIC_ERROR(
            "String to value convertion error during input parsing" );
    };

    m_traverser.remainder( argc, argv );
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE const_argument_ptr
parser::operator[]( cstring string_id ) const
{
    parameter_ptr found_param;

    BOOST_TEST_FOREACH( parameter_ptr const&, curr_param, m_parameters ) {
        if( curr_param->responds_to( string_id ) ) {
            BOOST_TEST_UTILS_RUNTIME_PARAM_VALIDATE_LOGIC( !found_param,
                "Ambiguous parameter string id: " << string_id );

            found_param = curr_param;
        }
    }

    return found_param ? found_param->actual_argument() : argument_ptr();
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE cstring
parser::get( cstring string_id ) const
{
    return get<cstring>( string_id );
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
parser::usage( std::ostream& ostr )
{
    if( m_program_name.empty() )
        assign_op( m_program_name, "<program>", 0 );

    format_stream fs;

    fs << m_program_name;

    BOOST_TEST_FOREACH( parameter_ptr const&, curr_param, m_parameters ) {
        fs << ' ';

        if( curr_param->p_optional )
            fs << '[';

        curr_param->usage_info( fs );

        if( curr_param->p_optional )
            fs << ']';

        if( curr_param->p_multiplicable ) {
            fs << " ... ";

            if( curr_param->p_optional )
                fs << '[';

            curr_param->usage_info( fs );

            if( curr_param->p_optional )
                fs << ']';
        }
    }

    ostr << "Usage:\n" << fs.str() << std::endl;
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
parser::help( std::ostream& ostr )
{
    usage( ostr );

    bool need_where = true;

    BOOST_TEST_FOREACH( parameter_ptr const&, curr_param, m_parameters ) {
        if( curr_param->p_description->empty() )
            continue;

        if( need_where ) {
            ostr << "where:\n";
            need_where = false;
        }

        ostr << curr_param->id_2_report() << " - " << curr_param->p_description << std::endl;
    }
}

//____________________________________________________________________________//

BOOST_TEST_UTILS_RUNTIME_PARAM_INLINE void
parser::reset()
{
    BOOST_TEST_FOREACH( parameter_ptr const&, param, m_parameters )
        param->reset();
}

//____________________________________________________________________________//

} // namespace cla
} // namespace runtime
} // namespace boost

#endif // BOOST_TEST_UTILS_RUNTIME_CLA_PARSER_IPP
