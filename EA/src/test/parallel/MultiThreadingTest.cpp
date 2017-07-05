/*
 * MultiThreadingTest.cpp
 *
 *  Created on: May 15, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../EA.h"
#include <unistd.h>

namespace ea {
namespace test {

BOOST_AUTO_TEST_SUITE(MultiThreadingTest)

BOOST_AUTO_TEST_CASE(NumThreadsTest) {
	uint cores = sysconf(_SC_NPROCESSORS_ONLN);

	Population population(1);
	BOOST_CHECK(population.GetNumThreads() == 0);
	BOOST_CHECK(population.GetRealNumThreads() == cores);

	for (uint i = 1; i <= 4; i++) {
		BOOST_REQUIRE_NO_THROW(population.SetNumThreads(i));
		BOOST_CHECK(population.GetNumThreads() == i);
		BOOST_CHECK(population.GetRealNumThreads() == i);
	}

	BOOST_REQUIRE_NO_THROW(population.SetNumThreads(0));
	BOOST_CHECK(population.GetNumThreads() == 0);
	BOOST_CHECK(population.GetRealNumThreads() == cores);
}

BOOST_AUTO_TEST_SUITE_END()

}}

