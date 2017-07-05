/*
 * InformedTerminationHook.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../../pch.h"
#include "InformedTerminationHook.h"
#include "../../Common.h"
#include <locale>

namespace ea {

using namespace std;

/**
 * @class InformedTerminationHook
 * A Hook which tracks the speed an attribute and terminate when it reaches a given target.
 * This hook also provides a command-line tracker which prints out the attribute value,
 * the speed and the estimated remaining time.
 *
 * This class is the base functionality class for GenerationTerminationHook and EvaluationTerminationHook
 * with the number of generations and evaluations as the attributes respectively.
 *
 * Child classes should override the GetValue() to provide the value of the attribute.
 */

/**
 * @fn ullong InformedTerminationHook::GetValue()
 * Return the attribute value (need to be overridden in child class).
 * The implementation of this function should return the value of the wanted attribute.
 * @return The attribute value.
 */

/**
 * Create an InformedTerminationHook with the target attribute value.
 * @param pTarget The target value. The Hook will terminate the process when the attribute value is greater or equal to this value.
 * @param pInform Whether the tracker will be shown or not.
 * @param pUnit The unit name of the attribute (e.g. "generation")
 * @param pShort The sort name of the attribute with first letter cappital (e.g. "Gen" for generation)
 * @param pChar The character symbolizing the attribute (e.g. "g" for generation, "e" for evaluation)
 */
InformedTerminationHook::InformedTerminationHook(ullong pTarget, bool pInform,
		string pUnit, string pShort, string pChar) :
		mInform(pInform), mUnit(pUnit), mShort(pShort), mChar(pChar), mTarget(
				pTarget), mStartValue(0), mSpeed(0), mLastValue(0), mLastTime() {
}

InformedTerminationHook::~InformedTerminationHook() {
}

void InformedTerminationHook::UpdateSpeed() {
	auto current = chrono::high_resolution_clock::now();
	ullong currentValue = GetValue();

	ullong deltaValue = currentValue - mLastValue;
	auto duration = chrono::duration_cast<chrono::duration<float>>(
			current - mLastTime);

	float currentSpeed = deltaValue / duration.count();
	mSpeed = 0.9 * mSpeed + 0.1 * currentSpeed;

	mLastTime = current;
	mLastValue = currentValue;
}

void DisplayDuration(chrono::duration<float> duration) {
	int level = 0;

	auto hours = chrono::duration_cast<chrono::hours>(duration);
	if (hours.count() != 0) {
		cout << hours.count() << "h ";
		level++;
	}
	duration -= hours;

	auto minutes = chrono::duration_cast<chrono::minutes>(duration);
	if (minutes.count() != 0 || level > 0) {
		cout << setfill('0') << setw(2) << minutes.count() << "m ";
		level++;
	}

	if (level >= 2)
		return;
	duration -= minutes;
	auto seconds = chrono::duration_cast<chrono::seconds>(duration);
	cout << setfill('0') << setw(2) << seconds.count() << "s";
}

void InformedTerminationHook::Display() {
	float perc = float(GetValue() - mStartValue) * 100
			/ (mTarget - mStartValue);
	cout << mShort << " ";

	cout.imbue(std::locale("en_US.UTF-8"));
	ostringstream oss;
	oss.imbue(std::locale("en_US.UTF-8"));
	oss << mTarget;
	string sTarget = oss.str();

	cout << setfill(' ') << setw(sTarget.length()) << GetValue();
	cout << " / " << sTarget;
	cout.imbue(std::locale());

	cout << "    ";
	cout << fixed << setprecision(1) << setw(4) << perc << "%";

	cout << "    Spd: ";
	if (mSpeed >= 10000 || mSpeed < 0.001)
		cout << scientific << setprecision(2);
	else
		cout << fixed << setprecision(3);
	cout << setw(8) << mSpeed << " " << mChar << "/s";

	cout << "    ETA: ";
	DisplayDuration(chrono::duration<float>((mTarget - GetValue()) / mSpeed));

	cout << flush;
	cout << setfill(' ');
	cout << "\e[2K\r";
}

void InformedTerminationHook::DoStart() {
	if (mInform) {
		mLastValue = mStartValue = GetValue();
		mLastTime = chrono::high_resolution_clock::now();
	}
}

void InformedTerminationHook::DoGenerational() {
	if (mInform) {
		UpdateSpeed();
		Display();
	}
	if (GetValue() >= mTarget) {
		EA_LOG_DEBUG << GetTypeName() << ": Reached " << mUnit << " #" << GetValue()
		<< " >= " << mTarget << ". Terminating..." << flush;
		Terminate();
	}
}

}
/* namespace ea */
