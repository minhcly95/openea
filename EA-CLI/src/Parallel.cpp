/*
 * Parallel.cpp
 *
 *  Created on: Apr 16, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "CLIOptions.h"

namespace ea {
namespace cli {

using namespace std;

function<void(void)> Parallel(istringstream& iss) {
	ullong numThreads = ExtractNumber(iss, "<numThreads>");

	return [numThreads] () {
		MultiThreading::SetNumThreads(numThreads);
	};
}

}
}

