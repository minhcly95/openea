/*
 * Common.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#define BOOST_ALL_DYN_LINK

#include <memory>
#include <vector>
#include <stdexcept>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>
#include <map>
#include <unordered_map>
#include <string>
#include <type_traits>

namespace ea {

using uint = unsigned int;
using llong = long long;
using ullong = unsigned long long;

template<typename T>
using Ptr = std::shared_ptr<T>;

template<typename T>
using VectorPtr = Ptr<std::vector<T>>;

template<typename Key, typename Value>
using HashMap = std::unordered_map<Key, Value>;
template<typename Key, typename Value>
using HashMapPtr = Ptr<HashMap<Key, Value>>;

// Initialize helper
#define make_shared_base(BASE, DERIVED, ...)\
		std::static_pointer_cast<BASE>(std::make_shared<DERIVED>(__VA_ARGS__))

// Pointer alias macro
#define DEFINE_PTR_TYPE(CLASS)\
	class CLASS;\
	using CLASS ## Ptr = ea::Ptr<CLASS>;

#define DEFINE_PTR_TYPE_WITH_TEMPLATE(CLASS)\
	template <class T> class CLASS;\
	template <class T> using CLASS ## Ptr = ea::Ptr<CLASS<T>>;

#define DEFINE_PTR_TYPE_FROM_TEMPLATE(CLASS, TCLASS, T)\
	using CLASS = TCLASS<T>;\
	using CLASS ## Ptr = ea::Ptr<CLASS>;

#define DEFINE_PTR_TYPE_TEMPLATE_PACK(TCLASS)\
	DEFINE_PTR_TYPE_FROM_TEMPLATE(Bool ## TCLASS, TCLASS, bool)\
	DEFINE_PTR_TYPE_FROM_TEMPLATE(Int ## TCLASS, TCLASS, int)\
	DEFINE_PTR_TYPE_FROM_TEMPLATE(Double ## TCLASS, TCLASS, double)

#define NOP_MACRO(...)
#define GET_MACRO_2(_1, _2, NAME, ...) NAME
#define GET_MACRO_4(_1, _2, _3, _4, NAME, ...) NAME

} /* namespace ea */

#include "misc/EAException.h"
#include "misc/Log.h"
#include "misc/MultiThreading.h"
#include "misc/Cluster.h"
#include "misc/Random.h"


