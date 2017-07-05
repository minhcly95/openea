/*
 * Mutator.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../../pch.h"
#include "Mutator.h"

namespace ea {

/**
 * @class Mutator
 * The interface for mutation operator.
 * This class provides the interface to implement custom mutation operator.
 * Users should override DoApply() in child class to implement their customized code.
 *
 * Mutator is a specialization of Recombinator when GetParentCount() returns 1. Therefore,
 * users can also use any Mutator as a Recombinator.
 *
 * Mutator is Operator compatible via Recombinator interface.
 * It will produce an output GenomePool with size equal to the input GenomePool size.
 * Although Mutator is compatible with Operator, if some mutation rate is considered,
 * please take a look at MetaMutator.
 *
 * @see DoApply()
 */

/**
 * @fn GenomePtr Mutator::DoApply(const GenomePtr& pTarget)
 * Implementation of the mutation operator.
 * The function requires a single Genome as the mutation target and produces a mutated Genome as the output.
 *
 * Genomes in OpenEA library are passed by reference, therefore if users want to modify an existing Genome,
 * it should be cloned instead of modified directly. Otherwise, any Genome which shares the same reference
 * will be modified which, in most case, is not the wanted behavior.
 *
 * @param pTarget The target Genome to be mutated.
 * @return A new Genome object which is the mutated version of pTarget.
 * @see Recombinator::DoCombine()
 */

/**
 * @fn uint Mutator::GetParentCount()
 * Specialized implementation to be compatible with Recombinator.
 * This function will always return 1 because a Mutator requires only a parent to spawn offspring.
 * @return Always 1.
 */

/**
 * Specialized implementation to be compatible with Recombinator.
 * This function will call Apply() internally.
 * @see Recombinator::DoCombine()
 */
GenomePtr Mutator::DoCombine(vector<GenomePtr>& pParents) {
	return Apply(*pParents.begin());
}

/**
 * Public method of DoApply().
 * This function handles the exception and then call DoApply() internally.
 * @param pTarget The target Genome to be mutated.
 * @return A new Genome object which is the mutated version of pTarget.
 * @see DoApply()
 */
GenomePtr Mutator::Apply(const GenomePtr& pTarget) {
	try {
		return DoApply(pTarget);
	} catch (exception& e) {
		throw EA_EXCEPTION(EAException, MUTATOR_ERROR,
				"Exception caught when executing Mutator.", current_exception());
	}
}

} /* namespace ea */
