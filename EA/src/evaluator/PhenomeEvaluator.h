/*
 * PhenomeEvaluator.h
 *
 *  Created on: Jun 29, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Utility.h"
#include "../core/interface/Evaluator.h"

namespace ea {

class PhenomeDecoder : public Constructible {
public:
	inline virtual ~PhenomeDecoder() { }

	virtual PhenomePtr Decode(const GenomePtr& pGenome) = 0;
};

class PhenomeEvaluator : public Evaluator {
public:
	PhenomeEvaluator(const PhenomeDecoderPtr& pDecoder);
	inline virtual ~PhenomeEvaluator() { }

private:
	PhenomeDecoderPtr mDecoder;

	virtual OrganismPoolPtr DoEvaluate(const PhenomePoolPtr& pPhenomePool) = 0;
	virtual OrganismPoolPtr DoEvaluate(const GenomePoolPtr& pGenomePool) override;
};

}
