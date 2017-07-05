/*
 * TypeInfo.h
 *
 *  Created on: Apr 18, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"
#include "UnifiedData.h"

namespace ea {

using namespace std;

class TypeInfo {
public:
	TypeInfo(string pTypeName);
	virtual ~TypeInfo();

	string GetTypeName() const;
	using ConstructiblePtrRef = const ConstructiblePtr&;
	using UnifiedDataRef = const UnifiedData&;
	using UnifiedMap = HashMap<string, UnifiedData>;

private:
	using UnifiedSetter = function<void(ConstructiblePtrRef, UnifiedDataRef)>;

	void Add(string pName, UnifiedSetter pSetter);

	inline const string SetterMsgString(string pName) {
		return "Setter for " + GetTypeName() + ":" + pName;
	}

	using ConstructorCaller = function<ConstructiblePtr(UnifiedMap&)>;

	void SetConstructor(ConstructorCaller pCaller);

	template<class T>
	inline static T GetFromMap(UnifiedMap& uniMap, string pName) {
		return uniMap.at(pName).Get<T>();
	}

public:
	template<class MemberT, class ClassT>
	inline TypeInfo* Add(string pName, MemberT ClassT::*pMember) {
		Add(pName,
				[this, pName, pMember](ConstructiblePtrRef obj, UnifiedDataRef value) {
					Ptr<ClassT> castedObj = dynamic_pointer_cast<ClassT>(obj);
					if (!castedObj)
					throw EA_EXCEPTION(RTOCException, WRONG_OBJ_TYPE,
							SetterMsgString(pName) + ": Cannot cast object into " + GetTypeName() + ".");

					castedObj.get()->*pMember = value.Get<MemberT>();
				});
		return this;
	}

	template<class T>
	struct StringWrapper {
		using type = string;
	};

	template<class T, class ... Args>
	inline TypeInfo* SetConstructor(typename StringWrapper<Args>::type ... pNames) {
		string name = mTypeName;
		SetConstructor(ConstructorCaller(
				[name, pNames...] (UnifiedMap& uniMap) {
					try {
						Ptr<T> obj = make_shared<T>(GetFromMap<Args>(uniMap, pNames)...);

						vector<string> vNames { pNames... };
						for (string key : vNames)
							uniMap.erase(key);

						return static_pointer_cast<Constructible>(obj);
					} catch (out_of_range& o) {
						throw EA_EXCEPTION(RTOCException, FIELD_MISSING,
								"Constructor of " + name + ": Missing attributes or elements.");
					}
				}));
		return this;
	}

	ConstructiblePtr Construct(const UnifiedMap& pData = { }) const;
	bool Set(string pName, ConstructiblePtrRef pObj, UnifiedDataRef pData) const;

private:
	string mTypeName;

	using UnifiedSetterMap = HashMap<string, UnifiedSetter>;

	UnifiedSetterMap mSetterMap;
	ConstructorCaller mConstructor;
};

} /* namespace ea */
