/*
 * ConstructibleFixture.h
 *
 *  Created on: Mar 27, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA.h"
#include "../core/TestInterfaces.h"
#include "TestConstructibles.h"

#define MAX_COUNT 10
#define MAX_RATIO 5

namespace ea {

namespace test {

using namespace std;

#define FORCE_REGISTER(CLASS)\
	try {\
		NameService::Add<CLASS>();\
	} catch (exception&) {\
	}

struct ConstructibleFixture {
	ConstructibleFixture() {
		// Name service register
		FORCE_REGISTER(Servant1Constructible);
		FORCE_REGISTER(Servant2Constructible);
		FORCE_REGISTER(MasterConstructible);
		FORCE_REGISTER(DoubleRegAttrConstructible);
		FORCE_REGISTER(DoubleRegElemConstructible);

		FORCE_REGISTER(TestEvaluator);
		FORCE_REGISTER(TestInitializer);
		FORCE_REGISTER(TestMutator);
		FORCE_REGISTER(TestRecombinator);
		FORCE_REGISTER(TestSelector);
	}
	~ConstructibleFixture() {
	}
};

}	// namespace test

}	// namespace ea


