//  (C) Copyright Gennadiy Rozental 2005-2010.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//  File        : $RCSfile$
//
//  Version     : $Revision$
//
//  Description : implements framework API - main driver for the test
// ***************************************************************************

#ifndef BOOST_TEST_FRAMEWORK_IPP_021005GER
#define BOOST_TEST_FRAMEWORK_IPP_021005GER

// Boost.Test
#include <boost/test/framework.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/test/debug.hpp>
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_monitor.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/progress_monitor.hpp>
#include <boost/test/results_reporter.hpp>
#include <boost/test/test_tools.hpp>

#include <boost/test/tree/observer.hpp>
#include <boost/test/tree/test_unit.hpp>
#include <boost/test/tree/visitor.hpp>
#include <boost/test/tree/traverse.hpp>
#include <boost/test/tree/test_case_counter.hpp>

#if BOOST_TEST_SUPPORT_TOKEN_ITERATOR
#include <boost/test/utils/iterator/token_iterator.hpp>
#endif

#include <boost/test/detail/unit_test_parameters.hpp>
#include <boost/test/detail/global_typedef.hpp>

#include <boost/test/utils/foreach.hpp>

// Boost
#include <boost/timer.hpp>
#include <boost/bind.hpp>

// STL
#include <map>
#include <set>
#include <cstdlib>
#include <ctime>

#ifdef BOOST_NO_STDC_NAMESPACE
namespace std { using ::time; using ::srand; }
#endif

#include <boost/test/detail/suppress_warnings.hpp>

//____________________________________________________________________________//

namespace boost {
namespace unit_test {

// ************************************************************************** //
// **************            test_init call wrapper            ************** //
// ************************************************************************** //

namespace ut_detail {

struct test_init_invoker {
    explicit    test_init_invoker( init_unit_test_func init_func )  : m_init_func( init_func ) {}
    int         operator()()
    {
#ifdef BOOST_TEST_ALTERNATIVE_INIT_API
        if( !(*m_init_func)() )
            throw std::runtime_error( "test module initialization failed" );
#else
        test_suite*  manual_test_units = (*m_init_func)( framework::master_test_suite().argc, framework::master_test_suite().argv );

        if( manual_test_units )
            framework::master_test_suite().add( manual_test_units );
#endif
        return 0;
    }

    // Data members
    init_unit_test_func m_init_func;
};

// ************************************************************************** //
// **************                  name_filter                 ************** //
// ************************************************************************** //

typedef std::list<std::pair<test_unit_id,bool> > tu_enable_list;

class name_filter : public test_tree_visitor {
    struct component {
        component( const_string name ) // has to be implicit
        {
            if( name == "*" )
                m_kind  = SFK_ALL;
            else if( first_char( name ) == '*' && last_char( name ) == '*' ) {
                m_kind  = SFK_SUBSTR;
                m_name  = name.substr( 1, name.size()-1 );
            }
            else if( first_char( name ) == '*' ) {
                m_kind  = SFK_TRAILING;
                m_name  = name.substr( 1 );
            }
            else if( last_char( name ) == '*' ) {
                m_kind  = SFK_LEADING;
                m_name  = name.substr( 0, name.size()-1 );
            }
            else {
                m_kind  = SFK_MATCH;
                m_name  = name;
            }
        };

        bool            pass( test_unit const& tu ) const
        {
            const_string name( tu.p_name );
    
            switch( m_kind ) {
            default:
            case SFK_ALL:
                return true;
            case SFK_LEADING:
                return name.substr( 0, m_name.size() ) == m_name;
            case SFK_TRAILING:
                return name.size() >= m_name.size() && name.substr( name.size() - m_name.size() ) == m_name;
            case SFK_SUBSTR:
                return name.find( m_name ) != const_string::npos;
            case SFK_MATCH:
                return m_name == tu.p_name.get();
            }
        }
        enum kind { SFK_ALL, SFK_LEADING, SFK_TRAILING, SFK_SUBSTR, SFK_MATCH };

