/*
 * RandomArrayInitializer.h
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Array.h"
#include "../core/interface/Initializer.h"
#include "../core/pool/GenomePool.h"

namespace ea {

/**
 * A random Initializer for ArrayGenome.
 * This Initializer will create Genome of type ArrayGenome with random content for a given array length.
 * The Initializer accepts a Randomizer to generate random content (it doesn't require for \ref BoolRandomArrayInitializer).
 *
 * @tparam T Type of genes.
 * @name{BoolRandomArrayInitializer, IntRandomArrayInitializer, DoubleRandomArrayInitializer}
 *
 * @eaml
 * @attr{length, uint - Required - The length of generated ArrayGenome.}
 * @attr{randomizer, Randomizer - Required (Optional for @ref BoolRandomArrayInitializer) - The random value generator.}
 * @endeaml
 */
template<class T>
class RandomArrayInitializer: public Initializer {
public:
	EA_TYPEINFO_CUSTOM_DECL

	RandomArrayInitializer(uint pLength, RandomizerPtr<T> pRandomizer = nullptr);

	inline virtual ~RandomArrayInitializer() {
	}

private:
	uint mLength;
	RandomizerPtr<T> mRandomizer;

protected:
	virtual GenomePoolPtr DoInitialize(uint pSize) override;
};

/**
 * Create a RandomArrayInitializer with the given array length and Randomizer.
 * The Randomizer must be provided except for \ref BoolRandomArrayInitializer. Otherwise run-time error will be thrown.
 * @param pLength The length of generated ArrayGenome.
 * @param pRandomizer A Randomizer to generate random value.
 */
template<class T>
inline RandomArrayInitializer<T>::RandomArrayInitializer(uint pLength,
		RandomizerPtr<T> pRandomizer) :
		mLength(pLength), mRandomizer(pRandomizer) {
	if (!pRandomizer)
		throw EA_EXCEPTION(EAException, ARGUMENT_REQUIRED,
				"A Randomizer is required for RandomArrayInitializer");
}

template<class T>
GenomePoolPtr RandomArrayInitializer<T>::DoInitialize(uint pSize) {
	GenomePoolPtr pool = make_shared<GenomePool>();
	for (uint i = 0; i < pSize; i++) {
		vector<T> genes;
		for (uint j = 0; j < mLength; j++)
			genes.push_back(mRandomizer->Get());

		pool->push_back(make_shared_base(Genome, ArrayGenome<T>, genes));
	}
	return pool;
}

#ifndef DOXYGEN_IGNORE
template<>
inline RandomArrayInitializer<bool>::RandomArrayInitializer(uint pLength,
		BoolRandomizerPtr pRandomizer) :
		mLength(pLength), mRandomizer(pRandomizer) {
	if (!mRandomizer)
		mRandomizer = make_shared<BoolRandomizer>();
}

EA_TYPEINFO_SPECIALIZED_INLINE(RandomArrayInitializer, bool) {
	return *ea::TypeInfo("BoolRandomArrayInitializer")
		.Add("randomizer", &BoolRandomArrayInitializer::mRandomizer)
		->SetConstructor<BoolRandomArrayInitializer, uint>("length");
}

EA_TYPEINFO_SPECIALIZED_INLINE(RandomArrayInitializer, int) {
	return *ea::TypeInfo("IntRandomArrayInitializer")
		.SetConstructor<IntRandomArrayInitializer, uint, IntRandomizerPtr>("length", "randomizer");
}

EA_TYPEINFO_SPECIALIZED_INLINE(RandomArrayInitializer, double) {
	return *ea::TypeInfo("DoubleRandomArrayInitializer")
		.SetConstructor<DoubleRandomArrayInitializer, uint, DoubleRandomizerPtr>("length", "randomizer");
}
#endif

} /* namespace ea */


