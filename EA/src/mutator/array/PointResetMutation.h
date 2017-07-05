/*
 * PointResetMutation.h
 *
 *  Created on: Mar 18, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include <ctime>

#include "../../EA/Type/Array.h"
#include "../../genome/ArrayGenome.h"
#include "../TypedMutator.h"

namespace ea {

using namespace std;

/**
 * Implementation of **Point reset mutation** operator for ArrayGenome.
 * This Mutator is only applicable on ArrayGenome of the same type of genes T.
 *
 * When apply, each gene in the ArrayGenome will have a chance to be reset or flipped
 * (i.e. take a new random value).
 * Users can configure whether the new gene is allowed to have the same value with the old one.
 *
 * @note
 * The name used for PointResetMutation of gene type bool  in NameService
 * is \ref FlipBitMutation instead of <code>BoolPointResetMutation</code>
 *
 * @tparam T Type of genes.
 *
 * @method
 * Iterate through the array of genes, then apply probability test.
 *
 * @time
 * O(N)
 *
 * @name{FlipBitMutation, IntPointResetMutation, DoublePointResetMutation}
 *
 * @eaml
 * @attr{rate, double - Required - The probability that each gene will be reset with.}
 * @attr{randomizer, Randomizer - Required (Optional for @ref FlipBitMutation) - The random value generator.}
 * @attr{allow-dupplicate, bool - Optional (Not exist for @ref FlipBitMutation) -
 * Whether the new gene is allowed to have the same value with the old one.}
 * @endeaml
 */
template<class T>
class PointResetMutation: public TypedMutator<ArrayGenome<T>> {
private:
	double mRate;
	RandomizerPtr<T> mRandomizer;
	bool mAllowDuplicate;

protected:
	virtual ArrayGenomePtr<T> DoApply(const ArrayGenomePtr<T>& pTarget)
			override;

public:
	EA_TYPEINFO_CUSTOM_DECL

	/**
	 * Create a PointResetMutation with the given rate and Randomizer.
	 * The Randomizer must be provided except for \ref FlipBitMutation. Otherwise run-time error will be thrown.
	 * @param pRate The probability that each gene will be reset with.
	 * @param pRandomizer A Randomizer to generate random value.
	 * @param pAllowDuplicate Whether the new gene is allowed to have the same value with the old one.
	 */
	inline PointResetMutation(double pRate, RandomizerPtr<T> pRandomizer = nullptr,
			bool pAllowDuplicate = false) :
	mRate(pRate), mRandomizer(pRandomizer), mAllowDuplicate(
			pAllowDuplicate) {
		if (!pRandomizer)
		throw EA_EXCEPTION(EAException, ARGUMENT_REQUIRED,
				"A Randomizer is required for PointResetMutation");
	}
	inline virtual ~PointResetMutation() {
	}

	/**
	 * Whether the new gene is allowed to have the same value with the old one.
	 * @return true if duplication is allowed.
	 */
	inline bool IsDuplicateAllowed() const {
		return mAllowDuplicate;
	}
	/**
	 * Get the probability that each gene will be reset with.
	 * @return The reset probability.
	 */
	inline double GetRate() const {
		return mRate;
	}
};

#ifndef DOXYGEN_IGNORE
template<>
inline PointResetMutation<bool>::PointResetMutation(double pRate,
		RandomizerPtr<bool> pRandomizer, bool pAllowDuplicate) :
		mRate(pRate), mRandomizer(pRandomizer), mAllowDuplicate(pAllowDuplicate) {
	if (!mRandomizer)
		mRandomizer = make_shared<BoolRandomizer>();
}

EA_TYPEINFO_SPECIALIZED_INLINE(PointResetMutation, bool){
	return *ea::TypeInfo("FlipBitMutation")
		.Add("randomizer", &PointResetMutation::mRandomizer)
		->SetConstructor<FlipBitMutation, double>("rate");
}
EA_TYPEINFO_SPECIALIZED_INLINE(PointResetMutation, int){
	return *ea::TypeInfo("IntPointResetMutation")
		.Add("allow-dupplicate", &IntPointResetMutation::mAllowDuplicate)
		->SetConstructor<IntPointResetMutation, double, IntRandomizerPtr>("rate", "randomizer");
	}
EA_TYPEINFO_SPECIALIZED_INLINE(PointResetMutation, double){
	return *ea::TypeInfo("DoublePointResetMutation")
		.Add("allow-dupplicate", &DoublePointResetMutation::mAllowDuplicate)
		->SetConstructor<DoublePointResetMutation, double, DoubleRandomizerPtr>("rate", "randomizer");
}
#endif

/**
 * Implementation of TypedMutator::DoApply(const Ptr<T>&)
 */
template<class T>
ArrayGenomePtr<T> PointResetMutation<T>::DoApply(
		const ArrayGenomePtr<T>& pTarget) {
	auto genome = pTarget->Clone();

	uniform_real_distribution<double> dist(0.0, 1.0);
	vector<T>& genes = genome->GetGenes();
	for (uint i = 0; i < genes.size(); i++)
		if (mRate == 1.0 || dist(Random::generator) < mRate) {
			if (mAllowDuplicate)
				genes[i] = mRandomizer->Get();
			else {
				T oldGene = genes[i];
				do
					genes[i] = mRandomizer->Get();
				while (genes[i] == oldGene);
			}
		}
	return genome;
}

} /* namespace ea */

