#include <openea/EA.h>

using namespace ea;
using namespace std;

#ifndef DOXYGEN_IGNORE

//! [Normalization]
void Normalize(vector<double>& x) {
	double norm = 0;
	for (uint i = 0; i < x.size(); i++)
		norm += x[i] * x[i];
	norm = sqrt(norm);

	for (uint i = 0; i < x.size(); i++)
		x[i] /= norm;
}
//! [Normalization]

//! [Initialization]
class SphereInitializer: public Initializer {
private:
	uint mLength;

public:
	SphereInitializer(uint pLength) : mLength(pLength) { }

protected:
	virtual GenomePoolPtr DoInitialize(uint pSize) override {
		GenomePoolPtr pool = make_shared<GenomePool>();

		for (uint i = 0; i < pSize; i++) {
			DoubleArrayGenomePtr newGenome = make_shared<DoubleArrayGenome>();
			vector<double>& x = newGenome->GetGenes();

			for (uint j = 0; j < mLength; j++)
				x.push_back(Random::Rate());

			Normalize(x);

			pool->push_back(static_pointer_cast<Genome>(newGenome));
		}

		return pool;
	}
};
//! [Initialization]

//! [Mutation]
class SphereMutation: public TypedMutator<DoubleArrayGenome> {
protected:
	virtual DoubleArrayGenomePtr DoApply(const DoubleArrayGenomePtr& pTarget) override {
		DoubleArrayGenomePtr newGenome = pTarget->Clone();
		pair<uint, uint> pair = Random::Pair<uint>(0, pTarget->GetSize() - 1);

		vector<double>& x = newGenome->GetGenes();
		uint i = pair.first, j = pair.second;
		double p = Random::Rate();
		double q = sqrt(pow(x[i] / x[j], 2) * (1 - p * p) + 1);
		x[i] *= p;
		x[j] *= q;

		Normalize(x);

		return newGenome;
	}
};
//! [Mutation]

//! [Recombination]
class SphereCrossover: public TypedRecombinator<DoubleArrayGenome> {
protected:
	virtual uint GetParentCount() override {
		return 2;
	}
	virtual DoubleArrayGenomePtr DoCombine(vector<DoubleArrayGenomePtr>& pParents) override {
		DoubleArrayGenomePtr newGenome = make_shared<DoubleArrayGenome>();

		vector<double>& x = pParents[0]->GetGenes();
		vector<double>& y = pParents[1]->GetGenes();
		vector<double>& z = newGenome->GetGenes();

		double alpha = Random::Rate();
		for (uint i = 0; i < x.size(); i++)
			z.push_back(alpha * x[i] + (1 - alpha) * y[i]);

		Normalize(z);

		return newGenome;
	}
};
//! [Recombination]

//! [Main]
#define GENOME_LENGTH				25
#define OFFSPRING_RATIO				1
#define MUTATION_RATE				0.1
#define POP_SIZE					100
#define TOURNAMENT_SIZE				5
#define TARGET_GENERATION			10000

int main() {
	EvolutionStrategyPtr strategy = make_shared<EvolutionStrategy>(POP_SIZE);

	strategy->initializer.Create<SphereInitializer>(GENOME_LENGTH);

	strategy->mutators.CreateBase<SphereMutation>()->Rate(MUTATION_RATE);

	strategy->recombinators.CreateBase<SphereCrossover>()->Parent<UniformSelection>()->Ratio(OFFSPRING_RATIO);

	strategy->survivalSelector.Create<TournamentSelection>(TOURNAMENT_SIZE);

	strategy->evaluator.Create<TypedFunctionalEvaluator<DoubleArrayGenome>>( [] (const DoubleArrayGenomePtr& genome) {
		vector<double>& x = genome->GetGenes();
		uint n = x.size();
		return std::accumulate(x.begin(), x.end(), pow(n, n / 2.0), std::multiplies<double>());
	});

	strategy->hooks.Create<StandardOutputHook>();
	strategy->hooks.Create<GenerationTerminationHook>(TARGET_GENERATION);

	SessionPtr session = strategy->Evolve();

	return 0;
}
//! [Main]

#endif
