/*
 * permutation.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */


#pragma once

#include "Type/Permutation.h"

#include "../genome/PermutationGenome.h"

#include "../initializer/PermutationInitializer.h"

#include "../mutator/permutation/InsertMutation.h"
#include "../mutator/permutation/InversionMutation.h"
#include "../mutator/permutation/ScrambleMutation.h"
#include "../mutator/permutation/SwapMutation.h"

#include "../recombinator/permutation/CycleCrossover.h"
#include "../recombinator/permutation/EdgeCrossover.h"
#include "../recombinator/permutation/OrderCrossover.h"
#include "../recombinator/permutation/PartiallyMappedCrossover.h"
