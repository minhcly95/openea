/*
 * RestoreInitializer.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "Restore.h"

#include "../pch.h"
#include "../hook/BackupHook.h"
#include "../rtoc/BinarySerializer.h"
#include "../core/Population.h"

namespace ea {

/**
 * @class Restore
 * A static class which restores the Population from a back-up file.
 * This class is used to restore a Population has been backed up by BackupHook.
 * The generation and evaluation number is also restored from the back-up file.
 * The restored Population includes the fitness evaluated in the previous run.
 */

/**
 * Restore which restores from a back-up directory.
 * The source location is the same with the target directory of BackupHook.
 * The Restore class will look for the file with greatest generation number.
 * Users can also set the location directly to a file, or set the generation number to be restored from.
 * If a generation number is provided but doesn't exist in the directory,
 * the Restore class will look for the file with the closest smaller generation number.
 *
 * @param pLocation The path of the back-up directory or the back-up file.
 * @param pFrom The generation number to be restored from (default is maximum).
 */
PopulationPtr Restore::FromBackup(string pLocation, ullong pFrom) {
	using namespace boost::filesystem;

	path p(pLocation);
	if (!exists(p))
		throw EA_EXCEPTION(EAException, FILE_DOES_NOT_EXIST,
				"Restore::FromBackup: \"" + pLocation
						+ "\" does not exist.");

	if (is_directory(p)) {
		long long maxGen = -1;
		for (directory_entry& entry : directory_iterator(p)) {
			path entryPath = entry.path();
			if (is_regular_file(entryPath)) {
				long long gen = BackupHook::ExtractGenerationNumber(
						entryPath.string());
				if (gen > maxGen && ullong(gen) <= pFrom) {
					p = entryPath;
					maxGen = gen;
				}
			}
		}
		if (maxGen != -1 && pFrom != UINT64_MAX && ullong(maxGen) != pFrom)
			EA_LOG_DEBUG << "Restore::FromBackup: Backup of gen " << pFrom
					<< " does not exist. Restoring from gen " << maxGen << "..."
					<< flush;
	}

	if (!is_regular_file(p))
		throw EA_EXCEPTION(EAException, EXISTED_FILE_IS_NOT_REGULAR_FILE,
				"Restore::FromBackup: \"" + pLocation
						+ "\" is not a regular file or does not contain any backup file.");

	std::ifstream ifs(p.string(), ios::binary);
	PopulationPtr population = BinarySerializer<PopulationPtr>::Read(ifs);

	EA_LOG_DEBUG << "Restore::FromBackup: Population restored from \"" << p.string()
			<< "\", gen " << population->GetGeneration() << ", evl " << population->GetEvaluation() << flush;

	return population;
}

} /* namespace ea */