        kind            m_kind;
        const_string    m_name;
    };

public:
    // Constructor
    name_filter( tu_enable_list& tu_to_enable, const_string tc_to_run ) : m_tu_to_enable( tu_to_enable ), m_depth( 0 )
    {
#ifdef BOOST_TEST_SUPPORT_TOKEN_ITERATOR
        string_token_iterator tit( tc_to_run, (dropped_delimeters = "/", kept_delimeters = dt_none) );

        while( tit != string_token_iterator() ) {
            m_components.push_back( std::vector<component>( string_token_iterator( *tit, (dropped_delimeters = ",", kept_delimeters = dt_none)  ), 
                                                            string_token_iterator() ) );

            ++tit;           
        }
#endif
    }

private:
    bool            filter_unit( test_unit const& tu )
    {
        // skip master test suite
        if( m_depth == 0 )
            return true;

        // corresponding name filters are at level m_depth-1
        std::vector<component> const& filters = m_components[m_depth-1];

        // look for match
        return std::find_if( filters.begin(), filters.end(), bind( &component::pass, _1, boost::ref(tu) ) ) != filters.end();
    }

    // test_tree_visitor interface
    virtual void    visit( test_case const& tc )
    {
        if( filter_unit( tc ) )
            m_tu_to_enable.push_back( std::make_pair( tc.p_id, false ) ); // found a test case; add it to enable list without children
    }
    virtual bool    test_suite_start( test_suite const& ts )
    {
        if( filter_unit( ts ) ) {
            if( m_depth < m_components.size() ) {
                ++m_depth;
                return true;
            }

            m_tu_to_enable.push_back( std::make_pair( ts.p_id, true ) ); // found a test suite; add it to enable list with children and stop recursion
        }

        return false;
    }

    // Data members
    typedef std::vector<std::vector<component> > components_per_level;

    components_per_level    m_components;
    tu_enable_list&         m_tu_to_enable;
    unsigned                m_depth;
};

// ************************************************************************** //
// **************                 label_filter                 ************** //
// ************************************************************************** //

class label_filter : public test_tree_visitor {
public:
    label_filter( tu_enable_list& tu_to_enable, const_string label )
    : m_tu_to_enable( tu_to_enable )
    , m_label( label )
    {}

private:
    // test_tree_visitor interface
    virtual bool    visit( test_unit const& tu )
    {
        if( tu.has_label( m_label ) ) {
            // found a test unit; add it to list of tu to enable with children and stop recursion in case of suites
            m_tu_to_enable.push_back( std::make_pair( tu.p_id, tu.p_type == tut_suite ) );
            return false;
        }

        return true;
    }

    // Data members
    tu_enable_list& m_tu_to_enable;
    const_string    m_label;
};

// ************************************************************************** //
// **************                 change_status                ************** //
// ************************************************************************** //

class change_status : public test_tree_visitor {
public:
    explicit        change_status( bool enable_or_disable ) : m_new_status( enable_or_disable ) {}

private:
    // test_tree_visitor interface
    virtual bool    visit( test_unit const& tu ) { tu.p_enabled.value = m_new_status; return true; }

    // Data members
    bool            m_new_status;
};

} // namespace ut_detail

// ************************************************************************** //
// **************                   framework                  ************** //
// ************************************************************************** //

class framework_impl : public test_tree_visitor {
public:
    framework_impl()
    : m_master_test_suite( 0 )
    , m_curr_test_case( INV_TEST_UNIT_ID )
    , m_next_test_case_id( MIN_TEST_CASE_ID )
    , m_next_test_suite_id( MIN_TEST_SUITE_ID )
    , m_is_initialized( false )
    , m_test_in_progress( false )
    , m_context_idx( 0 )
    {}

    ~framework_impl() { clear(); }

    void            clear()
    {
        while( !m_test_units.empty() ) {
            test_unit_store::value_type const& tu     = *m_test_units.begin();
            test_unit const*                   tu_ptr = tu.second;

            // the delete will erase this element from map
            if( ut_detail::test_id_2_unit_type( tu.second->p_id ) == tut_suite )
                delete static_cast<test_suite const*>(tu_ptr);
            else
                delete static_cast<test_case const*>(tu_ptr);
        }
    }
                                    
    void            set_tu_id( test_unit& tu, test_unit_id id ) { tu.p_id.value = id; }

