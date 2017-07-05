/*
 * Evaluator.h
 *
 *  Created on: May 27, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../rtoc/Constructible.h"

namespace ea {

class Evaluator: public Constructible {
public:
	using InputType = GenomePoolPtr;
	using OutputType = OrganismPoolPtr;

	virtual ~Evaluator();

	void SetSession(const SessionPtr& pSession);
	OrganismPoolPtr Evaluate(const GenomePoolPtr& pGenomePool);

	OrganismPoolPtr operator() (const SessionPtr& pSession, const GenomePoolPtr& pGenomePool);

protected:
	virtual OrganismPoolPtr DoEvaluate(const GenomePoolPtr& pGenomePool) = 0;
	void IncreaseEvaluationCount();
	const SessionPtr& GetSession() const;

private:
	SessionPtr mSession;
};

} /* namespace ea */
