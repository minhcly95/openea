/*
 * Plugin.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../rtoc/NameService.h"

#pragma once

// Plugin macro
#ifdef EA_PLUGIN_MAIN_SOURCE

#define REGISTER_ADDON(CLASS)\
	struct AddonRegister ## CLASS ## Struct {\
		static struct InitStruct {\
			InitStruct() { ea::NameService::Add<CLASS>(); }\
		} __init;\
	};\
	AddonRegister ## CLASS ## Struct::InitStruct AddonRegister ## CLASS ## Struct::__init;

extern "C" void EABootstrapper() { }

#else
#define REGISTER_ADDON(CLASS)
#endif
