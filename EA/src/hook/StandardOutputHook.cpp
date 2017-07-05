/*
 * StandardOutputHook.cpp
 *
 *  Created on: Apr 3, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "StandardOutputHook.h"
#include "../core/Organism.h"
#include "../core/interface/Genome.h"

namespace ea {

using namespace std;

EA_TYPEINFO_CUSTOM_IMPL(StandardOutputHook) {
	return *ea::TypeInfo("StandardOutputHook")
		.Add("print-genome", &StandardOutputHook::mPrintGenome)
		->SetConstructor<StandardOutputHook>();
}

/**
 * @class StandardOutputHook
 * A Hook which prints out best organism every generation.
 * This Hook will print out the best fitness value and its corresponding Genome using Genome::Print().
 *
 * @name{StandardOutputHook}
 *
 * @eaml
 * @attr{print-genome, bool - Optional - Whether the Genome will be printed out or not (default is true).}
 * @endeaml
 */

/**
 * Create a StandardOutputHook.
 * Users have the option to let the Genome to be printed out or not.
 * The best fitness value will always be printed out.
 * @param pPrintGenome Whether the Genome will be printed out or not.
 */
StandardOutputHook::StandardOutputHook(bool pPrintGenome) :
		mPrintGenome(pPrintGenome) {
}

StandardOutputHook::~StandardOutputHook() {
}

void StandardOutputHook::DoGenerational() {
	auto organism = GetBestOrganism();
	LogStream info(Log::INFO);
	info.imbue(std::locale("en_US.UTF-8"));
	if (mPrintGenome) {
		info <<"Gen " << setw(7) << GetGeneration() << "\tBest = " << setw(10) << organism->GetFitnessValue()
		<< "\t" << *organism->GetGenome() << flush;
	} else {
		info <<"Gen " << setw(7) << GetGeneration() << "\tBest = " << setw(10) << organism->GetFitnessValue() << flush;
	}
}

}
/* namespace ea */
