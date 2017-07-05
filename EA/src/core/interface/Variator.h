/*
 * Variator.h
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "../../EA/Type/Core.h"
#include "../../rtoc/Constructible.h"

namespace ea {

class Variator: public Constructible {
public:
	using InputType = GenomePoolPtr;
	using OutputType = GenomePoolPtr;

	virtual ~Variator();

	void SetSession(const SessionPtr& pSession);
	GenomePoolPtr Variate(GenomePoolPtr const& pPool);

	GenomePoolPtr operator ()(const SessionPtr& pSession, const GenomePoolPtr& pGenomePool);

protected:
	virtual GenomePoolPtr DoVariate(const GenomePoolPtr& pPool) = 0;

	const SessionPtr& GetSession() const;

private:
	SessionPtr mSession;
};

} /* namespace ea */
