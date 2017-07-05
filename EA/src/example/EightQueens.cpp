/*
 * EightQueens.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: Kiet Vo, Bui Quang Minh
 */

#include <iostream>
#include "../EA.h"

using namespace ea;
using namespace std;

class EightQueenEvaluator: public ScalarEvaluator {
public:
	EightQueenEvaluator() {
	}
	~EightQueenEvaluator() {
	}

	virtual inline bool IsMaximizer() override {
		return false;
	}

private:
	bool IsDiagonallyChecked(vector<uint>& genes, uint index1, uint index2) {
		int diff1 = index1 - index2;
		int diff2 = genes[index1] - genes[index2];
		return abs(diff1) == abs(diff2);
	}

	virtual double DoScalarEvaluate(const GenomePtr& pGenome) override {
		vector<uint>& genes = dynamic_pointer_cast<PermutationGenome>(pGenome)->GetGenes();

		double count = 0;
		for (uint i = 0; i < genes.size() - 1; i++)
			for (uint j = i + 1; j < genes.size(); j++)
				count += IsDiagonallyChecked(genes, i, j);

		return count;
	}
};

class EightQueenLinearEvaluator: public TypedScalarEvaluator<PermutationGenome> {
public:
	EightQueenLinearEvaluator() {
	}
	~EightQueenLinearEvaluator() {
	}

	virtual inline bool IsMaximizer() override {
		return false;
	}

private:
	virtual double DoScalarEvaluate(InputType pGenome) override {
		vector<uint>& genes = pGenome->GetGenes();
		uint size = genes.size();

		// Bucket method, inspired by bucket sort = linear complexity

		// Buckets
		vector<uint> mainDiagonal(size * 2 - 1, 0);		// Indexed by i + j
		vector<uint> offDiagonal(size * 2 - 1, 0);		// Indexed by (size - 1 - i) + j

		// Put to buckets
		for (uint i = 0; i < size; i++) {
			uint j = genes[i];
			mainDiagonal[i + j]++;
			offDiagonal[size - 1 - i + j]++;
		}

		// Count the buckets
		uint count = 0;
		for (uint b = 0; b < 2 * size - 1; b++) {
			count += mainDiagonal[b] * (mainDiagonal[b] - 1) / 2;
			count += offDiagonal[b] * (offDiagonal[b] - 1) / 2;
		}

		return count;
	}
};

#define POP_SIZE 100
#define GENOME_LENGTH 1000

int main(int argc, char** argv) {
	//Log::Redirect(Log::TRACE, "stdout");

	EvolutionStrategyPtr strategy = make_shared<EvolutionStrategy>(POP_SIZE);

	strategy->initializer.Create<PermutationInitializer>(GENOME_LENGTH);

	if (argc > 1)
		strategy->evaluator.Create<EightQueenEvaluator>();
	else
		strategy->evaluator.Create<EightQueenLinearEvaluator>();

	strategy->recombinators.CreateBase<OrderCrossover>()->Parent<TournamentSelection>(5)->Ratio(1);

	strategy->mutators.CreateBase<SwapMutation>()->Rate(0.8);

	strategy->survivalSelector.Create<GreedySelection>();

	strategy->hooks.Create<StandardOutputHook>(false);
	strategy->hooks.Create<FitnessTerminationHook>(
			FitnessTerminationHook::EQUAL_TO, 0, false);

	SessionPtr session = strategy->Evolve();

	return 0;
}
