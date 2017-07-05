/*
 * PluginLoader.cpp
 *
 *  Created on: Apr 13, 2017
 *      Author: Bui Quang Minh
 */

#include <pch.h>
#include <dlfcn.h>
#include "CommandLineInterface.h"
#include "PluginLoader.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <unistd.h>
#include <fstream>

#define PLUGIN_CONF "plugin.conf"

namespace fs = boost::filesystem;
using namespace std;

namespace ea {
namespace cli {

vector<void*> PluginLoader::sLoadedLib;

int PluginLoader::LoadSharedLib(fs::path execDir, string line) {
	boost::algorithm::trim(line);

	if (line == "")
		return 0;

	fs::path libPath(line);
	if (libPath.is_relative())
		libPath = execDir / libPath;

	if (!fs::is_regular_file(libPath)) {
		EA_LOG_ERROR << "PluginLoader: " << line
				<< " does not exist. Skip this plugin." << flush;
		return 0;
	}

	void* dlObj = dlopen(libPath.c_str(), RTLD_LAZY);
	if (!dlObj) {
		EA_LOG_ERROR << "PluginLoader: Failed to open " << line << " : "
				<< dlerror() << flush;
		sLoadedLib.push_back(dlObj);
		return 0;
	}

	using BootstrapperType = void (*)();
	void* bootstrapper = dlsym(dlObj, "EABootstrapper");
	if (!bootstrapper)
		EA_LOG_ERROR << "PluginLoader: Failed to get bootstrapper of "
				<< line << " : " << dlerror() << flush;

	(*BootstrapperType(bootstrapper))();
	return 1;
}

void PluginLoader::Load() {
	fs::path execDir;
	try {
#ifndef EA_CLI_RELEASE
		execDir = CommandLineInterface::QueryExecDir();
#else
		execDir = CommandLineInterface::QueryHomeDir() / ".openea";
#endif
	} catch (system_error& e) {
		EA_LOG_ERROR << "PluginLoader: Error when query the executable file directory." << flush;
	}

#ifndef EA_CLI_RELEASE
	LoadSharedLib(execDir, "./libopenea_addon.so");
#else
	LoadSharedLib(execDir, "./libopenea_addon.so");
#endif

	fs::path pluginConf = execDir / PLUGIN_CONF;

	if (fs::exists(pluginConf)) {
		int count = 0;
		ifstream ifs(pluginConf.c_str());
		while (!ifs.eof()) {
			string line;
			std::getline(ifs, line);
			count += LoadSharedLib(execDir, line);
		}
		EA_LOG_DEBUG << "PluginLoader: Loaded " << count << " plugin(s)." << flush;
	} else
		EA_LOG_ERROR
				<< "PluginLoader: " PLUGIN_CONF " does not exist. Continue without plugins."
				<< flush;
}

void PluginLoader::Unload() {
	for (auto dlObj : sLoadedLib)
		if (dlclose(dlObj))
			EA_LOG_ERROR << "PluginLoader: Failed to close library : " << dlerror()
					<< flush;
}

} /* namespace cli */
} /* namespace ea */
