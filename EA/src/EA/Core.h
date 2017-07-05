/*
 * core.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "Type/Core.h"

#include "../core/Organism.h"
#include "../core/Population.h"
#include "../core/Session.h"

#include "../core/Operator.h"
#include "../core/OperatorGroup.h"
#include "../core/SeriesOperatorGroup.h"

#include "../core/interface/Genome.h"
#include "../core/interface/Evaluator.h"
#include "../core/interface/Initializer.h"
#include "../core/interface/Mutator.h"
#include "../core/interface/Recombinator.h"
#include "../core/interface/Selector.h"
#include "../core/interface/Variator.h"
#include "../core/interface/Hook.h"
#include "../core/interface/Fitness.h"
#include "../core/interface/Strategy.h"

#include "../core/pool/GenomePool.h"
#include "../core/pool/OrganismPool.h"
#include "../core/pool/MetaPool.h"
#include "../core/pool/Pool.h"
