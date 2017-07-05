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

#define DIMENSION 20

int main(int argc, char** argv) {
	//Log::Redirect(Log::TRACE, "stdout");

	CMAEvolutionStrategyPtr strategy = make_shared<CMAEvolutionStrategy>(DIMENSION);

	strategy->evaluator.Create<TypedFunctionalEvaluator<DoubleArrayGenome>>(
			[] (const DoubleArrayGenomePtr& genome) {
				vector<double>& x = genome->GetGenes();
				double f = 0;
				for (uint i = 0; i < x.size() - 1; i++)
					f += 100 * pow(x[i+1] - x[i]*x[i], 2) + pow(1-x[i], 2);
				return f;
			}, false);

	strategy->hooks.Create<StandardOutputHook>();
	strategy->hooks.Create<EvaluationTerminationHook>(1000 * DIMENSION * DIMENSION);
	strategy->hooks.Create<FitnessTerminationHook>(FitnessTerminationHook::LESS_THAN, 1e-14);

	SessionPtr session = strategy->Evolve();

	return 0;
}

