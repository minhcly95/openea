/*
 * MultiThreading.h
 *
 *  Created on: May 29, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

namespace ea {

using namespace std;

class MultiThreading {
public:
	static void For(int pFrom, int pTo, function<void(int)> pFunc);

	static uint GetNumThreads();
	static uint GetRealNumThreads();
	static void SetNumThreads(uint pNumThreads);

	static void SetForceEnabled(bool pForced = true);
	static bool IsForceEnabled();

private:
	static uint sNumThreads;
	static bool sForced;
};

} /* namespace ea */
