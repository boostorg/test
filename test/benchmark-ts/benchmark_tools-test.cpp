//  (C) Copyright Raffi Enficiaud 2020.
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/test for the library home page.
//
//! @file
//! @brief Unit test for benchmark::test results and logging
// ***************************************************************************

#define BOOST_TEST_MODULE benchmark::tools test
#include <boost/test/included/unit_test.hpp>
#include <boost/test/data/test_case.hpp>


// Boost.Test Benchmark
#include <boost/test/benchmark/time.hpp>
#include <boost/test/benchmark/sampling/average.hpp>
#include <boost/test/benchmark/sampling/skip_first.hpp>
#include <boost/test/benchmark/sampling/skip_outliers.hpp>
#include <boost/test/benchmark/sampling/stop_on_convergence.hpp>
#include <boost/test/benchmark/sampling/chain.hpp>

#include <boost/test/benchmark/benchmark_tools.hpp>

#include <thread>
#include <mutex>

namespace ut = boost::unit_test;
namespace tt = boost::test_tools;
namespace bm = boost::benchmark;
namespace data=boost::unit_test::data;
namespace chrono=boost::chrono;


template <
  class timer_t,
  class sampling_policy_t>
struct probe_observer_thread_impl;
template <
  class timer_t,
  class sampling_policy_t>
struct probe_observer_impl;


// thread acc
// at destruction (when thread exits) contributes to global acc
template <
  class timer_t = boost::benchmark::timer::high_resolution,
  class sampling_policy_t = boost::benchmark::sampling::average<typename timer_t::duration> >
struct probe_observer_thread_impl {

  using observer = probe_observer_impl<timer_t, sampling_policy_t>;

  probe_observer_thread_impl(observer& ref_probe)
  : probe(&ref_probe)
  {}

  ~probe_observer_thread_impl() {
    collect_results();
  }

  struct scoped_time_t {
    timer_t time;
    scoped_time_t(probe_observer_thread_impl& ref)
    : m_collector(ref)
    {}

    ~scoped_time_t() {
      m_collector.samples.add_sample( time.elapsed() );
    }

    probe_observer_thread_impl& m_collector;
  };


  scoped_time_t time() {
    return scoped_time_t(*this);
  }

  void collect_results();

  observer* probe;
  std::thread::id m_thread_id = std::this_thread::get_id();
  sampling_policy_t samples;
};


// global acc
template <
  class timer_t = boost::benchmark::timer::high_resolution,
  class sampling_policy_t = boost::benchmark::sampling::average<>>
struct probe_observer_impl {

  using thread_observer = probe_observer_thread_impl<timer_t, sampling_policy_t>;

  thread_observer& get_scope_probe() {
    static thread_local thread_observer thread_inst = thread_observer(*this);
    return thread_inst;
  }

  void collect_results(std::thread::id) {

  }

  std::mutex collect_mutex;

};

template <class timer_t, class sampling_policy_t>
void probe_observer_thread_impl<timer_t, sampling_policy_t>::collect_results() {
  probe->collect_results(this->m_thread_id);
}



struct probe_register_impl {

  using probe_number_id = size_t;
  struct probe_info;
  struct probe_observer;
  using probe_name_t = std::string;

  using probe_info_store_t = std::map<probe_number_id, probe_info>;
  using probe_id = probe_info_store_t::iterator;

  using probe_observer_store_t = std::map<probe_name_t, probe_observer>;

  using probe_observer_t = probe_observer_impl<>;


  probe_info_store_t probe_info_store;
  probe_observer_store_t probe_observer_store;
  probe_number_id m_current_id = 1;


  //! Information about a probe
  struct probe_info {
    int line;
    std::string name;
    const char* function;
    const char* file;
    probe_observer *p_observer;
  };

  struct probe_observer {
    probe_observer_t observer;
  };


  //! @returns a unique id on a new probe
  probe_id
  register_probe(const char* name, const char *function, const char* file, int line) {
    probe_observer& observer = probe_observer_store[name];
    const auto it_success = probe_info_store.insert({m_current_id, probe_info{line, name, function, file, &observer}});
    return it_success.first;
  }

  //! Returns the observer of an exact probe
  auto get_probe(probe_id id) {
      return id->second.p_observer->observer.get_scope_probe().time();
  }

  //! Returns the set of observers associated to a particular probe name
  probe_observer& get_probe_observers(const char* name) {
    return probe_observer_store[name];
  }

};

inline
probe_register_impl& get_probe_register() {
  static probe_register_impl inst = probe_register_impl();
  return inst;
}

#define BOOST_TEST_BENCHMARK_PROBE_REGISTER( name )                       \
static probe_register_impl::probe_id BOOST_TEST_PROBE_UNIQUE_ID( name ) = \
    get_probe_register().register_probe(                                  \
        BOOST_STRINGIZE(name),                                            \
        BOOST_CURRENT_FUNCTION,                                           \
        __FILE__,                                                         \
        __LINE__                                                          \
    );                                                                    \
  auto BOOST_TEST_PROBE_INSTANCE_UNIQUE_ID( name ) =                      \
    get_probe_register().get_probe( BOOST_TEST_PROBE_UNIQUE_ID( name ) )


void subfunction_to_probe() {
  BOOST_TEST_BENCHMARK_PROBE_REGISTER( test1 );

}



void function_to_probe() {
  BOOST_TEST_BENCHMARK_PROBE_REGISTER( test1 );

  subfunction_to_probe();
}



BOOST_AUTO_TEST_CASE( check_probe )
{

  function_to_probe();

}
