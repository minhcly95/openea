/*
 * PhenomePool.cpp
 *
 *  Created on: Jun 29, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../../pch.h"
#include "PhenomePool.h"
#include "../Organism.h"

using namespace std;

namespace ea {

/**
 * @class PhenomePool
 * Data structure which contains a list of Phenome.
 * This class inherits directly from vector<PhenomePtr> to provide several useful functions
 * such as Shuffle() while keeping most of functions of class vector.
 */

/**
 * @fn PhenomePool::PhenomePool()
 * Default constructor.
 * Create an empty PhenomePool.
 */

/**
 * @fn PhenomePool::PhenomePool(uint count)
 * Create a PhenomePool with the given size.
 * The PhenomePool only reserves space for future uses. The entries in the PhenomePool only contain nullptr.
 * @param count The size to be reserved.
 */

/**
 * @fn PhenomePool::PhenomePool(InputIt first, InputIt last)
 * Create a PhenomePool from the given container.
 * This function is the same with its vector's counterpart.
 * @tparam InputIt The iterator type of the container.
 * @param first The begin iterator of the container.
 * @param last The end iterator of the container.
 */

/**
 * Shuffle the PhenomePool.
 * After calling this function, the PhenomePool will be shuffled in random order. The random generating engine is picked
 * from Random::generator.
 */
void PhenomePool::Shuffle() {
	shuffle(this->begin(), this->end(), Random::generator);
}

/**
 * Join a list of PhenomePool into one.
 * After joining, the size of the returned PhenomePool is the sum of the size of elements.
 * @param pPools The list of PhenomePool.
 * @return The joined PhenomePool.
 */
PhenomePoolPtr PhenomePool::Join(vector<PhenomePoolPtr> pPools) {
	PhenomePoolPtr aggregated = make_shared<PhenomePool>();
	for (auto pool : pPools)
		aggregated->insert(aggregated->end(), pool->begin(), pool->end());
	return aggregated;
}

void PhenomePool::DoSerialize(ostream& pStream) const {
	Write(pStream, vector<PhenomePtr>(*this));
}

void PhenomePool::DoDeserialize(istream& pStream) {
	Read(pStream, static_cast<vector<PhenomePtr>&>(*this));
}

}	// namespace ea