    // test_tree_visitor interface implementation
    void            visit( test_case const& tc )
    {
        if( !tc.check_dependencies() ) {
            BOOST_TEST_FOREACH( test_observer*, to, m_observers )
                to->test_unit_skipped( tc );

            return;
        }

        // setup contexts
        m_context_idx = 0;

        // notify all observers
        BOOST_TEST_FOREACH( test_observer*, to, m_observers )
            to->test_unit_start( tc );

        // execute the test case body
        boost::timer tc_timer;
        test_unit_id bkup = m_curr_test_case;
        m_curr_test_case = tc.p_id;
        unit_test_monitor_t::error_level run_result = unit_test_monitor.execute_and_translate( tc );

        unsigned long elapsed = static_cast<unsigned long>( tc_timer.elapsed() * 1e6 );

        // notify all observers about abortion
        if( unit_test_monitor.is_critical_error( run_result ) ) {
            BOOST_TEST_FOREACH( test_observer*, to, m_observers )
                to->test_aborted();
        }

        // notify all observers about completion
        BOOST_TEST_REVERSE_FOREACH( test_observer*, to, m_observers )
            to->test_unit_finish( tc, elapsed );

        // cleanup leftover context
        m_context.clear();

        // restore state and abort if necessary
        m_curr_test_case = bkup;

        if( unit_test_monitor.is_critical_error( run_result ) )
            throw framework::test_being_aborted();
    }

    bool            test_suite_start( test_suite const& ts )
    {
        if( !ts.check_dependencies() ) {
            BOOST_TEST_FOREACH( test_observer*, to, m_observers )
                to->test_unit_skipped( ts );

            return false;
        }

        BOOST_TEST_FOREACH( test_observer*, to, m_observers )
            to->test_unit_start( ts );

        return true;
    }

    void            test_suite_finish( test_suite const& ts )
    {
        BOOST_TEST_REVERSE_FOREACH( test_observer*, to, m_observers )
            to->test_unit_finish( ts, 0 );
    }

    //////////////////////////////////////////////////////////////////
    struct priority_order {
        bool operator()( test_observer* lhs, test_observer* rhs ) const
        {
            return (lhs->priority() < rhs->priority()) || ((lhs->priority() == rhs->priority()) && (lhs < rhs));
        }
    };

    typedef std::map<test_unit_id,test_unit*>       test_unit_store;
    typedef std::set<test_observer*,priority_order> observer_store;
    struct context_frame {
        context_frame( std::string const& d, int id, bool sticky )
        : descr( d )
        , frame_id( id )
        , is_sticky( sticky )
        {}

        std::string descr;
        int         frame_id;
        bool        is_sticky;
    };
    typedef std::vector<context_frame> context_data;

    master_test_suite_t* m_master_test_suite;
    test_unit_id    m_curr_test_case;
    test_unit_store m_test_units;

    test_unit_id    m_next_test_case_id;
    test_unit_id    m_next_test_suite_id;

    bool            m_is_initialized;
    bool            m_test_in_progress;

