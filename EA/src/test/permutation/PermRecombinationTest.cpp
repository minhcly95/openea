/*
 * PermRecombinationTest.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../EA.h"

namespace ea {
namespace test {

using namespace std;

template<class T>
struct PermRecombinationFixture {
	PermRecombinationFixture() {
	}

	~PermRecombinationFixture() {
	}

	bool Run(uint length1, uint length2) {
		vector<uint> p1genes(length1);
		vector<uint> p2genes(length2);

		iota(p1genes.begin(), p1genes.end(), 0);
		iota(p2genes.begin(), p2genes.end(), 0);

		shuffle(p1genes.begin(), p1genes.end(), Random::generator);
		shuffle(p2genes.begin(), p2genes.end(), Random::generator);

		GenomePtr p1 = make_shared<PermutationGenome>(p1genes);
		GenomePtr p2 = make_shared<PermutationGenome>(p2genes);

		vector<GenomePtr> parents;
		parents.push_back(p1), parents.push_back(p2);

		//
		RecombinatorPtr recomb = make_shared_base(Recombinator, T);
		PermutationGenomePtr child = dynamic_pointer_cast<PermutationGenome>(
				recomb->Combine(parents));

		return child->IsValid();
	}
};

BOOST_AUTO_TEST_SUITE(PermutationRecombinationTest)

BOOST_FIXTURE_TEST_CASE(CycleCrossoverTest, PermRecombinationFixture<CycleCrossover>) {
	BOOST_CHECK_THROW(Run(1, 1), EAException);
	BOOST_CHECK_THROW(Run(2, 3), EAException);
	BOOST_CHECK_EQUAL(Run(2, 2), true);
	BOOST_CHECK_EQUAL(Run(1231, 1231), true);
}
BOOST_FIXTURE_TEST_CASE(EdgeCrossoverTest, PermRecombinationFixture<EdgeCrossover>) {
	BOOST_CHECK_THROW(Run(1, 1), EAException);
	BOOST_CHECK_THROW(Run(2, 3), EAException);
	BOOST_CHECK_EQUAL(Run(2, 2), true);
	BOOST_CHECK_EQUAL(Run(897, 897), true);
}
BOOST_FIXTURE_TEST_CASE(OrderCrossoverTest, PermRecombinationFixture<OrderCrossover>) {
	BOOST_CHECK_THROW(Run(0, 0), EAException);
	BOOST_CHECK_THROW(Run(2, 3), EAException);
	BOOST_CHECK_EQUAL(Run(2, 2), true);
	BOOST_CHECK_EQUAL(Run(977, 977), true);
}
BOOST_FIXTURE_TEST_CASE(PartiallyMappedCrossoverTest, PermRecombinationFixture<PartiallyMappedCrossover>) {
	BOOST_CHECK_THROW(Run(1, 1), EAException);
	BOOST_CHECK_THROW(Run(2, 3), EAException);
	BOOST_CHECK_EQUAL(Run(2, 2), true);
	BOOST_CHECK_EQUAL(Run(1197, 1197), true);
}

BOOST_AUTO_TEST_SUITE_END()

}}

