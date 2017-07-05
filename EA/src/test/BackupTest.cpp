/*
 * BackupTest.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include "../pch.h"
#include <boost/test/unit_test.hpp>

#include "../EA.h"
#include "core/PopulationFixture.h"
#include <boost/filesystem.hpp>

namespace ea {

namespace test {

namespace utf = boost::unit_test;
using namespace boost::filesystem;

BOOST_AUTO_TEST_SUITE(BackupTest)

BOOST_FIXTURE_TEST_CASE(BackupHookClearTest, PopulationFixture) {
	remove_all("backup");

	BackupHookPtr hook;
	BOOST_REQUIRE_NO_THROW(
			hook = population->CreateHook<BackupHook>("backup", 2, true));
	BOOST_REQUIRE_NO_THROW(population->Evolve(RandomCount()));

	uint noFiles = std::count_if(directory_iterator("backup"),
			directory_iterator(),
			static_cast<bool (*)(const path&)>(is_regular_file));

	BOOST_CHECK(noFiles == (population->GetGeneration() + 1) / 2);
}

BOOST_TEST_DECORATOR(*utf::depends_on("BackupTest/BackupHookClearTest"))
BOOST_FIXTURE_TEST_CASE(BackupHookKeepTest, PopulationFixture) {
	uint oldNoFiles = std::count_if(directory_iterator("backup"),
			directory_iterator(),
			static_cast<bool (*)(const path&)>(is_regular_file));

	BackupHookPtr hook;
	BOOST_REQUIRE_NO_THROW(
			hook = population->CreateHook<BackupHook>("backup", 2));
	BOOST_REQUIRE_NO_THROW(population->Evolve(RandomCount()));

	uint noFiles = std::count_if(directory_iterator("backup"),
			directory_iterator(),
			static_cast<bool (*)(const path&)>(is_regular_file));

	BOOST_CHECK(noFiles >= std::max<uint>((population->GetGeneration() + 1) / 2, oldNoFiles));

	remove_all("backup");
}

BOOST_TEST_DECORATOR(*utf::depends_on("BackupTest/BackupHookClearTest"))
BOOST_FIXTURE_TEST_CASE(RestoreTest, PopulationFixture) {
	BackupHookPtr hook;
	hook = population->CreateHook<BackupHook>("backup", 2, true);
	uint count = RandomCount();
	population->Evolve(count);
	uint evaluation = population->GetEvaluation();

	population->Evolve(RandomCount());
	uint newCount = population->GetGeneration();
	uint newEvl = population->GetEvaluation();

	// Start from last
	PopulationPtr newPop = make_shared<Population>(10);
	BOOST_REQUIRE_NO_THROW(newPop->CreateInitializer<RestoreInitializer>("backup"));
	newPop->SetEvaluator(evaluator);
	newPop->Initialize();

	BOOST_CHECK(newPop->GetGeneration() == newCount);
	BOOST_CHECK(newPop->GetEvaluation() == newEvl);

	// Start from count
	newPop = make_shared<Population>(10);
	BOOST_REQUIRE_NO_THROW(newPop->CreateInitializer<RestoreInitializer>("backup", count));
	newPop->SetEvaluator(evaluator);
	newPop->Initialize();

	BOOST_CHECK(newPop->GetGeneration() == count);
	BOOST_CHECK(newPop->GetEvaluation() == evaluation);

	remove_all("backup");
}

BOOST_AUTO_TEST_SUITE_END()

}// namespace test

}// namespace ea
