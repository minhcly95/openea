/*
 * PopulationFixture.h
 *
 *  Created on: Apr 10, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../Type.h"
#include "TestInterfaces.h"
#include "InterfaceFixture.h"

namespace ea {

namespace test {

using namespace std;

struct PopulationFixture: public InterfaceFixture {
	PopulationPtr population;

	PopulationFixture() {
		population = make_shared<Population>(10);
		population->SetInitializer(initializer);
		population->SetEvaluator(evaluator);

		population->AddMutator(mutator, RandomRate());
		population->AddRecombinator(recombinator, selector, RandomRatio());
		population->SetSurvivalSelector(selector);
		population->AddHook(hook);
	}
	~PopulationFixture() {
	}
};

}// namespace test

}// namespace ea


