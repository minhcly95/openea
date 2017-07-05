/*
 * GenomePool.cpp
 *
 *  Created on: May 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "GenomePool.h"

#include "../../pch.h"
#include "../Organism.h"

using namespace std;

namespace ea {

/**
 * @class GenomePool
 * Data structure which contains a list of Genome.
 * This class inherits directly from vector<GenomePtr> to provide several useful functions
 * such as Shuffle() while keeping most of functions of class vector.
 */

/**
 * @fn GenomePool::GenomePool()
 * Default constructor.
 * Create an empty GenomePool.
 */

/**
 * @fn GenomePool::GenomePool(uint count)
 * Create a GenomePool with the given size.
 * The GenomePool only reserves space for future uses. The entries in the GenomePool only contain nullptr.
 * @param count The size to be reserved.
 */

/**
 * @fn GenomePool::GenomePool(InputIt first, InputIt last)
 * Create a GenomePool from the given container.
 * This function is the same with its vector's counterpart.
 * @tparam InputIt The iterator type of the container.
 * @param first The begin iterator of the container.
 * @param last The end iterator of the container.
 */

/**
 * Shuffle the GenomePool.
 * After calling this function, the GenomePool will be shuffled in random order. The random generating engine is picked
 * from Random::generator.
 */
void GenomePool::Shuffle() {
	shuffle(this->begin(), this->end(), Random::generator);
}

/**
 * Join a list of GenomePool into one.
 * After joining, the size of the returned GenomePool is the sum of the size of elements.
 * @param pPools The list of GenomePool.
 * @return The joined GenomePool.
 */
GenomePoolPtr GenomePool::Join(vector<GenomePoolPtr> pPools) {
	GenomePoolPtr aggregated = make_shared<GenomePool>();
	for (auto pool : pPools)
		aggregated->insert(aggregated->end(), pool->begin(), pool->end());
	return aggregated;
}

void GenomePool::DoSerialize(ostream& pStream) const {
	Write(pStream, vector<GenomePtr>(*this));
}

void GenomePool::DoDeserialize(istream& pStream) {
	Read(pStream, static_cast<vector<GenomePtr>&>(*this));
}

}	// namespace ea
