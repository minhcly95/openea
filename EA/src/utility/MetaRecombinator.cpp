/*
 * MetaRecombinator.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "MetaRecombinator.h"
#include "../EA/Core.h"
#include "../EA/Selector.h"

#include "../pch.h"

namespace ea {

EA_TYPEINFO_CUSTOM_IMPL(MetaRecombinator) {
	return *ea::TypeInfo("MetaRecombinator")
		.SetConstructor<MetaRecombinator, RecombinatorPtr, ResizableSelectorPtr, double>
				("recombinator", "selector", "ratio");
}

/**
 * @class MetaRecombinator
 * Wrapper of Recombinator with its corresponding parent Selector and offspring ratio.
 * This class is used mainly in DefaultStrategy
 * to wrap the Recombinator with its parent Selector and offspring ratio.
 * It provides an interface to manipulate the population at the Pool level (GenomePool)
 * where Recombinator is only applicable on individual level (Genome).
 * It does so by executing the wrapped Recombinator multiple time to generate mutiple Genome
 * and aggregate them all into a new GenomePool.
 *
 * MetaRecombinator provides multi-threading feature via MultiThreading class.
 * It is compatible with Operator and OperatorGroup by taking
 * a parent OrganismPool as input and produces an offspring GenomePool.
 *
 * @name{MetaRecombinator}
 *
 * @eaml
 * @attr{recombinator,
 * Recombinator - Required - The recombination operator}
 * @attr{selector,
 * ResizableSelector - Required - The parent selection operator}
 * @attr{ratio,
 * double - Required - The offspring ratio (the ratio between the number of offspring and the population size)}
 * @endeaml
 */

/**
 * Create a wrapper of the given Recombinator, its parent Selector and its offspring ratio.
 * This function also check the value of the ratio. If the ratio is negative,
 * an exception will be thrown.
 * @param pRecombinator The recombination operator.
 * @param pSelector The parent selection operator.
 * @param pRatio The offspring ratio (the ratio between the number of offspring and the population size).
 */
MetaRecombinator::MetaRecombinator(const RecombinatorPtr& pRecombinator,
		const ResizableSelectorPtr& pSelector, double pRatio) :
		mRecombinator(pRecombinator), mSelector(pSelector), mRatio(pRatio) {
	if (pRatio < 0)
		throw EA_EXCEPTION(EAException, META_RECOMBINATOR_INVALID_RATIO,
				"Recombination ratio must be non-negative.");
}

/**
 * Create a wrapper for the given Recombinator using default settings.
 * The parent selection method is set to UniformSelection and the default ratio is 1.
 * @param pRecombinator The recombination operator.
 */
MetaRecombinator::MetaRecombinator(const RecombinatorPtr& pRecombinator) :
		mRecombinator(pRecombinator), mRatio(1) {
	mSelector = make_shared<UniformSelection>();
}

MetaRecombinator::~MetaRecombinator() {
}

/**
 * Get the offspring ratio of this operator.
 * @return The offspring ratio.
 */
double MetaRecombinator::GetRatio() const {
	return mRatio;
}
/**
 * Get the parent selection operator of this MetaRecombinator.
 * @return The parent Selector.
 */
const ResizableSelectorPtr& MetaRecombinator::GetSelector() const {
	return mSelector;
}
/**
 * Get the Recombinator wrapped in this MetaRecombinator.
 * @return The wrapped Recombinator object.
 */
const RecombinatorPtr& MetaRecombinator::GetRecombinator() const {
	return mRecombinator;
}

/**
 * Set the offspring ratio of this operator.
 * @param pRatio The offspring ratio.
 */
void MetaRecombinator::SetRatio(double pRatio) {
	mRatio = pRatio;
}
/**
 * Set the Recombinator wrapped in this MetaRecombinator.
 * @param pRecombinator The recombination operator to be wrapped.
 */
void MetaRecombinator::SetRecombinator(const RecombinatorPtr& pRecombinator) {
	mRecombinator = pRecombinator;
}
/**
 * Set the parent selection operator of this MetaRecombinator.
 * @return pSelector The parent Selector.
 */
void MetaRecombinator::SetSelector(const ResizableSelectorPtr& pSelector) {
	mSelector = pSelector;
}

/**
 * @fn MetaRecombinator::MetaRecombinator(Args&&... args)
 * Create a Recombinator of type R with given arguments and wrap it in this MetaRecombinator.
 * The parent selection method is set to UniformSelection and the default ratio is 1.
 * @tparam R Must be a child of Recombinator.
 * @tparam Args The type of the arguments (auto-deduced)..
 * @param args The arguments to be forwarded to the constructor of type R.
 */

/**
 * @fn MetaRecombinator::Parent(Args&&... args)
 * Create a Selector of type S with given arguments and and set it as the parent selection method.
 * @tparam S Must be a child of ResizableSelector.
 * @tparam Args The type of the arguments (auto-deduced)..
 * @param args The arguments to be forwarded to the constructor of type S.
 * @return A pointer to this to cascade the calling of Ratio().
 */

/**
 * Alias of SetRatio().
 * @param pRatio The offspring ratio.
 */
void MetaRecombinator::Ratio(double pRatio) {
	SetRatio(pRatio);
}

/**
 * Operation execution function. This function will be called when Operator::operator()() is invoked.
 * @param pSession The current running Session.
 * @param pPool The input OrganismPool.
 * @return The spawned GenomePool.
 */
GenomePoolPtr MetaRecombinator::operator ()(const SessionPtr& pSession, OrganismPoolPtr pPool) const {
	int numOffspring = ceil(mRatio * pPool->size());
	GenomePoolPtr outputPool = make_shared<GenomePool>(numOffspring);

	mRecombinator->SetSession(pSession);

	MultiThreading::For(0, numOffspring, [&] (int i) {
		OrganismPoolPtr parentPool = mSelector->Select(pPool, mRecombinator->GetParentCount());
		GenomePoolPtr parentGenome = parentPool->Extract();

		(*outputPool)[i] = mRecombinator->Combine(*parentGenome);
	});

	return outputPool;
}

} /* namespace ea */
