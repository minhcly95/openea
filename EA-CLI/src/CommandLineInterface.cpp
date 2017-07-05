/*
 * CommandLineInterface.cpp
 *
 *  Created on: Mar 24, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include <pch.h>
#include "CommandLineInterface.h"
#include "CLIOptions.h"
#include <csignal>
#include <boost/filesystem.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

using namespace std;

namespace ea {
namespace cli {

bool CheckFileExist(string fileName);
function<void(void)> ProcessOption(const char* option);

vector<function<void(StrategyPtr&)>> CommandLineInterface::sModificationList;
SessionPtr CommandLineInterface::sSession;
PopulationPtr CommandLineInterface::sPopulation;
string CommandLineInterface::sDefaultBackupFolder;
string CommandLineInterface::sFileName;
EAMLReader::VariableMap CommandLineInterface::sVariableMap;

uint CommandLineInterface::sRepeatTimes = 0;
string CommandLineInterface::sRepeatRoot = "";

int CommandLineInterface::Process(int argc, char** argv) {
	int code = ProcessOptions(argc, argv);
	if (code)
		return code;
	else
		return Run();
}

int CommandLineInterface::ProcessOptions(int argc, char** argv) {
	try {
		if (argc < 2) {
			PrintHelp();
			return CLI_WRONG_ARGUMENTS;
		}
		if (argc == 2 && (argv[1] == string("-h") || argv[1] == string("--help"))) {
			PrintHelp();
			return CLI_HELP;
		}

		// Check for file existance
		sFileName = argv[1];
		if (!CheckFileExist(sFileName)) {
			cerr << "CLI Error: " << sFileName << " does not exist." << endl;
			return CLI_FILE_DOES_NOT_EXIST;
		}

#define PRINT_MSG\
		cerr << "CLI Error: Invalid option " << argv[i];\
		if (msg != string())\
			cerr << " : " << msg;\
		cerr << "." << endl;\
		return CLI_WRONG_OPTIONS;

		// Get list of options procedures
		vector<function<void(void)>> procedures;
		for (int i = 2; i < argc; i++)
			try {
				if (auto func = ProcessOption(argv[i]))
					procedures.push_back(func);
			} catch (const char* msg) {
				PRINT_MSG
			} catch (string& msg) {
				PRINT_MSG
			}

		// Execute procedures
		for (auto f : procedures)
			f();

		return CLI_SUCCESS;

	} catch (exception& e) {
		EA_LOG_ERROR<< "Fatal Error: " << e.what() << flush;
		return CLI_OTHERS;
	}
}

void CommandLineInterface::CreateFolderIfNotExist(string dirName) {
	using namespace boost::filesystem;
	path p(dirName);

	if (exists(p)) {
		if (!is_directory(p))
			throw EA_EXCEPTION(EAException, EXISTED_FILE_IS_NOT_DIRECTORY,
					"CLI Repeat: \"" + dirName + "\" exists but is not a directory.");
	}
	else {
		if (create_directories(p))
			EA_LOG_DEBUG << "CLI Repeat: Directory \"" + dirName + "\" created" << flush;
		else
			throw EA_EXCEPTION(EAException, CANNOT_CREATE_FILE,
					"CLI Repeat: Cannot create directory \"" + dirName + "\"");
	}
}

int CommandLineInterface::Run() {
	using namespace boost::filesystem;

	// Start reading
	EA_LOG_DEBUG<< "CLI: Reading " << sFileName << flush;
	path p(sFileName);
	sFileName = absolute(p).string();

	if (sRepeatTimes <= 1)
		return Execute();

	EA_LOG_DEBUG << "CLI: Repeat " << sRepeatTimes << " times at folder \"" << sRepeatRoot << "\"" << flush;

	// Create root
	CreateFolderIfNotExist(sRepeatRoot);
	p = path(sRepeatRoot);
	current_path(p);

	// Loop
	for (uint i = 0; i < sRepeatTimes; i++) {
		ostringstream oss;
		oss << setw(3) << setfill('0') << i;
		CreateFolderIfNotExist(oss.str());

		current_path(oss.str());
		sDefaultBackupFolder = ".backup";

		EA_LOG_DEBUG << "CLI Repeat: Start run #" << i << " at folder " << current_path() << "" << flush;
		int code = Execute();
		EA_LOG_ERROR << "CLI Repeat: Run #" << i  << " finished with exit code " << code << flush;

		current_path("..");
	}

	return 0;
}

int CommandLineInterface::Execute() {
	try {
		// Build
		EAMLReader reader(sFileName.c_str());
		StrategyPtr strategy = reader.Build(sVariableMap);
		EA_LOG_DEBUG<< "CLI: \"" << strategy->GetTypeName() << "\" reconstructed" << flush;

		// Modifications
		for (auto f : sModificationList)
			f(strategy);

		// Implicit modification
		AddDefaultBackup(strategy);
		//AddDefaultServer(strategy);

		if (Cluster::IsEnabled())
			AddClusterOperators(reader);

		// Run
		strategy->Evolve(sSession, sPopulation);

		// Shut down
		if (Cluster::IsEnabled())
			Cluster::Destroy();

	} catch (exception& e) {
		EA_LOG_ERROR<< "Fatal Error: " << e.what() << flush;
		return CLI_OTHERS;
	}

	return CLI_SUCCESS;
}

void CommandLineInterface::Register(function<void(StrategyPtr&)> func) {
	sModificationList.push_back(func);
}

void CommandLineInterface::AddDefaultBackup(const StrategyPtr& pStrategy) {
	if (!pStrategy->hooks.Find<BackupHook>()) {
		bool clear = sDefaultBackupFolder == "";
		if (clear)
#ifndef EA_CLI_RELEASE
			sDefaultBackupFolder = (QueryExecDir() / DEFAULT_BACKUP_FOLDER).c_str();
#else
			sDefaultBackupFolder = (QueryHomeDir() / ".openea" / DEFAULT_BACKUP_FOLDER).c_str();
#endif
		pStrategy->hooks.Create<BackupHook>(sDefaultBackupFolder, 0, clear);
		EA_LOG_DEBUG<< "CLI: BackupHook not found. Default one added to folder \"" << sDefaultBackupFolder << "\"." << flush;
	}
}

void CommandLineInterface::AddDefaultServer(const StrategyPtr& pStrategy) {
	if (!pStrategy->hooks.Find<RealTimeInfoHook>()) {
		auto hook = pStrategy->hooks.Create<RealTimeInfoHook>();
		EA_LOG_DEBUG<< "CLI: RealTimeInfoHook not found. Default one added to port " << hook->GetPort() << "." << flush;
	}
}

void CommandLineInterface::AddClusterOperators(const EAMLReader& pReader) {
	vector<ConstructiblePtr> objs = pReader.GetConstructedObjects();

	for (auto obj : objs) {
		auto casted = dynamic_pointer_cast<ClusterComputableBase>(obj);
		if (casted)
			Cluster::AddOperator(casted);
	}
}

void CommandLineInterface::AddVariable(string input) {
	vector<string> tokens;
	boost::split(tokens, input, boost::is_any_of("="));
	if (tokens.size() == 2 && tokens[0] != "") {
		auto entry = sVariableMap.find(tokens[0]);
		if (entry == sVariableMap.end())
			sVariableMap[tokens[0]] = tokens[1];
		else
			throw "Key \"" + tokens[0] + "\" already exists";
	} else
		throw "--key=value required";
}

void CommandLineInterface::Repeat(uint pTimes, string pRoot) {
	sRepeatTimes = pTimes;
	sRepeatRoot = pRoot;
}

fs::path CommandLineInterface::QueryExecDir() {
	char buf[1024];
	ssize_t size = readlink("/proc/self/exe", buf, sizeof(buf));
	if (size == 0 || size == sizeof(buf))
		throw system_error();
	fs::path execDir(string(buf, size));
	return execDir.parent_path();
}

fs::path CommandLineInterface::QueryHomeDir() {
	const char *homedir;

	if ((homedir = getenv("HOME")) == NULL) {
		homedir = getpwuid(getuid())->pw_dir;
	}

	return fs::path(homedir);
}

#define BOLD(STR) "\e[1m"#STR"\e[0m"

void PrintHelp() {
	cout
			<< "Usage: openea run <config file> <options>\n"
					"Options:\n"
					"\t-l<level>[=<path>]\tRedirect log <level>(s) to <path>\n"
					"\t  <level>       \te (Error), i (Info), d (Debug), t (Trace), a (All)\n"
					"\t          =<path>\t\"\", stdout, stderr, <file>. Not specified (no \"=\") means stdout (stderr for e level).\n"
					"\n"
					"\t-tg[=]<target>\t\tTerminate when reaching generation #<target>\n"
					"\t-te[=]<target>\t\tTerminate when reaching evaluation #<target>\n"
					"\t\t" BOLD(Note) ": -t will overwrite any termination hook existed in <config file>\n"
			"\n"
			"\t-b[<num>=]<dir>\t\tBack-up to <dir> every <num> generations (default is <num>=0)\n"
			"\t\t" BOLD(Note) ": if not specified in <options> or <config file>, -b0=\".backup\" will be added by default\n"
			"\t-r[[<num>=]<dir>]\tRestore from <dir> from generation <num> (default is <num>=max, <dir>=\".backup\")\n"
			"\t\t" BOLD(Note) ": -r implies -b0=<dir> option on the same <dir> of -r unless otherwise specified\n\n"
			"\t--<key>=<value>\t\tSet variable named <key> in <config file> with <value>\n\n";
}

bool CheckFileExist(string fileName) {
	using namespace boost::filesystem;
	path p(fileName);
	return is_regular_file(p);
}

function<void(void)> ProcessOption(const char* option) {
	istringstream iss(option);
	char chr;

	GETCHAR(chr);
	if (chr != '-')
		THROW;

	GETCHAR(chr);
	switch (chr) {
	case 'l':
		return LogRedirect(iss);
	case 't':
		return TerminateHook(iss);
	case 'b':
		return Backup(iss);
	case 'r':
		return Restore(iss);
	case 'f':
		return FitnessReport(iss);
	case 'p':
		return Parallel(iss);
	case 's':
		return Server(iss);
	case 'n':
		return Repeat(iss);
	case 'c':
		return [] () {
			Cluster::SetEnabled(true);
		};
	case '-':
		CommandLineInterface::AddVariable(string(option + 2));
		return function<void(void)>();
	default:
		THROW;
	}
}

} /* namespace cli */
} /* namespace ea */
