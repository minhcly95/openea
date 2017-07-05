/*
 * PopulationTest.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_MODULE EA-Test
#define BOOST_TEST_DYN_LINK
#include "../../pch.h"
#include <boost/test/unit_test.hpp>

#include "../../EA.h"
#include "TestInterfaces.h"
#include "InterfaceFixture.h"

namespace ea {

namespace test {

namespace utf = boost::unit_test;
using namespace std;

struct GlobalFixture {
	GlobalFixture() {
		Log::Clear(Log::DEBUG);
	}
};
BOOST_GLOBAL_FIXTURE(GlobalFixture);

BOOST_AUTO_TEST_SUITE(CoreTest)

BOOST_AUTO_TEST_SUITE(PopulationTest)

BOOST_FIXTURE_TEST_CASE(ConstructionTest, InterfaceFixture) {
	uint count = RandomCount();
	PopulationPtr pop;
	BOOST_REQUIRE_NO_THROW(pop = make_shared<Population>(count));

	BOOST_CHECK(pop->GetSize() == count);
	BOOST_CHECK(pop->GetGeneration() == 0);
	BOOST_CHECK(pop->IsInitialized() == false);

	BOOST_REQUIRE_NO_THROW(pop->mRecombinators.size());
	BOOST_CHECK(pop->mRecombinators.size() == 0);

	BOOST_REQUIRE_NO_THROW(pop->mMutators.size());
	BOOST_CHECK(pop->mMutators.size() == 0);
}

BOOST_TEST_DECORATOR(*utf::depends_on("CoreTest/PopulationTest/ConstructionTest"))
BOOST_FIXTURE_TEST_CASE(InitializationTest, InterfaceFixture) {
	// Construction
	uint size = RandomCount();
	PopulationPtr pop;
	BOOST_REQUIRE_NO_THROW(pop = make_shared<Population>(size));

	BOOST_REQUIRE_NO_THROW(pop->Initialize(initializer, evaluator));

	BOOST_CHECK(pop->mMainPool->size() == size);
	BOOST_CHECK(pop->GetGeneration() == 0);
	BOOST_CHECK(pop->IsInitialized() == true);

	for (OrganismPtr org : *pop->mMainPool)
		BOOST_CHECK(org->GetFitness());

	BOOST_CHECK(pop->mEvaluator == evaluator);
}

BOOST_TEST_DECORATOR(*utf::depends_on("CoreTest/PopulationTest/InitializationTest"))
BOOST_FIXTURE_TEST_CASE(ConfigurationTest, InterfaceFixture) {
	// Construction
	uint size = RandomCount();
	PopulationPtr pop;
	BOOST_REQUIRE_NO_THROW(pop = make_shared<Population>(size));

	// Mutators add/remove
	uint count = RandomCount() + 1;
	for (uint i = 0; i < count; i++)
		BOOST_REQUIRE_NO_THROW(pop->AddMutator(mutator, RandomRate()));
	for (uint i = 0; i < count; i++)
		BOOST_CHECK(pop->GetMutator(i)->GetMutator() == mutator);
	BOOST_CHECK(pop->mMutators.size() == count);
	BOOST_REQUIRE_NO_THROW(pop->RemoveMutator(0));
	BOOST_CHECK(pop->mMutators.size() == count - 1);

	// Recombinators add/remove
	count = RandomCount() + 1;
	for (uint i = 0; i < count; i++)
		BOOST_REQUIRE_NO_THROW(pop->AddRecombinator(recombinator, selector, RandomRatio()));
	for (uint i = 0; i < count; i++) {
		BOOST_CHECK(pop->GetRecombinator(i)->GetRecombinator() == recombinator);
		BOOST_CHECK(pop->GetRecombinator(i)->GetSelector() == selector);
	}
	BOOST_CHECK(pop->mRecombinators.size() == count);
	BOOST_REQUIRE_NO_THROW(pop->RemoveRecombinator(0));
	BOOST_CHECK(pop->mRecombinators.size() == count - 1);

	// Selector set/remove
	BOOST_REQUIRE_NO_THROW(pop->SetSurvivalSelector(selector));
	BOOST_CHECK(pop->GetSurvivalSelector() == selector);

	// Evaluator set/remove
	BOOST_REQUIRE_NO_THROW(pop->SetEvaluator(evaluator));
	BOOST_CHECK(pop->GetEvaluator() == evaluator);

	// Is ready
	BOOST_REQUIRE_NO_THROW(pop->Initialize(initializer, evaluator));
	BOOST_CHECK(pop->IsReady());
}

BOOST_TEST_DECORATOR(*utf::depends_on("CoreTest/PopulationTest/ConfigurationTest"))
BOOST_FIXTURE_TEST_CASE(PhaseTest, InterfaceFixture) {
	// Construct + Initialize

	uint size = RandomCount();
	PopulationPtr pop;
	BOOST_REQUIRE_NO_THROW(pop = make_shared<Population>(size));
	BOOST_REQUIRE_NO_THROW(pop->Initialize(initializer, evaluator));

	double rate = RandomRate(), ratio = RandomRatio();
	BOOST_REQUIRE_NO_THROW(pop->AddMutator(mutator, rate));
	BOOST_REQUIRE_NO_THROW(pop->AddRecombinator(recombinator, selector, ratio));
	BOOST_REQUIRE_NO_THROW(pop->SetSurvivalSelector(selector));

	// Is ready
	BOOST_REQUIRE(pop->IsReady());
	BOOST_REQUIRE(pop->GetGeneration() == 0);

	// Phase test
	// Spawn
	BOOST_REQUIRE_NO_THROW(pop->mSpawnPool->size());
	uint numOffspring = ceil(ratio * size);
	BOOST_REQUIRE_NO_THROW(pop->Spawn());
	BOOST_CHECK(pop->mSpawnPool->size() == numOffspring);

	// Mutate
	numOffspring = pop->mSpawnPool->size();
	BOOST_REQUIRE_NO_THROW(pop->Mutate());
	BOOST_CHECK(pop->mSpawnPool->size() == numOffspring);

	// Evaluate
	numOffspring = pop->mSpawnPool->size();
	uint numPool = pop->mMainPool->size();
	BOOST_REQUIRE_NO_THROW(pop->Evaluate());
	BOOST_CHECK(pop->mMainPool->size() == numOffspring + numPool);

	for (uint i = 0; i < pop->mMainPool->size(); i++)
		BOOST_CHECK(pop->mMainPool->at(0)->GetFitness());

	// Filter
	BOOST_REQUIRE_NO_THROW(pop->Filter());
	BOOST_CHECK(pop->mMainPool->size() == size);
}

BOOST_TEST_DECORATOR(*utf::depends_on("CoreTest/PopulationTest/PhaseTest"))
BOOST_FIXTURE_TEST_CASE(EvolutionTest, InterfaceFixture) {
	// Construct + Initialize

	uint size = RandomCount();
	PopulationPtr pop;
	BOOST_REQUIRE_NO_THROW(pop = make_shared<Population>(size));
	BOOST_REQUIRE_NO_THROW(pop->SetInitializer(initializer));
	BOOST_REQUIRE_NO_THROW(pop->SetEvaluator(evaluator));

	BOOST_REQUIRE_NO_THROW(pop->AddMutator(mutator, RandomRate()));
	BOOST_REQUIRE_NO_THROW(pop->AddRecombinator(recombinator, selector, RandomRatio()));
	BOOST_REQUIRE_NO_THROW(pop->SetSurvivalSelector(selector));
	BOOST_REQUIRE_NO_THROW(pop->AddHook(hook));

	shared_ptr<TestHook> testHook = static_pointer_cast<TestHook>(hook);

	// Is ready
	BOOST_REQUIRE(pop->IsReady());
	BOOST_REQUIRE(pop->GetGeneration() == 0);

	// Evolve test & Hook check
	uint target = RandomCount();
	BOOST_REQUIRE_NO_THROW(pop->EvolveTo(target));
	BOOST_CHECK(pop->GetGeneration() == target);
	BOOST_CHECK(testHook->generational == target);
	BOOST_CHECK(testHook->initial == 1);
	BOOST_CHECK(testHook->start == 1);
	BOOST_CHECK(testHook->end == 1);

	BOOST_REQUIRE_NO_THROW(pop->EvolveOnce());
	BOOST_CHECK(pop->GetGeneration() == 1 + target);
	BOOST_CHECK(testHook->generational == 1 + target);
	BOOST_CHECK(testHook->initial == 1);
	BOOST_CHECK(testHook->start == 1);
	BOOST_CHECK(testHook->end == 1);

	uint count = RandomCount();
	BOOST_REQUIRE_NO_THROW(pop->Evolve(count));
	BOOST_CHECK(pop->GetGeneration() == 1 + count + target);
	BOOST_CHECK(testHook->generational == 1 + count + target);
	BOOST_CHECK(testHook->initial == 1);
	BOOST_CHECK(testHook->start == 2);
	BOOST_CHECK(testHook->end == 2);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}	// namespace test

}	// namespace ea

