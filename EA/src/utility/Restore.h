/*
 * RestoreInitializer.h
 *
 *  Created on: Apr 7, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../EA/Type/Core.h"
#include "../core/interface/Initializer.h"
#include <climits>

#pragma once

namespace ea {

class Restore {
public:
	static PopulationPtr FromBackup(string pLocation, ullong pFrom = UINT64_MAX);
};

} /* namespace ea */


