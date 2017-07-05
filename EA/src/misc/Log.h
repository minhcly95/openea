/*
 * Log.h
 *
 *  Created on: Apr 4, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"

namespace ea {

using namespace std;

/**
 * Static class which controls log levels and redirection.
 * This class only provides control on log levels.
 * Log levels are defined in Log::SeverityLevel and are used in all the redirection functions
 * defined in Log class.
 * Redirection functions include Redirect(), Clear(), Default() and Add().
 * To actually write a log record, please use LogStream.
 *
 * @see LogStream
 * @see Log::SeverityLevel
 * @see Redirect()
 */
class Log {
public:
	enum SeverityLevel {
		TRACE,	///< Trace log level, display which processes and functions are being called (only available at some points).
		DEBUG,	///< Debug log level, display debugging messages which describe how the system is currently working.
		INFO,	///< Infomation log level, display normal user messages.
		ERROR,	///< Error log level, display error messages happened in the system.
		SeverityLevel_MAX = ERROR	///< Utility value to identify the size of the enum.
	};

	/**
	 * Clear all redirection of the given level.
	 * After calling this function, the given level will produce no output to any target.
	 * @param pLevel The log level to be cleared.
	 */
	static void Clear(SeverityLevel pLevel);
	/**
	 * Set redirection setting of the given level to default.
	 * The default setting for TRACE level is None (Clear()), for DEBUG and INFO is stdout, and ERROR is stderr.
	 * @param pLevel The log level to be reset.
	 */
	static void Default(SeverityLevel pLevel);
	/**
	 * Redirect the given log level.
	 * The target file can be empty string "", stdout, stderr or a path to file system
	 * (if existed, log records will be appended). Old targets will be cleared.
	 *
	 * The target file could be multiple targets, seperated by comma (,) with no space.
	 * For example:
	 *
	 * @code
	 * Log::Redirect(Log::INFO, "stdout,log/info.txt");
	 * @endcode
	 *
	 * @param pLevel The log level to be redirected.
	 * @param pFile The target of redirection.
	 */
	static void Redirect(SeverityLevel pLevel, string pFile);
	/**
	 * Add a redirection target to the given log level.
	 * Similar to Redirect() but new targets will be appended instead of clearing the old ones.
	 * The target file could also be multiple targets as described in Redirect().
	 * @param pLevel The log level to be redirected.
	 * @param pFile The target of redirection.
	 */
	static void Add(SeverityLevel pLevel, string pFile);

	/**
	 * Constant string equivalent to empty string "".
	 */
	static const string NoOutput;
};

/**
 * Stream class which is used to write log.
 * This class connects to the log infrastructure to actually write log into memory and to the console.
 * Users can use the operator << to push data into the stream. To write the log to console or memory,
 * std::flush must be pushed to the stream (std::endl is not needed, a log record already has a new line symbol).
 * Users can use the constructor to create a LogStream directly and use provided macros such as:
 * - #EA_LOG(LEVEL) : Open LogStream at LEVEL.
 * - #EA_LOG_TRACE : Open LogStream at Log::TRACE level.
 * - #EA_LOG_DEBUG : Open LogStream at Log::DEBUG level.
 * - #EA_LOG_INFO : Open LogStream at Log::INFO level.
 * - #EA_LOG_ERROR : Open LogStream at Log::ERROR level.
 * Example of using macros:
 *
 * @code
 * EA_LOG_INFO << "Hello world!" << std::flush;
 * @endcode
 */
class LogStream : public ostream {
public:
	/**
	 * Create a log stream at the given level.
	 * @param pLevel The level of log stream.
	 */
	LogStream(Log::SeverityLevel pLevel);
	~LogStream();
};

#define EA_LOG(LEVEL) ea::LogStream(ea::Log::LEVEL)
#define EA_LOG_TRACE EA_LOG(TRACE)
#define EA_LOG_DEBUG EA_LOG(DEBUG)
#define EA_LOG_INFO EA_LOG(INFO)
#define EA_LOG_ERROR EA_LOG(ERROR)

} /* namespace ea */
