/*
 * TypedMutator.h
 *
 *  Created on: Apr 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../core/interface/Mutator.h"
#include "../core/interface/Genome.h"

namespace ea {

/**
 * A Mutator which requires the input and output Genome of type T instead of Genome.
 * It is usually the case that users know which type of Genome they are processing. Therefore,
 * to avoid putting too much responsibility of type-casting on users, this class will automatically
 * cast the input and output Genome of Mutator::DoApply(const GenomePtr&) into a genome type of type T.
 * Users will override the new function with the same name \ref DoApply(const Ptr<T>&) instead of
 * the old one.
 *
 * @tparam T Must be a child of Genome.
 */
template<class T>
class TypedMutator: public Mutator {
public:
	inline virtual ~TypedMutator() {
	}

protected:
	/**
	 * Implementation of the mutation operator with input and output of type T.
	 * This is the new implementation point of the mutation operator. The input will be auto-casted
	 * to type T and the function only requires an input of type<code> const \ref Ptr<T>&</code>.
	 * Moreover, users only need to return an output of type T also, and it will be converted back into Genome
	 * automatically to be compatible with Mutator.
	 *
	 * The implementation must still follow the rule described in Mutator::DoApply(const GenomePtr&).
	 *
	 * @param pTarget The target genome of type T to be mutated.
	 * @return A new genome of type T which is the mutated version of pTarget.
	 * @see Mutator::DoApply(const GenomePtr&)
	 */
	virtual Ptr<T> DoApply(const Ptr<T>& pTarget) = 0;
	/**
	 * Specialized implementation to be compatible with Mutator.
	 * This function will cast the input, call DoApply(const Ptr<T>&) and convert back the output internally.
	 * This function also provides an informative error message if bad cast happened.
	 * @see Mutator::DoApply(const GenomePtr&)
	 */
	virtual GenomePtr DoApply(const GenomePtr& pTarget) override final {
		Ptr<T> casted = dynamic_pointer_cast<T>(pTarget);
		if (!casted)
			throw EA_EXCEPTION(EAException, TYPED_OPERATOR_BAD_CAST,
					GetTypeNameSafe("This TypedMutator") + " requires " + T().GetTypeName()
							+ " as input.");
		return static_pointer_cast<Genome>(DoApply(casted));
	}
};

} /* namespace ea */




