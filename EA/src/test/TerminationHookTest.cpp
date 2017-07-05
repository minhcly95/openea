/*
 * TerminationHookTest.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include "../pch.h"
#include <boost/test/unit_test.hpp>

#include "../EA.h"
#include "core/PopulationFixture.h"

namespace ea {

namespace test {

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(TerminationHookTest)

BOOST_FIXTURE_TEST_CASE(GenerationTerminationTest, PopulationFixture) {
	uint target = RandomCount();
	BOOST_REQUIRE_NO_THROW(population->CreateHook<GenerationTerminationHook>(target, false));

	population->Evolve();
	BOOST_CHECK(population->GetGeneration() >= target);
}

BOOST_FIXTURE_TEST_CASE(EvaluationTerminationTest, PopulationFixture) {
	uint target = RandomCount() * 10;
	BOOST_REQUIRE_NO_THROW(population->CreateHook<EvaluationTerminationHook>(target, false));

	population->Evolve();
	BOOST_CHECK(population->GetEvaluation() >= target);
}

BOOST_AUTO_TEST_SUITE_END()

}// namespace test

}// namespace ea
