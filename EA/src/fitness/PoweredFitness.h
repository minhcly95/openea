/*
 * PoweredFitness.h
 *
 *  Created on: Jul 6, 2017
 *      Author: Bui Quang Minh
 */

#pragma once

#include "ScalarFitness.h"

namespace ea {

class PoweredFitness : public ScalarFitness {
public:
	EA_TYPEINFO_DEFAULT(PoweredFitness)

	PoweredFitness(double pRawValue = 0, double pAlpha = 1, bool pMaximizer = true);
	virtual ~PoweredFitness();

	double GetRawValue() const;

	virtual ostream& Print(ostream& os) const override;

protected:
	virtual void DoSerialize(ostream& pStream) const override;
	virtual void DoDeserialize(istream& pStream) override;

private:
	double mRawValue;
};

} /* namespace ea */
