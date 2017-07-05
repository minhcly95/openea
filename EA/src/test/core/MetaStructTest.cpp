/*
 * MetaStructTest.cpp
 *
 *  Created on: Mar 21, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include "../../pch.h"
#include <boost/test/unit_test.hpp>

#include "../../EA.h"
#include "TestInterfaces.h"
#include "InterfaceFixture.h"

namespace ea {

namespace test {

BOOST_AUTO_TEST_SUITE(CoreTest)

BOOST_AUTO_TEST_SUITE(MetaStructTest)

BOOST_FIXTURE_TEST_CASE(MetaMutatorTest, InterfaceFixture) {
	double rate = RandomRate();
	MetaMutatorPtr meta;

	BOOST_REQUIRE_NO_THROW(meta = make_shared<MetaMutator>(mutator, rate));
	BOOST_TEST(meta->GetMutator() == mutator);
	BOOST_TEST(meta->GetRate() == rate);

	BOOST_CHECK_NO_THROW(make_shared<MetaMutator>(mutator, 0));
	BOOST_CHECK_NO_THROW(make_shared<MetaMutator>(mutator, 1));

	BOOST_CHECK_THROW(make_shared<MetaMutator>(mutator, 1.5), EAException);
	BOOST_CHECK_THROW(make_shared<MetaMutator>(mutator, -0.5), EAException);
}

BOOST_FIXTURE_TEST_CASE(MetaRecombinatorTest, InterfaceFixture) {
	double ratio = RandomRatio();
	MetaRecombinatorPtr meta;

	BOOST_REQUIRE_NO_THROW(meta = make_shared<MetaRecombinator>(recombinator, selector, ratio));
	BOOST_TEST(meta->GetRecombinator() == recombinator);
	BOOST_TEST(meta->GetSelector() == selector);
	BOOST_TEST(meta->GetRatio() == ratio);

	BOOST_CHECK_NO_THROW(make_shared<MetaRecombinator>(recombinator, selector, 0));

	BOOST_CHECK_THROW(make_shared<MetaRecombinator>(recombinator, selector, -0.5), EAException);
}

BOOST_FIXTURE_TEST_CASE(OrganismTest, InterfaceFixture) {
	OrganismPtr org;

	BOOST_REQUIRE_NO_THROW(org = make_shared<Organism>(genome));
	BOOST_TEST(org->GetGenome() == genome);
	BOOST_TEST(!org->GetFitness());

	BOOST_REQUIRE_NO_THROW(org = evaluator->Evaluate(genome));
	BOOST_TEST(org->GetGenome() == genome);
	BOOST_TEST(org->GetFitness());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()

}	// namespace test

}	// namespace ea
