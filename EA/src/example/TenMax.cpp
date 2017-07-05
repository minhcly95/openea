/*
 * TenMax.cpp
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include <iostream>
#include <unistd.h>
#include "../EA.h"

using namespace ea;
using namespace std;

class TenMaxEvaluator: public TypedScalarEvaluator<IntArrayGenome> {
public:
	inline TenMaxEvaluator() {
	}

	inline ~TenMaxEvaluator() {
	}

private:
	virtual double DoScalarEvaluate(InputType pGenome) override {
		vector<int>& genes = pGenome->GetGenes();
		int intCount = 0;
		for (uint i = 0; i < genes.size(); i++)
			intCount += (*genes)[i];

		return intCount;
	}
};

#define POP_SIZE 100
#define GENOME_LENGTH 10
#define MAX_INT 10

int main(int argc, char** argv) {
	EvolutionStrategyPtr strategy = make_shared<EvolutionStrategy>(POP_SIZE);

	IntRandomizerPtr randomizer = make_shared<IntRandomizer>(0, MAX_INT);
	strategy->initializer.Create<IntRandomArrayInitializer>(GENOME_LENGTH, randomizer);

	strategy->evaluator.Create<TenMaxEvaluator>();

	strategy->recombinators.CreateBase<IntUniformCrossover>()->Parent<UniformSelection>()->Ratio(1);

	strategy->mutators.CreateBase<IntPointResetMutation>(0.1, randomizer)->Rate(0.1);

	strategy->survivalSelector.Create<GreedySelection>();

	strategy->hooks.Create<StandardOutputHook>();
	strategy->hooks.Create<FitnessTerminationHook>(
			FitnessTerminationHook::EQUAL_TO, GENOME_LENGTH, false);

	SessionPtr session = strategy->Evolve();

	return 0;
}

