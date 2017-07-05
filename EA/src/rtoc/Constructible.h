/*
 * Constructible.h
 *
 *  Created on: Mar 22, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Utility.h"
#include "Serializer.h"
#include "TypeInfo.h"
#include <cxxabi.h>
#include <type_traits>

namespace ea {

using namespace std;

class Constructible : public std::enable_shared_from_this<Constructible> {
public:
	virtual ~Constructible();

	virtual TypeInfo GetTypeInfo();
	const string GetTypeName();
	const string GetTypeNameSafe(string defaultStr);

	void Set(string pName, string pValue);
	void Set(string pName, const ConstructiblePtr& pObj);

	template<class T>
	void Set(string pName, const Ptr<T>& pObj) {
		Set(pName, static_pointer_cast<Constructible>(pObj));
	}

	template<class T>
	Ptr<T> To() {
		static_assert(is_base_of<Constructible, T>::value,
				"T must be a child of Constructible.");
		auto casted = dynamic_pointer_cast<T>(shared_from_this());
		if (!casted)
			throw EA_EXCEPTION(EAException, CONSTRUCTIBLE_BAD_CAST, "This object cannot be casted to the given type.");
		return casted;
	}
};

#define EA_TYPEINFO_GET \
	inline virtual ea::TypeInfo GetTypeInfo() override {\
		return this->TypeInfo();\
	}

#define EA_TYPEINFO_DEFAULT(CLASS)\
	inline static ea::TypeInfo TypeInfo() {\
		return *ea::TypeInfo(string(#CLASS)).SetConstructor<CLASS>();\
	}\
	EA_TYPEINFO_GET

#define EA_TYPEINFO_CUSTOM_INLINE inline static ea::TypeInfo TypeInfo()

#define EA_TYPEINFO_CUSTOM_DECL \
	static ea::TypeInfo TypeInfo();\
	EA_TYPEINFO_GET

#define EA_TYPEINFO_CUSTOM_IMPL(CLASS) ea::TypeInfo CLASS::TypeInfo()


#define EA_TYPEINFO_SPECIALIZED_DECL(CLASS, TYPE) template<> ea::TypeInfo CLASS<TYPE>::TypeInfo();
#define EA_TYPEINFO_SPECIALIZED_IMPL(CLASS, TYPE) template<> ea::TypeInfo CLASS<TYPE>::TypeInfo()
#define EA_TYPEINFO_SPECIALIZED_INLINE(CLASS, TYPE) template<> inline ea::TypeInfo CLASS<TYPE>::TypeInfo()

#define EA_TYPEINFO_SPECIALIZED_DEFAULT(ALIAS)\
	template<>\
	inline ea::TypeInfo ALIAS::TypeInfo() {\
		return *ea::TypeInfo(string(#ALIAS)).SetConstructor<ALIAS>();\
	}

} /* namespace ea */


