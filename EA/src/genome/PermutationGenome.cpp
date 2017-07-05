/*
 * PermutationGenome.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#include "../pch.h"
#include "PermutationGenome.h"

namespace ea {

/**
 * @class PermutationGenome
 * Genome which stores a permutation of integers.
 *
 * This genome type uses a vector of type uint as the representation. The value ranges from 0 to the given size minus 1.
 * A value can only occur once. This class re-uses the functionality of ArrayGenome by inheriting ArrayGenomeBase class.
 *
 * Functionality of this class is provided by ArrayGenomeBase class. Please check GetSize() and GetGenes().
 *
 * As described in ArrayGenome,
 * it is not recommended that users build a vector outside than input it to the PermutationGenome by using the constructor.
 * Users should create an empty PermutationGenome using the default constructor, then access the vector by calling GetGenes()
 * to get the reference of the contained vector. By this method, the vector will be built in-place.
 *
 * The operator working on PermutationGenome must preserve the permutation property. A function IsValid() is also provided,
 * but won't be called automatically.
 *
 * @name{PermutationGenome}
 */

/**
 * Create an PermutationGenome with content copied from the given vector of genes.
 * @param pGenes A vector of genes of type T.
 */
PermutationGenome::PermutationGenome(vector<uint>& pGenes) :
		ArrayGenomeBase(pGenes) {
}
/**
 * Create an PermutationGenome with size = 0.
 */
PermutationGenome::PermutationGenome() :
		ArrayGenomeBase() {
}

PermutationGenome::~PermutationGenome() {
}

/**
 * Check the validity of the PermutationGenome.
 * A PermutationGenome should maintain its permutation property. However this function won't be called automatically.
 *
 * @method
 * Using an array of boolean to check existence of values.
 *
 * @time
 * O(N)
 *
 * @return false if permutation property is violated (values are out of range or a value has occurred more than once).
 */
bool PermutationGenome::IsValid() const {
	vector<bool> exist(mGenes.size(), false);

	for (uint i = 0; i < mGenes.size(); i++) {
		uint value = mGenes[i];
		if (value >= mGenes.size())
			return false;

		if (exist[value])
			return false;
		exist[value] = true;
	}
	return true;
}

ostream& PermutationGenome::Print(ostream& os) const {
	if (GetSize() == 0)
		os << "<Empty>";
	else
		os << mGenes[0];

	for (uint i = 1; i < GetSize(); i++)
		os << "-" << mGenes[i];
	return os;
}

}
/* namespace ea */
