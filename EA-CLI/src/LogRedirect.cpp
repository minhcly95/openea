/*
 * LogRedirect.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Bui Quang Minh
 */

#include <pch.h>
#include "CLIOptions.h"

namespace ea {
namespace cli {

using namespace std;

function<void(void)> LogRedirect(istringstream& iss) {
	char chr;
	bool lvl[Log::SeverityLevel_MAX + 1] = { false };

#define TURN_ON(LEVEL)\
	if (lvl[Log::LEVEL]) THROW; else lvl[Log::LEVEL] = true

	bool sign = false;
	bool add = false;
	bool level = false;
	while (iss >> chr) {
		if (chr == '=' || chr == '+') {
			if (!level)
				throw "<level> was not specified";
			sign = true;
			add = chr == '+';
			break;
		}
		level = true;
		switch (chr) {
		case 'e':
			TURN_ON(ERROR);
			break;
		case 'i':
			TURN_ON(INFO);
			break;
		case 'd':
			TURN_ON(DEBUG);
			break;
		case 't':
			TURN_ON(TRACE);
			break;
		case 'a':
			TURN_ON(ERROR);
			TURN_ON(INFO);
			TURN_ON(DEBUG);
			TURN_ON(TRACE);
			break;
		default:
			throw "Invalid level";
		}
	}

	vector<function<void(void)>> funcs;
	if (!sign) {
		for (int i = 0; i <= Log::SeverityLevel_MAX; i++) {
			if (lvl[i]) {
				funcs.push_back(
						bind(Log::Redirect, Log::SeverityLevel(i),
								(i == Log::ERROR) ? "stderr" : "stdout"));
			}
		}
	} else {
		string path;
		getline(iss, path);
		for (int i = 0; i <= Log::SeverityLevel_MAX; i++)
			if (lvl[i])
				funcs.push_back(
						bind(add ? Log::Add : Log::Redirect, Log::SeverityLevel(i), path));
	}

	return [funcs] () {
		for (auto f : funcs)
		f();
	};
}

}
}
