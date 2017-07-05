/*
 * InterfaceFixture.h
 *
 *  Created on: Mar 21, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../Type.h"
#include <ctime>
#include "TestInterfaces.h"

#define MAX_COUNT 10
#define MAX_RATIO 5

namespace ea {

namespace test {

using namespace std;

struct InterfaceFixture {
	MutatorPtr mutator;
	RecombinatorPtr recombinator;
	InitializerPtr initializer;
	SelectorPtr selector;
	EvaluatorPtr evaluator;
	GenomePtr genome;
	HookPtr hook;
	uniform_int_distribution<uint> countDist;
	uniform_real_distribution<double> rateDist;
	uniform_real_distribution<double> ratioDist;

	InterfaceFixture() :
			countDist(1, MAX_COUNT), rateDist(0.0, 1.0), ratioDist(
					0.0, MAX_RATIO) {
		mutator = make_shared_base(Mutator, TestMutator);
		recombinator = make_shared_base(Recombinator, TestRecombinator, RandomCount());
		initializer = make_shared_base(Initializer, TestInitializer);
		selector = make_shared_base(Selector, TestSelector);
		evaluator = make_shared_base(IndividualEvaluator, TestEvaluator);
		genome = make_shared_base(Genome, TestGenome);
		hook = make_shared_base(Hook, TestHook);
	}
	~InterfaceFixture() {
	}

	uint RandomCount() {
		return countDist(Random::generator);
	}
	double RandomRate() {
		return rateDist(Random::generator);
	}
	double RandomRatio() {
		return ratioDist(Random::generator);
	}
};

}	// namespace test

}	// namespace ea


