#include "OneMaxEvaluator.h"

using namespace ea;
using namespace std;

double ea::addon::OneMaxEvaluator::DoScalarEvaluate(const BoolArrayGenomePtr& pGenome) {
	vector<bool>& genes = pGenome->GetGenes();
	return (double)std::count(genes.begin(), genes.end(), true);
}
