/*
 * Main.cpp
 *
 *  Created on: Mar 22, 2017
 *      Author: Bui Quang Minh
 */

#include <pch.h>

#ifdef EA_CLI_DEBUG

#define EA_PLUGIN_MAIN_SOURCE
#include "EA-CLI.h"
#include "Add-on.h"

#else
#include "EA-CLI.h"
#endif

using namespace ea::cli;

int main(int argc, char** argv) {
	int code = CommandLineInterface::ProcessOptions(argc, argv);
	if (code) return code;

	PluginLoader::Load();
	code = CommandLineInterface::Run();
	PluginLoader::Unload();

	return code;
}
