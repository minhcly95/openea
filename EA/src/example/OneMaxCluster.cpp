/*
 * OneMax.cpp
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../EA.h"

using namespace ea;
using namespace std;

#define POP_SIZE 100
#define GENOME_LENGTH 100

int main(int argc, char** argv) {
	//Log::Redirect(Log::TRACE, "stdout");

	EvolutionStrategyPtr strategy = make_shared<EvolutionStrategy>(POP_SIZE);

	strategy->initializer.Create<BoolRandomArrayInitializer>(GENOME_LENGTH);

	auto evaluator = strategy->evaluator.Create<TypedFunctionalEvaluator<BoolArrayGenome>>(
			[] (const BoolArrayGenomePtr& genome) {
				vector<bool>& genes = genome->GetGenes();
				return (double)std::count(genes.begin(), genes.end(), true);
			});

	strategy->recombinators.CreateBase<BoolOnePointCrossover>()->Parent<UniformSelection>()->Ratio(1);

	strategy->mutators.CreateBase<FlipBitMutation>(0.01)->Rate(0.1);

	strategy->survivalSelector.Create<GreedySelection>();

	strategy->hooks.Create<StandardOutputHook>();
	strategy->hooks.Create<FitnessTerminationHook>(
			FitnessTerminationHook::EQUAL_TO, GENOME_LENGTH, false);

	Cluster::AddOperator(evaluator);

	SessionPtr session = strategy->Evolve();

	Cluster::Destroy();

	return 0;
}

