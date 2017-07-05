/*
 * OrganismPool.cpp
 *
 *  Created on: Mar 31, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "OrganismPool.h"

#include "../../pch.h"
#include "../Organism.h"
#include "GenomePool.h"

using namespace std;

namespace ea {

/**
 * @class OrganismPool
 * Data structure which contains a list of Organism.
 * This class inherits directly from vector<OrganismPtr> to provide several useful functions
 * such as Sort() and Shuffle() while keeping most of functions of class vector.
 */

/**
 * @fn OrganismPool::OrganismPool()
 * Default constructor.
 * Create an empty OrganismPool.
 */

/**
 * @fn OrganismPool::OrganismPool(uint count)
 * Create a OrganismPool with the given size.
 * The OrganismPool only reserves space for future uses. The entries in the OrganismPool only contain nullptr.
 * @param count The size to be reserved.
 */

/**
 * @fn OrganismPool::OrganismPool(InputIt first, InputIt last)
 * Create a OrganismPool from the given container.
 * This function is the same with its vector's counterpart.
 * @tparam InputIt The iterator type of the container.
 * @param first The begin iterator of the container.
 * @param last The end iterator of the container.
 */

/**
 * Sort the OrganismPool.
 * After calling this function, the OrganismPool will be sorted with the best Organism at first position.
 * The order of sorting is decided by the type of Fitness contained in the member Organism.
 */
void OrganismPool::Sort() {
	sort(this->begin(), this->end(),
			[](const OrganismPtr& o1, const OrganismPtr& o2) {
				return *o1 > *o2;
			});
}

/**
 * Shuffle the OrganismPool.
 * After calling this function, the OrganismPool will be shuffled in random order. The random generating engine is picked
 * from Random::generator.
 */
void OrganismPool::Shuffle() {
	shuffle(this->begin(), this->end(), Random::generator);
}

/**
 * Extract the underlying GenomePool.
 * This function will extract the Genome from each Organism in the pool and put it in a new GenomePool.
 * @return The GenomePool.
 */
GenomePoolPtr OrganismPool::Extract() {
	GenomePoolPtr genomes = make_shared<GenomePool>(size());
	transform(begin(), end(), genomes->begin(), [] (OrganismPtr o) {
		return o->GetGenome();
	});
	return genomes;
}

/**
 * Join a list of OrganismPool into one.
 * After joining, the size of the returned OrganismPool is the sum of the size of elements.
 * @param pPools The list of OrganismPool.
 * @return The joined OrganismPool.
 */
OrganismPoolPtr ea::OrganismPool::Join(vector<OrganismPoolPtr> pPools) {
	OrganismPoolPtr aggregated = make_shared<OrganismPool>();
	for (auto pool : pPools)
		aggregated->insert(aggregated->end(), pool->begin(), pool->end());
	return aggregated;
}

void OrganismPool::DoSerialize(ostream& pStream) const {
	Write(pStream, vector<OrganismPtr>(*this));
}

void OrganismPool::DoDeserialize(istream& pStream) {
	Read<vector<OrganismPtr>>(pStream, *this);
}

}	// namespace ea
