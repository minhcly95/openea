/*
 * TypedRecombinator.h
 *
 *  Created on: Apr 16, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../core/interface/Recombinator.h"
#include "../core/interface/Genome.h"

namespace ea {

/**
 * A Recombinator which requires the input and output Genome of type T instead of Genome.
 * It is usually the case that users know which type of Genome they are processing. Therefore,
 * to avoid putting too much responsibility of type-casting on users, this class will automatically
 * cast the input and output Genome of Recombinator::DoCombine(vector<GenomePtr>&) into genomes of type T.
 * Users will override the new function with the same name \ref DoCombine(vector<Ptr<T>>&) instead of
 * the old one.
 *
 * @tparam T Must be a child of Genome.
 */
template<class T>
class TypedRecombinator: public Recombinator {
public:
	inline virtual ~TypedRecombinator() {
	}

protected:
	/**
	 * Implementation of the recombination operator with input and output of type T.
	 * This is the new implementation point of the recombination operator. The input will be auto-casted
	 * to type T and the function only requires a vector of \ref Ptr<T>.
	 * Moreover, users only need to return an output of type T also, and it will be converted back into Genome
	 * automatically to be compatible with Recombinator.
	 *
	 * The implementation must still follow the rule described in Recombinator::DoCombine(vector<GenomePtr>&).
	 *
	 * @param pParents The list of parents of type T which need to be recombined.
	 * @return A completely new genome of type T object which is the combined result of the input parents.
	 * @see Recombinator::DoCombine(vector<GenomePtr>&)
	 */
	virtual Ptr<T> DoCombine(vector<Ptr<T>>& pParents) = 0;
	/**
	 * Specialized implementation to be compatible with Recombinator.
	 * This function will cast the input, call DoCombine(vector<Ptr<T>>&) and convert back the output internally.
	 * This function also provides an informative error message if bad cast happened.
	 * @see Recombinator::DoCombine(vector<GenomePtr>&)
	 */
	virtual GenomePtr DoCombine(vector<GenomePtr>& pParents) override final {
		vector<Ptr<T>> castedParents(pParents.size());
		transform(pParents.begin(), pParents.end(), castedParents.begin(),
				[this](GenomePtr parent) {
					Ptr<T> casted = dynamic_pointer_cast<T>(parent);
					if (!casted)
					throw EA_EXCEPTION(EAException, TYPED_OPERATOR_BAD_CAST,
							GetTypeNameSafe("This TypedRecombinator") + " requires " + T().GetTypeName()
							+ " as input.");
					return casted;
				});
		return static_pointer_cast<Genome>(DoCombine(castedParents));
	}
};

} /* namespace ea */