    observer_store  m_observers;
    context_data    m_context;
    int             m_context_idx;
};

//____________________________________________________________________________//

namespace {

#if defined(__CYGWIN__)
framework_impl& s_frk_impl() { static framework_impl* the_inst = 0; if(!the_inst) the_inst = new framework_impl; return *the_inst; }
#else
framework_impl& s_frk_impl() { static framework_impl the_inst; return the_inst; }
#endif

} // local namespace

//____________________________________________________________________________//

namespace framework {

void
init( init_unit_test_func init_func, int argc, char* argv[] )
{
    runtime_config::init( argc, argv );

    // set the log level and format
    unit_test_log.set_threshold_level( runtime_config::log_level() );
    unit_test_log.set_format( runtime_config::log_format() );

    // set the report level and format
    results_reporter::set_level( runtime_config::report_level() );
    results_reporter::set_format( runtime_config::report_format() );

    register_observer( results_collector );
    register_observer( unit_test_log );

    if( runtime_config::show_progress() )
        register_observer( progress_monitor );

    if( runtime_config::detect_memory_leaks() > 0 ) {
        debug::detect_memory_leaks( true );
        debug::break_memory_alloc( runtime_config::detect_memory_leaks() );
    }

    // init master unit test suite
    master_test_suite().argc = argc;
    master_test_suite().argv = argv;

    try {
        boost::execution_monitor em;

        ut_detail::test_init_invoker tic( init_func );

        em.execute( tic );
    }
    catch( execution_exception const& ex )  {
        throw setup_error( ex.what() );
    }

    // Apply all decorators to the auto test units
    class apply_decorators : public test_tree_visitor {
    private:
        // test_tree_visitor interface
        virtual bool    visit( test_unit const& tu )
        {
            if( tu.p_decorators.get() )
                tu.p_decorators.get()->apply( const_cast<test_unit&>(tu) );

            return true;
        }
    } ad;
    traverse_test_tree( master_test_suite().p_id, ad, true );

    // Let's see if anything was disabled during construction. These test units and anything 
    // that depends on them are removed from the test tree
    class remove_disabled : public test_tree_visitor {
    public:
        // test_tree_visitor interface
        virtual bool    visit( test_unit const& tu )
        {
            // check if any of dependencies are disabled
            if( tu.p_enabled ) {
                BOOST_TEST_FOREACH( test_unit_id, dep_id, tu.p_dependencies.get() ) {
                    test_unit const& dep = framework::get( dep_id, tut_any );

                    if( !dep.p_enabled ) {
                        BOOST_TEST_MESSAGE( "Disable test " << tu.p_type_name << ' ' << tu.p_name << 
                                            " since it depends on desabled test " << dep.p_type_name << ' ' << dep.p_name );

                        tu.p_enabled.value = false;
                        break;
                    }
                }
            }

            // if this test unit is disabled - remove it from the tree and disable all subunits
            if( !tu.p_enabled && tu.p_id != master_test_suite().p_id ) {
                if( tu.p_type == tut_suite ) {
                    ut_detail::change_status disabler( false );
                    traverse_test_tree( tu.p_id, disabler, true );
                }

                framework::get<test_suite>( tu.p_parent_id ).remove( tu.p_id );
                m_made_changes = true;
            }

            return tu.p_enabled;
        }
        bool m_made_changes;
    } rd;

    do {
        rd.m_made_changes = false;
        traverse_test_tree( master_test_suite().p_id, rd, true );
    } while( rd.m_made_changes );

    // apply all name and label filters
    impl::apply_filters( master_test_suite().p_id );

    // We are Done!
    s_frk_impl().m_is_initialized = true;
}

//____________________________________________________________________________//

namespace impl {
void
apply_filters( test_unit_id tu_id )
{
    if( runtime_config::test_to_run().empty() ) {
        // enable all test units for this run
        ut_detail::change_status enabler( true );
        traverse_test_tree( tu_id, enabler, true );
    }
    else {
        // 10. First disable all test units. We'll re-enable only those that pass the filters
        ut_detail::change_status disabler( false );
        traverse_test_tree( tu_id, disabler, true );

        // 20. collect tu to enable based on filters
        ut_detail::tu_enable_list tu_to_enable;

        BOOST_TEST_FOREACH( std::string const&, filter, runtime_config::test_to_run() ) {
            if( filter.empty() )
                continue;

            if( filter[0] == '@' ) {
                ut_detail::label_filter lf( tu_to_enable, const_string(filter).trim_left(1) );
                traverse_test_tree( tu_id, lf, true );
            }
            else {
                ut_detail::name_filter nf( tu_to_enable, filter );
                traverse_test_tree( tu_id, nf, true );
            }
        }

        // 30. enable all tu collected along with their parents, dependencies and children where necessary
        while( !tu_to_enable.empty() ) {
            std::pair<test_unit_id,bool>    data = tu_to_enable.front();
            test_unit const&                tu   = framework::get( data.first, tut_any );

            tu_to_enable.pop_front();

            if( tu.p_enabled ) 
                continue;

            // 31. enable tu
            tu.p_enabled.value = true;

            // 32. master test suite - we are done
            if( tu.p_id == tu_id )
                continue;

            // 33. add parent to the list (without children)
            if( !framework::get( tu.p_parent_id, tut_any ).p_enabled )
                tu_to_enable.push_back( std::make_pair( tu.p_parent_id, false ) );

            // 34. add dependencies to the list (with children)
            BOOST_TEST_FOREACH( test_unit_id, dep_id, tu.p_dependencies.get() ) {
                test_unit const& dep = framework::get( dep_id, tut_any );

                if( !dep.p_enabled ) {
                    BOOST_TEST_MESSAGE( "Including test " << dep.p_type_name << ' ' << dep.p_name << 
                                        " as a dependacy of test " << tu.p_type_name << ' ' << tu.p_name );

                    tu_to_enable.push_back( std::make_pair( dep_id, true ) );
                }
            }

            // 35. add all children to the list recursively
            if( data.second && tu.p_type == tut_suite ) {
                class collect_disabled : public test_tree_visitor {
                public:
                    explicit        collect_disabled( ut_detail::tu_enable_list& tu_to_enable ) : m_tu_to_enable( tu_to_enable ) {}

                private:
                    // test_tree_visitor interface
                    virtual bool    visit( test_unit const& tu )
                    {
                        if( !tu.p_enabled )
                            m_tu_to_enable.push_back( std::make_pair( tu.p_id, false ) );

                        return true;
                    }

                    // Data members
                    ut_detail::tu_enable_list& m_tu_to_enable;
                } V( tu_to_enable );

                traverse_test_tree( tu.p_id, V, true );
            }
        }
    }
}

//____________________________________________________________________________//

} // namespace impl

bool
is_initialized()
{
    return  s_frk_impl().m_is_initialized;
}

//____________________________________________________________________________//

void
register_test_unit( test_case* tc )
{
    BOOST_TEST_SETUP_ASSERT( tc->p_id == INV_TEST_UNIT_ID, BOOST_TEST_L( "test case already registered" ) );

    test_unit_id new_id = s_frk_impl().m_next_test_case_id;

    BOOST_TEST_SETUP_ASSERT( new_id != MAX_TEST_CASE_ID, BOOST_TEST_L( "too many test cases" ) );

    typedef framework_impl::test_unit_store::value_type map_value_type;

    s_frk_impl().m_test_units.insert( map_value_type( new_id, tc ) );
    s_frk_impl().m_next_test_case_id++;

    s_frk_impl().set_tu_id( *tc, new_id );
}

//____________________________________________________________________________//

void
register_test_unit( test_suite* ts )
{
    BOOST_TEST_SETUP_ASSERT( ts->p_id == INV_TEST_UNIT_ID, BOOST_TEST_L( "test suite already registered" ) );

    test_unit_id new_id = s_frk_impl().m_next_test_suite_id;

    BOOST_TEST_SETUP_ASSERT( new_id != MAX_TEST_SUITE_ID, BOOST_TEST_L( "too many test suites" ) );

    typedef framework_impl::test_unit_store::value_type map_value_type;
    s_frk_impl().m_test_units.insert( map_value_type( new_id, ts ) );
    s_frk_impl().m_next_test_suite_id++;

    s_frk_impl().set_tu_id( *ts, new_id );
}

//____________________________________________________________________________//

void
deregister_test_unit( test_unit* tu )
{
    s_frk_impl().m_test_units.erase( tu->p_id );
}

//____________________________________________________________________________//

void
clear()
{
    s_frk_impl().clear();
}

//____________________________________________________________________________//

void
register_observer( test_observer& to )
{
    s_frk_impl().m_observers.insert( &to );
}

//____________________________________________________________________________//

void
deregister_observer( test_observer& to )
{
    s_frk_impl().m_observers.erase( &to );
}

//____________________________________________________________________________//

void
reset_observers()
{
    s_frk_impl().m_observers.clear();
}

//____________________________________________________________________________//

int
add_context( ::boost::unit_test::lazy_ostream const& context_descr, bool sticky )
{
    std::stringstream buffer;
    context_descr( buffer );
    int res_idx  = s_frk_impl().m_context_idx++;

    s_frk_impl().m_context.push_back( framework_impl::context_frame( buffer.str(), res_idx, sticky ) );

    return res_idx;
}

//____________________________________________________________________________//

struct frame_with_id {
    explicit frame_with_id( int id ) : m_id( id ) {}

