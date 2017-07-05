/*
 * FitnessReportHook.h
 *
 *  Created on: Jun 13, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../Common.h"
#include "../core/interface/Hook.h"
#include "../rtoc/Constructible.h"
#include <fstream>

namespace ea {

using namespace std;

class FitnessReportHook : public Hook {
public:
	EA_TYPEINFO_CUSTOM_DECL

	FitnessReportHook(string pFileName, uint pFrequency = 0, bool pOverride = false);
	virtual ~FitnessReportHook();

protected:
	virtual void DoInitial() override;
	virtual void DoGenerational() override;
	virtual void DoEnd() override;

private:
	string mFileName;
	uint mFrequency;
	bool mOverride;
	fstream fs;

	void CreateEntry();
};

} /* namespace ea */
