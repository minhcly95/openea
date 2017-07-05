/*
 * PermutationInitializer.cpp
 *
 *  Created on: Apr 6, 2017
 *      Author: Kiet Vo
 */

#include "../pch.h"
#include "PermutationInitializer.h"
#include "../genome/PermutationGenome.h"
#include "../core/pool/GenomePool.h"

namespace ea {

EA_TYPEINFO_CUSTOM_IMPL(PermutationInitializer) {
	return *ea::TypeInfo("PermutationInitializer")
		.SetConstructor<PermutationInitializer, uint>("length");
}

/**
 * @class PermutationInitializer
 * A random Initializer for PermutationGenome.
 * This Initializer will create Genome of type PermutationGenome with random order for a given array length.
 *
 * @name{PermutationInitializer}
 *
 * @eaml
 * @attr{length, uint - Required - The length of generated PermutationGenome.}
 * @endeaml
 */

/**
 * Create a PermutationInitializer with the given array length.
 * @param pLength The length of generated PermutationGenome.
 */
PermutationInitializer::PermutationInitializer(uint pLength) :
		mLength(pLength) {
}

PermutationInitializer::~PermutationInitializer() {
}

GenomePoolPtr PermutationInitializer::DoInitialize(uint pSize) {
	GenomePoolPtr pool = make_shared<GenomePool>();
	for (uint i = 0; i < pSize; i++) {
		PermutationGenomePtr newGenome = make_shared<PermutationGenome>();
		vector<uint>& genes = newGenome->GetGenes();
		genes.resize(mLength);

		iota(genes.begin(), genes.end(), 0);
		shuffle(genes.begin(), genes.end(), Random::generator);

		pool->push_back(static_pointer_cast<Genome>(newGenome));
	}
	return pool;
}

}
/* namespace ea */
