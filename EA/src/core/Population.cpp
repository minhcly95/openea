/*
 * Population.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "Population.h"
#include "../Common.h"
#include "pool/GenomePool.h"
#include "pool/OrganismPool.h"

using namespace std;

namespace ea {

const string Population::GENERATION_COUNTER = "generation";
const string Population::EVALUATION_COUNTER = "evaluation";

#define RUN_HOOK(NAME)\
	for (HookPtr hook : mHooks)\
		hook->NAME();


/**
 * @class Population
 * The class containing all the data of the evolution process.
 * Population object is the target of the evolution process where it stores all
 * the information of Genome, Organism, Pool as well as counters like generation
 * and evaluation number. There are two kinds of data in a Population object:
 * - A list of Pool indexed by an unsigned number. These Pool can be get and set
 * by using GetPool() and SetPool() functions. By default, it contains no Pool.
 * - A list of counter variables indexed by a string. They can be get and set by
 * calling GetCounter() and SetCounter(). Counter variables are atomic, therefore
 * are thread-safe. By default, generation and evaluation counter is set to 0.
 *
 * Population object can be created directly or indirectly.
 * To start an evolution process, first an implementation of Strategy is needed
 * (e.g. DefaultStrategy). Then, call the function Strategy::Evolve().
 * If called with a Population object, the Strategy will manipulate the given Population.
 * Otherwise, it will create a new Population, initialize it, then apply the algorithm.
 *
 * Population is Storable,
 * which means the whole Population can be serialized to and deserialized from binary format.
 */

/**
 * Create a Population containing nothing.
 * The two counters for generation and evaluation number will be set to 0 by default.
 */
Population::Population() : mPools(), mCounters() {
	SetCounter(GENERATION_COUNTER, 0);
	SetCounter(EVALUATION_COUNTER, 0);
}

Population::~Population() {
}

/**
 * Get the value of a counter variable.
 * @param pId The string used to identify the counter.
 * @return The value of the counter.
 */
ullong Population::GetCounter(string pId) const {
	try {
		return mCounters.at(pId);
	} catch (out_of_range& e) {
		throw EA_EXCEPTION(EAException, ID_DOES_NOT_EXIST,
				"Counter ID for Population does not existed. (received \"" + pId + "\")",
				current_exception());
	}
}

/**
 * Set the value of a counter variable.
 * @param pId The string used to identify the counter.
 * @param pValue The new value of the counter.
 */
void Population::SetCounter(string pId, ullong pValue) {
	mCounters[pId] = pValue;
}
/**
 * Increase the value of a counter variable by 1.
 * @param pId The string used to identify the counter.
 * @return The increased value of the counter.
 */
ullong Population::IncreaseCounter(string pId) {
	return ++mCounters[pId];
}

/**
 * Get all counter variables in the Population.
 * @return The map of counters.
 */
const Population::CounterMap& Population::GetCounters() {
	return mCounters;
}

/**
 * Get the current generation number.
 * This is equivalent with calling GetCounter() using GENERATION_COUNTER as the index.
 * @return The current generation number.
 */
ullong Population::GetGeneration() const {
	return GetCounter(GENERATION_COUNTER);
}

/**
 * Get the current evaluation number.
 * This is equivalent with calling GetCounter() using EVALUATION_COUNTER as the index.
 * @return The current evaluation number.
 */
ullong Population::GetEvaluation() const {
	return GetCounter(EVALUATION_COUNTER);
}

/**
 * Increase the generation number by 1.
 * This is equivalent with calling IncreaseCounter() using GENERATION_COUNTER as the index.
 * @return The increased generation number.
 */
ullong Population::IncreaseGeneration() {
	return IncreaseCounter(GENERATION_COUNTER);
}
/**
 * Increase the evaluation number by 1.
 * This is equivalent with calling IncreaseCounter() using EVALUATION_COUNTER as the index.
 * @return The increased evaluation number.
 */
ullong Population::IncreaseEvaluation() {
	return IncreaseCounter(EVALUATION_COUNTER);
}

/**
 * Get a Pool stored in the Population.
 * @param pIndex The index of the Pool.
 * @return The wanted Pool. If there's no Pool at that index, nullptr is retuned.
 */
PoolPtr Population::GetPool(uint pIndex) const {
	try {
		return mPools.at(pIndex);
	} catch (out_of_range& e) {
		return nullptr;
	}
}
/**
 * Get a GenomePool stored in the Population.
 * This function is similar to GetPool() with automatic casting to GenomePool.
 * @param pIndex The index of the Pool.
 * @return The wanted GenomePool. If there's no Pool at that index, nullptr is retuned.
 * If the Pool exists but is not a GenomePool, exception will be thrown.
 */
GenomePoolPtr Population::GetGenomePool(uint pIndex) const {
	auto pool = GetPool(pIndex);
	if (!pool)
		return nullptr;

	auto casted = dynamic_pointer_cast<GenomePool>(GetPool(pIndex));
	if (!casted)
		throw EA_EXCEPTION(EAException, POOL_BAD_CAST,
				"Pool #" + to_string(pIndex) + " is not a GenomePool.");

	return casted;
}
/**
 * Get a OrganismPool stored in the Population.
 * This function is similar to GetPool() with automatic casting to OrganismPool.
 * @param pIndex The index of the Pool.
 * @return The wanted OrganismPool. If there's no Pool at that index, nullptr is retuned.
 * If the Pool exists but is not a OrganismPool, exception will be thrown.
 */
OrganismPoolPtr Population::GetOrganismPool(uint pIndex) const {
	auto pool = GetPool(pIndex);
	if (!pool)
		return nullptr;

	auto casted = dynamic_pointer_cast<OrganismPool>(GetPool(pIndex));
	if (!casted)
		throw EA_EXCEPTION(EAException, POOL_BAD_CAST,
				"Pool #" + to_string(pIndex) + " is not an OrganismPool.");

	return casted;
}

/**
 * Store the given Pool into the Population.
 * @param pIndex The index of the Pool.
 * @param pPool The new Pool object to be set.
 */
void Population::SetPool(uint pIndex, const PoolPtr& pPool) {
	mPools[pIndex] = pPool;
}

/**
 * @fn void Population::SetPool(uint pIndex, const Ptr<P>& pPool)
 * Store the given Pool of type P into the Population.
 * This function is similar to SetPool() with automatic casting to type P.
 * @tparam P Must be a child of Pool.
 * @param pIndex The index of the Pool.
 * @param pPool The new Pool object of type P to be set.
 */

void Population::DoSerialize(ostream& pStream) const {
	Write(pStream, mCounters);
	Write(pStream, mPools);
}

void Population::DoDeserialize(istream& pStream) {
	Read(pStream, mCounters);
	Read(pStream, mPools);
}

} /* namespace ea */
