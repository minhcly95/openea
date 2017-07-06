/*
 * PoweredFitness.cpp
 *
 *  Created on: Jul 6, 2017
 *      Author: Bui Quang Minh
 */

#include "PoweredFitness.h"

namespace ea {

PoweredFitness::PoweredFitness(double pRawValue, double pAlpha,
		bool pMaximizer) : ScalarFitness(pow(pRawValue, pAlpha), pMaximizer), mRawValue(pRawValue) {
}

PoweredFitness::~PoweredFitness() {
}

void PoweredFitness::DoSerialize(ostream& pStream) const {
	ScalarFitness::DoSerialize(pStream);
	Write(pStream, mRawValue);
}

void PoweredFitness::DoDeserialize(istream& pStream) {
	ScalarFitness::DoDeserialize(pStream);
	mRawValue = Read<double>(pStream);
}

double PoweredFitness::GetRawValue() const {
	return mRawValue;
}

ostream& PoweredFitness::Print(ostream& os) const {
	os << GetValue() << " (Raw: " << mRawValue << ")";
	return os;
}

} /* namespace ea */
