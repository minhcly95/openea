/*
 * Organism.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../rtoc/Storable.h"

namespace ea {

using namespace std;

class Organism final: public Storable {
public:
	Organism(GenomePtr const& pGenome = nullptr, FitnessPtr const& pFitness = nullptr);
	virtual ~Organism();

	FitnessPtr GetFitness() const;
	double GetFitnessValue() const;
	const GenomePtr& GetGenome() const;

	bool operator <(const Organism& pOther);
	bool operator >(const Organism& pOther);

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;

private:
	GenomePtr mGenome;
	FitnessPtr mFitness;
};

} /* namespace ea */
