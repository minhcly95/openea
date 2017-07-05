/*
 * PhenomeEvaluator.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: Bui Quang Minh
 */

#include "../pch.h"
#include "PhenomeEvaluator.h"
#include "../core/pool/GenomePool.h"
#include "../core/pool/PhenomePool.h"

namespace ea {

PhenomeEvaluator::PhenomeEvaluator(const PhenomeDecoderPtr& pDecoder) :
		mDecoder(pDecoder) {
}

OrganismPoolPtr PhenomeEvaluator::DoEvaluate(const GenomePoolPtr& pGenomePool) {
	PhenomePoolPtr phenomePool = make_shared<PhenomePool>(pGenomePool->size());
	transform(pGenomePool->begin(), pGenomePool->end(), phenomePool->begin(), [&] (const GenomePtr& genome) {
		return mDecoder->Decode(genome);
	});
	return DoEvaluate(phenomePool);
}

}
