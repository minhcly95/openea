/*
 * ScalarFitness.h
 *
 *  Created on: Apr 24, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../core/interface/Fitness.h"
#include "../rtoc/Constructible.h"

namespace ea {

class ScalarFitness: public Fitness {
private:
	double mValue;
	bool mMaximizer;

public:
	EA_TYPEINFO_DEFAULT(ScalarFitness)

	ScalarFitness(double pValue = 0, bool pMaximizer = true);
	virtual ~ScalarFitness();

	double GetValue() const;
	bool IsMaximizer() const;

	virtual int Compare(const Fitness& other) const;

	virtual ostream& Print(ostream& os) const override;

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;
};

} /* namespace ea */
