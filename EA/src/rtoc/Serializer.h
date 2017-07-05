/*
 * Serializer.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Core.h"
#include <string>
#include <sstream>
#include <ios>
#include <locale>
#include <boost/algorithm/string/trim.hpp>
#include <iterator>

namespace ea {

using namespace std;

/**
 * Static class provide type conversion from string to simple type.
 * This class provides a de-serialize method from a string to a simple type such as int or long long.
 * If users plan to convert an EAML attribute value to a custom type, a specialization of DoParse()
 * and TypeName() should be implemented (or using #EA_DEFINE_CUSTOM_SERIALIZER macro to ease the process).
 *
 * Supported types include bool, int, uint, float, double, ullong, string and FitnessTerminationHook::Condition.
 * @tparam T The output type.
 */
template<class T>
class Serializer {
public:
	/**
	 * Parse a string into a value of type T.
	 * This function will pre-process the input and check the validity of the output after calling DoParse().
	 * @param str The string to be parsed.
	 * @return The converted value.
	 */
	inline static T Parse(string str) {
		istringstream ss(boost::algorithm::trim_copy(str));
		T data;
		if (!DoParse(data, ss) || ss.rdbuf()->in_avail())
			throw EA_EXCEPTION(RTOCException, DESERIALIZE_FAILED,
					"Serializer: Error when parsing \"" + str + "\" into " + TypeName() + ".");
		return data;
	}
	/**
	 * Get the name of the output type of this Serializer.
	 * This function is used in debugging message. Normally, this function would be automatically defined if
	 * using the #EA_DEFINE_CUSTOM_SERIALIZER macro.
	 * @return The name of the output type.
	 */
	inline static string TypeName();

protected:
	/**
	 * Implementation of parsing function (need to be specialized for custom type).
	 * The implementation should read the input stream and write the value into the given reference.
	 * If the conversion failed, this should return false so Parse() function could throw an exception.
	 * Normally, the signature of this function would be declared if using the #EA_DEFINE_CUSTOM_SERIALIZER macro.
	 *
	 * @param data The output data. Implementation must write the parsed value into this reference.
	 * @param ss The input stream containing the input string. This string is guaranteed to be trimmed all the spaces at two ends.
	 * @return Whether the conversion is succeed.
	 *
	 * @see #EA_DEFINE_CUSTOM_SERIALIZER
	 */
	inline static bool DoParse(T& data, istringstream& ss);
};

#ifndef DOXYGEN_IGNORE
template<class T>
class Serializer<Ptr<T>> {
public:
	inline static Ptr<T> Parse(string str) {
		throw EA_EXCEPTION(RTOCException, DESERIALIZE_FAILED,
				"Serializer<Ptr>: Serializer for Ptr is not defined by default.");
	}
};

template<class T>
class Serializer<vector<Ptr<T>>> {
public:
	inline static vector<Ptr<T>> Parse(string str) {
		throw EA_EXCEPTION(RTOCException, DESERIALIZE_FAILED,
				"Serializer<vector<Ptr>>: Serializer for vector<Ptr> is not defined by default.");
	}
};
#endif

/**
 * Define a specialization of Serializer.
 * This macro will specialize ea::Serializer::TypeName() by using the string form of TYPE
 * and declare the signature of ea::Serializer::DoParse(). Users should follow this macro by
 * the brackets to define the implementation.
 *
 * Example of specialization of ea::Serializer for type string :
 *
 * @code
 * EA_DEFINE_CUSTOM_SERIALIZER(string, data, ss) {
 *     int length = ss.rdbuf()->in_avail() + 1;
 *     char* buffer = new char[length];
 *     ss.read(buffer, length);
 *     buffer[length - 1] = '\0';
 *     data = string(buffer);
 *     delete[] buffer;
 *     return true;
 * }
 * @endcode
 *
 * @param TYPE The type as in Serializer<TYPE>.
 * @param DATA The placeholder for data parameter in Serializer::DoParse().
 * @param STREAM The placeholder for ss parameter in Serializer::DoParse().
 */
#define EA_DEFINE_CUSTOM_SERIALIZER(TYPE, DATA, STREAM)\
	template<>\
	inline std::string ea::Serializer<TYPE>::TypeName() {\
		return #TYPE;\
	}\
	template<>\
	inline bool ea::Serializer<TYPE>::DoParse(TYPE& DATA, std::istringstream& STREAM)

#define EA_SERIALIZER_BASE_MACRO(TYPE, EXPR) EA_DEFINE_CUSTOM_SERIALIZER(TYPE, data, ss) {\
		return bool(EXPR);\
	}

#define EA_DEFINE_SERIALIZER_FOR_1(TYPE)\
		EA_SERIALIZER_BASE_MACRO(TYPE, ss >> data)
#define EA_DEFINE_SERIALIZER_FOR_2(TYPE, FILTER)\
		EA_SERIALIZER_BASE_MACRO(TYPE, ss >> FILTER >> data)

#define EA_DEFINE_SERIALIZER_FOR(...)\
	GET_MACRO_2(__VA_ARGS__, EA_DEFINE_SERIALIZER_FOR_2, EA_DEFINE_SERIALIZER_FOR_1)(__VA_ARGS__)

#ifndef DOXYGEN_IGNORE
EA_DEFINE_SERIALIZER_FOR(bool, std::boolalpha)
EA_DEFINE_SERIALIZER_FOR(int)
EA_DEFINE_SERIALIZER_FOR(uint)
EA_DEFINE_SERIALIZER_FOR(float)
EA_DEFINE_SERIALIZER_FOR(double)
EA_DEFINE_SERIALIZER_FOR(ullong)

EA_DEFINE_CUSTOM_SERIALIZER(string, data, ss) {
	int length = ss.rdbuf()->in_avail() + 1;
	char* buffer = new char[length];
	ss.read(buffer, length);
	buffer[length - 1] = '\0';
	data = string(buffer);
	delete[] buffer;
	return true;
}
#endif

}	// namespace ea


