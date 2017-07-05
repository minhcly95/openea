/*
 * InformedTerminationHook.h
 *
 *  Created on: Apr 6, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../core/interface/Hook.h"
#include <chrono>

namespace ea {

class InformedTerminationHook: public Hook {
public:
	InformedTerminationHook(ullong pTarget, bool pInform, string pUnit,
			string pShort, string pChar);
	virtual ~InformedTerminationHook();

protected:
	bool mInform;

	virtual ullong GetValue() = 0;

	virtual void DoStart() override;
	virtual void DoGenerational() override;

private:
	string mUnit, mShort, mChar;

	ullong mTarget;
	ullong mStartValue;
	float mSpeed;

	ullong mLastValue;
	chrono::time_point<chrono::high_resolution_clock> mLastTime;

	void UpdateSpeed();
	void Display();
};

} /* namespace ea */


