/*
 * InterfaceTest.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include "../../pch.h"
#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>

using boost::test_tools::output_test_stream;

#include "../../EA.h"
#include "TestInterfaces.h"
#include "InterfaceFixture.h"

namespace ea {

namespace test {

BOOST_AUTO_TEST_SUITE(CoreTest)

BOOST_AUTO_TEST_SUITE(InterfaceTest)

BOOST_FIXTURE_TEST_CASE(RecombinatorTest, InterfaceFixture) {
	uint count = recombinator->GetParentCount();

	vector<GenomePtr> parents(count);
	BOOST_CHECK_NO_THROW(recombinator->Combine(parents));

	uint newCount;
	do
		newCount = RandomCount();
	while (newCount == count);

	parents.resize(newCount);
	BOOST_CHECK_THROW(recombinator->Combine(parents), EAException);
}

BOOST_FIXTURE_TEST_CASE(MutatorTest, InterfaceFixture) {
	BOOST_REQUIRE(mutator->GetParentCount() == 1);
	BOOST_CHECK_NO_THROW(mutator->Apply(genome));

	// Inheritance check
	vector<GenomePtr> parents(1);
	BOOST_CHECK_NO_THROW(mutator->Combine(parents));

	uint newCount;
	do
		newCount = RandomCount();
	while (newCount == 1);

	parents.resize(newCount);
	BOOST_CHECK_THROW(mutator->Combine(parents), EAException);
	parents.resize(0);
	BOOST_CHECK_THROW(mutator->Combine(parents), EAException);
}

BOOST_FIXTURE_TEST_CASE(InitializerTest, InterfaceFixture) {
	VectorPtr<GenomePtr> pool;
	uint oldCount, newCount;

	BOOST_REQUIRE(initializer->GetParentCount() == 0);

	// Length check
	BOOST_REQUIRE_NO_THROW(pool = initializer->Initialize(0));
	BOOST_TEST(pool->size() == 0);

	oldCount = RandomCount();
	BOOST_REQUIRE_NO_THROW(pool = initializer->Initialize(oldCount));
	BOOST_TEST(pool->size() == oldCount);

	// Inheritance check
	vector<GenomePtr> parents(0);
	BOOST_CHECK_NO_THROW(initializer->Combine(parents));

	parents.resize(RandomCount());
	BOOST_CHECK_THROW(initializer->Combine(parents), EAException);

	// Fake length check
	do
		newCount = RandomCount();
	while (newCount == oldCount);

	static_pointer_cast<TestInitializer>(initializer)->SetFakeSize(newCount);
	BOOST_CHECK_THROW(initializer->Initialize(oldCount), EAException);
}

BOOST_FIXTURE_TEST_CASE(SelectorTest, InterfaceFixture) {
	OrganismPoolPtr newPool, pool = make_shared<OrganismPool>(1u);
	uint oldCount, newCount;

	// Length check
	BOOST_REQUIRE_NO_THROW(newPool = selector->Select(pool, 0));
	BOOST_TEST(newPool->size() == 0);

	oldCount = RandomCount();
	BOOST_REQUIRE_NO_THROW(newPool = selector->Select(pool, oldCount));
	BOOST_TEST(newPool->size() == oldCount);

	// Fake length check
	do
		newCount = RandomCount();
	while (newCount == oldCount);

	static_pointer_cast<TestSelector>(selector)->SetFakeSize(newCount);
	BOOST_CHECK_THROW(selector->Select(pool, oldCount), EAException);
}

BOOST_FIXTURE_TEST_CASE(EvaluatorTest, InterfaceFixture) {
	OrganismPtr org = evaluator->Evaluate(genome);
	BOOST_REQUIRE(org->GetFitness());

	double target = RandomRatio();
	static_pointer_cast<TestEvaluator>(evaluator)->SetTargetFitness(target);

	org = evaluator->Evaluate(genome);
	BOOST_CHECK(org->GetFitness());
	BOOST_CHECK(org->GetFitnessValue() == target);
}

BOOST_FIXTURE_TEST_CASE(GenomeTest, InterfaceFixture) {
	auto testGenome = dynamic_pointer_cast<TestGenome>(genome);
	BOOST_REQUIRE(testGenome);

	// Print test
	output_test_stream output;
	output << *genome;
	BOOST_CHECK(output.is_equal("<TestGenome>"));

	// Clone test
	GenomePtr copy = genome->CloneBase();
	auto testGenomeCopy = dynamic_pointer_cast<TestGenome>(copy);
	BOOST_REQUIRE(testGenomeCopy);

	BOOST_CHECK(testGenome != testGenomeCopy);
}

BOOST_FIXTURE_TEST_CASE(HookTest, InterfaceFixture) {
	auto testHook = dynamic_pointer_cast<TestHook>(hook);
	BOOST_REQUIRE(testHook);

	BOOST_CHECK_THROW(testHook->Generational(), EAException);
	BOOST_CHECK_THROW(testHook->Initial(), EAException);
	BOOST_CHECK_THROW(testHook->Start(), EAException);
	BOOST_CHECK_THROW(testHook->End(), EAException);

	Population pop(1);
	BOOST_REQUIRE_NO_THROW(pop.AddHook(testHook));

	// Functional test
	uint count = RandomCount();
	for (uint i = 0; i < count; i++)
		BOOST_REQUIRE_NO_THROW(testHook->Generational());
	BOOST_CHECK(testHook->generational == count);

	count = RandomCount();
	for (uint i = 0; i < count; i++)
		BOOST_REQUIRE_NO_THROW(testHook->Initial());
	BOOST_CHECK(testHook->initial == count);

	count = RandomCount();
	for (uint i = 0; i < count; i++)
		BOOST_REQUIRE_NO_THROW(testHook->Start());
	BOOST_CHECK(testHook->start == count);

	count = RandomCount();
	for (uint i = 0; i < count; i++)
		BOOST_REQUIRE_NO_THROW(testHook->End());
	BOOST_CHECK(testHook->end == count);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}	// namespace test

}	// namespace ea
