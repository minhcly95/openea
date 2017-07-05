/*
 * FitnessReportHook.cpp
 *
 *  Created on: Jun 13, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "FitnessReportHook.h"
#include "../core/Organism.h"
#include "../core/pool/OrganismPool.h"

namespace ea {

/**
 * @class FitnessReportHook
 * A Hook which reports the fitness values of the main pool to file.
 * This class is only compatible with Evaluator which outputs ScalarFitness.
 * Using this Hook, a CSV file will be created and stores all the fitness values existed in the main pool (OrganismPool #0).
 * Each row of the CSV file represents a generation and each column represents a fitness value (except for the first column which is
 * the generation number). There is also an option to configure the frequency at which the report will be generated.
 *
 * @name{FitnessReportHook}
 *
 * @eaml
 * @attr{file-name, string - Required - The target file to be written to.}
 * @attr{frequency, uint - Optional - The interval of generations between two rows in the file.}
 * @attr{override, bool - Optional - If true\, existed file will be overridden.
 * Otherwise\, an error will be thrown if the file has been existed.}
 * @endeaml
 */

EA_TYPEINFO_CUSTOM_IMPL(FitnessReportHook) {
	return *ea::TypeInfo("FitnessReportHook")
		.Add("override", &FitnessReportHook::mOverride)
		->Add("frequency", &FitnessReportHook::mFrequency)
		->SetConstructor<FitnessReportHook, string>("file-name");
}

/**
 * Create a FitnessReportHook to save fitness reports to the target file.
 * @param pFileName The target file to be written to.
 * @param pFrequency The interval of generations between two rows.
 * (0 is same as 1, which will save the report every generation).
 * @param pOverride  If true, existed file will be overridden.
 * Otherwise, an error will be thrown if the file has been existed.
 *
 */
FitnessReportHook::FitnessReportHook(string pFileName, uint pFrequency, bool pOverride)
		: mFileName(pFileName), mFrequency(pFrequency), mOverride(pOverride), fs() {
}

FitnessReportHook::~FitnessReportHook() {
}

void FitnessReportHook::DoInitial() {
	using namespace boost::filesystem;

	path p(mFileName);
	if (exists(p) && !mOverride) {
		throw EA_EXCEPTION(EAException, FILE_ALREADY_EXISTS,
				"\"" + mFileName + "\" already exists.");
	} else {
		if (exists(p))
			EA_LOG_DEBUG << "FitnessReportHook::DoInitial: File \"" + mFileName + "\" overrode." << flush;
		else
			EA_LOG_DEBUG << "FitnessReportHook::DoInitial: File \"" + mFileName + "\" created." << flush;
		fs = std::fstream(mFileName, ios_base::out | ios_base::trunc);
	}
}

void FitnessReportHook::DoGenerational() {
	ullong generation = GetGeneration();
	if ((mFrequency != 0 && generation % mFrequency != 0) || generation == 0)
		return;

	CreateEntry();
}

void FitnessReportHook::DoEnd() {
	ullong generation = GetGeneration();
	if (mFrequency != 0 && generation % mFrequency != 0)
		CreateEntry();

	if (fs.is_open())
		fs.close();
}

void FitnessReportHook::CreateEntry() {
	OrganismPoolPtr pool = GetMainPool();
	fs << setprecision(17);
	fs << GetGeneration() << ",";
	for (OrganismPtr org : *pool)
		fs << org->GetFitnessValue() << ",";
	fs.seekp(-1, ios_base::cur);
	fs << endl;
}

} /* namespace ea */
