/*
 * Log.cpp
 *
 *  Created on: Apr 4, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"

#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/common.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "Log_private.h"

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

using namespace std;

namespace ea {

#ifndef DOXYGEN_IGNORE
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(EALogger,
		src::severity_logger_mt<Log::SeverityLevel>);

src::severity_logger_mt<Log::SeverityLevel> Log::sLogger = EALogger::get();
vector<boost::shared_ptr<sinks::sink>> Log::mSinks[Log::SeverityLevel_MAX + 1] =
		{ };
uint Log::sNumSinks = 0;

const string Log::NoOutput("");

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", Log::SeverityLevel);

Log::InitStruct::InitStruct() {
	logging::core::get()->set_logging_enabled(false);
	logging::add_common_attributes();
	Default(TRACE);
	Default(DEBUG);
	Default(INFO);
	Default(ERROR);
	logging::add_console_log(cout,
			keywords::filter = (severity > SeverityLevel_MAX));
}
Log::InitStruct Log::__init;
#endif

void Log::Default(SeverityLevel pLevel) {
	switch (pLevel) {
	case TRACE:
		Clear(pLevel);
		break;
	case DEBUG:
	case INFO:
		Redirect(pLevel, "stdout");
		break;
	case ERROR:
		Redirect(pLevel, "stderr");
		break;
	}
}

void Log::Clear(SeverityLevel pLevel) {
	for (auto sink : mSinks[pLevel]) {
		logging::core::get()->remove_sink(sink);
		sNumSinks--;
		if (!sNumSinks)
			logging::core::get()->set_logging_enabled(false);
	}
	mSinks[pLevel].clear();
}

void Log::Redirect(SeverityLevel pLevel, string pFile) {
	Clear(pLevel);
	Add(pLevel, pFile);
}

void Log::Add(SeverityLevel pLevel, string pFile) {
	vector<string> targets;
	boost::algorithm::split(targets, pFile, boost::algorithm::is_any_of(","));
	for (string target : targets)
		AddSink(pLevel, target);
}

#ifndef DOXYGEN_IGNORE
void Log::AddSink(SeverityLevel pLevel, string pFile) {
	if (pFile == NoOutput)
		return;

	bool withTimeStamp = (pLevel != INFO);
	string format = withTimeStamp ? "[%TimeStamp%] %Message%" : "%Message%";

	string color;
#define COLOR(LEVEL, CODE) case LEVEL: color = "\e[0;" CODE "m"; break;
	switch (pLevel) {
	COLOR(ERROR, "31")
	COLOR(DEBUG, "33")
	COLOR(TRACE, "34")
	default:
		break;
	}

	if (pFile == "stdout")
		mSinks[pLevel].push_back((boost::shared_ptr<sinks::sink>)logging::add_console_log(
				cout,
				keywords::filter = (severity == pLevel),
				keywords::auto_flush = true,
				keywords::format = color + format + "\e[0m"));

	else if (pFile == "stderr")
		mSinks[pLevel].push_back((boost::shared_ptr<sinks::sink>)logging::add_console_log(
				cerr,
				keywords::filter = (severity == pLevel),
				keywords::auto_flush = true,
				keywords::format = color + format + "\e[0m"));

	else
		mSinks[pLevel].push_back((boost::shared_ptr<sinks::sink>)logging::add_file_log(
				pFile,
				keywords::filter = (severity == pLevel),
				keywords::open_mode = ios_base::app,
				keywords::auto_flush = true,
				keywords::format = format));

	sNumSinks++;
	logging::core::get()->set_logging_enabled(true);
}

LogStreamBuf::LogStreamBuf(Log::SeverityLevel pLevel) :
		mLevel(pLevel) {
}

int LogStreamBuf::sync() {
	if (str().empty())
		return 0;

	logging::record rec = Log::sLogger.open_record(keywords::severity = mLevel);
	if (rec) {
		logging::record_ostream strm(rec);
		strm << str();
		strm.flush();
		Log::sLogger.push_record(std::move(rec));
	}
	str("");
	return 0;
}
#endif

LogStream::LogStream(Log::SeverityLevel pLevel) :
		ostream(new LogStreamBuf(pLevel)) {
}

LogStream::~LogStream() {
	flush();
	delete rdbuf();
}

} /* namespace ea */
