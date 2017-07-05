/*
 * CMAStateOutputHook.cpp
 *
 *  Created on: Jun 8, 2017
 *      Author: Bui Quang Minh
 */

#include "CMAStateOutputHook.h"

#include "../../pch.h"
#include "../../EA/Type/Strategy.h"
#include "CMAStatePool.h"
#include "../../core/Population.h"
#include "../../core/Organism.h"
#include "../../core/interface/Genome.h"

namespace ea {

using namespace std;

EA_TYPEINFO_CUSTOM_IMPL(CMAStateOutputHook) {
	return *ea::TypeInfo("CMAStateOutputHook")
		.Add("print-genome", &CMAStateOutputHook::mPrintGenome)
		->SetConstructor<CMAStateOutputHook>();
}

/**
 * @class CMAStateOutputHook
 * A Hook which prints out the state of CMAEvolutionStrategy algorithm.
 * This Hook will print out the step size and the path cummulative length of the algorithm.
 *
 * @name{CMAStateOutputHook}
 */

CMAStateOutputHook::CMAStateOutputHook(bool pPrintGenome) :
		mPrintGenome(pPrintGenome) {
}

CMAStateOutputHook::~CMAStateOutputHook() {
}

void CMAStateOutputHook::DoGenerational() {
	CMAStatePoolPtr cmapool = GetPopulation()->GetPool(2)->To<CMAStatePool>();

	LogStream info(Log::INFO);
	info.imbue(std::locale("en_US.UTF-8"));
	info << "Gen " << setw(7) << GetGeneration() << setprecision(5)
			<< "\tBest = " << setw(10) <<  GetBestOrganism()->GetFitnessValue() << setprecision(3)
			<< "\tMax= " << setw(8) <<  cmapool->D(cmapool->D.size()-1) * cmapool->sigma
			<< "\tMin= " << setw(8) <<  cmapool->D(0) * cmapool->sigma
			//<< "\tAvg= " << setw(8) << pow(cmapool->D.prod(), 1.0/N) * cmapool->sigma
			<< "\tRatio= " << setw(6) << cmapool->D(cmapool->D.size()-1) / cmapool->D(0);

	if (mPrintGenome)
		info << "\t" << *GetBestOrganism()->GetGenome();

	info << flush;
}

}
/* namespace ea */
