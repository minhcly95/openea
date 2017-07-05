/*
 * Server.cpp
 *
 *  Created on: May 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "CLIOptions.h"

namespace ea {
namespace cli {

using namespace std;

function<void(void)> Server(istringstream& iss) {
	ullong port = ExtractNumber(iss, "<port>");

	return [port] () {
		CommandLineInterface::Register([port] (StrategyPtr& strategy) {
					uint count = 0;
					while (auto serverHook = strategy->hooks.Remove<RealTimeInfoHook>()) {
						count++;
					}
					if (count > 0)
						EA_LOG_DEBUG << "CLI: Overwrote " << count << " RealTimeInfoHook(s) because of -s option.";
					strategy->hooks.Create<RealTimeInfoHook>(port);
				});
	};
}

}
}

