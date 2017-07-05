/*
 * CommandLineInterface.h
 *
 *  Created on: Mar 24, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "Common.h"
#include <boost/filesystem.hpp>

namespace ea {
namespace cli {

namespace test {
struct CLIFixture;
}

namespace fs = boost::filesystem;

class CommandLineInterface {
public:
	static int Process(int argc, char** argv);

	static int ProcessOptions(int argc, char** argv);
	static int Run();

	static int Execute();

	static void Register(function<void(StrategyPtr&)> func);
	static void AddVariable(string input);

	static void Repeat(uint pTimes, string pRoot);

	static fs::path QueryExecDir();
	static fs::path QueryHomeDir();

	static string sDefaultBackupFolder;
	static PopulationPtr sPopulation;

private:
	static vector<function<void(StrategyPtr&)>> sModificationList;
	static SessionPtr sSession;
	static EAMLReader::VariableMap sVariableMap;
	static string sFileName;

	static uint sRepeatTimes;
	static string sRepeatRoot;

	static void AddDefaultBackup(const StrategyPtr& pStrategy);
	static void AddDefaultServer(const StrategyPtr& pStrategy);

	static void AddClusterOperators(const EAMLReader& pReader);

	static void CreateFolderIfNotExist(string dirName);

	friend struct test::CLIFixture;
};

enum CLIReturnCode {
	CLI_SUCCESS,
	CLI_WRONG_ARGUMENTS,
	CLI_FILE_DOES_NOT_EXIST,
	CLI_WRONG_OPTIONS,
	CLI_HELP,
	CLI_OTHERS = -1
};

} /* namespace cli */
} /* namespace ea */


