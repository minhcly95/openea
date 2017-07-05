/*
 * Core.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../Common.h"

namespace ea {

DEFINE_PTR_TYPE(Genome)
DEFINE_PTR_TYPE(Organism)
DEFINE_PTR_TYPE(Recombinator)
DEFINE_PTR_TYPE(Mutator)
DEFINE_PTR_TYPE(Selector)
DEFINE_PTR_TYPE(Variator)
DEFINE_PTR_TYPE(Evaluator)
DEFINE_PTR_TYPE(Initializer)
DEFINE_PTR_TYPE(Population)
DEFINE_PTR_TYPE(Pool)
DEFINE_PTR_TYPE(GenomePool)
DEFINE_PTR_TYPE(OrganismPool)
DEFINE_PTR_TYPE(MetaPool)
DEFINE_PTR_TYPE(Hook)
DEFINE_PTR_TYPE(Fitness)
DEFINE_PTR_TYPE(Strategy)
DEFINE_PTR_TYPE(Session)
DEFINE_PTR_TYPE_WITH_TEMPLATE(Operator)
DEFINE_PTR_TYPE_WITH_TEMPLATE(OperatorGroup)
DEFINE_PTR_TYPE_WITH_TEMPLATE(SeriesOperatorGroup)

}
