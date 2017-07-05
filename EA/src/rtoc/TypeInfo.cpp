/*
 * TypeInfo.cpp
 *
 *  Created on: Apr 18, 2017
 *      Author: Bui Quang Minh
 */

#include "TypeInfo.h"

namespace ea {

/**
 * @class TypeInfo
 * Data structure containing class information.
 * This class provides functions to describe class information such as the attribute names,
 * class name and constructor. In C++, classes and types by themselves cannot be stored in variables so
 * this class is created to serve as a placeholder for types.
 *
 * Every class which could be constructed in run-time must be a child of Constructible and override
 * Constructible::GetTypeInfo() to return the TypeInfo of that class. First, a TypeInfo must be created
 * by calling the constructor TypeInfo() with the class name. Then users can add attribute names by using
 * Add(string, MemberT ClassT::*) function.
 * Finally, a valid TypeInfo must assign a main constructor by using SetConstructor<T, Args...>() function.
 *
 * Example of writing a custom TypeInfo for a class:
 *
 * @code
 * class MyClass : public ea::Constructible {
 * private:
 *     double myAttr;
 *
 * public:
 *     MyClass(int pSize);
 *
 *     EA_TYPEINFO_CUSTOM_INLINE {
 *         return *ea::TypeInfo("MyClass").Add("attr", &MyClass::myAttr)->SetConstructor<MyClass, int>("size");
 *     }
 *     EA_TYPEINFO_GET
 * };
 * @endcode
 *
 * The above code will create a TypeInfo named "MyClass", add a attribute named "attr" binded to myAttr field of MyClass,
 * and define the main constructor MyClass(int) with the parameter named "size". The name of parameters and attributes
 * will be used in EAML file or Constructible::Set() function. For more details on type info macros, see Constructible::GetTypeInfo().
 *
 * @see Constructible
 * @see Constructible::GetTypeInfo()
 */

/**
 * @class TypeInfo::StringWrapper
 * Template helper structure whose type will always return string.
 * This struct is used in @tt{TypeInfo::SetConstructor()} to make the number of parameters equals to the
 * number of template arguments.
 */

/**
 * Create a TypeInfo with the given class name.
 * It is not required that the class name must be the same as the real class name.
 * One can name a class MyClass by another name like "AnotherClass", but if a RTOC service is used
 * (such as NameService or EAMLReader), the given class name must be referred instead of the real one.
 * @param pTypeName The class name.
 */
TypeInfo::TypeInfo(string pTypeName) :
		mTypeName(pTypeName), mSetterMap(), mConstructor() {
}

TypeInfo::~TypeInfo() {
}

/**
 * Get the class name of this type.
 * @return The class name.
 */
string TypeInfo::GetTypeName() const {
	return mTypeName;
}

/**
 * @fn TypeInfo* TypeInfo::Add(string pName, MemberT ClassT::*pMember)
 * Add an attribute to the definition of this type.
 * Adding an attribute into a TypeInfo allows the field pMember can be set by referring to its name pName
 * by calling Constructible::Set() or TypeInfo::Set() function. Similar to class name, attribute name is not
 * required to be the same with the real field name, but users must use the new name when referring that
 * field. The field defined of this function is optional when calling Construct().
 * @tparam MemberT The type of the field (auto-deduced).
 * @tparam ClassT The type of the class containing the field, must be a child of Constructible (auto-deduced).
 * @param pName The name of the attribute.
 * @param pMember The pointer to the class field.
 * @return Pointer to this TypeInfo for cascade invocation.
 */

void TypeInfo::Add(string pName, UnifiedSetter pSetter) {
	if (mSetterMap.find(pName) == mSetterMap.end())
		mSetterMap.emplace(pName, pSetter);
	else
		throw EA_EXCEPTION(RTOCException, FIELD_NAME_REGISTERED,
				"Field \"" + GetTypeName() + ":" + pName + "\" is already registered.");
}

/**
 * @fn TypeInfo* TypeInfo::SetConstructor(typename StringWrapper<Args>::type ... pNames)
 * Set the main constructor of this type.
 * Every valid TypeInfo must set a constructor. Setting a constructor allows RTOC services uses that constructor
 * to create a real object of this type. Parameters of the constructor is referred by pNames and are mandatory
 * when calling Construct(). The order of Args and pName must be the same with the order of parameters in the real constructor.
 * The names defined in this function cannot be used in Constructible::Set() or TypeInfo::Set()
 * because they don't bind to a determined field (they only bind to the parameter of the constructor).
 * @tparam T The type of this class.
 * @tparam Args The type of parameters with the same order with the real constructor.
 * @param pNames The name of the parameters with the same order.
 * @return Pointer to this TypeInfo for cascade invocation.
 */

void TypeInfo::SetConstructor(ConstructorCaller pCaller) {
	mConstructor = pCaller;
}

/**
 * Construct a Constructible object of this type.
 * This function will construct a real object of this type using the defined constructor in SetConstructor().
 * The parameters for the constructor are provided by the input data map. Parameters for constructor are mandatory
 * and the function will throw an exception if some parameters are missing. Then, the remaining data fields will be used
 * to Set() the optional attributes defined by Add(string, MemberT ClassT::*).
 * Non-existed attribute name won't throw an exception.
 * @param pData The data map providing parameter and argument values.
 * @return The constructed object.
 */
ConstructiblePtr TypeInfo::Construct(const UnifiedMap& pData) const {
	if (!mConstructor)
		throw EA_EXCEPTION(RTOCException, CONSTRUCTOR_MISSING,
				GetTypeName() + " didn't define a constructor.");

	UnifiedMap copy(pData);
	ConstructiblePtr obj = mConstructor(copy);

	for (auto field : copy)
		Set(field.first, obj, field.second);

	return obj;
}

/**
 * Set the value of an optional attribute of the given object.
 * This function will set the value of the field corresponding to the given name.
 * The attribute name must be registered by Add(string, MemberT ClassT::*) function.
 * If the name doesn't exist, the function will return false instead of throwing exception.
 * Any type casting is automatically done by using the type definition of Add(string, MemberT ClassT::*).
 * @param pName The name of the attribute.
 * @param pObj The object containing the attribute.
 * @param pData The data to be set.
 * @return true if the attribute exists.
 */
bool TypeInfo::Set(string pName, ConstructiblePtrRef pObj, UnifiedDataRef pData) const {
	auto setter = mSetterMap.find(pName);
	if (setter == mSetterMap.end())
		return false;

	setter->second(pObj, pData);
	return true;
}

} /* namespace ea */
