/*
 * NameService.h
 *
 *  Created on: Mar 23, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../EA/Type/Utility.h"
#include <functional>
#include "Constructible.h"
#include "TypeInfo.h"
#include <type_traits>

namespace ea {

using namespace std;

class NameService {
private:
	using NameMap = HashMap<string, const TypeInfo>;
	static NameMap sMap;

	static struct InitStruct {
		InitStruct();
	} __init;

	static void Add(const TypeInfo pTypeInfo);

public:
	static const TypeInfo& Get(string pName);

	template<class T>
	static void Add() {
		static_assert(is_base_of<Constructible, T>::value, "NameService::Add<T>() : The type T must be Constructible.");
		static_assert(!is_base_of<Storable, T>::value || is_default_constructible<T>::value,
				"NameService::Add<T>() : The type T is a Storable but doesn't have a default constructor.");
		NameService::Add(T::TypeInfo());
	}

	static vector<string> GetAllNames();
};

} /* namespace ea */


