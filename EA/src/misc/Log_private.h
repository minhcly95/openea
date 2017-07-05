/*
 * Log_private.h
 *
 *  Created on: Apr 15, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include <sstream>
#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>

namespace ea {

#ifndef DOXYGEN_IGNORE
namespace logging = boost::log;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
using namespace std;

class LogStreamBuf;

class Log {
public:
	enum SeverityLevel {
		TRACE, DEBUG, INFO, ERROR, SeverityLevel_MAX = ERROR
	};

	static void Clear(SeverityLevel pLevel);
	static void Default(SeverityLevel pLevel);
	static void Redirect(SeverityLevel pLevel, string pFile);
	static void Add(SeverityLevel pLevel, string pFile);

	static const string NoOutput;

private:
	static struct InitStruct {
		InitStruct();
	} __init;

	static vector<boost::shared_ptr<sinks::sink>> mSinks[SeverityLevel_MAX + 1];
	static src::severity_logger_mt<SeverityLevel> sLogger;
	static uint sNumSinks;

	static void AddSink(SeverityLevel pLevel, string pFile);

	friend class LogStreamBuf;
};

class LogStreamBuf : public stringbuf {
public:
	LogStreamBuf(Log::SeverityLevel pLevel);

protected:
   virtual int sync() override;

private:
   Log::SeverityLevel mLevel;
};

class LogStream : public ostream {
public:
	LogStream(Log::SeverityLevel pLevel);
	~LogStream();
};
#endif

} /* namespace ea */
