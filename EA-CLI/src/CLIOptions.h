/*
 * CLIOptions.h
 *
 *  Created on: Apr 6, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "CommandLineInterface.h"
#include <sstream>
#include <functional>
#include "Common.h"

#define THROW throw ""
#define GETCHAR(CHAR) if (!(iss >> CHAR)) THROW;
#define DEFAULT_BACKUP_FOLDER ".backup"

namespace ea {
namespace cli {

using namespace std;

void PrintHelp();
ullong ExtractNumber(istringstream& iss, string name = "<target>");

function<void(void)> LogRedirect(istringstream& iss);
function<void(void)> TerminateHook(istringstream& iss);
function<void(void)> Backup(istringstream& iss);
function<void(void)> Restore(istringstream& iss);
function<void(void)> Parallel(istringstream& iss);
function<void(void)> Server(istringstream& iss);
function<void(void)> FitnessReport(istringstream& iss);
function<void(void)> Repeat(istringstream& iss);

}
}


