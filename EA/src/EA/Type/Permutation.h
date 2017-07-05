/*
 * Permutation.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../Common.h"
#include "Core.h"

namespace ea {

DEFINE_PTR_TYPE(PermutationGenome)

DEFINE_PTR_TYPE(PermutationInitializer)

DEFINE_PTR_TYPE(InsertMutation)
DEFINE_PTR_TYPE(InversionMutation)
DEFINE_PTR_TYPE(ScrambleMutation)
DEFINE_PTR_TYPE(SwapMutation)

DEFINE_PTR_TYPE(OrderCrossover)
DEFINE_PTR_TYPE(CycleCrossover)
DEFINE_PTR_TYPE(EdgeCrossover)
DEFINE_PTR_TYPE(PartiallyMappedCrossover)

}
