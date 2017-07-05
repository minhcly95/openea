/*
 * Constructible.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "Constructible.h"
#include "Serializer.h"
#include <regex>
#include "../Common.h"

namespace ea {

using namespace std;

/**
 * @class Constructible
 * Base class for all classes which can be constructed in run-time.
 * This class is a part of the Run-Time Object Construction (RTOC) module.
 * All classes which is constructed during run-time, e.g. from an EAML file, must derive from this class.
 * From this class, other RTOC classes like NameService or EAMLReader can reference to the object without
 * knowing its true type. all the interfaces like Strategy and Selector are already defined as its child.
 *
 * Child classes which will be constructed during run-time should override GetTypeInfo()
 * to provide the type information such as class name and main constructor. For more details, see GetTypeInfo().
 *
 * @see GetTypeInfo()
 */

Constructible::~Constructible() {
}

/**
 * Get the TypeInfo which describes information to construct object in run-time.
 * Child classes which will be constructed during run-time should override GetTypeInfo()
 * to provide the type information such as class name and main constructor.
 * See TypeInfo to know how to write such definition.
 * Numerous macros are also included to automate the process:
 *
 * - #EA_TYPEINFO_DEFAULT(Class) : Define a default TypeInfo for the corresponding Class.
 * - #EA_TYPEINFO_CUSTOM_INLINE : Define a custom TypeInfo inline with the class declaration.
 * Must followed by #EA_TYPEINFO_GET macro.
 * - #EA_TYPEINFO_CUSTOM_DECL : Declare a custom TypeInfo whose implementation is defined by #EA_TYPEINFO_CUSTOM_IMPL(Class).
 * - #EA_TYPEINFO_CUSTOM_IMPL(Class) : Implement the TypeInfo of Class which has been declared by by #EA_TYPEINFO_CUSTOM_DECL.
 *
 * For template and specialized classes, please refer to:
 * #EA_TYPEINFO_SPECIALIZED_DEFAULT, #EA_TYPEINFO_SPECIALIZED_DECL, #EA_TYPEINFO_SPECIALIZED_IMPL, #EA_TYPEINFO_SPECIALIZED_INLINE.
 *
 * Default implementation is throwing an exception. Therefore, users should not invoke this if the child class does not override
 * this function.
 * @return The TypeInfo of the type.
 */
TypeInfo Constructible::GetTypeInfo() {
	throw EA_EXCEPTION(RTOCException, NOT_RUNTIME_CONSTRUCTED_OBJ,
			"This object is not runtime-constructible because TypeInfo is not defined.");
}

/**
 * Get the type name of this object.
 * The child class must override GetTypeInfo() or using type info macros to define the TypeInfo.
 * Without defining a TypeInfo, class name cannot be known and therefore an exception will be thrown.
 *
 * @return The type name.
 * @see GetTypeInfo()
 */
const string Constructible::GetTypeName() {
	return GetTypeInfo().GetTypeName();
}

/**
 * Get the type name without throwing an exception.
 * The functionality is the same with GetTypeName(). However, if a TypeInfo is not defined, then this function
 * will return the given default string without throwing any exception.
 *
 * @param defaultStr The default string would be returned if TypeInfo is not defined.
 * @return The type name or the given default string if TypeInfo is not defined.
 * @see GetTypeName()
 */
const string Constructible::GetTypeNameSafe(string defaultStr = "") {
	try {
		return GetTypeName();
	} catch (RTOCException& e) {
		return defaultStr;
	}
}

/**
 * Set an attribute with the given value.
 * The name of the attribute must be defined in the TypeInfo. Otherwise, an exception will be thrown.
 *
 * The input data type is a string. Data type conversion is automatically done by looking the type when defining
 * TypeInfo.
 *
 * @param pName The name of the attribute.
 * @param pValue The value to be assigned to the attribute.
 */
void Constructible::Set(string pName, string pValue) {
	TypeInfo info = GetTypeInfo();
	UnifiedData data(pValue);
	if (!info.Set(pName, shared_from_this(), data))
		throw EA_EXCEPTION(RTOCException, FIELD_NOT_EXIST,
				"Constructible::Set: Attribute \"" + info.GetTypeName() + ":"
						+ pName + "\" does not exist.");
}

/**
 * Set an element with the given object.
 * The name of the element must be defined in the TypeInfo. Otherwise, an exception will be thrown.
 *
 * The input data type is another Constructible object.
 * Object type casting is automatically done by looking the type when defining TypeInfo.
 *
 * @param pName The name of the element.
 * @param pObj The object to be assigned to the element.
 */
void Constructible::Set(string pName, const ConstructiblePtr& pObj) {
	TypeInfo info = GetTypeInfo();
	UnifiedData data(pObj);
	if (!info.Set(pName, shared_from_this(), data))
		throw EA_EXCEPTION(RTOCException, FIELD_NOT_EXIST,
				"Constructible::Set: Element \"" + info.GetTypeName() + ":"
						+ pName + "\" does not exist.");
}

/**
 * @fn void Constructible::Set(string pName, const Ptr<T>& pObj)
 * Set an element with the given object of type T.
 * The name of the element must be defined in the TypeInfo. Otherwise, an exception will be thrown.
 *
 * The input data type is an object of type T for convenient use.
 * Object type casting is automatically done by looking the type when defining TypeInfo.
 *
 * @tparam T Must be a child class of Constructible (auto-deduced).
 * @param pName The name of the element.
 * @param pObj The object to be assigned to the element.
 */

}	// namespace ea

