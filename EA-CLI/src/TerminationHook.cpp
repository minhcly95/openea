/*
 * TerminationHook.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Bui Quang Minh
 */

#include <pch.h>
#include "CLIOptions.h"

namespace ea {
namespace cli {

using namespace std;

ullong ExtractNumber(istringstream& iss, string name) {
	int next = iss.peek();
	if (next == EOF)
		throw name + " is required";
	if (next == '=')
		iss.ignore(1);

	ullong target;
	if (!(iss >> target))
		throw name + " is required";
	if (iss.rdbuf()->in_avail())
		throw name + " must be an integer";

	return target;
}

function<void(void)> TerminateHook(istringstream& iss) {
	char chr;
	HookPtr hook;

	GETCHAR(chr);
	switch (chr) {
	case 'g':
		hook = make_shared_base(Hook, GenerationTerminationHook, ExtractNumber(iss));
		break;
	case 'e':
		hook = make_shared_base(Hook, EvaluationTerminationHook, ExtractNumber(iss));
		break;
	default:
		THROW;
	}

	return [hook] () {
		CommandLineInterface::Register([hook] (StrategyPtr& strategy) {
			uint count = 0;
			while (auto informedHook = strategy->hooks.Find<InformedTerminationHook>()) {
				strategy->hooks.Remove(informedHook);
				count++;
			}
			if (count > 0)
				EA_LOG_DEBUG << "CLI: Overwrote " << count << " TerminationHook(s) because of -t option.";
			strategy->hooks.Add(hook);
		});
	};
}

}
}
