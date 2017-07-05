/*
 * EAException.cpp
 *
 *  Created on: Mar 31, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "EAException.h"
#include <regex>

namespace ea {

using namespace std;

/**
 * @class EAException
 * Exception class for the EA core library.
 * All logical exceptions thrown in the library are mostly EAException.
 * This exception not only stores a message, but also an error code (defined by EAException::ErrorCode enum)
 * and an inner exception if there is any. Developers should not throw this exception
 * directly, but via #EA_EXCEPTION which will include also the file name and line number of the exception.
 */

/**
 * Create an exception with given error code, message and optional inner exception.
 * @param pCode The error code, converted to int implicitly from EAException::ErrorCode.
 * @param pMessage The message of the exception, which will be shown when calling std::exception::what().
 * @param pInner The inner exception get from std::current_exception().
 */
EAException::EAException(int pCode, string pMessage, exception_ptr pInner) :
		mCode(pCode), mInner(pInner) {
	mMessage = pMessage;
	if (pInner)
		try {
			rethrow_exception(pInner);
		} catch (exception& e) {
			mMessage += "\n\tInner: ";
			mMessage += e.what();
		}
}

EAException::~EAException() {
}

/**
 * Get the error code of this exception.
 * @return The error code.
 */
int EAException::GetCode() const {
	return mCode;
}

/**
 * Get the inner exception of this exception.
 * @return The inner exception. nullptr if there isn't any.
 */
const exception_ptr& EAException::GetInnerException() const {
	return mInner;
}

/**
 * Get the message of this exception.
 * @return The message.
 */
const string& EAException::GetMessage() const {
	return mMessage;
}

/**
 * Implementation of std::exception::what().
 * This function returns GetMessage() in the old C style string format.
 * @return The message in C style string format.
 */
const char* EAException::what() const noexcept {
	return GetMessage().c_str();
}

/**
 * Helper function to reduce file name to canonical form.
 * @param fileName The file path provided from #EA_EXCEPTION macro.
 * @return The reduced path.
 */
string ea::EAException::Reduce(string fileName) {
	regex dot("(^|\\/)\\.\\/");
	regex dotdot("[^\\/]*\\/\\.{2}\\/");

	fileName = regex_replace(fileName, dot, "$1");
	while (regex_search(fileName, dotdot))
		fileName = regex_replace(fileName, dotdot, "");

	return fileName;
}

/**
 * @class RTOCException
 * Specialization of EAException for RTOC module.
 * Logical exceptions thrown in the RTOC module are RTOCException.
 * This class provides a new bunch of RTOCException::ErrorCode which is useful
 * in the context of the RTOC module.
 */

/**
 * Create an RTOC exception with given error code, message and an optional inner exception.
 * @param pCode The error code, converted to int implicitly from RTOCException::ErrorCode.
 * @param pMessage The message of the exception, which will be shown when calling std::exception::what().
 * @param pInner The inner exception get from std::current_exception().
 */
RTOCException::RTOCException(int pCode, string pMessage, exception_ptr pInner) :
		EAException(pCode, pMessage, pInner) {
}

RTOCException::~RTOCException() {
}

/**
 * @class ConfigFileException
 * Specialization of EAException for the EAMLReader.
 * Logical exceptions thrown in the EAMLReader are mostly ConfigFileException.
 * This class provides a new bunch of ConfigFileException::ErrorCode which is useful
 * in the context of reading XML file.
 */

/**
 * Create a ConfigFileException with given error code, message and an optional inner exception.
 * @param pCode The error code, converted to int implicitly from ConfigFileException::ErrorCode.
 * @param pMessage The message of the exception, which will be shown when calling std::exception::what().
 * @param pInner The inner exception get from std::current_exception().
 */
ConfigFileException::ConfigFileException(int pCode, string pMessage,
		exception_ptr pInner) :
		EAException(pCode, pMessage, pInner) {
}

ConfigFileException::~ConfigFileException() {
}

} /* namespace ea */
