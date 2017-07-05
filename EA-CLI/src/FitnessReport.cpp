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

function<void(void)> FitnessReport(istringstream& iss) {
	string token;
	uint gen = 0;

	if (!getline(iss, token, '='))
		THROW;

	if (!iss.eof()) {
		if (token != "") {
			try {
				istringstream issToken(token);
				issToken >> gen;
				if (issToken.rdbuf()->in_avail())
					throw "<num> must be an integer";
			} catch (exception& e) {
				THROW;
			}
		}
		if (!getline(iss, token))
			THROW;
	}

	return [token, gen] () {
		HookPtr hook = make_shared_base(Hook, FitnessReportHook, token, gen, true);
		CommandLineInterface::Register([hook] (StrategyPtr& strategy) {
			strategy->hooks.Add(hook);
		});
	};
}

}
}

