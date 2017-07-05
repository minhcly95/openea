/*
 * LogTest.cpp
 *
 *  Created on: Apr 10, 2017
 *      Author: Bui Quang Minh
 */

#define BOOST_TEST_DYN_LINK
#include "../pch.h"
#include <boost/test/unit_test.hpp>

#include "../EA.h"
#include <cstdio>
#include <unistd.h>

namespace ea {

namespace test {

namespace utf = boost::unit_test;

BOOST_AUTO_TEST_SUITE(LogTest)

struct LogFixture {
	int stdoutfd, stderrfd;
	LogFixture() {
		stdoutfd = dup(STDOUT_FILENO);
		stderrfd = dup(STDERR_FILENO);
		freopen("stdout.log", "w", stdout);
		freopen("stderr.log", "w", stderr);

		Log::Default(Log::TRACE);
		Log::Default(Log::DEBUG);
		Log::Default(Log::INFO);
		Log::Default(Log::ERROR);
	}
	void Restore() {
		dup2(stdoutfd, STDOUT_FILENO);
		dup2(stderrfd, STDERR_FILENO);
		close(stdoutfd);
		close(stderrfd);
	}
	void CheckLine(istream& is, string value, int ignore = 0) {
		string line;
		is.ignore(ignore);
		getline(is, line);
		BOOST_CHECK(line == value);
	}
	void CheckEOF(istream& is) {
		string line;
		getline(is, line);
		BOOST_CHECK(is.eofbit);
	}
	void PushMessage() {
		EA_LOG_TRACE << "Trace message" << flush;
		EA_LOG_DEBUG << "Debug message" << flush;
		EA_LOG_INFO << "Info message" << flush;
		EA_LOG_ERROR << "Error message" << flush;
	}
	~LogFixture() {
		Log::Default(Log::TRACE);
		Log::Clear(Log::DEBUG);
		Log::Default(Log::INFO);
		Log::Default(Log::ERROR);

		remove("stdout.log");
		remove("stderr.log");
	}
};

#define COLOR 7
#define TIMESTAMP 29

BOOST_FIXTURE_TEST_CASE(DefaultTest, LogFixture) {
	BOOST_CHECK_NO_THROW(PushMessage());
	Restore();

	ifstream ifs("stdout.log");

	CheckLine(ifs, "Debug message\e[0m", COLOR + TIMESTAMP);
	CheckLine(ifs, "Info message\e[0m");
	CheckEOF(ifs);
	ifs.close();

	ifs = ifstream("stderr.log");
	CheckLine(ifs, "Error message\e[0m", COLOR + TIMESTAMP);
	CheckEOF(ifs);
	ifs.close();
}

BOOST_FIXTURE_TEST_CASE(ClearTest, LogFixture) {
	Log::Clear(Log::INFO);
	Log::Clear(Log::ERROR);

	BOOST_CHECK_NO_THROW(PushMessage());
	Restore();

	ifstream ifs("stdout.log");

	CheckEOF(ifs);
	ifs.close();

	ifs = ifstream("stderr.log");
	CheckEOF(ifs);
	ifs.close();
}

BOOST_FIXTURE_TEST_CASE(AllStdOutTest, LogFixture) {
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::TRACE, "stdout"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::DEBUG, "stdout"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::INFO, "stdout"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::ERROR, "stdout"));

	BOOST_CHECK_NO_THROW(PushMessage());
	Restore();

	ifstream ifs("stdout.log");

	CheckLine(ifs, "Trace message\e[0m", COLOR + TIMESTAMP);
	CheckLine(ifs, "Debug message\e[0m", COLOR + TIMESTAMP);
	CheckLine(ifs, "Info message\e[0m");
	CheckLine(ifs, "Error message\e[0m", COLOR + TIMESTAMP);
	CheckEOF(ifs);
	ifs.close();

	ifs = ifstream("stderr.log");
	CheckEOF(ifs);
	ifs.close();
}

BOOST_FIXTURE_TEST_CASE(AllStdErrTest, LogFixture) {
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::TRACE, "stderr"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::DEBUG, "stderr"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::INFO, "stderr"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::ERROR, "stderr"));

	BOOST_CHECK_NO_THROW(PushMessage());
	Restore();

	ifstream ifs("stdout.log");
	CheckEOF(ifs);
	ifs.close();

	ifs = ifstream("stderr.log");
	CheckLine(ifs, "Trace message\e[0m", COLOR + TIMESTAMP);
	CheckLine(ifs, "Debug message\e[0m", COLOR + TIMESTAMP);
	CheckLine(ifs, "Info message\e[0m");
	CheckLine(ifs, "Error message\e[0m", COLOR + TIMESTAMP);
	CheckEOF(ifs);
	ifs.close();
}

BOOST_FIXTURE_TEST_CASE(FileTest, LogFixture) {
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::TRACE, "file1.log"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::DEBUG, "file2.log"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::INFO, "file1.log"));
	BOOST_CHECK_NO_THROW(Log::Redirect(Log::ERROR, "file2.log"));

	BOOST_CHECK_NO_THROW(PushMessage());
	Restore();

	ifstream ifs("stdout.log");
	CheckEOF(ifs);
	ifs.close();

	ifs = ifstream("stderr.log");
	CheckEOF(ifs);
	ifs.close();

	ifs = ifstream("file1.log");
	CheckLine(ifs, "Trace message", TIMESTAMP);
	CheckLine(ifs, "Info message");
	CheckEOF(ifs);
	ifs.close();

	ifs = ifstream("file2.log");
	CheckLine(ifs, "Debug message", TIMESTAMP);
	CheckLine(ifs, "Error message", TIMESTAMP);
	CheckEOF(ifs);
	ifs.close();

	remove("file1.log");
	remove("file2.log");
}

BOOST_AUTO_TEST_SUITE_END()

}// namespace test

}// namespace ea
