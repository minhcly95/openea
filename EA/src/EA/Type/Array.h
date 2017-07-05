/*
 * Array.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../Common.h"
#include "Core.h"

namespace ea {

DEFINE_PTR_TYPE_WITH_TEMPLATE(ArrayGenome)
DEFINE_PTR_TYPE_TEMPLATE_PACK(ArrayGenome)

DEFINE_PTR_TYPE_WITH_TEMPLATE(RandomArrayInitializer)
DEFINE_PTR_TYPE_TEMPLATE_PACK(RandomArrayInitializer)

DEFINE_PTR_TYPE_WITH_TEMPLATE(PointResetMutation)
DEFINE_PTR_TYPE_FROM_TEMPLATE(FlipBitMutation, PointResetMutation, bool)
DEFINE_PTR_TYPE_TEMPLATE_PACK(PointResetMutation)

#ifndef DOXYGEN_IGNORE
template <class T, uint ... N> class NPointCrossover;
template <class T, uint ... N> using NPointCrossoverPtr = Ptr<NPointCrossover<T, N...>>;
#endif

template <class T> class NPointCrossover<T>;
DEFINE_PTR_TYPE_TEMPLATE_PACK(NPointCrossover)

template <class T, uint N> class NPointCrossover<T, N>;

template <class T> using OnePointCrossover = NPointCrossover<T, 1>;
template <class T> using OnePointCrossoverPtr = NPointCrossoverPtr<T, 1>;
DEFINE_PTR_TYPE_TEMPLATE_PACK(OnePointCrossover)

template <class T> using TwoPointCrossover = NPointCrossover<T, 2>;
template <class T> using TwoPointCrossoverPtr = NPointCrossoverPtr<T, 2>;
DEFINE_PTR_TYPE_TEMPLATE_PACK(TwoPointCrossover)

DEFINE_PTR_TYPE_WITH_TEMPLATE(UniformCrossover)
DEFINE_PTR_TYPE_TEMPLATE_PACK(UniformCrossover)

}
