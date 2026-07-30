//
// Copyright (c) 2003 Boost.Test contributors
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Documentation-only translation unit: the single target MrDocs analyses.
//
// It exists because no Boost.Test header transitively includes everything that
// belongs in the reference -- unit_test.hpp pulls in neither the datasets, nor
// the output formatters, nor the tools/detail manipulators. The list below is
// the one the Doxygen target in doc/Jamfile.v2 curated before the Antora port,
// carried over unchanged; it is the public surface, and is the place to add a
// header that should appear in the reference.
//
// Compiling one umbrella TU rather than the whole library is also what keeps
// MrDocs fast: it has the entire public API in a single compilation database
// entry.
//
// Deliberately absent: boost/test/included/*, minimal.hpp, auto_unit_test.hpp,
// test_exec_monitor.hpp and the top-level floating_point_comparison.hpp and
// output_test_stream.hpp shims. The first pulls in the .ipp definitions; the
// rest are deprecated forwarding headers that emit #pragma message or #error.

// BOOST_TEST_DOXYGEN_DOC__ is deliberately NOT defined here, even though the
// Doxygen target that preceded this one set it. It exists for a tool that does
// not compile what it reads, and MrDocs does:
//
//   - data/monomorphic/fwd.hpp hides its forward declarations behind
//     `#if !defined(BOOST_TEST_DOXYGEN_DOC__)`, so with the macro set,
//     make_delayed() refers to an undeclared delayed_dataset and the header
//     does not compile;
//   - data/config.hpp defines BOOST_TEST_NO_{RANDOM_DATASET,GRID_COMPOSITION,
//     ZIP_COMPOSITION}_AVAILABLE when it is set, which would drop the random,
//     grid and zip datasets from the reference entirely.
//
// The three configuration macros it also reveals -- those and
// BOOST_TEST_DISABLE_ALT_STACK in execution_monitor.hpp -- carry no doc
// comments MrDocs could use anyway; they are documented by hand in
// modules/ROOT/pages/utf_reference/link_references.adoc.

// Core
#include <boost/test/debug_config.hpp>
#include <boost/test/debug.hpp>
#include <boost/test/execution_monitor.hpp>
#include <boost/test/framework.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_parameters.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/test/progress_monitor.hpp>
#include <boost/test/detail/global_typedef.hpp>

// Test tree
#include <boost/test/tree/observer.hpp>
#include <boost/test/tree/test_unit.hpp>

// Logs, reports and formatters
#include <boost/test/unit_test_log.hpp>
#include <boost/test/unit_test_log_formatter.hpp>
#include <boost/test/results_collector.hpp>
#include <boost/test/results_reporter.hpp>
#include <boost/test/output/compiler_log_formatter.hpp>
#include <boost/test/output/plain_report_formatter.hpp>
#include <boost/test/output/xml_log_formatter.hpp>
#include <boost/test/output/xml_report_formatter.hpp>

// Assertion support
#include <boost/test/tools/assertion_result.hpp>
#include <boost/test/tools/floating_point_comparison.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/test/tools/detail/bitwise_manip.hpp>
#include <boost/test/tools/detail/lexicographic_manip.hpp>
#include <boost/test/tools/detail/per_element_manip.hpp>
#include <boost/test/tools/detail/tolerance_manip.hpp>

// Datasets
#include <boost/test/data/config.hpp>
#include <boost/test/data/for_each_sample.hpp>
#include <boost/test/data/size.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic/fwd.hpp>
#include <boost/test/data/monomorphic/array.hpp>
#include <boost/test/data/monomorphic/collection.hpp>
#include <boost/test/data/monomorphic/delayed.hpp>
#include <boost/test/data/monomorphic/generate.hpp>
#include <boost/test/data/monomorphic/grid.hpp>
#include <boost/test/data/monomorphic/initializer_list.hpp>
#include <boost/test/data/monomorphic/join.hpp>
#include <boost/test/data/monomorphic/singleton.hpp>
#include <boost/test/data/monomorphic/zip.hpp>

// Dataset generators
#include <boost/test/data/monomorphic/generators.hpp>
#include <boost/test/data/monomorphic/generators/keywords.hpp>
#include <boost/test/data/monomorphic/generators/random.hpp>
#include <boost/test/data/monomorphic/generators/xrange.hpp>

// Utilities
#include <boost/test/utils/algorithm.hpp>
#include <boost/test/utils/is_forward_iterable.hpp>
#include <boost/test/utils/named_params.hpp>
