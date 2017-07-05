#include <openea/EA.h>

using namespace ea;
using namespace std;

#define GENOME_LENGTH				50
#define OFFSPRING_RATIO				1
#define INDIVIDUAL_MUTATION_RATE	0.1
#define POP_SIZE					100

int main() {

	auto strategy = make_shared<EvolutionStrategy>(POP_SIZE);

	strategy->initializer.Create<BoolRandomArrayInitializer>(GENOME_LENGTH);

	strategy->mutators.CreateBase<FlipBitMutation>(1.0 / GENOME_LENGTH)->Rate(INDIVIDUAL_MUTATION_RATE);

	strategy->recombinators.CreateBase<BoolOnePointCrossover>()->Parent<UniformSelection>()->Ratio(OFFSPRING_RATIO);

	strategy->survivalSelector.Create<GreedySelection>();

	strategy->evaluator.Create<TypedFunctionalEvaluator<BoolArrayGenome>>( [] (const BoolArrayGenomePtr& genome) {
		vector<bool>& genes = genome->GetGenes();
		return (double)std::count(genes.begin(), genes.end(), true);
	});

	strategy->hooks.Create<StandardOutputHook>();
	strategy->hooks.Create<FitnessTerminationHook>(FitnessTerminationHook::EQUAL_TO, GENOME_LENGTH, false);

	SessionPtr session = strategy->Evolve();

	return 0;
}
