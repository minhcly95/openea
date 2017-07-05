/*
 * CLITest.cpp
 *
 *  Created on: Mar 27, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_MODULE EA-CLI-Test
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <fstream>

#include "../EA-CLI.h"
#include "../../add-on/sample/OneMax/SampleOneMaxEvaluator.h"
#include <boost/filesystem.hpp>

namespace ea {
namespace cli {
namespace test {

#define FORCE_REGISTER(CLASS)\
	try {\
		NameService::Add<CLASS>();\
	} catch (exception&) {\
	}

struct CLIFixture {
	char* argv[5];

	CLIFixture() {
		std::cout.setstate(std::ios_base::failbit);
		std::cerr.setstate(std::ios_base::failbit);
		Reset();

		using namespace ea::addon;
		FORCE_REGISTER(SampleOneMaxEvaluator);

		argv[0] = const_cast<char*>("EA-CLI");
		argv[1] = const_cast<char*>("src/test/resources/OneMax.eaml");
	}
	void Reset() {
		CommandLineInterface::sVariableMap.clear();
		CommandLineInterface::sModificationList.clear();
		CommandLineInterface::sDefaultBackupFolder = string();
		CommandLineInterface::sStrategy = nullptr;
	}
	PopulationPtr& GetPopulation() {
		return CommandLineInterface::sStrategy;
	}
	~CLIFixture() {
		std::cout.clear();
		std::cerr.clear();

		using namespace boost::filesystem;
		remove_all(".backup");
	}

	void SetOutputEnabled(bool enabled) {
		if (!enabled) {
			std::cout.setstate(std::ios_base::failbit);
			std::cerr.setstate(std::ios_base::failbit);
		} else {
			std::cout.clear();
			std::cerr.clear();
		}
	}
};

#define TEST_CLI(ARGV, VALUE, ARGC, CODE)\
	Reset();\
	argv[ARGV] = const_cast<char*>(VALUE);\
	BOOST_CHECK(CommandLineInterface::Process(ARGC, argv) == CODE)

BOOST_FIXTURE_TEST_CASE(ArgumentTest, CLIFixture) {
	BOOST_CHECK(
			CommandLineInterface::Process(1, nullptr) == CLI_WRONG_ARGUMENTS);

	BOOST_CHECK(CommandLineInterface::Process(2, argv) == CLI_SUCCESS);

	// Wrong file name
	TEST_CLI(1, "abcd/non-existed.eaml", 2, CLI_FILE_DOES_NOT_EXIST);

	// Help
	TEST_CLI(1, "-h", 2, CLI_HELP);
	TEST_CLI(1, "--help", 2, CLI_HELP);
}

BOOST_FIXTURE_TEST_CASE(RedirectOptionTest, CLIFixture) {
	// Valid options
	TEST_CLI(2, "-ld", 3, CLI_SUCCESS);
	TEST_CLI(2, "-le=", 3, CLI_SUCCESS);
	TEST_CLI(2, "-li=stdout", 3, CLI_SUCCESS);
	TEST_CLI(2, "-lt=stderr", 3, CLI_SUCCESS);
	TEST_CLI(2, "-la", 3, CLI_SUCCESS);
	TEST_CLI(2, "-lde", 3, CLI_SUCCESS);
	TEST_CLI(2, "-litd", 3, CLI_SUCCESS);

	// Invalid level
	TEST_CLI(2, "-lb", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-lidc", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-lietz", 3, CLI_WRONG_OPTIONS);

	// No level specified
	TEST_CLI(2, "-l=", 3, CLI_WRONG_OPTIONS);

	// Repeated level
	TEST_CLI(2, "-ldd", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-leie", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-litdit", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-lai", 3, CLI_WRONG_OPTIONS);
}

BOOST_FIXTURE_TEST_CASE(TerminationOptionTest, CLIFixture) {
	// Valid options
	TEST_CLI(2, "-tg=10", 3, CLI_SUCCESS);
	TEST_CLI(2, "-te=1000", 3, CLI_SUCCESS);
	TEST_CLI(2, "-tg20", 3, CLI_SUCCESS);
	TEST_CLI(2, "-te1563", 3, CLI_SUCCESS);

	// Invalid options
	TEST_CLI(2, "-ta=100", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-tgb=250", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-tecc=2000", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-t=100", 3, CLI_WRONG_OPTIONS);

	// Invalid value
	TEST_CLI(2, "-tg", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-te=", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-tg=2.3", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-te=32ddsw", 3, CLI_WRONG_OPTIONS);

	// Termination hook overwrite
	Reset();
	argv[2] = const_cast<char*>("-tg=10");
	BOOST_CHECK(CommandLineInterface::Process(3, argv) == CLI_SUCCESS);

	uint count = 0;
	while (auto hook = GetPopulation()->FindHook<InformedTerminationHook>()) {
		GetPopulation()->RemoveHook(hook);
		count++;
	}
	BOOST_CHECK(count == 1);
}

BOOST_FIXTURE_TEST_CASE(BackupRestoreOptionTest, CLIFixture) {
	// Valid options
	TEST_CLI(2, "-b.backup", 3, CLI_SUCCESS);
	TEST_CLI(2, "-b=backup", 3, CLI_SUCCESS);
	TEST_CLI(2, "-b3=backup", 3, CLI_SUCCESS);
	TEST_CLI(2, "-r", 3, CLI_SUCCESS);
	TEST_CLI(2, "-rbackup", 3, CLI_SUCCESS);
	TEST_CLI(2, "-r=.backup", 3, CLI_SUCCESS);
	TEST_CLI(2, "-r10=backup", 3, CLI_SUCCESS);

	// Invalid options
	TEST_CLI(2, "-b", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-b=", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-r=", 3, CLI_WRONG_OPTIONS);

	// Invalid value
	TEST_CLI(2, "-b2.3=backup", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-rdas=backup", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "-bs2d3=backup", 3, CLI_WRONG_OPTIONS);

	// Default backup hook
	Reset();
	BOOST_CHECK(CommandLineInterface::Process(2, argv) == CLI_SUCCESS);

	using namespace boost::filesystem;

	SetOutputEnabled(true);
	uint count = 0;
	while (auto hook = GetPopulation()->FindHook<BackupHook>()) {
		BOOST_CHECK(path(hook->GetDirectory()).filename().string() == ".backup");
		GetPopulation()->RemoveHook(hook);
		count++;
	}
	BOOST_CHECK(count == 1);
	SetOutputEnabled(false);

	// Default backup hook with restore
	Reset();
	argv[2] = const_cast<char*>("-rbackup");
	BOOST_CHECK(CommandLineInterface::Process(3, argv) == CLI_SUCCESS);

	SetOutputEnabled(true);
	count = 0;
	while (auto hook = GetPopulation()->FindHook<BackupHook>()) {
		BOOST_CHECK(hook->GetDirectory() == "backup");
		GetPopulation()->RemoveHook(hook);
		count++;
	}
	BOOST_CHECK(count == 1);
	SetOutputEnabled(false);

	using namespace boost::filesystem;
	remove_all("backup");
}

BOOST_FIXTURE_TEST_CASE(VariableOptionTest, CLIFixture) {
	// Valid options
	TEST_CLI(2, "--abc=dsa", 3, CLI_SUCCESS);
	TEST_CLI(2, "--321=fd231", 3, CLI_SUCCESS);
	TEST_CLI(2, "--size=25", 3, CLI_SUCCESS);

	// Invalid options
	TEST_CLI(2, "--size", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "--=", 3, CLI_WRONG_OPTIONS);
	TEST_CLI(2, "--sda=ds=ad", 3, CLI_WRONG_OPTIONS);

	// Effect test
	Reset();
	argv[2] = const_cast<char*>("--size=25");
	BOOST_CHECK(CommandLineInterface::Process(3, argv) == CLI_SUCCESS);
	BOOST_CHECK(GetPopulation()->GetSize() == 25);
}

}
}
}
