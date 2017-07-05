/*
 * TestConstructibles.h
 *
 *  Created on: Mar 27, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA.h"
#include "../core/TestInterfaces.h"

namespace ea {

namespace test {

DEFINE_PTR_TYPE(Servant1Constructible)
DEFINE_PTR_TYPE(Servant2Constructible)
DEFINE_PTR_TYPE(MasterConstructible)
DEFINE_PTR_TYPE(DoubleRegAttrConstructible)
DEFINE_PTR_TYPE(DoubleRegElemConstructible)

class Servant1Constructible: public Constructible {
public:
	EA_TYPEINFO_DEFAULT(Servant1Constructible)};

class Servant2Constructible: public Constructible {
public:
	EA_TYPEINFO_DEFAULT(Servant2Constructible)};

class MasterConstructible: public TestEvaluator {
private:
	int mAttr;
	Servant1ConstructiblePtr mServant;

public:
	inline MasterConstructible(int pAttr, Servant1ConstructiblePtr pServant) :
			mAttr(pAttr), mServant(pServant) {
	}
	inline virtual ~MasterConstructible() {
	}

	EA_TYPEINFO_CUSTOM_INLINE{
	return *ea::TypeInfo("MasterConstructible")
		.SetConstructor<MasterConstructible, int, Servant1ConstructiblePtr>("attr", "servant");
}
EA_TYPEINFO_GET

inline int GetAttr() const {
	return mAttr;
}

inline const Servant1ConstructiblePtr& GetServant() const {
	return mServant;
}
};

class DoubleRegAttrConstructible: public Constructible {
private:
	int mAttr;
	int mAttr2;

public:
	inline DoubleRegAttrConstructible() :
			mAttr(0), mAttr2(0) {
	}
	inline virtual ~DoubleRegAttrConstructible() {
	}

	EA_TYPEINFO_CUSTOM_INLINE{
	return *ea::TypeInfo("DoubleRegAttrConstructible")
	.Add("attr", &DoubleRegAttrConstructible::mAttr)
	->Add("attr", &DoubleRegAttrConstructible::mAttr2)
	->SetConstructor<DoubleRegAttrConstructible>();
}
EA_TYPEINFO_GET
};

class DoubleRegElemConstructible: public Constructible {
private:
	Servant2ConstructiblePtr s1, s2;

public:
	inline DoubleRegElemConstructible() :
			s1(nullptr), s2(nullptr) {
	}
	inline virtual ~DoubleRegElemConstructible() {
	}

	EA_TYPEINFO_CUSTOM_INLINE{
	return *ea::TypeInfo("DoubleRegElemConstructible")
	.Add("servant", &DoubleRegElemConstructible::s1)
	->Add("servant", &DoubleRegElemConstructible::s2)
	->SetConstructor<DoubleRegElemConstructible>();
}
EA_TYPEINFO_GET
};

}
/* namespace test */
} /* namespace ea */

