/*
 * MetaMutator.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"

#include "MetaMutator.h"
#include "../EA/Core.h"
#include "../EA/Type/Core.h"

namespace ea {

EA_TYPEINFO_CUSTOM_IMPL(MetaMutator) {
	return *ea::TypeInfo("MetaMutator")
		.SetConstructor<MetaMutator, MutatorPtr, double>("mutator", "rate");
}

/**
 * @class MetaMutator
 * Wrapper of Mutator and its corresponding mutation rate.
 * This class is used to wrap the Mutator with its rate. It provides an interface to manipulate
 * the population at the Pool level (GenomePool) where Mutator is only applicable on individual level (Genome).
 * It does this by executing the wrapped Mutator on each Genome in the GenomePool which
 * passes the probability test.
 *
 * MetaMutator provides multi-threading feature via MultiThreading class.
 * It is a derived class of Variator therefore compatible with Operator, OperatorGroup
 * and SeriesOperatorGroup.
 *
 * @name{MetaMutator}
 *
 * @eaml
 * @attr{mutator,
 * Mutator - Required - The mutation operator}
 * @attr{rate,
 * double - Required - The mutation rate}
 * @endeaml
 */

/**
 * Create a wrapper of the given Mutator and its mutation rate.
 * This function also check the value of the rate. If the rate is not between 0 and 1,
 * an exception will be thrown.
 * @param pMutator The mutation operator.
 * @param pRate The mutation rate.
 */
MetaMutator::MetaMutator(const MutatorPtr& pMutator, double pRate) :
		mMutator(pMutator), mRate(pRate) {
	if (pRate > 1 || pRate < 0)
		throw EA_EXCEPTION(EAException, META_MUTATOR_INVALID_RATE,
				"Mutation rate must be in range [0.0, 1.0].");
}

/**
 * @fn MetaMutator::MetaMutator(Args&&... args)
 * Create a Mutator of type M with given arguments and wrap it in this MetaMutator.
 * The rate is set to default value of 1.
 * @tparam M Must be a child of Mutator.
 * @tparam Args The type of the arguments (auto-deduced)..
 * @param args The arguments to be forwarded to the constructor of type M.
 */

MetaMutator::~MetaMutator() {
}

/**
 * Set the mutation rate of this operator.
 * @param pRate The mutation rate.
 */
void MetaMutator::SetRate(double pRate) {
	mRate = pRate;
}

/**
 * Get the mutation rate of this operator.
 * @return The mutation rate.
 */
double MetaMutator::GetRate() const {
	return mRate;
}

/**
 * Set the Mutator wrapped in this MetaMutator.
 * @param pMutator The Mutator object to be wrapped.
 */
void MetaMutator::SetMutator(const MutatorPtr& pMutator) {
	mMutator = pMutator;
}

/**
 * Get the Mutator wrapped in this MetaMutator.
 * @return The wrapped Mutator object.
 */
const MutatorPtr& MetaMutator::GetMutator() const {
	return mMutator;
}

GenomePoolPtr MetaMutator::DoVariate(const GenomePoolPtr& pPool) {
	GenomePoolPtr inputPool = pPool, outputPool = make_shared<GenomePool>(inputPool->size());

	try {
		mMutator->SetSession(GetSession());
	} catch (EAException& e) {
		mMutator->SetSession(nullptr);
	}

	MultiThreading::For(0, inputPool->size(), [&] (int i) {
		if (mRate == 1.0 || Random::Rate() < mRate)
			(*outputPool)[i] = mMutator->Apply((*inputPool)[i]);
		else
			(*outputPool)[i] = (*inputPool)[i];
	});

	return outputPool;
}

/**
 * Alias of SetRate().
 * @param pRate The mutation rate.
 */
void MetaMutator::Rate(double pRate) {
	SetRate(pRate);
}

} /* namespace ea */
