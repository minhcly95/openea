/*
 * Restore.cpp
 *
 *  Created on: Apr 7, 2017
 *      Author: Bui Quang Minh
 */

#include <pch.h>
#include "CLIOptions.h"
#include "CommandLineInterface.h"
#include <climits>
#include <boost/filesystem.hpp>

namespace ea {
namespace cli {

using namespace std;

function<void(void)> Restore(istringstream& iss) {
	namespace filesystem = boost::filesystem;

	string token;
	ullong gen = UINT64_MAX;

	if (iss.peek() == EOF)
#ifndef EA_CLI_RELEASE
		token = (CommandLineInterface::QueryExecDir() / DEFAULT_BACKUP_FOLDER).c_str();
#else
		token = (CommandLineInterface::QueryHomeDir() / ".openea" / DEFAULT_BACKUP_FOLDER).c_str();
#endif

	else {
		if (!getline(iss, token, '='))
			throw "Unexpected error when parsing";

		if (!iss.eof()) {
			if (token != "") {
				try {
					gen = stoll(token);
				} catch (exception& e) {
					throw "Cannot translate generation number";
				}
			}
			if (!getline(iss, token))
				throw "Unexpected error when parsing";
			if (gen != UINT64_MAX && !filesystem::is_directory(filesystem::path(token)))
				throw "\"" + token + "\" is not a directory";
		}
	}

	return [token, gen] () {
		CommandLineInterface::Register([token, gen] (StrategyPtr& strategy) {
			CommandLineInterface::sPopulation = Restore::FromBackup(token, gen);
			filesystem::path p(token);
			if (filesystem::is_directory(p) || !filesystem::exists(p))
				CommandLineInterface::sDefaultBackupFolder = p.string();
			else
				CommandLineInterface::sDefaultBackupFolder = p.parent_path().string();
		});
	};
}

}
}

