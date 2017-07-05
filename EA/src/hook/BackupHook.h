/*
 * BackupHook.h
 *
 *  Created on: Apr 5, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../core/interface/Hook.h"
#include "../rtoc/Constructible.h"

namespace ea {

class BackupHook: public Hook {
private:
	string mDir;
	uint mFrequency;
	bool mClear;

	void CreateBackup();

protected:
	virtual void DoInitial() override;
	virtual void DoGenerational() override;
	virtual void DoEnd() override;

public:
	EA_TYPEINFO_CUSTOM_DECL

	BackupHook(string pDir, uint pFrequency = 0, bool pClear = false);
	virtual ~BackupHook();

	static long long ExtractGenerationNumber(string pFilename);

	bool IsClear() const;
	const string& GetDirectory() const;
	uint GetInterval() const;
};

} /* namespace ea */


