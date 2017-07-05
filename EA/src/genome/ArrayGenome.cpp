/*
 * ArrayGenome.cpp
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "../EA/Type/Array.h"
#include "ArrayGenome.h"

namespace ea {

using namespace std;

#ifndef DOXYGEN_IGNORE
template<>
ostream& ArrayGenome<bool>::Print(ostream& os) const {
	for (uint i = 0; i < GetSize(); i++)
		os << (mGenes[i] ? "1" : "0");
	return os;
}

template<>
ostream& ArrayGenome<int>::Print(ostream& os) const {
	if (GetSize() == 0)
		os << "<Empty>";
	else
		os << mGenes[0];

	for (uint i = 1; i < GetSize(); i++)
		os << "-" << mGenes[i];
	return os;
}

template<>
ostream& ArrayGenome<double>::Print(ostream& os) const {
	os << fixed << setprecision(4);
	if (GetSize() == 0)
		os << "<Empty>";
	else
		os << mGenes[0];

	for (uint i = 1; i < GetSize(); i++)
		os << "-" << mGenes[i];
	return os;
}
#endif

}  // namespace ea
