/*
 * PermInitializerTest.cpp
 *
 *  Created on: Apr 12, 2017
 *      Author: Kiet Vo
 */

#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include "../../EA.h"

namespace ea {
namespace test {

struct PermInitializerFixture {
	PermInitializerFixture() {
	}
	~PermInitializerFixture() {
	}

	bool Run(uint length, uint size) {
		InitializerPtr init = make_shared_base(Initializer,
				PermutationInitializer, length);
		VectorPtr<GenomePtr> genomes = init->Initialize(size);

		for (uint i = 0; i < genomes->size(); i++) {
			PermutationGenomePtr perGenome = dynamic_pointer_cast<
					PermutationGenome>((*genomes)[i]);

			if (!perGenome->IsValid())
				return false;
		}
		return true;
	}
};

BOOST_AUTO_TEST_SUITE(PermutationInitializerTest)

BOOST_FIXTURE_TEST_CASE(PermInitTest, PermInitializerFixture) {
	BOOST_CHECK_EQUAL(Run(1, 1), true);
	BOOST_CHECK_EQUAL(Run(2, 3), true);
	BOOST_CHECK_EQUAL(Run(281, 1), true);
	BOOST_CHECK_EQUAL(Run(513, 3), true);
}

BOOST_AUTO_TEST_SUITE_END()

}

}

