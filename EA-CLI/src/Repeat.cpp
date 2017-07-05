/*
 * Backup.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Bui Quang Minh
 */

#include <pch.h>
#include "CLIOptions.h"

namespace ea {
namespace cli {

using namespace std;

function<void(void)> Repeat(istringstream& iss) {
	string token;
	uint times = 0;

	if (!getline(iss, token, '='))
		THROW;

	if (!iss.eof()) {
		if (token != "") {
			try {
				istringstream issToken(token);
				issToken >> times;
				if (issToken.rdbuf()->in_avail())
					throw "<times> must be an integer";
				if (times <= 1)
					throw "<times> must be greater than 1";
			} catch (exception& e) {
				THROW;
			}
		}
		if (!getline(iss, token))
			THROW;
	} else
		throw "both <times> and <root> is required";

	return [token, times] () {
		CommandLineInterface::Repeat(times, token);
	};
}

}
}

