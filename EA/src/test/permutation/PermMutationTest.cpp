/*
 * PermMutationTest.cpp
 *
 *  Created on: Apr 11, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../EA.h"

namespace ea {
namespace test {

template<class T>
struct PermMutationFixture {
	PermMutationFixture() {
	}

	~PermMutationFixture() {
	}

	bool Run(uint genomeLength) {
		vector<uint> p1genes(genomeLength);
		iota(p1genes.begin(), p1genes.end(), 0);
		shuffle(p1genes.begin(), p1genes.end(), Random::generator);

		GenomePtr p1 = make_shared<PermutationGenome>(p1genes);

		//
		MutatorPtr mutator = make_shared_base(Mutator, T);
		PermutationGenomePtr child = dynamic_pointer_cast<PermutationGenome>(mutator->Apply(p1));

		return child->IsValid();
	}
};

BOOST_AUTO_TEST_SUITE(PermutationMutationTest)

BOOST_FIXTURE_TEST_CASE(InsertMutationTest, PermMutationFixture<InsertMutation>) {
	BOOST_CHECK_THROW(Run(1), EAException);
	BOOST_CHECK_EQUAL(Run(2), true);
	BOOST_CHECK_EQUAL(Run(12345), true);
}
BOOST_FIXTURE_TEST_CASE(InversionMutationTest, PermMutationFixture<InversionMutation>) {
	BOOST_CHECK_THROW(Run(1), EAException);
	BOOST_CHECK_EQUAL(Run(2), true);
	BOOST_CHECK_EQUAL(Run(8971), true);
}
BOOST_FIXTURE_TEST_CASE(ScrambleMutationTest, PermMutationFixture<ScrambleMutation>) {
	BOOST_CHECK_THROW(Run(1), EAException);
	BOOST_CHECK_EQUAL(Run(2), true);
	BOOST_CHECK_EQUAL(Run(9977), true);
}
BOOST_FIXTURE_TEST_CASE(SwapMutationTest, PermMutationFixture<SwapMutation>) {
	BOOST_CHECK_THROW(Run(1), EAException);
	BOOST_CHECK_EQUAL(Run(2), true);
	BOOST_CHECK_EQUAL(Run(11987), true);
}

BOOST_AUTO_TEST_SUITE_END()

}}

