/*
 * BackupHook.cpp
 *
 *  Created on: Apr 5, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "BackupHook.h"
#include "../core/Organism.h"
#include "../core/pool/OrganismPool.h"
#include "../rtoc/BinarySerializer.h"
#include "../core/Population.h"

namespace ea {

using namespace std;
using boost::format;

/**
 * @class BackupHook
 * A Hook which backs up the Population to file.
 * This class provides back-up functionality for the Population.
 * Using this Hook, it is guaranteed that a back-up file will be saved when the evolution process has ended.
 * Users can also configure the Hook so it will create back-up files after some number of generations.
 *
 * Back-up files will be saved in a provided folder. The name of the files will be \<generation_number\>.eabak.
 * Old back-up file with the same generation number will be overwritten.
 *
 * @name{BackupHook}
 *
 * @eaml
 * @attr{dir, string - Required - The target directory to save back-up files.}
 * @attr{frequency, uint - Optional - The interval of generations between two back-up files.}
 * @attr{clear, bool - Optional - If true\, all files in the given directory
 * will be cleared before writing any back-up file (default is false).}
 * @endeaml
 */

EA_TYPEINFO_CUSTOM_IMPL(BackupHook)  {
	return *ea::TypeInfo("BackupHook")
		.Add("frequency", &BackupHook::mFrequency)
		->Add("clear", &BackupHook::mClear)
		->SetConstructor<BackupHook, string>("dir");
}

/**
 * Create a BackupHook to save back-up to the given directory.
 * @param pDir The target directory to save back-up files.
 * @param pFrequency The interval of generations between two back-up files.
 * (If it is 0, the Hook only saves back-up at the end of evolution).
 * @param pClear If true, all files in pDir will be cleared before writing back-up files (be cautious).
 *
 */
BackupHook::BackupHook(string pDir, uint pFrequency, bool pClear) :
		mDir(pDir), mFrequency(pFrequency), mClear(pClear) {
}

BackupHook::~BackupHook() {
}

void BackupHook::DoInitial() {
	using namespace boost::filesystem;

	path p(mDir);
	if (exists(p)) {
		if (is_directory(p)) {
			if (mClear) {
				long long generation = GetGeneration();
				uint fileCleared = 0;
				for (directory_entry& entry : directory_iterator(p)) {
					path entryPath = entry.path();
					if (is_regular_file(entryPath)
							&& ExtractGenerationNumber(entryPath.string())
									> generation) {
						remove(entryPath);
						fileCleared++;
					}
				}
				EA_LOG_DEBUG<< "BackupHook::DoInitial: Cleared " << fileCleared << " file(s) in \"" << mDir << "\"." << flush;
			}
		} else
		throw EA_EXCEPTION(EAException, EXISTED_FILE_IS_NOT_DIRECTORY,
				"BackupHook::DoInitial: \"" + mDir + "\" exists but is not a directory.");
	} else {
		if (create_directories(p))
		EA_LOG_DEBUG << "BackupHook::DoInitial: Directory \"" + mDir + "\" created." << flush;
		else
		throw EA_EXCEPTION(EAException, CANNOT_CREATE_FILE,
				"BackupHook::DoInitial: Cannot create directory \"" + mDir + "\".");
	}
}

void BackupHook::DoGenerational() {
	ullong generation = GetGeneration();
	if (mFrequency != 0 && generation % mFrequency == 0 && generation != 0)
		CreateBackup();
}

void BackupHook::DoEnd() {
	CreateBackup();
}

long long BackupHook::ExtractGenerationNumber(string pFilename) {
	using namespace boost::filesystem;

	path p(pFilename);
	if (!p.has_extension() || !p.has_stem() || p.extension() != ".eabak")
		return -1;

	try {
		return stoll(p.stem().string());
	} catch (exception& e) {
		return -1;
	}
}

void BackupHook::CreateBackup() {
	ullong generation = GetGeneration();
	boost::filesystem::path file(mDir);
	file /= (format("%06llu.eabak") % generation).str();

	ofstream ofs(file.string(), ios::binary);
	BinarySerializer<PopulationPtr>::Write(ofs, GetPopulation());
	EA_LOG_DEBUG<<"BackupHook: Backup saved to \"" << file.string() << "\"." << flush;
}

/**
 * Whether the target directory will be cleared before writing back-up files.
 * @return true if the target directory will be cleared.
 */
bool BackupHook::IsClear() const {
	return mClear;
}

/**
 * Get the target directory.
 * @return The target directory.
 */
const string& BackupHook::GetDirectory() const {
	return mDir;
}

/**
 * Get the interval of generations between back-up files.
 * @return The interval of generations.
 */
uint BackupHook::GetInterval() const {
	return mFrequency;
}

}/* namespace ea */
