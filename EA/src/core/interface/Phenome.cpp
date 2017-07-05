/*
 * Phenome.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: Bui Quang Minh
 */

#include "../../pch.h"
#include "Phenome.h"
#include "Genome.h"

using namespace std;

namespace ea {

ostream& Phenome::Print(ostream& os) const {
	os << "<Phenome of " << *mGenome << ">";
	return os;
}

const GenomePtr& Phenome::GetGenome() const {
	return mGenome;
}

void Phenome::DoSerialize(ostream& pStream) const {
	Write(pStream, mGenome);
}

void Phenome::DoDeserialize(istream& pStream) {
	mGenome = Read<GenomePtr>(pStream);
}

}	// namespace ea
