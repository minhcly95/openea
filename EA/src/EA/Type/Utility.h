/*
 * Utility.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../../Common.h"
#include "Core.h"

namespace ea {

DEFINE_PTR_TYPE(MetaRecombinator)
DEFINE_PTR_TYPE(MetaMutator)
DEFINE_PTR_TYPE(ResizableSelector)
DEFINE_PTR_TYPE(IndividualEvaluator)

DEFINE_PTR_TYPE(Constructible)
DEFINE_PTR_TYPE(Storable)
DEFINE_PTR_TYPE(EAMLReader)
DEFINE_PTR_TYPE(ConfigFileException)
DEFINE_PTR_TYPE(TypeInfo)

DEFINE_PTR_TYPE_WITH_TEMPLATE(Randomizer)
DEFINE_PTR_TYPE_TEMPLATE_PACK(Randomizer)

DEFINE_PTR_TYPE(FunctionalEvaluator)
DEFINE_PTR_TYPE_WITH_TEMPLATE(TypedScalarEvaluator)
DEFINE_PTR_TYPE_WITH_TEMPLATE(TypedFunctionalEvaluator)

DEFINE_PTR_TYPE(PhenomeDecoder)
DEFINE_PTR_TYPE(PhenomeEvaluator)

DEFINE_PTR_TYPE(ScalarFitness)
DEFINE_PTR_TYPE(PoweredFitness)

DEFINE_PTR_TYPE(StandardOutputHook);
DEFINE_PTR_TYPE(FitnessTerminationHook);
DEFINE_PTR_TYPE(GenerationTerminationHook);
DEFINE_PTR_TYPE(EvaluationTerminationHook);
DEFINE_PTR_TYPE(BackupHook);
DEFINE_PTR_TYPE(RealTimeInfoHook);
DEFINE_PTR_TYPE(FitnessReportHook);
DEFINE_PTR_TYPE(CyclicHook);

DEFINE_PTR_TYPE_WITH_TEMPLATE(TypedRecombinator)

template <class InputT, class OutputT> class ClusterComputable;
template <class InputT, class OutputT> using ClusterComputablePtr = Ptr<ClusterComputable<InputT, OutputT>>;

}
