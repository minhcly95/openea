/*
 * PluginLoader.h
 *
 *  Created on: Apr 13, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "Common.h"
#include <boost/filesystem.hpp>

namespace ea {
namespace cli {

namespace fs = boost::filesystem;

class PluginLoader {
public:
	static void Load();
	static void Unload();

private:
	static vector<void*> sLoadedLib;
	static int LoadSharedLib(fs::path execDir, string line);
};

} /* namespace cli */
} /* namespace ea */