    bool    operator()( framework_impl::context_frame const& f )
    {
        return f.frame_id == m_id;
    }
    int     m_id;
};

void
clear_context( int frame_id )
{
    if( frame_id == -1 ) {   // clear all non sticky frames
        for( int i=s_frk_impl().m_context.size()-1; i>=0; i-- )
            if( !s_frk_impl().m_context[i].is_sticky )
                s_frk_impl().m_context.erase( s_frk_impl().m_context.begin()+i );
    }
 
    else { // clear specific frame
        framework_impl::context_data::iterator it = 
            std::find_if( s_frk_impl().m_context.begin(), s_frk_impl().m_context.end(), frame_with_id( frame_id ) );

        if( it != s_frk_impl().m_context.end() ) // really an internal error if this is not true
            s_frk_impl().m_context.erase( it );
    }
}

//____________________________________________________________________________//

context_generator
get_context()
{
    return context_generator();
}

//____________________________________________________________________________//

bool
context_generator::is_empty() const
{
    return s_frk_impl().m_context.empty();
}

//____________________________________________________________________________//

const_string
context_generator::next() const
{
    return m_curr_frame < s_frk_impl().m_context.size() ? s_frk_impl().m_context[m_curr_frame++].descr : const_string();
}

//____________________________________________________________________________//

master_test_suite_t&
master_test_suite()
{
    if( !s_frk_impl().m_master_test_suite )
        s_frk_impl().m_master_test_suite = new master_test_suite_t;

    return *s_frk_impl().m_master_test_suite;
}

//____________________________________________________________________________//

test_case const&
current_test_case()
{
    return get<test_case>( s_frk_impl().m_curr_test_case );
}

//____________________________________________________________________________//

test_unit&
get( test_unit_id id, test_unit_type t )
{
    test_unit* res = s_frk_impl().m_test_units[id];

    if( (res->p_type & t) == 0 )
        throw internal_error( "Invalid test unit type" );

    return *res;
}

//____________________________________________________________________________//

void
run( test_unit_id id, bool continue_test )
{
    if( id == INV_TEST_UNIT_ID )
        id = master_test_suite().p_id;

    test_case_counter tcc;
    traverse_test_tree( id, tcc );

    BOOST_TEST_SETUP_ASSERT( tcc.p_count != 0 , runtime_config::test_to_run().empty() 
        ? BOOST_TEST_L( "test tree is empty" ) 
        : BOOST_TEST_L( "no test cases matching filter" ) );

    bool    call_start_finish   = !continue_test || !s_frk_impl().m_test_in_progress;
    bool    was_in_progress     = s_frk_impl().m_test_in_progress;

    s_frk_impl().m_test_in_progress = true;

    if( call_start_finish ) {
        BOOST_TEST_FOREACH( test_observer*, to, s_frk_impl().m_observers ) {
            boost::execution_monitor em;

            try {
                em.vexecute( boost::bind( &test_observer::test_start, to, tcc.p_count ) );
            }
            catch( execution_exception const& ex )  {
                throw setup_error( ex.what() );
            }
        }
    }

    switch( runtime_config::random_seed() ) {
    case 0:
        break;
    case 1: {
        unsigned int seed = static_cast<unsigned int>( std::time( 0 ) );
        BOOST_TEST_MESSAGE( "Test cases order is shuffled using seed: " << seed );
        std::srand( seed );
        break;
    }
    default:
        BOOST_TEST_MESSAGE( "Test cases order is shuffled using seed: " << runtime_config::random_seed() );
        std::srand( runtime_config::random_seed() );
    }

    try {
        traverse_test_tree( id, s_frk_impl() );
    }
    catch( framework::test_being_aborted const& ) {
        // abort already reported
    }

    if( call_start_finish ) {
        BOOST_TEST_REVERSE_FOREACH( test_observer*, to, s_frk_impl().m_observers )
            to->test_finish();
    }

    s_frk_impl().m_test_in_progress = was_in_progress;
}

//____________________________________________________________________________//

void
run( test_unit const* tu, bool continue_test )
{
    run( tu->p_id, continue_test );
}

//____________________________________________________________________________//

void
assertion_result( bool passed )
{
    BOOST_TEST_FOREACH( test_observer*, to, s_frk_impl().m_observers )
        to->assertion_result( passed );
}

//____________________________________________________________________________//

void
exception_caught( execution_exception const& ex )
{
    BOOST_TEST_FOREACH( test_observer*, to, s_frk_impl().m_observers )
        to->exception_caught( ex );
}

//____________________________________________________________________________//

void
test_unit_aborted( test_unit const& tu )
{
    BOOST_TEST_FOREACH( test_observer*, to, s_frk_impl().m_observers )
        to->test_unit_aborted( tu );
}

//____________________________________________________________________________//

} // namespace framework
} // namespace unit_test
} // namespace boost

#include <boost/test/detail/enable_warnings.hpp>

#endif // BOOST_TEST_FRAMEWORK_IPP_021005GER
